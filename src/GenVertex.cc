
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenEvent.h"

#include <iomanip>       // needed for formatted output

namespace HepMC {

GenVertex::GenVertex( const FourVector& position,
		      int id, const WeightContainer& weights ) 
    : m_position(position),
      m_particles_in(),
      m_particles_out(),
      m_event_index(-1),
      m_id(id), 
      m_weights(weights), 
      m_barcode(0),
      m_event(0)
{}

GenVertex::GenVertex( const GenVertex& invertex ) 
: m_position     ( invertex.position() ),
  m_particles_in (),
  m_particles_out(),
  m_event_index  ( invertex.event_index() ),
  m_id           ( invertex.id() ),
  m_weights      ( invertex.weights() ),
  m_barcode      ( invertex.barcode() ) ,
  m_event        ( invertex.parent_event() )
{
  // need to make sure that the order is unchanged
  for( size_t i = 0; i < invertex.particles_in_size(); ++i ) {
      m_particles_in[i] = invertex.lookup_incoming_particle(i).event_index();
  }
  for( size_t i = 0; i < invertex.particles_out_size(); ++i ) {
      m_particles_out[i] = invertex.lookup_outgoing_particle(i).event_index();
  }
}

GenVertex::~GenVertex() {
    if ( parent_event() ) parent_event()->remove_vertex(m_event_index);
}

void GenVertex::swap( GenVertex & other)
{
    m_position.swap(          other.m_position );
    m_particles_in.swap(      other.m_particles_in );
    m_particles_out.swap(     other.m_particles_out );
    std::swap( m_event_index, other.m_event_index );
    std::swap(          m_id, other.m_id );
    m_weights.swap(           other.m_weights );
    std::swap(     m_barcode, other.m_barcode );
    std::swap(       m_event, other.m_event );
}

GenVertex& GenVertex::operator=( const GenVertex& invertex ) {
    // best practices implementation
    GenVertex tmp( invertex );
    swap( tmp );
    return *this;
}

GenVertex::particles_in_const_iterator 
GenVertex::particles_in_const_begin() const { 
    return m_particles_in.begin(); 
}

GenVertex::particles_in_const_iterator 
GenVertex::particles_in_const_end() const { 
    return m_particles_in.end(); 
}

GenVertex::particles_out_const_iterator 
GenVertex::particles_out_const_begin() const { 
    return m_particles_out.begin();
}

GenVertex::particles_out_const_iterator 
GenVertex::particles_out_const_end() const {	
    return m_particles_out.end(); 
}

size_t GenVertex::particles_in_size() const {
    return m_particles_in.size(); 
}

size_t GenVertex::particles_out_size() const {
    return m_particles_out.size(); 
}	

GenVertex::particles_in_iterator GenVertex::particles_in_begin() { 
    return m_particles_in.begin(); 
}

GenVertex::particles_in_iterator GenVertex::particles_in_end(){ 
    return m_particles_in.end(); 
}

GenVertex::particles_out_iterator GenVertex::particles_out_begin(){ 
    return m_particles_out.begin();
}

GenVertex::particles_out_iterator GenVertex::particles_out_end(){	
    return m_particles_out.end(); 
}

GenParticle& GenVertex::lookup_incoming_particle( size_t ind ) const {
    return m_event->lookup_particle(ind);
}

GenParticle& GenVertex::lookup_outgoing_particle( size_t ind ) const {
    return m_event->lookup_particle(ind);
}

bool GenVertex::operator==( const GenVertex& a ) const {
    /// Returns true if the positions and the particles in the lists of a 
    ///  and this are identical. Does not compare barcodes.
    /// Since GenVertex contains a vector of indices, 
    /// it is sufficient to verify that the indices are the same.
    //
    if ( a.position() !=  this->position() ) return false;
    // if the size of the inlist differs, return false.
    if ( a.particles_in_size() !=  this->particles_in_size() ) return false;
    // if the size of the outlist differs, return false.
    if ( a.particles_out_size() !=  this->particles_out_size() ) return false;
    // loop over the inlist and ensure particles are identical
    //   (only do this if the lists aren't empty - we already know
    //   if one isn't, then other isn't either!)
    if ( a.particles_in_const_begin() != a.particles_in_const_end() ) {
	for ( GenVertex::particles_in_const_iterator 
		  ia = a.particles_in_const_begin(),
		  ib = this->particles_in_const_begin();
	      ia != a.particles_in_const_end(); ia++, ib++ ){
	    if ( *ia != *ib ) return false;
	}
    }
    // loop over the outlist and ensure particles are identical
    //   (only do this if the lists aren't empty - we already know
    //   if one isn't, then other isn't either!)
    if ( a.particles_out_const_begin() != a.particles_out_const_end() ) {
	for ( GenVertex::particles_out_const_iterator 
		  ia = a.particles_out_const_begin(),
		  ib = this->particles_out_const_begin();
	      ia != a.particles_out_const_end(); ia++, ib++ ){
	    if ( *ia != *ib ) return false;
	}
    }
    return true;
}

bool GenVertex::operator!=( const GenVertex& a ) const {
    // Returns true if the positions and lists of a and this are not equal.
    return !( a == *this );
}  

void GenVertex::print( std::ostream& ostr ) const {
    // find the current stream state
    std::ios_base::fmtflags orig = ostr.flags();
    std::streamsize prec = ostr.precision();
    if ( barcode()!=0 ) {
	if ( position() != FourVector(0,0,0,0) ) {
	    ostr << "Vertex:";
	    ostr.width(9);
	    ostr << barcode();
	    ostr << " ID:";
	    ostr.width(5);
	    ostr << id();
	    ostr << " (X,cT)=";
	    ostr.width(9);
            ostr.precision(2);
            ostr.setf(std::ios::scientific, std::ios::floatfield);
	    ostr.setf(std::ios_base::showpos);
	    ostr << position().x() << ",";
	    ostr.width(9);
	    ostr << position().y() << ",";
	    ostr.width(9);
	    ostr << position().z() << ",";
	    ostr.width(9);
	    ostr << position().t();
            ostr.setf(std::ios::fmtflags(0), std::ios::floatfield);
	    ostr.unsetf(std::ios_base::showpos);
	    ostr << std::endl;
	} else {
	    ostr << "GenVertex:";
	    ostr.width(9);
	    ostr << barcode();
	    ostr << " ID:";
	    ostr.width(5);
	    ostr << id();
	    ostr << " (X,cT):0";
	    ostr << std::endl;
	}
    } else {
	// If the vertex doesn't have a unique barcode assigned, then
	//  we print its memory address instead... so that the
	//  print out gives us a unique tag for the particle.
	if ( position() != FourVector(0,0,0,0) ) {
	    ostr << "Vertex:";
	    ostr.width(9);
	    ostr << (void*)this;
	    ostr << " ID:";
	    ostr.width(5);
	    ostr << id();
	    ostr << " (X,cT)=";
	    ostr.width(9);
            ostr.precision(2);
            ostr.setf(std::ios::scientific, std::ios::floatfield);
	    ostr.setf(std::ios_base::showpos);
	    ostr << position().x();
	    ostr.width(9);
	    ostr << position().y();
	    ostr.width(9);
	    ostr << position().z();
	    ostr.width(9);
	    ostr << position().t();
            ostr.setf(std::ios::fmtflags(0), std::ios::floatfield);
	    ostr.unsetf(std::ios_base::showpos);
	    ostr << std::endl;
	} else {
	    ostr << "GenVertex:";
	    ostr.width(9);
	    ostr << (void*)this;
	    ostr << " ID:";
	    ostr.width(5);
	    ostr << id();
	    ostr << " (X,cT):0";
	    ostr << std::endl;
	}
    }

    // print the weights if there are any
    if ( ! weights().empty() ) {
	ostr << " Wgts(" << weights().size() << ")=";
	for ( WeightContainer::const_iterator wgt = weights().begin();
	      wgt != weights().end(); wgt++ ) { ostr << *wgt << " "; }
	ostr << std::endl;
    }
    // print out all the incoming, then outgoing particles
    for ( particles_in_const_iterator part1 = particles_in_const_begin();
	  part1 != particles_in_const_end(); part1++ ) {
	if ( part1 == particles_in_const_begin() ) {
	    ostr << " I:";
	    ostr.width(2);
	    ostr << m_particles_in.size();
	} else { ostr << "     "; }
	//m_event->lookup_particle(*part1)->print( ostr );  //uncomment for long debugging printout
	ostr << m_event->lookup_particle(*part1) << std::endl;
    }
    for ( particles_out_const_iterator part2 = particles_out_const_begin();
	  part2 != particles_out_const_end(); part2++ ) {
	if ( part2 == particles_out_const_begin() ) { 
	    ostr << " O:";
	    ostr.width(2);
	    ostr << m_particles_out.size();
	} else { ostr << "     "; }
	//m_event->lookup_particle(*part2)->print( ostr ); // uncomment for long debugging printout
 	ostr << m_event->lookup_particle(*part2) << std::endl;
   }
    // restore the stream state
    ostr.flags(orig);
    ostr.precision(prec);
}





    /// send vertex information to ostr for printing
    /// this is a friend method
    std::ostream& operator<<( std::ostream& ostr, const GenVertex& vtx ) {
        if ( vtx.barcode()!=0 ) ostr << "BarCode " << vtx.barcode();
        else ostr << "Address " << &vtx;
        ostr << " (X,cT)=";
        if ( vtx.position() != FourVector(0,0,0,0)) {
            ostr << vtx.position().x() << ","
                 << vtx.position().y() << ","
                 << vtx.position().z() << ","
                 << vtx.position().t();
        } else { ostr << 0; }
        ostr << " #in:" << vtx.particles_in_size()
             << " #out:" << vtx.particles_out_size();
        return ostr;
    }






double GenVertex::check_momentum_conservation() const {
    /// finds the difference between the total momentum out and the total
    /// momentum in vectors, and returns the magnitude of this vector
    /// i.e.         returns | vec{p_in} - vec{p_out} |
    double sumpx = 0, sumpy = 0, sumpz = 0;
    for ( particles_in_const_iterator part1 = particles_in_const_begin();
	  part1 != particles_in_const_end(); part1++ ) {
	sumpx   += m_event->lookup_particle(*part1).momentum().px();
	sumpy   += m_event->lookup_particle(*part1).momentum().py();
	sumpz   += m_event->lookup_particle(*part1).momentum().pz();
    }
    for ( particles_out_const_iterator part2 = particles_out_const_begin();
	  part2 != particles_out_const_end(); part2++ ) {
	sumpx   -= m_event->lookup_particle(*part2).momentum().px();
	sumpy   -= m_event->lookup_particle(*part2).momentum().py();
	sumpz   -= m_event->lookup_particle(*part2).momentum().pz();
    }
    return sqrt( sumpx*sumpx + sumpy*sumpy + sumpz*sumpz );
}

void GenVertex::add_particle_in( GenParticle& inparticle ) {
    // if inparticle previously had a decay vertex, remove it from that
    // vertex's list
    if ( inparticle.end_vertex_index() >= 0 ) {
	inparticle.end_vertex().remove_particle_in( inparticle );
    }
    m_particles_in.push_back( m_event->add_particle( inparticle ) );
    inparticle.set_end_vertex_( -1 );
}

void GenVertex::add_particle_out( GenParticle& outparticle ) {
    // if outparticle previously had a production vertex,
    // remove it from that vertex's list
    if ( outparticle.production_vertex_index() >= 0 ) {
	outparticle.production_vertex().remove_particle_out( outparticle );
    }
    m_particles_out.push_back( m_event->add_particle( outparticle ) );
    outparticle.set_production_vertex_( -1 );
}

GenVertex::operator HepMC::FourVector() const { return position(); }

GenVertex::operator HepMC::ThreeVector() const { return point3d(); }

const FourVector & GenVertex::position() const { return m_position; }

GenEvent* GenVertex::parent_event() const { return m_event; }

ThreeVector GenVertex::point3d() const { 
    return ThreeVector(m_position.x(),m_position.y(),m_position.z()); 
}

int GenVertex::id() const { return m_id; }

int  GenVertex::barcode() const { return m_barcode; }

WeightContainer& GenVertex::weights() { return m_weights; }

const WeightContainer& GenVertex::weights() const 
{ return m_weights; }

void GenVertex::set_position( const FourVector& pos ) {
    m_position = pos;
}

void GenVertex::set_id( int pid ) { m_id = pid; }

void GenVertex::set_event_index( size_t ind ) { m_event_index = ind; }

GenParticle& GenVertex::remove_particle( GenParticle& particle ) {
    /// this searches both m_particles_in and m_particles_out
    remove_particle_out( particle );
    remove_particle_in( particle );
    return particle;
}

// ----------- protected methods ------------------

void GenVertex::remove_particle_in( GenParticle& particle ) {
    /// this finds particle in m_particles_in and removes it from that list
    if ( particle.production_vertex_index() == 0 ) return;
    m_particles_in[ particle.production_vertex().event_index() ] = -1;
    particle.set_production_vertex_(-1);
}

void GenVertex::remove_particle_out( GenParticle& particle ) {
    /// this finds particle in m_particles_out and removes it from that list
    if ( particle.end_vertex_index() == 0 ) return;
    m_particles_out[ particle.end_vertex().event_index() ] = -1;
    particle.set_end_vertex_(-1);
}

/// scale the position vector
/// this method is only for use by GenEvent
/// convert_position assumes that 4th component of the position vector 
/// is ctau rather than time and has units of length-time
void GenVertex::convert_position( const double& f ) {
    m_position = FourVector( f*m_position.x(),
                             f*m_position.y(),
                             f*m_position.z(),
                             f*m_position.t() );
}

size_t GenVertex::event_index() const
{ return m_event_index; }

void GenVertex::set_parent_event_( GenEvent* new_evt ) 
{ 
    m_event = new_evt; 
    //
    // every time a vertex's parent event changes, the map of barcodes
    //   in the new and old parent event needs to be modified to 
    //   reflect this
/*    if ( orig_evt != new_evt ) {
	if (new_evt) new_evt->set_barcode( this, barcode() );
	if (orig_evt) orig_evt->remove_vertex( this );
	// we also need to loop over all the particles which are owned by 
	//  this vertex, and remove their barcodes from the old event.
	for ( particles_in_const_iterator part1=particles_in_const_begin();
	      part1 != particles_in_const_end(); part1++ ) {
	    if ( !(*part1)->production_vertex() ) { 
		if ( orig_evt ) orig_evt->remove_barcode( *part1 );
		if ( new_evt ) new_evt->set_barcode( *part1, 
						     (*part1)->barcode() );
	    }
	}
	for ( particles_out_const_iterator
		  part2 = particles_out_const_begin();
	      part2 != particles_out_const_end(); part2++ ) {
		if ( orig_evt ) orig_evt->remove_barcode( *part2 );
		if ( new_evt ) new_evt->set_barcode( *part2, 
						     (*part2)->barcode() );
	}
    } */
}

void GenVertex::change_parent_event_( GenEvent* new_evt ) 
{ 
    //
    // this method is for use with swap
    // particles and vertices have already been exchanged, 
    // but the backpointer needs to be fixed
    //GenEvent* orig_evt = m_event;
    m_event = new_evt; 
}

void GenVertex::set_barcode_( int bc ) { m_barcode = bc; }

} // HepMC
