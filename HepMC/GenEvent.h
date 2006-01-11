//--------------------------------------------------------------------------
#ifndef HEPMC_GEN_EVENT_H
#define HEPMC_GEN_EVENT_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// Event record for MC generators (for use at any stage of generation)
//////////////////////////////////////////////////////////////////////////
//
// This class is intended as both a "container class" ( to store a MC
//  event for interface between MC generators and detector simulation )
//  and also as a "work in progress class" ( that could be used inside
//  a generator and modified as the event is built ).
//
// Iterators are provided which allow the user to easily obtain a
//  list of particles or vertices in an event --- this list can be filled
//  subject to some sort of selection criteria. Examples are given below
//  ( see HepMC::copy_if and std::copy )

namespace HepMC {
    // To create a list from an iterator, use: (i.e. for a list of particles);
    // #include <algorithm>
    //     list<GenParticle*> thelist;
    //     copy( evt->particles_begin(), evt->particles_end(), 
    //           back_inserter(thelist) );
    // to create a list subject to a condition (predicate) use:
    //     list<GenParticle*> thelist;
    //     HepMC::copy_if( evt->particles_begin(), evt->particles_end(), 
    //                     back_inserter(thelist), is_photon() );
    // where is_photon() is a predicate like:
    //     class is_photon {
    //       public:
    //         bool operator() ( const GenParticle* p ) {
    //             if ( p && p->pdg_id() == 22 ) return 1;
    //             return 0;
    //         }
    //     };
    // which the user defines herself.
    template <class InputIterator, class OutputIterator, class Predicate>
    void copy_if( InputIterator first, InputIterator last, OutputIterator out,
		  Predicate pred ) {
	for ( ; first != last; ++first ) { if ( pred(*first) ) out = *first; }
    }
} // HepMC

// Since a container of all vertices in the event is maintained, the time
//  required to loop over all vertices (or particles) is very fast -- and 
//  the user does not gain much by first making his own list.
//  (this is not true for the GenVertex:: versions of these iterators, which
//   allow you to specify the vertex starting point and range)

// Data Members:
// signal_process_id()   The integer ID that uniquely specifies this signal
//                       process, i.e. MSUB in Pythia. It is necessary to
//                       package this with each event rather than with the run
//                       because many processes may be generated within one
//                       run.
// event_number()        Strictly speaking we cannot think of any reason that
//                       an event would need to know its own event number, it
//                       is more likely something that would be assigned by
//                       a database. It is included anyway (tradition?) since
//                       we expect it may be useful for debugging. It can
//                       be reset later by a database.
// signal_process_vertex() pointer to the vertex containing the signal process
// weights()             Vector of doubles which specify th weight of the evnt,
//                       the first entry will be the "event weight" used for
//                       hit and miss etc., but a general vector is used to
//                       allow for reweighting etc. We envision a list of
//                       WeightTags to be included with a run class which
//                       would specify the meaning of the Weights .
// random_states()       Vector of integers which specify the random number 
//                       generator's state for this event. It is left to the
//                       generator to make use of this. We envision a vector of
//                       RndmStatesTags to be included with a run class which
//                       would specify the meaning of the random_states.
//
///////////////////////
// Memory allocation //
///////////////////////
// -When a vertex (particle) is added to a event (vertex), it is "adopted" 
//  and becomes the responsibility of the event (vertex) to delete that 
//  particle. 
// -objects responsible for deleting memory:
//    -events delete included vertices
//    -each vertex deletes its outgoing particles which do not have decay
//     vertices
//    -each vertex deletes its incoming particles which do not
//     have creation vertices 
//
////////////////////////
// About the Barcodes //
////////////////////////
// - each vertex or particle has a barcode, which is just an integer which
//   uniquely identifies it inside the event (i.e. there is a one to one
//   mapping between particle memory addresses and particle barcodes... and 
//   the same applied for vertices)
// - The value of a barcode has NO MEANING and NO ORDER!
//   For the user's convenience, when an event is read in via an IO_method
//   from an indexed list (like the HEPEVT common block), then the index will
//   become the barcode for that particle.
// - particle barcodes are always positive integers
//   vertex barcodes are always negative integers
//   The barcodes are chosen and set automatically when a vertex or particle
//   comes under the ownership of an event (i.e. it is contained in an event).
// - You can tell when a particle or vertex is owned, because its 
//   parent_event() return value will return a pointer to the event which owns
//   it (or null if its an orphan).
// 

#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/WeightContainer.h"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

namespace HepMC {

    class GenEvent {
	friend class GenParticle;
	friend class GenVertex;  
    public:
	GenEvent( int signal_process_id = 0, int event_number = 0,
		  GenVertex* signal_vertex = 0,
		  const WeightContainer& weights = std::vector<double>(),
		  const std::vector<long int>& randomstates
		  = std::vector<long int>() );
	GenEvent( const GenEvent& inevent );          // deep copy
	GenEvent& operator=( const GenEvent& inevent ); // deep.
	virtual ~GenEvent(); //deletes all vertices/particles in this evt
    
	void print( std::ostream& ostr = std::cout ) const; // dumps to ostr

	GenParticle* barcode_to_particle( int barCode ) const;
	GenVertex*   barcode_to_vertex(   int barCode ) const;

	////////////////////
	// access methods //
	////////////////////

	int signal_process_id() const;
	int event_number() const;
	double event_scale() const;
	double alphaQCD() const;
	double alphaQED() const;
	GenVertex* signal_process_vertex() const;

	// direct access to the weights container is allowed. 
	// Thus you can user myevt.weights()[2];
	// to access element 2 of the weights.
	// or use myevt.weights().push_back( mywgt ); to add an element.
	// and you can set the weights with myevt.weights() = myvector;
	WeightContainer&        weights();
	const WeightContainer&  weights() const;

	std::vector<long int> random_states() const;

	void set_signal_process_id( int id );
	void set_event_number( int eventno );
	void set_event_scale( double scale );
	void set_alphaQCD( double a );
	void set_alphaQED( double a );
	void set_signal_process_vertex( GenVertex* );
	void set_random_states( const std::vector<long int>& randomstates );

	int     particles_size() const;
	bool    particles_empty() const;
	int     vertices_size() const;
	bool    vertices_empty() const;

	bool    add_vertex( GenVertex* vtx );    // adds to evt and adopts
	bool    remove_vertex( GenVertex* vtx ); // erases vtx from evt, 

    public:
	///////////////////////////////
	// vertex_iterators          //
	///////////////////////////////
	// Note:  the XXX_iterator is "resolvable" as XXX_const_iterator, but 
	//  not the reverse, which is consistent with STL, 
	//  see Musser, Derge, Saini 2ndEd. p. 69,70.
	class vertex_const_iterator :
	  public std::iterator<std::forward_iterator_tag,GenVertex*,ptrdiff_t>{
	    // Iterates over all vertices in this event
	public:
	    vertex_const_iterator(
		const 
		std::map<int,GenVertex*,std::greater<int> >::const_iterator& i)
		: m_map_iterator(i) {}
	    vertex_const_iterator() {}
	    vertex_const_iterator( const vertex_const_iterator& i )
		{ *this = i; }
	    virtual ~vertex_const_iterator() {}
	    vertex_const_iterator&  operator=( const vertex_const_iterator& i )
		{ m_map_iterator = i.m_map_iterator; return *this; }
	    GenVertex* operator*(void) const { return m_map_iterator->second; }
	    vertex_const_iterator&  operator++(void)  //Pre-fix increment 
		{ ++m_map_iterator; return *this; }
	    vertex_const_iterator   operator++(int)   //Post-fix increment
		{ vertex_const_iterator out(*this); ++(*this); return out; }
	    bool  operator==( const vertex_const_iterator& a ) const
		{ return m_map_iterator == a.m_map_iterator; }
	    bool  operator!=( const vertex_const_iterator& a ) const
		{ return !(m_map_iterator == a.m_map_iterator); }
	protected:
	    std::map<int,GenVertex*,std::greater<int> >::const_iterator 
	                                                        m_map_iterator;
	};
	friend class vertex_const_iterator;
	vertex_const_iterator      vertices_begin() const
	    { return GenEvent::vertex_const_iterator( 
		m_vertex_barcodes.begin() ); }
	vertex_const_iterator      vertices_end() const
	    { return GenEvent::vertex_const_iterator(
		m_vertex_barcodes.end() ); }

	class vertex_iterator :
	  public std::iterator<std::forward_iterator_tag,GenVertex*,ptrdiff_t>{
	    // Iterates over all vertices in this event
	public:
	    vertex_iterator( 
		const 
		std::map<int,GenVertex*,std::greater<int> >::iterator& i )
		: m_map_iterator( i ) {}
	    vertex_iterator() {}
	    vertex_iterator( const vertex_iterator& i ) { *this = i; }
	    virtual ~vertex_iterator() {}
	    vertex_iterator&  operator=( const vertex_iterator& i ) {
		m_map_iterator = i.m_map_iterator;
		return *this;
	    }
	    operator vertex_const_iterator() const
		{ return vertex_const_iterator(m_map_iterator); }
	    GenVertex*        operator*(void) const
		{ return m_map_iterator->second; }
	    vertex_iterator&  operator++(void)  //Pre-fix increment 
		{ ++m_map_iterator;     return *this; }
	    vertex_iterator   operator++(int)   //Post-fix increment
		{ vertex_iterator out(*this); ++(*this); return out; }
	    bool              operator==( const vertex_iterator& a ) const
		{ return m_map_iterator == a.m_map_iterator; }
	    bool              operator!=( const vertex_iterator& a ) const
		{ return !(m_map_iterator == a.m_map_iterator); }
	protected:
	    std::map<int,GenVertex*,std::greater<int> >::iterator 
	                                                       m_map_iterator;
	};
	friend class vertex_iterator;
	vertex_iterator            vertices_begin() 
	    { return GenEvent::vertex_iterator( 
		m_vertex_barcodes.begin() ); }
        vertex_iterator            vertices_end()
	    { return GenEvent::vertex_iterator(
		m_vertex_barcodes.end() ); }

    public:
	///////////////////////////////
	// particle_iterator         //
	///////////////////////////////
	// Example of iterating over all particles in the event:
	//      for ( GenEvent::particle_const_iterator p = particles_begin();
	//            p != particles_end(); ++p ) {
	//         (*p)->print();
	//      }
	//
	class particle_const_iterator :
	  public std::iterator<std::forward_iterator_tag,GenParticle*,ptrdiff_t>{
	    // Iterates over all vertices in this event
	public:
	    particle_const_iterator(
		const std::map<int,GenParticle*>::const_iterator& i )
		: m_map_iterator(i) {}
	    particle_const_iterator() {}
	    particle_const_iterator( const particle_const_iterator& i )
		{ *this = i; }
	    virtual ~particle_const_iterator() {}
	    particle_const_iterator& operator=(
		const particle_const_iterator& i )
		{ m_map_iterator = i.m_map_iterator; return *this; }
	    GenParticle*        operator*(void) const
		{ return m_map_iterator->second; }
	    particle_const_iterator&  operator++(void)  //Pre-fix increment 
		{ ++m_map_iterator; return *this; }
	    particle_const_iterator   operator++(int)   //Post-fix increment
		{ particle_const_iterator out(*this); ++(*this); return out; }
	    bool  operator==( const particle_const_iterator& a ) const
		{ return m_map_iterator == a.m_map_iterator; }
	    bool  operator!=( const particle_const_iterator& a ) const
		{ return !(m_map_iterator == a.m_map_iterator); }
	protected:
	    std::map<int,GenParticle*>::const_iterator m_map_iterator;
	};	
	friend class particle_const_iterator;
	particle_const_iterator      particles_begin() const
	    { return GenEvent::particle_const_iterator( 
		m_particle_barcodes.begin() ); }
	particle_const_iterator      particles_end() const
	    { return GenEvent::particle_const_iterator(
		m_particle_barcodes.end() ); }

	class particle_iterator :
	  public std::iterator<std::forward_iterator_tag,GenParticle*,ptrdiff_t>{
	    // Iterates over all vertices in this event
	public:
	    particle_iterator( const std::map<int,GenParticle*>::iterator& i )
		: m_map_iterator( i ) {}
	    particle_iterator() {}
	    particle_iterator( const particle_iterator& i ) { *this = i; }
	    virtual ~particle_iterator() {}
	    particle_iterator&  operator=( const particle_iterator& i ) {
		m_map_iterator = i.m_map_iterator;
		return *this;
	    }
	    operator particle_const_iterator() const
		{ return particle_const_iterator(m_map_iterator); }
	    GenParticle*        operator*(void) const
		{ return m_map_iterator->second; }
	    particle_iterator&  operator++(void)  //Pre-fix increment 
		{ ++m_map_iterator;     return *this; }
	    particle_iterator   operator++(int)   //Post-fix increment
		{ particle_iterator out(*this); ++(*this); return out; }
	    bool              operator==( const particle_iterator& a ) const
		{ return m_map_iterator == a.m_map_iterator; }
	    bool              operator!=( const particle_iterator& a ) const
		{ return !(m_map_iterator == a.m_map_iterator); }
	protected:
	    std::map<int,GenParticle*>::iterator m_map_iterator;
	};
	friend class particle_iterator;
	particle_iterator particles_begin() 
	    { return GenEvent::particle_iterator(
		m_particle_barcodes.begin() ); }
        particle_iterator particles_end()
	    { return GenEvent::particle_iterator(
		m_particle_barcodes.end() ); }

	////////////////////////////////////////////////
    protected:
	//
	// Following methods intended for use by GenParticle/Vertex classes:
	// In general there is no reason they should be used elsewhere.
	bool         set_barcode( GenParticle* p, int suggested_barcode =0 );
	bool         set_barcode( GenVertex*   v, int suggested_barcode =0 );
	void         remove_barcode( GenParticle* p );
	void         remove_barcode( GenVertex*   v );

	static unsigned int counter(); //num GenEvent objects in memory
   	void delete_all_vertices();

    private: // data members
	int                   m_signal_process_id;
	int                   m_event_number;  
	double                m_event_scale;// energy scale, see hep-ph/0109068
	double                m_alphaQCD;   // QCD coupling, see hep-ph/0109068
	double                m_alphaQED;   // QED coupling, see hep-ph/0109068
	GenVertex*            m_signal_process_vertex;
	WeightContainer       m_weights; // weights for this event first weight
	                                 // is used by default for hit and miss
	std::vector<long int> m_random_states; // container of rndm num 
	                                       // generator states

	std::map< int,GenVertex*,std::greater<int> >   m_vertex_barcodes;
	std::map< int,GenParticle*,std::less<int> >    m_particle_barcodes;

	static unsigned int   s_counter;
    };

    ///////////////////////////
    // INLINE Access Methods //
    ///////////////////////////

    inline int GenEvent::signal_process_id() const 
    { return m_signal_process_id; }

    inline int GenEvent::event_number() const { return m_event_number; }

    inline double GenEvent::event_scale() const { return m_event_scale; }

    inline double GenEvent::alphaQCD() const { return m_alphaQCD; }

    inline double GenEvent::alphaQED() const { return m_alphaQED; }
 
    inline GenVertex* GenEvent::signal_process_vertex() const {
	// returns a (mutable) pointer to the signal process vertex
	return m_signal_process_vertex;
    }  

    inline WeightContainer& GenEvent::weights() { return m_weights; }

    inline const WeightContainer& GenEvent::weights() const 
    { return m_weights; }

    inline std::vector<long int> GenEvent::random_states() const 
    { return m_random_states; }

    inline void GenEvent::set_signal_process_id( int id )
    { m_signal_process_id = id; }

    inline void GenEvent::set_event_number( int eventno )
    { m_event_number = eventno; }


    inline void GenEvent::set_event_scale( double sc ) { m_event_scale = sc; }

    inline void GenEvent::set_alphaQCD( double a ) { m_alphaQCD = a; }

    inline void GenEvent::set_alphaQED( double a ) { m_alphaQED = a; }

    inline void GenEvent::set_signal_process_vertex( GenVertex* vtx ) {
	m_signal_process_vertex = vtx;
	if ( m_signal_process_vertex ) add_vertex( m_signal_process_vertex );
    }

    inline void GenEvent::set_random_states( const std::vector<long int>&
					     randomstates )
    { m_random_states = randomstates; }

    inline void GenEvent::remove_barcode( GenParticle* p )
    { m_particle_barcodes.erase( p->barcode() ); }

    inline void GenEvent::remove_barcode( GenVertex* v )
    { m_vertex_barcodes.erase( v->barcode() ); }

    inline GenParticle* GenEvent::barcode_to_particle( int barCode ) const
    { 
	std::map<int,GenParticle*>::const_iterator i 
	    = m_particle_barcodes.find(barCode);
	return ( i != m_particle_barcodes.end() ) ? (*i).second : 0;
    }

    inline GenVertex* GenEvent::barcode_to_vertex( int barCode ) const
    {
	std::map<int,GenVertex*,std::greater<int> >::const_iterator i 
	    = m_vertex_barcodes.find(barCode);
	return ( i != m_vertex_barcodes.end() ) ? (*i).second : 0;
    }

    inline int GenEvent::particles_size() const {
	return (int)m_particle_barcodes.size();
    }
    inline bool GenEvent::particles_empty() const {
	return (bool)m_particle_barcodes.empty();
    }
    inline int GenEvent::vertices_size() const {
	return (int)m_vertex_barcodes.size();
    }
    inline bool GenEvent::vertices_empty() const {
	return (bool)m_vertex_barcodes.empty();
    }

} // HepMC

#endif  // HEPMC_GEN_EVENT_H

//--------------------------------------------------------------------------


