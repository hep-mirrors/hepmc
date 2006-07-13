//--------------------------------------------------------------------------
#ifndef HEPMC_IO_ASCIIPARTICLES_H
#define HEPMC_IO_ASCIIPARTICLES_H

//////////////////////////////////////////////////////////////////////////
// Mikhail.Kirsanov@Cern.CH, 2006
// event input/output in ascii format for eye and machine reading
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
//  "HepMC::IO_AsciiParticles-START_EVENT_LISTING\n"
//  and terminated by the key:
//  "HepMC::IO_AsciiParticles-END_EVENT_LISTING\n"
// Comments are allowed. They need not be preceded by anything, though if
//  a comment is written using write_comment( const string ) then it will be
//  preceded by "HepMC::IO_AsciiParticles-COMMENT\n"
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

    class IO_AsciiParticles : public IO_BaseClass {
    public:
	IO_AsciiParticles( const char* filename="IO_AsciiParticles.dat", 
		  std::ios::openmode mode=std::ios::out );
	virtual       ~IO_AsciiParticles();

  	void          write_event( const GenEvent* evt );
	bool          fill_next_event( GenEvent* evt );
    inline void          write_particle_data_table(const ParticleDataTable*);
    inline bool          fill_particle_data_table( ParticleDataTable* );
	// insert a comment directly into the output file --- normally you
	//  only want to do this at the beginning or end of the file. All
	//  comments are preceded with "HepMC::IO_AsciiParticles-COMMENT\n"
  	void          write_comment( const std::string comment );

    void          setPrecision(int iprec);

	int           rdstate() const;
	void          clear();

	void          print( std::ostream& ostr = std::cout ) const;

    protected: // for internal use only
	bool          write_end_listing();
    private: // use of copy constructor is not allowed
	IO_AsciiParticles( const IO_AsciiParticles& ) : IO_BaseClass() {}
    private: // data members
    int                 m_precision;
	std::ios::openmode  m_mode;
	std::fstream*       m_file;
    std::ostream*       m_outstream;
	bool                m_finished_first_event_io;
    };

    //////////////
    // Inlines  //
    //////////////

    inline int  IO_AsciiParticles::rdstate() const { return (int)m_file->rdstate(); }
    inline void IO_AsciiParticles::clear() { m_file->clear(); }
    inline void IO_AsciiParticles::setPrecision(int iprec) { m_precision=iprec; }

    /////////////////////
    // Inline dummies  //
    /////////////////////

  void IO_AsciiParticles::write_particle_data_table(const ParticleDataTable*) {;}
  bool IO_AsciiParticles::fill_particle_data_table( ParticleDataTable* ) {return false;}

} // HepMC

#endif  // HEPMC_IO_ASCIIPARTICLES_H
//--------------------------------------------------------------------------