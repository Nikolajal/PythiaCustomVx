//! --- --- --- --- --- --- --- --- --- ---
//! A simple library for the PythiaCustomVx package
//! author: Nicola Rubini
//! last edit: 18/12/2022
//! --- --- --- --- --- --- --- --- --- ---
//! Includes and namespaces
#include "Pythia8/Pythia.h"
#include "TString.h"
#include "TH1F.h"
using namespace Pythia8;
using namespace ROOT;
using namespace std;
//! Global functions
//! --- General utility
#define uVarString(name) uVarStringInner(#name)
TString uVarStringInner ( string name ) { return TString(name); }
//!
void
SetRunOption
( int fOption, Pythia &pythia ) {
    cout << "[INFO] Using option tune " << fOption << endl;
    switch (fOption) {
        case 1:
            pythia.readFile("Pythia_RunCmds/QCD_based.cmnd");
            system("cat Pythia_RunCmds/QCD_based.cmnd");
            break;
        case 2:
            pythia.readFile("Pythia_RunCmds/Gluon_move_model.cmnd");
            system("cat Pythia_RunCmds/Gluon_move_model.cmnd");
            break;
        case 3:
            pythia.readFile("Pythia_RunCmds/SKI_model.cmnd");
            system("cat Pythia_RunCmds/SKI_model.cmnd");
            break;
        case 4:
            pythia.readFile("Pythia_RunCmds/SKII_model.cmnd");
            system("cat Pythia_RunCmds/SKII_model.cmnd");
            break;
        case 5:
            pythia.readFile("Pythia_RunCmds/CR_mode0.cmnd");
            system("cat Pythia_RunCmds/CR_mode0.cmnd");
            break;
        case 6:
            pythia.readFile("Pythia_RunCmds/CR_mode2.cmnd");
            system("cat Pythia_RunCmds/CR_mode2.cmnd");
            break;
        case 7:
            pythia.readFile("Pythia_RunCmds/CR_mode3.cmnd");
            system("cat Pythia_RunCmds/CR_mode3.cmnd");
            break;
        case 8:
            pythia.readFile("Pythia_RunCmds/QCD_CR_ropes.cmnd");
            system("cat Pythia_RunCmds/QCD_CR_ropes.cmnd");
            break;
        case 9:
            pythia.readFile("Pythia_RunCmds/QCD_CR_no_ropes.cmnd");
            system("cat Pythia_RunCmds/QCD_CR_no_ropes.cmnd");
            break;
        default:
            cout << "[INFO] Using default settings" << endl;
            cout << "To change settings choose from below:" << endl;
            cout << "D. The MPI-based original Pythia 8 scheme." << endl;
            cout << "1. The new more QCD based scheme." << endl;
            cout << "2. The new gluon-move model." << endl;
            cout << "3. The SK I e^+ e^- CR model." << endl;
            cout << "4. The SK II e^+ e^- CR model." << endl;
            cout << "5. Mode 0 from https://arxiv.org/pdf/1505.01681.pdf." << endl;
            cout << "6. Mode 2 from https://arxiv.org/pdf/1505.01681.pdf." << endl;
            cout << "7. Mode 3 from https://arxiv.org/pdf/1505.01681.pdf." << endl;
            cout << "8. Custom mode 1: QCD based CR w/ ropes" << endl;
            cout << "9. Custom mode 2: QCD based CR w/o ropes" << endl;
            break;
    }
}
//!
template < typename ... kOptionType >
void
SetkEventStatistics
 ( std::map<TString,TH1F*> hStatistics, int kOption ... ) {
    va_list args;
    va_start(args, kOption);
    while (kOption != '\0') {
        kOption++;
    }
    va_end(args);
}
