//////////////////////////////////////////////////////////////////////////
// SimpleVector.h
//////////////////////////////////////////////////////////////////////////
#ifndef  HEPMC_SIMPLEVECTOR_H
#define  HEPMC_SIMPLEVECTOR_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, July 2006
//
// This header provides a place to hold the doubles which are part of one of 
// three types of physics vectors:
//    momentum 4 vector 
//    position or displacement 4 vector
//    position or displacement 3 vector
//
// For compatibility with existing code, 
// the basic expected geometrical access methods are povided 
// Also, both FourVector and ThreeVector have a templated constructor that will 
// take another vector (HepLorentzVector, GenVector, ...)
//    --> this vector must have the following methods: x(), y(), z()
//    -->  FourVector also requires the t() method
//
//////////////////////////////////////////////////////////////////////////


#include "HepMC/enable_if.h"
#include "HepMC/is_arithmetic.h"


namespace HepMC {

//! FourVector is a simple representation of a physics 4 vector

///
/// \class  FourVector
/// For compatibility with existing code, 
/// the basic expected geometrical access methods are povided.
/// Also, there is a templated constructor that will 
/// take another vector (HepLorentzVector, GenVector, ...)
/// which must have the following methods: x(), y(), z(), t().
///
class FourVector {

public:

  /// constructor requiring at least x, y, and z
  FourVector( double xin, double yin, double zin, double tin=0) 
  : m_x(xin), m_y(yin), m_z(zin), m_t(tin) {}

  /// constructor requiring only t 
  FourVector(double t)
  : m_x(0), m_y(0), m_z(0), m_t(t) {}

  FourVector() 
  : m_x(0), m_y(0), m_z(0), m_t(0) {}

  /// templated constructor
  /// this is used ONLY if T is not arithmetic
  template <class T >
  FourVector( const T& v,
         typename detail::disable_if< detail::is_arithmetic<T>::value, void >::type * = 0 )
  : m_x(v.x()), m_y(v.y()), m_z(v.z()), m_t(v.t()) {}

  /// copy constructor
  FourVector(const FourVector & v)
  : m_x(v.x()), m_y(v.y()), m_z(v.z()), m_t(v.t()) {}

  void swap( FourVector & other );  //!< swap

  double px() const { return m_x; }  //!< return px
  double py() const { return m_y; }  //!< return py
  double pz() const { return m_z; }  //!< return pz
  double e()  const { return m_t; }  //!< return E

  double x() const { return m_x; }  //!< return x
  double y() const { return m_y; }  //!< return y
  double z() const { return m_z; }  //!< return z
  double t() const { return m_t; }  //!< return t

  double m2() const;  //!< Invariant mass squared.
  double m() const;   //!< Invariant mass. If m2() is negative then -sqrt(-m2()) is returned.

  double perp2() const;  //!< Transverse component of the spatial vector squared.
  double perp() const;   //!< Transverse component of the spatial vector (R in cylindrical system).
  double mag() const;    //!< Magnitude of the spatial vector

  // Get spatial vector components in spherical coordinate system.
  double theta() const;  //!< The polar angle.
  double phi() const;  //!< The azimuth angle.
  double rho() const;  //!< spatial vector component magnitude

  FourVector & operator = (const FourVector &); //!< make a copy

  bool operator == (const FourVector &) const; //!< equality
  bool operator != (const FourVector &) const; //!< inequality

  double pseudoRapidity() const;  //!< Returns the pseudo-rapidity, i.e. -ln(tan(theta/2))
  double eta() const;             //!< Pseudorapidity (of the space part)

  /// set x, y, z, and t
  void set        (double x, double y, double z, double  t);

  void setX(double x) { m_x=x; }  //!< set x
  void setY(double y) { m_y=y; }  //!< set y
  void setZ(double z) { m_z=z; }  //!< set z
  void setT(double t) { m_t=t; }  //!< set t

  void setPx(double x) { m_x=x; }  //!< set px
  void setPy(double y) { m_y=y; }  //!< set py
  void setPz(double z) { m_z=z; }  //!< set pz
  void setE(double t)  { m_t=t; }  //!< set E

  // simple math
  FourVector   operator +  (const FourVector &) const; //!< addition
  FourVector & operator += (const FourVector &);       //!< addition
  FourVector   operator -  (const FourVector &) const; //!< subtraction
  FourVector & operator -= (const FourVector &);       //!< subtraction
  FourVector   operator -  () const;                   //!< unary minus
  FourVector & operator *= (double);                   //!< scale

private:

  double m_x;
  double m_y;
  double m_z;
  double m_t;
  
};

//! ThreeVector is a simple representation of a position or displacement 3 vector

///
/// \class  ThreeVector
/// For compatibility with existing code, 
/// the basic expected geometrical access methods are povided.
/// Also, there is a templated constructor that will 
/// take another vector (HepLorentzVector, GenVector, ...)
/// which must have the following methods: x(), y(), z().
///
class ThreeVector {

public:

  /// construct using x, y, and z (only x is required)
  ThreeVector( double xin, double yin =0, double zin =0 ) 
  : m_x(xin), m_y(yin), m_z(zin) {}

  ThreeVector( ) 
  : m_x(0), m_y(0), m_z(0) {}
  
  /// templated constructor
  /// this is used ONLY if T is not arithmetic
  template <class T >
  ThreeVector( const T& v,
         typename detail::disable_if< detail::is_arithmetic<T>::value, void >::type * = 0 )
  : m_x(v.x()), m_y(v.y()), m_z(v.z()) {}

  /// copy constructor
  ThreeVector(const ThreeVector & v)
  : m_x(v.x()), m_y(v.y()), m_z(v.z()) {}

  void swap( ThreeVector & other );  //!< swap

  double x() const { return m_x; }  //!< return x
  double y() const { return m_y; }  //!< return y
  double z() const { return m_z; }  //!< return z

  void setX(double x) { m_x=x; }  //!< set x
  void setY(double y) { m_y=y; }  //!< set y
  void setZ(double z) { m_z=z; }  //!< set z
  void set( double x, double y, double z);   //!< set x, y, and z

  double phi()   const;  //!< The azimuth angle.
  double theta() const;  //!< The polar angle.
  double r()     const;  //!< The magnitude

  double mag() const;  //!< The magnitude (r in spherical coordinate system).

  void setPhi(double);  //!< Set phi keeping mag and theta constant (BaBar).
  void setTheta(double);  //!< Set theta keeping mag and phi constant (BaBar).

  double perp2() const;  //!< The transverse component squared (rho^2 in cylindrical coordinate system).
  double perp() const;  //!< The transverse component (rho in cylindrical coordinate system).

  ThreeVector & operator = (const ThreeVector &); //!< make a copy

  bool operator == (const ThreeVector &) const; //!< equality
  bool operator != (const ThreeVector &) const; //!< inequality

  // simple math
  ThreeVector   operator +  (const ThreeVector &) const; //!< addition
  ThreeVector & operator += (const ThreeVector &);       //!< addition
  ThreeVector   operator -  (const ThreeVector &) const; //!< subtraction
  ThreeVector & operator -= (const ThreeVector &);       //!< subtraction
  ThreeVector   operator -  () const;                    //!< unary minus
  ThreeVector & operator *= (double);                    //!< scale

private:

  double m_x;
  double m_y;
  double m_z;

};  


} // HepMC

#include "HepMC/SimpleVector.icc"

#endif  // HEPMC_SIMPLEVECTOR_H

