
#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"

#include <iomanip>       // needed for formatted output

namespace HepMC {

GenParticle::GenParticle( void ) :
    m_momentum(0), 
    m_pdg_id(0), 
    m_status(0), 
    m_flow(this),
    m_polarization(0), 
    m_production_vertex(-1), 
    m_end_vertex(-1),
    m_event_index(-1),
    m_barcode(0), 
    m_generated_mass(0.),
    m_event(0)
{}

GenParticle::GenParticle( const FourVector& momentum, 
			  int pdg_id, int status, 
			  const Flow& itsflow,
			  const Polarization& polar ) : 
    m_momentum(momentum), 
    m_pdg_id(pdg_id), 
    m_status(status), 
    m_flow(this),
    m_polarization(polar), 
    m_production_vertex(-1), 
    m_end_vertex(-1),
    m_event_index(-1),
    m_barcode(0), 
    m_generated_mass(momentum.m()),
    m_event(0)
{
    // Establishing *this as the owner of m_flow is done above,
    // then we set it equal to the other flow pattern (subtle)
    set_flow(itsflow);
}

GenParticle::GenParticle( const GenParticle& inparticle ) : 
    m_momentum( inparticle.momentum() ),
    m_pdg_id           ( inparticle.pdg_id() ), 
    m_status           ( inparticle.status() ), 
    m_flow             ( inparticle.flow()),
    m_polarization     ( inparticle.polarization() ),
    m_production_vertex( inparticle.production_vertex_index() ), 
    m_end_vertex       ( inparticle.end_vertex_index() ), 
    m_event_index      ( inparticle.event_index() ),
    m_barcode          ( inparticle.barcode() ), 
    m_generated_mass   ( inparticle.generated_mass() ),
    m_event            ( inparticle.parent_event() )
{}

GenParticle::~GenParticle() {    
    if ( parent_event() ) parent_event()->remove_particle(m_event_index);
}

void GenParticle::swap( GenParticle & other)
{
    // if a container has a swap method, use that for improved performance
    m_momentum.swap( other.m_momentum );
    std::swap(            m_pdg_id, other.m_pdg_id );
    std::swap(            m_status, other.m_status );
    m_flow.swap(                    other.m_flow );
    m_polarization.swap(            other.m_polarization );
    std::swap( m_production_vertex, other.m_production_vertex );
    std::swap(        m_end_vertex, other.m_end_vertex );
    std::swap(       m_event_index, other.m_event_index );
    std::swap(           m_barcode, other.m_barcode );
    std::swap(    m_generated_mass, other.m_generated_mass );
    std::swap(             m_event, other.m_event );
}

GenParticle& GenParticle::operator=( const GenParticle& inparticle ) {
    // best practices implementation
    GenParticle tmp( inparticle );
    swap( tmp );
    return *this;
}

bool GenParticle::operator==( const GenParticle& a ) const {
    ///  This operator does not test the vertex indices.
    ///  Does not compare barcodes.
    if ( a.momentum() != this->momentum() ) return false;
    if ( a.generated_mass() != this->generated_mass() ) return false;
    if ( a.pdg_id() != this->pdg_id() ) return false;
    if ( a.status() != this->status() ) return false;
    if ( a.m_flow != this->m_flow ) return false;
    if ( a.polarization() != this->polarization() ) return false;
    return true;
}

bool GenParticle::operator!=( const GenParticle& a ) const {
    return !( a == *this );
}

void GenParticle::print( std::ostream& ostr ) const {
    /// Dump this particle's full info to ostr, where by default
    ///  particle.print(); will dump to cout.
    ostr << "GenParticle: " 
	 << barcode() << " ID:" << pdg_id()
	 << " (P,E)=" << momentum().px() << "," << momentum().py() 
	 << "," << momentum().pz() << "," << momentum().e()
	 << " Stat:" << status();
    if ( production_vertex_index() >= 0 && production_vertex().barcode()!=0 ) {
	ostr << " PV:" << production_vertex().barcode();
    } else ostr << " PV:" << production_vertex();
    if ( end_vertex_index() >= 0 && end_vertex().barcode()!=0 ) {
	ostr << " EV:" << end_vertex().barcode();
    } else ostr << " EV:" << end_vertex();
    ostr << " Pol:" << polarization() << " F:" << m_flow << std::endl;
}

std::ostream& operator<<( std::ostream& ostr, const GenParticle& part ) {
    // find the current stream state
    std::ios_base::fmtflags orig = ostr.flags();
    std::streamsize prec = ostr.precision();
    ostr << " ";
    ostr.width(9);
    ostr << part.barcode();
    ostr.width(9);
    ostr << part.pdg_id() << " ";
    ostr.width(9);
    ostr.precision(2);
    ostr.setf(std::ios::scientific, std::ios::floatfield);
    ostr.setf(std::ios_base::showpos);
    ostr << part.momentum().px() << ",";
    ostr.width(9);
    ostr << part.momentum().py() << ",";
    ostr.width(9);
    ostr << part.momentum().pz() << ",";
    ostr.width(9);
    ostr << part.momentum().e() << " ";
    ostr.setf(std::ios::fmtflags(0), std::ios::floatfield);
    ostr.unsetf(std::ios_base::showpos);
    if ( part.end_vertex_index() >= 0 && part.end_vertex().barcode()!=0 ) {
	ostr.width(3);
	ostr << part.status() << " ";
	ostr.width(9);
	ostr << part.end_vertex().barcode();
    } else if ( part.end_vertex_index() < 0 ) {
	// There is no valid end_vertex 
	// For consistency across different compilers, do not print anything
	ostr.width(3);
	ostr << part.status();
    } else {
	// In this case the end_vertex does not have a unique 
	//   barcode assigned, so we choose instead to print its event index
	ostr.width(3);
	ostr << part.status() << " ";
	ostr.width(9);
	ostr << part.end_vertex().event_index();
    }
    // restore the stream state
    ostr.flags(orig);
    ostr.precision(prec);
    return ostr;
}

GenParticle::operator HepMC::FourVector() const 
{ return m_momentum; }

const FourVector & GenParticle::momentum() const 
{ return m_momentum; }

int GenParticle::pdg_id() const { return m_pdg_id; }

int GenParticle::status() const { return m_status; }

const Flow & GenParticle::flow() const { return m_flow; }

int GenParticle::flow( int code_index ) const
{ return m_flow.icode( code_index ); }

const Polarization & GenParticle::polarization() const 
{ return m_polarization; }

GenVertex& GenParticle::production_vertex() const { 
    //return const_cast<GenVertex*>(m_event->lookup_vertex[m_production_vertex]); 
    return m_event->lookup_vertex(m_production_vertex); 
}

GenVertex& GenParticle::end_vertex() const { 
    return m_event->lookup_vertex(m_end_vertex); 
}

size_t GenParticle::production_vertex_index() const { 
    return m_production_vertex; 
}

size_t GenParticle::end_vertex_index() const { 
    return m_end_vertex; 
}

GenEvent* GenParticle::parent_event() const {
    return m_event; 
}

double  GenParticle::generated_mass() const {
    return m_generated_mass;
}

void   GenParticle::set_generated_mass( const double & m ) {
    m_generated_mass = m;
}

int  GenParticle::barcode() const { return m_barcode; }

void GenParticle::set_barcode_( int bc ) { m_barcode = bc; }

bool GenParticle::is_undecayed() const {
    return ( m_status==1 ) ?  true : false;
}
bool GenParticle::has_decayed() const {
    return ( m_status==2 ) ?  true : false;
}
bool GenParticle::is_beam() const {
    return ( m_status==4 ) ?  true : false;
}

void GenParticle::set_momentum( const FourVector& vec4 )
{ m_momentum = vec4; }

void GenParticle::set_pdg_id( int id ) { m_pdg_id = id; }

void GenParticle::set_status( int st ) { m_status = st; }

void GenParticle::set_flow( const Flow& f ) { m_flow = f; }

void GenParticle::set_flow( int code_index, int code ) 
{
    if ( code == 0 ) { 
	m_flow.set_unique_icode( code_index );
    } else { 
	m_flow.set_icode( code_index, code );
    }
}

void GenParticle::set_polarization( const Polarization& polar )
{ m_polarization = polar; }

size_t GenParticle::event_index()
{ return m_event_index; }

const size_t GenParticle::event_index() const
{ return m_event_index; }

void GenParticle::set_event_index( size_t ind ) { m_event_index = ind; }

void GenParticle::set_parent_event_( GenEvent* new_evt ) 
{ 
    m_event = new_evt; 
}

void GenParticle::set_production_vertex_( size_t prodvertex )
{ 
    GenEvent* its_orig_event = parent_event();
    m_production_vertex = parent_event()->lookup_vertex(prodvertex).event_index(); 
    GenEvent* its_new_event = parent_event();
    // Next bit of logic ensures the barcode maps are kept up to date
    //  in the GenEvent containers.
    if ( its_orig_event != its_new_event ) {
	if ( its_new_event ) its_new_event->add_particle( *this );
	if ( its_orig_event ) its_orig_event->remove_particle( *this );
    }
}

void GenParticle::set_end_vertex_( size_t decayvertex ) 
{ 
    GenEvent* its_orig_event = parent_event();
    m_end_vertex = parent_event()->lookup_vertex(decayvertex).event_index();
    GenEvent* its_new_event = parent_event();
    if ( its_orig_event != its_new_event ) {
	if ( its_new_event ) its_new_event->add_particle( *this );
	if ( its_orig_event ) its_orig_event->remove_particle( *this );
    }
}	

void GenParticle::convert_momentum( const double & f ) {
   m_momentum = FourVector( f*m_momentum.px(),
                            f*m_momentum.py(),
                            f*m_momentum.pz(),
                            f*m_momentum.e() );
   if( m_generated_mass > 0. ) m_generated_mass = f*m_generated_mass;
}


} // HepMC
