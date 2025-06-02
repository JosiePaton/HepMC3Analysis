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
        std::cout << "Usage: " << argv[0] << " <HepMC3_input_file>" << std::endl;
        exit(-1);
    }

    //Get flux files
    TFile* fflux = TFile::Open("prism_fluxes_numu.root");

    string prism[3] = {"0","4","7"};
    TH1D* prismFluxes[3];
    //Loop through plots and scale them to MeV
    for(int j = 0;j<3;j++){
      std::string inName = "prism_flux_"+prism[j];
      TH1D* numuFlux_Gev = (TH1D*)fflux->Get(inName.c_str());
      std::string fluxName = "numuFlux_"+prism[j];
      prismFluxes[j] = new TH1D(fluxName.c_str(),fluxName.c_str(),80,0,4000);
      for(int i=0;i<prismFluxes[j]->GetNbinsX();i++){
	prismFluxes[j]->SetBinContent(i+1,numuFlux_Gev->GetBinContent(i+1));
      }
    }
    //Set up variables 
    int events_parsed = 0;
    double xsec = 0;
    double sum_weights = 0;

    //Read in file
    ReaderAscii input_file(argv[1]);

    //Set up histograms
    TH1D* hInE_f[2];
    TH1D* hInE[2][3];
    TH1D* hOutE[2][3];
    TH1D* hQ2[2][3];
    TH1D* homega[2][3];
    TH1D* hCos[2][3];
    TH1D* hCosOp[2][3];
    TH2D* hCosQ[2][3];
    TH2D* hCosE[2][3];
    TH1D* hdpt[2][3];
    TH1D* hdphit[2][3];
    TH1D* hdalphat[2][3];
    
    string inter[2] = {"qe","intf"};
    for(int i=0;i<2;i++){
      std::string histName_InE_f = "hInE_f_"+inter[i];
      hInE_f[i] = new TH1D(histName_InE_f.c_str(),histName_InE_f.c_str(),80,0,4000);
      for(int j=0;j<3;j++){
	std::string histName_InE = "hInE_"+inter[i]+"_"+prism[j];
	hInE[i][j] = new TH1D(histName_InE.c_str(),histName_InE.c_str(),80,0,4000);
	std::string histName_OutE = "hOutE_"+inter[i]+"_"+prism[j];
	hOutE[i][j] = new TH1D(histName_OutE.c_str(),histName_OutE.c_str(),80,0,4000);
	std::string histName_Q2 = "hQ2_"+inter[i]+"_"+prism[j];
	hQ2[i][j] = new TH1D(histName_Q2.c_str(),histName_Q2.c_str(),100,0,1000000);
	std::string histName_omega = "homega_"+inter[i]+"_"+prism[j];
	homega[i][j] = new TH1D(histName_omega.c_str(),histName_omega.c_str(),100,0,1000);
	std::string histName_Cos = "hCos_"+inter[i]+"_"+prism[j];
	hCos[i][j] = new TH1D(histName_Cos.c_str(),histName_Cos.c_str(),100,-1,1);
	std::string histName_CosOp = "hCosOp_"+inter[i]+"_"+prism[j];
	hCosOp[i][j] = new TH1D(histName_CosOp.c_str(),histName_CosOp.c_str(),100,-1,1);
	std::string histName_CosQ = "hCosQ_"+inter[i]+"_"+prism[j];
	hCosQ[i][j] = new TH2D(histName_CosQ.c_str(),histName_CosQ.c_str(),100,-1,1,100,0,1000000);
	std::string histName_CosE = "hCosE_"+inter[i]+"_"+prism[j];
	hCosE[i][j] = new TH2D(histName_CosE.c_str(),histName_CosE.c_str(),100,-1,1,100,0,4000);
	std::string histName_dpt = "hdpt_"+inter[i]+"_"+prism[j];
	hdpt[i][j] = new TH1D(histName_dpt.c_str(),histName_dpt.c_str(),80,0,1000);
	std::string histName_dphit = "hdphit_"+inter[i]+"_"+prism[j];
	hdphit[i][j] = new TH1D(histName_dphit.c_str(),histName_dphit.c_str(),90,0,180);
	std::string histName_dalphat = "hdalphat_"+inter[i]+"_"+prism[j];
	hdalphat[i][j] = new TH1D(histName_dalphat.c_str(),histName_dalphat.c_str(),90,0,180);
      }
    }
    
    while(!input_file.failed()) {
        GenEvent evt(Units::MEV, Units::MM);

        // Read event from input file
        input_file.read_event(evt);

        // Exit if failed to read next event
        if(input_file.failed()) break;

        // Get the cross section estimate and the event weight
        std::shared_ptr<GenCrossSection> cs = evt.attribute<GenCrossSection>("GenCrossSection");
        xsec = cs->xsec();
        sum_weights += evt.weights()[0];

        if(events_parsed++==0) {
            std::cout << " First event: " << std::endl;
            Print::listing(evt);
            Print::content(evt);

            std::cout << " GenCrossSection:  ";
            Print::line(cs);
        }
        
        if(events_parsed%10000 == 0) {
            std::cout << "Events parsed: " << events_parsed << std::endl;
        }

	ConstGenParticlePtr lepton_out;
	ConstGenParticlePtr neutrino_in;
	std::vector<ConstGenParticlePtr> proton_out;

	//Loop through particles to find relevant ones
	for(const auto &part : evt.particles()) {
	  if(part->status() == 4) neutrino_in = part->children()[0];
	  if(part->status() != 1) continue;
	  if(std::abs(part->pid()) == 13) lepton_out = part;
	  else if(part->pid() == 2212) proton_out.push_back(part);
	}
	
	//Get variables for plotting
	double inE = neutrino_in->momentum().e();
	double omega = neutrino_in->momentum().e() - lepton_out->momentum().e();
	double costheta = std::cos(lepton_out->momentum().theta());
	FourVector q = neutrino_in->momentum() - lepton_out->momentum();
	double Q2 = -1*q.m2();
	double outE = lepton_out->momentum().e();
	TVector3 mumom(lepton_out->momentum().x(),lepton_out->momentum().y(),lepton_out->momentum().z());
	TVector3 protmom(proton_out[0]->momentum().x(),proton_out[0]->momentum().y(),proton_out[0]->momentum().z());
	double cosalpha = std::cos(mumom.Angle(protmom));
	TVector3 mumomT(lepton_out->momentum().x(),lepton_out->momentum().y(),0);
        TVector3 protmomT(proton_out[0]->momentum().x(),proton_out[0]->momentum().y(),0);
        TVector3 dmomT = mumomT + protmomT;
        double dpt = dmomT.Mag();
        double dphit = protmomT.Angle(-mumomT)*(180/3.14159);
        double dalphat = dmomT.Angle(-mumomT)*(180/3.14159);

	
	//Sort via interaction type
	auto procID = evt.attribute<IntAttribute>("signal_process_id")->value();
	int proc = 10;
	if(procID>=200 && procID<=300) proc = 0; //qe
	if(procID>=700 && procID<=800) proc = 1; //intf
	
	//Fill incoming energy hist (flat flux)
	hInE_f[proc]->Fill(inE,evt.weights()[0]);

	//Loop through prism bins
	for(int j = 0;j<3;j++){
	  //Fill flux weighted incoming and outgoing energy hist
	  float weight =  prismFluxes[j]->GetBinContent(prismFluxes[j]->FindBin(inE))*evt.weights()[0] ;
	  hInE[proc][j]->Fill(inE,weight);
	  hOutE[proc][j]->Fill(outE,weight);
	  
	  //Plot variables
	  hQ2[proc][j]->Fill(Q2,weight);
	  homega[proc][j]->Fill(omega,weight);
	  hCos[proc][j]->Fill(costheta,weight);
	  hCosOp[proc][j]->Fill(cosalpha,weight);
	  hCosQ[proc][j]->Fill(costheta,Q2,weight);
	  hCosE[proc][j]->Fill(costheta,outE,weight);
	  hdpt[proc][j]->Fill(dpt,weight);
	  hdphit[proc][j]->Fill(dphit,weight);
	  hdalphat[proc][j]->Fill(dalphat,weight);
	}
	//Debugging code snippet
	/*
	std::cout << "Incoming neutrino energy: " << neutrino_in->momentum().e() << std::endl;
	std::cout << "Found outgoing muon with energy: " << lepton_out->momentum().e() << std::endl;
	std::cout << "Omega = " << omega << ", cosTheta = " << costheta << std::endl;
	std::cout << "Event had multiplicity of: " << proton_out.size() << std::endl;
	if(events_parsed == 20) break;
	*/
	
    }

    std::cout << "Normalization factor (xsec/sum_weights) = " << xsec/sum_weights << std::endl;
    for(int i = 0;i<2;i++){
      hInE_f[i]->Scale(xsec/sum_weights);
      for(int j=0;j<3;j++){
	hInE[i][j]->Scale(xsec/sum_weights);
	hOutE[i][j]->Scale(xsec/sum_weights);
	hQ2[i][j]->Scale(xsec/sum_weights);
	homega[i][j]->Scale(xsec/sum_weights);
	hCos[i][j]->Scale(xsec/sum_weights);
	hCosOp[i][j]->Scale(xsec/sum_weights);
	hCosQ[i][j]->Scale(xsec/sum_weights);
	hCosE[i][j]->Scale(xsec/sum_weights);
	hdpt[i][j]->Scale(xsec/sum_weights);
	hdphit[i][j]->Scale(xsec/sum_weights);
	hdalphat[i][j]->Scale(xsec/sum_weights);
      }
    }
    input_file.close();

    //TFile* outfile = new TFile("Achilles_prismFlux_kinVar_Intf_NewFile.root","RECREATE");
    TFile* outfile = new TFile(argv[2],"RECREATE");
    for(int j=0;j<3;j++){
      prismFluxes[j]->Write();
      for(int i=0;i<2;i++){
	hInE[i][j]->Write();
	hOutE[i][j]->Write();
	hQ2[i][j]->Write();
	homega[i][j]->Write();
	hCos[i][j]->Write();
	hCosOp[i][j]->Write();
	hCosQ[i][j]->Write();
	hCosE[i][j]->Write();
	hdpt[i][j]->Write();
	hdphit[i][j]->Write();
	hdalphat[i][j]->Write();
      }
    }
    hInE_f[0]->Write();
    hInE_f[1]->Write();
	
    delete outfile;
    delete fflux;
    
    return 0;
}
