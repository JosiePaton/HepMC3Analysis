void PRISMProp(){

  TFile* fileWeight = new TFile("Achilles_prismFlux_kinVar_Intf_NewFile.root");

  string bins[3] = {"0.0-0.2","0.8-1.0","1.4-1.6"};
  string prism[3] = {"0","4","7"};
  string var = "Cos";
  TLegend* l1 = new TLegend(0.7,0.6,0.9,0.8);

  TCanvas* c1 = new TCanvas();
  for(int i=0;i<3;i++){
    
    string histQE = "h"+var+"_qe_"+prism[i];
    string histIntf = "h"+var+"_intf_"+prism[i];
    
    TH1D* hQE = (TH1D*)fileWeight->Get(histQE.c_str()); 
    TH1D* hIn = (TH1D*)fileWeight->Get(histIntf.c_str()); 

    TH1D* hSum = (TH1D*)hQE->Clone("hSum");
    hSum->Add(hIn);

    cout<<"For PRISM Bin "<<bins[i]<<", "<<hIn->Integral()/hSum->Integral()<<endl;

    TH1D* hProp = (TH1D*)hIn->Clone("hProp");
    hProp->Divide(hSum);
    cout<<hProp->Integral()<<endl;
    
    //hSum->Rebin(5);
    //hQE->Rebin(5);
    //hIn->Rebin(5);
    string lab = "Intf Proportion: "+bins[i];

    hProp->SetLineColor(i+2);
    l1->AddEntry(hProp,lab.c_str(),"l");

    if(i==0){
      hProp->GetYaxis()->SetTitle("Probability");
      hProp->SetStats(0);
      hProp->Draw("hist");
    }
    else{
      hProp->Draw("hist same");
    }
  }
    
  l1->Draw();
    
}
