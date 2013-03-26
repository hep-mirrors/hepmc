#ifndef HEPMC_GEN_PARTICLE_H
#define HEPMC_GEN_PARTICLE_H

#include "HepMC/Flow.h"
#include "HepMC/Polarization.h"
#include "HepMC/SimpleVector.h"

#include <iostream>

namespace HepMC {

    class GenVertex;
    class GenEvent; 

    //! The GenParticle class contains information about generated particles

    ///
    /// \class GenParticle 
    /// HepMC::GenParticle 
    /// contains momentum, generated mass, particle ID, decay status, 
    /// flow, polarization, indices to production and decay vertices
    /// and a unique barcode identfier.
    ///
    class GenParticle {

	friend class GenVertex; // so vertex can set decay/production vertexes
	friend class GenEvent;  // so event can set the barCodes
	/// print particle
	friend std::ostream& operator<<( std::ostream&, const GenParticle& );

    public:
        /// default constructor
        GenParticle(void);
        /// constructor requires momentum and particle ID
 	GenParticle( const FourVector& momentum, int pdg_id,
		     int status = 0, const Flow& itsflow = Flow(),
		     const Polarization& polar = Polarization(0,0) );
	GenParticle( const GenParticle& inparticle ); //!< shallow copy.
	virtual ~GenParticle();

        void swap( GenParticle & other); //!< swap
	GenParticle& operator=( const GenParticle& inparticle ); //!< shallow.
        /// check for equality
	bool         operator==( const GenParticle& ) const;
        /// check for inequality
	bool         operator!=( const GenParticle& ) const;

	/// dump this particle's full info to ostr
	void       print( std::ostream& ostr = std::cout ) const; 

	operator HepMC::FourVector() const; //!< conversion operator

	////////////////////
	// access methods //
	////////////////////

	/// standard 4 momentum
	const FourVector &          momentum() const;
	/// particle ID
	int                  pdg_id() const;
	/// HEPEVT decay status
	int                  status() const;
	/// particle flow
	const Flow &         flow() const;
	/// particle flow index
	int                  flow( int code_index ) const;
        /// polarization information
	const Polarization & polarization() const;
	/// pointer to the production vertex
	GenVertex&           production_vertex() const;
	/// pointer to the decay vertex
	GenVertex&           end_vertex() const;
	/// index to the production vertex
	size_t           production_vertex_index() const;
	/// index to the decay vertex
	size_t           end_vertex_index() const;
	/// pointer to the event that owns this particle
	GenEvent*            parent_event() const;

        /// Because of precision issues, the generated mass is not always the 
	/// same as the mass calculated from the momentum 4 vector.
        /// If the generated mass has been set, then generated_mass() 
	/// returns that value.
        /// If the generated mass has not been set, then generated_mass() 
	/// returns the mass calculated from the momentum 4 vector.
        double               generated_mass() const; //!< mass as generated

	/// generatedMass() is included for backwards compatibility with CLHEP HepMC
        double               generatedMass() const { return generated_mass(); }


	///
	/// The barcode is the particle's reference number, every vertex in the
	/// event has a unique barcode. Particle barcodes are positive numbers,
	/// vertex barcodes are negative numbers.
	/// 
	/// Please note that the barcodes are intended for internal use within 
	/// HepMC as a unique identifier for the particles and vertices.
	/// Using the barcode to encode extra information is an abuse of 
	/// the barcode data member and causes confusion among users. 
	/// 
	int                  barcode() const; //!< particle barcode

        size_t       event_index();
        const size_t event_index() const;
	
	/// Convenience method.  Returns true if status==1
	bool                 is_undecayed() const;
	/// Convenience method.  Returns true if status==2
	bool                 has_decayed() const;
	/// Convenience method.  Returns true if status==4
	/// Note that using status 4 for beam particles is a new convention which
	/// may not have been implemented by the code originating this GenEvent.
	bool                 is_beam() const;

	/////////////////////
	// mutator methods //
	/////////////////////

	void   set_momentum( const FourVector& vec4 ); //!< set standard 4 momentum
	void   set_pdg_id( int id ); //!< set particle ID
	void   set_status( int status = 0 ); //!< set decay status
	void   set_flow( const Flow& f ); //!< set particle flow
	void   set_flow( int code_index, int code = 0 ); //!< set particle flow index
	/// set polarization
	void   set_polarization( const Polarization& pol = Polarization(0,0) );
        ///  If you do not call set_generated_mass(), then 
        ///  generated_mass() will simply return the mass calculated from momentum()
        void   set_generated_mass( const double & m ); //!< define the actual generated mass

        /// This method is ONLY to be used by VectorID.  Any attempt by
	/// another to set the event index will result in corrupt code.
        void   set_event_index(size_t);

    protected: // for internal use only by friend GenVertex class

	//static unsigned int counter(); //!< temporary for debugging

        /// set production vertex - for internal use only
	void   set_production_vertex_( size_t productionvertex = 0);
        /// set decay vertex - for internal use only
	void   set_end_vertex_( size_t decayvertex = 0 );
	void   set_barcode_( int the_bar_code ); //!< for use by GenEvent only

        /// scale the momentum vector and generated mass 
        /// this method is only for use by GenEvent
	void convert_momentum( const double& );

	/// only the GenEvent (friend) is allowed to set the parent_event
	void                    set_parent_event_( GenEvent* evt ); //!< set parent event

    private:
    
        // persistent data
	FourVector       m_momentum;          // momentum vector
	int              m_pdg_id;            // id according to PDG convention
	int              m_status;            // As defined for HEPEVT
	Flow             m_flow;
	Polarization     m_polarization;
	size_t           m_production_vertex; // index in production vertex list, -1 if vacuum or beam
	size_t           m_end_vertex;        // index in production end list, -1 if not-decayed
	size_t           m_event_index;       // index in master list, -1 if not set
	int              m_barcode;           // unique identifier in the event
        double           m_generated_mass;    // mass of this particle when it was generated

        // transient data
	GenEvent*        m_event;             // parent event

    };  


} // HepMC

#endif  // HEPMC_GEN_PARTICLE_H

