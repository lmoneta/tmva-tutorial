
/***

    # TMVA Classification Example Using a Convolutional Neural Network

    This is an example of using a CNN in TMVA. We do classification using a toy image data set


    ## Declare Factory

    Create the Factory class. Later you can choose the methods
    whose performance you'd like to investigate. 

    The factory is the major TMVA object you have to interact with. Here is the list of parameters you need to pass

    - The first argument is the base of the name of all the output
    weightfiles in the directory weight/ that will be created with the 
    method parameters 

    - The second argument is the output file for the training results
  
    - The third argument is a string option defining some general configuration for the TMVA session. For example all TMVA output can be suppressed by removing the "!" (not) in front of the "Silent" argument in the option string

**/

void TMVA_CNN_Classification(int type = 15) { 

   bool useKeras = type & 1;
   bool useCNN = type & 2;
   bool useDNN = type & 4;
   bool useBDT = type & 8;

   TMVA::Tools::Instance();

   // do enable MT running
   ROOT::EnableImplicitMT(); 

   // for using Keras
   gSystem->Setenv("KERAS_BACKEND","tensorflow");
// for setting openblas in single thread on SWAN
   gSystem->Setenv("OMP_NUM_THREADS","1"); 
   TMVA::PyMethodBase::PyInitialize();



   auto outputFile = TFile::Open("CNN_ClassificationOutput.root", "RECREATE");

   TMVA::Factory factory("TMVA_CNN_Classification", outputFile,
                         "!V:ROC:!Silent:Color:!DrawProgressBar:AnalysisType=Classification" ); 


/***

    ## Declare DataLoader(s)

    The next step is to declare the DataLoader class that deals with input variables 

    Define the input variables that shall be used for the MVA training
    note that you may also use variable expressions, which can be parsed by TTree::Draw( "expression" )]

    In this case the input data consists of an image of 16x16 pixels. Each single pixel is a branch in a ROOT TTree

**/


   TMVA::DataLoader * loader = new TMVA::DataLoader("dataset");

   int imgSize = 8 * 8; 
   for(auto i = 0; i < imgSize; i++)
   {
      loader->AddVariable(Form("var%d",i),'F');
   }


/***

    ## Setup Dataset(s)

    Define input data file and signal and background trees

**/

   TString inputFileName = "images_data.root";

//TString inputFileName = "tmva_class_example.root";

   auto inputFile = TFile::Open( inputFileName );

// --- Register the training and test trees

   TTree *signalTree     = (TTree*)inputFile->Get("sig_tree");
   TTree *backgroundTree = (TTree*)inputFile->Get("bkg_tree");

// global event weights per tree (see below for setting event-wise weights)
   Double_t signalWeight     = 1.0;
   Double_t backgroundWeight = 1.0;
   
// You can add an arbitrary number of signal or background trees
   loader->AddSignalTree    ( signalTree,     signalWeight     );
   loader->AddBackgroundTree( backgroundTree, backgroundWeight );


// Set individual event weights (the variables must exist in the original TTree)
//    for signal    : factory->SetSignalWeightExpression    ("weight1*weight2");
//    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
//loader->SetBackgroundWeightExpression( "weight" );

// Apply additional cuts on the signal and background samples (can be different)
   TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
   TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";

// Tell the factory how to use the training and testing events
//
// If no numbers of events are given, half of the events in the tree are used 
// for training, and the other half for testing:
//    loader->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
// To also specify the number of testing events, use:

   loader->PrepareTrainingAndTestTree( mycuts, mycutb,
                                       "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V:!CalcCorrelations" );



//loader->PrepareTrainingAndTestTree(mycuts, mycutb,
//                                   "nTrain_Signal=5000:nTrain_Background=5000:nTest_Signal=5000:nTest_Background=5000:SplitMode=Random:NormMode=NumEvents:!V" ); 

/***

    DataSetInfo              : [dataset] : Added class "Signal"
    : Add Tree sig_tree of type Signal with 10000 events
    DataSetInfo              : [dataset] : Added class "Background"
    : Add Tree bkg_tree of type Background with 10000 events



**/

   signalTree->Print();

/**

    inspect the tree and plot images using RDataFrame 
 **/

   ROOT::RDataFrame dfs(*signalTree);
   ROOT::RDataFrame dfb(*backgroundTree);

   std::vector<std::string> varNames = dfs.GetColumnNames();

   auto signalTensor = TMVA::Experimental::AsTensor<float>(dfs);
   auto backgroundTensor = TMVA::Experimental::AsTensor<float>(dfb);
   auto shapeS = signalTensor.GetShape();
   auto shapeB = backgroundTensor.GetShape();
   // TMVA::Experimental::RTensor::Slice_t slice = { 0, -1 } ;
   auto firstEventS =  signalTensor.Slice( { {0,1}, {0,64} } );
   auto firstEventB = backgroundTensor.Slice( { {0,1}, {0,64} } );
   std::cout << "RTensor from an RDataFrame: signal shape  " << shapeS[0] << " , " << shapeS[1] << "\n" <<  firstEventS << "\n\n";
   std::cout << "RTensor from an RDataFrame: background shape  " << shapeB[0] << " , " << shapeB[1] << "\n" <<  firstEventB << "\n\n";


   auto hS = new TH2F("hs","Signal image (first event)",8,0,8,8,0,8); 
   auto hB = new TH2F("hb","Background image (first event)",8,0,8,8,0,8);
   auto hSa = new TH2F("hsa","Signal image (average)",8,0,8,8,0,8); 
   auto hBa = new TH2F("hba","Background image (average)",8,0,8,8,0,8);
   auto x0S = firstEventS.Reshape( { 8,8} );
   auto x0B = firstEventB.Reshape( { 8,8} );
   for (int i = 0; i < 8; ++i) { 
      for (int j = 0; j < 8; ++j) {
         std::vector<size_t> index(2);
         index[0] = i;
         index[1] = j; 
         hS->Fill(i+0.2, j+0.2, x0S( index ) );
         hB->Fill(i+0.2, j+0.2, x0B( index ) );
      }
   }
   auto c = new TCanvas();
   c->Divide(2,2);
   c->cd(1);
   hS->Draw("COLZ");
   c->cd(2);
   hB->Draw("COLZ");

   // make average image plots
   int nevtS = shapeS[0];
   int nevtB = shapeB[0];
   int nevt = std::min(nevtS, nevtB);
   for (int evt = 0; evt < nevt; ++evt) { 
      for (int i = 0; i < 8; ++i) { 
         for (int j = 0; j < 8; ++j) {
            float * xS = signalTensor.GetData(); 
            float * xB = backgroundTensor.GetData();
            size_t index = evt*64 + i *8 + j;
            hSa->Fill(i+0.2, j+0.2, xS[index]/nevt );
            hBa->Fill(i+0.2, j+0.2, xB[index]/nevt );
         }
      }
   }
   c->cd(3); 
   hSa->Draw("COLZ"); 
   c->cd(4); 
   hBa->Draw("COLZ");

   c->Update(); 


/****
     # Booking Methods

     Here we book the TMVA methods. We book a Likelihood based on KDE, a Fischer discriminant and a BDT

**/

//Boosted Decision Trees
   if (useBDT) { 
      factory.BookMethod(loader,TMVA::Types::kBDT, "BDT",
                         "!V:NTrees=800:MinNodeSize=2.5%:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
   }
/**

   #### Booking Deep Neural Network

   Here we book the DNN of TMVA. See the example TMVA_Higgs_Classification.C for a detailed description of the options

**/


   if (useDNN) { 
    
      TString inputLayoutString = "InputLayout=1|1|64"; 
      TString batchLayoutString= "BatchLayout=1|128|64";
      TString layoutString ("Layout=DENSE|64|TANH,DENSE|64|TANH,DENSE|64|TANH,DENSE|64|TANH,DENSE|1|LINEAR");

      // Training strategies 
      // one can catenate several training strategies 
      TString training1("LearningRate=1e-3,Momentum=0.9,Repetitions=1,"
                        "ConvergenceSteps=10,BatchSize=128,TestRepetitions=1,"
                        "MaxEpochs=20,WeightDecay=1e-4,Regularization=L2,"
                        "Optimizer=ADAM,DropConfig=0.0+0.0+0.0+0.");
  
      TString trainingStrategyString ("TrainingStrategy=");
      trainingStrategyString += training1; // + "|" + training2 + "|" + training3;

      // General Options.

      TString dnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=None:"
                          "WeightInitialization=XAVIER");
      dnnOptions.Append (":"); dnnOptions.Append (inputLayoutString);
      dnnOptions.Append (":"); dnnOptions.Append (batchLayoutString);
      dnnOptions.Append (":"); dnnOptions.Append (layoutString);
      dnnOptions.Append (":"); dnnOptions.Append (trainingStrategyString);

#ifdef R__HAS_TMVAGPU
      dnnOptions += ":Architecture=GPU";
#elif defined( R__HAS_TMVACPU)
      dnnOptions += ":Architecture=GPU";
#else
      dnnOptions += ":Architecture=Standard";
#endif
      
      factory.BookMethod(loader, TMVA::Types::kDL, "DL_DENSE", dnnOptions);

   }

  /***
   ### Book Convolutional Neural Network in TMVA

   For building a CNN one needs to define 

   -  Input Layout :  number of channels (in this case = 1)  | image height | image width
   -  Batch Layout :  batch size | number of channels | image size = (height*width)

   Then one add Convolutional layers and MaxPool layers. 

   -  For Convolutional layer the option string has to be: 
      - CONV | number of units | filter height | filter width | stride height | stride width | padding height | paddig width | activation function 

      - note in this case we are using a filer 3x3 and padding=1 and stride=1 so we get the output dimension of the conv layer equal to the input

    - For the MaxPool layer: 
       - MAXPOOL  | pool height | pool width | stride height | stride width

   The RESHAPE layer is needed to flatten the output before the Dense layer


   Note that to run the CNN is required to have CPU  or GPU support 

  ***/

#ifndef R__HAS_TMVACPU
#ifndef R__HAS_TMVAGPU
      useCNN = false;
#endif
#endif

      if (useCNN) {
      
          TString inputLayoutString("InputLayout=1|8|8");

// Batch Layout
          TString batchLayoutString("BatchLayout=128|1|64");


          TString layoutString("Layout=CONV|10|3|3|1|1|1|1|RELU,CONV|10|3|3|1|1|1|1|RELU,MAXPOOL|2|2|2|2,"
         "RESHAPE|FLAT,DENSE|64|SIGMOID,DENSE|1|LINEAR");



          // Training strategies.
          TString training0("LearningRate=1.e-3,Momentum=0.9,Repetitions=1,"
                            "ConvergenceSteps=10,BatchSize=128,TestRepetitions=1,"
                            "MaxEpochs=30,WeightDecay=1e-4,Regularization=None,"
                            "Optimizer=ADAM,DropConfig=0.0+0.0+0.0+0.0");
 
          TString trainingStrategyString ("TrainingStrategy=");
          trainingStrategyString += training0; 
    
// General Options.
          TString cnnOptions ("!H:V:ErrorStrategy=CROSSENTROPY:VarTransform=None:"
                              "WeightInitialization=XAVIER");

          cnnOptions.Append(":"); cnnOptions.Append(inputLayoutString);
          cnnOptions.Append(":"); cnnOptions.Append(batchLayoutString);
          cnnOptions.Append(":"); cnnOptions.Append(layoutString);
          cnnOptions.Append(":"); cnnOptions.Append(trainingStrategyString);
#ifdef R__HAS_TMVAGPU
          // use GPU if available
          cnnOptions.Append(":Architecture=GPU");
#else
          cnnOptions.Append(":Architecture=CPU");
#endif

          //// New DL (CNN)

          factory.BookMethod(loader, TMVA::Types::kDL, "DL_CNN", cnnOptions);
      }

/**
   ### Book Convolutional Neural Network in Keras using a generated model 

**/

      if (useKeras) { 
      factory.BookMethod(loader, TMVA::Types::kPyKeras, 
         "PyKeras","H:!V:VarTransform=None:FilenameModel=model_cnn.h5:GpuOptions=allow_growth=True:"
         "FilenameTrainedModel=trained_model_cnn.h5:NumEpochs=20:BatchSize=128");
   }


////  ## Train Methods



      factory.TrainAllMethods();

/// ## Test and Evaluate Methods

      factory.TestAllMethods();

      factory.EvaluateAllMethods();    

/// ## Plot ROC Curve

      auto c1 = factory.GetROCCurve(loader);
      c1->Draw();

// close outputfile to save output file
      outputFile->Close();


   }
