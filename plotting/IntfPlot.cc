void IntfPlot(){

  //TFile* fileWeight = new TFile("Achilles_sbndFlux_kinVar_Intf_weights.root");
  TFile* fileWeight = new TFile("Achilles_sbndFlux_Carbon_kinVar_Intf_NewFile.root");
 
  string var = "InE";
  string histQE = "h"+var+"_qe";
  string histIntf = "h"+var+"_intf";
  
  TH1D* hQEW = (TH1D*)fileWeight->Get(histQE.c_str()); 
  TH1D* hInW = (TH1D*)fileWeight->Get(histIntf.c_str()); 

  TH1D* hSum = (TH1D*)hQEW->Clone("hSum");
  hSum->Add(hInW);
  
  //double scaleW = 1/(hQEW->Integral()+hInW->Integral());
  double scaleW = 1/(hSum->Integral());
  if(hSum->Integral()!=(hQEW->Integral()+hInW->Integral())) cout<<"NO MATCH, "<<hSum->Integral()<<" vs "<<hQEW->Integral()+hInW->Integral()<<endl;

  hSum->Scale(scaleW);
  hQEW->Scale(scaleW);
  hInW->Scale(scaleW);

  //hSum->Rebin(5);
  //hQEW->Rebin(5);
  //hInW->Rebin(5);
  
  hSum->GetYaxis()->SetTitle("Probability");
  hSum->SetStats(0);
  hSum->SetTitle("");
  hSum->SetLineColor(1);
  
  hQEW->SetLineColor(2);

  hSum->Draw("hist");
  hQEW->Draw("hist same");
  hInW->Draw("hist same");

  TLegend* l1 = new TLegend(0.7,0.6,0.9,0.8);
  l1->AddEntry(hSum,"Total","l");
  l1->AddEntry(hQEW,"QE","l");
  l1->AddEntry(hInW,"Intf","l");
  l1->Draw();
    
}
