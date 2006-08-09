//--------------------------------------------------------------------------
#ifndef HEPMC_POLARIZATION_H
#define HEPMC_POLARIZATION_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// Polarization object for a particle. All angles are in radians.
//////////////////////////////////////////////////////////////////////////

#include "HepMC/SimpleVector.h"
#include <iostream>
#include <cmath>

namespace HepMC {

    static const double HepMC_pi = 3.14159265358979323846;  // copy of pi from CLHEP
    
    class Polarization {

	friend std::ostream& operator<<( std::ostream&, const Polarization& );

    public:
	Polarization( double theta = 0, double phi = 0 );
	Polarization( const Polarization& inpolar );
	Polarization( const ThreeVector& vec3in );
	virtual       ~Polarization() {}

	Polarization& operator=( const Polarization& inpolar );
	bool          operator==( const Polarization& ) const;
	bool          operator!=( const Polarization& ) const;

	void          print( std::ostream& ostr = std::cout ) const;
    
	////////////////////
	// access methods //
	////////////////////
	double        theta() const;    // returns polar angle in radians
	double        phi() const;      // returns azimuthal angle in radians
	ThreeVector   normal3d() const; // unit 3 vector for easy manipulation

	double        set_theta( double theta );
	double        set_phi( double phi ); 
	void          set_theta_phi( double theta, double phi );
	// sets polarization according to direction of 3 vec
	ThreeVector   set_normal3d( const ThreeVector& vec3in ); 

    private:
	double m_theta; //polar angle of polarization in radians 0< theta <pi
	double m_phi;   //azimuthal angle of polarization in rad. 0< phi <2pi
    };

    ///////////////////////////
    // INLINE Access Methods //
    ///////////////////////////

    inline double Polarization::theta() const { return m_theta; }
    inline double Polarization::phi() const { return m_phi; }

    ///////////////////////////
    // INLINE Operators      //
    ///////////////////////////

    inline bool Polarization::operator==( const Polarization& a ) const 
    {
	return ( a.theta() == this->theta() && a.phi() == this->phi() );
    }

    inline bool Polarization::operator!=(const Polarization& a ) const 
    {
	return !( a == *this );
    }

} // HepMC

#endif  // HEPMC_POLARIZATION_H
//--------------------------------------------------------------------------
