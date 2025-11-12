#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/Print.h"
#include "ROOT/TH1D.h"
#include "ROOT/TH2D.h"
#include "ROOT/TFile.h"
#include "ROOT/TVector3.h"
#include <iostream>

using namespace HepMC3;

int main(int argc, char **argv) {
  if(argc != 3) {
    std::cout << "Usage: " << argv[0] << " <HepMC3_input_file> <Output_file_name>" << std::endl;
    exit(-1);
  }
  
  //Get flux plot for weighting
  TFile* fflux = TFile::Open("fluxes/sbnd_flux.root");
  TH1D* numuFlux_Gev = (TH1D*)fflux->Get("flux_sbnd_numu");
  
  //Set up variables 
  int events_parsed = 0;
  double xsec = 0;
  double sum_weights = 0;
  
  //Read in file
  ReaderAscii input_file(argv[1]);
  
  //Set up arrays of histograms. Each array corresponds to a variable we're plotting.
  //Current histograms:
  //                   hIn_nu_E - Incoming neutrino energy
  //                   hOut_mu_pz - Outgoing muon z-momentum
  //                   hOut_p_cosTheta - Outgoing proton cos(theta)
  TH1D* hIn_nu_E[3];
  TH1D* homega[3];
  TH1D* hOut_mu_pz[3];
  TH1D* hOut_p_cosTheta[3];
  
  //Create 3 histograms per variable: total events, QE (quasi-elastic) events, and inft (interference) events
  string inter[3] = {"total","qe","intf"};
  for(int i=0;i<3;i++){
    std::string histName_In_nu_E = "hIn_nu_E_"+inter[i];
    hIn_nu_E[i] = new TH1D(histName_In_nu_E.c_str(),histName_In_nu_E.c_str(),100,0,4000);
    std::string histName_omega = "homega_"+inter[i];
    homega[i] = new TH1D(histName_omega.c_str(),histName_omega.c_str(),100,0,1000);
    std::string histName_Out_mu_pz = "hOut_mu_pz_"+inter[i];
    hOut_mu_pz[i] = new TH1D(histName_Out_mu_pz.c_str(),histName_Out_mu_pz.c_str(),100,0,4000);
    std::string histName_Out_p_cosTheta = "hOut_p_cosTheta"+inter[i];
    hOut_p_cosTheta[i] = new TH1D(histName_Out_p_cosTheta.c_str(),histName_Out_p_cosTheta.c_str(),100,-1,1);
    hOut_p_cosTheta[i]->Sumw2();
  }
  
  
  //Enter loop through events, will stop running when the file "fails" (when you run out of events)
  while(!input_file.failed()) {
    
    // Read event from input file
    GenEvent evt(Units::MEV, Units::MM);
    input_file.read_event(evt);
    
    // Exit if failed to read next event
    if(input_file.failed()) break;
    
    // Get the cross section estimate and the event weight
    std::shared_ptr<GenCrossSection> cs = evt.attribute<GenCrossSection>("GenCrossSection");
    xsec = cs->xsec();
    sum_weights += evt.weights()[0];

    //Read out the details of the first event (good for debugging or understanding the event structure)
    if(events_parsed++==0) {
      std::cout << " First event: " << std::endl;
      Print::listing(evt);
      Print::content(evt);
      std::cout << " GenCrossSection:  ";
      Print::line(cs);
    }

    //Read out the number of events analyzed (good for seeing your code running)
    if(events_parsed%10000 == 0) {
      std::cout << "Events parsed: " << events_parsed << std::endl;
    }

    //Define the particles we'll get variables from
    ConstGenParticlePtr lepton_out;
    ConstGenParticlePtr neutrino_in;
    ConstGenParticlePtr proton_out;
    
    //Loop through particles to find relevant ones
    for(const auto &part : evt.particles()) {
      //Within hepMC event structure, the incoming neutrino is the "child" of the particle with status "4"
      if(part->status() == 4) neutrino_in = part->children()[0];
      //Once we have the neutrino, we can now ignore all other particles other than final state particles, i.e. skip everything that doesn't have status "1"
      if(part->status() != 1) continue;
      //Now take the particles with the pdg ids of a muon (13) and a proton (2212) in the final state
      if(std::abs(part->pid()) == 14) lepton_out = part;
      else if(part->pid() == 2212 || part->pid() == 2112) proton_out = part;
    }
    
    //Get variables for plotting
    //Start with the energy of the incoming neutrino
    double in_nu_E = neutrino_in->momentum().e();
    //Get omega (energy transfer)
    double omega = neutrino_in->momentum().e() - lepton_out->momentum().e();
    //Then get the z-momentum of the outgoing muon
    double out_mu_pz = lepton_out->momentum().z();
    //Finally get the cosTheta of the outgoing proton
    double out_p_cosTheta = std::cos(proton_out->momentum().theta());

    //Get event weight = fluxWeight*generatorWeight (remembering to convert the neutrino energy to GeV)
    float weight =  numuFlux_Gev->GetBinContent(numuFlux_Gev->FindBin(in_nu_E/1000.))*evt.weights()[0] ;

    //Fill "total" histograms with all events
    hIn_nu_E[0]->Fill(in_nu_E,weight);
    homega[0]->Fill(omega,weight);
    hOut_mu_pz[0]->Fill(out_mu_pz,weight);
    hOut_p_cosTheta[0]->Fill(out_p_cosTheta,weight);
        
    //Sort via interaction type to fill qe and intf histograms
    auto procID = evt.attribute<IntAttribute>("signal_process_id")->value();
    int proc = 10;
    if(procID>=200 && procID<=300) proc = 1; //qe
    if(procID>=700 && procID<=800) proc = 2; //intf
    if(proc != 1 && proc !=2) std::cout<<"Missing process! Not qe or intf!"<<std::endl;

    //Fill either qe or intf histogram based on signal process id
    hIn_nu_E[proc]->Fill(in_nu_E,weight);
    homega[proc]->Fill(omega,weight);
    hOut_mu_pz[proc]->Fill(out_mu_pz,weight);
    hOut_p_cosTheta[proc]->Fill(out_p_cosTheta,weight);
        
  }
  
  std::cout << "Normalization factor (xsec/sum_weights) = " << xsec/sum_weights << std::endl;

  //Scale factors applied to the histograms in order to create cross sections
  for(int j=0;j<3;j++){
    hIn_nu_E[j]->Scale(xsec/sum_weights);
    homega[j]->Scale(xsec/sum_weights);
    hOut_mu_pz[j]->Scale(xsec/sum_weights);
    hOut_p_cosTheta[j]->Scale(xsec/sum_weights);
  }
  //Close the input file so it doesn't crash
  input_file.close();

  double QEerror,Intferror;
  double QEint = hOut_p_cosTheta[1]->IntegralAndError(1,100,QEerror);
  double Intfint = hOut_p_cosTheta[2]->IntegralAndError(1,100,Intferror);
  std::cout<<"Integral of QE = "<<QEint<<" +/- "<<QEerror<<std::endl;
  std::cout<<"Integral of Intf =  "<<Intfint<<" +/- "<<Intferror<<std::endl;
  
  //Create an output file to save the histograms
  TFile* outfile = new TFile(argv[2],"RECREATE");
  for(int n=0;n<3;n++){
    hIn_nu_E[n]->Write();
    homega[n]->Write();
    hOut_mu_pz[n]->Write();
    hOut_p_cosTheta[n]->Write();
  }
  delete outfile;
  
  return 0;
}
