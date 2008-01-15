//////////////////////////////////////////////////////////////////////////
// testHerwigCopies.cc
//
// garren@fnal.gov, January 2008
// Multiple events in memory at the same time
// run with valgrind or some other leak checker
//////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include "HepMC/HerwigWrapper.h"
#include "HepMC/IO_HERWIG.h"
#include "HepMC/GenEvent.h"
#include "HepMC/CompareGenEvent.h"
#include "HepMC/HEPEVT_Wrapper.h"

int main() { 
    //
    //........................................HEPEVT
    // Herwig 6.4 uses HEPEVT with 4000 entries and 8-byte floating point
    //  numbers. We need to explicitly pass this information to the 
    //  HEPEVT_Wrapper.
    //
    HepMC::HEPEVT_Wrapper::set_max_number_entries(4000);
    HepMC::HEPEVT_Wrapper::set_sizeof_real(8);
    //
    //.......................................INITIALIZATIONS

    hwproc.PBEAM1 = 7000.; // energy of beam1
    hwproc.PBEAM2 = 7000.; // energy of beam2
    // 1610 = gg->H--> WW, 1706 = qq-->ttbar, 2510 = ttH -> ttWW
    hwproc.IPROC = 1706; // qq -> ttbar production 
    hwproc.MAXEV = 100; // number of events
    // tell it what the beam particles are:
    for ( unsigned int i = 0; i < 8; ++i ) {
        hwbmch.PART1[i] = (i < 1) ? 'P' : ' ';
        hwbmch.PART2[i] = (i < 1) ? 'P' : ' ';
    }
    hwigin();    // INITIALISE OTHER COMMON BLOCKS
    hwevnt.MAXPR = 1; // number of events to print
    hwuinc(); // compute parameter-dependent constants
    hweini(); // initialise elementary process

    //........................................HepMC INITIALIZATIONS
    //
    // Instantiate an IO strategy for reading from HEPEVT.
    HepMC::IO_HERWIG hepevtio;
    //
    // open some output files
    std::ofstream out1( "testHerwigOriginals.dat" );
    std::ofstream out2( "testHerwigCopies1.dat" );
    std::ofstream out3( "testHerwigCopies2.dat" );
    //
    //........................................EVENT LOOP
    for ( int i = 1; i <= hwproc.MAXEV; i++ ) {
	if ( i%50==1 ) std::cout << "Processing Event Number " 
				 << i << std::endl;
	// initialise event
	hwuine();
	// generate hard subprocess
	hwepro();
	// generate parton cascades
	hwbgen();
	// do heavy object decays
	hwdhob();
	// do cluster formation
	hwcfor();
	// do cluster decays
	hwcdec();
	// do unstable particle decays
	hwdhad();
	// do heavy flavour hadron decays
	hwdhvy();
	// add soft underlying event if needed
	hwmevt();
	// finish event
	hwufne();
	HepMC::GenEvent* evt = hepevtio.read_next_event();
	// add some information to the event
	evt->set_event_number(i);
	evt->set_signal_process_id(20);
	if (i<=hwevnt.MAXPR) {
	    std::cout << "\n\n This is the FIXED version of HEPEVT as "
		      << "coded in IO_HERWIG " << std::endl;
	    HepMC::HEPEVT_Wrapper::print_hepevt();
	    evt->print();
	}
	//
	//.......................make some copies
	evt->print(out1);
	HepMC::GenEvent ec = (*evt);
	ec.print(out2);
	HepMC::GenEvent* evt4 = new HepMC::GenEvent(*evt);
	evt4->print(out3);
 	if( !compareGenEvent(evt,evt4) ) { 
	   std::cerr << "GenEvent comparison fails" << std::endl;
	   return -1; 
	}

	// we also need to delete the created event from memory
	delete evt;
        delete evt4;
    }
    //........................................TERMINATION
    hwefin();

    return 0;
}
