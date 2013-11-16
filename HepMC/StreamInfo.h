#ifndef HEPMC_STREAM_INFO_H
#define HEPMC_STREAM_INFO_H

#include "HepMC/Units.h"
#include <string>

namespace HepMC {

  /// Enum used to track which type of input is being read
  enum known_io { gen=1, ascii, extascii, ascii_pdt, extascii_pdt };


  /// Container for the extra information needed when using streaming IO to
  /// process HepMC GenEvents
  ///
  class StreamInfo {
  public:
    /// Default constructor
    StreamInfo( );

    /// @name IO_GenEvent handling
    //@{

    /// IO_GenEvent begin event block key
    std::string IO_GenEvent_Key()          const { return m_io_genevent_start; }
    /// IO_GenEvent end event block key
    std::string IO_GenEvent_End()          const { return m_io_genevent_end; }

    //@}

    /// @name IO_Ascii handling
    ///
    /// IO_Ascii has been removed, but we want to be able to read
    /// existing files written by IO_Ascii
    //@{

    /// IO_Ascii begin event block key
    std::string IO_Ascii_Key()             const { return m_io_ascii_start; }
    /// IO_Ascii end event block key
    std::string IO_Ascii_End()             const { return m_io_ascii_end; }
    /// IO_Ascii begin particle data block key
    std::string IO_Ascii_PDT_Key()         const { return m_io_ascii_pdt_start; }
    /// IO_Ascii end particle data block key
    std::string IO_Ascii_PDT_End()         const { return m_io_ascii_pdt_end; }

    //@}

    /// @name IO_ExtendedAscii handling
    ///
    /// IO_ExtendedAscii has been removed, but we want to be able to read
    /// existing files written by IO_Ascii
    //@{

    /// IO_ExtendedAscii begin event block key
    std::string IO_ExtendedAscii_Key()     const { return m_io_extendedascii_start; }
    /// IO_ExtendedAscii end event block key
    std::string IO_ExtendedAscii_End()     const { return m_io_extendedascii_end; }
    /// IO_ExtendedAscii begin particle data block key
    std::string IO_ExtendedAscii_PDT_Key() const { return m_io_extendedascii_pdt_start; }
    /// IO_ExtendedAscii end particle data block key
    std::string IO_ExtendedAscii_PDT_End() const { return m_io_extendedascii_pdt_end; }

    //@}

    /// Get IO type
    int io_type() const { return m_io_type; }
    /// Set IO type
    void set_io_type( int );

    /// True if the stream has a file type key
    ///
    /// has_key is true by default
    bool has_key() const { return m_has_key; }
    /// Set to false if the stream does not have a file type key
    void set_has_key( bool );

    /// Get the I/O momentum units
    Units::MomentumUnit io_momentum_unit() const { return m_io_momentum_unit; }
    /// Get the I/O length units
    Units::LengthUnit io_position_unit() const { return m_io_position_unit; }

    /// Get the I/O stream id
    ///
    /// This is used for sanity checking.
    int stream_id() const { return m_stream_id; }

    /// Special information is processed the first time we use the IO
    bool finished_first_event() const { return m_finished_first_event_io; }
    /// Special information is processed the first time we use the IO
    void set_finished_first_event( bool b ) { m_finished_first_event_io = b; }

    /// Specify units to use
    ///
    /// Needed when reading a file without units if those units are different
    /// than the declared default units (e.g. the default units are MeV, but
    /// the file was written with GeV). This method is not necessary if the units
    /// are written in the file.
    void use_input_units( Units::MomentumUnit, Units::LengthUnit );

    /// Return true when streaming input is processing the GenEvent header
    bool reading_event_header();
    /// Set the reading_event_header flag
    void set_reading_event_header(bool);

  private:

    /// @name Data members
    //@{
    bool        m_finished_first_event_io;
    // GenEvent I/O method keys
    std::string m_io_genevent_start;
    std::string m_io_ascii_start;
    std::string m_io_extendedascii_start;
    std::string m_io_genevent_end;
    std::string m_io_ascii_end;
    std::string m_io_extendedascii_end;
    // Particle data I/O method keys
    std::string m_io_ascii_pdt_start;
    std::string m_io_extendedascii_pdt_start;
    std::string m_io_ascii_pdt_end;
    std::string m_io_extendedascii_pdt_end;
    // I/O information
    int         m_io_type;
    bool        m_has_key;
    // Default I/O units - used only when reading a file with no units
    Units::MomentumUnit m_io_momentum_unit;
    Units::LengthUnit   m_io_position_unit;
    // Used to keep identify the I/O stream
    unsigned int m_stream_id;
    static unsigned int m_stream_counter;
    // Used to keep track when reading event
    bool m_reading_event_header;
    //@}

  };

} // HepMC

#endif  // HEPMC_STREAM_INFO_H
