#ifndef HEPMC_MOMENTUM_UNITS_H
#define HEPMC_MOMENTUM_UNITS_H

//--------------------------------------------------------------------------
// MomentumUnits.h
// Author:  Lynn Garren
//
// momentum units used by a GenEvent
//--------------------------------------------------------------------------

#include <string>

namespace HepMC {

//! The MomentumUnits class contains the momentum units used by a GenEvent

///
/// \class MomentumUnits 
/// HepMC::MomentumUnits contains information about momentum units
/// and associated methods.
///
class MomentumUnits {

public:

enum HepMCmomentumUnits { unknown = 0, eV, keV, MeV, GeV, TeV };
// this enum scheme can accomodate additional definitions

    MomentumUnits( HepMCmomentumUnits mom = unknown ); //!< default constructor
    ~MomentumUnits() {}

    MomentumUnits( const MomentumUnits& inevent );            //!< copy
    MomentumUnits& operator=( const MomentumUnits& inevent ); //!< copy
    MomentumUnits& operator=( const HepMCmomentumUnits ); //!< copy
    void swap( MomentumUnits & other );                       //!< swap

    // accessors
    HepMCmomentumUnits units()      const;   //!< return momentum unit enum
    std::string        name()       const;   //!< return unit name
    double             unitFactor() const;   //!< scaling factor relative to eV
    double             conversion_factor( HepMCmomentumUnits ) const; //!< return a conversion multiplier

    // mutators
    bool set_units  ( HepMCmomentumUnits ); //!< set the momentum units
    bool set_units  ( std::string& );       //!< set the momentum units

private: // data members
    HepMCmomentumUnits m_units;    // unknown by default
};

//--------------------------------------------------------------------------
// INLINE Methods

inline MomentumUnits::MomentumUnits( HepMCmomentumUnits mom )
: m_units( mom ) {}

inline MomentumUnits::MomentumUnits( const MomentumUnits& inevent )
: m_units( inevent.units() ) {}

inline MomentumUnits& MomentumUnits::operator=( const MomentumUnits& inevent )
{
    /// best practices implementation
    MomentumUnits tmp( inevent );
    swap( tmp );
    return *this;
}

inline MomentumUnits& MomentumUnits::operator=( const HepMCmomentumUnits u )
{
    /// best practices implementation
    MomentumUnits tmp( u );
    swap( tmp );
    return *this;
}

inline void MomentumUnits::swap( MomentumUnits & other )
{ std::swap( m_units, other.m_units ); }

inline MomentumUnits::HepMCmomentumUnits MomentumUnits::units() const
{ return m_units; }

inline bool MomentumUnits::set_units( HepMCmomentumUnits mom )
{
   /// set_units will FAIL if the units are already defined
   if( m_units != unknown ) return false;
   m_units = mom;
   return true;
}

inline bool MomentumUnits::set_units( std::string& mom )
{
   /// set_units will FAIL if the units are already defined
   if( m_units != unknown ) return false;
   if( mom == "eV"     )       m_units = eV;
   else if( mom == "keV"     ) m_units = keV;
   else if( mom == "MeV"     ) m_units = MeV;
   else if( mom == "GeV"     ) m_units = GeV;
   else if( mom == "TeV"     ) m_units = TeV;
   else if( mom == "unknown" ) m_units = unknown;
   else return false;
   return true;
}

inline double MomentumUnits::conversion_factor( HepMCmomentumUnits mom ) const
{
   /// This routine returns the desired scale factor
   /// If momentum units are unknown, the scale factor is 0.
   if( m_units == unknown ) return 0.;
   if( m_units == mom ) return 1.;
   double f = MomentumUnits(mom).unitFactor();
   return  f < 0. ? 0. : unitFactor()/f;
}

inline double MomentumUnits::unitFactor() const
{
    switch( m_units ) {
	case eV      : return 1.;
	case keV     : return 1000.;
	case MeV     : return 1000000.;
	case GeV     : return 1000000000.;
	case TeV     : return 1000000000000.;
	default      : return -1.;
    }
}

inline std::string MomentumUnits::name() const
{
    switch( m_units ) {
	case eV      : return "eV";
	case keV     : return "keV";
	case MeV     : return "MeV";
	case GeV     : return "GeV";
	case TeV     : return "TeV";
	case unknown : return "unknown";
	default      : return "badValue";
    }
}

} // HepMC

#endif  // HEPMC_MOMENTUM_UNITS_H
