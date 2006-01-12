//--------------------------------------------------------------------------
#ifndef HEPMC_HEAVY_ION_H
#define HEPMC_HEAVY_ION_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, September 1999
//
// Additional information storage for Heavy Ion generators
//////////////////////////////////////////////////////////////////////////
//
//      int   fNHardScatters;            // Number of hard scatterings
//      int   fNProjectileParticipants;  // Number of projectiles participants
//      int   fNTargetParticipants;      // Number of target participants
//      int   fNNColl;                   // Number of N-N  collisions
//      int   fNNwColl;                  // Number of N-Nwounded  collisions
//      int   fNwNColl;                  // Number of Nwounded-N  collisons
//      int   fNwNwColl;                 // Number of Nwounded-Nwounded collisions
//      int   fSpecn;                    // Number of spectators neutrons
//      int   fSpecp;                    // Number of spectators  protons
//      float fImpactParameter;          // Impact Parameter
//
//////////////////////////////////////////////////////////////////////////

namespace HepMC {

class HeavyIon {

public:
  // ---  birth/death:
  //
  HeavyIon()
    : itsNHardScatters(0), 
      itsNProjectileParticipants(0),
      itsNTargetParticipants(0),
      itsNNColl(0),
      itsNNwColl(0),
      itsNwNColl(0),
      itsNwNwColl(0),
      itsSpecn(0),
      itsSpecp(0),
      itsImpactParameter(0)
    {}

  ~HeavyIon() {}
    
  // ---  copying:
  //
  inline HeavyIon( HeavyIon const & orig );
  inline HeavyIon &  operator = ( HeavyIon const & rhs );
  inline void swap( HeavyIon & other );

  // ---  equivalence:
  //
  inline bool    operator==( const HeavyIon& ) const;
  inline bool    operator!=( const HeavyIon& ) const;

  // ---  accessors:
    int   fNHardScatters()           const { return itsNHardScatters; }
    int   fNProjectileParticipants() const { return itsNProjectileParticipants; }
    int   fNTargetParticipants()     const { return itsNTargetParticipants; }
    int   fNNColl()                  const { return itsNNColl; }
    int   fNNwColl()                 const { return itsNNwColl; }
    int   fNwNColl()                 const { return itsNwNColl; }
    int   fNwNwColl()                const { return itsNwNwColl; }
    int   fSpecn()                   const { return itsSpecn; }
    int   fSpecp()                   const { return itsSpecp; }
    float fImpactParameter()         const { return itsImpactParameter; }

  // ---  mutators:
    void   setNHardScatters(const int &i)           { itsNHardScatters=i; }
    void   setNProjectileParticipants(const int &i) { itsNProjectileParticipants=i; }
    void   setNTargetParticipants(const int &i)     { itsNTargetParticipants=i; }
    void   setNNColl(const int &i)                  { itsNNColl=i; }
    void   setNNwColl(const int &i)                 { itsNNwColl=i; }
    void   setNwNColl(const int &i)                 { itsNwNColl=i; }
    void   setNwNwColl(const int &i)                { itsNwNwColl=i; }
    void   setSpecn(const int &i)                   { itsSpecn=i; }
    void   setSpecp(const int &i)                   { itsSpecp=i; }
    void   setImpactParameter(const float &f)       { itsImpactParameter=f; }

private: // data members
    int   itsNHardScatters; 
    int   itsNProjectileParticipants; 
    int   itsNTargetParticipants;
    int   itsNNColl;
    int   itsNNwColl;
    int   itsNwNColl;
    int   itsNwNwColl;
    int   itsSpecn;
    int   itsSpecp;
    float itsImpactParameter;

};

// inline operators
HeavyIon::HeavyIon( HeavyIon const & orig )
    : itsNHardScatters(orig.itsNHardScatters), 
      itsNProjectileParticipants(orig.itsNProjectileParticipants),
      itsNTargetParticipants(orig.itsNTargetParticipants),
      itsNNColl(orig.itsNNColl),
      itsNNwColl(orig.itsNNwColl),
      itsNwNColl(orig.itsNwNColl),
      itsNwNwColl(orig.itsNwNwColl),
      itsSpecn(orig.itsSpecn),
      itsSpecp(orig.itsSpecp),
      itsImpactParameter(orig.itsImpactParameter)
   {}

HeavyIon &  HeavyIon::operator = ( HeavyIon const & rhs ) 
{
  HeavyIon temp( rhs );
  swap( temp );
  return *this;
}

void HeavyIon::swap( HeavyIon & other ) 
{
  std::swap(itsNHardScatters, other.itsNHardScatters); 
  std::swap(itsNProjectileParticipants, other.itsNProjectileParticipants);
  std::swap(itsNTargetParticipants, other.itsNTargetParticipants);
  std::swap(itsNNColl, other.itsNNColl);
  std::swap(itsNNwColl, other.itsNNwColl);
  std::swap(itsNwNColl, other.itsNwNColl);
  std::swap(itsNwNwColl, other.itsNwNwColl);
  std::swap(itsSpecn, other.itsSpecn);
  std::swap(itsSpecp, other.itsSpecp);
  std::swap(itsImpactParameter, other.itsImpactParameter);
}

bool    HeavyIon::operator==( const HeavyIon& a ) const
{
    return ( a.fNHardScatters() == this->fNHardScatters() 
             && a.fNProjectileParticipants() == this->fNProjectileParticipants() 
             && a.fNTargetParticipants() == this->fNTargetParticipants() 
	     && a.fNNColl() == this->fNNColl() 
             && a.fNNwColl() == this->fNNwColl() 
	     && a.fNwNColl() == this->fNwNColl() 
             && a.fNwNwColl() == this->fNwNwColl() 
	     && a.fSpecn() == this->fSpecn() 
             && a.fSpecp() == this->fSpecp() 
	     && a.fImpactParameter() == this->fImpactParameter() );
}

bool    HeavyIon::operator!=( const HeavyIon& a ) const
{
    return !( a == *this );
}

} // HepMC

#endif  // HEPMC_HEAVY_ION_H
