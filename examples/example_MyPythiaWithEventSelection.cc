//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, December 1999
// November 2000, updated to use Pythia 6.1
// example of generating events with Pythia
// using HepMC/PythiaWrapper.h 
// Events are read into the HepMC event record from the FORTRAN HEPEVT 
// common block using the IO_HEPEVT strategy and then a very simple event
// selection is performed.
//////////////////////////////////////////////////////////////////////////
// To Compile: go to the HepMC directory and type:
// gmake examples/example_MyPythiaWithEventSelection.exe
//
// See comments in examples/example_MyPythia.cxx regarding the HEPEVT wrapper.
//

#include <iostream>
#include "HepMC/PythiaWrapper.h"
#include "HepMC/IO_HEPEVT.h"
#include "HepMC/GenEvent.h"
#include "PythiaHelper.h"


//! example class

/// \class  IsGoodEventMyPythia
/// event selection predicate. returns true if the event contains
/// a photon with pT > 25 GeV
class IsGoodEventMyPythia {
public:
    /// returns true if event is "good"
    bool operator()( const HepMC::GenEvent* evt ) { 
	for ( HepMC::GenEvent::particle_const_iterator p 
		  = evt->particles_begin(); p != evt->particles_end(); ++p ){
	    if ( (*p)->pdg_id() == 22 && (*p)->momentum().perp() > 25. ) {
		//std::cout << "Event " << evt->event_number()
		//     << " is a good event." << std::endl;
		//(*p)->print();
		return 1;
	    }
	}
	return 0;
    }
};
    
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
    //
    //........................................HepMC INITIALIZATIONS
    // Instantiate an IO strategy for reading from HEPEVT.
    HepMC::IO_HEPEVT hepevtio;
    // declare an instance of the event selection predicate
    IsGoodEventMyPythia is_good_event;
    //........................................EVENT LOOP
    int icount=0;
    int num_good_events=0;
    for ( int i = 1; i <= 100; i++ ) {
	icount++;
	if ( i%50==1 ) std::cout << "Processing Event Number " 
				 << i << std::endl;
	call_pyevnt(); // generate one event with Pythia
	// pythia pyhepc routine convert common PYJETS in common HEPEVT
	call_pyhepc( 1 );
	HepMC::GenEvent* evt = hepevtio.read_next_event();
	// set number of multi parton interactions
	evt->set_mpi( pypars.msti[31-1] );
	// do event selection
	if ( is_good_event(evt) ) ++num_good_events;
	// we also need to delete the created event from memory
	delete evt;
    }
    //........................................TERMINATION
    // write out some information from Pythia to the screen
    call_pystat( 1 );    
    //........................................PRINT RESULTS
    std::cout << num_good_events << " out of " << icount 
	      << " processed events passed the cuts. Finished." << std::endl;
    return 0;
}


 
