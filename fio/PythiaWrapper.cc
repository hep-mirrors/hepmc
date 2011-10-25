//--------------------------------------------------------------------------
//
// PythiaWrapper.cc
// Author:  Lynn Garren
//
// ----------------------------------------------------------------------

#include <cmath>

#include "HepMC/PythiaWrapper.h"
#include "HepMC/GenCrossSection.h"

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
