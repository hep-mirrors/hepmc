#ifndef HEPMC_GEN_PARTICLE_H
#define HEPMC_GEN_PARTICLE_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// particle within an event coming in/out of a vertex
// particle is the basic building block or unit of the event record
//////////////////////////////////////////////////////////////////////////
//
// example:
//      GenParticle* p = new GenParticle( FourVector(1,1,1,3), 11, 1 );
// creates a particle with 4-vector (p,E)=1,1,1,3 - with pdg id 11 (electron)
// and give this particle status =1.
//
// the pointers to end/production vertices can only be set by the
//  vertices themselves - thus to set the production vertex for a particle,
//  you add the particle to that vertex with GenVertex::add_particle_out()
//
// We decide not to have a separate 4 vector for the momentum at decay time
// (which MC++ includes to allow dE/dX losses etc).  If you want that, just add
// a decay vertex with the same particle (modified momentum) going out
//

#include "HepMC/Flow.h"
#include "HepMC/Polarization.h"
#include "HepMC/SimpleVector.h"
#include "HepMC/IteratorRange.h"
#include <iostream>

/// @todo Why? And why here?
#ifdef _WIN32
#define hepmc_uint64_t  __int64
#else
#include <stdint.h> // for uint64_t
#define hepmc_uint64_t   uint64_t
#endif

namespace HepMC {

  class GenVertex;
  class GenEvent;

  class GenParticleProductionRange;
  class ConstGenParticleProductionRange;
  class GenParticleEndRange;
  class ConstGenParticleEndRange;

  /// @brief The GenParticle class contains information about generated particles
  ///
  /// HepMC::GenParticle contains momentum, generated mass, particle ID, decay
  /// status, flow, polarization, pointers to production and decay vertices and
  /// a unique barcode identifier.
  ///
  class GenParticle {

    friend class GenVertex; // so vertex can set decay/production vertexes
    friend class GenEvent;  // so event can set the barCodes
    /// print particle
    friend std::ostream& operator<<( std::ostream&, const GenParticle& );

  public:

    /// Default constructor
    GenParticle();

    /// Constructor requiring momentum and particle ID args
    /// @todo Remove default-constructed flow and polarization objs -- these should be nullable
    GenParticle( const FourVector& momentum, int pdg_id,
                 int status = 0, const Flow& itsflow = Flow(),
                 const Polarization& polar = Polarization(0,0) );

    GenParticle( const GenParticle& inparticle ); //!< shallow copy.

    virtual ~GenParticle();

    void swap( GenParticle& other); //!< swap
    GenParticle& operator=( const GenParticle& inparticle ); //!< shallow.

    /// check for equality
    bool operator == ( const GenParticle& ) const;
    /// check for inequality
    bool operator != ( const GenParticle& ) const;

    /// dump this particle's full info to ostr
    void print( std::ostream& ostr = std::cout ) const;

    /// Conversion operator to 4-vector
    operator HepMC::FourVector() const { return m_momentum; }

    ////////////////////
    // access methods //
    ////////////////////

    /// Get the 4-momentum
    const FourVector& momentum() const { return m_momentum; }

    /// Get the particle ID code (preferred simpler name)
    int pid() const { return m_pdg_id; }
    /// Get the particle ID code
    int pdg_id() const { return pid(); }

    /// Get the absolute value of the particle ID code (preferred simpler name)
    int abspid() const { return std::abs(pid()); }
    /// Get the absolute value of the particle ID code
    int abs_pdg_id() const { return abspid(); }

    /// Get the particle status
    int status() const { return m_status; }

    /// Particle flow
    /// @todo Should be handled as a pointer, since optional... but too late now
    /// @deprecated Flow will be removed in HepMC3 -- stop using it!
    const Flow& flow() const { return m_flow; }
    /// Particle flow index
    int flow( int code_index ) const { return m_flow.icode( code_index ); }

    /// Polarization information
    /// @deprecated Polarization will be removed in HepMC3 -- stop using it!
    /// @todo Should be handled as a pointer, since optional... but too late now
    const Polarization& polarization() const { return m_polarization; }

    /// Pointer to the production vertex
    GenVertex* production_vertex() { return m_production_vertex; }
    /// Pointer to the production vertex (const)
    const GenVertex* production_vertex() const { return m_production_vertex; }

    /// Pointer to the decay vertex
    GenVertex* end_vertex() { return m_end_vertex; }
    /// Pointer to the decay vertex (const)
    const GenVertex* end_vertex() const { return m_end_vertex; }

    /// Pointer to the event that owns this particle
    GenEvent* parent_event() const;

    /// @brief Return the generated mass
    ///
    /// Because of precision issues, the generated mass is not always the
    /// same as the mass calculated from the momentum 4 vector.
    ///
    /// @note There is no invalid value; the default is 0.0 which cannot be distinguished from true masslessness! Oops. This will be improve in HepMC3.
    double generated_mass() const { return m_generated_mass; }

    /// An alias to generated_mass() included for backwards compatibility with CLHEP HepMC
    /// @deprecated Use generated_mass()
    /// @todo REMOVE
    // double generatedMass() const { return generated_mass(); }

    /// Return the particle barcode
    ///
    /// The barcode is the particle's reference number: every vertex in an
    /// event has a unique barcode. Particle barcodes are positive numbers,
    /// vertex barcodes are negative numbers.
    ///
    /// @note Barcodes are primarily intended for internal use within HepMC as a
    /// unique identifier for the particles and vertices.  Using the barcode to
    /// encode extra information is not recommended and cannot be guaranteed to work.
    ///
    /// @note Barcodes will change a lot in HepMC3!
    int barcode() const { return m_barcode; }

    /// Check if the particle is undecayed. Returns true if status==1
    /// @todo Also check that the end_vertex is null?
    bool is_undecayed() const { return status() == 1; }

    /// Check if the particle is undecayed. Returns true if status==2
    /// @todo Also check that the end_vertex is not null?
    bool has_decayed() const { return status() == 2; }

    /// Check if this is a beam particle.  Returns true if status==4
    ///
    /// @note Using status 4 for beam particles was a late addition to the
    /// status coding convention: old event generators may not respect it.
    ///
    /// @todo Also check beam_particles() and for a null production_vertex
    bool is_beam() const { return status() == 4; }

    /// Incoming particle range
    GenParticleProductionRange particles_in( IteratorRange range = relatives );
    /// Incoming particle range
    ConstGenParticleProductionRange particles_in( IteratorRange range = relatives ) const;
    /// Outgoing particle range
    GenParticleEndRange particles_out( IteratorRange range = relatives );
    /// Outgoing particle range
    ConstGenParticleEndRange particles_out( IteratorRange range = relatives ) const;


    /////////////////////
    // Mutator methods //
    /////////////////////

    /// @brief Suggest a barcode for this particle.
    ///
    /// Usually it is better to let the event automatically pick the barcode.
    ///
    /// Returns TRUE if the suggested barcode has been accepted (i.e. the
    ///  suggested barcode has not already been used in the event,
    ///  and so it was used).
    /// Returns FALSE if the suggested barcode was rejected, or if the
    ///  particle is not yet part of an event, such that it is not yet
    ///  possible to know if the suggested barcode will be accepted).
    ///
    /// @deprecated Will be removed in HepMC3
    bool suggest_barcode( int the_bar_code );

    /// Set standard 4 momentum
    void set_momentum( const FourVector& vec4 ) { m_momentum = vec4; }

    /// Set the particle ID code (preferred shorter name)
    void set_pid( int id ) { m_pdg_id = id; }
    /// Set the particle ID code
    void set_pdg_id( int id ) { m_pdg_id = id; }

    /// Set the status code
    void set_status( int status = 0 ) { m_status = status; }

    /// Set the particle flow object
    /// @todo -> ptr
    void set_flow( const Flow& f ) { m_flow = f; }

    /// Set the particle flow index
    void set_flow( int code_index, int code = 0 ) {
      if ( code == 0 ) {
        m_flow.set_unique_icode( code_index );
      } else {
        m_flow.set_icode( code_index, code );
      }
    }

    /// Set the polarization object
    /// @todo -> ptr
    void set_polarization( const Polarization& pol = Polarization(0,0) ) {
      m_polarization = pol;
    }

    /// @brief Set the generated mass
    ///
    /// @note There is no invalid value; the default is 0.0 which cannot be distinguished from true masslessness! Oops.
    void set_generated_mass(double m) { m_generated_mass = m; }

    /// Alias for backwards compatibility with CLHEP HepMC
    /// @deprecated Use set_generated_mass
    /// @todo Get rid of the silly double reference
    /// @todo REMOVE
    // void setGeneratedMass( const double& m ) { set_generated_mass(m); }


  protected: // for internal use only by friend GenVertex class

    /// set production vertex - for internal use only
    void set_production_vertex_( GenVertex* productionvertex = 0);
    /// set decay vertex - for internal use only
    void set_end_vertex_( GenVertex* decayvertex = 0 );
    //!< for use by GenEvent only
    void set_barcode_( int bc ) { m_barcode = bc; }

    /// scale the momentum vector and generated mass
    /// this method is only for use by GenEvent
    void convert_momentum( const double& );

  private:
    FourVector       m_momentum;          //< Momentum vector
    int              m_pdg_id;            //< Particle ID code according to the PDG scheme
    int              m_status;            //< Particle status
    Flow             m_flow;              //< Colour flow object
    Polarization     m_polarization;      //< Polarization object
    GenVertex*       m_production_vertex; //< Null if vacuum or beam
    GenVertex*       m_end_vertex;        //< Null if not-decayed
    int              m_barcode;           //< Unique identifier in the event
    double           m_generated_mass;    //< Mass of this particle as set by the generator

  };


} // HepMC

#endif  // HEPMC_GEN_PARTICLE_H
