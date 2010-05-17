//--------------------------------------------------------------------------
//
// PythiaWrapper.cc
// Author:  Lynn Garren
//
// ----------------------------------------------------------------------

#include <cmath>

#include "HepMC/PythiaWrapper.h"
#include "HepMC/GenCrossSection.h"

#ifdef _WIN32 // Platform: Windows MS Visual C++

#else // Generic version, tested on Linux ecgs/gcc

// declare the structs here to keep the shared library happy
struct pin3 pyint3_;
struct pin5 pyint5_;
struct pin7 pyint7_;
struct pin8 pyint8_;
struct pin9 pyint9_;
struct pssm pyssm_;
struct prvnv pyrvnv_;
struct prvpm pyrvpm_;

#endif // Platform

namespace HepMC {

HepMC::GenCrossSection getPythiaCrossSection() {

  HepMC::GenCrossSection xsec;
  // xsec(0,2) contains the sum of differential cross sections in mb
  // ngen(0,2) contains the combined number of generated events
  // convert to pb (HepMC convention)
  double xsecval = pyint5.xsec[2][0] * 1.0e9;
  // statistical error
  double xsecerr = xsecval / std::sqrt( (double)pyint5.ngen[2][0] );
  // set and return cross section information
  xsec.set_cross_section(xsecval, xsecerr);
  return xsec;
}

} // HepMC
