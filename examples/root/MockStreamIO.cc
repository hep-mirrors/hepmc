//--------------------------------------------------------------------------
//
// MockStreamIO.cc
// Author:  Lynn Garren
//
// Implement operator >> and operator <<
//
// ----------------------------------------------------------------------

#include <iostream>
#include <ostream>
#include <istream>
#include <sstream>

#include "HepMC/GenEvent.h"
#include "HepMC/GenCrossSection.h"
#include "HepMC/StreamInfo.h"
#include "HepMC/StreamHelpers.h"
#include "HepMC/Version.h"
#include "HepMC/IO_Exception.h"

namespace HepMC {

std::ostream& GenEvent::writeMockRoot( std::ostream& os )
{
    /// Writes evt to an output stream.

    //
    StreamInfo & info = get_stream_info(os);
    //
    // if this is the first event, set precision
    if ( !info.finished_first_event() ) {
	// precision 16 (# digits following decimal point) is the minimum that
	//  will capture the full information stored in a double
        //  However, we let the user set precision, since that is the expected functionality
	// we use decimal to store integers, because it is smaller than hex!
	os.setf(std::ios::dec,std::ios::basefield);
	os.setf(std::ios::scientific,std::ios::floatfield);
	//
	info.set_finished_first_event(true);
    }
    //
    // output the event data including the number of primary vertices
    //  and the total number of vertices
    //std::vector<long> random_states = random_states();
    os << 'E';
    detail::output( os, event_number() );
    detail::output( os, mpi() );
    detail::output( os, event_scale() );
    detail::output( os, alphaQCD() );
    detail::output( os, alphaQED() );
    detail::output( os, signal_process_id() );
    detail::output( os,   ( signal_process_vertex() ?
		signal_process_vertex()->barcode() : 0 )   );
    detail::output( os, vertices_size() ); // total number of vertices.
    write_beam_particles( os, beam_particles() );
    // random state
    detail::output( os, (int)m_random_states.size() );
    for ( std::vector<long>::iterator rs = m_random_states.begin(); 
	  rs != m_random_states.end(); ++rs ) {
	 detail::output( os, *rs );
    }
    // weights
    // we need to iterate over the map so that the weights printed 
    // here will be in the same order as the names printed next
    os << ' ' << (int)weights().size() ;
    for ( WeightContainer::const_map_iterator w = weights().map_begin(); 
	  w != weights().map_end(); ++w ) {
        detail::output( os, m_weights[w->second] );
    }
    detail::output( os,'\n');
    // now add names for weights
    // note that this prints a new line if and only if the weight container
    // is not empty
    if ( ! weights().empty() ) {
	os << "N " << weights().size() << " " ;
	for ( WeightContainer::const_map_iterator w = weights().map_begin(); 
	      w != weights().map_end(); ++w ) {
	    detail::output( os,'"');
	    os << w->first;
	    detail::output( os,'"');
	    detail::output( os,' ');
	}
	detail::output( os,'\n');
    }
    //
    // Units
    os << "U " << name(momentum_unit());
    os << " " << name(length_unit());
    detail::output( os,'\n');
    //
    // write GenCrossSection if it has been set
    if( m_cross_section ) m_cross_section->write(os);
    //
    // write HeavyIon and PdfInfo if they have been set
    if( m_heavy_ion ) os << heavy_ion() ;
    if( m_pdf_info ) os << pdf_info() ;
    //
    // Output vertices
    write_vertex_list(os);
    // Output particles
    write_particle_list(os);
    return os;
}

std::istream& GenEvent::readMockRoot( std::istream& is )
{
    /// read a GenEvent from streaming input
    //
    StreamInfo & info = get_stream_info(is);
    clear();
    //
    // search for event listing key before first event only.
    if ( !info.finished_first_event() ) {
	//
	find_file_type(is);
	info.set_finished_first_event(true);
    }
    //
    // make sure the stream is good
    if ( !is ) {
	std::cerr << "streaming input: end of stream found "
		  << "setting badbit." << std::endl;
	is.clear(std::ios::badbit); 
        return is;
    }
    return is;
}

std::ostream & write_HepMC_MockRoot_block_begin(std::ostream & os )
{
    //
    StreamInfo & info = get_stream_info(os);

    if( !info.finished_first_event() ) {
    os << "\n" << "HepMC::Version " << versionName();
    os << "\n";
    os << info.IO_MockRoot_Key() << "\n";
    }
    return os;
}

std::ostream & write_HepMC_MockRoot_block_end(std::ostream & os )
{
    //
    StreamInfo & info = get_stream_info(os);

    if( info.finished_first_event() ) {
	os << info.IO_MockRoot_End() << "\n";
	os << std::flush;
    }
    return os;
}

} // HepMC
