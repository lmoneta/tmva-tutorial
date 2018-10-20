void TMVA_Reader_ManyMethods() { 


TMVA::Tools::Instance();

std::vector<std::string> methodNames = {"BDT","DL_CPU"};

TFile * inputFile = TFile::Open("Higgs_data.root");

inputFile->ls();


TTree* sig_tree = (TTree*) inputFile->Get("sig_tree");
TTree* bkg_tree = (TTree*) inputFile->Get("bkg_tree");

//sig_tree->Print(); 

TMVA::Reader reader( "!Color:!Silent" );

float m_jj, m_jjj, m_lv, m_jlv, m_bb, m_wbb, m_wwbb; 

// add variables 
reader.AddVariable("m_jj",&m_jj);
reader.AddVariable("m_jjj",&m_jjj);
reader.AddVariable("m_lv",&m_lv);
reader.AddVariable("m_jlv",&m_jlv);
reader.AddVariable("m_bb",&m_bb);
reader.AddVariable("m_wbb",&m_wbb);
reader.AddVariable("m_wwbb",&m_wwbb);

for (size_t i = 0; i < methodNames.size(); ++i) { 
   TString weightfile = "dataset/weights/TMVA_Higgs_Classification_" + methodNames[i] + ".weights.xml";
   reader.BookMVA( methodNames[i], weightfile );
}



auto h1 = new TH1D("h1","Classifier Output on Background Events",100,-1,1);
auto h2 = new TH1D("h2","Classifier Output on Signal Events",100,-1,1);
std::vector<std::vector<float>> vout(methodNames.size());

auto readEvents = [&](TTree & tree, TH1 & histo) {
  tree.SetBranchAddress("m_jj",&m_jj);
  tree.SetBranchAddress("m_jjj",&m_jjj);
  tree.SetBranchAddress("m_lv",&m_lv) ;
  tree.SetBranchAddress("m_jlv",&m_jlv);
  tree.SetBranchAddress("m_bb",&m_bb);
  tree.SetBranchAddress("m_wbb",&m_wbb);
  tree.SetBranchAddress("m_wwbb",&m_wwbb);
  for (size_t i = 0; i < methodNames.size(); ++i ) {
    vout[i].clear(); 
    vout[i].resize(tree.GetEntries());
  }
  for (Long64_t ievt=0; ievt < tree.GetEntries();ievt++) {

      if (ievt%500 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;

      tree.GetEntry(ievt);
      
      for (size_t i = 0; i < methodNames.size(); ++i ) {
    
         auto output = reader.EvaluateMVA(methodNames[i]);
  
         vout[i][ievt] = output;  
      }
  }
};
    

// read events and fill histogram
readEvents(*bkg_tree,*h1); 

std::vector<std::vector<float>> bkg_results = vout; 

readEvents(*sig_tree,*h2); 

std::vector<std::vector<float>> sig_results = vout; 

 std::vector<float> & vsig = sig_results[0];
std::vector<float> & vbkg = bkg_results[0];
TMVA::ROCCurve * roc = new TMVA::ROCCurve(vsig, vbkg);

for (size_t i = 0; i < methodNames.size(); ++i) {
    std::vector<float> & vsig = sig_results[i];
    std::vector<float> & vbkg = bkg_results[i];
    TMVA::ROCCurve * roc = new TMVA::ROCCurve(vsig, vbkg);
//    TGraph *  roc_graph  = roc->GetROCCurve(); 
//    roc_graph->SetTitle(methodNames[i]);
    std::cout << "method " << methodNames[i] << " ROC integral = " << roc->GetROCIntegral() << std::end;
//    roc_graph->SetLineColor(i+1);
//    if (i == 0) roc_graph->Draw("AL");
//    else roc_graph->Draw("L");
}




}
