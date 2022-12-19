// Pythia
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include "../inc/custom_functions.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
//
using namespace std;
using namespace Pythia8;
using namespace ROOT;
//
int
main
 ( int argc, char *argv[] ) {
    //! Check minimum options are given
    if (argc < 3 )  {
        cout << "[ERROR] Insufficient parameters given!" << endl;
        cout << "[INFO] Please use as: ./GeneratorMC [filename] [nevents] [seed] opt: [option] [Energy] [beam:idA] [beam:idB] " << endl;
        return -1;
    }
    //! Definition of production variables
    TString kFileName   = argv[1];                          //! Output filename
    int     nSeed       = atoi(argv[3]);                    //! Production seed
    int     nEvents     = atoi(argv[2]);                    //! Production n events
    int     fOption     = argc < 4 ? 0 : atoi(argv[4]);     //! Production option
    int     kEnergy     = argc < 5 ? 7000 : atoi(argv[5]);  //! Production energy
    int     kBeamidA    = argc < 6 ? 2212 : atoi(argv[6]);  //! Incoming A partilce
    int     kBeamidB    = argc < 7 ? 2212 : atoi(argv[7]);  //! Incoming B partilce
    //! Print out production
    cout << "[INFO] Output file: " << kFileName.Data() << endl
    << "[INFO] Number of events: " << nEvents << endl
    << "[INFO] Energy: " << kEnergy << endl
    << "[INFO] Option: " << fOption << endl
    << "[INFO] Seed: " << nSeed << endl
    << "[INFO] Collision: " << kBeamidA << " - " << kBeamidB << endl;
    //! Output structure
    //! --- Output File
    TFile * outFile     =   new   TFile   (Form("%s.root",kFileName.Data()),   "recreate", "", 101);
    //! --- Event record structures
    std::map<TString,Int_t>                     kEventDataset;
    std::map<TString,TH1F*>                     kEventStatistics;
    std::map<Int_t,std::vector<Int_t>>          kParticleDataset;
    std::map<std::pair<Int_t,TString>,TH1F*>    kParticle1DStats;
    std::map<std::pair<Int_t,TString>,TH2F*>    kParticle2DStats;
    //! --- --- Set to 0 all counters
    kEventDataset["nMult05"] = 0;
    kEventDataset["nMult08"] = 0;
    kEventDataset["nMult10"] = 0;
    //! --- --- Building structure
    kParticleDataset[333]   = {};
    for ( auto& [key,value] : kEventDataset ) {
        kEventStatistics[key]       = new TH1F(key,key,6000,0,6000);
        for ( auto& [ kParticleID, kPartVec ] : kParticleDataset ) {
            TString sPraticleID = TString(Form("_%i",kParticleID));
            kParticle1DStats[{kParticleID,key}] = new TH1F(key+sPraticleID,key+sPraticleID,6000,0,6000);
            kParticle1DStats[{kParticleID,key}] = new TH1F(key+TString("_2D")+sPraticleID,key+TString("_2D")+sPraticleID,6000,0,6000);
        }
    }
    for ( auto& [ kParticleID, kPartVec ] : kParticleDataset ) {
        TString sPraticleID = TString(Form("_%i",kParticleID));
        kParticle1DStats[{kParticleID,"PtSpectrum1D"}]   = new TH1F( TString("PtSpectrum1D_")+sPraticleID,   TString("PtSpectrum1D_")+sPraticleID,   3000,0,300);
        kParticle1DStats[{kParticleID,"hDeltaPhiPar"}]   = new TH1F( TString("hDeltaPhiPar_")+sPraticleID,   TString("hDeltaPhiPar_")+sPraticleID,   75,-0.5*3.14159265358979323846,+1.5*3.14159265358979323846);
        kParticle2DStats[{kParticleID,"hDeltaPhiLPr"}]   = new TH2F( TString("hDeltaPhiLPr_")+sPraticleID,   TString("hDeltaPhiLPr_")+sPraticleID,   75,-0.5*3.14159265358979323846,+1.5*3.14159265358979323846, 75,-0.5*3.14159265358979323846,+1.5*3.14159265358979323846);
        kParticle2DStats[{kParticleID,"PtSpectrum2D"}]   = new TH2F( TString("PtSpectrum2D_")+sPraticleID,   TString("PtSpectrum2D_")+sPraticleID,   3000,0,300, 3000,0,300);
    }
    //! Pythia inisialisation
    Pythia8::Pythia pythia;
    //! --- General settings
    pythia.readString(      "Random:setSeed = on");
    pythia.readString(      "Print:quiet = on");
    pythia.readString(Form( "Random:seed = %i",nSeed));
    pythia.readFile(        "Pythia_RunCmds/GeneralSettings.cmnd");
    //! --- --- Collision Physics
    pythia.readString(Form( "Beams:idA = %i",kBeamidA));
    pythia.readString(Form( "Beams:idB = %i",kBeamidB));
    pythia.readString(Form( "Beams:eCM = %i",kEnergy));
    pythia.readString(      "SoftQCD:all = on");
    SetRunOption(fOption,pythia);
    pythia.init();
    //! Start generating events
    for ( int iEvent = 0; iEvent < nEvents; iEvent++ )  {
        //! Next event
        pythia.next();
        //! Set to 0 all counters
        for ( auto& [key,value] : kEventDataset ) value = 0;
        for ( auto& [ kParticleID, kPartVec ] : kParticleDataset ) kPartVec = {};
        auto knLeadingParticle  = -1;
        //! Starting cycling through event particles
        for ( int iParticle = 0; iParticle < pythia.event.size() ; iParticle++ ) {
            const auto kCurrent_Particle = pythia.event[iParticle];
            //! Saving indices for later
            for ( auto& [ kParticleID, kPartVec ] : kParticleDataset ) if ( kCurrent_Particle.id() == kParticleID ) kPartVec.push_back(iParticle);
            //! Multiplicity estimation
            if ( !kCurrent_Particle.isFinal() )         continue;
            if ( !kCurrent_Particle.isCharged() )       continue;
            if ( knLeadingParticle < 0 && kCurrent_Particle.pT() > 5. ) knLeadingParticle = iParticle;
            if ( knLeadingParticle > 0 ) if ( kCurrent_Particle.pT() > pythia.event[knLeadingParticle].pT() ) knLeadingParticle = iParticle;
            if ( fabs(kCurrent_Particle.eta()) < 0.5 )  kEventDataset["nMult05"]++;
            if ( fabs(kCurrent_Particle.eta()) < 0.8 )  kEventDataset["nMult08"]++;
            if ( fabs(kCurrent_Particle.eta()) < 1.0 )  kEventDataset["nMult10"]++;
            for ( auto& [key,value] : kEventDataset ) kEventStatistics[key]->Fill(value);
        }
        for ( auto& [ kParticleID, kPartVec ] : kParticleDataset ) {
            for ( auto iParticle : kPartVec ) {
                const auto iCurrent_Particle = pythia.event[iParticle];
                if ( fabs( iCurrent_Particle.y() ) < 0.5 ) continue;
                for ( auto& [key,value] : kEventDataset ) for ( auto& [ kParticleID, kPartVec ] : kParticleDataset ) kParticle1DStats[{kParticleID,key}]->Fill(value);
                kParticle1DStats[{kParticleID,"PtSpectrum1D"}]->Fill(iCurrent_Particle.pT());
                for ( auto jParticle : kPartVec ) {
                    if ( iParticle == jParticle ) continue;
                    const auto jCurrent_Particle = pythia.event[jParticle];
                    if ( fabs( jCurrent_Particle.y() ) < 0.5 ) continue;
                    kParticle2DStats[{kParticleID,"PtSpectrum2D"}]->Fill(iCurrent_Particle.pT(),jCurrent_Particle.pT(),0.5);
                    auto hDeltaPhiParticle = iCurrent_Particle.phi()-jCurrent_Particle.phi();
                    hDeltaPhiParticle = hDeltaPhiParticle < -0.5*3.14159265358979323846 ? hDeltaPhiParticle + 2*3.14159265358979323846 : hDeltaPhiParticle;
                    hDeltaPhiParticle = hDeltaPhiParticle > +1.5*3.14159265358979323846 ? hDeltaPhiParticle - 2*3.14159265358979323846 : hDeltaPhiParticle;
                    kParticle1DStats[{kParticleID,"hDeltaPhiPar"}]->Fill(hDeltaPhiParticle,0.5);
                    if ( knLeadingParticle < 0 ) continue;
                    const auto kLeading_Particle = pythia.event[knLeadingParticle];
                    auto hDeltaPhiLeadPar1 = kLeading_Particle.phi()-iCurrent_Particle.phi();
                    hDeltaPhiLeadPar1 = hDeltaPhiLeadPar1 < -0.5*3.14159265358979323846 ? hDeltaPhiLeadPar1 + 2*3.14159265358979323846 : hDeltaPhiLeadPar1;
                    hDeltaPhiLeadPar1 = hDeltaPhiLeadPar1 > +1.5*3.14159265358979323846 ? hDeltaPhiLeadPar1 - 2*3.14159265358979323846 : hDeltaPhiLeadPar1;
                    auto hDeltaPhiLeadPar2 = kLeading_Particle.phi()-jCurrent_Particle.phi();
                    hDeltaPhiLeadPar2 = hDeltaPhiLeadPar2 < -0.5*3.14159265358979323846 ? hDeltaPhiLeadPar2 + 2*3.14159265358979323846 : hDeltaPhiLeadPar2;
                    hDeltaPhiLeadPar2 = hDeltaPhiLeadPar2 > +1.5*3.14159265358979323846 ? hDeltaPhiLeadPar2 - 2*3.14159265358979323846 : hDeltaPhiLeadPar2;
                    kParticle2DStats[{kParticleID,"hDeltaPhiLPr"}]->Fill(hDeltaPhiLeadPar1,hDeltaPhiLeadPar2,0.5);
                }
            }
        }
    }
    //! Save production settings for later
    SetkEventStatistics(kEventStatistics,nSeed,nEvents,fOption,kEnergy,kBeamidA,kBeamidB);
    //! Save output
    for ( auto& [key,value] : kEventStatistics ) value->Write();
    for ( auto& [key,value] : kParticle1DStats ) value->Write();
    for ( auto& [key,value] : kParticle2DStats ) value->Write();
    outFile     ->Close();
    return 0;
}
