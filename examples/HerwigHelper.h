#ifndef HERWIG_HELPER_H
#define HERWIG_HELPER_H
//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, January 2010
//
// definitions needed by the Herwig examples
//
//////////////////////////////////////////////////////////////////////////
#include "HepMC/HerwigWrapper.h"
#include "HepMC/GenCrossSection.h"

/// calculate the Herwig cross section and statistical error
inline HepMC::GenCrossSection getHerwigCrossSection(int ngen) {

  HepMC::GenCrossSection xsec;
  // set cross section information and convert to pb (HepMC convention)
  double xsecval = hwevnt.AVWGT * 1000.0;
  // statistical error
  // Herwig has a better calculation of the error, 
  // but that information does not appear to be saved anywhere
  double xsecerr = xsecval / std::sqrt(ngen);
  // set and return cross section information
  xsec.set_cross_section(xsecval, xsecerr);
  return xsec;
}

#endif  // HERWIG_HELPER_H
