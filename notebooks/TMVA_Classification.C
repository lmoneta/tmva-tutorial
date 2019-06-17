void TMVA_Classification() {
   
TMVA::Tools::Instance();


auto outputFile = TFile::Open("TMVA_ClassificationOutput.root", "RECREATE");

TMVA::Factory factory("TMVAClassification", outputFile,
                      "!V:ROC:!Silent:Color:!DrawProgressBar:AnalysisType=Classification" ); 

TString inputFileName = "http://root.cern.ch/files/tmva_class_example.root";
//TString inputFileName = "tmva_class_example.root";

auto inputFile = TFile::Open( inputFileName );

// --- Register the training and test trees

TTree *signalTree     = (TTree*)inputFile->Get("TreeS");
TTree *backgroundTree = (TTree*)inputFile->Get("TreeB");


TMVA::DataLoader * loader = new TMVA::DataLoader("dataset");

// global event weights per tree (see below for setting event-wise weights)
Double_t signalWeight     = 1.0;
Double_t backgroundWeight = 1.0;
   
// You can add an arbitrary number of signal or background trees
loader->AddSignalTree    ( signalTree,     signalWeight     );
loader->AddBackgroundTree( backgroundTree, backgroundWeight );


signalTree->Print();

loader->AddVariable( "myvar1 := var1+var2", 'F' );
loader->AddVariable( "myvar2 := var1-var2", "Expression 2", "", 'F' );
loader->AddVariable( "var3",                "Variable 3", "units", 'F' );
loader->AddVariable( "var4",                "Variable 4", "units", 'F' );

// You can add so-called "Spectator variables", which are not used in the MVA training,
// but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the
// input variables, the response values of all trained MVAs, and the spectator variables
loader->AddSpectator( "spec1 := var1*2",  "Spectator 1", "units", 'F' );
loader->AddSpectator( "spec2 := var1*3",  "Spectator 2", "units", 'F' );


//  We can define also the event weights

// Set individual event weights (the variables must exist in the original TTree)
//    for signal    : factory->SetSignalWeightExpression    ("weight1*weight2");
//    for background: factory->SetBackgroundWeightExpression("weight1*weight2");
loader->SetBackgroundWeightExpression( "weight" );


// Apply additional cuts on the signal and background samples (can be different)
TCut mycuts = ""; // for example: TCut mycuts = "abs(var1)<0.5 && abs(var2-0.5)<1";
TCut mycutb = ""; // for example: TCut mycutb = "abs(var1)<0.5";

// Tell the factory how to use the training and testing events
//
// If no numbers of events are given, half of the events in the tree are used 
// for training, and the other half for testing:
//    loader->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );
// To also specify the number of testing events, use:
//    loader->PrepareTrainingAndTestTree( mycut,
//                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );
loader->PrepareTrainingAndTestTree( mycuts, mycutb,
                                    "nTrain_Signal=4000:nTrain_Background=2000:SplitMode=Random:NormMode=NumEvents:!V" );


// Likelihood ("naive Bayes estimator")
factory.BookMethod(loader, TMVA::Types::kLikelihood, "Likelihood",
                           "H:!V:TransformOutput:PDFInterpol=Spline2:NSmoothSig[0]=20:NSmoothBkg[0]=20:NSmoothBkg[1]=10:NSmooth=1:NAvEvtPerBin=50" );

// Use a kernel density estimator to approximate the PDFs
factory.BookMethod(loader, TMVA::Types::kLikelihood, "LikelihoodKDE",
                           "!H:!V:!TransformOutput:VarTransform=D:PDFInterpol=KDE:KDEtype=Gauss:KDEiter=Adaptive:KDEFineFactor=0.3:KDEborder=None:NAvEvtPerBin=50" ); 


// Fisher discriminant (same as LD)
factory.BookMethod(loader, TMVA::Types::kFisher, "Fisher", "H:!V:Fisher:VarTransform=None:CreateMVAPdfs:PDFInterpolMVAPdf=Spline2:NbinsMVAPdf=50:NsmoothMVAPdf=10" );

//Boosted Decision Trees
factory.BookMethod(loader,TMVA::Types::kBDT, "BDT",
                   "!V:NTrees=200:MinNodeSize=2.5%:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );

//Multi-Layer Perceptron (Neural Network)
factory.BookMethod(loader, TMVA::Types::kMLP, "MLP",
                   "!H:!V:NeuronType=tanh:VarTransform=N:NCycles=100:HiddenLayers=N+5:TestRate=5:!UseRegulator" );

factory.TrainAllMethods();

factory.TestAllMethods();  

factory.EvaluateAllMethods();  

//We enable JavaScript visualisation for the plots
//%jsroot on

auto c1 = factory.GetROCCurve(loader);
c1->Draw();

outputFile->Close();

}
