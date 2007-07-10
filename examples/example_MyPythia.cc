//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, December 1999
// November 2000, updated to use Pythia 6.1
// example of generating events with Pythia
// using HepMC/PythiaWrapper.h 
// Events are read into the HepMC event record from the FORTRAN HEPEVT 
// common block using the IO_HEPEVT strategy and then output to file in
// ascii format using the IO_Ascii strategy.
//////////////////////////////////////////////////////////////////////////
/// To Compile: go to the HepMC directory and type:
/// gmake examples/example_MyPythia.exe
///
/// In this example the precision and number of entries for the HEPEVT 
/// fortran common block are explicitly defined to correspond to those 
/// used in the Pythia version of the HEPEVT common block. 
///
/// If you get funny output from HEPEVT in your own code, probably you have
/// set these values incorrectly!
///

#include <iostream>
#include "HepMC/PythiaWrapper.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/IO_Ascii.h"
#include "HepMC/IO_ExtendedAscii.h"
#include "HepMC/GenEvent.h"
#include "PythiaHelper.h"

int main() { 
    //
    //........................................HEPEVT
    // Pythia 6.1 uses HEPEVT with 4000 entries and 8-byte floating point
    //  numbers. We need to explicitly pass this information to the 
    //  HEPEVT_Wrapper.
    //
    HepMC::HEPEVT_Wrapper::set_max_number_entries(4000);
    HepMC::HEPEVT_Wrapper::set_sizeof_real(8);
    //
    //........................................PYTHIA INITIALIZATIONS
    initPythia();

    //........................................HepMC INITIALIZATIONS
    //
    // Instantiate an IO strategy for reading from HEPEVT.
    HepMC::IO_HEPEVT hepevtio;
    //
    { // begin scope of ascii_io
	// Instantiate an IO strategy to write the data to file 
	HepMC::IO_Ascii ascii_io("example_MyPythia.dat",std::ios::out);
	// declare an IO_ExtendedAscii for output
	HepMC::IO_ExtendedAscii xout("example_MyPythia.exdat",std::ios::out);
	//
	//........................................EVENT LOOP
	for ( int i = 1; i <= 100; i++ ) {
	    if ( i%50==1 ) std::cout << "Processing Event Number " 
				     << i << std::endl;
	    call_pyevnt();      // generate one event with Pythia
	    // pythia pyhepc routine converts common PYJETS in common HEPEVT
	    call_pyhepc( 1 );
	    HepMC::GenEvent* evt = hepevtio.read_next_event();
	    // add some information to the event
	    evt->set_event_number(i);
	    evt->set_signal_process_id(20);
	    // set number of multi parton interactions
	    evt->set_mpi( pypars.msti[31-1] );
	    // write the event out to the ascii files
	    ascii_io << evt;
	    xout << evt;
	    // we also need to delete the created event from memory
	    delete evt;
	}
	//........................................TERMINATION
	// write out some information from Pythia to the screen
	call_pystat( 1 );    
    } // end scope of ascii_io

    return 0;
}


 
