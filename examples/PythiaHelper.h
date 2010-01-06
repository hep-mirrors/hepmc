#ifndef PYTHIA_HELPER_H
#define PYTHIA_HELPER_H
//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, April 2007
//
// definitions needed by the pythia examples
//
//////////////////////////////////////////////////////////////////////////
#include "HepMC/PythiaWrapper.h"
#include "HepMC/GenCrossSection.h"

void initPythia();


/// calculate the Pythia cross section and statistical error
inline HepMC::GenCrossSection getPythiaCrossSection() {

  HepMC::GenCrossSection xsec;
  // xsec(0,2) contains the sum of differential cross sections in mb
  // ngen(0,2) contains the combined number of generated events
  // convert to pb (HepMC convention)
  double xsecval = pyint5.xsec[2][0] * 1.0e9;
  // statistical error
  double xsecerr = xsecval / std::sqrt( pyint5.ngen[2][0] );
  // set and return cross section information
  xsec.set_cross_section(xsecval, xsecerr);
  return xsec;
}

#endif  // PYTHIA_HELPER_H
