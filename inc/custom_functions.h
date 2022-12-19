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
( int fOption, Pythia &pythia ){
    cout << "[INFO] Using Mode " << fOption << endl;
    switch (fOption)
    {
        case 1:
            pythia.readFile("Pythia_RunCmds/QCD_based.cmnd");
            cout << "The new more QCD based scheme." << endl;
            break;
        case 2:
            pythia.readString("ColourReconnection:reconnect = on");
            pythia.readString("ColourReconnection:mode = 2");
            cout << "[INFO] Using Mode 2" << endl;
            cout << "The new gluon-move model." << endl;
            break;
        case 3:
            pythia.readString("ColourReconnection:reconnect = on");
            pythia.readString("ColourReconnection:mode = 3");
            pythia.readString("ColourReconnection:forceResonance = on");
            pythia.readString("PartonLevel:earlyResDec = off");
            cout << "[INFO] Using Mode 3" << endl;
            cout << "The SK I e^+ e^- CR model." << endl;
            break;
        case 4:
            pythia.readString("ColourReconnection:reconnect = on");
            pythia.readString("ColourReconnection:mode = 4");
            pythia.readString("ColourReconnection:forceResonance = on");
            pythia.readString("PartonLevel:earlyResDec = off");
            cout << "[INFO] Using Mode 4" << endl;
            cout << "The SK II e^+ e^- CR model." << endl;
            break;
        case 5:
            pythia.readString("StringPT:sigma = 0.335");
            pythia.readString("StringZ:aLund = 0.36");
            pythia.readString("StringZ:aLund = 0.56");
            pythia.readString("StringFlav:probQQtoQ = 0.078");
            pythia.readString("StringFlav:probStoUD = 0.2");
            pythia.readString("StringFlav:probQQ1toQQ0join = 0.0275, 0.0275, 0.0275, 0.0275");
            pythia.readString("MultiPartonInteractions:pT0Ref = 2.12");
            pythia.readString("BeamRemnants:remnantMode = 1");
            pythia.readString("BeamRemnants:saturation = 5");
            pythia.readString("ColourReconnection:mode = 1");
            pythia.readString("ColourReconnection:allowDoubleJunRem = off");
            pythia.readString("ColourReconnection:m0 = 2.9");
            pythia.readString("ColourReconnection:allowJunctions = on");
            pythia.readString("ColourReconnection:junctionCorrection = 1.43");
            pythia.readString("ColourReconnection:timeDilationMode = 0");
            cout << "[INFO] Using Mode 5" << endl;
            cout << "Mode 0 from https://arxiv.org/pdf/1505.01681.pdf" << endl;
            break;
        case 6:
            pythia.readString("StringPT:sigma = 0.335");
            pythia.readString("StringZ:aLund = 0.36");
            pythia.readString("StringZ:aLund = 0.56");
            pythia.readString("StringFlav:probQQtoQ = 0.078");
            pythia.readString("StringFlav:probStoUD = 0.2");
            pythia.readString("StringFlav:probQQ1toQQ0join = 0.0275, 0.0275, 0.0275, 0.0275");
            pythia.readString("MultiPartonInteractions:pT0Ref = 2.15");
            pythia.readString("BeamRemnants:remnantMode = 1");
            pythia.readString("BeamRemnants:saturation = 5");
            pythia.readString("ColourReconnection:mode = 1");
            pythia.readString("ColourReconnection:allowDoubleJunRem = off");
            pythia.readString("ColourReconnection:m0 = 0.3");
            pythia.readString("ColourReconnection:allowJunctions = on");
            pythia.readString("ColourReconnection:junctionCorrection = 1.20");
            pythia.readString("ColourReconnection:timeDilationMode = 2");
            pythia.readString("ColourReconnection:timeDilationPar = 0.18");
            cout << "[INFO] Using Mode 6" << endl;
            cout << "Mode 2 from https://arxiv.org/pdf/1505.01681.pdf" << endl;
            break;
        case 7:
            pythia.readString("StringPT:sigma = 0.335");
            pythia.readString("StringZ:aLund = 0.36");
            pythia.readString("StringZ:aLund = 0.56");
            pythia.readString("StringFlav:probQQtoQ = 0.078");
            pythia.readString("StringFlav:probStoUD = 0.2");
            pythia.readString("StringFlav:probQQ1toQQ0join = 0.0275, 0.0275, 0.0275, 0.0275");
            pythia.readString("MultiPartonInteractions:pT0Ref = 2.05");
            pythia.readString("BeamRemnants:remnantMode = 1");
            pythia.readString("BeamRemnants:saturation = 5");
            pythia.readString("ColourReconnection:mode = 1");
            pythia.readString("ColourReconnection:allowDoubleJunRem = off");
            pythia.readString("ColourReconnection:m0 = 0.3");
            pythia.readString("ColourReconnection:allowJunctions = on");
            pythia.readString("ColourReconnection:junctionCorrection = 1.15");
            pythia.readString("ColourReconnection:timeDilationMode = 3");
            pythia.readString("ColourReconnection:timeDilationPar = 0.073");
            cout << "[INFO] Using Mode 7" << endl;
            cout << "Mode 3 from https://arxiv.org/pdf/1505.01681.pdf" << endl;
            break;
        case 8:
            pythia.readString("MultiPartonInteractions:pT0Ref = 2.15");
            pythia.readString("BeamRemnants:remnantMode = 1");
            pythia.readString("BeamRemnants:saturation = 5");
            pythia.readString("ColourReconnection:mode = 1");
            pythia.readString("ColourReconnection:allowDoubleJunRem = off");
            pythia.readString("ColourReconnection:m0 = 0.3");
            pythia.readString("ColourReconnection:allowJunctions = on");
            pythia.readString("ColourReconnection:junctionCorrection = 1.2");
            pythia.readString("ColourReconnection:timeDilationMode = 2");
            pythia.readString("ColourReconnection:timeDilationPar = 0.18");
            pythia.readString("Ropewalk:RopeHadronization = on");
            pythia.readString("Ropewalk:doShoving = on");
            pythia.readString("Ropewalk:tInit = 1.5");
            pythia.readString("Ropewalk:deltat = 0.05");
            pythia.readString("Ropewalk:tShove = 0.1");
            pythia.readString("Ropewalk:gAmplitude = 0");
            pythia.readString("Ropewalk:doFlavour = on");
            pythia.readString("Ropewalk:r0 = 0.5");
            pythia.readString("Ropewalk:m0 = 0.2");
            pythia.readString("Ropewalk:beta = 0.1");
            pythia.readString("PartonVertex:setVertex = on");
            pythia.readString("PartonVertex:protonRadius = 0.7");
            pythia.readString("PartonVertex:emissionWidth = 0.1");
            cout << "[INFO] Using Mode 8" << endl;
            cout << "Custom mode 1: QCD based CR w/ ropes" << endl;
            break;
        case 9:
            pythia.readString("MultiPartonInteractions:pT0Ref = 2.15");
            pythia.readString("BeamRemnants:remnantMode = 1");
            pythia.readString("BeamRemnants:saturation = 5");
            pythia.readString("ColourReconnection:mode = 1");
            pythia.readString("ColourReconnection:allowDoubleJunRem = off");
            pythia.readString("ColourReconnection:allowJunctions = on");
            pythia.readString("ColourReconnection:m0 = 0.3");
            pythia.readString("ColourReconnection:junctionCorrection = 1.2");
            pythia.readString("ColourReconnection:timeDilationMode = 2");
            pythia.readString("ColourReconnection:timeDilationPar = 0.18");
            pythia.readString("Ropewalk:RopeHadronization = off");
            cout << "[INFO] Using Mode 9" << endl;
            cout << "Custom mode 2: QCD based CR w/o ropes" << endl;
            break;
        case 10:
            pythia.readString("StringFlav:probStoUD = 0.000");
            cout << "[INFO] Using Mode 10" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probStoUD = 0.000 (D 0.217)" << endl;
            break;
        case 11:
            pythia.readString("StringFlav:probStoUD = 0.100");
            cout << "[INFO] Using Mode 11" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probStoUD = 0.100 (D 0.217)" << endl;
            break;
        case 12:
            pythia.readString("StringFlav:probStoUD = 0.300");
            cout << "[INFO] Using Mode 12" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probStoUD = 0.300 (D 0.217)" << endl;
            break;
        case 13:
            pythia.readString("StringFlav:probStoUD = 0.400");
            cout << "[INFO] Using Mode 13" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probStoUD = 0.400 (D 0.217)" << endl;
            break;
        case 14:
            pythia.readString("StringFlav:probSQtoQQ = 0.700");
            cout << "[INFO] Using Mode 14" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probSQtoQQ = 0.700 (D 0.915)" << endl;
            break;
        case 15:
            pythia.readString("StringFlav:probSQtoQQ = 0.800");
            cout << "[INFO] Using Mode 15" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probSQtoQQ = 0.800 (D 0.915)" << endl;
            break;
        case 16:
            pythia.readString("StringFlav:probSQtoQQ = 0.900");
            cout << "[INFO] Using Mode 16" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probSQtoQQ = 0.900 (D 0.915)" << endl;
            break;
        case 17:
            pythia.readString("StringFlav:probSQtoQQ = 1.000");
            cout << "[INFO] Using Mode 17" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probSQtoQQ = 1.000 (D 0.915)" << endl;
            break;
        case 18:
            pythia.readString("StringFlav:probStoUD = 0.280");
            cout << "[INFO] Using Mode 18" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probStoUD = 0.280 (D 0.217)" << endl;
            break;
        case 19:
            pythia.readString("StringFlav:probStoUD = 0.260");
            cout << "[INFO] Using Mode 19" << endl;
            cout << "Personal Mode 0: mode 0 + StringFlav:probStoUD = 0.260 (D 0.217)" << endl;
            break;
        case 20:
            pythia.readString("StringFlav:probStoUD = 0.260");
            pythia.readString("StringFlav:probSQtoQQ = 0.600");
            cout << "[INFO] Using Mode 20" << endl;
            cout << "Personal Mode 10: mode 0 + StringFlav:probStoUD = 0.260 (D 0.217) + StringFlav:probSQtoQQ = 0.600 (D 0.915)" << endl;
            break;
        case 21:
            pythia.readString("MultiPartonInteractions:pT0Ref = 2.15");
            pythia.readString("BeamRemnants:remnantMode = 1");
            pythia.readString("BeamRemnants:saturation = 5");
            pythia.readString("ColourReconnection:mode = 1");
            pythia.readString("ColourReconnection:allowDoubleJunRem = off");
            pythia.readString("ColourReconnection:m0 = 0.3");
            pythia.readString("ColourReconnection:allowJunctions = on");
            pythia.readString("ColourReconnection:junctionCorrection = 1.2");
            pythia.readString("ColourReconnection:timeDilationMode = 2");
            pythia.readString("ColourReconnection:timeDilationPar = 0.18");
            pythia.readString("Ropewalk:RopeHadronization = on");
            pythia.readString("Ropewalk:doShoving = on");
            pythia.readString("Ropewalk:tInit = 1.5");
            pythia.readString("Ropewalk:deltat = 0.05");
            pythia.readString("Ropewalk:tShove = 0.1");
            pythia.readString("Ropewalk:gAmplitude = 0");
            pythia.readString("Ropewalk:doFlavour = on");
            pythia.readString("Ropewalk:r0 = 0.5");
            pythia.readString("Ropewalk:m0 = 0.2");
            pythia.readString("Ropewalk:beta = 0.1");
            pythia.readString("PartonVertex:setVertex = on");
            pythia.readString("PartonVertex:protonRadius = 0.7");
            pythia.readString("PartonVertex:emissionWidth = 0.1");
            pythia.readString("StringFlav:probStoUD = 0.260");
            cout << "[INFO] Using Mode 20" << endl;
            cout << "Personal Mode 21: mode 8 + StringFlav:probStoUD = 0.260 (D 0.217)" << endl;
            break;
        case 22:
            pythia.readString("MultiPartonInteractions:pT0Ref = 2.15");
            pythia.readString("BeamRemnants:remnantMode = 1");
            pythia.readString("BeamRemnants:saturation = 5");
            pythia.readString("ColourReconnection:mode = 1");
            pythia.readString("ColourReconnection:allowDoubleJunRem = off");
            pythia.readString("ColourReconnection:m0 = 0.3");
            pythia.readString("ColourReconnection:allowJunctions = on");
            pythia.readString("ColourReconnection:junctionCorrection = 1.2");
            pythia.readString("ColourReconnection:timeDilationMode = 2");
            pythia.readString("ColourReconnection:timeDilationPar = 0.18");
            pythia.readString("Ropewalk:RopeHadronization = on");
            pythia.readString("Ropewalk:doShoving = on");
            pythia.readString("Ropewalk:tInit = 1.5");
            pythia.readString("Ropewalk:deltat = 0.05");
            pythia.readString("Ropewalk:tShove = 0.1");
            pythia.readString("Ropewalk:gAmplitude = 0");
            pythia.readString("Ropewalk:doFlavour = on");
            pythia.readString("Ropewalk:r0 = 0.5");
            pythia.readString("Ropewalk:m0 = 0.2");
            pythia.readString("Ropewalk:beta = 0.1");
            pythia.readString("PartonVertex:setVertex = on");
            pythia.readString("PartonVertex:protonRadius = 0.7");
            pythia.readString("PartonVertex:emissionWidth = 0.1");
            pythia.readString("StringFlav:probStoUD = 0.240");
            cout << "[INFO] Using Mode 20" << endl;
            cout << "Personal Mode 21: mode 8 + StringFlav:probStoUD = 0.240 (D 0.217)" << endl;
            break;
            /*
        case 23:
            uSetStringTension(pythia,1.);
            break;
        case 24:
            uSetStringTension(pythia,0.9);
            break;
        case 25:
            uSetStringTension(pythia,1.1);
            break;
        case 26:
            uSetStringTension(pythia,0.5);
            break;
        case 27:
            uSetStringTension(pythia,1.5);
            break;
        case 28:
            uSetStringTension(pythia,0.2);
            break;
        case 29:
            uSetStringTension(pythia,2.0);
            break;
             */
            //  --- --- --- --- --- --- ---
            //  --- --- --- AGANTYR
            //  --- --- --- --- --- --- ---
        case 100:
            pythia.readString("Beams:idA = 1000822080");
            pythia.readString("Beams:idB = 1000822080");
            cout << "[INFO] Using Mode 100" << endl;
            cout << "100. Vanilla Agantyr" << endl;
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
            cout << "10<N<100. Custom investigating modes" << endl;
            cout << "100. Vanilla Agantyr" << endl;
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
        cout << "kOption Name: " << uVarString(kOption) << endl;
        cout << "kOption Vals: " << kOption << endl;
        kOption++;
    }
    va_end(args);
}
