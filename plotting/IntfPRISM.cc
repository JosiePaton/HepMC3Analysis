void IntfPRISM(){

  TFile* fileWeight = new TFile("Achilles_prismFlux_kinVar_Intf_NewFile.root");

  TH1D* hQE[3];
  TH1D* hIn[3];
  string bins[3] = {"0.0-0.2","0.8-1.0","1.4-1.6"};
  string prism[3] = {"0","4","7"};
  string var = "InE";
  TLegend* l1 = new TLegend(0.7,0.6,0.9,0.8);

  TCanvas* c1 = new TCanvas();
  for(int i=0;i<3;i++){
    
    string histQE = "h"+var+"_qe_"+prism[i];
    string histIntf = "h"+var+"_intf_"+prism[i];
    
    hQE[i] = (TH1D*)fileWeight->Get(histQE.c_str()); 
    hIn[i] = (TH1D*)fileWeight->Get(histIntf.c_str()); 

    TH1D* hSum = (TH1D*)hQE[i]->Clone("hSum");
    hSum->Add(hIn[i]);

    cout<<"For PRISM Bin "<<bins[i]<<", "<<hIn[i]->Integral()/hSum->Integral()<<endl;
    //double scaleW = 1/(hQE[i]->Integral()+hIn[i]->Integral());
    double scaleW = 1/hSum->Integral();

    hSum->Scale(scaleW);
    hQE[i]->Scale(scaleW);
    hIn[i]->Scale(scaleW);

    //hSum->Rebin(5);
    //hQE[i]->Rebin(5);
    //hIn[i]->Rebin(5);
    string Totlab = "Total "+bins[i];
    string QElab = "QE "+bins[i];
    string Inlab = "Intf "+bins[i];

    hQE[i]->SetLineStyle(2);
    hIn[i]->SetLineStyle(3);
    hSum->SetLineColor(i+2);
    hQE[i]->SetLineColor(i+2);
    hIn[i]->SetLineColor(i+2);
    //hSum->SetLineWidth(2);
    hQE[i]->SetLineWidth(3);
    hIn[i]->SetLineWidth(3);
    l1->AddEntry(hSum,Totlab.c_str(),"l");
    l1->AddEntry(hQE[i],QElab.c_str(),"l");
    l1->AddEntry(hIn[i],Inlab.c_str(),"l");
    
    if(i==0){
      hSum->GetYaxis()->SetTitle("Probability");
      hSum->SetStats(0);
      hSum->Draw("hist");
      hQE[i]->Draw("hist same");
      hIn[i]->Draw("hist same");
    }
    else{
      hSum->Draw("hist same");
      hQE[i]->Draw("hist same");
      hIn[i]->Draw("hist same");
    }
  }
    
  l1->Draw();
    
}
