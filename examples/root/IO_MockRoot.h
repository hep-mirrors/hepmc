//--------------------------------------------------------------------------
#ifndef HEPMC_IO_MOCKROOT_H
#define HEPMC_IO_MOCKROOT_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, May 2011
//
// event input/output in ascii format for machine reading
// This class persists all information found in a GenEvent
// Format: event header, list of vertices, list of particles
//////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "HepMC/IO_BaseClass.h"
#include "HepMC/IO_Exception.h"
#include "HepMC/Units.h"

namespace HepMC {

class GenEvent;
class GenVertex;
class GenParticle;
class HeavyIon;
class PdfInfo;

//! IO_MockRoot also deals with HeavyIon and PdfInfo 

///
/// \class  IO_MockRoot
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
///  "HepMC::IO_MockRoot-START_EVENT_LISTING\n"
///  and terminated by the key:
///  "HepMC::IO_MockRoot-END_EVENT_LISTING\n"
/// Comments are allowed. They need not be preceded by anything, though if
///  a comment is written using write_comment( const string ) then it will be
///  preceded by "HepMC::IO_MockRoot-COMMENT\n"
/// Each event, vertex, particle, particle data, heavy ion, or pdf info line
///  is preceded by   "E ","V ","P ","D ","H ","F "    respectively.
/// Comments may appear anywhere in the file -- so long as they do not contain
///  any of the start/stop keys.
///
class IO_MockRoot : public IO_BaseClass {
public:
    /// constructor requiring a file name and std::ios mode
    IO_MockRoot( const std::string& filename="IO_MockRoot.dat", 
	      std::ios::openmode mode=std::ios::out );
    /// constructor requiring an input stream
    IO_MockRoot( std::istream & );
    /// constructor requiring an output stream
    IO_MockRoot( std::ostream & );
    virtual       ~IO_MockRoot();

    /// write this event
    void          write_event( const GenEvent* evt );
    /// get the next event
    bool          fill_next_event( GenEvent* evt );
    /// insert a comment directly into the output file --- normally you
    ///  only want to do this at the beginning or end of the file. All
    ///  comments are preceded with "HepMC::IO_MockRoot-COMMENT\n"
    void          write_comment( const std::string comment );

    int           rdstate() const;  //!< check the state of the IO stream
    void          clear();  //!< clear the IO stream

    /// write to ostr
    void          print( std::ostream& ostr = std::cout ) const;

    /// needed when reading a file without units if those units are 
    /// different than the declared default units 
    /// (e.g., the default units are MeV, but the file was written with GeV)
    /// This method is not necessary if the units are written in the file
    void use_input_units( Units::MomentumUnit, Units::LengthUnit );

    /// set output precision
    /// The default precision is 16.
    void precision( int );
	
    /// integer (enum) associated with read error
    int           error_type()    const;
    /// the read error message string
    const std::string & error_message() const;

private: // use of copy constructor is not allowed
    IO_MockRoot( const IO_MockRoot& ) : IO_BaseClass() {}

private: // data members
    std::ios::openmode  m_mode;
    std::fstream        m_file;
    std::ostream *      m_ostr;
    std::istream *      m_istr;
    std::ios *          m_iostr;
    bool                m_have_file;
    IO_Exception::ErrorType m_error_type;
    std::string         m_error_message;

};

//////////////
// Inlines  //
//////////////

inline int  IO_MockRoot::rdstate() const { 
    int state;
    if( m_istr ) {
	state =  (int)m_istr->rdstate();
    } else {
	state =  (int)m_ostr->rdstate();
    }
    return state; 
}

inline void IO_MockRoot::clear() { 
    if( m_istr ) {
	m_istr->clear();
    } else {
	m_ostr->clear();
    }
}

inline int IO_MockRoot::error_type() const {
    return m_error_type;
}

inline const std::string & IO_MockRoot::error_message() const {
    return m_error_message;
}

} // HepMC

#endif  // HEPMC_IO_MOCKROOT_H
//--------------------------------------------------------------------------
