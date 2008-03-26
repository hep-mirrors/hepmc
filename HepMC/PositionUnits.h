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

enum HepMCpositionUnits { unknown = 0, mm, cm, meter };
// this enum scheme can accomodate additional definitions
// use "meter" because we don't want to have a single character enum ("m")

    PositionUnits( HepMCpositionUnits pos = unknown ); //!< default constructor
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
    HepMCpositionUnits m_units;    // unknown by default
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
   if( m_units != unknown ) return false;
   m_units = pos;
   return true;
}

inline bool PositionUnits::set_units( std::string& pos )
{
   /// set_units will FAIL if the units are already defined
   if( m_units != unknown ) return false;
   if( pos == "mm"      )      m_units = mm;
   else if( pos == "cm"      ) m_units = cm;
   else if( pos == "meter"   ) m_units = meter;
   else if( pos == "unknown" ) m_units = unknown;
   else return false;
   return true;
}

inline double PositionUnits::conversion_factor( HepMCpositionUnits pos ) const
{
   /// This routine returns the desired scale factor
   /// If position units are unknown, the scale factor is 0.
   if( m_units == unknown ) return 0.;
   if( m_units == pos ) return 1.;
   double f = PositionUnits(pos).unitFactor();
   return  f < 0. ? 0. : unitFactor()/f;
}

inline double PositionUnits::unitFactor() const
{
    switch( m_units ) {
        case mm      : return 1.;
        case cm      : return 10.;
        case meter   : return 1000.;
        default      : return -1.;
    }
}

inline std::string PositionUnits::name(  ) const 
{
    switch( m_units ) {
        case mm      : return "mm";
        case cm      : return "cm";
        case meter   : return "meter";
        case unknown : return "unknown";
        default      : return "bad value";
    }
}

} // HepMC

#endif  // HEPMC_POSITION_UNITS_H
