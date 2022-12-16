#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include "Pythia8Plugins/HepMC3.h"

using namespace Pythia8;
using namespace HepMC3;

int main() {

    // Interface for conversion from Pythia8::Event to HepMC event.
    HepMC3::Pythia8ToHepMC3 topHepMC;

    // Specify file where HepMC events will be stored.
    HepMC3::WriterAscii ascii_io("hepmcout41.hepmc");

    // Generator. Process selection. LHC initialization. Histogram.
    Pythia pythia;
    pythia.readString("Beams:eCM = 8000.");
    pythia.readString("HardQCD:all = on");
    pythia.readString("PhaseSpace:pTHatMin = 20.");
    pythia.init();
    
    // Begin event loop. Generate event. Skip if error.
    for (int iEvent = 0; iEvent < 100; ++iEvent) {
        pythia.next();

        HepMC3::GenEvent hepmcevt;
        topHepMC.fill_next_event( pythia, &hepmcevt );
        ascii_io.write_event(hepmcevt);
  }
  return 0;
}
