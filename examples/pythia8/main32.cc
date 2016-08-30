// main32.cc is a part of the PYTHIA event generator.
// Copyright (C) 2011 Mikhail Kirsanov, Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It illustrates how a file with HepMC events can be generated by Pythia8.
// Input and output files are specified on the command line, e.g. like
// ./main32.exe main32.cmnd hepmcout32.dat > out
// The main program contains no analysis; this is intended to happen later.
// It therefore "never" has to be recompiled to handle different tasks.

// WARNING: typically one needs 25 MB/100 events at the LHC.
// Therefore large event samples may be impractical.

#include "Pythia.h"
#if PYTHIA_VERSION_INTEGER > 8199
#include "HepMC2.h"
//* The include below should be used for recent Pythia8.
//* However, PYTHIA_VERSION is defined as float, so it is not that 
//* easy to work with it.
#else
#include "HepMCInterface.h"
#endif

#include "HepMC/GenEvent.h"   
#include "HepMC/IO_GenEvent.h"

// Following line is a deprecated alternative, removed in recent versions.
//#include "HepMC/IO_Ascii.h"
//#include "HepMC/IO_AsciiParticles.h"

// Following line to be used with HepMC 2.04 onwards.
#ifdef HEPMC_HAS_UNITS
#include "HepMC/Units.h"
#endif

using namespace Pythia8; 

int main(int argc, char* argv[]) {

  // Check that correct number of command-line arguments
  if (argc != 3) {
    cerr << " Unexpected number of command-line arguments. \n You are"
         << " expected to provide one input and one output file name. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Check that the provided input name corresponds to an existing file.
  ifstream is(argv[1]);  
  if (!is) {
    cerr << " Command-line file " << argv[1] << " was not found. \n"
         << " Program stopped! " << endl;
    return 1;
  }

  // Confirm that external files will be used for input and output.
  cout << " PYTHIA settings will be read from file " << argv[1] << endl;
  cout << " HepMC events will be written to file " << argv[2] << endl;

  // Interface for conversion from Pythia8::Event to HepMC one. 
#if PYTHIA_VERSION_INTEGER > 8199
  HepMC::Pythia8ToHepMC ToHepMC;
#else  
  HepMC::I_Pythia8 ToHepMC;
#endif 
  //  ToHepMC.set_crash_on_problem();

  // Specify file where HepMC events will be stored.
  HepMC::IO_GenEvent ascii_io(argv[2], std::ios::out);
  // Following line is a deprecated alternative, removed in recent versions
  // HepMC::IO_Ascii ascii_io("hepmcout32.dat", std::ios::out);
  // Line below is an eye-readable one-way output, uncomment the include above
  // HepMC::IO_AsciiParticles ascii_io("hepmcout32.dat", std::ios::out);
 
  // Generator. 
  Pythia pythia;

  // Read in commands from external file.
  pythia.readFile(argv[1]);    

  // Extract settings to be used in the main program.
  int    nEvent    = pythia.mode("Main:numberOfEvents");
  int    nShow     = pythia.mode("Main:timesToShow");
  int    nAbort    = pythia.mode("Main:timesAllowErrors");
  bool   showCS    = pythia.flag("Main:showChangedSettings");
  bool   showAS    = pythia.flag("Main:showAllSettings");
  bool   showCPD   = pythia.flag("Main:showChangedParticleData");
  bool   showAPD   = pythia.flag("Main:showAllParticleData");
 
  // Initialization. Beam parameters set in .cmnd file.
  pythia.init();

  // List settings.
  if (showCS) pythia.settings.listChanged();
  if (showAS) pythia.settings.listAll();

  // List particle data.  
  if (showCPD) pythia.particleData.listChanged();
  if (showAPD) pythia.particleData.listAll();

  // Begin event loop.
  int nPace  = max(1, nEvent / max(1, nShow) ); 
  int iAbort = 0; 
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if (nShow > 0 && iEvent%nPace == 0) 
      cout << " Now begin event " << iEvent << endl;

    // Generate event. 
    if (!pythia.next()) {

      // If failure because reached end of file then exit event loop.
      if (pythia.info.atEndOfFile()) {
        cout << " Aborted since reached end of Les Houches Event File\n"; 
        break; 
      }

      // First few failures write off as "acceptable" errors, then quit.
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error!\n"; 
      break;
    }

    // Construct new empty HepMC event. 
#ifdef HEPMC_HAS_UNITS
    // This form with arguments is only meaningful for HepMC 2.04 onwards, 
    // and even then unnecessary if HepMC was built with GeV and mm as units..
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent( 
      HepMC::Units::GEV, HepMC::Units::MM);
#else
    // This form is needed for backwards compatibility. 
    // In HepMCInterface.cc a conversion from GeV to MeV will be done.
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
#endif

    // Fill HepMC event, including PDF info.
    ToHepMC.fill_next_event( pythia, hepmcevt );
    // This alternative older method fills event, without PDF info.
    // ToHepMC.fill_next_event( pythia.event, hepmcevt );

    // Write the HepMC event to file. Done with it.
    ascii_io << hepmcevt;
    delete hepmcevt;

  // End of event loop. Statistics. 
  }
#if PYTHIA_VERSION_INTEGER > 8199
  pythia.stat();
#else  
  pythia.statistics();
#endif 

  // Done.
  return 0;
}
