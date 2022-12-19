// Pythia
#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"
#include "TBenchmark.h"
#include "TFile.h"
#include "TMath.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TH1F.h"
#include "TTree.h"
//
using namespace std;
using namespace ROOT;
using namespace Pythia8;
//
const auto fMinIMMC = 0.75;
const auto fMaxIMMC = 1.25;
//  RopeFragPars::
// Initial step size for a calculation.
const double DELTAA = 0.1;
// Convergence criterion for a calculation.
const double ACONV = 0.001;
// Low z cut-off in fragmentation function.
const double ZCUT = 1.0e-4;
//
TBenchmark         *uBenchmark                      =   new TBenchmark();
TString                 kMSG_PrintTimer             =   "\r[INFO] [%s] Event # %7.f %s | %3.1f %% | %7.2f %s events/s | Time: %02.0f:%02.0f:%02.0f | ETA: %02.0f:%02.0f:%02.0f [%s]";
//
// Check flag  StringFragmentation:TraceColours
//                                                                                                                                                  --- BENCHMARK UTILITIES
//---------------------------------------------------------------------------------------------------------------------------------------------------
void
fStartTimer
 ( TString fTimerName )    {
    uBenchmark->Reset();
    uBenchmark->Start(fTimerName.Data());
    printf("[INFO] Starting %s \n", fTimerName.Data());
    fflush(stdout);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
void
fStopTimer
 ( TString fTimerName )     {
    uBenchmark->Stop(fTimerName.Data());
    cout << endl;
    printf("[INFO] Stopping %s \n", fTimerName.Data());
    Float_t fElapsedS   = (float)(uBenchmark->GetRealTime(fTimerName.Data()));
    Float_t fElapsedM   = (Int_t)(fElapsedS/60.);
    printf("[INFO] It took %02.0f:%02.0f \n",   fElapsedM,  fElapsedS - 60.*fElapsedM);
    fflush(stdout);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
TString
uCurrentDateTimeOffset
( Int_t fOffset ) {
    time_t     now = time(0) + fOffset;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
    return TString(buf);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
//  TODO: pass stringfromat from outside
TString
uCurrentDateTime
() {
    return uCurrentDateTimeOffset(0);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
void
fPrintLoopTimer
 ( TString fTimerName, Int_t iEvent, Int_t nEntries, Int_t iPrintInterval )   {
    if ( iEvent%iPrintInterval != 0 || iEvent == 0 ) return;
    
    // Suffix for events
    TString     fSuffix =   "";
    Int_t       fSfxCor =   iPrintInterval;
    if ( iPrintInterval/1e3 < 1e3         && iPrintInterval/1e3 >= 1 )       {
        fSuffix =   "k";
        fSfxCor =   (int)(iPrintInterval/1e3) + iPrintInterval%(int)1e3;
    }
    if ( iPrintInterval/1e6 < 1e6         && iPrintInterval/1e6 >= 1 )       {
        fSuffix =   "mln";
        fSfxCor =   (int)(iPrintInterval/1e6) + iPrintInterval%(int)1e6;
    }
    if ( iPrintInterval/1e9 < 1e9         && iPrintInterval/1e9 >= 1 )       {
        fSuffix =   "mld";
        fSfxCor =   (int)(iPrintInterval/1e9) + iPrintInterval%(int)1e9;
    }
    
    // Stopping timer
    uBenchmark->Stop(fTimerName.Data());

    //
    //  Elapsed Time
    Float_t fRealElapsedSec =   (float)(uBenchmark->GetRealTime(fTimerName.Data()));
    Float_t fRealElapsedMin =   (Int_t)(fRealElapsedSec/60.);
    Float_t fRealElapsedHor =   (Int_t)(fRealElapsedSec/3600.);
    //
    Float_t fShowElapsedSec =   fRealElapsedSec - fRealElapsedMin*60.;
    Float_t fShowElapsedMin =   fRealElapsedMin - fRealElapsedHor*60.;
    Float_t fShowElapsedHor =   fRealElapsedHor;
    
    //
    //  Event utilities
    Float_t fProcessedFrac  =   (float)iEvent/((float)nEntries);
    Float_t fShowPrintEvnt  =   (float)iEvent*(float)fSfxCor/((float)iPrintInterval);   //TODO: Clean-Up
    Float_t fShowSpeedEvnt  =   fShowPrintEvnt/fRealElapsedSec;
    
    //
    //  ETA
    Float_t fRealEstimatSec =   fRealElapsedSec/fProcessedFrac - fRealElapsedSec;
    Float_t fRealEstimatMin =   (Int_t)(fRealEstimatSec/60.);
    Float_t fRealEstimatHor =   (Int_t)(fRealEstimatSec/3600.);
    //
    Float_t fShowEstimatSec =   fRealEstimatSec - fRealEstimatMin*60.;
    Float_t fShowEstimatMin =   fRealEstimatMin - fRealEstimatHor*60.;
    Float_t fShowEstimatHor =   fRealEstimatHor;
    
    
    // Resuming timer
    uBenchmark->Start(fTimerName.Data());
    
    // Printing
    cout << "\33[2K" << flush;
    cout << Form(kMSG_PrintTimer.Data(), uCurrentDateTime().Data(),  fShowPrintEvnt,  fSuffix.Data(), 100.*fProcessedFrac, fShowSpeedEvnt,  fSuffix.Data(), fShowElapsedHor, fShowElapsedMin,  fShowElapsedSec, fShowEstimatHor, fShowEstimatMin, fShowEstimatSec, uCurrentDateTimeOffset( fRealEstimatSec ).Data()) << flush;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
Float_t
uCalculateAlpha
( Float_t kRho, Float_t kX, Float_t kY ) {
    return (1+2*kRho*kX+9*kY+6*kX*kY*kRho+3*kX*kX*kY*kRho*kRho)/(2+kRho);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
//! from Ropewalk.h/cc
Double_t
fragf(double z, double a, double b, double mT2) {
  if (z < ZCUT) return 0.0;
  return pow(1 - z, a) * exp(-b * mT2 / z) / z;
}
Double_t
trapIntegrate
 ( double a, double b, double mT2, double sOld, int n ) {

  // Compute the nth correction to the integral of fragfunc between 0 and 1
  // using extended trapezoidal rule.
  if (n == 1) return 0.5 * (fragf(0.0, a, b, mT2) + fragf(1.0, a, b, mT2));
  // We want 2^(n-2) interior points (intp). Use bitwise shift to speed up.
  int intp = 1;
  intp <<= n - 2;
  double deltaz = 1.0 / double(intp);
  double z = 0.5 * deltaz;
  double sum = 0.0;
  // Do the integral.
  for (int i = 0; i < intp; ++i, z += deltaz) sum += fragf( z, a, b, mT2);
  return 0.5 * (sOld + sum / double(intp));

}
Double_t
integrateFragFun(double a, double b, double mT2) {

  // Using Simpson's rule to integrate the Lund fragmentation function.
  double nextIter, nextComb;
  double thisComb = 0.0, thisIter = 0.0;
  // The target error on the integral should never be changed.
  double error = 1.0e-2;

  // 20 is the max number of iterations, 3 is min. Should not be changed.
  for (int i = 1; i < 20; ++i) {
    nextIter = trapIntegrate( a, b, mT2, thisIter, i);
    nextComb = (4.0 * nextIter - thisIter) / 3.0;
    if (i > 3 && abs(nextComb - thisComb) < error * abs(nextComb))
      return nextComb;
    thisIter = nextIter;
    thisComb = nextComb;
  }
  return 0.0;

}
Double_t
aEffective
 (double aOrig, double thisb, double mT2, Float_t kDefaultB) {

  // Calculate initial normalization constants.
  double N    = integrateFragFun(aOrig, kDefaultB, mT2);
  double NEff = integrateFragFun(aOrig, thisb, mT2);
  int    s    = (N < NEff) ? -1 : 1;
  double da   = DELTAA;
  double aNew = aOrig - s * da;

  // Iterate until we meet preset convergence criterion.
  do {
    // Calculate normalization with current a.
    NEff = integrateFragFun(aNew, thisb, mT2);
    if ( ((N < NEff) ? -1 : 1) != s ) {
      s = (N < NEff) ? -1 : 1;
      // If we have crossed over the solution, decrease
      // the step size and turn around.
      da /= 10.0;
    }
    aNew -= s * da;
    if (aNew < 0.0) {aNew = 0.1; break;}
    if (aNew > 2.0) {aNew = 2.0; break;}
  } while (da > ACONV);
  return aNew;

}
Float_t
uCalculateA
( Float_t kRe_CalcB, Float_t mT2, Bool_t isDiquark, Float_t kDefaultA, Float_t kDefaultAdiq, Float_t kDefaultB ) {
    return aEffective( kDefaultA + ( isDiquark? kDefaultAdiq : 0. ), kRe_CalcB, mT2, kDefaultB );
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
void
uSetStringTension
( Pythia8::Pythia &pythia, Float_t kRelativeTension = 1., Float_t kUsedBeta = 0.2, Float_t kUsedKappa = 0.2 ) {
    cout << "[INFO] Requested a custom string tension enhacnement of: " << kRelativeTension << endl;
    if ( kRelativeTension < 0. ) {
        cout << "[ERROR] Invalid query, enhancement factor must be positive. Nothing done." << endl;
        return;
    }
    if ( kRelativeTension == 1. ) {
        cout << "[WARNING] Invalid query, enhancement factor must be different than 1. Nothing done." << endl;
        return;
    }
    //! Defining h and 1/h for utility
    const Float_t kEnhancement    = kRelativeTension;
    const Float_t kInvEnhancement = 1./kRelativeTension;
    //!
    //! --- --- Standard Parameter
    //!
    //! Kappa: A base value of the string tension can be added, and modified along with other parameters, to allow for studies of exotic quark production in the Rope model.
    //! StringFlav:kappa
    const Float_t kDefaultKappa = 0.2;
    //! Rho: Suppression of s quark production relative to u or d type production
    //! StringFlav:probStoUD
    const Float_t kDefaultRho   = 0.217;
    //! Xi: Suppression of diquark production relative to quark production
    //! StringFlav:probQQtoQ
    const Float_t kDefaultXi    = 0.081;
    //! X: Suppression of strange diquark production relative to light diquark production
    //! StringFlav:probSQtoQQ
    const Float_t kDefaultX     = 0.915;
    //! Y: Suppression of spin 1 diquarks relative to spin 0 diquarks
    //! StringFlav:probQQ1toQQ0
    const Float_t kDefaultY     = 0.0275;
    //! Sigma: Width of the transverse momentum distribution in string break-ups
    //! StringPT:sigma
    const Float_t kDefaultSigma = 0.335;
    //! Alpha: Derived parameter for Xi calculation
    //! -
    const Float_t kDefaultAlpha = uCalculateAlpha( kDefaultRho, kDefaultX, kDefaultY );
    //! Beta: In the current implementation of the rope model, the theoretical ignorance about baryon production has been parameterized, assuming that the parameter StringFlav:probQQtoQ will factorize into two parts, one which will scale with effective string tension, one which will not. This parameter controls how large a fraction of the parameter will scale with string tension.
    //! Ropewalk:beta
    const Float_t kDefaultBeta  = 0.2;
    //! A: The a parameter of the Lund symmetric fragmentation function.
    //! StringZ:aLund
    const Float_t kDefaultA     = 0.680;
    //! Adiq: The a parameter of the Lund symmetric fragmentation function.
    //! StringZ:aExtraDiquark
    const Float_t kDefaultAdiq  = 0.970;
    //! B: The a parameter of the Lund symmetric fragmentation function.
    //! StringZ:bLund
    const Float_t kDefaultB     = 0.980;
    //!
    //! --- --- Re-calculated parameters
    //!
    const Float_t kRe_CalcKappa = kUsedKappa*kEnhancement;
    const Float_t kRe_CalcBeta  = kUsedBeta;
    const Float_t kRe_CalcRho   = pow( kDefaultRho,     kInvEnhancement );
    const Float_t kRe_CalcX     = pow( kDefaultX,       kInvEnhancement );
    const Float_t kRe_CalcY     = pow( kDefaultY,       kInvEnhancement );
    const Float_t kRe_CalcSigma = pow( kDefaultSigma,   kInvEnhancement );
    const Float_t kRe_CalcAlpha = uCalculateAlpha( kRe_CalcRho, kRe_CalcX, kRe_CalcY );
    const Float_t kRe_CalcXi_U  = kRe_CalcAlpha*kRe_CalcBeta*pow( kDefaultXi/(kDefaultAlpha*kRe_CalcBeta), kInvEnhancement );
    const Float_t kRe_CalcXi    = min( 1., 1.*max( kRe_CalcXi_U, kDefaultXi ) );
    const Float_t kRe_CalcB_U   = kDefaultB*(2+kRe_CalcRho)/(2+kDefaultRho);
    const Float_t kRe_CalcB     = min( 2., 1.*max( kRe_CalcB_U, kDefaultXi ) );
    const Float_t kRe_CalcA     = uCalculateA( kRe_CalcB, 1.0, false, kDefaultA, kDefaultAdiq, kDefaultB );
    const Float_t kRe_CalcAdiq  = uCalculateA( kRe_CalcB, 1.0, false, kDefaultA, kDefaultAdiq, kDefaultB ) - kRe_CalcA;
    //!
    //! --- --- Verbose info
    //!
    cout << "[WARNING] !! Preliminary implementation of custom string tension !! Please use caution and cross-check results" << endl;
    cout << "[INFO] Def.    K:"     << kUsedKappa       << "        Re-Calc. K:"    << kRe_CalcKappa    << endl;
    cout << "[INFO] Def.    Beta:"  << kDefaultBeta     << "     Re-Calc. Beta:"    << kRe_CalcBeta     << endl;
    cout << "[INFO] Def.    Rho:"   << kDefaultRho      << "      Re-Calc. Rho:"    << kRe_CalcRho      << endl;
    cout << "[INFO] Def.    X:"     << kDefaultX        << "        Re-Calc. X:"    << kRe_CalcX        << endl;
    cout << "[INFO] Def.    Y:"     << kDefaultY        << "        Re-Calc. Y:"    << kRe_CalcY        << endl;
    cout << "[INFO] Def.    Sigma:" << kDefaultSigma    << "    Re-Calc. Sigma:"    << kRe_CalcSigma    << endl;
    cout << "[INFO] Def*.   Alpha:" << kDefaultAlpha    << "    Re-Calc*.Alpha:"    << kRe_CalcAlpha    << endl;
    cout << "[INFO] Def.    Xi:"    << kDefaultXi       << "       Re-Calc. Xi:"    << kRe_CalcXi       << "    No-Cut Xi:" << kRe_CalcXi_U << endl;
    cout << "[INFO] Def.    A:"     << kDefaultA        << "        Re-Calc. A:"    << kRe_CalcA        << endl;
    cout << "[INFO] Def.    Adiq:"  << kDefaultAdiq     << "     Re-Calc. Adiq:"    << kRe_CalcAdiq     << endl;
    cout << "[INFO] Def.    B:"     << kDefaultB        << "        Re-Calc. B:"    << kRe_CalcB        << "     No-Cut B:" << kRe_CalcB_U << endl;
    //!
    //! --- --- Setting parameters
    //!
    pythia.readString(Form("StringFlav:kappa = %f",         kRe_CalcKappa));
    pythia.readString(Form("StringFlav:probStoUD = %f",     kDefaultRho));
    pythia.readString(Form("StringFlav:probQQtoQ = %f",     kDefaultXi));
    pythia.readString(Form("StringFlav:probSQtoQQ = %f",    kRe_CalcX));
    pythia.readString(Form("StringFlav:probQQ1toQQ0 = %f",  kRe_CalcY));
    pythia.readString(Form("StringPT:sigma = %f",           kRe_CalcSigma));
    pythia.readString(Form("Ropewalk:beta = %f",            kRe_CalcBeta));
    pythia.readString(Form("StringZ:aLund = %f",            kRe_CalcA));
    pythia.readString(Form("StringZ:aExtraDiquark = %f",    kRe_CalcAdiq));
    pythia.readString(Form("StringZ:bLund = %f",            kRe_CalcB));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------
//                                  // quarks
std::vector<Int_t> kStringEnds = {  1,2,3,4,5,6,7,8,
//                                  // di-quarks
                                    1103,2101,2103,2203,3101,3103,3201,3203,3303,4101,4103,4201,4203,4301,4303,4403,5101,5103,5201,5203,5301,5303,5401,5403,5503,
//                                  // gluons
                                    21
};
Bool_t
uCheckString
( Particle kTestParticle ) {
    if ( kTestParticle.mother1() > 0 && kTestParticle.mother2() > 0 && kTestParticle.mother1() < kTestParticle.mother2() && kTestParticle.statusAbs() >= 81 && kTestParticle.statusAbs() <= 86 ) return true;
    return false;
}
//
//---------------------------------------------------------------------------------------------------------------------------------------------------
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
