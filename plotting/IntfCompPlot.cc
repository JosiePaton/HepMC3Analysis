void IntfCompPlot(){

  TFile* fileWeight = new TFile("Achilles_sbndFlux_kinVar_Intf_weights.root");
  TFile* fileNoweight = new TFile("Achilles_sbndFlux_kinVar_Intf.root");
  
  string var = "OutE";
  string histQE = "h"+var+"_qe";
  string histIntf = "h"+var+"_intf";
  
  TH1D* hQEW = (TH1D*)fileWeight->Get(histQE.c_str()); 
  TH1D* hInW = (TH1D*)fileWeight->Get(histIntf.c_str()); 
  TH1D* hQEN = (TH1D*)fileNoweight->Get(histQE.c_str()); 
  TH1D* hInN = (TH1D*)fileNoweight->Get(histIntf.c_str()); 
  
  double scaleW = 1/(hQEW->Integral()+hInW->Integral());
  double scaleN = 1/(hQEN->Integral()+hInN->Integral());

  hQEW->Scale(scaleW);
  hInW->Scale(scaleW);
  hQEN->Scale(scaleN);
  hInN->Scale(scaleN);

  hQEN->GetYaxis()->SetTitle("AU");
  hQEW->SetLineColor(2);
  hQEN->SetLineColor(2);
  hQEN->SetLineStyle(2);
  hInN->SetLineStyle(2);
  hQEN->SetStats(0);
  
  hQEN->Draw("hist");
  hInN->Draw("hist same");
  hQEW->Draw("hist same");
  hInW->Draw("hist same");

  TLegend* l1 = new TLegend(0.5,0.6,0.8,0.8);
  l1->AddEntry(hQEN,"QE, no weight","l");
  l1->AddEntry(hInN,"Intf, no weight","l");
  l1->AddEntry(hQEW,"QE, weight","l");
  l1->AddEntry(hInW,"Intf, weight","l");
  l1->Draw();
    
}
