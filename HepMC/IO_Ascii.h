//--------------------------------------------------------------------------
#ifndef HEPMC_IO_ASCII_H
#define HEPMC_IO_ASCII_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, January 2000, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// event input/output in ascii format for machine reading
//////////////////////////////////////////////////////////////////////////
//
// Strategy for reading or writing events/particleData as machine readable
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
//  "HepMC::IO_Ascii-START_EVENT_LISTING\n"
//  and terminated by the key:
//  "HepMC::IO_Ascii-END_EVENT_LISTING\n"
// GenParticle Data tables are preceded by the key:
//  "HepMC::IO_Ascii-START_PARTICLE_DATA\n"
//  and terminated by the key:
//  "HepMC::IO_Ascii-END_PARTICLE_DATA\n"
// Comments are allowed. They need not be preceded by anything, though if
//  a comment is written using write_comment( const string ) then it will be
//  preceded by "HepMC::IO_Ascii-COMMENT\n"
// Each event, vertex, particle, particle data is preceded by 
//  "E ","V ","P ","D "    respectively.
// Comments may appear anywhere in the file -- so long as they do not contain
//  any of the 4 start/stop keys.
//

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "HepMC/IO_BaseClass.h"

namespace HepMC {

    class GenEvent;
    class GenVertex;
    class GenParticle;
    class ParticleData;

    class IO_Ascii : public IO_BaseClass {
    public:
	IO_Ascii( const char* filename="IO_Ascii.dat", 
		  std::ios::openmode mode=std::ios::out );
	virtual       ~IO_Ascii();

	void          write_event( const GenEvent* evt );
	bool          fill_next_event( GenEvent* evt );
	void          write_particle_data_table(const ParticleDataTable*);
	bool          fill_particle_data_table( ParticleDataTable* );
	// insert a comment directly into the output file --- normally you
	//  only want to do this at the beginning or end of the file. All
	//  comments are preceded with "HepMC::IO_Ascii-COMMENT\n"
  	void          write_comment( const std::string comment );

	int           rdstate() const;
	void          clear();

	void          print( std::ostream& ostr = std::cout ) const;

    protected: // for internal use only
	void          write_vertex( GenVertex* );
	void          write_particle( GenParticle* p );
	void          write_particle_data( const ParticleData* d );
	GenVertex*    read_vertex( std::map<GenParticle*,int>& 
				   particle_to_end_vertex );
	GenParticle*  read_particle( std::map<GenParticle*,int>& 
				     particle_to_end_vertex );
	ParticleData* read_particle_data( ParticleDataTable* );
	bool          write_end_listing();
	bool          search_for_key_end( std::istream& in, 
					  const char* key);
	bool          search_for_key_beginning( std::istream& in, 
						const char* key );
	bool          eat_key( std::iostream& in, const char* key );
	int           find_in_map( const std::map<GenVertex*,int>& m, 
				   GenVertex* v) const;
	void          output( const double& );
	void          output( const int& );
	void          output( const long int& );
	void          output( const char& );
    private: // use of copy constructor is not allowed
	IO_Ascii( const IO_Ascii& ) : IO_BaseClass() {}
    private: // data members
	std::ios::openmode  m_mode;
	std::fstream        m_file;
	bool                m_finished_first_event_io;
    };

    //////////////
    // Inlines  //
    //////////////

    inline void IO_Ascii::output( const double& d ) {
	if ( d == 0. ) {
	    m_file << ' ' << (int)0;
	} else {
	    m_file << ' ' << d;
	}
    }
    inline void IO_Ascii::output( const int& i ) { m_file << ' ' << i; }
    inline void IO_Ascii::output( const long int& i ) { m_file << ' ' << i; }
    inline void IO_Ascii::output( const char& c ) { m_file << c; }
    inline int  IO_Ascii::rdstate() const { return (int)m_file.rdstate(); }
    inline void IO_Ascii::clear() { m_file.clear(); }

} // HepMC

#endif  // HEPMC_IO_ASCII_H
//--------------------------------------------------------------------------



