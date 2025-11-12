void VariablePlotting(){

  string exp = "dune";
  string casc = "OFF";
  string var = "Out_p_cosTheta";
  
  string fileName = "../files/fromNathan/"+exp+"flux_cascades"+casc+".root";
  
  TFile* file = new TFile(fileName.c_str());
 
  string histTot = "h"+var+"_total";
  string histQE = "h"+var+"_qe";
  string histIntf = "h"+var+"_intf";
  string histRes = "h"+var+"_res";
  
  TH1D* hTot = (TH1D*)file->Get(histTot.c_str()); 
  TH1D* hQE = (TH1D*)file->Get(histQE.c_str()); 
  TH1D* hIn = (TH1D*)file->Get(histIntf.c_str()); 
  TH1D* hRes = (TH1D*)file->Get(histRes.c_str()); 
  
  TH1D* hSum = (TH1D*)hQE->Clone("hSum");
  hSum->Add(hIn);
  hSum->Add(hRes);

  if(hSum->Integral() != hTot->Integral()){
    cout<<"Mismatch in hists!!"<<endl;
    cout<<"Sum = "<<hSum->Integral()<<endl;
    cout<<"Total = "<<hTot->Integral()<<endl;
  }

  if(var!="Emu"){
    /*hTot->Rebin(2);
    hQE->Rebin(2);
    hIn->Rebin(2);
    hRes->Rebin(2);*/
    }

  string yaxis;
  double scale = 1.;
  if(exp=="sbnd"){
    double Nt = 1.6693858e+30;
    double POTscale = 1e+15;
    //double POTscale = 1e-6;
    double intScale = 1./4000.;
    double deltaE = 50.;
    double areaScale = 1e-40;
    scale = Nt*deltaE*POTscale*intScale*areaScale;
    //scale = deltaE*POTscale*intScale*areaScale;
    hTot->Scale(scale);
    hQE->Scale(scale);
    hIn->Scale(scale);
    hRes->Scale(scale);
    cout<<"Total events = "<<hTot->Integral()<<endl;
    yaxis = "SBND Events / 10^{21} POT";
  }
  else{
    double areaScale = 1e-40;
    double fluxInt = 0.00104;
    scale = areaScale*fluxInt;
    hTot->Scale(scale);
    hQE->Scale(scale);
    hIn->Scale(scale);
    hRes->Scale(scale);
    yaxis = "DUNE Events / POT / target";
    cout<<"Total events = "<<hTot->Integral()<<endl;
  }
  
  hTot->GetYaxis()->SetTitle(yaxis.c_str());
  hTot->SetStats(0);
  hTot->SetTitle("");
  hTot->SetLineColor(1);
  hQE->SetLineColor(2);
  hIn->SetLineColor(3);
  hRes->SetLineColor(4);
  hTot->SetLineWidth(2);
  hQE->SetLineWidth(2);
  hIn->SetLineWidth(2);
  hRes->SetLineWidth(2);
  
  hTot->Draw("hist E");
  hQE->Draw("hist same E");
  hIn->Draw("hist same E");
  hRes->Draw("hist same E");

  TLegend* l1 = new TLegend(0.7,0.6,0.9,0.8);
  l1->SetLineWidth(0);
  l1->AddEntry(hTot,"Total","l");

  if(exp=="sbnd" && var=="omega"){
    l1->AddEntry(hQE,"QE (67.8 #pm 0.2 %)","l");
    l1->AddEntry(hIn,"Intf (9.97 #pm 0.08 %)","l");
    l1->AddEntry(hRes,"Res (22.2 #pm 0.1 %)","l");
  }
  else if(exp=="dune" && var=="omega"){
    l1->AddEntry(hQE,"QE (58.2 #pm 0.1 %)","l");
    l1->AddEntry(hIn,"Intf (7.22 #pm 0.03 %)","l");
    l1->AddEntry(hRes,"Res (34.5 #pm 0.1 %)","l");
  }
  else{
    l1->AddEntry(hQE,"QE","l");
    l1->AddEntry(hIn,"Intf","l");
    l1->AddEntry(hRes,"Res","l");
  }
  l1->Draw();

  if(var=="Out_p_cosTheta"){
    double TotError,QEError,IntfError,ResError;
    double Totint = hTot->IntegralAndError(1,150,TotError);
    double QEint = hQE->IntegralAndError(1,150,QEError);
    double Intfint = hIn->IntegralAndError(1,150,IntfError);
    double Resint = hRes->IntegralAndError(1,150,ResError);
    std::cout<<"Integral of Total = "<<Totint<<" +/- "<<TotError<<std::endl;
    std::cout<<"Integral of QE = "<<QEint<<" +/- "<<QEError<<std::endl;
    std::cout<<"Integral of Intf = "<<Intfint<<" +/- "<<IntfError<<std::endl;
    std::cout<<"Integral of Res = "<<Resint<<" +/- "<<ResError<<std::endl;

    double fracQE = QEint/Totint;
    double fracIntf = Intfint/Totint;
    double fracRes = Resint/Totint;

    double uncQE = fracQE*sqrt((QEError/QEint)*(QEError/QEint) + (TotError/Totint)*(TotError/Totint));
    double uncIntf = fracIntf*sqrt((IntfError/Intfint)*(IntfError/Intfint) + (TotError/Totint)*(TotError/Totint));
    double uncRes = fracRes*sqrt((ResError/Resint)*(ResError/Resint) + (TotError/Totint)*(TotError/Totint));
    std::cout<<std::endl;
    std::cout<<"Fraction of QE = "<<fracQE<<" +/- "<<uncQE<<std::endl;
    std::cout<<"Fraction of Intf = "<<fracIntf<<" +/- "<<uncIntf<<std::endl;
    std::cout<<"Fraction of Res = "<<fracRes<<" +/- "<<uncRes<<std::endl;
    cout<<"Sum = "<<fracQE+fracIntf+fracRes<<endl;
  }
}
