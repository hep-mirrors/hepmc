//--------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, May 2011
// event input/output in ascii format for machine reading
// Format: event header, list of vertices, list of particles
//////////////////////////////////////////////////////////////////////////

#include "IO_MockRoot.h"
#include "HepMC/IO_Exception.h"
#include "HepMC/GenEvent.h"
#include "HepMC/StreamHelpers.h"
#include "HepMC/Version.h"

namespace HepMC {

    IO_MockRoot::IO_MockRoot( const std::string& filename, std::ios::openmode mode ) 
    : m_mode(mode), 
      m_file(filename.c_str(), mode), 
      m_ostr(0),
      m_istr(0),
      m_iostr(0),
      m_have_file(false),
      m_error_type(IO_Exception::OK),
      m_error_message(),
      m_io_mockroot_start("HepMC::IO_MockRoot-START_EVENT_LISTING"),
      m_io_mockroot_end("HepMC::IO_MockRoot-END_EVENT_LISTING")
    {
	if ( (m_mode&std::ios::out && m_mode&std::ios::in) ||
	     (m_mode&std::ios::app && m_mode&std::ios::in) ) {
            m_error_type = IO_Exception::InputAndOutput;
	    m_error_message ="IO_MockRoot::IO_MockRoot Error, open of file requested of input AND output type. Not allowed. Closing file.";
	    std::cerr << m_error_message << std::endl;
	    m_file.close();
	    return;
	}
	// now we set the streams
	m_iostr = &m_file;
	if ( m_mode&std::ios::in ) {
	    m_istr = &m_file;
	    m_ostr = NULL;
	    detail::establish_input_stream_info(m_file);
	}
	if ( m_mode&std::ios::out ) {
	    m_ostr = &m_file;
	    m_istr = NULL;
	    detail::establish_output_stream_info(m_file);
	}
	m_have_file = true;
    }


    IO_MockRoot::IO_MockRoot( std::istream & istr ) 
    : m_ostr(0),
      m_istr(&istr),
      m_iostr(&istr),
      m_have_file(false),
      m_error_type(IO_Exception::OK),
      m_error_message(),
      m_io_mockroot_start("HepMC::IO_MockRoot-START_EVENT_LISTING"),
      m_io_mockroot_end("HepMC::IO_MockRoot-END_EVENT_LISTING")
    { 
        detail::establish_input_stream_info( istr );
    }

    IO_MockRoot::IO_MockRoot( std::ostream & ostr )
    : m_ostr(&ostr),
      m_istr(0),
      m_iostr(&ostr),
      m_have_file(false),
      m_error_type(IO_Exception::OK),
      m_error_message(),
      m_io_mockroot_start("HepMC::IO_MockRoot-START_EVENT_LISTING"),
      m_io_mockroot_end("HepMC::IO_MockRoot-END_EVENT_LISTING")
   {
        detail::establish_output_stream_info( ostr );
   }

    IO_MockRoot::~IO_MockRoot() {
    	if ( m_ostr != NULL ) {
	    write_HepMC_MockRoot_block_end();
	}
	if(m_have_file) m_file.close();
    }

    void IO_MockRoot::use_input_units( Units::MomentumUnit mom, 
                                       Units::LengthUnit len ) {
        if( m_istr != NULL ) {
            set_input_units( *m_istr, mom, len );
	}
    }

    void IO_MockRoot::print( std::ostream& ostr ) const { 
	ostr << "IO_MockRoot: unformated ascii file IO for machine reading.\n"; 
	if(m_have_file)    ostr  << "\tFile openmode: " << m_mode ;
	ostr << " stream state: " << m_ostr->rdstate()
	     << " bad:" << (m_ostr->rdstate()&std::ios::badbit)
	     << " eof:" << (m_ostr->rdstate()&std::ios::eofbit)
	     << " fail:" << (m_ostr->rdstate()&std::ios::failbit)
	     << " good:" << (m_ostr->rdstate()&std::ios::goodbit) << std::endl;
    }

    void IO_MockRoot::precision( int size )  { 
        if( size > 16 ) { 
	    std::cerr << "IO_MockRoot::precision Error, "
		      << "precision is greater than 16. "
		      << "Not allowed. Using default precision of 16."
		      << std::endl;
            size = 16;
	}
        if(m_ostr) {
	    m_ostr->precision(size);
	}
    }
	
    bool IO_MockRoot::fill_next_event( GenEvent* evt ){
	//
	// reset error type
        m_error_type = IO_Exception::OK;
	//
	// test that evt pointer is not null
	if ( !evt ) {
            m_error_type = IO_Exception::NullEvent;
	    m_error_message = "IO_MockRoot::fill_next_event error - passed null event.";
	    std::cerr << m_error_message << std::endl;
	    return false;
	}
	// make sure the stream is good, and that it is in input mode
	if ( !(*m_istr) ) return false;
	if ( !m_istr ) {
            m_error_type = IO_Exception::WrongFileType;
	    m_error_message = "HepMC::IO_MockRoot::fill_next_event attempt to read from output file.";
	    std::cerr << m_error_message << std::endl;
	    return false;
	}
	// use streaming input
        try {
	    readMockRoot(evt);
	}
        catch (IO_Exception& e) {
            m_error_type = IO_Exception::InvalidData;
	    m_error_message = e.what();
	    evt->clear();
 	    return false;
        }
	if( evt->is_valid() ) return true;
	return false;
    }

    void IO_MockRoot::readMockRoot(GenEvent* evt)
    {}

    void IO_MockRoot::write_event( const GenEvent* evt ) {
	/// Writes evt to output stream. It does NOT delete the event after writing.
	//
	// make sure the state is good, and that it is in output mode
	if ( !evt  ) return;
	if ( m_ostr == NULL ) {
            m_error_type = IO_Exception::WrongFileType;
	    m_error_message = "HepMC::IO_MockRoot::write_event attempt to write to input file.";
	    std::cerr << m_error_message << std::endl;
	    return;
	}
	//
	// write event listing key before first event only.
	write_HepMC_MockRoot_block_begin();
	// write event
	// explicit cast is necessary
	GenEvent e = *evt;
	writeMockRoot(e);
    }

void IO_MockRoot::writeMockRoot(GenEvent& evt)
{
    //
    // output the event data including the number of primary vertices
    //  and the total number of vertices
    //std::vector<long> random_states = random_states();
    *m_ostr << 'E';
    detail::output( *m_ostr, evt.event_number() );
    detail::output( *m_ostr, evt.mpi() );
    detail::output( *m_ostr, evt.event_scale() );
    detail::output( *m_ostr, evt.alphaQCD() );
    detail::output( *m_ostr, evt.alphaQED() );
    detail::output( *m_ostr, evt.signal_process_id() );
    detail::output( *m_ostr,   ( evt.signal_process_vertex() ?
		evt.signal_process_vertex()->barcode() : 0 )   );
    detail::output( *m_ostr, evt.vertices_size() ); // total number of vertices.
    write_beam_particles( evt.beam_particles() );
    // random state
    std::vector<long> randomstates = evt.random_states();
    detail::output( *m_ostr, (int)randomstates.size() );
    for ( std::vector<long>::iterator rs = randomstates.begin(); 
	  rs != randomstates.end(); ++rs ) {
	 detail::output( *m_ostr, *rs );
    }
    // weights
    evt.weights().write_io(*m_ostr);
    //
    // Units
    *m_ostr << "U " << name(evt.momentum_unit());
    *m_ostr << " " << name(evt.length_unit());
    detail::output( *m_ostr,'\n');
    //
    // write GenCrossSection if it has been set
    if( evt.cross_section() ) evt.cross_section()->write(*m_ostr);
    //
    // write HeavyIon and PdfInfo if they have been set
    if( evt.heavy_ion() ) *m_ostr << evt.heavy_ion() ;
    if( evt.pdf_info() ) *m_ostr << evt.pdf_info() ;
    //
    // Output vertices
    write_vertex_list(evt);
    //
    // Output particles
    write_particle_list(evt);
}

void IO_MockRoot::write_beam_particles( std::pair<HepMC::GenParticle *,HepMC::GenParticle *> pr)
{
    GenParticle* p = pr.first;
    if(!p) {
       detail::output( *m_ostr, 0 );
    } else {
       detail::output( *m_ostr, p->barcode() );
    }
    p = pr.second;
    if(!p) {
       detail::output( *m_ostr, 0 );
    } else {
       detail::output( *m_ostr, p->barcode() );
    }
}

void IO_MockRoot::write_vertex_list(GenEvent& evt)
{
    for ( GenEvent::vertex_iterator v = evt.vertices_begin();
          v != evt.vertices_end(); ++v ) {
	//
	*m_ostr << 'V';
	detail::output( *m_ostr, (*v)->barcode() ); // v's unique identifier
	detail::output( *m_ostr, (*v)->id() );
	detail::output( *m_ostr, (*v)->position().x() );
	detail::output( *m_ostr, (*v)->position().y() );
	detail::output( *m_ostr, (*v)->position().z() );
	detail::output( *m_ostr, (*v)->position().t() );
	std::vector<size_t> pin = (*v)->particles_in_index();
	detail::output( *m_ostr, (int)pin.size() );
	for( std::vector<size_t>::const_iterator p1 = pin.begin(); p1 < pin.end(); ++p1 )
	{
	    detail::output( *m_ostr, (int)(*p1) );
	}
	std::vector<size_t> pout = (*v)->particles_out_index();
	detail::output( *m_ostr, (int)pout.size() );
	for( std::vector<size_t>::const_iterator p2 = pout.begin(); p2 < pout.end(); ++p2 )
	{
	    detail::output( *m_ostr, (int)(*p2) );
	}
	detail::output( *m_ostr, (int)(*v)->weights().size() );
	for ( WeightContainer::const_iterator w = (*v)->weights().begin(); 
	      w != (*v)->weights().end(); ++w ) {
	    detail::output( *m_ostr, *w );
	}
	detail::output( *m_ostr,'\n');
        
    }

}

void IO_MockRoot::write_particle_list(GenEvent& evt)
{
    for ( GenEvent::particle_iterator p = evt.particles_begin();
          p != evt.particles_end(); ++p ) {
	*m_ostr << 'P';
	detail::output( *m_ostr, (*p)->barcode() );
	detail::output( *m_ostr, (*p)->pdg_id() );
	detail::output( *m_ostr, (*p)->momentum().px() );
	detail::output( *m_ostr, (*p)->momentum().py() );
	detail::output( *m_ostr, (*p)->momentum().pz() );
	detail::output( *m_ostr, (*p)->momentum().e() );
	detail::output( *m_ostr, (*p)->generated_mass() );
	detail::output( *m_ostr, (*p)->status() );
	detail::output( *m_ostr, (*p)->polarization().theta() );
	detail::output( *m_ostr, (*p)->polarization().phi() );
	detail::output( *m_ostr,   ( (*p)->end_vertex() ? (*p)->end_vertex()->barcode() : 0 )  );
	*m_ostr << ' ' << (*p)->flow() << "\n";
    }

}

void IO_MockRoot::write_comment( const std::string comment ) {
    // make sure the stream is good, and that it is in output mode
    if ( !(*m_ostr) ) return;
    if ( m_ostr == NULL ) {
        m_error_type = IO_Exception::WrongFileType;
	m_error_message = "HepMC::IO_MockRoot::write_event attempt to write to input file.";
	std::cerr << m_error_message << std::endl;
	return;
    }
    // write end of event listing key if events have already been written
    write_HepMC_MockRoot_block_end();
    // insert the comment key before the comment
    *m_ostr << "\n" << "HepMC::IO_MockRoot-COMMENT\n";
    *m_ostr << comment << std::endl;
}
	
void IO_MockRoot::write_HepMC_MockRoot_block_begin( )
{
    // make sure the stream is good, and that it is in output mode
    if ( !(*m_ostr) ) return;
    if ( m_ostr == NULL ) {
        m_error_type = IO_Exception::WrongFileType;
	m_error_message = "HepMC::IO_MockRoot::write_HepMC_IO_block_begin attempt to write to input file.";
	std::cerr << m_error_message << std::endl;
	return;
    }

    *m_ostr << "\n" << "HepMC::Version " << versionName();
    *m_ostr << "\n";
    *m_ostr << m_io_mockroot_start << "\n";
}

void IO_MockRoot::write_HepMC_MockRoot_block_end( )
{
    // make sure the stream is good, and that it is in output mode
    if ( !(*m_ostr) ) return;
    if ( m_ostr == NULL ) {
        m_error_type = IO_Exception::WrongFileType;
	m_error_message = "HepMC::IO_MockRoot::write_HepMC_IO_block_end attempt to write to input file.";
	std::cerr << m_error_message << std::endl;
	return;
    }

    *m_ostr << m_io_mockroot_end << "\n";
    *m_ostr << std::flush;
}

} // HepMC
