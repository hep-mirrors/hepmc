#ifndef HEPMC_GEN_EVENT_H
#define HEPMC_GEN_EVENT_H

#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/VectorID.h"
#include "HepMC/WeightContainer.h"
#include "HepMC/GenCrossSection.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/Units.h"
#include "HepMC/HepMCDefs.h"

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstddef>	// for std::size_t

///
/// \namespace HepMC
/// All classes in the HepMC packages are in the HepMC namespace 
///
namespace HepMC {

//! The GenEvent class is the core of HepMC

///
/// \class GenEvent 
/// HepMC::GenEvent contains information about generated particles.
/// GenEvent is structured as a set of vertices which contain the particles.
///
/// Data Members:
/// signal_process_id()   The integer ID that uniquely specifies this signal
///                       process, i.e. MSUB in Pythia. It is necessary to
///                       package this with each event rather than with the run
///                       because many processes may be generated within one
///                       run.
/// event_number()        Strictly speaking we cannot think of any reason that
///                       an event would need to know its own event number, it
///                       is more likely something that would be assigned by
///                       a database. It is included anyway (tradition?) since
///                       we expect it may be useful for debugging. It can
///                       be reset later by a database.
/// mpi()                 The number of multi parton interactions in the event.
///                       This is NOT beam pileup.  Set to -1 by default.
/// beam_particles()      A pair of pointers to the incoming beam particles.
/// signal_process_vertex() the vertex containing the signal process
/// weights()             Vector of doubles which specify th weight of the evnt,
///                       the first entry will be the "event weight" used for
///                       hit and miss etc., but a general vector is used to
///                       allow for reweighting etc. We envision a list of
///                       WeightTags to be included with a run class which
///                       would specify the meaning of the Weights .
/// random_states()       Vector of integers which specify the random number 
///                       generator's state for this event. It is left to the
///                       generator to make use of this. We envision a vector of
///                       RndmStatesTags to be included with a run class which
///                       would specify the meaning of the random_states.
///
class GenEvent {
    friend class GenParticle;
    friend class GenVertex;  
public:

    /// default constructor creates null pointers to HeavyIon, PdfInfo, and GenCrossSection
    GenEvent( int signal_process_id = 0, 
              int event_number = 0,
	      std::size_t signal_vertex = -1,
	      const WeightContainer& weights = std::vector<double>(),
	      const std::vector<long>& randomstates = std::vector<long>(),
	      Units::MomentumUnit = Units::default_momentum_unit(), 
	      Units::LengthUnit = Units::default_length_unit() );
    /// explicit constructor that takes HeavyIon and PdfInfo
    GenEvent( int signal_process_id, 
              int event_number,
	      std::size_t signal_vertex, 
	      const WeightContainer& weights,
	      const std::vector<long>& randomstates,
	      const HeavyIon& ion, const PdfInfo& pdf,
	      Units::MomentumUnit = Units::default_momentum_unit(), 
	      Units::LengthUnit = Units::default_length_unit() );
    /// constructor requiring units - all else is default
    GenEvent( Units::MomentumUnit, Units::LengthUnit,
	      int signal_process_id = 0, 
	      int event_number = 0,
	      std::size_t signal_vertex = -1,
	      const WeightContainer& weights = std::vector<double>(),
	      const std::vector<long>& randomstates = std::vector<long>() );
    /// explicit constructor with units first that takes HeavyIon and PdfInfo
    GenEvent( Units::MomentumUnit, Units::LengthUnit,
	      int signal_process_id, 
	      int event_number,
	      std::size_t signal_vertex, 
	      const WeightContainer& weights,
	      const std::vector<long>& randomstates,
	      const HeavyIon& ion, const PdfInfo& pdf );
    GenEvent( const GenEvent& inevent );          //!< deep copy
    GenEvent& operator=( const GenEvent& inevent ); //!< make a deep copy
    virtual ~GenEvent(); //!<deletes all vertices/particles in this evt

    void swap( GenEvent & other );  //!< swap

    void print( std::ostream& ostr = std::cout ) const; //!< dumps to ostr
    void print_version( std::ostream& ostr = std::cout ) const; //!< dumps release version to ostr

    ////////////////////
    // access methods //
    ////////////////////

    int signal_process_id() const; //!<  unique signal process id
    int event_number() const; //!<  event number
    /// Returns the number of multi parton interactions in the event.
    /// This number is -1 if it is not set.
    int mpi() const;
    double event_scale() const; //!< energy scale, see hep-ph/0109068
    double alphaQCD() const; //!<  QCD coupling, see hep-ph/0109068
    double alphaQED() const; //!<  QED coupling, see hep-ph/0109068
    /// pointer to the vertex containing the signal process
    GenVertex const & signal_process_vertex() const;
    /// test to see if we have two valid beam particles
    bool valid_beam_particles() const;
    /// pair of pointers to the two incoming beam particles
    std::pair<HepMC::GenParticle,HepMC::GenParticle> beam_particles() const;
    /// check GenEvent for validity
    /// A GenEvent is presumed valid if it has particles and/or vertices.
    bool is_valid() const;

    ///  lookup particles by index
    GenParticle& lookup_particle( std::size_t ind );
    ///  lookup vertices by index
    GenVertex&   lookup_vertex(   std::size_t ind );

    /// direct access to the weights container is allowed. 
    /// Thus you can use myevt.weights()[2];
    /// to access element 2 of the weights.
    /// or use myevt.weights().push_back( mywgt ); to add an element.
    /// and you can set the weights with myevt.weights() = myvector;
    WeightContainer&        weights(); //!< direct access to WeightContainer
    const WeightContainer&  weights() const; //!< direct access to WeightContainer

    /// access the GenCrossSection container if it exists
    GenCrossSection const *     cross_section() const;
    GenCrossSection*            cross_section();
    /// access the HeavyIon container if it exists
    HeavyIon const *         heavy_ion() const;
    HeavyIon*                heavy_ion();
    /// access the PdfInfo container if it exists
    PdfInfo const *          pdf_info() const;
    PdfInfo*                 pdf_info();

    /// vector of integers containing information about the random state
    const std::vector<long>& random_states() const;

    /// how many particles exist?
    int     particles_size() const;
    /// return true if there are no particles
    bool    particles_empty() const;
    /// how many vertices exist?
    int     vertices_size() const;
    /// return true if there are no vertices
    bool    vertices_empty() const;

    /// Write the unit information to an output stream.  
    /// If the output stream is not defined, use std::cout.
    void write_units( std::ostream & os = std::cout ) const; 
    /// If the cross section is defined,
    /// write the cross section information to an output stream.  
    /// If the output stream is not defined, use std::cout.
    void write_cross_section( std::ostream& ostr = std::cout ) const;

    /// Units used by the GenParticle momentum FourVector.
    Units::MomentumUnit momentum_unit() const;
    /// Units used by the GenVertex position FourVector.
    Units::LengthUnit   length_unit()   const;

    std::ostream& write(std::ostream&);
    std::istream& read(std::istream&);

    /////////////////////
    // mutator methods //
    /////////////////////

    bool    add_vertex( GenVertex& vtx );    //!< adds to evt and adopts

    /// NOT RECOMMENDED - SLOW
    bool    remove_vertex( GenVertex& );     //!< erases vtx from evt
    /// NOT RECOMMENDED - SLOW
    bool    remove_vertex( std::size_t );    //!< erases vtx from evt by index
    /// NOT RECOMMENDED - SLOW
    bool    remove_particle( GenParticle& ); //!< erases particle from evt
    /// NOT RECOMMENDED - SLOW
    bool    remove_particle( std::size_t );  //!< erases particle from evt by index

    void    clear();                         //!< empties the entire event

    void set_signal_process_id( int id ); //!< set unique signal process id
    void set_event_number( int eventno ); //!< set event number
    void set_mpi( int  ); //!< set number of multi parton interactions
    void set_event_scale( double scale ); //!< set energy scale
    void set_alphaQCD( double a ); //!< set QCD coupling
    void set_alphaQED( double a ); //!< set QED coupling

    /// set pointer to the vertex containing the signal process
    void set_signal_process_vertex( GenVertex& );
    void set_signal_process_vertex( std::size_t );
    /// set incoming beam particles
    bool set_beam_particles(GenParticle&, GenParticle&);
    bool set_beam_particles(std::size_t, std::size_t);
    /// use a pair of GenParticle indices to set incoming beam particles
    bool set_beam_particles(std::pair<std::size_t,std::size_t> const &);
    /// provide random state information
    void set_random_states( const std::vector<long>& randomstates );

    /// provide a pointer to the GenCrossSection container
    void set_cross_section( const GenCrossSection& );
    /// provide a pointer to the HeavyIon container
    void set_heavy_ion( const HeavyIon& ion );
    /// provide a pointer to the PdfInfo container
    void set_pdf_info( const PdfInfo& p );

    /// set the units using enums
    /// This method will convert momentum and position data if necessary
    void use_units( Units::MomentumUnit, Units::LengthUnit );
    /// set the units using strings
    /// the string must match the enum exactly
    /// This method will convert momentum and position data if necessary
    void use_units( std::string&, std::string& );

    /// set the units using enums
    /// This method will NOT convert momentum and position data
    void define_units( Units::MomentumUnit, Units::LengthUnit );
    /// set the units using strings
    /// the string must match the enum exactly
    /// This method will NOT convert momentum and position data
    void define_units( std::string&, std::string& );

    //////////////////////
    // simple iterators //
    //////////////////////

    /// const iterator for vertices
    typedef VectorID<HepMC::GenVertex>::const_iterator vertex_const_iterator;
    /// const iterator for particles
    typedef VectorID<HepMC::GenParticle>::const_iterator particle_const_iterator;
    /// begin iteration of vertices
    vertex_const_iterator         vertices_begin() const;
    /// end iteration of vertices
    vertex_const_iterator         vertices_end() const;
    /// begin iteration of particles
    particle_const_iterator      particles_begin() const;
    /// end iteration of particles
    particle_const_iterator      particles_end() const;

    /// iterator for vertices
    typedef VectorID<HepMC::GenVertex>::iterator vertex_iterator;
    /// iterator for particles
    typedef VectorID<HepMC::GenParticle>::iterator particle_iterator;
    /// begin iteration of vertices
    vertex_iterator        vertices_begin();
    /// end iteration of vertices
    vertex_iterator        vertices_end();
    /// begin iteration of particles
    particle_iterator      particles_begin();
    /// end iteration of particles
    particle_iterator      particles_end();

protected:
    //
    // Following methods intended for use by GenParticle/Vertex classes:
    // In general there is no reason they should be used elsewhere.
    ///  intended for use by GenVertex
    std::size_t         add_particle( GenParticle& p );

 private: // methods
    /// internal method used when converting momentum units
    bool use_momentum_unit( Units::MomentumUnit );
    bool use_momentum_unit( std::string& );
    /// internal method used when converting length units
    bool use_length_unit( Units::LengthUnit );
    bool use_length_unit( std::string& );
    
    /// verify that barcodes match vector index
    bool verify_indices();

    // the following internal methods are used by read() and write()

    /// send the beam particles to ASCII output
    std::ostream & write_beam_particles( std::ostream &, 
                	 std::pair<HepMC::GenParticle *,HepMC::GenParticle *> );
    /// send a GenVertex to ASCII output
    std::ostream & write_vertex( std::ostream &, GenVertex const * );
    /// send a GenParticle to ASCII output
    std::ostream & write_particle( std::ostream&, GenParticle const * );
    /// find the file type
    std::istream & find_file_type( std::istream & );
    /// find the key at the end of the block
    std::istream & find_end_key( std::istream &, int & );
    /// get unit information from ASCII input
    std::istream & read_units( std::istream & );
    /// get weight names from ASCII input
    std::istream & read_weight_names( std::istream & );
    /// read the event header line
    std::istream & process_event_line( std::istream &, int &, int &, int &, int & );

    /// add particle to m_particles
    std::size_t add_particle_to_list( HepMC::GenParticle * );
    /// add vertex to m_vertices
    std::size_t add_vertex_to_list( HepMC::GenVertex * );

private: // data members

    // persistent data
    int                   m_signal_process_id;
    int                   m_event_number;  
    int                   m_mpi;        // number of multi paricle interactions
    double                m_event_scale;// energy scale, see hep-ph/0109068
    double                m_alphaQCD;   // QCD coupling, see hep-ph/0109068
    double                m_alphaQED;   // QED coupling, see hep-ph/0109068
    std::size_t           m_signal_process_vertex;	// m_vertices index
    std::size_t           m_beam_particle_1;	// m_particles index
    std::size_t           m_beam_particle_2;	// m_particles index
    WeightContainer       m_weights; // weights for this event first weight
	                             // is used by default for hit and miss
    std::vector<long> m_random_states; // container of rndm num generator states
    VectorID<HepMC::GenVertex>   m_vertices;
    VectorID<HepMC::GenParticle> m_particles;
    GenCrossSection*      m_cross_section;        // undefined by default
    HeavyIon*             m_heavy_ion;            // undefined by default
    PdfInfo*              m_pdf_info;             // undefined by default
    Units::MomentumUnit   m_momentum_unit;    // default value set by configure switch
    Units::LengthUnit     m_position_unit;    // default value set by configure switch

};

// Free Functions

/// standard streaming IO output operator
std::ostream & operator << (std::ostream &, GenEvent &);
/// standard streaming IO input operator
std::istream & operator >> (std::istream &, GenEvent &);
/// set the units for this input stream
std::istream & set_input_units(std::istream &, 
                               Units::MomentumUnit, Units::LengthUnit);
/// Explicitly write the begin block lines that IO_GenEvent uses
std::ostream & write_HepMC_IO_block_begin(std::ostream & );
/// Explicitly write the end block line that IO_GenEvent uses
std::ostream & write_HepMC_IO_block_end(std::ostream & );

// Implemented in terms of GenEvent::use_...
inline GenEvent& convert_units(GenEvent & evt, Units::MomentumUnit m, Units::LengthUnit l)
{
  evt.use_units(m, l);
  return evt;
}

} // HepMC

#endif  // HEPMC_GEN_EVENT_H
