//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, January 2007
// This example is an extension of example_MyPythia.cc
//  
// generate events with Pythia, write a file, and read the resulting output
// Notice that we use scope to explicitly close the ouput files.
// The two output files should be the same size, but because particles are
// saved as sets within a vertex, they will be written in arbitrary order.
//////////////////////////////////////////////////////////////////////////
// To Compile: go to the HepMC directory and type:
// gmake examples/example_MyPythiaRead.exe
//
// In this example the precision and number of entries for the HEPEVT 
// fortran common block are explicitly defined to correspond to those 
// used in the Pythia version of the HEPEVT common block. 
//
// If you get funny output from HEPEVT in your own code, probably you have
// set these values incorrectly!
//

#include <iostream>
#include "HepMC/PythiaWrapper.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/IO_Ascii.h"
#include "HepMC/GenEvent.h"
#include "PythiaHelper.h"

int main() { 
    //
    //........................................HEPEVT
    // Pythia 6.3 uses HEPEVT with 4000 entries and 8-byte floating point
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
    //........................................define the output scope
    {
	// Instantial an IO strategy to write the data to file - it uses the 
	//  same ParticleDataTable
	HepMC::IO_Ascii ascii_io("example_MyPythiaRead.dat",std::ios::out);
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
	    // write the event out to the ascii file
	    ascii_io << evt;
	    // we also need to delete the created event from memory
	    delete evt;
	}
	//........................................TERMINATION
	// write out some information from Pythia to the screen
	call_pystat( 1 );    
    }  // ascii_io destructor is called here
    //
    //........................................define an input scope
    {
	// now read the file we wrote
	HepMC::IO_Ascii ascii_in("example_MyPythiaRead.dat",std::ios::in);
	HepMC::IO_Ascii ascii_io2("example_MyPythiaRead2.dat",std::ios::out);
	int icount=0;
	HepMC::GenEvent* evt = ascii_in.read_next_event();
	while ( evt ) {
            icount++;
            if ( icount%50==1 ) std::cout << "Processing Event Number " << icount
                                	  << " its # " << evt->event_number() 
                                	  << std::endl;
	    // write the event out to the ascii file
	    ascii_io2 << evt;
            delete evt;
            ascii_in >> evt;
	}
	//........................................PRINT RESULT
	std::cout << icount << " events found. Finished." << std::endl;
    } // ascii_io2 and ascii_in destructors are called here

    return 0;
}


 
