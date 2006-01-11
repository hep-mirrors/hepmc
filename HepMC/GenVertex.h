//--------------------------------------------------------------------------
#ifndef HEPMC_GEN_VERTEX_H
#define HEPMC_GEN_VERTEX_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// GenVertex within an event
// A vertex is indirectly (via particle "edges") linked to other 
//   vertices ("nodes") to form a composite "graph"
//////////////////////////////////////////////////////////////////////////

// --> HANDLE COMPILER INCONSISTENCIES
// This pre-compiler directive is included (2002-01-16) to allow compatibility
// with several compilers.
// Mar 27, 2004: HepMC is now standard compliant only. 
//   I've removed forward_iterator, and it will no longer compile on gcc < 3. 
#ifdef __SUNPRO_CC    // Solaris CC 5.2
#define NEED_SOLARIS_FRIEND_FEATURE
#endif // Platform

#include "HepMC/WeightContainer.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Geometry/Point3D.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>

namespace HepMC {

    enum IteratorRange { parents, children, family, 
			 ancestors, descendants, relatives };
    class GenParticle;
    class GenEvent;

    class GenVertex {

	friend std::ostream& operator<<( std::ostream&, const GenVertex& );
	friend class GenEvent;

#ifdef NEED_SOLARIS_FRIEND_FEATURE
	// This bit of ugly code is only for CC-5.2 compiler. 
	// M.Dobbs 2002/02/19 
	// It is not needed by linux gcc, nor Windows Visual C++.
    public: 
	class vertex_iterator;
	friend class vertex_iterator;
	class particle_iterator;
	friend class particle_iterator;
#endif // NEED_SOLARIS_FRIEND_FEATURE

    public:
	GenVertex( const HepLorentzVector& position =HepLorentzVector(0,0,0,0),
		   int id = 0, 
		   const WeightContainer& weights = std::vector<double>() );
	GenVertex( const GenVertex& invertex );            // shallow copy
	virtual    ~GenVertex();

	GenVertex& operator= ( const GenVertex& invertex ); // shallow
	bool       operator==( const GenVertex& a ) const;
	bool       operator!=( const GenVertex& a ) const;
	void       print( std::ostream& ostr = std::cout ) const;

	double     check_momentum_conservation() const;//|Sum (mom_in-mom_out)|

	void       add_particle_in( GenParticle* inparticle );
	void       add_particle_out( GenParticle* outparticle );
	// remove_particle finds *particle in the in and/or out list and
	//  removes it from these lists ... it DOES NOT DELETE THE PARTICLE 
	//  or its relations. You could delete the particle too as follows:
	//      delete vtx->remove_particle( particle );
	GenParticle* remove_particle( GenParticle* particle );

	operator HepLorentzVector() const; // conversion operator
	operator HepPoint3D() const; // conversion operator

	////////////////////
	// access methods //
	////////////////////

	GenEvent*               parent_event() const;
	HepPoint3D              point3d() const;
	HepLorentzVector        position() const;
	void                    set_position( const HepLorentzVector& position 
					      = HepLorentzVector(0,0,0,0) );
	// we don't define what you use the id for -- but we imagine,
	// for example it might code the meaning of the weights()
	int                     id() const;
	void                    set_id( int id );

	//
	// The barcode is the vertex's reference number, every vertex in the
	//  event has a unique barcode. Vertex barcodes are negative numbers,
	//  particle barcodes are positive numbers.
	// In general there is no reason to "suggest_barcode", if a vertex is
	//  added to the event without a suggested barcode, the event will
	//  assign one for it.
	int                     barcode() const;
	bool                    suggest_barcode( int the_bar_code );
	
	// direct access to the weights container is allowed. 
	WeightContainer&        weights();
	const WeightContainer&  weights() const;

	////////////////////
	// Iterators      // users should use prefer to use particle_iterator
	////////////////////    

	typedef std::set<GenParticle*>::const_iterator 
	particles_in_const_iterator;
	typedef std::set<GenParticle*>::const_iterator 
	particles_out_const_iterator;
	particles_in_const_iterator         particles_in_const_begin() const;
	particles_in_const_iterator         particles_in_const_end() const;
	particles_out_const_iterator        particles_out_const_begin() const;
	particles_out_const_iterator        particles_out_const_end() const;
	int                                 particles_in_size() const;
	int                                 particles_out_size() const;

    protected:
	static unsigned int     counter(); // temporary for debugging

	// only the GenEvent (friend) is allowed to set the parent_event,
	//  and barcode. It is done automatically anytime you add a 
	//  vertex to an event
	void                    set_parent_event_( GenEvent* evt );
	void                    set_barcode_( int the_bar_code );

	/////////////////////////////
	// edge_iterator           // (protected - for internal use only)
	/////////////////////////////
	// If the user wants the functionality of the edge_iterator, he should
	// use particle_iterator with IteratorRange = family, parents, children
	//
	class edge_iterator :
	  public std::iterator<std::forward_iterator_tag,GenParticle*,ptrdiff_t>{
	    // iterate over the family of edges connected to m_vertex begins 
	    // with parents (incoming particles) then children (outgoing)
	    // This is not a recursive iterator ... it is a building block
	    // for the public iterators and is intended for internal use only.
	    // The acceptable Iterator Ranges are: family, parents, children
	public:
	    edge_iterator();
	    edge_iterator( const GenVertex& vtx, IteratorRange range =family );
	    edge_iterator( const edge_iterator& p );
	    virtual        ~edge_iterator();
	    edge_iterator& operator=( const edge_iterator& p );
	    GenParticle*      operator*(void) const;
	    edge_iterator& operator++(void); // Pre-fix increment 
	    edge_iterator  operator++(int);   // Post-fix increment
	    bool           operator==( const edge_iterator& a ) const;
	    bool           operator!=( const edge_iterator& a ) const;
	    bool           is_parent() const; // true if parent of root vtx
	    bool           is_child() const;  // true if child of root vtx
	    const GenVertex*  vertex_root() const;
	private:
	    const GenVertex*  m_vertex;
	    IteratorRange  m_range;
	    std::set<GenParticle*>::const_iterator m_set_iter;
	    bool           m_is_inparticle_iter;
	    bool           m_is_past_end;
	};
	friend class edge_iterator;
	int              edges_size( IteratorRange range = family ) const;
	edge_iterator    edges_begin( IteratorRange range = family) const;
	edge_iterator    edges_end( IteratorRange /* dummy_range */ ) const;

    public:
	///////////////////////////////
	// vertex_iterator           //
	///////////////////////////////
	class vertex_iterator :
	  public std::iterator<std::forward_iterator_tag,GenVertex*,ptrdiff_t>{
	    // Iterates over all vertices connected via a graph to this vertex.
	    // this is made friend to that it can access protected edge
	    // iterator the range can be IteratorRange= ( parents, children, 
	    // family, ancestors, descendants, relatives )
	    // example for range=descendants the iterator 
	    // will return all vertices
	    // which are children (connected by an outgoing particle edge),
	    // grandchildren, great-grandchildren, etc. of this vertex
	    // In all cases the iterator always returns this vertex
	    // (returned last).
	    // The algorithm is accomplished by converting the graph to a tree
	    // (by "chopping" the edges connecting to an already visited
	    // vertex) and returning the vertices in POST ORDER traversal.
	    //
	public:
	    vertex_iterator();
	    vertex_iterator( GenVertex& vtx_root, IteratorRange range );
	    // next constructor is intended for internal use only
	    vertex_iterator( GenVertex& vtx_root, IteratorRange range,
			     std::set<const GenVertex*>& visited_vertices );
	    vertex_iterator( const vertex_iterator& v_iter );
	    virtual             ~vertex_iterator();
	    vertex_iterator&    operator=( const vertex_iterator& );
	    GenVertex*          operator*(void) const;
	    vertex_iterator&    operator++(void);  //Pre-fix increment 
	    vertex_iterator     operator++(int);   //Post-fix increment
	    bool                operator==( const vertex_iterator& ) const;
	    bool                operator!=( const vertex_iterator& ) const;
	    GenVertex*          vertex_root() const;
	    IteratorRange       range() const;
	    void                copy_with_own_set( const vertex_iterator& 
						   v_iter,
						   std::set<const GenVertex*>& 
						   visited_vertices );
	protected:                  // intended for internal use only
	    GenVertex* follow_edge_(); // non-null if recursive iter. created
	    void    copy_recursive_iterator_( const vertex_iterator* 
					      recursive_v_iter );
	private:
	    GenVertex*       m_vertex;   // the vertex associated to this iter
	    IteratorRange    m_range;
	    std::set<const GenVertex*>* m_visited_vertices;
	    bool             m_it_owns_set;  // true if it is responsible for 
	                                     // deleting the visited vertex set
	    edge_iterator    m_edge; // particle edge pointing to return vtx
	    vertex_iterator* m_recursive_iterator;
	};	
	friend class vertex_iterator;
	vertex_iterator     vertices_begin( IteratorRange range = relatives );
	vertex_iterator     vertices_end( IteratorRange /* dummy_range */ );
 
    public:
	///////////////////////////////
	// particle_iterator         //
	///////////////////////////////
	class particle_iterator :
	  public std::iterator<std::forward_iterator_tag,GenParticle*,ptrdiff_t>{
	    // Iterates over all particles connected via a graph.
	    // by iterating through all vertices in the m_range. For each
	    // vertex it returns orphaned parent particles 
	    // (i.e. parents without production vertices) 
	    // then children ... in this way each particle is associated
	    // to exactly one vertex and so it is returned exactly once.
	    // Is made friend so that it can access protected edge iterator
	public:
	    particle_iterator();
	    particle_iterator( GenVertex& vertex_root, IteratorRange range );
	    particle_iterator( const particle_iterator& );
	    virtual             ~particle_iterator();
	    particle_iterator&  operator=( const particle_iterator& );
	    GenParticle*        operator*(void) const;
	    particle_iterator&  operator++(void);  //Pre-fix increment 
	    particle_iterator   operator++(int);   //Post-fix increment
	    bool                operator==( const particle_iterator& ) const;
	    bool                operator!=( const particle_iterator& ) const;
	protected:
	    GenParticle*        advance_to_first_();
	private:
	    vertex_iterator     m_vertex_iterator;
	    edge_iterator       m_edge;     // points to the return
	};
	friend class particle_iterator;
	particle_iterator       particles_begin( IteratorRange range 
						 = relatives );
	particle_iterator       particles_end( IteratorRange 
					       /* dummy_range */ );

	////////////////////////////////////////////////
    protected: // for internal use only
	void delete_adopted_particles();
	
    private: // GenVertex data members
	HepLorentzVector     m_position;      //4-vec of vertex [mm]
	std::set<GenParticle*>  m_particles_in;  //all incoming particles
	std::set<GenParticle*>  m_particles_out; //all outgoing particles
	int                  m_id;
	WeightContainer      m_weights;       // weights for this vtx
	GenEvent*            m_event;
	int                  m_barcode;   // unique identifier in the event

	static unsigned int  s_counter;
    };  

    ////////////////////////////
    // INLINES access methods //
    ////////////////////////////

    inline GenVertex::operator HepLorentzVector() const { return position(); }

    inline GenVertex::operator HepPoint3D() const { return point3d(); }

    inline HepLorentzVector GenVertex::position() const { return m_position; }

    inline GenEvent* GenVertex::parent_event() const { return m_event; }

    inline HepPoint3D GenVertex::point3d() const { 
	return (HepPoint3D)m_position; 
    }

    inline int GenVertex::id() const { return m_id; }

    inline int  GenVertex::barcode() const { return m_barcode; }
    inline void GenVertex::set_barcode_( int bc ) { m_barcode = bc; }

    inline WeightContainer& GenVertex::weights() { return m_weights; }

    inline const WeightContainer& GenVertex::weights() const 
    { return m_weights; }

    inline void GenVertex::set_position( const HepLorentzVector& position ) {
	m_position = position;
    }

    inline void GenVertex::set_id( int id ) { m_id = id; }

    //////////////
    // INLINES  //
    //////////////

    inline GenVertex::particles_in_const_iterator 
    GenVertex::particles_in_const_begin() const { 
	return m_particles_in.begin(); 
    }

    inline GenVertex::particles_in_const_iterator 
    GenVertex::particles_in_const_end() const { 
	return m_particles_in.end(); 
    }

    inline GenVertex::particles_out_const_iterator 
    GenVertex::particles_out_const_begin() const { 
	return m_particles_out.begin();
    }

    inline GenVertex::particles_out_const_iterator 
    GenVertex::particles_out_const_end() const {	
	return m_particles_out.end(); 
    }

    inline int GenVertex::particles_in_size() const {
	return m_particles_in.size(); 
    }

    inline int GenVertex::particles_out_size() const {
	return m_particles_out.size(); 
    }	

    inline bool GenVertex::edge_iterator::operator==( 
	const edge_iterator& a ) const { 
	return **this == *a; 
    }

    inline bool GenVertex::edge_iterator::operator!=(
	const edge_iterator& a ) const { 
	return !(**this == *a); 
    }

    inline const GenVertex* GenVertex::edge_iterator::vertex_root() const {
	return m_vertex;
    }

    inline GenVertex::edge_iterator GenVertex::edges_begin( IteratorRange 
						      range ) const {
	return GenVertex::edge_iterator(*this, range);
    }

    inline GenVertex::edge_iterator GenVertex::edges_end( IteratorRange 
						    /* dummy_range */ ) const {
	return GenVertex::edge_iterator();
    }

    inline bool GenVertex::vertex_iterator::operator==( 
	const vertex_iterator& a ) const {
	return **this == *a; 
    }

    inline bool GenVertex::vertex_iterator::operator!=( 
	const vertex_iterator& a ) const {
	return !(**this == *a); 
    }

    inline GenVertex* GenVertex::vertex_iterator::vertex_root() const {
	return m_vertex; 
    }

    inline IteratorRange GenVertex::vertex_iterator::range() const {
	return m_range; 
    }

    inline GenVertex::vertex_iterator GenVertex::vertices_begin( 
	IteratorRange range ){
	// this is not const because the it could return itself
	return vertex_iterator( *this, range );
    }

    inline GenVertex::vertex_iterator GenVertex::vertices_end( 
	IteratorRange /* dummy_range */ ) {
	return vertex_iterator();
    }

    inline bool GenVertex::particle_iterator::operator==( 
	const particle_iterator& a ) const {
	return **this == *a; 
    }

    inline bool GenVertex::particle_iterator::operator!=( 
	const particle_iterator& a ) const {
	return !(**this == *a); 
    }

    inline GenVertex::particle_iterator GenVertex::particles_begin( 
	IteratorRange range ) {
	return particle_iterator( *this, range );
    }

    inline GenVertex::particle_iterator GenVertex::particles_end(
	IteratorRange /* dummy_range */ ){
	return particle_iterator();
    }

} // HepMC

#endif  // HEPMC_GEN_VERTEX_H
//--------------------------------------------------------------------------




