// Pythia
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
//
using namespace std;
using namespace Pythia8;
//
int
main
 ( int argc, char *argv[] ) {
    // Check everything is good
    if (argc < 3 )  {
        cout << "ERROR: Insufficient parameters given!" << endl;
        cout << "Please use as: ./GeneratorMC [filename] [nevents] [seed] [option] [Energy]" << endl;
        return -1;
    }
    
    // Definition of Filename
    TString kFileName   = argv[1];
    
    // Definition of Seed
    int     nSeed   = atoi(argv[3]);
    
    // Definition of number of events
    int     nEvents = atoi(argv[2]);
    
    // Definition of option
    int     fOption = argc < 4 ? 0 : atoi(argv[4]);
    
    // Definition of energy
    int     kEnergy = argc < 5 ? 7000 : atoi(argv[5]);

    cout << "Filename: " << kFileName.Data() <<  " - Nevs: " << nEvents <<  " - Energy: " << kEnergy << " - Option: " << fOption << " - Seed: " << nSeed << endl;
    
    // Output File
    TFile * outFile     =   new   TFile   (Form("%s.root",kFileName.Data()),   "recreate", "", 101);
    cout << Form("%s.root",kFileName.Data()) << endl;
    //Output Tree
    Int_t   nPrt;
    Float_t   nMult05  =   0;
    Float_t   nMult08  =   0;
    Float_t   nMult10  =   0;
    Int_t   fPDG[1024];
    Float_t fPx[1024];
    Float_t fPy[1024];
    Float_t fPz[1024];
    Int_t   isString[1024];
    TTree*  outTree     =   new TTree(Form("Prt_E%i_M%i",kEnergy,fOption),Form("Phi_E%i_M%i",kEnergy,fOption));
    outTree->Branch       ("nPart",     &nPrt,      "nPrt/I");
    outTree->Branch       ("Eta_05",    &nMult05,   "nMult05/F");
    outTree->Branch       ("Eta_08",    &nMult08,   "nMult08/F");
    outTree->Branch       ("Eta_10",    &nMult10,   "nMult10/F");
    outTree->Branch       ("PDG",       &fPDG,      "fPDG[nPrt]/I");
    outTree->Branch       ("Px",        &fPx,       "fPx[nPrt]/F");
    outTree->Branch       ("Py",        &fPy,       "fPy[nPrt]/F");
    outTree->Branch       ("Pz",        &fPz,       "fPz[nPrt]/F");
    outTree->Branch       ("isString",  &isString,  "isString[nPrt]/I");
    
    //  Output Histos
    TList*  lQCParticle    =   new TList();
    //
    TH1D*   fQC_Event_Enum_FLL              = new TH1D("fQC_Event_Enum_FLL",    "Event Selection",                                      29, -0.5, 28.5);
    fQC_Event_Enum_FLL              ->  GetXaxis()  ->  SetTitle("");
    fQC_Event_Enum_FLL              ->  GetYaxis()  ->  SetTitle("Accepted Events");
    lQCParticle    ->  Add(fQC_Event_Enum_FLL);
    //
    TH1D*   fQC_Event_Enum_E05              = new TH1D("fQC_Event_Enum_E05",    "Events with tracks in #eta < 0.5",                     500,   0., 500.);
    fQC_Event_Enum_E05              ->  GetXaxis()  ->  SetTitle("N Tracks");
    fQC_Event_Enum_E05              ->  GetYaxis()  ->  SetTitle("Accepted Events");
    lQCParticle    ->  Add(fQC_Event_Enum_E05);
    //
    TH1D*   fQC_Event_Enum_E08              = new TH1D("fQC_Event_Enum_E08",    "Events with tracks in #eta < 0.8",                     500,   0., 500.);
    fQC_Event_Enum_E08              ->  GetXaxis()  ->  SetTitle("N Tracks");
    fQC_Event_Enum_E08              ->  GetYaxis()  ->  SetTitle("Accepted Events");
    lQCParticle    ->  Add(fQC_Event_Enum_E08);
    //
    TH1D*   fQC_Event_Enum_E10              = new TH1D("fQC_Event_Enum_E10",    "Events with tracks in #eta < 1.0",                     500,   0., 500.);
    fQC_Event_Enum_E10              ->  GetXaxis()  ->  SetTitle("N Tracks");
    fQC_Event_Enum_E10              ->  GetYaxis()  ->  SetTitle("Accepted Events");
    lQCParticle    ->  Add(fQC_Event_Enum_E10);
    //
    // PYTHIA INITIALISATION
    Pythia8::Pythia pythia;
    
    // Settings
    pythia.readString("SoftQCD:all = on");
    pythia.readString("ParticleDecays:limitTau0 = on");
    pythia.readString("Beams:idA = 2212");
    pythia.readString("Beams:idB = 2212");
    pythia.readString(Form("Beams:eCM = %i",kEnergy));
    cout << Form("[INFO] Energy: %i",kEnergy) << endl;
    pythia.readString(Form("Tune:pp = %i",14));
    pythia.readString(Form("333:mMin = %f",fMinIMMC)); //0.75
    pythia.readString(Form("333:mMax = %f",fMaxIMMC)); //1.25
    //
    pythia.readString("Random:setSeed = on");
    pythia.readString("Print:quiet = on");
    pythia.readString(Form("Random:seed = %i",nSeed));
    cout << Form("[INFO] Seed: %i",nSeed) << endl;
    pythia.readString("ColourReconnection:reconnect = on");
    pythia.readString("ColourReconnection:mode = 0");
    switch (fOption)
    {
        case 1:
            pythia.readString("ColourReconnection:reconnect = on");
            pythia.readString("ColourReconnection:mode = 1");
            pythia.readString("BeamRemnants:remnantMode = 1");
            cout << "[INFO] Using Mode 1" << endl;
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
    pythia.init();
    
    // Start Timer
    fStartTimer("Production");
    
    // Cycling through events
    for ( int iEvent = 0; iEvent < nEvents; iEvent++ )  {
        // Next event
        pythia.next();
        //pythia.stat();
        //
        // Print progress
        fPrintLoopTimer("Production",iEvent,nEvents,10000);
        //
        std::vector<Int_t>  kLabel_X0333_Phi;
        //
        // Starting cycling through event particles
        nPrt    = 0;
        nMult05 = 0;
        nMult08 = 0;
        nMult10 = 0;
        for ( int iParticle = 0; iParticle < pythia.event.size() ; iParticle++ )
        {
            // Saving particles
            const auto Current_Particle = pythia.event[iParticle];

            //  Selecting Phis
            if ( Current_Particle.id() == 333 ) { kLabel_X0333_Phi.push_back(iParticle); }
            
            //  Calculating Multiplicity
            
            //Skipping non-final particles
            if ( !Current_Particle.isFinal() )          continue;
            
            //Skipping non-charged particles
            if ( !Current_Particle.isCharged() )        continue;
            
            //Skipping particles outside eta range
            if ( fabs(Current_Particle.eta()) < 0.5 )   nMult05++;
            
            //Skipping particles outside eta range
            if ( fabs(Current_Particle.eta()) < 0.8 )   nMult08++;
            
            //Skipping particles outside eta range
            if ( fabs(Current_Particle.eta()) < 1.0 )   nMult10++;
            
        }
        //
        //  Save for later
        for ( auto kCurrent_Prt : kLabel_X0333_Phi )    {
            // Saving particles
            const auto Current_Particle = pythia.event[kCurrent_Prt];
            //
            fPDG    [nPrt]  =   Current_Particle.id();
            fPx     [nPrt]  =   Current_Particle.px();
            fPy     [nPrt]  =   Current_Particle.py();
            fPz     [nPrt]  =   Current_Particle.pz();
            isString[nPrt]  =   uCheckString(Current_Particle) ? Current_Particle.mother1() + Current_Particle.mother2()*10000 : -1;
            nPrt++;
        }
        fQC_Event_Enum_FLL->Fill(1);
        fQC_Event_Enum_E05->Fill(nMult05);
        fQC_Event_Enum_E08->Fill(nMult08);
        fQC_Event_Enum_E10->Fill(nMult10);
        if ( nPrt > 0 ) outTree->Fill();
    }
    fStopTimer("Production");
    //
    lQCParticle->Write();
    outTree->Write();
    //
    outFile     ->Close();
    return 0;
}
