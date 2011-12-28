//--------------------------------------------------------------------------
//
// MockStreamInfo.cc
// Author:  Lynn Garren
//
// ----------------------------------------------------------------------

#include <string>
#include "HepMC/MockStreamInfo.h"

namespace HepMC {


MockStreamInfo::MockStreamInfo( )
: m_finished_first_event_io(false),
  m_io_genevent_start("HepMC::IO_GenEvent-START_EVENT_LISTING"),
  m_io_mockroot_start("HepMC::IO_MockRoot-START_EVENT_LISTING"),
  m_io_ascii_start("HepMC::IO_Ascii-START_EVENT_LISTING"),
  m_io_extendedascii_start("HepMC::IO_ExtendedAscii-START_EVENT_LISTING"),
  m_io_genevent_end("HepMC::IO_GenEvent-END_EVENT_LISTING"),
  m_io_mockroot_end("HepMC::IO_MockRoot-END_EVENT_LISTING"),
  m_io_ascii_end("HepMC::IO_Ascii-END_EVENT_LISTING"),
  m_io_extendedascii_end("HepMC::IO_ExtendedAscii-END_EVENT_LISTING"),
  m_io_ascii_pdt_start("HepMC::IO_Ascii-START_PARTICLE_DATA"),
  m_io_extendedascii_pdt_start("HepMC::IO_ExtendedAscii-START_PARTICLE_DATA"),
  m_io_ascii_pdt_end("HepMC::IO_Ascii-END_PARTICLE_DATA"),
  m_io_extendedascii_pdt_end("HepMC::IO_ExtendedAscii-END_PARTICLE_DATA"),
  m_io_type(0),
  m_has_key(true),
  m_io_momentum_unit(Units::default_momentum_unit()),
  m_io_position_unit(Units::default_length_unit()),
  m_stream_id(m_stream_counter),
  m_reading_event_header(false)
{
    ++m_stream_counter;
}

/// static counter 
unsigned int MockStreamInfo::m_stream_counter = 0; 

void MockStreamInfo::use_input_units( Units::MomentumUnit mom, Units::LengthUnit len ) {
    m_io_momentum_unit = mom;
    m_io_position_unit = len;
}

void MockStreamInfo::set_io_type( int io ) {
    m_io_type = io;
}

void MockStreamInfo::set_has_key( bool io ) {
    m_has_key = io;
}

bool MockStreamInfo::reading_event_header() {
    return m_reading_event_header;
}

void MockStreamInfo::set_reading_event_header(bool tf) {
    m_reading_event_header = tf;
}

} // HepMC
