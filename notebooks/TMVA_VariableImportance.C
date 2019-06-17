void TMVA_VariableImportance() {
   
auto inputFile = TFile::Open("Higgs_data.root");

inputFile->ls();

TMVA::DataLoader loader("dataset");

// add variables to loader 
loader.AddVariable("m_jj");
loader.AddVariable("m_jjj");
loader.AddVariable("m_lv");
loader.AddVariable("m_jlv");
loader.AddVariable("m_bb");
loader.AddVariable("m_wbb");
loader.AddVariable("m_wwbb");

TMVA::gConfig().SetDrawProgressBar( false);

TCut mycuts;
TCut mycutb;

auto tsignal =  (TTree*) inputFile->Get("sig_tree");
auto tbackground =  (TTree*) inputFile->Get("bkg_tree");

loader.AddSignalTree    (tsignal,     1.0);   //signal weight  = 1
loader.AddBackgroundTree(tbackground, 1.0);   //background weight = 1 


loader.PrepareTrainingAndTestTree(mycuts, mycutb,
                                  "nTrain_Signal=1000:nTrain_Background=1000:nTest_Signal=1000:nTest_Background=1000:"
                                  "SplitMode=Random:NormMode=NumEvents:!V" );


TMVA::VariableImportance * vi = new TMVA::VariableImportance(&loader);

vi->BookMethod(TMVA::Types::kBDT, "BDT",
             "V:NTrees=200:MinNodeSize=2.5%:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.5:"
             "UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );


TStopwatch tw; tw.Start();
//vi->SetType(TMVA::kRandom);
vi->SetType(TMVA::kShort);

vi->Evaluate();
tw.Print();

auto results=vi->GetResults();
results.Print();
results.Draw();
gPad->Draw();



}
