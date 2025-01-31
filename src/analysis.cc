#include "HepMC3/GenEvent.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/Print.h"
#include <iostream>

using namespace HepMC3;

void analyze_event(const GenEvent &evt) {
    // Loop over particles in event, keep the outgoing lepton and all protons
    ConstGenParticlePtr lepton_out;
    std::vector<ConstGenParticlePtr> proton_out;
    for(const auto &part : evt.particles()) {
        if(part->status() != 1) continue;
        if(std::abs(part->pid()) == 13) lepton_out = part;
        else if(part->pid() == 2212) proton_out.push_back(part);
    }

    std::cout << "Found outgoing lepton with energy: " << lepton_out->momentum().e() << std::endl;
    std::cout << "Event had multiplicity of: " << proton_out.size() << std::endl;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        std::cout << "Usage: " << argv[0] << " <HepMC3_input_file>" << std::endl;
        exit(-1);
    }

    int events_parsed = 0;
    double xsec = 0;
    double sum_weights = 0;
    ReaderAscii input_file(argv[1]);

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
            analyze_event(evt);
        }
    }

    std::cout << "Normalization factor (xsec/sum_weights) = " << xsec/sum_weights << std::endl;

    input_file.close();

    return 0;
}
