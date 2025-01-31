#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/Print.h"
#include "ROOT/TH1D.h"
#include "ROOT/TH2D.h"
#include "ROOT/TFile.h"
#include <iostream>

using namespace HepMC3;

int main(int argc, char **argv) {
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <HepMC3_input_file>" << std::endl;
        exit(-1);
    }

    //Get flux plot for weighting and convert it to MeV
    TFile* fflux = TFile::Open("sbnd_flux.root");
    TH1D* numuFlux_Gev = (TH1D*)fflux->Get("flux_sbnd_numu");
    TH1D* numuFlux = new TH1D("numuFlux","numuFlux",80,0,4000);
    for(int i=0;i<numuFlux->GetNbinsX();i++){
      numuFlux->SetBinContent(i+1,numuFlux_Gev->GetBinContent(i+1));
    }
    
    //Set up variables 
    int events_parsed = 0;
    double xsec = 0;
    double sum_weights = 0;

    //Read in file
    ReaderAscii input_file(argv[1]);

    //Set up histograms
    TH1D* hInE_f = new TH1D("hInE_f","hInE_f",80,0,4000);
    TH1D* hInE = new TH1D("hInE","hInE",80,0,4000);
    TH1D* hOutE = new TH1D("hOutE","hOutE",80,0,4000);
    TH1D* hQ2 = new TH1D("hQ2","hQ2",100,0,1000000);
    TH1D* homega = new TH1D("homega","homega",100,0,1000);
    TH1D* hCos = new TH1D("hCos","hCos",100,-1,1);
    TH2D* hCosQ = new TH2D("hCosQ","hCosQ",100,-1,1,100,0,1000000);
    TH2D* hCosE = new TH2D("hCosE","hCosE",100,-1,1,100,0,4000);
    
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
        
        if(events_parsed%1000 == 0) {
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
	
	//Fill incoming energy hist (flat flux)
	hInE_f->Fill(inE);

	//Fill flux weighted incoming and outgoing energy hist
	float weight =  numuFlux->GetBinContent(numuFlux->FindBin(inE)) ;
	hInE->Fill(inE,weight);
	hOutE->Fill(outE,weight);

	//Plot variables
	hQ2->Fill(Q2,weight);
	homega->Fill(omega,weight);
	hCos->Fill(costheta,weight);
	hCosQ->Fill(costheta,Q2,weight);
	hCosE->Fill(costheta,outE,weight);
	
	//TODO: Find how to separate by interaction type
	
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
    hInE_f->Scale(xsec/sum_weights);
    hInE->Scale(xsec/sum_weights);
    hOutE->Scale(xsec/sum_weights);
    hQ2->Scale(xsec/sum_weights);
    homega->Scale(xsec/sum_weights);
    hCos->Scale(xsec/sum_weights);
    hCosQ->Scale(xsec/sum_weights);
    hCosE->Scale(xsec/sum_weights);
    
    input_file.close();

    TFile* outfile = new TFile("Achilles_sbndFlux_kinVar.root","RECREATE");
    numuFlux->Write();
    hInE_f->Write();
    hInE->Write();
    hOutE->Write();
    hQ2->Write();
    homega->Write();
    hCos->Write();
    hCosQ->Write();
    hCosE->Write();
    delete outfile;
    delete fflux;
    
    return 0;
}
