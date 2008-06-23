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
#include "HepMC/ParticleDataTable.h"

namespace HepMC {

/// The known_io enum is used to track which type of input is being read
enum known_io { gen=1, ascii, extascii, ascii_pdt, extascii_pdt };

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
  /// get IO type
  int io_type() const { return m_io_type; }

  // write keys
  void write_IO_GenEvent_Key( std::ostream& );
  void write_IO_GenEvent_End( std::ostream& );
  // write keys for deprecated IO methods
  void write_IO_Ascii_Key( std::ostream& );
  void write_IO_Ascii_End( std::ostream& );
  void write_IO_ExtendedAscii_Key( std::ostream& );
  void write_IO_ExtendedAscii_End( std::ostream& );
  // write keys for deprecated particle data IO methods
  void write_IO_Ascii_PDT_Key( std::ostream& );
  void write_IO_Ascii_PDT_End( std::ostream& );
  void write_IO_ExtendedAscii_PDT_Key( std::ostream& );
  void write_IO_ExtendedAscii_PDT_End( std::ostream& );

  // methods to read input

  /// look for line type (key)
  int find_file_type( std::istream& );

  /// look for line type (key)
  int find_end_key( std::istream& );

  bool read_io_ascii( std::istream* is, GenEvent* evt );

  bool read_io_extendedascii( std::istream* is, GenEvent* evt );

  bool read_io_genevent( std::istream* is, GenEvent* evt );

  /// ParticleDataTable is deprecated.
  /// We include this method for reading old files which may have ParticleData information.
  bool read_io_particle_data_table( std::istream*, ParticleDataTable* );
 
protected:
  // methods used by the read_io* methods
  HeavyIon*    read_heavy_ion( std::istream* );
  PdfInfo*     read_pdf_info( std::istream* );
  GenParticle* read_particle( std::istream*, TempParticleMap&  );
  GenVertex*   read_vertex( std::istream*, TempParticleMap&  );
  /// ParticleDataTable is deprecated.
  /// We include this method for reading old files which may have ParticleData information.
  ParticleData* read_particle_data( std::istream*, ParticleDataTable* );
  // call this to read a double and check for invalid data (e.g., NaN)
  double read_double( std::istream* is );
  // called by read_double to find the end of the current event
  void find_event_end( std::istream* is );

private:
  std::string m_io_genevent_start;
  std::string m_io_ascii_start;
  std::string m_io_extendedascii_start;
  std::string m_io_genevent_end;
  std::string m_io_ascii_end;
  std::string m_io_extendedascii_end;
  // particle data method keys
  std::string m_io_ascii_pdt_start;
  std::string m_io_extendedascii_pdt_start;
  std::string m_io_ascii_pdt_end;
  std::string m_io_extendedascii_pdt_end;
  int         m_io_type;


};

// inline methods

inline CommonIO::CommonIO()
: m_io_genevent_start("HepMC::IO_GenEvent-START_EVENT_LISTING"),
  m_io_ascii_start("HepMC::IO_Ascii-START_EVENT_LISTING"),
  m_io_extendedascii_start("HepMC::IO_ExtendedAscii-START_EVENT_LISTING"),
  m_io_genevent_end("HepMC::IO_GenEvent-END_EVENT_LISTING"),
  m_io_ascii_end("HepMC::IO_Ascii-END_EVENT_LISTING"),
  m_io_extendedascii_end("HepMC::IO_ExtendedAscii-END_EVENT_LISTING"),
  m_io_ascii_pdt_start("HepMC::IO_Ascii-START_PARTICLE_DATA"),
  m_io_extendedascii_pdt_start("HepMC::IO_ExtendedAscii-START_PARTICLE_DATA"),
  m_io_ascii_pdt_end("HepMC::IO_Ascii-END_PARTICLE_DATA"),
  m_io_extendedascii_pdt_end("HepMC::IO_ExtendedAscii-END_PARTICLE_DATA"),
  m_io_type(0)
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

inline void CommonIO::write_IO_Ascii_PDT_Key( std::ostream& os )
{ os << m_io_ascii_pdt_start << "\n"; }

inline void CommonIO::write_IO_Ascii_PDT_End( std::ostream& os )
{ os << m_io_ascii_pdt_end << "\n"; }

inline void CommonIO::write_IO_ExtendedAscii_PDT_Key( std::ostream& os )
{ os << m_io_extendedascii_pdt_start << "\n"; }

inline void CommonIO::write_IO_ExtendedAscii_PDT_End( std::ostream& os )
{ os << m_io_extendedascii_pdt_end << "\n"; }


}

#endif // HEPMC_COMMON_IO_H
