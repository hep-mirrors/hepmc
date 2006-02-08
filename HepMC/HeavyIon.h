//--------------------------------------------------------------------------
#ifndef HEPMC_HEAVY_ION_H
#define HEPMC_HEAVY_ION_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, February 2006
//
// Additional information storage for Heavy Ion generators
//////////////////////////////////////////////////////////////////////////
//
//      int   hard_scatters;                // Number of hard scatterings
//      int   projectile_participants;      // Number of projectiles participants
//      int   target_participants;          // Number of target participants
//      int   N_N_collisions;               // Number of N-N  collisions
//      int   N_Nwounded_collisions;        // Number of N-Nwounded  collisions
//      int   Nwounded_N_collisions;        // Number of Nwounded-N  collisons
//      int   Nwounded_Nwounded_collisions; // Number of Nwounded-Nwounded collisions
//      int   spectator_neutrons;           // Number of spectators neutrons
//      int   spectator_protons;            // Number of spectators  protons
//      float impact_parameter;             // Impact Parameter
//      float event_plane_angle;
//
//////////////////////////////////////////////////////////////////////////
// Feb. 7, 2006: first pass at making method names consistent with existing
//               HepMC code
//////////////////////////////////////////////////////////////////////////

namespace HepMC {

class HeavyIon {

public:
  // ---  birth/death:
  //
  HeavyIon()
    : m_hard_scatters(0), 
      m_projectile_participants(0),
      m_target_participants(0),
      m_N_N_collisions(0),
      m_N_Nwounded_collisions(0),
      m_Nwounded_N_collisions(0),
      m_Nwounded_Nwounded_collisions(0),
      m_spectator_neutrons(0),
      m_spectator_protons(0),
      m_impact_parameter(0),
      m_event_plane_angle(0)
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
    int   hard_scatters()                const { return m_hard_scatters; }
    int   projectile_participants()      const { return m_projectile_participants; }
    int   target_participants()          const { return m_target_participants; }
    int   N_N_collisions()               const { return m_N_N_collisions; }
    int   N_Nwounded_collisions()        const { return m_N_Nwounded_collisions; }
    int   Nwounded_N_collisions()        const { return m_Nwounded_N_collisions; }
    int   Nwounded_Nwounded_collisions() const { return m_Nwounded_Nwounded_collisions; }
    int   spectator_neutrons()           const { return m_spectator_neutrons; }
    int   spectator_protons()            const { return m_spectator_protons; }
    float impact_parameter()             const { return m_impact_parameter; }
    float event_plane_angle()            const { return m_event_plane_angle; }

  // ---  mutators:
    void   set_hard_scatters(const int &i)           { m_hard_scatters=i; }
    void   set_projectile_participants(const int &i) { m_projectile_participants=i; }
    void   set_target_participants(const int &i)     { m_target_participants=i; }
    void   set_N_N_collisions(const int &i)          { m_N_N_collisions=i; }
    void   set_N_Nwounded_collisions(const int &i)   { m_N_Nwounded_collisions=i; }
    void   set_Nwounded_N_collisions(const int &i)   { m_Nwounded_N_collisions=i; }
    void   set_Nwounded_Nwounded_collisions(const int &i) 
                                          { m_Nwounded_Nwounded_collisions=i; }
    void   set_spectator_neutrons(const int &i)      { m_spectator_neutrons=i; }
    void   set_spectator_protons(const int &i)       { m_spectator_protons=i; }
    void   set_impact_parameter(const float &f)      { m_impact_parameter=f; }
    void   set_event_plane_angle(const float &f)     { m_event_plane_angle=f; }

private: // data members
    int   m_hard_scatters; 
    int   m_projectile_participants; 
    int   m_target_participants;
    int   m_N_N_collisions;
    int   m_N_Nwounded_collisions;
    int   m_Nwounded_N_collisions;
    int   m_Nwounded_Nwounded_collisions;
    int   m_spectator_neutrons;
    int   m_spectator_protons;
    float m_impact_parameter;
    float m_event_plane_angle;

};

// inline operators
HeavyIon::HeavyIon( HeavyIon const & orig )
    : m_hard_scatters(orig.m_hard_scatters), 
      m_projectile_participants(orig.m_projectile_participants),
      m_target_participants(orig.m_target_participants),
      m_N_N_collisions(orig.m_N_N_collisions),
      m_N_Nwounded_collisions(orig.m_N_Nwounded_collisions),
      m_Nwounded_N_collisions(orig.m_Nwounded_N_collisions),
      m_Nwounded_Nwounded_collisions(orig.m_Nwounded_Nwounded_collisions),
      m_spectator_neutrons(orig.m_spectator_neutrons),
      m_spectator_protons(orig.m_spectator_protons),
      m_impact_parameter(orig.m_impact_parameter)
      m_event_plane_angle(orig.m_event_plane_angle)
   {}

HeavyIon &  HeavyIon::operator = ( HeavyIon const & rhs ) 
{
  HeavyIon temp( rhs );
  swap( temp );
  return *this;
}

void HeavyIon::swap( HeavyIon & other ) 
{
  std::swap(m_hard_scatters, other.m_hard_scatters); 
  std::swap(m_projectile_participants, other.m_projectile_participants);
  std::swap(m_target_participants, other.m_target_participants);
  std::swap(m_N_N_collisions, other.m_N_N_collisions);
  std::swap(m_N_Nwounded_collisions, other.m_N_Nwounded_collisions);
  std::swap(m_Nwounded_N_collisions, other.m_Nwounded_N_collisions);
  std::swap(m_Nwounded_Nwounded_collisions, other.m_Nwounded_Nwounded_collisions);
  std::swap(m_spectator_neutrons, other.m_spectator_neutrons);
  std::swap(m_spectator_protons, other.m_spectator_protons);
  std::swap(m_impact_parameter, other.m_impact_parameter);
  std::swap(m_event_plane_angle, other.m_event_plane_angle);
}

bool    HeavyIon::operator==( const HeavyIon& a ) const
{
    return ( a.hard_scatters() == this->hard_scatters() 
             && a.projectile_participants() == this->projectile_participants() 
             && a.target_participants() == this->target_participants() 
	     && a.N_N_collisions() == this->N_N_collisions() 
             && a.N_Nwounded_collisions() == this->N_Nwounded_collisions() 
	     && a.Nwounded_N_collisions() == this->Nwounded_N_collisions() 
             && a.Nwounded_Nwounded_collisions() == this->Nwounded_Nwounded_collisions() 
	     && a.spectator_neutrons() == this->spectator_neutrons() 
             && a.spectator_protons() == this->spectator_protons() 
	     && a.impact_parameter() == this->impact_parameter() 
	     && a.event_plane_angle() == this->event_plane_angle() );
}

bool    HeavyIon::operator!=( const HeavyIon& a ) const
{
    return !( a == *this );
}

} // HepMC

#endif  // HEPMC_HEAVY_ION_H
