void TMVA_Regression() {
   
TMVA::Tools::Instance();

auto inputFile = TFile::Open("inputdata_regression.root");
auto outputFile = TFile::Open("TMVA_RegressionOutput.root", "RECREATE");

TMVA::Factory factory("TMVARegression", outputFile,
                      "!V:!Silent:Color:!DrawProgressBar:AnalysisType=Regression" ); 

TMVA::DataLoader loader("dataset"); 

// Add the feature variables, names reference branches in inputFile ttree
loader.AddVariable("var1");
loader.AddVariable("var2");
loader.AddVariable("var3");
loader.AddVariable("var4");
loader.AddVariable("var5 := var1-var3"); // create new features
loader.AddVariable("var6 := var1+var2");

loader.AddTarget( "target := var2+var3" ); // define the target for the regression


TTree *tree;
inputFile->GetObject("Sig", tree);

TCut mycuts = ""; // e.g. TCut mycuts = "abs(var1)<0.5";

loader.AddRegressionTree(tree, 1.0);   // link the TTree to the loader, weight for each event  = 1
loader.PrepareTrainingAndTestTree(mycuts,
                                   "nTrain_Regression=1000:nTest_Regression=1000:SplitMode=Random:NormMode=NumEvents:!V" );

// Boosted Decision Trees 
factory.BookMethod(&loader,TMVA::Types::kBDT, "BDTG_AD",
                   TString("!H:!V:NTrees=64::BoostType=Grad:Shrinkage=0.3:nCuts=20:MaxDepth=4:")+
                   TString("RegressionLossFunctionBDTG=AbsoluteDeviation"));

// Boosted Decision Trees 
factory.BookMethod(&loader,TMVA::Types::kBDT, "BDTG_LS",
                   TString("!H:!V:NTrees=64::BoostType=Grad:Shrinkage=0.3:nCuts=20:MaxDepth=4:")+
                   TString("RegressionLossFunctionBDTG=LeastSquares"));

// Boosted Decision Trees                                                                              
factory.BookMethod(&loader,TMVA::Types::kBDT, "BDTG_HU",
                   TString("!H:!V:NTrees=64::BoostType=Grad:Shrinkage=0.3:nCuts=20:MaxDepth=4:")+
                   TString("RegressionLossFunctionBDTG=Huber"));

factory.TrainAllMethods();

factory.TestAllMethods();    

factory.EvaluateAllMethods();

//%jsroot on
outputFile->Close();
auto resultsFile = TFile::Open("TMVA_RegressionOutput.root");
auto resultsTree = (TTree*) resultsFile->Get("dataset/TestTree"); 
// BDTG-AD is the predicted value, target is the true value
auto h1 = new TH1D("h1","BDTG-target",100,1,0);
resultsTree->Draw("BDTG_AD-target >> h1"); 
auto h2 = (TH1*) h1->Clone("h2");
auto h3 = (TH1*) h1->Clone("h3");
resultsTree->Draw("BDTG_LS-target >> h2"); 
resultsTree->Draw("BDTG_AD-target >> h3"); 
h1->SetLineColor(kBlue); h1->Draw();
h2->SetLineColor(kRed); h2->Draw("SAME");
h3->Draw("SAME");
gPad->Draw();

}
