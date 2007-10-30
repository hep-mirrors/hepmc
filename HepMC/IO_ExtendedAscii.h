//--------------------------------------------------------------------------
#ifndef HEPMC_IO_EXTENDEDASCII_H
#define HEPMC_IO_EXTENDEDASCII_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, July 2006
//
// event input/output in ascii format for machine reading
// extended format contains HeavyIon and PdfInfo classes
//////////////////////////////////////////////////////////////////////////
//
// Strategy for reading or writing events as machine readable
//  ascii to a file. When instantiating, the mode of file to be created 
//  must be specified. Options are:
//      std::ios::in     open file for input 
//      std::ios::out    open file for output
//      std::ios::trunc  erase old file when opening (i.e. ios::out|ios::trunc
//                    removes oldfile, and creates a new one for output )
//      std::ios::app    append output to end of file
//  for the purposes of this class, simultaneous input and output mode 
//  ( std::ios::in | std::ios::out ) is not allowed.
// 
// Event listings are preceded by the key:
//  "HepMC::IO_ExtendedAscii-START_EVENT_LISTING\n"
//  and terminated by the key:
//  "HepMC::IO_ExtendedAscii-END_EVENT_LISTING\n"
// GenParticle Data tables are preceded by the key:
//  "HepMC::IO_ExtendedAscii-START_PARTICLE_DATA\n"
//  and terminated by the key:
//  "HepMC::IO_ExtendedAscii-END_PARTICLE_DATA\n"
// Comments are allowed. They need not be preceded by anything, though if
//  a comment is written using write_comment( const string ) then it will be
//  preceded by "HepMC::IO_ExtendedAscii-COMMENT\n"
// Each event, vertex, particle, particle data, heavy ion, or pdf info line
//  is preceded by   "E ","V ","P ","D ","H ","F "    respectively.
// ExtendedAscii ignores particle data blocks
// Comments may appear anywhere in the file -- so long as they do not contain
//  any of the start/stop keys.
//

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "HepMC/IO_BaseClass.h"
#include "HepMC/TempParticleMap.h"

namespace HepMC {

    class GenEvent;
    class GenVertex;
    class GenParticle;
    class ParticleData;
    class HeavyIon;
    class PdfInfo;

    //! IO_ExtendedAscii also deals with HeavyIon and PdfInfo 

    ///
    /// \class  IO_ExtendedAscii
    /// event input/output in ascii format for machine reading
    /// extended format contains HeavyIon and PdfInfo classes
    ///
    class IO_ExtendedAscii : public IO_BaseClass {
    public:
        /// constructor requiring a file name and std::ios mode
	IO_ExtendedAscii( const char* filename="IO_ExtendedAscii.dat", 
		  std::ios::openmode mode=std::ios::out );
	virtual       ~IO_ExtendedAscii();

        /// write this event
	void          write_event( const GenEvent* evt );
        /// get the next event
	bool          fill_next_event( GenEvent* evt );
	void          write_particle_data_table(const ParticleDataTable*);
	bool          fill_particle_data_table( ParticleDataTable* );
	/// insert a comment directly into the output file --- normally you
	///  only want to do this at the beginning or end of the file. All
	///  comments are preceded with "HepMC::IO_ExtendedAscii-COMMENT\n"
  	void          write_comment( const std::string comment );

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
	void          write_heavy_ion( HeavyIon* );
	/// write PDF information
	void          write_pdf_info( PdfInfo* );
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
	/// look for line type (key)
	bool          search_for_key_end( std::istream& in, 
					  const char* key);
	/// look for line type (key)
	bool          search_for_key_beginning( std::istream& in, 
						const char* key );
	/// string manipulation accounting
	bool          eat_key( std::iostream& in, const char* key );
	/// find this vertex in the map of vertices
	int           find_in_map( const std::map<HepMC::GenVertex*,int>& m, 
				   GenVertex* v) const;

	void          output( const double& );  //!< write double
	void          output( const float& );  //!< write float
	void          output( const int& ); //!< write int
	void          output( const long int& ); //!< write long int
	void          output( const char& );  //!< write a single character
    private: // use of copy constructor is not allowed
	IO_ExtendedAscii( const IO_ExtendedAscii& ) : IO_BaseClass() {}
    private: // data members
	std::ios::openmode  m_mode;
	std::fstream        m_file;
	bool                m_finished_first_event_io;
    };

    //////////////
    // Inlines  //
    //////////////

    inline void IO_ExtendedAscii::output( const double& d ) {
	if ( d == 0. ) {
	    m_file << ' ' << (int)0;
	} else {
	    m_file << ' ' << d;
	}
    }
    inline void IO_ExtendedAscii::output( const float& d ) {
	if ( d == 0. ) {
	    m_file << ' ' << (int)0;
	} else {
	    m_file << ' ' << d;
	}
    }
    inline void IO_ExtendedAscii::output( const int& i ) { m_file << ' ' << i; }
    inline void IO_ExtendedAscii::output( const long int& i ) { m_file << ' ' << i; }
    inline void IO_ExtendedAscii::output( const char& c ) { m_file << c; }
    inline int  IO_ExtendedAscii::rdstate() const { return (int)m_file.rdstate(); }
    inline void IO_ExtendedAscii::clear() { m_file.clear(); }

} // HepMC

#endif  // HEPMC_IO_EXTENDEDASCII_H
//--------------------------------------------------------------------------
