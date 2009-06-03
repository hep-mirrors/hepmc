//--------------------------------------------------------------------------
//
// CommonIO.cc
// Author:  Lynn Garren
//
// ----------------------------------------------------------------------

#include <string>
#include <sstream>

#include "HepMC/CommonIO.h"
#include "HepMC/GenEvent.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/TempParticleMap.h"
#include "HepMC/ParticleDataTable.h"

namespace HepMC {

int CommonIO::find_file_type( std::istream& istr )
{
    std::string line;
    while ( std::getline(istr,line) ) {
	//
	// search for event listing key before first event only.
	//
	if( line == m_io_genevent_start ) {
	    m_io_type = gen;
	    return gen;
	} else if( line == m_io_ascii_start ) {
	    m_io_type = ascii;
	    return ascii;
	} else if( line == m_io_extendedascii_start ) {
	    m_io_type = extascii;
	    return extascii;
	} else if( line == m_io_ascii_pdt_start ) {
	    m_io_type = ascii_pdt;
	    return ascii_pdt;
	} else if( line == m_io_extendedascii_pdt_start ) {
	    m_io_type = extascii_pdt;
	    return extascii_pdt;
	}
    }
    return -1;
}

int CommonIO::find_end_key( std::istream& istr )
{
    // peek at the first character before proceeding
    if( istr.peek()!='H' ) return false;
    //
    // we only check the next line
    std::string line;
    std::getline(istr,line);
    //
    // check to see if this is an end key
    int iotype = 0;
    if( line == m_io_genevent_end ) {
	m_io_type = gen;
    } else if( line == m_io_ascii_end ) {
	m_io_type = ascii;
    } else if( line == m_io_extendedascii_end ) {
	m_io_type = extascii;
    } else if( line == m_io_ascii_pdt_end ) {
	m_io_type = ascii_pdt;
    } else if( line == m_io_extendedascii_pdt_end ) {
	m_io_type = extascii_pdt;
    }
    if( iotype != 0 && m_io_type != iotype ) {
        std::cerr << "CommonIO::find_end_key: iotype keys have changed" << std::endl;
    } else {
        return iotype;
    }
    //
    // if we get here, then something has gotten badly confused
    std::cerr << "CommonIO::find_end_key: MALFORMED INPUT" << std::endl;
    return -1;
}

void CommonIO::use_input_units( Units::MomentumUnit mom, Units::LengthUnit len ) 
{
     m_io_momentum_unit = mom;
     m_io_position_unit = len;
}

bool CommonIO::read_io_ascii( std::istream* istr, GenEvent* evt )
{
	// read values into temp variables, then create a new GenEvent
	int event_number = 0, signal_process_id = 0, signal_process_vertex = 0,
	    num_vertices = 0, random_states_size = 0, weights_size = 0;
	double eventScale = 0, alpha_qcd = 0, alpha_qed = 0;
	*istr >> event_number >> eventScale >> alpha_qcd >> alpha_qed
	       >> signal_process_id >> signal_process_vertex
	       >> num_vertices >> random_states_size;
	std::vector<long> random_states(random_states_size);
	for ( int i = 0; i < random_states_size; ++i ) {
	    *istr >> random_states[i];
	}
	*istr >> weights_size;
	WeightContainer weights(weights_size);
	for ( int ii = 0; ii < weights_size; ++ii ) *istr >> weights[ii];
	istr->ignore(2,'\n');
	// 
	// fill signal_process_id, event_number, weights, random_states
	evt->set_signal_process_id( signal_process_id );
	evt->set_event_number( event_number );
	evt->weights() = weights;
	evt->set_random_states( random_states );
	// no unit information, so just set it
	// this is done before the event has any vertices, so it is safe
 	evt->use_units( m_io_momentum_unit, m_io_position_unit );
	//
	// the end vertices of the particles are not connected until
	//  after the event is read --- we store the values in a map until then
       	TempParticleMap particle_to_end_vertex;
	//
	// read in the vertices
	for ( int iii = 1; iii <= num_vertices; ++iii ) {
	    GenVertex* v = read_vertex(istr,particle_to_end_vertex);
	    evt->add_vertex( v );
	}
	// set the signal process vertex
	if ( signal_process_vertex ) {
	    evt->set_signal_process_vertex( 
		evt->barcode_to_vertex(signal_process_vertex) );
	}
	//
	// last connect particles to their end vertices
	for ( std::map<int,GenParticle*>::iterator pmap 
		  = particle_to_end_vertex.order_begin(); 
	      pmap != particle_to_end_vertex.order_end(); ++pmap ) {
	    GenParticle* p =  pmap->second;
	    int vtx = particle_to_end_vertex.end_vertex( p );
	    GenVertex* itsDecayVtx = evt->barcode_to_vertex(vtx);
	    if ( itsDecayVtx ) itsDecayVtx->add_particle_in( p );
	    else {
		std::cerr << "IO_Ascii::fill_next_event ERROR particle points"
			  << "\n to null end vertex. " <<std::endl;
	    }
	}
	return true;
}

bool CommonIO::read_io_extendedascii( std::istream* istr, GenEvent* evt )
{
	// read values into temp variables, then create a new GenEvent
	int event_number = 0, signal_process_id = 0, signal_process_vertex = 0,
	    num_vertices = 0, random_states_size = 0, weights_size = 0,
	    nmpi = 0, bp1 = 0, bp2 = 0;
	double eventScale = 0, alpha_qcd = 0, alpha_qed = 0;
	*istr >> event_number >> nmpi >> eventScale >> alpha_qcd >> alpha_qed
	       >> signal_process_id >> signal_process_vertex
	       >> num_vertices >> bp1 >> bp2 >> random_states_size;
	std::vector<long> random_states(random_states_size);
	for ( int i = 0; i < random_states_size; ++i ) {
	    *istr >> random_states[i];
	}
	*istr >> weights_size;
	WeightContainer weights(weights_size);
	for ( int ii = 0; ii < weights_size; ++ii ) *istr >> weights[ii];
	istr->ignore(2,'\n');
	// 
	// fill signal_process_id, event_number, weights, random_states
	evt->set_signal_process_id( signal_process_id );
	evt->set_event_number( event_number );
	evt->set_mpi( nmpi );
	evt->weights() = weights;
	evt->set_random_states( random_states );
	evt->set_event_scale( eventScale );
	evt->set_alphaQCD( alpha_qcd );
	evt->set_alphaQED( alpha_qed );
	// get HeavyIon and PdfInfo
	HeavyIon* ion = read_heavy_ion(istr);
	if(ion) evt->set_heavy_ion( *ion );
	PdfInfo* pdf = read_pdf_info(istr);
	if(pdf) evt->set_pdf_info( *pdf );
	// no unit information, so just set it
	// this is done before the event has any vertices, so it is safe
 	evt->use_units( m_io_momentum_unit, m_io_position_unit );
	//
	// the end vertices of the particles are not connected until
	//  after the event is read --- we store the values in a map until then
       	TempParticleMap particle_to_end_vertex;
	//
	// read in the vertices
	for ( int iii = 1; iii <= num_vertices; ++iii ) {
	    GenVertex* v = read_vertex(istr,particle_to_end_vertex);
	    evt->add_vertex( v );
	}
	// set the signal process vertex
	if ( signal_process_vertex ) {
	    evt->set_signal_process_vertex( 
		evt->barcode_to_vertex(signal_process_vertex) );
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
	    GenVertex* itsDecayVtx = evt->barcode_to_vertex(vtx);
	    if ( itsDecayVtx ) itsDecayVtx->add_particle_in( p );
	    else {
		std::cerr << "read_io_extendedascii: ERROR particle points"
			  << "\n to null end vertex. " <<std::endl;
	    }
	    // also look for the beam particles
	    if( p->barcode() == bp1 ) beam1 = p;
	    if( p->barcode() == bp2 ) beam2 = p;
	}
	evt->set_beam_particles(beam1,beam2);
	return true;
}

bool CommonIO::read_io_genevent( std::istream* is, GenEvent* evt )
{
    /// this method ONLY works if called from fill_next_event
    //
    // assume that we've been handed a valid stream and event pointer
    //
    // read values into temp variables, then fill GenEvent
    int event_number = 0, signal_process_id = 0, signal_process_vertex = 0,
	num_vertices = 0, random_states_size = 0, weights_size = 0,
	nmpi = 0, bp1 = 0, bp2 = 0;
    double eventScale = 0, alpha_qcd = 0, alpha_qed = 0;
    *is >> event_number >> nmpi >> eventScale >> alpha_qcd >> alpha_qed
	   >> signal_process_id >> signal_process_vertex
	   >> num_vertices >> bp1 >> bp2 >> random_states_size;
    std::vector<long> random_states(random_states_size);
    for ( int i = 0; i < random_states_size; ++i ) {
	*is >> random_states[i];
    }
    *is >> weights_size;
    WeightContainer weights(weights_size);
    for ( int ii = 0; ii < weights_size; ++ii ) *is >> weights[ii];
    is->ignore(2,'\n');
    // 
    // fill signal_process_id, event_number, weights, random_states, etc.
    evt->set_signal_process_id( signal_process_id );
    evt->set_event_number( event_number );
    evt->set_mpi( nmpi );
    evt->weights() = weights;
    evt->set_random_states( random_states );
    evt->set_event_scale( eventScale );
    evt->set_alphaQCD( alpha_qcd );
    evt->set_alphaQED( alpha_qed );
    // get unit information if it exists
    read_units( is, evt );
    // check to see if we have a GenCrossSection line
    // ignore this line if it exists
    // GenCrossSection is available in HepMC 2.05
    if ( is->peek()=='C' ) { 
        std::string line;
	getline( *is, line);
    }
    // get HeavyIon and PdfInfo
    HeavyIon* ion = read_heavy_ion(is);
    if(ion) evt->set_heavy_ion( *ion );
    PdfInfo* pdf = read_pdf_info(is);
    if(pdf) evt->set_pdf_info( *pdf );
    //
    // the end vertices of the particles are not connected until
    //  after the event is read --- we store the values in a map until then
    TempParticleMap particle_to_end_vertex;
    //
    // read in the vertices
    for ( int iii = 1; iii <= num_vertices; ++iii ) {
	GenVertex* v = read_vertex(is,particle_to_end_vertex);
	evt->add_vertex( v );
    }
    // set the signal process vertex
    if ( signal_process_vertex ) {
	evt->set_signal_process_vertex( 
	    evt->barcode_to_vertex(signal_process_vertex) );
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
	GenVertex* itsDecayVtx = evt->barcode_to_vertex(vtx);
	if ( itsDecayVtx ) itsDecayVtx->add_particle_in( p );
	else {
	    std::cerr << "read_io_genevent: ERROR particle points"
		      << " to null end vertex. " <<std::endl;
	}
	// also look for the beam particles
	if( p->barcode() == bp1 ) beam1 = p;
	if( p->barcode() == bp2 ) beam2 = p;
    }
    evt->set_beam_particles(beam1,beam2);
    return true;
}

bool CommonIO::read_io_particle_data_table( std::istream* is, ParticleDataTable* pdt )
{
    // 
    // read Individual GenParticle data entries
    while ( read_particle_data( is, pdt ) ) ;
    return true;
}

HeavyIon* CommonIO::read_heavy_ion(std::istream* is)
{
    // assumes mode has already been checked
    //
    // test to be sure the next entry is of type "H" then ignore it
    if ( !(*is) || is->peek()!='H' ) {
	std::cerr << "CommonIO::read_heavy_ion setting badbit." << std::endl;
	is->clear(std::ios::badbit); 
	return false;
    } 
    is->ignore();
    // read values into temp variables, then create a new HeavyIon object
    int nh =0, np =0, nt =0, nc =0, 
        neut = 0, prot = 0, nw =0, nwn =0, nwnw =0;
    float impact = 0., plane = 0., xcen = 0., inel = 0.; 
    *is >> nh >> np >> nt >> nc >> neut >> prot
	   >> nw >> nwn >> nwnw >> impact >> plane >> xcen >> inel;
    is->ignore(2,'\n');
    if( nh == 0 ) return false;
    HeavyIon* ion = new HeavyIon(nh, np, nt, nc, neut, prot,
	                         nw, nwn, nwnw, 
				 impact, plane, xcen, inel );
    //
    return ion;
}

PdfInfo* CommonIO::read_pdf_info(std::istream* is)
{
    // assumes mode has already been checked
    //
    // test to be sure the next entry is of type "F" then ignore it
    if ( !(*is) || is->peek() !='F') {
	std::cerr << "CommonIO::read_pdf_info setting badbit." << std::endl;
	is->clear(std::ios::badbit); 
	return false;
    } 
    is->ignore();
    // read values into temp variables, then create a new PdfInfo object
    int id1 =0, id2 =0, pdf_id1=0, pdf_id2=0;
    double  x1 = 0., x2 = 0., scale = 0., pdf1 = 0., pdf2 = 0.; 
    *is >> id1 >> id2 >> x1 >> x2 >> scale >> pdf1 >> pdf2;
    // check to see if we are at the end of the line
    if( is->peek() != 10 ) {
        *is >> pdf_id1 >> pdf_id2;
    }
    is->ignore(2,'\n');
    if( id1 == 0 ) return false;
    PdfInfo* pdf = new PdfInfo( id1, id2, x1, x2, scale, pdf1, pdf2,
                                pdf_id1, pdf_id2 );
    //
    return pdf;
}

GenVertex* CommonIO::read_vertex( std::istream* is, TempParticleMap& particle_to_end_vertex )
{
    // assumes mode has already been checked
    //
    // test to be sure the next entry is of type "V" then ignore it
    if ( !(*is) || is->peek()!='V' ) {
	std::cerr << "CommonIO::read_vertex setting badbit." << std::endl;
	std::string line;
	*is >> line;
	std::cerr << "attempting to read " << line << std::endl;
	is->clear(std::ios::badbit); 
	return false;
    } 
    is->ignore();
    // read values into temp variables, then create a new GenVertex object
    int identifier =0, id =0, num_orphans_in =0, 
        num_particles_out = 0, weights_size = 0;
    double x = 0., y = 0., z = 0., t = 0.; 
    *is >> identifier >> id >> x >> y >> z >> t
	   >> num_orphans_in >> num_particles_out >> weights_size;
    WeightContainer weights(weights_size);
    for ( int i1 = 0; i1 < weights_size; ++i1 ) *is >> weights[i1];
    is->ignore(2,'\n');
    GenVertex* v = new GenVertex( FourVector(x,y,z,t),
			    id, weights);
    v->suggest_barcode( identifier );
    //
    // read and create the associated particles. outgoing particles are
    //  added to their production vertices immediately, while incoming
    //  particles are added to a map and handles later.
    for ( int i2 = 1; i2 <= num_orphans_in; ++i2 ) {
	read_particle(is,particle_to_end_vertex);
    }
    for ( int i3 = 1; i3 <= num_particles_out; ++i3 ) {
	v->add_particle_out( read_particle(is,particle_to_end_vertex) );
    }
    return v;
}

GenParticle* CommonIO::read_particle(std::istream* is,
    TempParticleMap& particle_to_end_vertex ){
    // assumes mode has already been checked
    //
    // test to be sure the next entry is of type "P" then ignore it
    if ( !(*is) || is->peek()!='P' ) { 
	std::cerr << "CommonIO::read_particle setting badbit." 
		  << std::endl;
	is->clear(std::ios::badbit); 
	return false;
    } 
    is->ignore();
    //
    // declare variables to be read in to, and read everything except flow
    double px = 0., py = 0., pz = 0., e = 0., m = 0., theta = 0., phi = 0.;
    int bar_code = 0, id = 0, status = 0, end_vtx_code = 0, flow_size = 0;
    if( m_io_type == ascii ) {
	*is >> bar_code >> id >> px >> py >> pz >> e >> status 
	    >> theta >> phi >> end_vtx_code >> flow_size;
    } else {
        *is >> bar_code >> id >> px >> py >> pz >> e >> m >> status 
	    >> theta >> phi >> end_vtx_code >> flow_size;
    }
    //
    // read flow patterns if any exist
    Flow flow;
    int code_index, code;
    for ( int i = 1; i <= flow_size; ++i ) {
	*is >> code_index >> code;
	flow.set_icode( code_index,code);
    }
    is->ignore(2,'\n'); // '\n' at end of entry
    GenParticle* p = new GenParticle( FourVector(px,py,pz,e), 
				id, status, flow, 
				Polarization(theta,phi) );
    if( m_io_type != ascii ) {
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
    return p;
}

ParticleData* CommonIO::read_particle_data( std::istream* is, ParticleDataTable* pdt ) {
    // assumes mode has already been checked
    //
    // test to be sure the next entry is of type "D" then ignore it
    if ( !(*is) || is->peek()!='D' ) return 0;
    is->ignore();
    //
    // read values into temp variables then create new ParticleData object
    char its_name[22];
    int its_id = 0, its_spin = 0;  
    double its_charge = 0, its_mass = 0, its_clifetime = 0;
    *is >> its_id >> its_charge >> its_mass 
	   >> its_clifetime >> its_spin;
    is->ignore(1); // eat the " "
    is->getline( its_name, 22, '\n' );
    ParticleData* pdata = new ParticleData( its_name, its_id, its_charge, 
					    its_mass, its_clifetime, 
					    double(its_spin)/2.);
    pdt->insert(pdata);
    return pdata;
}

bool CommonIO::read_units( std::istream* is, GenEvent* evt ) {
    // assumes mode has already been checked
    //
    // test to be sure the next entry is of type "F" then ignore it
    if ( !(*is) ) {
	std::cerr << "CommonIO::read_units setting badbit." << std::endl;
	is->clear(std::ios::badbit); 
	return false;
    } 
    // have no units, but this is not an error
    // releases prior to 2.04.00 did not write unit information
    if ( is->peek() !='U') {
 	evt->use_units( m_io_momentum_unit, m_io_position_unit );
	return true;
    } 
    is->ignore();	// ignore the first character in the line
    std::string mom, pos;
    *is >> mom >> pos;
    is->ignore(1);      // eat the extra whitespace
    evt->use_units(mom,pos);
    //
    return true;
}

}	// end namespace HepMC

