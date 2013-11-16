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


  /// A simple representation of a physics 4 vector
  ///
  /// For compatibility with existing code, the basic expected geometrical
  /// access methods are provided.  Also, there is a templated constructor that
  /// will take another vector (HepLorentzVector, GenVector, ...)  which must
  /// have the following methods: x(), y(), z(), t().
  ///
  /// @todo Could use SFINAE to make the templated constructors more powerful,
  /// e.g. also support px(), py(), etc. and e()/E() rather than t()...
  class FourVector {
  public:

    /// Constructor requiring at least x, y, and z
    FourVector( double xin, double yin, double zin, double tin=0)
      : m_x(xin), m_y(yin), m_z(zin), m_t(tin) {}

    /// Constructor requiring only t
    ///
    /// @todo Why is this a good idea?!
    FourVector(double tin)
      : m_x(0), m_y(0), m_z(0), m_t(tin) {}

    FourVector()
      : m_x(0), m_y(0), m_z(0), m_t(0) {}

    /// Templated constructor
    ///
    /// This is used ONLY if T is not arithmetic
    ///
    /// @todo Use std:: functions for this in future, once C++11 can be used.
    template <class T >
    FourVector( const T& v,
                typename detail::disable_if< detail::is_arithmetic<T>::value, void >::type * = 0 )
      : m_x(v.x()), m_y(v.y()), m_z(v.z()), m_t(v.t()) {}

    /// Copy constructor
    FourVector(const FourVector & v)
      : m_x(v.x()), m_y(v.y()), m_z(v.z()), m_t(v.t()) {}

    /// Swap vector contents
    void swap( FourVector & other );

    /// x momentum component (if this is a momentum 4-vector)
    double px() const { return m_x; }
    /// y momentum component (if this is a momentum 4-vector)
    double py() const { return m_y; }
    /// z momentum component (if this is a momentum 4-vector)
    double pz() const { return m_z; }
    /// Energy component (if this is a momentum 4-vector)
    double e()  const { return m_t; }

    /// x component
    double x() const { return m_x; }  //!< return x
    /// y component
    double y() const { return m_y; }  //!< return y
    /// z component
    double z() const { return m_z; }  //!< return z
    /// Time component
    double t() const { return m_t; }  //!< return t

    /// Invariant mass squared
    double m2() const;
    /// Invariant mass
    ///
    /// If m2() is negative then -sqrt(-m2()) is returned
    double m() const;

    /// Transverse component of the spatial vector squared.
    double perp2() const;
    /// Transverse component of the spatial vector (R in cylindrical system).
    double perp() const;

    /// Polar angle (w.r.t. z)
    double theta() const;
    /// Azimuthal angle (w.r.t. z)
    double phi() const;
    /// Spatial vector component magnitude
    double rho() const;

    /// Copy assignment operator
    FourVector & operator = (const FourVector &);

    /// Test for equality
    bool operator == (const FourVector &) const;
    /// Test for inequality
    bool operator != (const FourVector &) const;

    /// Pseudorapidity, i.e. -ln(tan(theta/2))
    double pseudoRapidity() const;
    /// Alias for pseudorapidity
    double eta() const;

    /// @todo Add rapidity() for use with momentum 4-vectors

    /// @name Component value setters
    //@{

    /// Set x, y, z and t all at once
    void set(double x, double y, double z, double  t);

    /// Set x component
    void setX(double xin) { m_x = xin; }
    /// Set y component
    void setY(double yin) { m_y = yin; }
    /// Set z component
    void setZ(double zin) { m_z = zin; }
    /// Set t component
    void setT(double tin) { m_t = tin; }

    /// Set x momentum component (if this is a momentum 4-vector)
    void setPx(double xin) { setX(xin); }
    /// Set y momentum component (if this is a momentum 4-vector)
    void setPy(double yin) { setY(yin); }
    /// Set z momentum component (if this is a momentum 4-vector)
    void setPz(double zin) { setZ(zin); }
    /// Set energy component (if this is a momentum 4-vector)
    void setE(double tin)  { setT(tin); }

    //@}

  private:

    /// @name Data members
    //@{
    double m_x;
    double m_y;
    double m_z;
    double m_t;
    //@}

  };


  /// A simple representation of a 3 vector
  ///
  /// For compatibility with existing code, the basic expected geometrical
  /// access methods are povided.  Also, there is a templated constructor that
  /// will take another vector (HepLorentzVector, GenVector, ...)  which must
  /// have the following methods: x(), y(), z().
  class ThreeVector {

  public:

    /// Construct using x, y, and z (only x is required)
    ThreeVector( double xin, double yin =0, double zin =0 )
      : m_x(xin), m_y(yin), m_z(zin) {}

    /// Default constructor
    ThreeVector( )
      : m_x(0), m_y(0), m_z(0) {}

    /// Templated constructor
    ///
    /// This is used ONLY if T is not arithmetic
    template <class T >
    ThreeVector( const T& v,
                 typename detail::disable_if< detail::is_arithmetic<T>::value, void >::type * = 0 )
      : m_x(v.x()), m_y(v.y()), m_z(v.z()) {}

    /// Copy constructor
    ThreeVector(const ThreeVector & v)
      : m_x(v.x()), m_y(v.y()), m_z(v.z()) {}

    /// Swap momentum components
    void swap( ThreeVector & other );

    /// Get the x component
    double x() const { return m_x; }
    /// Get the y component
    double y() const { return m_y; }
    /// Get the z component
    double z() const { return m_z; }

    /// Add px(), ... methods since a 3 vector does not have to only be spatial

    /// Get the x component
    void setX(double xin) { m_x = xin; }
    /// Get the y component
    void setY(double yin) { m_y = yin; }
    /// Get the z component
    void setZ(double zin) { m_z = zin; }

    /// Set all components
    void set( double x, double y, double z);

    /// Polar angle (w.r.t. z)
    double theta() const;  //!< The
    /// Azimuthal angle (w.r.t. z)
    double phi()   const;
    /// 3-vector magnitude
    double r()     const;
    /// @todo Add an r2() since this is faster and often sufficient

    /// Set phi keeping magnitude and theta constant (BaBar)
    /// @todo Why is SimpleVector implementing BaBar-specific methods?
    void setPhi(double);
    /// Set theta keeping magnitude and phi constant (BaBar)
    /// @todo Why is SimpleVector implementing BaBar-specific methods?
    void setTheta(double);

    /// The transverse component squared (rho^2 in cylindrical coordinate system)
    double perp2() const;
    /// The transverse component (rho in cylindrical coordinate system)
    double perp() const;

    /// Copy assignment operator
    ThreeVector & operator = (const ThreeVector &);

    /// Test for equality
    bool operator == (const ThreeVector &) const;
    /// Test for inequality
    bool operator != (const ThreeVector &) const;

  private:

    /// @name Data members
    //@{
    double m_x;
    double m_y;
    double m_z;
    //@}

  };


} // HepMC

#include "HepMC/SimpleVector.icc"

#endif  // HEPMC_SIMPLEVECTOR_H
