void ArC_CompPlot(){

  TFile* fileAr = new TFile("Achilles_sbndFlux_kinVar_Intf_NewFile.root");
  TFile* fileC = new TFile("Achilles_sbndFlux_Carbon_kinVar_Intf_NewFile.root");
 
  string var = "OutE";
  string histQE = "h"+var+"_qe";
  string histIntf = "h"+var+"_intf";
  
  TH1D* hQEAr = (TH1D*)fileAr->Get(histQE.c_str()); 
  TH1D* hInAr = (TH1D*)fileAr->Get(histIntf.c_str()); 
  TH1D* hSumAr = (TH1D*)hQEAr->Clone("hSum");
  hSumAr->Add(hInAr);
  
  TH1D* hQEC = (TH1D*)fileC->Get(histQE.c_str()); 
  TH1D* hInC = (TH1D*)fileC->Get(histIntf.c_str()); 
  TH1D* hSumC = (TH1D*)hQEC->Clone("hSum");
  hSumC->Add(hInC);

  hQEAr->Rebin();
  hQEC->Rebin();
  hInAr->Rebin();
  hInC->Rebin(); 
  hSumAr->Rebin();
  hSumC->Rebin();
 
  TH1D* hQE = (TH1D*)hQEAr->Clone("hQE");
  hQE->Divide(hQEC);
  TH1D* hIn = (TH1D*)hInAr->Clone("hIn");
  hIn->Divide(hInC);
  TH1D* hSum = (TH1D*)hSumAr->Clone("hSum");
  hSum->Divide(hSumC);

  hSum->GetYaxis()->SetTitle("N(Ar)/N(C)");
  hSum->SetStats(0);
  hSum->SetTitle("");
  hSum->SetLineColor(1);
  
  hQE->SetLineColor(2);

  hSum->Draw("hist");
  hQE->Draw("hist same");
  hIn->Draw("hist same");

  TLegend* l1 = new TLegend(0.7,0.6,0.9,0.8);
  l1->AddEntry(hSum,"Total","l");
  l1->AddEntry(hQE,"QE","l");
  l1->AddEntry(hIn,"Intf","l");
  l1->Draw();
    
}
