//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999
//
// particle properties common to all particles of a given PDG id
//////////////////////////////////////////////////////////////////////////

#include "HepMC/ParticleData.h"
#include <cstdio>       // needed for formatted output using sprintf 

namespace HepMC {

    ParticleData::ParticleData( std::string name, int id, double charge, 
				double mass, double clifetime, double spin ) :
	m_name(name), m_pdg_id(id), m_mass(mass), m_clifetime(clifetime)
    {
	/// Units ID: defined by PDG group (particles are +ve, antipart are -ve)
	///              also consistent with the Pythia definitions
	///       charge: fraction of proton charge
	///       mass
	///       cLifetime: c*time
	/// Default mass=0 and cLifetime is -1 which means stable (width= 0.)
	/// These defaults exist because many very basic MC generators
	///   may produce only massless stable particles in the event record.
	//
	set_charge(charge);
	set_spin(spin);
        s_counter++;
    }

    ParticleData::ParticleData( const char* name, int id, double charge, 
				double mass, double clifetime, double spin ):
	m_name(name), m_pdg_id(id), m_mass(mass), m_clifetime(clifetime)
    {
	/// note, this constructor is redundant to the one above, 
	/// i.e. one could use: 
	/// new HepMC::ParticleData(string("electron"),11,-1,0.000511,-1,.5);
	/// but we keep it because it is convenient.
	//
	set_charge(charge);
	set_spin(spin);	
        s_counter++;
    }

    ParticleData::~ParticleData() {
	s_counter--;
    }

    void ParticleData::print( std::ostream& ostr ) const {
	ostr << "ParticleData: " << name() << "\t"
	     << " ID[pdg]:" << pdg_id()
	     << " Charge[e+]:" << charge()
	     << " Mass:" << mass()
	     << " Tau:" << clifetime()
	     << " J:" << spin() << std::endl;
    }

    ////////////////////
    // access methods //
    ////////////////////

    int ParticleData::model_independent_pdg_id_() const { 
	/// returns the particle id with the seventh
	/// digit removed for susy/excited/technicolor particles.
	/// Thus en excited electron (40000011) would be returned as 11
	/// Useful only internally for sorting particles!
	int id = m_pdg_id;
	if ( id/1000000 >=1 && id/1000000 <= 4 ) id %= 1000000;
	return id;
    }

    double ParticleData::width() const { 
	double width;
	if ( m_clifetime > 0 ) { 
	    width = HepMC_hbarc/m_clifetime;
	} else if ( m_clifetime == 0 ) { 
	    width = -1;
	} else { 
	    width = 0; 
	}
	return width;
    }

    /////////////
    // Static  //
    /////////////
    unsigned int ParticleData::counter() { return s_counter; }
    unsigned int ParticleData::s_counter = 0; 

    /////////////
    // Friends //
    /////////////

    /// write to ostr
    std::ostream& operator<<( std::ostream& ostr, const ParticleData& pdata ) {
	char outline[80];
	sprintf( outline,"%+9d%21s%+6.2f%19.11e%19.11e%5.1f",
		 pdata.pdg_id(),
		 pdata.name().substr(0,21).c_str(),
		 pdata.charge(),
		 pdata.mass(),
		 pdata.clifetime(),
		 pdata.spin() );
	return ostr << outline;
    }

    //////////////////////
    // Related Methods  //
    //////////////////////

    double clifetime_from_width( double width ) {
	/// if you want to instantiate the particle lifetime from its width,
	/// use this static method inside the constructor:
	// i.e. new ParticleData(
	if ( width > 0 ) return HepMC_hbarc/width;
	if ( width == 0. ) return -1.;
	return 0.;
    }
    
} // HepMC







