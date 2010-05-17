//--------------------------------------------------------------------------
#ifndef PYTHIA_WRAPPER_H
#define PYTHIA_WRAPPER_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, November 2000
// Just a link to whichever pythia version is current.
//////////////////////////////////////////////////////////////////////////

// This pre-compiler directive is included (2002-01-16) to allow compatibility
// with MS Visual C++, which interfaces to fortran in a different manner.
// For it to work you need to define the _WIN32 variable when compiling.
#ifdef _WIN32 // Platform: Windows MS Visual C++
#include "HepMC/PythiaWrapper6_4_WIN32.h"

#else // Generic version, tested on Linux ecgs/gcc
#include "HepMC/PythiaWrapper6_4.h"

#endif // Platform

#include "HepMC/GenCrossSection.h"

namespace HepMC {

/// calculate the Pythia cross section and statistical error
GenCrossSection getPythiaCrossSection();

} // HepMC

#endif  // PYTHIA_WRAPPER_H
//--------------------------------------------------------------------------
