//--------------------------------------------------------------------------
#ifndef HEPMC_IO_GENEVENT_H
#define HEPMC_IO_GENEVENT_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, July 2007
// with input from Gavin Salam, salam@lpthe.jussieu.fr
//
// event input/output in ascii format for machine reading
// This class persists all information found in a GenEvent
//////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "HepMC/IO_BaseClass.h"
#include "HepMC/TempParticleMap.h"
#include "HepMC/CommonIO.h"

namespace HepMC {

    class GenEvent;
    class GenVertex;
    class GenParticle;
    class ParticleData;
    class HeavyIon;
    class PdfInfo;

    //! IO_GenEvent also deals with HeavyIon and PdfInfo 

    ///
    /// \class  IO_GenEvent
    /// event input/output in ascii format for machine reading
    /// extended format contains HeavyIon and PdfInfo classes
    ///
    /// Strategy for reading or writing events using iostreams
    /// When instantiating with a file name, the mode of file to be created 
    ///  must be specified. Options are:
    ///      std::ios::in     open file for input 
    ///      std::ios::out    open file for output
    ///      std::ios::trunc  erase old file when opening (i.e. ios::out|ios::trunc
    ///                    removes oldfile, and creates a new one for output )
    ///      std::ios::app    append output to end of file
    ///  for the purposes of this class, simultaneous input and output mode 
    ///  ( std::ios::in | std::ios::out ) is not allowed.
    /// 
    /// Event listings are preceded by the key:
    ///  "HepMC::IO_GenEvent-START_EVENT_LISTING\n"
    ///  and terminated by the key:
    ///  "HepMC::IO_GenEvent-END_EVENT_LISTING\n"
    /// GenParticle Data tables are preceded by the key:
    ///  "HepMC::IO_GenEvent-START_PARTICLE_DATA\n"
    ///  and terminated by the key:
    ///  "HepMC::IO_GenEvent-END_PARTICLE_DATA\n"
    /// Comments are allowed. They need not be preceded by anything, though if
    ///  a comment is written using write_comment( const string ) then it will be
    ///  preceded by "HepMC::IO_GenEvent-COMMENT\n"
    /// Each event, vertex, particle, particle data, heavy ion, or pdf info line
    ///  is preceded by   "E ","V ","P ","D ","H ","F "    respectively.
    /// ExtendedAscii ignores particle data blocks
    /// Comments may appear anywhere in the file -- so long as they do not contain
    ///  any of the start/stop keys.
    ///
    class IO_GenEvent : public IO_BaseClass {
    public:
        /// constructor requiring a file name and std::ios mode
	IO_GenEvent( const char* filename="IO_GenEvent.dat", 
		  std::ios::openmode mode=std::ios::out );
        /// constructor requiring an input stream
	IO_GenEvent( std::istream & );
        /// constructor requiring an output stream
	IO_GenEvent( std::ostream & );
	virtual       ~IO_GenEvent();

        /// write this event
	void          write_event( const GenEvent* evt );
        /// get the next event
	bool          fill_next_event( GenEvent* evt );
	void          write_particle_data_table(const ParticleDataTable*);
	bool          fill_particle_data_table( ParticleDataTable* );
	/// insert a comment directly into the output file --- normally you
	///  only want to do this at the beginning or end of the file. All
	///  comments are preceded with "HepMC::IO_GenEvent-COMMENT\n"
  	void          write_comment( const std::string comment );
	
	/// integer associated with read error
	const int           error_type()    const;
	/// the read error message string
	const std::string & error_message() const;

	int           rdstate() const;  //!< check the state of the IO stream
	void          clear();  //!< clear the IO stream

        /// write to ostr
	void          print( std::ostream& ostr = std::cout ) const;

    protected: // for internal use only
	/// write vertex information
	void          write_vertex( GenVertex* );
	/// write beam particle information
	void          write_beam_particles( std::pair<HepMC::GenParticle *,HepMC::GenParticle *> );
	/// write heavy ion information
	void          write_heavy_ion( HeavyIon const * );
	/// write PDF information
	void          write_pdf_info( PdfInfo const * );
	/// write particle information
	void          write_particle( GenParticle* p );
	/// write particle data information
	void          write_particle_data( const ParticleData* d );
	/// read vertex information
	GenVertex*    read_vertex( TempParticleMap& particle_to_end_vertex );
	/// read GenParticle information
	GenParticle*  read_particle( TempParticleMap& particle_to_end_vertex );
	/// read particle data table information
	ParticleData* read_particle_data( ParticleDataTable* );
	/// read heavy ion information
	HeavyIon*     read_heavy_ion(  );
	/// read PDF information
	PdfInfo*      read_pdf_info(  );
	/// write end tag
	bool          write_end_listing();

	void          output( const double& );  //!< write double
	void          output( const float& );  //!< write float
	void          output( const int& ); //!< write int
	void          output( const long& ); //!< write long
	void          output( const char& );  //!< write a single character
    private: // use of copy constructor is not allowed
	IO_GenEvent( const IO_GenEvent& ) : IO_BaseClass() {}
    private: // data members
	std::ios::openmode  m_mode;
	std::fstream        m_file;
	std::ostream *      m_ostr;
	std::istream *      m_istr;
	std::ios *          m_iostr;
	bool                m_finished_first_event_io;
	bool                m_have_file;
	CommonIO            m_common_io;
	int                 m_error_type;
	std::string         m_error_message;
    };

    //////////////
    // Inlines  //
    //////////////

    inline void IO_GenEvent::output( const double& d ) {
	if( m_ostr  ) {
	    if ( d == 0. ) {
		*m_ostr << ' ' << (int)0;
	    } else {
		*m_ostr << ' ' << d;
	    }
	}
    }
    inline void IO_GenEvent::output( const float& d ) {
	if( m_ostr  ) {
	    if ( d == 0. ) {
		*m_ostr << ' ' << (int)0;
	    } else {
		*m_ostr << ' ' << d;
	    }
	}
    }
    inline void IO_GenEvent::output( const int& i ) { 
	if( m_ostr  ) {
	    if ( i == 0. ) {
		*m_ostr << ' ' << (int)0;
	    } else {
        	*m_ostr << ' ' << i; 
	    }
	}
    }
    inline void IO_GenEvent::output( const long& i ) {
	if( m_ostr  ) {
	    if ( i == 0. ) {
		*m_ostr << ' ' << (int)0;
	    } else {
        	*m_ostr << ' ' << i; 
	    }
	}
    }
    inline void IO_GenEvent::output( const char& c ) {
	if( m_ostr  ) {
	    if ( c ) {
        	*m_ostr << c; 
	    } else {
		*m_ostr << ' ' ;
	    }
	}
    }
    inline int  IO_GenEvent::rdstate() const { 
        int state;
	if( m_istr ) {
	    state =  (int)m_istr->rdstate();
	} else {
	    state =  (int)m_ostr->rdstate();
	}
        return state; 
    }
    inline void IO_GenEvent::clear() { 
	if( m_istr ) {
	    m_istr->clear();
	} else {
	    m_ostr->clear();
	}
    }

    inline const int IO_GenEvent::error_type() const {
        return m_error_type;
    }
   
    inline const std::string & IO_GenEvent::error_message() const {
        return m_error_message;
    }

    // these are required by IO_BaseClass, but not used here
    inline void IO_GenEvent::write_particle_data_table(const ParticleDataTable*) {;}
    inline bool IO_GenEvent::fill_particle_data_table( ParticleDataTable* ) 
    { return false;}

} // HepMC

#endif  // HEPMC_IO_GENEVENT_H
//--------------------------------------------------------------------------
