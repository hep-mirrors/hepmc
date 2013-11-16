#ifndef HEPMC_VERSION_H
#define HEPMC_VERSION_H

// ----------------------------------------------------------------------
//
// Version.h
// Author:  Lynn Garren
//
//  for now, these are free functions
//
// ----------------------------------------------------------------------

#include <string>
#include <iostream>
#include "HepMC/HepMCDefs.h"

namespace HepMC {

  /// Return HepMC version string
  inline std::string versionName( ) {
    return HEPMC_VERSION;
  }

  /// Print HepMC version (to optional ostream)
  inline void version( std::ostream & os = std::cout ) {
    os << " --------------- HepMC Version " << versionName() << " --------------- " << std::endl;
  }

  /// Write HepMC version to an ostream
  inline void writeVersion( std::ostream & os ) {
    /// @todo Why the leading spaces?
    os << "             HepMC Version: " << versionName() << std::endl;
  }

} // HepMC

#endif // HEPMC_VERSION_H
