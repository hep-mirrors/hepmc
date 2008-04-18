#ifndef HEPMC_POSITION_UNITS_H
#define HEPMC_POSITION_UNITS_H

//--------------------------------------------------------------------------
// PositionUnits.h
// Author:  Lynn Garren
//
// position units used by a GenEvent
//--------------------------------------------------------------------------

#include <string>

namespace HepMC {

//! The PositionUnits class contains the position units used by a GenEvent

///
/// \class PositionUnits 
/// HepMC::PositionUnits contains information about position units
/// and associated methods.
///
class PositionUnits {
public:

/// this enum scheme can accomodate additional definitions
enum HepMCpositionUnits { UNKNOWN = 0, MM, CM };

    PositionUnits( HepMCpositionUnits pos = UNKNOWN ); //!< default constructor
    ~PositionUnits() {}

    PositionUnits( const PositionUnits& inevent );            //!< copy
    PositionUnits& operator=( const PositionUnits& inevent ); //!< copy
    void swap( PositionUnits & other );                       //!< swap

    // accessors
    HepMCpositionUnits units()      const;   //!< return position unit enum
    std::string        name()       const;   //!< return unit name
    double             unitFactor() const;   //!< scaling factor relative to mm
    double             conversion_factor( HepMCpositionUnits ) const; //!< return a conversion multiplier

    // mutators
    bool   set_units( HepMCpositionUnits ); //!< set the position units
    bool   set_units( std::string& );       //!< set the position units

private: // data members
    HepMCpositionUnits m_units;    // UNKNOWN by default
};

//--------------------------------------------------------------------------
// INLINE Methods

inline PositionUnits::PositionUnits( HepMCpositionUnits pos )
: m_units( pos ) {}

inline PositionUnits::PositionUnits( const PositionUnits& inevent )
: m_units( inevent.units() )
{}

inline PositionUnits& PositionUnits::operator=( const PositionUnits& inevent )
{
    /// best practices implementation
    PositionUnits tmp( inevent );
    swap( tmp );
    return *this;
}

inline void PositionUnits::swap( PositionUnits & other )
{ std::swap( m_units, other.m_units ); }

inline PositionUnits::HepMCpositionUnits PositionUnits::units() const
{ return m_units; }

inline bool PositionUnits::set_units( HepMCpositionUnits pos )
{
   /// set_units will FAIL if the units are already defined
   if( m_units != UNKNOWN ) return false;
   m_units = pos;
   return true;
}

inline bool PositionUnits::set_units( std::string& pos )
{
   /// set_units will FAIL if the units are already defined
   if( m_units != UNKNOWN ) return false;
   if( pos == "MM"      )      m_units = MM;
   else if( pos == "CM"      ) m_units = CM;
   else if( pos == "UNKNOWN" ) m_units = UNKNOWN;
   else return false;
   return true;
}

inline double PositionUnits::conversion_factor( HepMCpositionUnits pos ) const
{
   /// This routine returns the desired scale factor
   /// If position units are UNKNOWN, the scale factor is 0.
   if( m_units == UNKNOWN ) return 0.;
   if( m_units == pos ) return 1.;
   double f = PositionUnits(pos).unitFactor();
   return  f < 0. ? 0. : unitFactor()/f;
}

inline double PositionUnits::unitFactor() const
{
    switch( m_units ) {
        case MM      : return 1.;
        case CM      : return 10.;
        default      : return -1.;
    }
}

inline std::string PositionUnits::name(  ) const 
{
    switch( m_units ) {
        case MM      : return "MM";
        case CM      : return "CM";
        case UNKNOWN : return "UNKNOWN";
        default      : return "bad value";
    }
}

} // HepMC

#endif  // HEPMC_POSITION_UNITS_H
