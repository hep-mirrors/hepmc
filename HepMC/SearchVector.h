#ifndef HEPMC_SearchVector_h
#define HEPMC_SearchVector_h

// ----------------------------------------------------------------------
//
// SearchVector.h
// Author: Lynn Garren
//
// Utilities to search std::vector<GenParticle*> a GenParticle instance
// ----------------------------------------------------------------------

#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

/// @todo Hide these in the implementation, not in the public interface
namespace HepMC {

  /// Return true if it cannot find GenParticle* in the vector
  bool not_in_vector( std::vector<HepMC::GenParticle*>*, GenParticle* );

  /// Return the index of a GenParticle* within a vector
  ///
  /// Returns -1 if GenParticle* is not in the vector.
  std::vector<HepMC::GenParticle*>::iterator already_in_vector( std::vector<HepMC::GenParticle*>*, GenParticle* );

} // HepMC

#endif // HEPMC_SearchVector_h
