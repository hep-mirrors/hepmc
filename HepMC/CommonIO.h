#ifndef HEPMC_COMMON_IO_H
#define HEPMC_COMMON_IO_H
// ----------------------------------------------------------------------
//
// CommonIO.h
// Author:  Lynn Garren
//
//  Allowed keys used at the beginning and end of HepMC data dumps
//
// ----------------------------------------------------------------------

#include <fstream>
#include <string>

#include "HepMC/GenEvent.h"
#include "HepMC/TempParticleMap.h"

namespace HepMC {

/// The known_io enum is used to track which type of input is being read
enum known_io { gen=1, ascii, extascii };

class CommonIO {

public:

  CommonIO();
  ~CommonIO() {;}

  // input keys - IO_GenEvent is strongly recommended
  std::string IO_GenEvent_Key()          const { return m_io_genevent_start; }
  std::string IO_GenEvent_End()          const { return m_io_genevent_end; }
  // IO_Ascii is deprecated, but we want to be able to read these files
  std::string IO_Ascii_Key()             const { return m_io_ascii_start; }
  std::string IO_Ascii_End()             const { return m_io_ascii_end; }
  // IO_ExtendedAscii is deprecated, but we want to be able to read these files
  std::string IO_ExtendedAscii_Key()     const { return m_io_extendedascii_start; }
  std::string IO_ExtendedAscii_End()     const { return m_io_extendedascii_end; }
  
  // write keys
  void write_IO_GenEvent_Key( std::ostream& );
  void write_IO_GenEvent_End( std::ostream& );
  void write_IO_Ascii_Key( std::ostream& );
  void write_IO_Ascii_End( std::ostream& );
  void write_IO_ExtendedAscii_Key( std::ostream& );
  void write_IO_ExtendedAscii_End( std::ostream& );

  // methods to read input
  int find_file_type( std::istream& );

  int find_end_key( std::istream& );

  bool read_io_ascii_event( std::istream* is, GenEvent* evt );

  bool read_io_extendedascii_event( std::istream* is, GenEvent* evt );

  bool read_io_genevent_event( std::istream* is, GenEvent* evt );
  
protected:
  // methods used by the read_io* methods
    HeavyIon*    read_heavy_ion( std::istream* );
    PdfInfo*     read_pdf_info( std::istream* );
    GenParticle* read_particle( std::istream*, TempParticleMap&  );
    GenVertex*   read_vertex( std::istream*, TempParticleMap&  );

private:
  std::string m_io_genevent_start;
  std::string m_io_ascii_start;
  std::string m_io_extendedascii_start;
  std::string m_io_genevent_end;
  std::string m_io_ascii_end;
  std::string m_io_extendedascii_end;

};

// inline methods

inline CommonIO::CommonIO()
: m_io_genevent_start("HepMC::IO_GenEvent-START_EVENT_LISTING"),
  m_io_ascii_start("HepMC::IO_Ascii-START_EVENT_LISTING"),
  m_io_extendedascii_start("HepMC::IO_ExtendedAscii-START_EVENT_LISTING"),
  m_io_genevent_end("HepMC::IO_GenEvent-END_EVENT_LISTING"),
  m_io_ascii_end("HepMC::IO_Ascii-END_EVENT_LISTING"),
  m_io_extendedascii_end("HepMC::IO_ExtendedAscii-END_EVENT_LISTING")
{}

inline void CommonIO::write_IO_GenEvent_Key( std::ostream& os )
{ os << m_io_genevent_start << "\n"; }

inline void CommonIO::write_IO_GenEvent_End( std::ostream& os )
{ os << m_io_genevent_end << "\n"; }

inline void CommonIO::write_IO_Ascii_Key( std::ostream& os )
{ os << m_io_ascii_start << "\n"; }

inline void CommonIO::write_IO_Ascii_End( std::ostream& os )
{ os << m_io_ascii_end << "\n"; }

inline void CommonIO::write_IO_ExtendedAscii_Key( std::ostream& os )
{ os << m_io_extendedascii_start << "\n"; }

inline void CommonIO::write_IO_ExtendedAscii_End( std::ostream& os )
{ os << m_io_extendedascii_end << "\n"; }

}

#endif // HEPMC_COMMON_IO_H
