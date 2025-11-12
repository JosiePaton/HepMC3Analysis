void IntfPRISM(){

  string var = "Emu";
  bool casc = true;

  string bins[3] = {"0.0-0.2","0.8-1.0","1.4-1.6"};
  string prism[3] = {"0","4","7"};
  TLegend* l1 = new TLegend(0.7,0.4,0.85,0.89);
  l1->SetLineWidth(0);
  TH1D* hTot[3];
  TH1D* hQE[3];
  TH1D* hIn[3];
  TH1D* hRes[3];

  TCanvas* c1 = new TCanvas();
  for(int i=0;i<3;i++){
    string fileName;
    if(casc) fileName = "../files/fromNathan/all_updated_prism_hists/prism_flux"+prism[i]+"cascades.root";
    else fileName = "../files/fromNathan/all_updated_prism_hists/prism_flux"+prism[i]+"_hists.root";
  
    TFile* file = new TFile(fileName.c_str());

    string histTot = "h"+var+"_total";
    string histQE = "h"+var+"_qe";
    string histIntf = "h"+var+"_intf";
    string histRes = "h"+var+"_res";
    
    hTot[i] = (TH1D*)file->Get(histTot.c_str()); 
    hQE[i] = (TH1D*)file->Get(histQE.c_str()); 
    hIn[i] = (TH1D*)file->Get(histIntf.c_str()); 
    hRes[i] = (TH1D*)file->Get(histRes.c_str()); 

    //hTot[i]->Rebin(2);
    //hQE[i]->Rebin(2);
    //hIn[i]->Rebin(2);
    //hRes[i]->Rebin(2);

    double scale = 1/hTot[i]->Integral();
    hTot[i]->Scale(scale);
    hQE[i]->Scale(scale);
    hIn[i]->Scale(scale);
    hRes[i]->Scale(scale);
    
    string Totlab = "Total "+bins[i];
    string QElab = "QE "+bins[i];
    string Inlab = "Intf "+bins[i];
    string Reslab = "Res "+bins[i];

    hQE[i]->SetLineStyle(2);
    hIn[i]->SetLineStyle(3);
    hRes[i]->SetLineStyle(9);
    hTot[i]->SetLineColor(i+2);
    hQE[i]->SetLineColor(i+2);
    hIn[i]->SetLineColor(i+2);
    hRes[i]->SetLineColor(i+2);
    hTot[i]->SetLineWidth(2);
    hQE[i]->SetLineWidth(4);
    hIn[i]->SetLineWidth(5);
    hRes[i]->SetLineWidth(2);
    if(i!=1){
      l1->AddEntry(hTot[i],Totlab.c_str(),"l");
      l1->AddEntry(hQE[i],QElab.c_str(),"l");
      l1->AddEntry(hIn[i],Inlab.c_str(),"l");
      l1->AddEntry(hRes[i],Reslab.c_str(),"l");
    }
    if(i==0){
      hTot[i]->GetYaxis()->SetTitle("Total Area Normalised Probability");
      hTot[i]->SetStats(0);
      hTot[i]->Draw("hist E");
      hQE[i]->Draw("hist same E");
      hIn[i]->Draw("hist same E");
      hRes[i]->Draw("hist same E");
    }
    else if(i==2){
      hTot[i]->Draw("hist same E");
      hQE[i]->Draw("hist same E");
      hIn[i]->Draw("hist same E");
      hRes[i]->Draw("hist same");
    }
  }
  
  l1->Draw();
    
}
