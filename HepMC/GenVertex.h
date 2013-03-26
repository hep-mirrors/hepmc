#ifndef HEPMC_GEN_VERTEX_H
#define HEPMC_GEN_VERTEX_H
 
#include "HepMC/WeightContainer.h"
#include "HepMC/SimpleVector.h"

#include <iostream>
#include <iterator>
#include <vector>
#include <set>
#include <algorithm>
#include <cstddef>

namespace HepMC {

class GenParticle;
class GenEvent;

//! GenVertex contains information about decay vertices.

///
/// \class GenVertex 
/// HepMC::GenVertex contains the position in space and time of a decay.
/// It also contains lists of incoming and outgoing particles.  
///
class GenVertex {

    /// print vertex information
    friend std::ostream& operator<<( std::ostream&, const GenVertex& );
    friend class GenEvent;

public:
    /// default constructor
    GenVertex( const FourVector& position =FourVector(0,0,0,0),
	       int id = 0, 
	       const WeightContainer& weights = std::vector<double>() );
    GenVertex( const GenVertex& invertex );            //!< shallow copy
    virtual    ~GenVertex();

    void swap( GenVertex & other); //!< swap
    GenVertex& operator= ( const GenVertex& invertex ); //!< shallow
    bool       operator==( const GenVertex& a ) const; //!< equality
    bool       operator!=( const GenVertex& a ) const; //!< inequality
    void       print( std::ostream& ostr = std::cout ) const; //!< print vertex information

    double     check_momentum_conservation() const;//!< |Sum (three_mom_in-three_mom_out)|

    /// add incoming particle
    void       add_particle_in( GenParticle& inparticle );
    /// add outgoing particle
    void       add_particle_out( GenParticle& outparticle );
    
    /// NOT RECOMMENDED - SLOW
    /// remove_particle finds *particle in the in and/or out list and
    ///  removes it from these lists ... it DOES NOT DELETE THE PARTICLE 
    ///  or its relations. You could delete the particle too as follows:
    ///      delete vtx->remove_particle( particle );
    GenParticle& remove_particle( GenParticle& particle ); //!< remove a particle

    operator    HepMC::FourVector() const; //!< conversion operator
    operator   HepMC::ThreeVector() const; //!< conversion operator

    ////////////////////
    // access methods //
    ////////////////////

    /// pointer to the event that owns this vertex
    GenEvent*               parent_event() const;
    /// vertex position
    ThreeVector             point3d() const;
    /// vertex position and time
    const FourVector &      position() const;
    /// set vertex position and time
    void                    set_position( const FourVector& position = FourVector(0,0,0,0) );
    /// we don't define what you use the id for -- but we imagine,
    /// for example it might code the meaning of the weights()
    int                     id() const;  //!< vertex ID
    void                    set_id( int id );  //!< set vertex ID

    size_t event_index() const;

    ///  lookup particles by index
    GenParticle& lookup_incoming_particle( size_t ind ) const;
    ///  lookup vertices by index
    GenParticle& lookup_outgoing_particle( size_t ind ) const;

    ///
    /// The barcode is the vertex's reference number, every vertex in the
    /// event has a unique barcode. Vertex barcodes are negative numbers,
    /// particle barcodes are positive numbers.
    /// 
    /// Please note that the barcodes are intended for internal use within 
    /// HepMC as a unique identifier for the particles and vertices.
    /// Using the barcode to encode extra information is an abuse of 
    /// the barcode data member and causes confusion among users. 
    /// 
    int                     barcode() const; //!< unique identifier

    /// direct access to the weights container is allowed. 
    WeightContainer&        weights();
    /// const direct access to the weights container
    const WeightContainer&  weights() const;

    /// const iterator for incoming particles
    typedef std::vector<size_t>::const_iterator particles_in_const_iterator;
    /// const iterator for outgoing particles
    typedef std::vector<size_t>::const_iterator particles_out_const_iterator;
    /// begin iteration of incoming particles
    particles_in_const_iterator         particles_in_const_begin() const;
    /// end iteration of incoming particles
    particles_in_const_iterator         particles_in_const_end() const;
    /// begin iteration of outgoing particles
    particles_out_const_iterator        particles_out_const_begin() const;
    /// end iteration of outgoing particles
    particles_out_const_iterator        particles_out_const_end() const;

    /// iterator for incoming particles
    typedef std::vector<size_t>::iterator particles_in_iterator;
    /// iterator for outgoing particles
    typedef std::vector<size_t>::iterator particles_out_iterator;
    /// begin iteration of incoming particles
    particles_in_iterator         particles_in_begin();
    /// end iteration of incoming particles
    particles_in_iterator         particles_in_end();
    /// begin iteration of outgoing particles
    particles_out_iterator        particles_out_begin();
    /// end iteration of outgoing particles
    particles_out_iterator        particles_out_end();

    /// number of incoming particles
    size_t                                 particles_in_size() const;
    /// number of outgoing particles
    size_t                                 particles_out_size() const;

    /////////////////////
    // mutator methods //
    /////////////////////

    /// This method is ONLY to be used by VectorID.  Any attempt by
    /// another to set the event index will result in corrupt code.
    void   set_event_index(size_t);

protected: 
    /// for internal use only
    void delete_adopted_particles();
    /// for internal use only - remove particle from incoming list
    void remove_particle_in( GenParticle& );
    /// for internal use only - remove particle from outgoing list
    void remove_particle_out( GenParticle& );
    /// scale the position vector
    /// this method is only for use by GenEvent
    void convert_position( const double& );

    /// only the GenEvent (friend) is allowed to set the parent_event,
    ///  and barcode. It is done automatically anytime you add a 
    ///  vertex to an event
    void                    set_parent_event_( GenEvent* evt ); //!< set parent event
    void                    set_barcode_( int the_bar_code ); //!< set identifier
    void                    change_parent_event_( GenEvent* evt ); //!< for use with swap

private: // use with copy constructor
    std::vector<size_t> particles_in()  { return m_particles_in; }
    std::vector<size_t> particles_out() { return m_particles_out; }


private: // GenVertex data members

    // persistent data
    FourVector           m_position;      //4-vec of vertex [mm]
    std::vector<size_t>  m_particles_in;  //all incoming particles - master list owned by GenEvent
    std::vector<size_t>  m_particles_out; //all outgoing particles - master list owned by GenEvent
    size_t               m_event_index;   // index in master list, -1 if not set
    int                  m_id;
    WeightContainer      m_weights;       // weights for this vtx
    int                  m_barcode;   // unique identifier in the event
    
    // transient data
    GenEvent*            m_event;   //! reconstruct this

};  


} // HepMC

#endif  // HEPMC_GEN_VERTEX_H
