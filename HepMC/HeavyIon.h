//--------------------------------------------------------------------------
#ifndef HEPMC_HEAVY_ION_H
#define HEPMC_HEAVY_ION_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, February 2006
//
// Additional information storage for Heavy Ion generators
//////////////////////////////////////////////////////////////////////////
//
//      int   Ncoll_hard          // Number of hard scatterings
//      int   Npart_proj          // Number of projectile participants
//      int   Npart_targ          // Number of target participants
//      int   Ncoll               // Number of NN (nucleon-nucleon) collisions
//      int   N_Nwounded_collisions        // Number of N-Nwounded collisions
//      int   Nwounded_N_collisions        // Number of Nwounded-N collisons
//      int   Nwounded_Nwounded_collisions // Number of Nwounded-Nwounded collisions
//      int   spectator_neutrons           // Number of spectators neutrons
//      int   spectator_protons            // Number of spectators protons
//      float impact_parameter        // Impact Parameter(fm) of collision
//      float event_plane_angle       // Azimuthal angle of event plane
//      float eccentricity            // eccentricity of participating nucleons
//                                        in the transverse plane 
//                                        (as in phobos nucl-ex/0510031) 
//      float sigma_inel_NN           // nucleon-nucleon inelastic 
//                                        (including diffractive) cross-section
//
//////////////////////////////////////////////////////////////////////////
// Feb. 17, 2006: adjust names according to suggestions from Heavy Ion users
// Feb.  7, 2006: first pass at making method names consistent with existing
//                HepMC code
//////////////////////////////////////////////////////////////////////////

namespace HepMC {

class HeavyIon {

public:
  // ---  birth/death:
  //
  HeavyIon()
    : m_Ncoll_hard(0), 
      m_Npart_proj(0),
      m_Npart_targ(0),
      m_Ncoll(0),
      m_spectator_neutrons(0),
      m_spectator_protons(0),
      m_N_Nwounded_collisions(0),
      m_Nwounded_N_collisions(0),
      m_Nwounded_Nwounded_collisions(0),
      m_impact_parameter(0),
      m_event_plane_angle(0),
      m_eccentricity(0),
      m_sigma_inel_NN(0)
    {}

  inline HeavyIon( int nh, int np, int nt, int nc, int ns, int nsp,
                   int nnw, int nwn, int nwnw, 
		   float im, float pl, float ec, float s );

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
    int   Ncoll_hard()                   const { return m_Ncoll_hard; }
    int   Npart_proj()                   const { return m_Npart_proj; }
    int   Npart_targ()                   const { return m_Npart_targ; }
    int   Ncoll()                        const { return m_Ncoll; }
    int   spectator_neutrons()           const { return m_spectator_neutrons; }
    int   spectator_protons()            const { return m_spectator_protons; }
    int   N_Nwounded_collisions()        const { return m_N_Nwounded_collisions; }
    int   Nwounded_N_collisions()        const { return m_Nwounded_N_collisions; }
    int   Nwounded_Nwounded_collisions() const { return m_Nwounded_Nwounded_collisions; }
    float impact_parameter()             const { return m_impact_parameter; }
    float event_plane_angle()            const { return m_event_plane_angle; }
    float eccentricity()                 const { return m_eccentricity;  }
    float sigma_inel_NN()                const { return m_sigma_inel_NN; }

  // ---  mutators:
    void   set_Ncoll_hard(const int &i)              { m_Ncoll_hard=i; }
    void   set_Npart_proj(const int &i)              { m_Npart_proj=i; }
    void   set_Npart_targ(const int &i)              { m_Npart_targ=i; }
    void   set_Ncoll(const int &i)                   { m_Ncoll=i; }
    void   set_spectator_neutrons(const int &i)      { m_spectator_neutrons=i; }
    void   set_spectator_protons(const int &i)       { m_spectator_protons=i; }
    void   set_N_Nwounded_collisions(const int &i)   { m_N_Nwounded_collisions=i; }
    void   set_Nwounded_N_collisions(const int &i)   { m_Nwounded_N_collisions=i; }
    void   set_Nwounded_Nwounded_collisions(const int &i) 
                                          { m_Nwounded_Nwounded_collisions=i; }
    void   set_impact_parameter(const float &f)      { m_impact_parameter=f; }
    void   set_event_plane_angle(const float &f)     { m_event_plane_angle=f; }
    void   set_eccentricity(const float &f)          { m_eccentricity=f;  }
    void   set_sigma_inel_NN(const float &f)         { m_sigma_inel_NN=f; }

private: // data members
    int   m_Ncoll_hard; 
    int   m_Npart_proj; 
    int   m_Npart_targ;
    int   m_Ncoll;
    int   m_spectator_neutrons;
    int   m_spectator_protons;
    int   m_N_Nwounded_collisions;
    int   m_Nwounded_N_collisions;
    int   m_Nwounded_Nwounded_collisions;
    float m_impact_parameter;
    float m_event_plane_angle;
    float m_eccentricity; 
    float m_sigma_inel_NN;

};

// inline operators
HeavyIon::HeavyIon( int nh, int np, int nt, int nc, int ns, int nsp,
                   int nnw=0, int nwn=0, int nwnw=0, 
		   float im=0., float pl=0., float ec=0., float s=0. )
    : m_Ncoll_hard(nh), 
      m_Npart_proj(np),
      m_Npart_targ(nt),
      m_Ncoll(nc),
      m_spectator_neutrons(ns),
      m_spectator_protons(nsp),
      m_N_Nwounded_collisions(nnw),
      m_Nwounded_N_collisions(nwn),
      m_Nwounded_Nwounded_collisions(nwnw),
      m_impact_parameter(im),
      m_event_plane_angle(pl),
      m_eccentricity(ec),
      m_sigma_inel_NN(s)
   {}

HeavyIon::HeavyIon( HeavyIon const & orig )
    : m_Ncoll_hard(orig.m_Ncoll_hard), 
      m_Npart_proj(orig.m_Npart_proj),
      m_Npart_targ(orig.m_Npart_targ),
      m_Ncoll(orig.m_Ncoll),
      m_spectator_neutrons(orig.m_spectator_neutrons),
      m_spectator_protons(orig.m_spectator_protons),
      m_N_Nwounded_collisions(orig.m_N_Nwounded_collisions),
      m_Nwounded_N_collisions(orig.m_Nwounded_N_collisions),
      m_Nwounded_Nwounded_collisions(orig.m_Nwounded_Nwounded_collisions),
      m_impact_parameter(orig.m_impact_parameter),
      m_event_plane_angle(orig.m_event_plane_angle),
      m_eccentricity(orig.m_eccentricity),
      m_sigma_inel_NN(orig.m_sigma_inel_NN)
   {}

HeavyIon &  HeavyIon::operator = ( HeavyIon const & rhs ) 
{
  HeavyIon temp( rhs );
  swap( temp );
  return *this;
}

void HeavyIon::swap( HeavyIon & other ) 
{
  std::swap(m_Ncoll_hard, other.m_Ncoll_hard); 
  std::swap(m_Npart_proj, other.m_Npart_proj);
  std::swap(m_Npart_targ, other.m_Npart_targ);
  std::swap(m_Ncoll, other.m_Ncoll);
  std::swap(m_N_Nwounded_collisions, other.m_N_Nwounded_collisions);
  std::swap(m_Nwounded_N_collisions, other.m_Nwounded_N_collisions);
  std::swap(m_Nwounded_Nwounded_collisions, other.m_Nwounded_Nwounded_collisions);
  std::swap(m_spectator_neutrons, other.m_spectator_neutrons);
  std::swap(m_spectator_protons, other.m_spectator_protons);
  std::swap(m_impact_parameter, other.m_impact_parameter);
  std::swap(m_event_plane_angle, other.m_event_plane_angle);
  std::swap(m_eccentricity, other.m_eccentricity);
  std::swap(m_sigma_inel_NN, other.m_sigma_inel_NN);
}

bool    HeavyIon::operator==( const HeavyIon& a ) const
{
    return ( a.Ncoll_hard() == this->Ncoll_hard() 
             && a.Npart_proj() == this->Npart_proj() 
             && a.Npart_targ() == this->Npart_targ() 
	     && a.Ncoll() == this->Ncoll() 
             && a.N_Nwounded_collisions() == this->N_Nwounded_collisions() 
	     && a.Nwounded_N_collisions() == this->Nwounded_N_collisions() 
             && a.Nwounded_Nwounded_collisions() == this->Nwounded_Nwounded_collisions() 
	     && a.spectator_neutrons() == this->spectator_neutrons() 
             && a.spectator_protons() == this->spectator_protons() 
	     && a.impact_parameter() == this->impact_parameter() 
	     && a.event_plane_angle() == this->event_plane_angle()
	     && a.eccentricity() == this->eccentricity() 
	     && a.sigma_inel_NN() == this->sigma_inel_NN() );
}

bool    HeavyIon::operator!=( const HeavyIon& a ) const
{
    return !( a == *this );
}

} // HepMC

#endif  // HEPMC_HEAVY_ION_H
