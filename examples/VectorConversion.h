#ifndef VECTOR_CONVERSION_H
#define VECTOR_CONVERSION_H
//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, January 2007
// Example conversion functions
// This example converts from ThreeVector and FourVector to CLHEP::HepLorentzVector
//////////////////////////////////////////////////////////////////////////

#include "HepMC/SimpleVector.h"
#include "CLHEP/Vector/LorentzVector.h"

CLHEP::HepLorentzVector SVtoLV( const HepMC::ThreeVector& v )
     { return CLHEP::HepLorentzVector( v.x(), v.y(), v.z() ); }

CLHEP::HepLorentzVector SVtoLV( const HepMC::FourVector& v )
     { return CLHEP::HepLorentzVector( v.x(), v.y(), v.z(), v.t() ); }

#endif  // VECTOR_CONVERSION_H
