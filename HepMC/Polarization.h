#ifndef HEPMC_POLARIZATION_H
#define HEPMC_POLARIZATION_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics",  Comput.Phys.Commun. 134 (2001) 41-46.
//
// Polarization object for a particle. All angles are in radians.
//////////////////////////////////////////////////////////////////////////

#include "HepMC/SimpleVector.h"
#include <iostream>
#include <cmath>

namespace HepMC {

  /// @todo Just use M_PI?
  static const double HepMC_pi = 3.14159265358979323846;

  /// The Polarization class stores theta and phi for a GenParticle
  ///
  /// HepMC::Polarization stores a particle's theta and phi in radians.
  /// Use of this information is optional.
  /// By default, the polarization is set to zero.
  class Polarization {

    /// Print polarization information
    friend std::ostream& operator<<( std::ostream&, const Polarization& );

  public:

    /// Default constructor
    Polarization( );

    /// Constructor requiring at least one value
    Polarization( double theta, double phi = 0 );

    /// Construct from another polarization object
    Polarization( const Polarization& inpolar );

    /// Construct using the polar and azimuthal angles from a ThreeVector
    Polarization( const ThreeVector& vec3in );

    /// Virtual destructor
    /// @todo Is inheritance expected? Otherwise can delete this
    virtual ~Polarization() {}

    /// Make a copy
    Polarization& operator=( const Polarization& inpolar );

    /// Swap
    void swap( Polarization & other);

    /// @name Access methods
    //@{

    /// Return true if the Polarization has been defined
    bool is_defined() const;

    /// Polar angle in radians
    double theta() const { return m_theta; }
    /// Azimuthal angle in radians
    double phi() const { return m_phi; }
    /// Unit 3-vector for easy manipulation
    ThreeVector normal3d() const;


    /// Set polar angle in radians
    double set_theta( double theta );
    /// Set azimuthal angle in radians
    double set_phi( double phi );
    /// Set both polar and azimuthal angles in radians
    void set_theta_phi( double theta, double phi );
    /// Set polarization according to direction of 3 vec
    ThreeVector set_normal3d( const ThreeVector& vec3in );
    /// Declare the Polarization as undefined and zeros the values
    void set_undefined();

    //@}


    /// @name Equality tests
    //@{

    /// Equality requires that theta and phi are equal
    ///
    /// @todo Add FP tolerance?
    bool operator==( const Polarization& a) const {
      return ( a.theta() == this->theta() && a.phi() == this->phi() && a.is_defined() == this->is_defined() );
    }

    /// Inequality results if either theta or phi differ
    bool operator!=( const Polarization& a) const { return !(a == *this); }

    //@}


    /// Print theta and phi to an ostream
    void print( std::ostream& ostr = std::cout ) const;


  private:

    /// Return a polar angle in the correct range
    /// @todo Can be a non-member in an unnamed namespace in the .cc
    double valid_theta( double theta );
    /// Return an azimuthal angle in the correct range
    /// @todo Can be a non-member in an unnamed namespace in the .cc
    double valid_phi( double phi );

    /// Polar angle of polarization in radians 0 < theta < pi
    double m_theta;
    /// Azimuthal angle of polarization in radians 0 < phi < 2pi
    double m_phi;
    /// Used to flag if the Polarization has been defined
    bool m_defined;

  };

} // HepMC

#endif  // HEPMC_POLARIZATION_H
