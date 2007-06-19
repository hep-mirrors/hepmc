//////////////////////////////////////////////////////////////////////////
// testHepMCIteration.h
//
// garren@fnal.gov, May 2007
//
// Define methods and classes used by testHepMCIteration
//////////////////////////////////////////////////////////////////////////

/// returns true if the GenParticle particle is a photon with pT > 10 GeV
bool IsPhoton( const HepMC::GenParticle* p ) { 
    if ( p->pdg_id() == 22 
	 && p->momentum().perp() > 10. ) return true;
    return false;
}

/// returns true if the GenParticle is a W+/W-
bool IsWBoson( const HepMC::GenParticle* p ) { 
    if ( abs(p->pdg_id()) == 24 ) return true;
    return false;
}

//! test class

/// \class  IsFinalState
/// this predicate returns true if the input has no decay vertex
class IsFinalState {
public:
    /// returns true if the GenParticle does not decay
    bool operator()( const HepMC::GenParticle* p ) { 
	if ( !p->end_vertex() && p->status()==1 ) return true;
	return false;
    }
};


