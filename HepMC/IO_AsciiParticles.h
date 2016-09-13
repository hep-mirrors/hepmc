#ifndef HEPMC_IO_ASCIIPARTICLES_H
#define HEPMC_IO_ASCIIPARTICLES_H

//////////////////////////////////////////////////////////////////////////
// Mikhail.Kirsanov@cern.ch, 2006
// Event input/output in ASCII format for eye and machine reading
//////////////////////////////////////////////////////////////////////////

/// Strategy for reading or writing events as machine readable
///  ascii to a file. When instantiating, the mode of file to be created
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
///  "HepMC::IO_AsciiParticles-START_EVENT_LISTING\n"
///  and terminated by the key:
///  "HepMC::IO_AsciiParticles-END_EVENT_LISTING\n"
/// Comments are allowed. They need not be preceded by anything, though if
///  a comment is written using write_comment( const string ) then it will be
///  preceded by "HepMC::IO_AsciiParticles-COMMENT\n"
/// Each event, vertex, particle, particle data is preceded by
///  "E ","V ","P ","D "    respectively.
/// Comments may appear anywhere in the file -- so long as they do not contain
///  any of the 4 start/stop keys.

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "HepMC/IO_BaseClass.h"
#define MINIMAL_HEPMC_IO_ASCIIPARTICLE_MASS 0.0001
namespace HepMC {

  /// @todo Why not just #include GenEvent.h, since it will definitely be needed if using an IO_* class?
  class GenEvent;
  class GenVertex;
  class GenParticle;

  //! Event input/output in ASCII format for eye and machine reading
  ///
  /// Strategy for reading or writing events as machine readable ASCII to a
  /// file or stream. When instantiating, the mode of file to be created must be
  /// specified.
  ///
  class IO_AsciiParticles : public IO_BaseClass {
  public:

    /// Constructor taking an IO stream name and std::ios mode
    IO_AsciiParticles( std::iostream& iostr,
                       std::ios::openmode mode=std::ios::out );
   /// Deprecated(???) constructor requiring a file name and std::ios mode
	IO_AsciiParticles( const char* filename="IO_AsciiParticles.dat", 
		  std::ios::openmode mode=std::ios::out );

    /// Constructor requiring a file name and std::ios mode
    IO_AsciiParticles( const std::string& filename,
                       std::ios::openmode mode=std::ios::out );

    /// Virtual destructor to allow inheritance
    /// @todo Is allowing inheritance from this class useful?
    virtual ~IO_AsciiParticles();

    /// Write this event
    void write_event( const GenEvent* evt );

    /// Get the next event
    bool fill_next_event( GenEvent* evt );

    /// Insert a comment directly into the output file --- normally you only
    /// want to do this at the beginning or end of the file. All comments are
    /// preceded with "HepMC::IO_AsciiParticles-COMMENT\n"
    void write_comment( const std::string comment );

    /// Set output precision
    void set_precision(int iprec) { m_precision = iprec; }

    /// @todo Need extra std:: IO functions, too

    /// Check the state of the IO stream
    /// @todo See STL documentation for meaning and usage of rdstate
    int rdstate() const { return (int)m_file->rdstate(); }

    /// Clear the IO stream
    void clear() { if (m_file) m_file->clear(); }

    /// Write to an ostream
    /// @deprecated Specify the ostream in the constructor if you want to write to the terminal.
    void print(std::ostream& ostr=std::cout) const;


  protected: // for internal use only

    /// Write end tag
    bool write_end_listing();

  private: // use of copy constructor is not allowed

    IO_AsciiParticles( const IO_AsciiParticles& ) : IO_BaseClass() {}

  private: // data members

    int                 m_precision;
    std::ios::openmode  m_mode;
    std::fstream*       m_file;
    std::ostream*       m_outstream;
    bool                m_finished_first_event_io;

  };


} // HepMC

#endif  // HEPMC_IO_ASCIIPARTICLES_H
