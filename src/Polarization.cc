//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999
//
// Polarization object for a particle. All angles are in radians.
//////////////////////////////////////////////////////////////////////////

#include "HepMC/Polarization.h"

namespace HepMC {

    Polarization::Polarization( double theta, double phi ){
	set_theta( theta );
	set_phi( phi );
    }

    Polarization::Polarization( const Polarization& inpolar ) {
	*this = inpolar;
    }

    Polarization::Polarization( const HepNormal3D& vec3in ) {
	set_theta( vec3in.theta() );
	set_phi( vec3in.phi() );
    }

    Polarization& Polarization::operator=( const Polarization& inpolar ) {
	set_theta( inpolar.theta() );
	set_phi( inpolar.phi() );
	return *this;
    }

    void Polarization::print( std::ostream& ostr ) const {
	ostr << "Polarization: " << *this << std::endl;
    }

    ////////////////////
    // access methods //
    ////////////////////

    HepNormal3D  Polarization::normal3d() const {
	// unit Hep3Vector for easy manipulation
	HepNormal3D outvec(0,0,1);      // makes unit vector along Z
	outvec.setTheta( theta() ); // sets phi keeping mag and theta constant
	outvec.setPhi( phi() );     // sets theta keeping mag and phi constant
	return outvec;
    }

    double Polarization::set_theta( double theta ) {
	// Theta is restricted to be between 0 --> pi
	// if an out of range value is given, it is translated to this range.
	theta = ( theta>0 ? theta : -theta ); // this is just absolute value.
	// translate to 0 < theta < 2pi
	theta = ( theta/(2*HepMC_pi) - int(theta/(2*HepMC_pi)) ) 
		* 2*HepMC_pi;
	if ( theta > HepMC_pi ) theta = 2*HepMC_pi - theta;
	return m_theta = theta;
    }

    double Polarization::set_phi( double phi ) {
	// Phi is restricted to be between 0 --> 2pi
	// if an out of range value is given, it is translated to this range.
	//
	// translate to -2pi < phi < 2pi
	phi = ( phi/(2*HepMC_pi) - int(phi/(2*HepMC_pi)) ) * 2*HepMC_pi;
	if ( phi < 0 ) phi = 2*HepMC_pi + phi; // translates to 0 < phi < 2pi
	return m_phi = phi;
    }

    void Polarization::set_theta_phi( double theta, double phi ) {
	set_theta( theta );
	set_phi( phi ) ;
    }

    HepNormal3D Polarization::set_normal3d( const HepNormal3D& vec3in ) {
	set_theta( vec3in.theta() );
	set_phi( vec3in.phi() );
	return vec3in;
    }

    /////////////
    // Friends //
    /////////////

    std::ostream& operator<<( std::ostream& ostr, const Polarization& polar ) {
	return ostr << "(" << polar.theta() 
		    << ","  << polar.phi() << ")";
    }
    
} // HepMC


