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

    //Get flux plot for weighting and convert it to MeV
    /*TFile* fflux = TFile::Open("sbnd_flux.root");
    TH1D* numuFlux_Gev = (TH1D*)fflux->Get("flux_sbnd_numu");
    TH1D* numuFlux = new TH1D("numuFlux","numuFlux",80,0,4000);
    for(int i=0;i<numuFlux->GetNbinsX();i++){
      numuFlux->SetBinContent(i+1,numuFlux_Gev->GetBinContent(i+1));
      }*/
    
    //Set up variables 
    int events_parsed = 0;
    double xsec = 0;
    double sum_weights = 0;

    //Read in file
    ReaderAscii input_file(argv[1]);

    //Set up histograms
    TH1D* hInE_f[2];
    TH1D* hInE[2];
    TH1D* hOutE[2];
    TH1D* hQ2[2];
    TH1D* homega[2];
    TH1D* hCos[2];
    TH1D* hCosOp[2];
    TH2D* hCosQ[2];
    TH2D* hCosE[2];

    string inter[2] = {"qe","intf"};
    for(int i=0;i<2;i++){
      std::string histName_InE = "hInE_"+inter[i];
      hInE[i] = new TH1D(histName_InE.c_str(),histName_InE.c_str(),100,0,5000);
      std::string histName_OutE = "hOutE_"+inter[i];
      hOutE[i] = new TH1D(histName_OutE.c_str(),histName_OutE.c_str(),100,0,5000);
      std::string histName_Q2 = "hQ2_"+inter[i];
      hQ2[i] = new TH1D(histName_Q2.c_str(),histName_Q2.c_str(),100,0,1000000);
      std::string histName_omega = "homega_"+inter[i];
      homega[i] = new TH1D(histName_omega.c_str(),histName_omega.c_str(),100,0,1000);
      std::string histName_Cos = "hCos_"+inter[i];
      hCos[i] = new TH1D(histName_Cos.c_str(),histName_Cos.c_str(),100,-1,1);
      std::string histName_CosOp = "hCosOp_"+inter[i];
      hCosOp[i] = new TH1D(histName_CosOp.c_str(),histName_CosOp.c_str(),100,-1,1);
      std::string histName_CosQ = "hCosQ_"+inter[i];
      hCosQ[i] = new TH2D(histName_CosQ.c_str(),histName_CosQ.c_str(),100,-1,1,100,0,1000000);
      std::string histName_CosE = "hCosE_"+inter[i];
      hCosE[i] = new TH2D(histName_CosE.c_str(),histName_CosE.c_str(),100,-1,1,100,0,5000);
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
	
	//Sort via interaction type
	auto procID = evt.attribute<IntAttribute>("signal_process_id")->value();
	int proc = 10;
	if(procID>=200 && procID<=300) proc = 0; //qe
	if(procID>=700 && procID<=800) proc = 1; //intf
	
	//Fill flux weighted incoming and outgoing energy hist
	//float weight =  numuFlux->GetBinContent(numuFlux->FindBin(inE))*evt.weights()[0] ;
	float weight =  evt.weights()[0] ;
	hInE[proc]->Fill(inE,weight);
	hOutE[proc]->Fill(outE,weight);

	//Plot variables
	hQ2[proc]->Fill(Q2,weight);
	homega[proc]->Fill(omega,weight);
	hCos[proc]->Fill(costheta,weight);
	hCosOp[proc]->Fill(cosalpha,weight);
	hCosQ[proc]->Fill(costheta,Q2,weight);
	hCosE[proc]->Fill(costheta,outE,weight);
	
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
    for(int j=0;j<2;j++){
      hInE[j]->Scale(xsec/sum_weights);
      hOutE[j]->Scale(xsec/sum_weights);
      hQ2[j]->Scale(xsec/sum_weights);
      homega[j]->Scale(xsec/sum_weights);
      hCos[j]->Scale(xsec/sum_weights);
      hCosOp[j]->Scale(xsec/sum_weights);
      hCosQ[j]->Scale(xsec/sum_weights);
      hCosE[j]->Scale(xsec/sum_weights);
    }
    input_file.close();

    TFile* outfile = new TFile(argv[2],"RECREATE");
    //numuFlux->Write();
    for(int n=0;n<2;n++){
      hInE[n]->Write();
      hOutE[n]->Write();
      hQ2[n]->Write();
      homega[n]->Write();
      hCos[n]->Write();
      hCosOp[n]->Write();
      hCosQ[n]->Write();
      hCosE[n]->Write();
    }
    delete outfile;
    //delete fflux;
    
    return 0;
}
