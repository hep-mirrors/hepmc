//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, January 2000
// particle's flow object
//////////////////////////////////////////////////////////////////////////

#include "HepMC/Flow.h"
#include "HepMC/GenParticle.h"
#include "HepMC/GenVertex.h"

namespace HepMC {

    Flow::Flow( GenParticle* particle_owner ) 
	: m_particle_owner(particle_owner) 
    {}

    Flow::Flow( const Flow& inflow ) : 
	m_particle_owner(inflow.m_particle_owner)
    {
	/// copies both the m_icode AND the m_particle_owner
	*this = inflow;
    }

    Flow::~Flow() {
	m_icode.clear();
    }

    void Flow::print( std::ostream& ostr ) const {
	ostr << "Flow(" << m_particle_owner << "): " << *this << std::endl;
    }
    
    std::set<GenParticle*> Flow::connected_partners( int code, int code_index, 
						  int num_indices ) const {
	/// Returns all flow partners which have "code" in any  of the 
	///  num_indices beginning with index code_index.
	///  m_particle_owner is included in the result.
	///  Return is by value since the set should never be very big.
	/// EXAMPLE: if you want to find all flow partners that have the same
	///   code in indices 2,3,4 as particle p has in index 2, you would use:
	///   set<GenParticle*> result = 
	///             p->flow().connected_partners(p->flow().icode(2),2,3);
	//
	std::set<GenParticle*> output;
	for ( int i = code_index; i!=code_index+num_indices; ++i ) {
	    if ( icode(i)==code ) {
		output.insert(m_particle_owner);
		connected_partners( &output, code, code_index, num_indices );
		break;
	    } 
	}
	return output;
    }

    void Flow::connected_partners( std::set<GenParticle*>* output, int code, 
				   int code_index, int num_indices ) const
    {
	/// protected: for recursive use by Flow::connected_partners()
	//
    	if ( !m_particle_owner ) return; // nothing to do
	// look for connected partners joined to this m_particle_owner
	// through its end_vertex
	if ( m_particle_owner->end_vertex() ) {
	    for ( GenVertex::particle_iterator p 
		     = m_particle_owner->end_vertex()->particles_begin(family);
		  p != m_particle_owner->end_vertex()->particles_end(family);
		  ++p ) {
		// if the particle has the correct flow code and is not yet in 
		// the set, then we recursively call connected_partners
		for ( int index = code_index; index!=code_index+num_indices; 
		      ++index ){
		    if ( (*p)->flow(index)==code &&
			 output->insert(*p).second ) {
			(*p)->flow().connected_partners( output, code,
							 code_index, 
							 num_indices );
		    }
		}
	    }
	}
	// same for production_vertex
	if ( m_particle_owner->production_vertex() ) {
	    for ( GenVertex::particle_iterator p 
		      = m_particle_owner->production_vertex()->
		      particles_begin( family );
		  p != m_particle_owner->production_vertex()->
		      particles_end( family ); ++p ) {
		// if the particle has the correct flow code and is not yet in 
		// the set, then we recursively call connected_partners
		for ( int index = code_index; index!=code_index+num_indices; 
		      ++index ){
		    if ( (*p)->flow(index)==code &&
			 output->insert(*p).second ) {
			(*p)->flow().connected_partners( output, code,
							 code_index, 
							 num_indices );
		    }
		}
	    }
	}
    }

    std::set<GenParticle*> Flow::dangling_connected_partners( int code, 
				     int code_index, int num_indices ) const {
	std::set<GenParticle*> output;
	std::set<GenParticle*> visited_particles;
	for ( int i = code_index; i!=code_index+num_indices; ++i ) {
	    if ( icode(i)==code ) {
		visited_particles.insert(m_particle_owner);
		dangling_connected_partners( &output, &visited_particles, code,
					     code_index, num_indices );
		break;
	    }
	}
	return output;
    }

    void Flow::dangling_connected_partners( std::set<GenParticle*>* output, 
					    std::set<GenParticle*>* 
					    visited_particles,
					    int code, int code_index, 
					    int num_indices ) const 
    {
	/// protected: for recursive use by Flow::dangling_connected_partners
	//
    	if ( !m_particle_owner ) return; // nothing to do
	int count_partners = 0;
	// look for connected partners joined to this m_particle_owner
	// through its end_vertex
	if ( m_particle_owner->end_vertex() ) {
	    for ( GenVertex::particle_iterator p 
		     = m_particle_owner->end_vertex()->particles_begin(family);
		  p != m_particle_owner->end_vertex()->particles_end(family);
		  ++p ) {
		// if the particle has the correct flow code and is not yet in 
		// the set, then we recursively call connected_partners
		for ( int index = code_index; index!=code_index+num_indices; 
		      ++index ){
		    if ( (*p)->flow(index)==code ) {
			if ( *p!=m_particle_owner ) ++count_partners;
			if ( visited_particles->insert(*p).second ) {
			    (*p)->flow().dangling_connected_partners( output, 
					             visited_particles, code,
						     code_index, num_indices );

			}
		    }		
		}
	    }
	}
	// same for production_vertex
	if ( m_particle_owner->production_vertex() ) {
	    for ( GenVertex::particle_iterator p = m_particle_owner->
						production_vertex()->
						particles_begin( family );
		  p != m_particle_owner->production_vertex()->
						particles_end( family ); 
		  ++p ) {
		// if the particle has the correct flow code and is not yet in 
		// the set, then we recursively call connected_partners
		for ( int index = code_index; index!=code_index+num_indices; 
		      ++index ){
		    if ( (*p)->flow(index)==code ) {
			if ( *p!=m_particle_owner ) ++count_partners;
			if ( visited_particles->insert(*p).second ) {
			    (*p)->flow().dangling_connected_partners( output, 
					             visited_particles, code,
						     code_index, num_indices );

			}
		    }
		}
	    }
	}
	if ( count_partners <= 1 ) output->insert( m_particle_owner );
    }
	
    /////////////
    // Friends //
    /////////////

    std::ostream& operator<<( std::ostream& ostr, const Flow& f ) {
        /// send Flow informatin to ostr for printing
	ostr << f.m_icode.size();
	for ( std::map<int,int>::const_iterator i = f.m_icode.begin();
	      i != f.m_icode.end(); ++i ) {
	    ostr << " " << (*i).first << " " << (*i).second;
	}
	return ostr;
    }

} // HepMC


















