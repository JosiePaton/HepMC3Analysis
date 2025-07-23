void plotDividedHistograms(){

  //Read in files
  TFile* fArgon = new TFile("argonFileName.root");
  TFile* fCarbon = new TFile("carbonFileName.root");

  //Select the variable you want to plot
  string variable = "hIn_nu_E";

  //Create the names of the 3 different histograms
  string totalName = variable+"_total";
  string qeName = variable+"_qe";
  string intfName = variable+"_intf";

  //Get the two total histograms from the files, create a ratio histogram
  TH1D* hArgon_total = (TH1D*)fArgon->Get(totalName.c_str());
  TH1D* hCarbon_total = (TH1D*)fCarbon->Get(totalName.c_str());
  TH1D* hRatio_total = (TH1D*)hArgon_total->Clone();
  hRatio_total->Divide(hCarbon_total);

  //Do the same for QE only
  TH1D* hArgon_qe = (TH1D*)fArgon->Get(qeName.c_str());
  TH1D* hCarbon_qe = (TH1D*)fCarbon->Get(qeName.c_str());
  TH1D* hRatio_qe = (TH1D*)hArgon_qe->Clone();
  hRatio_qe->Divide(hCarbon_qe);

  //Do the same for interference only
  TH1D* hArgon_intf = (TH1D*)fArgon->Get(intfName.c_str());
  TH1D* hCarbon_intf = (TH1D*)fCarbon->Get(intfName.c_str());
  TH1D* hRatio_intf = (TH1D*)hArgon_intf->Clone();
  hRatio_intf->Divide(hCarbon_intf);

  //Get rid of stats box from histogram
  hRatio_total->SetStats(0);
  //Set the line colors to be different
  hRatio_total->SetLineColor(1);
  hRatio_qe->SetLineColor(2);
  hRatio_intf->SetLineColor(3);
  //Set the axis titles
  hRatio_total->GetYaxis()->SetTitle("AU");
  hRatio_total->GetXaxis()->SetTitle("VariableNameAndUnits");
  //Get rid of histogram title
  hRatio_total->SetTitle("");

  //Create a canvas and draw the three histograms
  TCanvas* c1 = new TCanvas();
  hRatio_total->Draw("HIST");
  hRatio_qe->Draw("HIST same");
  hRatio_intf->Draw("HIST same");

  //Create the legend and draw on the canvas
  TLegend* l1 = new TLegend(0.6,0.7,0.9,0.9);
  l1->AddEntry(hRatio_total,"total","l");
  l1->AddEntry(hRatio_qe,"QE","l");
  l1->AddEntry(hRatio_intf,"Interference","l");
  l1->Draw();
}
