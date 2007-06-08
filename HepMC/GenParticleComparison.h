// ----------------------------------------------------------------------
//
// GenParticleComparison.hh
// Author: Lynn Garren
//
//  Usage: std::set<GenParticle*,GenParticleComparison> 
// ----------------------------------------------------------------------
#ifndef GenParticleComparison_h
#define GenParticleComparison_h

#include "HepMC/GenParticle.h"
#include <iostream>

namespace HepMC {
  
//! The GenParticleComparison class provides a utility for sorting GenParticle pointers.

///
/// \class GenParticleComparison
/// \author Lynn Garren
///
///  We'd like to use the barcode, but that is not always created when 
///  a GenParticle is created.  So, we create our own unique serialnumber 
///  which is based on the counter value when GenParticle is created.
///  Hum - possibly still has a problem since destructor decrements counter.
///
class GenParticleComparison  {

public:
  /// stateless class
  GenParticleComparison( ) {;}
  
  /// Comparison method sorts by serialnumber.
  bool operator() ( const GenParticle*, const GenParticle* ) const;

};  // GenParticleComparison


inline bool GenParticleComparison::operator() ( const GenParticle* p1, const GenParticle* p2 ) const
{
    // we count GenParticles as they are created and use that count
    // to set a unique serialnumber()
    return p1->serialnumber() < p2->serialnumber();
}


}	// HepMC

#endif // GenParticleComparison_h
