//--------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, November 200July 2002
// Just a link to whichever herwig version is current.
//////////////////////////////////////////////////////////////////////////

// This pre-compiler directive is included (2002-01-16) to allow compatibility
// with MS Visual C++, which interfaces to fortran in a different manner.
// For it to work you need to define the _WIN32 variable when compiling.
#ifdef _WIN32 // Platform: Windows MS Visual C++

//  Sorry, there is NO version currently available for Vusual C++.
//  If you need it, please complain to Matt.Dobbs@cern.ch

#else // Generic version, tested on Linux ecgs/gcc
#include "HepMC/HerwigWrapper6_4.h"

#endif // Platform

//--------------------------------------------------------------------------
