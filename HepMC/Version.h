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

namespace HepMC {

void version( );			//!< print HepMC version
void writeVersion( std::ostream & os );	//!< write HepMC version to os
std::string versionName( );	//!< return HepMC version

inline std::string versionName( )
{
    return "2.01.08";
}

inline void version( )
{
    std::cout << " --------------- HepMC Version " << versionName()
              << " --------------- " << std::endl;
}

inline void writeVersion( std::ostream & os )
{
    os << "             HepMC Version: " << versionName() << std::endl;
}

}	// HepMC

#endif // HEPMC_VERSION_H
