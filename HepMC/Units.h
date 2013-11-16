#ifndef HEPMC_UNITS_H
#define HEPMC_UNITS_H

//--------------------------------------------------------------------------
// Units.h
// Author:  A. Buckley, D. Grellscheid
//
// Units used by a GenEvent
// The default units are set by a configure switch at compile time in Units.cc.
//--------------------------------------------------------------------------

#include <iostream>
#include <string>

namespace HepMC {

  /// Allow units to be specified within HepMC.
  ///
  /// The default units are set at compile time.
  ///
  /// @note Convention: if both types are passed, MomentumUnit always goes first.
  namespace Units {

    /// Momentum units
    enum MomentumUnit { MEV, GEV };
    /// Length/position units
    enum LengthUnit   { MM, CM };

    /// Get the default momentum unit
    ///
    /// Defined at HepMC configure/compile time
    MomentumUnit default_momentum_unit();
    /// Get the default length unit
    ///
    /// Defined at HepMC configure/compile time
    LengthUnit   default_length_unit();

    /// Represent a momentum unit as a string
    std::string name( MomentumUnit );
    /// Represent a length unit as a string
    std::string name( LengthUnit );

    /// Get the scaling factor between two momentum units
    double conversion_factor( MomentumUnit from, MomentumUnit to );
    /// Get the scaling factor between two length units
    double conversion_factor( LengthUnit from, LengthUnit to );

  } // Units
} // HepMC

#endif // HEPMC_UNITS_H
