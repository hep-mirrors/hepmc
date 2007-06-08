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

///
/// \namespace HepMC
/// All classes in the HepMC packages are in the HepMC namespace 
///
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

    /// define the type of iterator to use
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
#include "HepMC/HeavyIon.h"
#include "HepMC/PdfInfo.h"
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

namespace HepMC {

    //! The GenEvent class is the core of HepMC

    ///
    /// \class GenEvent 
    /// HepMC::GenEvent contains information about generated particles.
    /// GenEvent is structured as a set of vertices which contain the particles.
    ///
    class GenEvent {
	friend class GenParticle;
	friend class GenVertex;  
    public:
        /// default constructor creates null pointers to HeavyIon and PdfInfo
	GenEvent( int signal_process_id = 0, int event_number = 0,
		  GenVertex* signal_vertex = 0,
		  const WeightContainer& weights = std::vector<double>(),
		  const std::vector<long int>& randomstates = std::vector<long int>() );
        /// explicit constructor that takes HeavyIon and PdfInfo
	GenEvent( int signal_process_id, int event_number,
		  GenVertex* signal_vertex, const WeightContainer& weights,
		  const std::vector<long int>& randomstates,
		  const HeavyIon& ion, const PdfInfo& pdf );
	GenEvent( const GenEvent& inevent );          //!< deep copy
	GenEvent& operator=( const GenEvent& inevent ); //!< make a deep copy
	virtual ~GenEvent(); //!<deletes all vertices/particles in this evt
    
	void print( std::ostream& ostr = std::cout ) const; //!< dumps to ostr

        /// assign a barcode to a particle
	GenParticle* barcode_to_particle( int barCode ) const;
        /// assign a barcode to a vertex
	GenVertex*   barcode_to_vertex(   int barCode ) const;

	////////////////////
	// access methods //
	////////////////////

	int signal_process_id() const; //!<  unique signal process id
	int event_number() const; //!<  event number
	double event_scale() const; //!< energy scale, see hep-ph/0109068
	double alphaQCD() const; //!<  QCD coupling, see hep-ph/0109068
	double alphaQED() const; //!<  QED coupling, see hep-ph/0109068
        /// pointer to the vertex containing the signal process
	GenVertex* signal_process_vertex() const;

	/// direct access to the weights container is allowed. 
	/// Thus you can use myevt.weights()[2];
	/// to access element 2 of the weights.
	/// or use myevt.weights().push_back( mywgt ); to add an element.
	/// and you can set the weights with myevt.weights() = myvector;
	WeightContainer&        weights(); //!< direct access to WeightContainer
	const WeightContainer&  weights() const; //!< direct access to WeightContainer

	/// access the HeavyIon container if it exists
	HeavyIon* const          heavy_ion() const;
	HeavyIon*                heavy_ion();
	/// access the PdfInfo container if it exists
	PdfInfo* const           pdf_info() const;
	PdfInfo*                 pdf_info();

	/// vector of integers containing information about the random state
	std::vector<long int> random_states() const;

	void set_signal_process_id( int id ); //!< set unique signal process id
	void set_event_number( int eventno ); //!< set event number
	void set_event_scale( double scale ); //!< set energy scale
	void set_alphaQCD( double a ); //!< set QCD coupling
	void set_alphaQED( double a ); //!< set QED coupling

        /// set pointer to the vertex containing the signal process
	void set_signal_process_vertex( GenVertex* );
	/// provide random state information
	void set_random_states( const std::vector<long int>& randomstates );

	/// provide a pointer to the HeavyIon container
	void set_heavy_ion( const HeavyIon& ion );
	/// provide a pointer to the PdfInfo container
	void set_pdf_info( const PdfInfo& p );

        /// how many particle barcodes exist?
	int     particles_size() const;
        /// return true if there are no particle barcodes
	bool    particles_empty() const;
        /// how many vertex barcodes exist?
	int     vertices_size() const;
        /// return true if there are no vertex barcodes
	bool    vertices_empty() const;

	////////////////////
	// change methods //
	////////////////////

	bool    add_vertex( GenVertex* vtx );    //!< adds to evt and adopts
	bool    remove_vertex( GenVertex* vtx ); //!< erases vtx from evt
	void    clear();                         //!< empties the entire event

    public:
	///////////////////////////////
	// vertex_iterators          //
	///////////////////////////////
	// Note:  the XXX_iterator is "resolvable" as XXX_const_iterator, but 
	//  not the reverse, which is consistent with STL, 
	//  see Musser, Derge, Saini 2ndEd. p. 69,70.

	//!  const vertex iterator

	/// \class  vertex_const_iterator
	/// HepMC::GenEvent::vertex_const_iterator
	/// is used to iterate over all vertices in the event.
	class vertex_const_iterator :
	  public std::iterator<std::forward_iterator_tag,GenVertex*,ptrdiff_t>{
	    // Iterates over all vertices in this event
	public:
	    /// constructor requiring vertex information
	    vertex_const_iterator(
		const 
		std::map<int,GenVertex*,std::greater<int> >::const_iterator& i)
		: m_map_iterator(i) {}
	    vertex_const_iterator() {}
	    /// copy constructor
	    vertex_const_iterator( const vertex_const_iterator& i )
		{ *this = i; }
	    virtual ~vertex_const_iterator() {}
	    /// make a copy
	    vertex_const_iterator&  operator=( const vertex_const_iterator& i )
		{ m_map_iterator = i.m_map_iterator; return *this; }
	    /// return a pointer to a GenVertex
	    GenVertex* operator*(void) const { return m_map_iterator->second; }
	    /// Pre-fix increment
	    vertex_const_iterator&  operator++(void)  //Pre-fix increment 
		{ ++m_map_iterator; return *this; }
	    /// Post-fix increment
	    vertex_const_iterator   operator++(int)   //Post-fix increment
		{ vertex_const_iterator out(*this); ++(*this); return out; }
	    /// equality
	    bool  operator==( const vertex_const_iterator& a ) const
		{ return m_map_iterator == a.m_map_iterator; }
	    /// inequality
	    bool  operator!=( const vertex_const_iterator& a ) const
		{ return !(m_map_iterator == a.m_map_iterator); }
	protected:
	    /// const iterator to a vertex map
	    std::map<int,GenVertex*,std::greater<int> >::const_iterator 
	                                                        m_map_iterator;
	};
	friend class vertex_const_iterator;
	/// begin vertex iteration
	vertex_const_iterator      vertices_begin() const
	    { return GenEvent::vertex_const_iterator( 
		m_vertex_barcodes.begin() ); }
	/// end vertex iteration
	vertex_const_iterator      vertices_end() const
	    { return GenEvent::vertex_const_iterator(
		m_vertex_barcodes.end() ); }


	//!  non-const vertex iterator

	/// \class  vertex_iterator
	/// HepMC::GenEvent::vertex_iterator
	/// is used to iterate over all vertices in the event.
	class vertex_iterator :
	  public std::iterator<std::forward_iterator_tag,GenVertex*,ptrdiff_t>{
	    // Iterates over all vertices in this event
	public:
	    /// constructor requiring vertex information
	    vertex_iterator( 
		const 
		std::map<int,GenVertex*,std::greater<int> >::iterator& i )
		: m_map_iterator( i ) {}
	    vertex_iterator() {}
	    /// copy constructor
	    vertex_iterator( const vertex_iterator& i ) { *this = i; }
	    virtual ~vertex_iterator() {}
	    /// make a copy
	    vertex_iterator&  operator=( const vertex_iterator& i ) {
		m_map_iterator = i.m_map_iterator;
		return *this;
	    }
	    /// const vertex iterator
	    operator vertex_const_iterator() const
		{ return vertex_const_iterator(m_map_iterator); }
	    /// return a pointer to a GenVertex
	    GenVertex*        operator*(void) const
		{ return m_map_iterator->second; }
	    /// Pre-fix increment
	    vertex_iterator&  operator++(void)  //Pre-fix increment 
		{ ++m_map_iterator;     return *this; }
	    /// Post-fix increment
	    vertex_iterator   operator++(int)   //Post-fix increment
		{ vertex_iterator out(*this); ++(*this); return out; }
	    /// equality
	    bool              operator==( const vertex_iterator& a ) const
		{ return m_map_iterator == a.m_map_iterator; }
	    /// inequality
	    bool              operator!=( const vertex_iterator& a ) const
		{ return !(m_map_iterator == a.m_map_iterator); }
	protected:
	    /// iterator to the vertex map
	    std::map<int,GenVertex*,std::greater<int> >::iterator 
	                                                       m_map_iterator;
	};
	friend class vertex_iterator;
	/// begin vertex iteration
	vertex_iterator            vertices_begin() 
	    { return GenEvent::vertex_iterator( 
		m_vertex_barcodes.begin() ); }
	/// end vertex iteration
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

	//!  const particle iterator

	/// \class  particle_const_iterator
	/// HepMC::GenEvent::particle_const_iterator 
	/// is used to iterate over all particles in the event.
	class particle_const_iterator :
	  public std::iterator<std::forward_iterator_tag,GenParticle*,ptrdiff_t>{
	    // Iterates over all vertices in this event
	public:
	    /// iterate over particles
	    particle_const_iterator(
		const std::map<int,GenParticle*>::const_iterator& i )
		: m_map_iterator(i) {}
	    particle_const_iterator() {}
	    /// copy constructor
	    particle_const_iterator( const particle_const_iterator& i )
		{ *this = i; }
	    virtual ~particle_const_iterator() {}
	    /// make a copy
	    particle_const_iterator& operator=(
		const particle_const_iterator& i )
		{ m_map_iterator = i.m_map_iterator; return *this; }
	    /// return a pointer to GenParticle
	    GenParticle*        operator*(void) const
		{ return m_map_iterator->second; }
	    /// Pre-fix increment
	    particle_const_iterator&  operator++(void)  //Pre-fix increment 
		{ ++m_map_iterator; return *this; }
	    /// Post-fix increment
	    particle_const_iterator   operator++(int)   //Post-fix increment
		{ particle_const_iterator out(*this); ++(*this); return out; }
	    /// equality
	    bool  operator==( const particle_const_iterator& a ) const
		{ return m_map_iterator == a.m_map_iterator; }
	    /// inequality
	    bool  operator!=( const particle_const_iterator& a ) const
		{ return !(m_map_iterator == a.m_map_iterator); }
	protected:
	    /// const iterator to the GenParticle map
	    std::map<int,GenParticle*>::const_iterator m_map_iterator;
	};	
	friend class particle_const_iterator;
	/// begin particle iteration
	particle_const_iterator      particles_begin() const
	    { return GenEvent::particle_const_iterator( 
		m_particle_barcodes.begin() ); }
	/// end particle iteration
	particle_const_iterator      particles_end() const
	    { return GenEvent::particle_const_iterator(
		m_particle_barcodes.end() ); }

	//!  non-const particle iterator

	/// \class  particle_iterator
	/// HepMC::GenEvent::particle_iterator 
	/// is used to iterate over all particles in the event.
 	class particle_iterator :
	  public std::iterator<std::forward_iterator_tag,GenParticle*,ptrdiff_t>{
	    // Iterates over all vertices in this event
	public:
	    /// iterate over particles
	    particle_iterator( const std::map<int,GenParticle*>::iterator& i )
		: m_map_iterator( i ) {}
	    particle_iterator() {}
	    /// copy constructor
	    particle_iterator( const particle_iterator& i ) { *this = i; }
	    virtual ~particle_iterator() {}
	    /// make a copy
	    particle_iterator&  operator=( const particle_iterator& i ) {
		m_map_iterator = i.m_map_iterator;
		return *this;
	    }
	    /// const particle iterator
	    operator particle_const_iterator() const
		{ return particle_const_iterator(m_map_iterator); }
	    /// return pointer to GenParticle
	    GenParticle*        operator*(void) const
		{ return m_map_iterator->second; }
            /// Pre-fix increment
	    particle_iterator&  operator++(void) 
		{ ++m_map_iterator;     return *this; }
            /// Post-fix increment
	    particle_iterator   operator++(int)   
		{ particle_iterator out(*this); ++(*this); return out; }
            /// equality
	    bool              operator==( const particle_iterator& a ) const
		{ return m_map_iterator == a.m_map_iterator; }
            /// inequality
	    bool              operator!=( const particle_iterator& a ) const
		{ return !(m_map_iterator == a.m_map_iterator); }
	protected:
	    /// iterator for GenParticle map
	    std::map<int,GenParticle*>::iterator m_map_iterator;
	};
	friend class particle_iterator;
	/// begin particle iteration
	particle_iterator particles_begin() 
	    { return GenEvent::particle_iterator(
		m_particle_barcodes.begin() ); }
	/// end particle iteration
        particle_iterator particles_end()
	    { return GenEvent::particle_iterator(
		m_particle_barcodes.end() ); }

	////////////////////////////////////////////////
    protected:
	//
	// Following methods intended for use by GenParticle/Vertex classes:
	// In general there is no reason they should be used elsewhere.
	/// set the barcode - intended for use by GenParticle
	bool         set_barcode( GenParticle* p, int suggested_barcode =0 );
	/// set the barcode - intended for use by GenVertex
	bool         set_barcode( GenVertex*   v, int suggested_barcode =0 );
	///  intended for use by GenParticle
	void         remove_barcode( GenParticle* p );
	///  intended for use by GenVertex
	void         remove_barcode( GenVertex*   v );

	static unsigned int counter(); //!<num GenEvent objects in memory
   	void delete_all_vertices(); //!<delete all vertices owned by this event

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
	HeavyIon*        m_heavy_ion; 	      // undefined by default
	PdfInfo*         m_pdf_info; 	      // undefined by default

	static unsigned int   s_counter;
    };

    ///////////////////////////
    // INLINE Access Methods //
    ///////////////////////////

    ///  The integer ID that uniquely specifies this signal
    ///  process, i.e. MSUB in Pythia. It is necessary to
    ///  package this with each event rather than with the run
    ///  because many processes may be generated within one run.
    inline int GenEvent::signal_process_id() const 
    { return m_signal_process_id; }

    inline int GenEvent::event_number() const { return m_event_number; }

    inline double GenEvent::event_scale() const { return m_event_scale; }

    inline double GenEvent::alphaQCD() const { return m_alphaQCD; }

    inline double GenEvent::alphaQED() const { return m_alphaQED; }
 
    inline GenVertex* GenEvent::signal_process_vertex() const {
	/// returns a (mutable) pointer to the signal process vertex
	return m_signal_process_vertex;
    }  

    inline WeightContainer& GenEvent::weights() { return m_weights; }

    inline const WeightContainer& GenEvent::weights() const 
    { return m_weights; }

    inline HeavyIon* const GenEvent::heavy_ion() const 
    { return m_heavy_ion; }

    inline HeavyIon*  GenEvent::heavy_ion()  
    { return m_heavy_ion; }

    inline PdfInfo* const GenEvent::pdf_info() const 
    { return m_pdf_info; }

    inline PdfInfo*  GenEvent::pdf_info()  
    { return m_pdf_info; }

    ///  Vector of integers which specify the random number 
    ///  generator's state for this event. It is left to the
    ///  generator to make use of this. We envision a vector of
    ///  RndmStatesTags to be included with a run class which
    ///  would specify the meaning of the random_states.
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

    inline void GenEvent::set_heavy_ion( const HeavyIon& ion )
    { m_heavy_ion = new HeavyIon(ion); }

    inline void GenEvent::set_pdf_info( const PdfInfo& p )
    { m_pdf_info = new PdfInfo(p); }

    inline void GenEvent::set_random_states( const std::vector<long int>&
					     randomstates )
    { m_random_states = randomstates; }

    inline void GenEvent::remove_barcode( GenParticle* p )
    { m_particle_barcodes.erase( p->barcode() ); }

    inline void GenEvent::remove_barcode( GenVertex* v )
    { m_vertex_barcodes.erase( v->barcode() ); }

    /// Each vertex or particle has a barcode, which is just an integer which
    /// uniquely identifies it inside the event (i.e. there is a one to one
    /// mapping between particle memory addresses and particle barcodes... and 
    /// the same applied for vertices).
    ///
    /// The value of a barcode has NO MEANING and NO ORDER!
    /// For the user's convenience, when an event is read in via an IO_method
    /// from an indexed list (like the HEPEVT common block), then the index will
    /// become the barcode for that particle.
    ///
    /// Particle barcodes are always positive integers.
    /// The barcodes are chosen and set automatically when a vertex or particle
    /// comes under the ownership of an event (i.e. it is contained in an event).
    inline GenParticle* GenEvent::barcode_to_particle( int barCode ) const
    { 
	std::map<int,GenParticle*>::const_iterator i 
	    = m_particle_barcodes.find(barCode);
	return ( i != m_particle_barcodes.end() ) ? (*i).second : 0;
    }

    /// Each vertex or particle has a barcode, which is just an integer which
    /// uniquely identifies it inside the event (i.e. there is a one to one
    /// mapping between particle memory addresses and particle barcodes... and 
    /// the same applied for vertices).
    ///
    /// The value of a barcode has NO MEANING and NO ORDER!
    /// For the user's convenience, when an event is read in via an IO_method
    /// from an indexed list (like the HEPEVT common block), then the index will
    /// become the barcode for that particle.
    ///
    /// Vertex barcodes are always negative integers.
    /// The barcodes are chosen and set automatically when a vertex or particle
    /// comes under the ownership of an event (i.e. it is contained in an event).
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


