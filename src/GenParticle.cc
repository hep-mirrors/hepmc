//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999
// Updated: 07.02.2000 no longer does particle point to ParticleData, 
//                     but rather it uses an int id which can be looked up
// particle within an event coming in/out of a vertex
//////////////////////////////////////////////////////////////////////////
#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include <cstdio>       // needed for formatted output using sprintf 

namespace HepMC {

    GenParticle::GenParticle( void ) :
	m_momentum(0), m_pdg_id(0), m_status(0), m_flow(this),
        m_polarization(0), m_production_vertex(0), m_end_vertex(0),
        m_barcode(0)
    {
	s_counter++;
    }

    GenParticle::GenParticle( const HepLorentzVector& momentum, 
			int pdg_id, int status, 
			const Flow& itsflow,
			const Polarization& polar ) : 
	m_momentum(momentum), m_pdg_id(pdg_id), m_status(status), m_flow(this),
	m_polarization(polar), m_production_vertex(0), m_end_vertex(0),
	m_barcode(0)
    {
	// Establishing *this as the owner of m_flow is done above,
	// then we set it equal to the other flow pattern (subtle)
	set_flow(itsflow);
	s_counter++;
    }

    GenParticle::GenParticle( const GenParticle& inparticle ) : 
	m_pdg_id(0), m_status(0), m_flow(this), 
	m_production_vertex(0), m_end_vertex(0), m_barcode(0) 
    {
	// Shallow copy: does not copy the vertex pointers
	// (note - impossible to copy vertex pointers which having the vertex
	//         and particles in/out point-back to one another -- unless you
	//         copy the entire tree -- which we don't want to do)
	*this = inparticle; 
	s_counter++;
    }

    GenParticle::~GenParticle() {    
	if ( parent_event() ) parent_event()->remove_barcode(this);
	s_counter--;
    }

    GenParticle& GenParticle::operator=( const GenParticle& inparticle ) {
	// Shallow: does not copy the vertex pointers
	// (note - impossible to copy vertex pointers which having the vertex
	//         and particles in/out point-back to one another -- unless you
	//         copy the entire tree -- which we don't want to do)
	set_momentum( inparticle.momentum() );
	set_pdg_id( inparticle.pdg_id() );
	set_status( inparticle.status() );
	set_production_vertex_( 0 );
	set_end_vertex_( 0 );
	set_flow(inparticle.m_flow);
	set_polarization( inparticle.polarization() );
	suggest_barcode( inparticle.barcode() );
	return *this;
    }

    bool GenParticle::operator==( const GenParticle& a ) const {
	// consistent with the definition of the copy constructor as a shallow
	//  constructor,.. this operator does not test the vertex pointers.
	//  Does not compare barcodes.
	if ( a.momentum() != this->momentum() ) return 0;
	if ( a.pdg_id() != this->pdg_id() ) return 0;
	if ( a.status() != this->status() ) return 0;
	if ( a.m_flow != this->m_flow ) return 0;
	if ( a.polarization() != this->polarization() ) return 0;
	return 1;
    }

    bool GenParticle::operator!=( const GenParticle& a ) const {
	return !( a == *this );
    }

    void GenParticle::print( std::ostream& ostr ) const {
	// Dump this particle's full info to ostr, where by default
	//  particle.print(); will dump to cout.
	ostr << "GenParticle: " 
	     << barcode() << " ID:" << pdg_id()
	     << " (P,E)=" << momentum().px() << "," << momentum().py() 
	     << "," << momentum().pz() << "," << momentum().e()
	     << " Stat:" << status();
	if ( production_vertex() && production_vertex()->barcode()!=0 ) {
	    ostr << " PV:" << production_vertex()->barcode();
	} else ostr << " PV:" << production_vertex();
	if ( end_vertex() && end_vertex()->barcode()!=0 ) {
	    ostr << " EV:" << end_vertex()->barcode();
	} else ostr << " EV:" << end_vertex();
	ostr << " Pol:" << polarization() << " F:" << m_flow << std::endl;
    }

    GenEvent* GenParticle::parent_event() const {
	if ( production_vertex() ) return production_vertex()->parent_event();
	if ( end_vertex() ) return end_vertex()->parent_event();
	return 0;
    }

    void GenParticle::set_production_vertex_( GenVertex* prodvertex )
    { 
	GenEvent* its_orig_event = parent_event();
	m_production_vertex = prodvertex; 
	GenEvent* its_new_event = parent_event();
	// Next bit of logic ensures the barcode maps are kept up to date
	//  in the GenEvent containers.
	if ( its_orig_event != its_new_event ) {
	    if ( its_new_event ) its_new_event->set_barcode( this, barcode() );
	    if ( its_orig_event ) its_orig_event->remove_barcode( this );
	}
    }

    void GenParticle::set_end_vertex_( GenVertex* decayvertex ) 
    { 
	GenEvent* its_orig_event = parent_event();
	m_end_vertex = decayvertex;
	GenEvent* its_new_event = parent_event();
	if ( its_orig_event != its_new_event ) {
	    if ( its_new_event ) its_new_event->set_barcode( this, barcode() );
	    if ( its_orig_event ) its_orig_event->remove_barcode( this );
	}
    }	

    bool GenParticle::suggest_barcode( int the_bar_code )
    {
	// allows a barcode to be suggested for this particle.
	// In general it is better to let the event pick the barcode for
	// you, which is automatic.
	// Returns TRUE if the suggested barcode has been accepted (i.e. the
	//  suggested barcode has not already been used in the event, 
	//  and so it was used).
	// Returns FALSE if the suggested barcode was rejected, or if the
	//  particle is not yet part of an event, such that it is not yet
	//  possible to know if the suggested barcode will be accepted).
	if ( the_bar_code <0 ) {
	    std::cerr << "GenParticle::suggest_barcode WARNING, particle bar "
		      << "\n codes MUST be positive integers. Negative  "
		      << "\n integers are reserved for vertices only. Your "
		      << "\n suggestion has been rejected." << std::endl;
	    return false;
	}
	bool success = false;
	if ( parent_event() ) {
	    success = parent_event()->set_barcode( this, the_bar_code );
	} else { set_barcode_( the_bar_code ); }
	return success;
    }

    /////////////
    // Static  //
    /////////////
    unsigned int GenParticle::counter() { return s_counter; }
    unsigned int GenParticle::s_counter = 0; 

    /////////////
    // Friends //
    /////////////
    std::ostream& operator<<( std::ostream& ostr, const GenParticle& part ) {
	char outline[80];
	if ( part.end_vertex() && part.end_vertex()->barcode()!=0 ) {
	    sprintf( outline," %9d%9d %+9.2e,%+9.2e,%+9.2e,%+9.2e %3d %9d",
		     part.barcode(), part.pdg_id(),
		     part.momentum().px(),part.momentum().py(),
		     part.momentum().pz(),part.momentum().e(),
		     part.status(),
		     part.end_vertex()->barcode() );
	} else {
	    // In this case the end_vertex does not have a unique 
	    //   barcode assigned, so we choose instead to print its address
	    sprintf( outline," %9d%9d %+9.2e,%+9.2e,%+9.2e,%+9.2e %3d %9p",
		     part.barcode(), part.pdg_id(),
		     part.momentum().px(),part.momentum().py(),
		     part.momentum().pz(),part.momentum().e(),
		     part.status(),
		     (void*)part.end_vertex() );
	}
	return ostr << outline;
    }

} // HepMC

