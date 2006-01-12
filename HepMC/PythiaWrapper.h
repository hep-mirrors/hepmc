//--------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, November 2000
// Just a link to whichever pythia version is current.
//////////////////////////////////////////////////////////////////////////

// This pre-compiler directive is included (2002-01-16) to allow compatibility
// with MS Visual C++, which interfaces to fortran in a different manner.
// For it to work you need to define the _WIN32 variable when compiling.
#ifdef _WIN32 // Platform: Windows MS Visual C++
#include "HepMC/PythiaWrapper6_2_WIN32.h"

#else // Generic version, tested on Linux ecgs/gcc
#include "HepMC/PythiaWrapper6_2.h"

#endif // Platform

//--------------------------------------------------------------------------
