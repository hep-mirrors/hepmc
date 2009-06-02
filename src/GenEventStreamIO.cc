//--------------------------------------------------------------------------
//
// GenEventStreamIO.cc
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

// ------------------------- local methods ----------------

/// This method is called by the stream destructor.
/// It does cleanup on stored user data (StreamInfo)
/// and is registered by the first call to get_stream_info().
void HepMCStreamCallback(std::ios_base::event e, std::ios_base& b, int i)
{
  // only clean up if the stream object is going away.
  if(i!=0 && e!= std::ios_base::erase_event) return;

  // retrieve the pointer to the object
  StreamInfo* hd = (StreamInfo*)b.pword(i);
  b.pword(i) = 0;
  b.iword(i) = 0;
#ifdef HEPMC_DEBUG
  // the following line is just for sanity checking
  if(hd) std::cerr << "deleted StreamInfo " << hd->stream_id() << "\n";
#endif
  delete hd;
}

// ------------------------- iomanip ----------------

/// A custom iomanip that allows us to store and access user data (StreamInfo)
/// associated with the stream.
/// This method creates the StreamInfo object the first time it is called.
template <class IO>
StreamInfo& get_stream_info(IO& iost)
{
  if(iost.iword(0) == 0)
    {
      // make sure we add the callback if this is the first time through
      iost.iword(0)=1;
      iost.register_callback(&HepMCStreamCallback, 0);
      // this is our special "context" record.
      // there is one of these at the head of each IO block.
      // allocate room for a StreamInfo in the userdata area
      iost.pword(0) = new StreamInfo;
#ifdef HEPMC_DEBUG
      // the following line is just for sanity checking
      std::cerr << "created StreamInfo " << ((StreamInfo*)iost.pword(0))->stream_id() << "\n";
#endif
    }
  return *(StreamInfo*)iost.pword(0);
}
	
// ------------------------- GenEvent member functions ----------------

std::ostream& GenEvent::write( std::ostream& os )
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
    os << ' ' << (int)weights().size() ;
    for ( WeightContainer::const_iterator w = weights().begin(); 
	  w != weights().end(); ++w ) {
	 detail::output( os, *w );
    }
    detail::output( os,'\n');
    //
    // Units
    os << "U " << name(momentum_unit());
    os << " " << name(length_unit());
    detail::output( os,'\n');
    //
    // write GenCrossSection if it has been set
    if( m_cross_section ) m_cross_section->write(os);
    //
    os << heavy_ion() ;
    os << pdf_info() ;
    //
    // Output all of the vertices - note there is no real order.
    for ( GenEvent::vertex_const_iterator v = vertices_begin();
	  v != vertices_end(); ++v ) {
	write_vertex(os, *v);
    }
    return os;
}

std::istream& GenEvent::read( std::istream& is )
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

    //
    // test to be sure the next entry is of type "E" then ignore it
    if ( is.peek()!='E' ) { 
	// if the E is not the next entry, then check to see if it is
	// the end event listing key - if yes, search for another start key
	int ioendtype;
	find_end_key(is,ioendtype);
	if ( ioendtype == info.io_type() ) {
	    find_file_type(is);
	    // are we at the end of the file?
	    if( !is ) return is;
	} else if ( ioendtype > 0 ) {
	    std::cerr << "streaming input: end key does not match start key "
		      << "setting badbit." << std::endl;
	    is.clear(std::ios::badbit); 
	    return is;
	} else if ( !info.has_key() ) {
	    find_file_type(is);
	    // are we at the end of the file?
	    if( !is ) return is;
	} else {
	    std::cerr << "streaming input: end key not found "
		      << "setting badbit." << std::endl;
	    is.clear(std::ios::badbit); 
	    return is;
	}
    } 
    // get the event line
    std::string line;
    std::getline(is,line);
    std::istringstream iline(line);
    std::string firstc;
    iline >> firstc;
    //
    // read values into temp variables, then fill GenEvent
    int event_number = 0, signal_process_id = 0, signal_process_vertex = 0,
	num_vertices = 0, random_states_size = 0, weights_size = 0,
	nmpi = -1, bp1 = 0, bp2 = 0;
    double eventScale = 0, alpha_qcd = 0, alpha_qed = 0;
    iline >> event_number;
    if(!iline) detail::find_event_end( is );
    if( info.io_type() == gen || info.io_type() == extascii ) {
        iline >> nmpi;
        if(!iline) detail::find_event_end( is );
        set_mpi( nmpi );
    }
    iline >> eventScale ;
    if(!iline) detail::find_event_end( is );
    iline >> alpha_qcd ;
    if(!iline) detail::find_event_end( is );
    iline >> alpha_qed;
    if(!iline) detail::find_event_end( is );
    iline >> signal_process_id ;
    if(!iline) detail::find_event_end( is );
    iline >> signal_process_vertex;
    if(!iline) detail::find_event_end( is );
    iline >> num_vertices;
    if(!iline) detail::find_event_end( is );
    if( info.io_type() == gen || info.io_type() == extascii ) {
        iline >> bp1 ;
        if(!iline) detail::find_event_end( is );
	iline >> bp2;
        if(!iline) detail::find_event_end( is );
    }
    iline >> random_states_size;
    if(!iline) detail::find_event_end( is );
    std::vector<long> random_states(random_states_size);
    for ( int i = 0; i < random_states_size; ++i ) {
	iline >> random_states[i];
        if(!iline) detail::find_event_end( is );
    }
    iline >> weights_size;
    if(!iline) detail::find_event_end( is );
    WeightContainer weights(weights_size);
    for ( int ii = 0; ii < weights_size; ++ii ) {
        iline >> weights[ii];
        if(!iline) detail::find_event_end( is );
    }
    // 
    // fill signal_process_id, event_number, weights, random_states, etc.
    set_signal_process_id( signal_process_id );
    set_event_number( event_number );
    m_weights = weights;
    set_random_states( random_states );
    set_event_scale( eventScale );
    set_alphaQCD( alpha_qcd );
    set_alphaQED( alpha_qed );
    if( info.io_type() == gen ) {
	// get unit information if it exists
	read_units( is );
    }
    // check to see if we have a GenCrossSection line
    if ( is.peek()=='C' ) { 
	// create cross section
	GenCrossSection* xs = new GenCrossSection();
	// check for invalid data
	try {
	    // read the line
	    xs->read(is);
	}
	catch (IO_Exception& e) {
            detail::find_event_end( is );
	}
	if(xs->is_set()) { 
	    set_cross_section( *xs );
	} else { 
	    delete xs; 
	    xs = 0;
	}
    }
    if( info.io_type() == gen || info.io_type() == extascii ) {
	// get HeavyIon and PdfInfo
	HeavyIon* ion = new HeavyIon();
	// check for invalid data
	try {
	    is >> ion;
	}
	catch (IO_Exception& e) {
            detail::find_event_end( is );
	}
	if(ion->is_valid()) { 
	    set_heavy_ion( *ion );
	} else { 
	    delete ion; 
	    ion = 0;
	}
	PdfInfo* pdf = new PdfInfo();
	// check for invalid data
	try {
	    is >> pdf;
	}
	catch (IO_Exception& e) {
            detail::find_event_end( is );
	}
	if(pdf->is_valid()) { 
	    set_pdf_info( *pdf );
	} else { 
	    delete pdf; 
	    pdf = 0;
	}
    }
    //
    // the end vertices of the particles are not connected until
    //  after the event is read --- we store the values in a map until then
    TempParticleMap particle_to_end_vertex;
    //
    // read in the vertices
    for ( int iii = 1; iii <= num_vertices; ++iii ) {
	GenVertex* v = new GenVertex();
	detail::read_vertex(is,particle_to_end_vertex,v);
	add_vertex( v );
    }
    // set the signal process vertex
    if ( signal_process_vertex ) {
	set_signal_process_vertex( 
	    barcode_to_vertex(signal_process_vertex) );
    }
    //
    // last connect particles to their end vertices
    GenParticle* beam1(0);
    GenParticle* beam2(0);
    for ( std::map<int,GenParticle*>::iterator pmap 
	      = particle_to_end_vertex.order_begin(); 
	  pmap != particle_to_end_vertex.order_end(); ++pmap ) {
	GenParticle* p =  pmap->second;
	int vtx = particle_to_end_vertex.end_vertex( p );
	GenVertex* itsDecayVtx = barcode_to_vertex(vtx);
	if ( itsDecayVtx ) itsDecayVtx->add_particle_in( p );
	else {
	    std::cerr << "read_io_genevent: ERROR particle points"
		      << " to null end vertex. " <<std::endl;
	}
	// also look for the beam particles
	if( p->barcode() == bp1 ) beam1 = p;
	if( p->barcode() == bp2 ) beam2 = p;
    }
    set_beam_particles(beam1,beam2);
    return is;
}

// ------------------------- operator << and operator >> ----------------

std::ostream & operator << (std::ostream & os, GenEvent & evt)
{
    /// Writes evt to an output stream.
    evt.write(os);
    return os;
}

std::istream & operator >> (std::istream & is, GenEvent & evt)
{
    evt.read(is);
    return is;
}

// ------------------------- set units ----------------

std::istream & set_input_units(std::istream & is, 
                               Units::MomentumUnit mom,
			       Units::LengthUnit len )
{
    //
    StreamInfo & info = get_stream_info(is);
    info.use_input_units( mom, len );
    return is;
}

// ------------------------- begin and end block lines ----------------

std::ostream & write_HepMC_IO_block_begin(std::ostream & os )
{
    //
    StreamInfo & info = get_stream_info(os);

    if( !info.finished_first_event() ) {
    os << "\n" << "HepMC::Version " << versionName();
    os << "\n";
    os << info.IO_GenEvent_Key() << "\n";
    }
    return os;
}

std::ostream & write_HepMC_IO_block_end(std::ostream & os )
{
    //
    StreamInfo & info = get_stream_info(os);

    if( info.finished_first_event() ) {
	os << info.IO_GenEvent_End() << "\n";
	os << std::flush;
    }
    return os;
}

std::istream & GenEvent::read_units( std::istream & is )
{
    //
    if ( !is ) {
	std::cerr << "StreamHelpers read_units setting badbit." << std::endl;
	is.clear(std::ios::badbit);
	return is;
    } 
    //
    StreamInfo & info = get_stream_info(is);
    // test to be sure the next entry is of type "U" then ignore it
    // if we have no units, this is not an error
    // releases prior to 2.04.00 did not write unit information
    if ( is.peek() !='U') {
 	use_units( info.io_momentum_unit(), 
	               info.io_position_unit() );
	return is;
    } 
    is.ignore();	// ignore the first character in the line
    std::string mom, pos;
    is >> mom >> pos;
    is.ignore(1);      // eat the extra whitespace
    use_units(mom,pos);
    //
    return is;
}

std::istream & GenEvent::find_file_type( std::istream & istr )
{
    //
    // make sure the stream is good
    if ( !istr ) return istr;

    //
    StreamInfo & info = get_stream_info(istr);

    // if there is no input block line, then we assume this stream
    // is in the IO_GenEvent format
    if ( istr.peek()=='E' ) {
	info.set_io_type( gen );
	info.set_has_key(false);
        return istr;
    }
    
    std::string line;
    while ( std::getline(istr,line) ) {
	//
	// search for event listing key before first event only.
	//
	if( line == info.IO_GenEvent_Key() ) {
	    info.set_io_type( gen );
	    info.set_has_key(true);
	    return istr;
	} else if( line == info.IO_Ascii_Key() ) {
	    info.set_io_type( ascii );
	    info.set_has_key(true);
	    return istr;
	} else if( line == info.IO_ExtendedAscii_Key() ) {
	    info.set_io_type( extascii );
	    info.set_has_key(true);
	    return istr;
	} else if( line == info.IO_Ascii_PDT_Key() ) {
	    info.set_io_type( ascii_pdt );
	    info.set_has_key(true);
	    return istr;
	} else if( line == info.IO_ExtendedAscii_PDT_Key() ) {
	    info.set_io_type( extascii_pdt );
	    info.set_has_key(true);
	    return istr;
	}
    }
    info.set_io_type( 0 );
    info.set_has_key(false);
    return istr;
}

std::istream & GenEvent::find_end_key( std::istream & istr, int & iotype )
{
    iotype = 0;
    // peek at the first character before proceeding
    if( istr.peek()!='H' ) return istr;
    //
    // we only check the next line
    std::string line;
    std::getline(istr,line);
    //
    StreamInfo & info = get_stream_info(istr);
    //
    // check to see if this is an end key
    if( line == info.IO_GenEvent_End() ) {
	iotype = gen;
    } else if( line == info.IO_Ascii_End() ) {
	iotype = ascii;
    } else if( line == info.IO_ExtendedAscii_End() ) {
	iotype = extascii;
    } else if( line == info.IO_Ascii_PDT_End() ) {
	iotype = ascii_pdt;
    } else if( line == info.IO_ExtendedAscii_PDT_End() ) {
	iotype = extascii_pdt;
    }
    if( iotype != 0 && info.io_type() != iotype ) {
        std::cerr << "GenEvent::find_end_key: iotype keys have changed" << std::endl;
    } else {
        return istr;
    }
    //
    // if we get here, then something has gotten badly confused
    std::cerr << "GenEvent::find_end_key: MALFORMED INPUT" << std::endl;
    istr.clear(std::ios::badbit); 
    return istr;
}

std::ostream & establish_output_stream_info( std::ostream & os )
{
    StreamInfo & info = get_stream_info(os);
    if ( !info.finished_first_event() ) {
	// precision 16 (# digits following decimal point) is the minimum that
	//  will capture the full information stored in a double
	os.precision(16);
	// we use decimal to store integers, because it is smaller than hex!
	os.setf(std::ios::dec,std::ios::basefield);
	os.setf(std::ios::scientific,std::ios::floatfield);
    }
    return os;
}

std::istream & establish_input_stream_info( std::istream & is )
{
    StreamInfo & info = get_stream_info(is);
    if ( !info.finished_first_event() ) {
	// precision 16 (# digits following decimal point) is the minimum that
	//  will capture the full information stored in a double
	is.precision(16);
	// we use decimal to store integers, because it is smaller than hex!
	is.setf(std::ios::dec,std::ios::basefield);
	is.setf(std::ios::scientific,std::ios::floatfield);
    }
    return is;
}


// ------------------------- helper functions ----------------

namespace detail {

// The functions defined here need to use get_stream_info

std::istream & read_particle( std::istream & is, 
                              TempParticleMap & particle_to_end_vertex, 
			      GenParticle * p )
{
    // get the next line
    std::string line;
    std::getline(is,line);
    std::istringstream iline(line);
    std::string firstc;
    iline >> firstc;
    if( firstc != "P" ) { 
	std::cerr << "StreamHelpers::detail::read_particle invalid line type: " 
	          << firstc << std::endl;
	std::cerr << "StreamHelpers::detail::read_particle setting badbit." 
		  << std::endl;
	is.clear(std::ios::badbit); 
	return is;
    } 
    //
    StreamInfo & info = get_stream_info(is);
    //
    // declare variables to be read in to, and read everything except flow
    double px = 0., py = 0., pz = 0., e = 0., m = 0., theta = 0., phi = 0.;
    int bar_code = 0, id = 0, status = 0, end_vtx_code = 0, flow_size = 0;
    // check that the input stream is still OK after reading item
    iline >> bar_code ;
    if(!iline) detail::find_event_end( is );
    iline >> id ;
    if(!iline) detail::find_event_end( is );
    iline >> px ;
    if(!iline) detail::find_event_end( is );
    iline >> py ;
    if(!iline) detail::find_event_end( is );
    iline >> pz ;
    if(!iline) detail::find_event_end( is );
    iline >> e ;
    if(!iline) detail::find_event_end( is );
    if( info.io_type() != ascii ) {
	iline >> m ;
        if(!iline) detail::find_event_end( is );
    }
    iline >> status ;
    if(!iline) detail::find_event_end( is );
    iline >> theta ;
    if(!iline) detail::find_event_end( is );
    iline >> phi ;
    if(!iline) detail::find_event_end( is );
    iline >> end_vtx_code ;
    if(!iline) detail::find_event_end( is );
    iline >> flow_size;
    if(!iline) detail::find_event_end( is );
    //
    // read flow patterns if any exist
    Flow flow;
    int code_index, code;
    for ( int i = 1; i <= flow_size; ++i ) {
	iline >> code_index >> code;
        if(!iline) detail::find_event_end( is );
	flow.set_icode( code_index,code);
    }
    p->set_momentum( FourVector(px,py,pz,e) );
    p->set_pdg_id( id );
    p->set_status( status );
    p->set_flow( flow );
    p->set_polarization( Polarization(theta,phi) );
    if( info.io_type() == ascii ) {
        p->set_generated_mass( p->momentum().m() );
    } else {
        p->set_generated_mass( m );
    }
    p->suggest_barcode( bar_code );
    //
    // all particles are connected to their end vertex separately 
    // after all particles and vertices have been created - so we keep
    // a map of all particles that have end vertices
    if ( end_vtx_code != 0 ) {
	particle_to_end_vertex.addEndParticle(p,end_vtx_code);
    }
    return is;
}

std::istream & read_units( std::istream & is, GenEvent & evt )
{
    //
    if ( !is ) {
	std::cerr << "StreamHelpers read_units setting badbit." << std::endl;
	is.clear(std::ios::badbit);
	return is;
    } 
    //
    StreamInfo & info = get_stream_info(is);
    // test to be sure the next entry is of type "U" then ignore it
    // if we have no units, this is not an error
    // releases prior to 2.04.00 did not write unit information
    if ( is.peek() !='U') {
 	evt.use_units( info.io_momentum_unit(), 
	               info.io_position_unit() );
	return is;
    } 
    is.ignore();	// ignore the first character in the line
    std::string mom, pos;
    is >> mom >> pos;
    is.ignore(1);      // eat the extra whitespace
    evt.use_units(mom,pos);
    //
    return is;
}

std::ostream & establish_output_stream_info( std::ostream & os )
{
    StreamInfo & info = get_stream_info(os);
    if ( !info.finished_first_event() ) {
	// precision 16 (# digits following decimal point) is the minimum that
	//  will capture the full information stored in a double
	os.precision(16);
	// we use decimal to store integers, because it is smaller than hex!
	os.setf(std::ios::dec,std::ios::basefield);
	os.setf(std::ios::scientific,std::ios::floatfield);
    }
    return os;
}

std::istream & establish_input_stream_info( std::istream & is )
{
    StreamInfo & info = get_stream_info(is);
    if ( !info.finished_first_event() ) {
	// precision 16 (# digits following decimal point) is the minimum that
	//  will capture the full information stored in a double
	is.precision(16);
	// we use decimal to store integers, because it is smaller than hex!
	is.setf(std::ios::dec,std::ios::basefield);
	is.setf(std::ios::scientific,std::ios::floatfield);
    }
    return is;
}

} // detail

} // HepMC
