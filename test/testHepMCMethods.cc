//////////////////////////////////////////////////////////////////////////
// testHepMCMethods.cc
//
// garren@fnal.gov, March 2009
//
// various methods used by the test jobs
//////////////////////////////////////////////////////////////////////////

#include "testHepMCMethods.h"

double findPiZero( HepMC::GenEvent * evt )
{
    for ( HepMC::GenEvent::particle_const_iterator p 
	      = evt->particles_begin(); p != evt->particles_end(); ++p ){
	if ( (*p)->pdg_id() == 111 ) {
	    return (*p)->generated_mass();
	}
    }
    return 0.;
}

void particleTypes( HepMC::GenEvent * evt )
{
    int numDecayed = 0, numUndecayed = 0, numBeam = 0;
    int numDecayed2 = 0, numUndecayed2 = 0, numBeam2 = 0;
    for ( HepMC::GenEvent::particle_const_iterator p 
	      = evt->particles_begin(); p != evt->particles_end(); ++p ){
	if ( (*p)->is_undecayed() ) {
	    ++numUndecayed;
	}
	if ( (*p)->has_decayed() ) {
	    ++numDecayed;
	}
	if ( (*p)->is_beam() ) {
	    ++numBeam;
	}
	if ( (*p)->status() == 1 ) {
	    ++numUndecayed2;
	}
	if ( (*p)->status() == 2 ) {
	    ++numDecayed2;
	}
	if ( (*p)->status() == 4 ) {
	    ++numBeam2;
	}
    }
    if( numUndecayed != numUndecayed2 ) {
        std::cerr << "ERROR: incorrect count of undecayed particles: "
	          << numUndecayed << " does not match " 
		  << numUndecayed2 << std::endl;
    }
    if( numDecayed != numDecayed2 ) {
        std::cerr << "ERROR: incorrect count of undecayed particles: "
	          << numDecayed << " does not match " 
		  << numDecayed2 << std::endl;
    }
    if( numBeam != numBeam2 ) {
        std::cerr << "ERROR: incorrect count of undecayed particles: "
	          << numBeam << " does not match " 
		  << numBeam2 << std::endl;
    }
    int ndcy = numUndecayed + numDecayed;
    if( ndcy > evt->particles_size() ) {
        std::cerr << "ERROR: count does not add up: "
	          << ndcy << " is greater than the number of particles in the event: " 
		  << evt->particles_size() << std::endl;
    }
    std::cout << "Event " << evt->event_number() 
	      << " has " << evt->particles_size() 
	      << " particles, " << numDecayed
	      << " decayed particles, " << numUndecayed
	      << " undecayed particles, and " << numBeam
	      << " beam particles " 
	      << std::endl;
    return;
}
