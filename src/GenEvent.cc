
#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenCrossSection.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/WeightContainer.h"
#include "HepMC/Units.h"

namespace HepMC {

GenEvent::GenEvent( int signal_process_id, 
                    int event_number,
		    std::size_t signal_vertex,
		    const WeightContainer& weights,
		    const std::vector<long>& random_states,
		    Units::MomentumUnit mom, 
		    Units::LengthUnit len ) :
    m_signal_process_id(signal_process_id), 
    m_event_number(event_number),
    m_mpi(-1),
    m_event_scale(-1), 
    m_alphaQCD(-1), 
    m_alphaQED(-1),
    m_signal_process_vertex(signal_vertex), 
    m_beam_particle_1(-1),
    m_beam_particle_2(-1),
    m_weights(weights),
    m_random_states(random_states),
    m_vertices(),
    m_particles(),
    m_cross_section(0), 
    m_heavy_ion(0), 
    m_pdf_info(0),
    m_momentum_unit(mom),
    m_position_unit(len)
{
    /// This constructor only allows null pointers to HeavyIon and PdfInfo
    ///
    /// note: default values for m_event_scale, m_alphaQCD, m_alphaQED
    ///       are as suggested in hep-ph/0109068, "Generic Interface..."
    ///
}

GenEvent::GenEvent( int signal_process_id, 
                    int event_number,
		    std::size_t signal_vertex,
		    const WeightContainer& weights,
		    const std::vector<long>& random_states,
		    const HeavyIon& ion, 
		    const PdfInfo& pdf,
		    Units::MomentumUnit mom, 
		    Units::LengthUnit len ) :
    m_signal_process_id(signal_process_id), 
    m_event_number(event_number),
    m_mpi(-1),
    m_event_scale(-1), 
    m_alphaQCD(-1), 
    m_alphaQED(-1),
    m_signal_process_vertex(signal_vertex), 
    m_beam_particle_1(-1),
    m_beam_particle_2(-1),
    m_weights(weights),
    m_random_states(random_states), 
    m_vertices(),
    m_particles(),
    m_cross_section(0), 
    m_heavy_ion( new HeavyIon(ion) ), 
    m_pdf_info( new PdfInfo(pdf) ),
    m_momentum_unit(mom),
    m_position_unit(len)
{
    /// GenEvent makes its own copy of HeavyIon and PdfInfo
    ///
    /// note: default values for m_event_scale, m_alphaQCD, m_alphaQED
    ///       are as suggested in hep-ph/0109068, "Generic Interface..."
}

GenEvent::GenEvent( Units::MomentumUnit mom, 
		    Units::LengthUnit len, 
		    int signal_process_id, 
                    int event_number,
		    std::size_t signal_vertex,
		    const WeightContainer& weights,
		    const std::vector<long>& random_states ) :
    m_signal_process_id(signal_process_id), 
    m_event_number(event_number),
    m_mpi(-1),
    m_event_scale(-1), 
    m_alphaQCD(-1), 
    m_alphaQED(-1),
    m_signal_process_vertex(signal_vertex), 
    m_beam_particle_1(-1),
    m_beam_particle_2(-1),
    m_weights(weights),
    m_random_states(random_states),
    m_vertices(),
    m_particles(),
    m_cross_section(0), 
    m_heavy_ion(0), 
    m_pdf_info(0),
    m_momentum_unit(mom),
    m_position_unit(len)
{
    /// constructor requiring units - all else is default
    /// This constructor only allows null pointers to HeavyIon and PdfInfo
    ///
    /// note: default values for m_event_scale, m_alphaQCD, m_alphaQED
    ///       are as suggested in hep-ph/0109068, "Generic Interface..."
    ///
}

GenEvent::GenEvent( Units::MomentumUnit mom, 
		    Units::LengthUnit len,
		    int signal_process_id, int event_number,
		    std::size_t signal_vertex,
		    const WeightContainer& weights,
		    const std::vector<long>& random_states,
		    const HeavyIon& ion, 
		    const PdfInfo& pdf ) :
    m_signal_process_id(signal_process_id), 
    m_event_number(event_number),
    m_mpi(-1),
    m_event_scale(-1), 
    m_alphaQCD(-1), 
    m_alphaQED(-1),
    m_signal_process_vertex(signal_vertex), 
    m_beam_particle_1(-1),
    m_beam_particle_2(-1),
    m_weights(weights),
    m_random_states(random_states), 
    m_vertices(),
    m_particles(),
    m_cross_section(0), 
    m_heavy_ion( new HeavyIon(ion) ), 
    m_pdf_info( new PdfInfo(pdf) ),
    m_momentum_unit(mom),
    m_position_unit(len)
{
    /// explicit constructor with units first that takes HeavyIon and PdfInfo
    /// GenEvent makes its own copy of HeavyIon and PdfInfo
    ///
    /// note: default values for m_event_scale, m_alphaQCD, m_alphaQED
    ///       are as suggested in hep-ph/0109068, "Generic Interface..."
}

GenEvent::GenEvent( const GenEvent& inevent ) 
  : m_signal_process_id    ( inevent.signal_process_id() ),
    m_event_number         ( inevent.event_number() ),
    m_mpi                  ( inevent.mpi() ),
    m_event_scale          ( inevent.event_scale() ),
    m_alphaQCD             ( inevent.alphaQCD() ),
    m_alphaQED             ( inevent.alphaQED() ),
    m_signal_process_vertex( inevent.m_signal_process_vertex ),
    m_beam_particle_1      ( inevent.m_beam_particle_1 ),
    m_beam_particle_2      ( inevent.m_beam_particle_2 ),
    m_weights              ( inevent.m_weights ),
    m_random_states        ( inevent.m_random_states ),
    m_vertices             ( inevent.m_vertices ),
    m_particles            ( inevent.m_particles ),
    m_cross_section        ( inevent.cross_section() ? new GenCrossSection(*inevent.cross_section()) : 0 ),
    m_heavy_ion            ( inevent.heavy_ion() ? new HeavyIon(*inevent.heavy_ion()) : 0 ),
    m_pdf_info             ( inevent.pdf_info() ? new PdfInfo(*inevent.pdf_info()) : 0 ),
    m_momentum_unit        ( inevent.momentum_unit() ),
    m_position_unit        ( inevent.length_unit() )
{}

GenEvent& GenEvent::operator=( const GenEvent& inevent ) 
{
    /// best practices implementation
    GenEvent tmp( inevent );
    swap( tmp );
    return *this;
}

GenEvent::~GenEvent() 
{
    /// Deep destructor.
    /// deletes the associated GenCrossSection, HeavyIon, and PdfInfo
    delete m_cross_section;
    delete m_heavy_ion;
    delete m_pdf_info;
}

void GenEvent::swap( GenEvent & other )
{
    // if a container has a swap method, use that for improved performance
    std::swap(m_signal_process_id    , other.m_signal_process_id    );
    std::swap(m_event_number         , other.m_event_number         );
    std::swap(m_mpi                  , other.m_mpi                  );
    std::swap(m_event_scale          , other.m_event_scale          );
    std::swap(m_alphaQCD             , other.m_alphaQCD             );
    std::swap(m_alphaQED             , other.m_alphaQED             );
    std::swap(m_signal_process_vertex, other.m_signal_process_vertex);
    std::swap(m_beam_particle_1      , other.m_beam_particle_1      );
    std::swap(m_beam_particle_2      , other.m_beam_particle_2      );
    m_weights.swap(                    other.m_weights              );
    m_random_states.swap(              other.m_random_states        );
    m_vertices.swap(                   other.m_vertices             );
    m_particles.swap(                  other.m_particles            );
    std::swap(m_cross_section        , other.m_cross_section        );
    std::swap(m_heavy_ion            , other.m_heavy_ion            );
    std::swap(m_pdf_info             , other.m_pdf_info             );
    std::swap(m_momentum_unit        , other.m_momentum_unit        );
    std::swap(m_position_unit        , other.m_position_unit        );
}

int GenEvent::signal_process_id() const 
{ return m_signal_process_id; }

int GenEvent::event_number() const { return m_event_number; }

int GenEvent::mpi() const { return m_mpi; }

double GenEvent::event_scale() const { return m_event_scale; }

double GenEvent::alphaQCD() const { return m_alphaQCD; }

double GenEvent::alphaQED() const { return m_alphaQED; }

GenVertex const & GenEvent::signal_process_vertex() const {
    /// returns a const reference to the signal process vertex
    return m_vertices[m_signal_process_vertex];
}  

WeightContainer& GenEvent::weights() { return m_weights; }

const WeightContainer& GenEvent::weights() const 
{ return m_weights; }

GenCrossSection const * GenEvent::cross_section() const 
{ return m_cross_section; }

GenCrossSection*  GenEvent::cross_section()  
{ return m_cross_section; }

HeavyIon const * GenEvent::heavy_ion() const 
{ return m_heavy_ion; }

HeavyIon*  GenEvent::heavy_ion()  
{ return m_heavy_ion; }

PdfInfo const * GenEvent::pdf_info() const 
{ return m_pdf_info; }

PdfInfo*  GenEvent::pdf_info()  
{ return m_pdf_info; }

const std::vector<long>& GenEvent::random_states() const 
{ return m_random_states; }

void GenEvent::set_signal_process_id( int id )
{ m_signal_process_id = id; }

void GenEvent::set_event_number( int eventno )
{ m_event_number = eventno; }

void GenEvent::set_mpi( int nmpi )
{ m_mpi = nmpi; }


void GenEvent::set_event_scale( double sc ) { m_event_scale = sc; }

void GenEvent::set_alphaQCD( double a ) { m_alphaQCD = a; }

void GenEvent::set_alphaQED( double a ) { m_alphaQED = a; }

void GenEvent::set_signal_process_vertex( GenVertex& vtx ) {
    if ( vtx.event_index() > 0 ) {
       m_signal_process_vertex = vtx.event_index();
    } else {
       m_signal_process_vertex = -1;
    }
}

void GenEvent::set_signal_process_vertex( std::size_t ind ) {
    m_signal_process_vertex = ind;
}

void GenEvent::set_cross_section( const GenCrossSection& xs )
{ 
    delete m_cross_section;
    m_cross_section = new GenCrossSection(xs); 
}

void GenEvent::set_heavy_ion( const HeavyIon& ion )
{ 
    delete m_heavy_ion;
    m_heavy_ion = new HeavyIon(ion); 
}

void GenEvent::set_pdf_info( const PdfInfo& p )
{ 
    delete m_pdf_info;
    m_pdf_info = new PdfInfo(p); 
}

void GenEvent::set_random_states( const std::vector<long>&
					 randomstates )
{ m_random_states = randomstates; }

bool  GenEvent::valid_beam_particles() const {
    // if the indices are defined, that should be sufficient
    if( m_beam_particle_1 < 0 || m_beam_particle_2 < 0) return false;
    return true;
}

std::pair<HepMC::GenParticle,HepMC::GenParticle> GenEvent::beam_particles() const {
    return std::pair<GenParticle,GenParticle> (m_particles[m_beam_particle_1], 
                                               m_particles[m_beam_particle_2]);
}

bool GenEvent::is_valid() const {
    /// A GenEvent is presumed valid if it has both associated
    /// particles and vertices.   No other information is checked.
    if ( vertices_empty() ) return false;
    if ( particles_empty() ) return false;
    return true;
}

int GenEvent::particles_size() const {
    return (int)m_particles.size();
}
bool GenEvent::particles_empty() const {
    return (bool)m_particles.empty();
}
int GenEvent::vertices_size() const {
    return (int)m_vertices.size();
}
bool GenEvent::vertices_empty() const {
    return (bool)m_vertices.empty();
}

bool GenEvent::add_vertex( GenVertex& vtx ) {
    /// returns true if successful - generally will only return false
    /// if the inserted vertex is already included in the event.
    // if vtx previously pointed to another GenEvent, remove it from that
    // GenEvent's list
    if ( vtx.parent_event() && vtx.parent_event() != this ) {
	bool remove_status = vtx.parent_event()->remove_vertex( vtx );
	if ( !remove_status ) {	       
	    std::cerr << "GenEvent::add_vertex ERROR "
		      << "GenVertex::parent_event points to \n"
		      << "an event that does not point back to the "
		      << "GenVertex. \n This probably indicates a deeper "
		      << "problem. " << std::endl;
	}
    }
    // if event_index is set at this point, the vertex is already in the event
    if ( vtx.event_index() > 0 ) return false;
    // add vertex to vector
    m_vertices.push_back(vtx);
    vtx.set_parent_event_( this );
    return true;
}

bool GenEvent::remove_vertex(GenVertex& vtx  ) {
    return remove_vertex( vtx.event_index() );
}

bool GenEvent::remove_vertex( std::size_t ind ) {
    /// returns True if an entry existed and was erased
    if ( m_signal_process_vertex == ind ) m_signal_process_vertex = -1;
    if ( m_vertices[ind].parent_event() == this ) m_vertices[ind].set_parent_event_( 0 );
    if( ind < 1  ) return false;
    m_particles.remove(ind);
    return true;
}

bool GenEvent::remove_particle( GenParticle& p ) {
    return remove_particle( p.event_index() );
}

bool GenEvent::remove_particle( std::size_t ind ) {
    /// returns True if an entry existed and was erased
    if ( m_beam_particle_1 == ind ) m_beam_particle_1 = -1;
    if ( m_beam_particle_2 == ind ) m_beam_particle_2 = -1;
    if ( m_particles[ind].production_vertex_index() >= 0 ) 
        m_particles[ind].production_vertex().remove_particle_in(m_particles[ind]);
    if ( m_particles[ind].end_vertex_index() >= 0 ) 
       m_particles[ind].end_vertex().remove_particle_out(m_particles[ind]);
    if ( m_particles[ind].parent_event() == this ) m_particles[ind].set_parent_event_( 0 );
    if( ind < 1 ) return false;
    m_particles.remove(ind);
    return true;
}

void GenEvent::clear() 
{
    /// remove all information from the event
    /// remove existing objects and set pointers to null
    ///
    delete m_cross_section;
    m_cross_section = 0;
    delete m_heavy_ion;
    m_heavy_ion = 0;
    delete m_pdf_info;
    m_pdf_info = 0;
    m_signal_process_id = 0;
    m_event_number = 0;
    m_mpi = -1;
    m_event_scale = -1;
    m_alphaQCD = -1;
    m_alphaQED = -1;
    m_signal_process_vertex = -1;
    m_beam_particle_1 = -1;
    m_beam_particle_2 = -1;
    // empty the containers
    m_weights.clear();
    m_random_states.clear();
    m_vertices.clear();
    m_particles.clear();
    // resetting unit information
    m_momentum_unit = Units::default_momentum_unit();
    m_position_unit = Units::default_length_unit();
    return;
}

bool  GenEvent::set_beam_particles(GenParticle& bp1, GenParticle& bp2) {
    return set_beam_particles(bp1.event_index(),bp2.event_index());
}

bool  GenEvent::set_beam_particles(std::size_t bp1, std::size_t bp2) {
    m_beam_particle_1 = bp1;
    m_beam_particle_2 = bp2;
    if( bp1 < 0 || bp1 >= m_particles.size() ) return false;
    if( bp2 < 0 || bp2 >= m_particles.size() ) return false;
    return true;
}

bool  GenEvent::set_beam_particles(std::pair<std::size_t, std::size_t> const & bp) {
    return set_beam_particles(bp.first,bp.second);
}

// ------- iterators ---------------

GenEvent::vertex_const_iterator 
GenEvent::vertices_begin() const { 
    return m_vertices.begin(); 
}

GenEvent::vertex_const_iterator 
GenEvent::vertices_end() const { 
    return m_vertices.end(); 
}

GenEvent::particle_const_iterator 
GenEvent::particles_begin() const { 
    return m_particles.begin();
}

GenEvent::particle_const_iterator 
GenEvent::particles_end() const {	
    return m_particles.end(); 
}

GenEvent::vertex_iterator 
GenEvent::vertices_begin() { 
    return m_vertices.begin(); 
}

GenEvent::vertex_iterator 
GenEvent::vertices_end() { 
    return m_vertices.end(); 
}

GenEvent::particle_iterator 
GenEvent::particles_begin() { 
    return m_particles.begin();
}

GenEvent::particle_iterator 
GenEvent::particles_end() {	
    return m_particles.end(); 
}

GenVertex& GenEvent::lookup_vertex( std::size_t ind ) {
    //if( ind < 0 || ind >= m_vertices.size() ) return 0;
    return m_vertices[ind];
}

GenParticle& GenEvent::lookup_particle( std::size_t ind ) {
    //if( ind < 0 || ind >= m_particles.size() ) return 0;
    return m_particles[ind];
}

// ----------- protected methods ------------------
// Following methods intended for use by GenParticle/Vertex classes:
// In general there is no reason they should be used elsewhere.

std::size_t GenEvent::add_particle( GenParticle& p ) {
    m_particles.push_back( p );
    std::size_t ind = m_particles.size() - 1;
    p.set_barcode_(ind);
    return ind;
}

// ------- units ---------------

Units::MomentumUnit GenEvent::momentum_unit() const {
    return m_momentum_unit; 
}
Units::LengthUnit   GenEvent::length_unit()   const {
    return m_position_unit; 
}

void GenEvent::use_units( Units::MomentumUnit new_m, Units::LengthUnit new_l ) { 
   use_momentum_unit( new_m );
   use_length_unit( new_l );
}

void GenEvent::use_units( std::string& new_m, std::string& new_l ) { 
   use_momentum_unit( new_m );
   use_length_unit( new_l );
}
    
void GenEvent::define_units( Units::MomentumUnit new_m, Units::LengthUnit new_l ) { 
    m_momentum_unit = new_m; 
    m_position_unit = new_l; 
}
    
void GenEvent::define_units( std::string& new_m, std::string& new_l ) { 

    if     ( new_m == "MEV" ) m_momentum_unit = Units::MEV ;
    else if( new_m == "GEV" ) m_momentum_unit = Units::GEV ;
    else std::cerr << "GenEvent::define_units ERROR: use either MEV or GEV\n";

    if     ( new_l == "MM" ) m_position_unit = Units::MM ;
    else if( new_l == "CM" ) m_position_unit = Units::CM ;
    else std::cerr << "GenEvent::define_units ERROR: use either MM or CM\n";

}

// ----------- internal unit methods ------------------

bool GenEvent::use_momentum_unit( Units::MomentumUnit newunit ) { 
    // currently not exception-safe. 
    // Easy to fix, though, if needed.
    if ( m_momentum_unit != newunit ) { 
	const double factor = Units::conversion_factor( m_momentum_unit, newunit );
	// multiply all momenta by 'factor',  
	// loop is entered only if particle list is not empty
        for ( particle_iterator p = m_particles.begin();
                                p != m_particles.end(); ++p ) 
        {
	    (*p).convert_momentum(factor);
        }
	// ... 
	m_momentum_unit = newunit; 
    }
    return true; 
}

bool GenEvent::use_length_unit( Units::LengthUnit newunit ) { 
    // currently not exception-safe. 
    // Easy to fix, though, if needed.
    if ( m_position_unit != newunit ) { 
	const double factor = Units::conversion_factor( m_position_unit, newunit );
	// multiply all lengths by 'factor', 
	// loop is entered only if vertex list is not empty
	for ( vertex_iterator vtx = m_vertices.begin();
	                      vtx != m_vertices.end(); ++vtx ) {
	    (*vtx).convert_position(factor);
	}
	// ... 
	m_position_unit = newunit; 
    } 
    return true; 
}  

bool GenEvent::use_momentum_unit( std::string& newunit ) { 
    if     ( newunit == "MEV" ) return use_momentum_unit( Units::MEV );
    else if( newunit == "GEV" ) return use_momentum_unit( Units::GEV );
    else std::cerr << "GenEvent::use_momentum_unit ERROR: use either MEV or GEV\n";
    return false;
}

bool GenEvent::use_length_unit( std::string& newunit ) { 
    if     ( newunit == "MM" ) return use_length_unit( Units::MM );
    else if( newunit == "CM" ) return use_length_unit( Units::CM );
    else std::cerr << "GenEvent::use_length_unit ERROR: use either MM or CM\n";
    return false;
}  


} // HepMC
