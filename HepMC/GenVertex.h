#ifndef HEPMC_GEN_VERTEX_H
#define HEPMC_GEN_VERTEX_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics",  Comput.Phys.Commun. 134 (2001) 41-46.
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
#include "HepMC/SimpleVector.h"
#include "HepMC/IteratorRange.h"
#include <iostream>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <cstddef>

namespace HepMC {

  class GenVertexParticleRange;
  class GenParticleProductionRange;
  class ConstGenParticleProductionRange;
  class GenParticleEndRange;
  class ConstGenParticleEndRange;

  class GenParticle;
  class GenEvent;


  //! GenVertex contains information about decay vertices.
  ///
  /// HepMC::GenVertex contains the position in space and time of a decay.
  /// It also contains lists of incoming and outgoing particles.
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
    /// default constructor
    GenVertex( const FourVector& position=FourVector(0,0,0,0), int status=0,
               const WeightContainer& weights = std::vector<double>() );
    GenVertex( const GenVertex& invertex );            //!< shallow copy
    virtual    ~GenVertex();

    void swap( GenVertex & other); //!< swap
    GenVertex& operator= ( const GenVertex& invertex ); //!< shallow
    bool operator==( const GenVertex& a ) const; //!< equality
    bool operator!=( const GenVertex& a ) const; //!< inequality
    void print( std::ostream& ostr = std::cout ) const; //!< print vertex information

    /// @todo Remove
    double check_momentum_conservation() const;//!< |Sum (three_mom_in-three_mom_out)|

    /// add incoming particle
    void add_particle_in( GenParticle* inparticle );
    /// add outgoing particle
    void add_particle_out( GenParticle* outparticle );
    /// remove_particle finds *particle in the in and/or out list and
    ///  removes it from these lists ... it DOES NOT DELETE THE PARTICLE
    ///  or its relations. You can simultaneously delete the particle, too:
    ///      delete vtx->remove_particle( particle );
    GenParticle* remove_particle( GenParticle* particle ); //!< remove a particle

    /// Conversion operator to a 4D point
    operator HepMC::FourVector() const { return position(); }
    /// Conversion operator to a 3D point
    operator HepMC::ThreeVector() const { return point3d(); }

    ////////////////////
    // access methods //
    ////////////////////

    /// Pointer to the event that owns this vertex
    GenEvent* parent_event() const { return m_event; }

    /// Vertex position
    /// @note Return by value, constructed from 4-vector components; position() is more efficient.
    /// @deprecated Will be removed in HepMC3
    ThreeVector point3d() const { return ThreeVector(m_position.x(), m_position.y(), m_position.z()); }
    /// Vertex position and time
    const FourVector& position() const { return m_position; }
    /// Set vertex position and time
    void set_position(const FourVector& pos=FourVector(0,0,0,0)) { m_position = pos; }

    /// Vertex statuses are used to encode the sort of transition represented: see docs. Used to be 'id'
    int status() const { return m_status; }
    /// Backward compatibility alias for status()
    /// @deprecated 'id' is a reserved name in HepMC3 -- use 'status' instead
    int id() const { return status(); }
    /// Set the vertex status
    void set_status( int i_status ) { m_status = i_status; }
    /// Set the vertex status (alias)
    /// @deprecated 'id' is a reserved name in HepMC3 -- use 'status' instead
    void set_id( int i_id ) { set_status(i_id); }

    /// @brief Get the vertex barcode
    ///
    /// The barcode is the vertex's reference number, every vertex in the
    /// event has a unique barcode. Vertex barcodes are negative numbers,
    /// particle barcodes are positive numbers.
    ///
    /// Please note that the barcodes are intended for internal use within
    /// HepMC as a unique identifier for the particles and vertices.
    /// Using the barcode to encode extra information is an abuse of
    /// the barcode data member and causes confusion among users.
    int barcode() const { return m_barcode; }

    /// Try to manually set the barcode: discouraged but widespread
    bool suggest_barcode( int the_bar_code );

    /// Direct access to the weights container is allowed.
    WeightContainer& weights() { return m_weights; }
    /// Const direct access to the weights container
    const WeightContainer& weights() const { return m_weights; }


    /// Immediate incoming particles
    std::vector<GenParticle*>& parents() { return m_particles_in; }
    /// Immediate incoming particles (const)
    const std::vector<GenParticle*>& parents() const { return m_particles_in; }

    /// Immediate outgoing particles
    std::vector<GenParticle*>& children() { return m_particles_out; }
    /// Immediate outgoing particles (const)
    const std::vector<GenParticle*>& children() const { return m_particles_out; }


    /// Particle ranges of various kinds
    GenVertexParticleRange particles( IteratorRange range = relatives );

    /// Incoming particle range
    /// @note The GenParticle arg is mysterious...
    /// @deprecated Prefer to use parents()
    GenParticleProductionRange particles_in(GenParticle&, IteratorRange range = relatives );
    /// Incoming particle range
    /// @note The GenParticle arg is mysterious...
    /// @deprecated Prefer to use parents()
    ConstGenParticleProductionRange particles_in(const GenParticle&, IteratorRange range = relatives ) const;

    /// Outgoing particle range
    GenParticleEndRange particles_out( GenParticle&, IteratorRange range = relatives );
    /// Outgoing particle range
    /// @note The GenParticle arg is mysterious...
    /// @deprecated Prefer to use parents()
    ConstGenParticleEndRange particles_out( const GenParticle&, IteratorRange range = relatives ) const;


    /// @name Iterators
    //@{

    /// Number of incoming particles
    int particles_in_size() const { return m_particles_in.size(); }
    /// Number of outgoing particles
    int particles_out_size() const { return m_particles_out.size(); }

    /// Iterator for incoming particles
    typedef std::vector<HepMC::GenParticle*>::iterator particles_in_iterator;
    /// Const iterator for incoming particles
    typedef std::vector<HepMC::GenParticle*>::const_iterator particles_in_const_iterator;

    /// Begin iteration of incoming particles
    particles_in_iterator particles_in_begin() { return m_particles_in.begin(); }
    /// Begin const iteration of incoming particles
    particles_in_const_iterator particles_in_const_begin() const { return m_particles_in.begin(); }
    /// End iteration of incoming particles
    particles_in_iterator particles_in_end() { return m_particles_in.end(); }
    /// End const iteration of incoming particles
    particles_in_const_iterator particles_in_const_end() const { return m_particles_in.end(); }

    /// Iterator for outgoing particles
    typedef std::vector<HepMC::GenParticle*>::iterator particles_out_iterator;
    /// Const iterator for outgoing particles
    typedef std::vector<HepMC::GenParticle*>::const_iterator particles_out_const_iterator;

    /// Begin iteration of outgoing particles
    particles_out_iterator particles_out_begin() { return m_particles_out.begin(); }
    /// Begin const iteration of outgoing particles
    particles_out_const_iterator particles_out_const_begin() const { return m_particles_out.begin(); }
    /// End iteration of outgoing particles
    particles_out_iterator particles_out_end() { return m_particles_out.end(); }
    /// End const iteration of outgoing particles
    particles_out_const_iterator particles_out_const_end() const { return m_particles_out.end(); }

    //@}


  protected:

    /// only the GenEvent (friend) is allowed to set the parent_event,
    ///  and barcode. It is done automatically anytime you add a
    ///  vertex to an event
    void set_parent_event_( GenEvent* evt ); //!< set parent event
    void set_barcode_( int bc ) { m_barcode = bc; } //!< set identifier
    void change_parent_event_( GenEvent* evt ); //!< for use with swap

    /////////////////////////////
    // edge_iterator           // (protected - for internal use only)
    /////////////////////////////
    // If the user wants the functionality of the edge_iterator, he should
    // use particle_iterator with IteratorRange = family, parents, children
    //

    //!  edge iterator

    /// \class  edge_iterator
    /// iterate over the family of edges connected to m_vertex begins
    /// with parents (incoming particles) then children (outgoing)
    /// This is not a recursive iterator ... it is a building block
    /// for the public iterators and is intended for internal use only.
    /// The acceptable Iterator Ranges are: family, parents, children
    class edge_iterator : public std::iterator<std::forward_iterator_tag,HepMC::GenParticle*,ptrdiff_t>{
    public:
      edge_iterator();
      /// used to set limits on the iteration
      edge_iterator( const GenVertex& vtx, IteratorRange range =family );
      /// copy
      edge_iterator( const edge_iterator& p );
      virtual ~edge_iterator();
      /// make a copy
      edge_iterator& operator=( const edge_iterator& p );
      /// return a pointer to a particle
      GenParticle* operator*(void) const;
      /// Pre-fix increment
      edge_iterator& operator++(void); // Pre-fix increment
      /// Post-fix increment
      edge_iterator  operator++(int);   // Post-fix increment
      /// equality
      bool operator==( const edge_iterator& a ) const { return **this == *a; }
      /// inequality
      bool operator!=( const edge_iterator& a ) const { return !(**this == *a); }
      /// true if parent of root vtx
      bool is_parent() const;
      /// true if child of root vtx
      bool is_child() const;
      /// root vertex of this iteration
      const GenVertex* vertex_root() const { return m_vertex; }
    private:
      /// Pre-fix increment -- is not allowed
      edge_iterator& operator--(void);
      /// Post-fix increment -- is not allowed
      edge_iterator  operator--(int);
    private:
      const GenVertex*  m_vertex;
      IteratorRange  m_range;
      std::vector<HepMC::GenParticle*>::const_iterator m_set_iter;
      bool m_is_inparticle_iter;
      bool m_is_past_end;
    };
    friend class edge_iterator;
    /// size
    int edges_size( IteratorRange range = family ) const;
    /// begin range
    edge_iterator edges_begin( IteratorRange range = family) const { return GenVertex::edge_iterator(*this, range); }
    /// end range
    edge_iterator edges_end( IteratorRange range ) const { return GenVertex::edge_iterator(); }


  public:

    ///////////////////////////////
    // vertex_iterator           //
    ///////////////////////////////

    //!  vertex iterator
    ///
    /// Iterates over all vertices connected via a graph to this vertex.
    /// this is made friend to that it can access protected edge
    /// iterator the range can be IteratorRange= ( PARENTS, CHILDREN,
    /// FAMILY, ANCESTORS, DESCENDANTS, RELATIVES )
    /// example for range=DESCENDANTS the iterator
    /// will return all vertices
    /// which are children (connected by an outgoing particle edge),
    /// grandchildren, great-grandchildren, etc. of this vertex
    /// In all cases the iterator always returns this vertex
    /// (returned last).
    /// The algorithm is accomplished by converting the graph to a tree
    /// (by "chopping" the edges connecting to an already visited
    /// vertex) and returning the vertices in POST ORDER traversal.
    ///
    class vertex_iterator : public std::iterator<std::forward_iterator_tag,HepMC::GenVertex*,ptrdiff_t>{
    public:
      vertex_iterator();
      /// used to set limits on the iteration
      vertex_iterator( GenVertex& vtx_root, IteratorRange range );
      /// next constructor is intended for internal use only
      vertex_iterator( GenVertex& vtx_root, IteratorRange range, std::set<const HepMC::GenVertex*>& visited_vertices );
      /// copy
      vertex_iterator( const vertex_iterator& v_iter );
      virtual ~vertex_iterator();
      /// make a copy
      vertex_iterator& operator=( const vertex_iterator& );
      /// return a pointer to a vertex
      GenVertex* operator*(void) const;
      /// Pre-fix increment
      vertex_iterator& operator++(void);  //Pre-fix increment
      /// Post-fix increment
      vertex_iterator operator++(int);   //Post-fix increment
      /// equality
      bool operator==( const vertex_iterator& a ) const { return **this == *a; }
      /// inequality
      bool operator!=( const vertex_iterator& a ) const { return !(**this == *a); }
      /// vertex that this iterator begins from
      GenVertex* vertex_root() const { return m_vertex; }
      /// iterator range
      IteratorRange range() const { return m_range; }
      /// intended for internal use only.
      void copy_with_own_set( const vertex_iterator& v_iter, std::set<const HepMC::GenVertex*>& visited_vertices );

    protected:
      // intended for internal use only
      /// non-null if recursive iter. created
      GenVertex* follow_edge_();
      /// copy recursive iterator
      void copy_recursive_iterator_( const vertex_iterator*
                                        recursive_v_iter );
    private:
      /// Pre-fix increment -- is not allowed
      vertex_iterator& operator--(void);
      /// Post-fix increment -- is not allowed
      vertex_iterator operator--(int);

    private:
      GenVertex* m_vertex;   // the vertex associated to this iter
      IteratorRange m_range;
      std::set<const HepMC::GenVertex*>* m_visited_vertices;
      bool m_it_owns_set;  // true if it is responsible for
      // deleting the visited vertex set
      edge_iterator m_edge; // particle edge pointing to return vtx
      vertex_iterator* m_recursive_iterator;
    };
    friend class vertex_iterator;
    /// begin vertex range
    // this is not const because the it could return itself
    vertex_iterator vertices_begin( IteratorRange range = relatives ) { return vertex_iterator( *this, range ); }
    /// end vertex range
    vertex_iterator vertices_end( IteratorRange range ) { return vertex_iterator(); }


    ///////////////////////////////
    // particle_iterator         //
    ///////////////////////////////

    //!  particle iterator
    ///
    /// Iterates over all particles connected via a graph.
    /// by iterating through all vertices in the m_range. For each
    /// vertex it returns orphaned parent particles
    /// (i.e. parents without production vertices)
    /// then children ... in this way each particle is associated
    /// to exactly one vertex and so it is returned exactly once.
    /// Is made friend so that it can access protected edge iterator
    class particle_iterator : public std::iterator<std::forward_iterator_tag,GenParticle*,ptrdiff_t>{
    public:
      particle_iterator();
      /// used to set limits on the iteration
      particle_iterator( GenVertex& vertex_root, IteratorRange range );
      /// copy
      particle_iterator( const particle_iterator& );
      virtual ~particle_iterator();
      /// make a copy
      particle_iterator&  operator=( const particle_iterator& );
      /// return a pointer to a particle
      GenParticle* operator*(void) const;
      /// Pre-fix increment
      particle_iterator&  operator++(void);
      /// Post-fix increment
      particle_iterator operator++(int);
      /// equality
      bool operator==( const particle_iterator& a ) const { return **this == *a; }
      /// inequality
      bool operator!=( const particle_iterator& a ) const { return !(**this == *a); }
    protected:
      GenParticle* advance_to_first_(); //!< "first" particle
    private:
      vertex_iterator m_vertex_iterator;
      edge_iterator m_edge;     // points to the return
    };
    friend class particle_iterator;
    /// begin particle range
    particle_iterator particles_begin( IteratorRange range ) { return particle_iterator( *this, range ); }
    particle_iterator particles_begin( IteratorRange range ) const { return particle_iterator( const_cast<GenVertex&>(*this), range ); } ///< Ewww
    /// end particle range
    particle_iterator particles_end( IteratorRange ) { return particle_iterator(); }
    particle_iterator particles_end( IteratorRange ) const { return particle_iterator(); }


  protected:

    /// for internal use only
    void delete_adopted_particles();
    /// for internal use only - remove particle from incoming list
    void remove_particle_in( GenParticle* );
    /// for internal use only - remove particle from outgoing list
    void remove_particle_out( GenParticle* );
    /// scale the position vector
    /// this method is only for use by GenEvent
    void convert_position( const double& );


  private:

    FourVector              m_position;      //< 4-vec of vertex
    std::vector<HepMC::GenParticle*>  m_particles_in;  //< All incoming particles
    std::vector<HepMC::GenParticle*>  m_particles_out; //< All outgoing particles
    int m_status;
    /// @todo Are vertex weights used by anyone?! Removed in HepMC3
    WeightContainer m_weights;
    GenEvent* m_event;
    int m_barcode;

  };

  /// Forward-compatibility define (typedefs don't work right with the const keyword)
  #define GenVertexPtr GenVertex*


}

#endif  // HEPMC_GEN_VERTEX_H
