#ifndef TEST_HEPMC_METHODS_H
#define TEST_HEPMC_METHODS_H
//////////////////////////////////////////////////////////////////////////
// testHepMCMethods.h
//
// garren@fnal.gov, March 2009
//
// various methods used by the test jobs
//////////////////////////////////////////////////////////////////////////

#include "HepMC/GenEvent.h"

double findPiZero( HepMC::GenEvent * );
void   particleTypes( HepMC::GenEvent * );

#endif
