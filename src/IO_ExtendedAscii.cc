//--------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, July 2006
// event input/output in ascii format for machine reading
// extended format contains HeavyIon and PdfInfo classes
//////////////////////////////////////////////////////////////////////////

#include "HepMC/IO_ExtendedAscii.h"
#include "HepMC/GenEvent.h"
#include "HepMC/ParticleDataTable.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/CommonIO.h"
#include "HepMC/Version.h"

namespace HepMC {

    IO_ExtendedAscii::IO_ExtendedAscii( const char* filename, std::ios::openmode mode ) 
	: m_mode(mode), m_file(filename, mode), m_finished_first_event_io(0),
          m_common_io() 
    {
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << "Use of HepMC/IO_ExtendedAscii is deprecated" << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
	if ( (m_mode&std::ios::out && m_mode&std::ios::in) ||
	     (m_mode&std::ios::app && m_mode&std::ios::in) ) {
	    std::cerr << "IO_ExtendedAscii::IO_ExtendedAscii Error, open of file requested "
		      << "of input AND output type. Not allowed. Closing file."
		      << std::endl;
	    m_file.close();
	    return;
	}
	// precision 16 (# digits following decimal point) is the minimum that
	//  will capture the full information stored in a double
	m_file.precision(16);
	// we use decimal to store integers, because it is smaller than hex!
	m_file.setf(std::ios::dec,std::ios::basefield);
	m_file.setf(std::ios::scientific,std::ios::floatfield);
    }

    IO_ExtendedAscii::~IO_ExtendedAscii() {
	write_end_listing();
	m_file.close();
    }

    void IO_ExtendedAscii::print( std::ostream& ostr ) const { 
	ostr << "IO_ExtendedAscii: unformated ascii file IO for machine reading.\n" 
	     << "\tFile openmode: " << m_mode 
	     << " file state: " << m_file.rdstate()
	     << " bad:" << (m_file.rdstate()&std::ios::badbit)
	     << " eof:" << (m_file.rdstate()&std::ios::eofbit)
	     << " fail:" << (m_file.rdstate()&std::ios::failbit)
	     << " good:" << (m_file.rdstate()&std::ios::goodbit) << std::endl;
    }

    void IO_ExtendedAscii::write_event( const GenEvent* evt ) {
	/// Writes evt to m_file. It does NOT delete the event after writing.
	//
	// check the state of m_file is good, and that it is in output mode
	if ( !evt || !m_file ) return;
	if ( !m_mode&std::ios::out ) {
	    std::cerr << "HepMC::IO_ExtendedAscii::write_event "
		      << " attempt to write to input file." << std::endl;
	    return;
	}
	//
	// write event listing key before first event only.
	if ( !m_finished_first_event_io ) {
	    m_finished_first_event_io = 1;
	    m_file << "\n" << "HepMC::Version " << versionName();
	    m_file << "\n" ;
	    m_common_io.write_IO_ExtendedAscii_Key(m_file);
	}
	//
	// output the event data including the number of primary vertices
	//  and the total number of vertices
	std::vector<long int> random_states = evt->random_states();
	m_file << 'E';
	output( evt->event_number() );
	output( evt->mpi() );
	output( evt->event_scale() );
	output( evt->alphaQCD() );
	output( evt->alphaQED() );
	output( evt->signal_process_id() );
	output(   ( evt->signal_process_vertex() ?
		    evt->signal_process_vertex()->barcode() : 0 )   );
	output( evt->vertices_size() ); // total number of vertices.
	write_beam_particles( evt->beam_particles() );
	output( (int)random_states.size() );
	for ( std::vector<long int>::iterator rs = random_states.begin(); 
	      rs != random_states.end(); ++rs ) {
	    output( *rs );
	}
	output( (int)evt->weights().size() );
	for ( WeightContainer::const_iterator w = evt->weights().begin(); 
	      w != evt->weights().end(); ++w ) {
	    output( *w );
	}
	output('\n');
	write_heavy_ion( evt->heavy_ion() );
	write_pdf_info( evt->pdf_info() );
	//
	// Output all of the vertices - note there is no real order.
	for ( GenEvent::vertex_const_iterator v = evt->vertices_begin();
	      v != evt->vertices_end(); ++v ) {
	    write_vertex( *v );
	}
    }

    bool IO_ExtendedAscii::fill_next_event( GenEvent* evt ){
	//
	//
	// test that evt pointer is not null
	if ( !evt ) {
	    std::cerr 
		<< "IO_ExtendedAscii::fill_next_event error - passed null event." 
		<< std::endl;
	    return false;
	}
	// check the state of m_file is good, and that it is in input mode
	if ( !m_file ) return false;
	if ( !(m_mode&std::ios::in) ) {
	    std::cerr << "HepMC::IO_ExtendedAscii::fill_next_event "
		      << " attempt to read from output file." << std::endl;
	    return false;
	}
	//
	// search for event listing key before first event only.
	//
	// skip through the file just after first occurence of the start_key
	int iotype;
	if ( !m_finished_first_event_io ) {
	    iotype = m_common_io.find_file_type(m_file);
	    if( iotype != extascii ) {
		std::cerr << "IO_ExtendedAscii::fill_next_event start key not found "
			  << "setting badbit." << std::endl;
		m_file.clear(std::ios::badbit); 
		return false;
	    }
	    m_finished_first_event_io = 1;
	}
	//
	// test to be sure the next entry is of type "E" then ignore it
	if ( !m_file ) { 
		std::cerr << "IO_ExtendedAscii::fill_next_event end of stream found "
			  << "setting badbit." << std::endl;
		m_file.clear(std::ios::badbit); 
		return false;
	}
	if ( !m_file || m_file.peek()!='E' ) { 
	    // if the E is not the next entry, then check to see if it is
	    // the end event listing key - if yes, search for another start key
	    if ( m_common_io.find_end_key(m_file) ) {
		iotype = m_common_io.find_file_type(m_file);
		if( iotype != extascii ) {
		    // this is the only case where we set an EOF state
		    m_file.clear(std::ios::eofbit);
		    return false;
		}
	    } else {
		std::cerr << "IO_ExtendedAscii::fill_next_event end key not found "
			  << "setting badbit." << std::endl;
		m_file.clear(std::ios::badbit); 
		return false;
	    }
	} 
	m_file.ignore();
	// call the read method
	return m_common_io.read_io_extendedascii(&m_file, evt);
    }

    void IO_ExtendedAscii::write_comment( const std::string comment ) {
	// check the state of m_file is good, and that it is in output mode
	if ( !m_file ) return;
	if ( !m_mode&std::ios::out ) {
	    std::cerr << "HepMC::IO_ExtendedAscii::write_comment "
		      << " attempt to write to input file." << std::endl;
	    return;
	}
	// write end of event listing key if events have already been written
	write_end_listing();
	// insert the comment key before the comment
	m_file << "\n" << "HepMC::IO_ExtendedAscii-COMMENT\n";
	m_file << comment << std::endl;
    }

    void IO_ExtendedAscii::write_particle_data_table( const ParticleDataTable* pdt) {
	//
	// check the state of m_file is good, and that it is in output mode
	if ( !m_file ) return;
	if ( !m_mode&std::ios::out ) {
	    std::cerr << "HepMC::IO_ExtendedAscii::write_particle_data_table "
		      << " attempt to write to input file." << std::endl;
	    return;
	}
	// write end of event listing key if events have already been written
	write_end_listing();
	//
	m_file << "\n" << "HepMC::IO_ExtendedAscii-START_PARTICLE_DATA\n";
        for ( ParticleDataTable::const_iterator pd = pdt->begin(); 
	      pd != pdt->end(); pd++ ) {
	    write_particle_data( pd->second );
        }
	m_file << "HepMC::IO_ExtendedAscii-END_PARTICLE_DATA\n" << std::flush;
    }

    bool IO_ExtendedAscii::fill_particle_data_table( ParticleDataTable* pdt ) {
	//
	// test that pdt pointer is not null
	if ( !pdt ) {
	    std::cerr 
		<< "IO_ExtendedAscii::fill_particle_data_table - passed null table." 
		<< std::endl;
	    return false;
	}
	//
	// check the state of m_file is good, and that it is in input mode
	if ( !m_file ) return false;
	if ( !m_mode&std::ios::in ) {
	    std::cerr << "HepMC::IO_ExtendedAscii::fill_particle_data_table "
		      << " attempt to read from output file." << std::endl;
	    return false;
	}
	// position to beginning of file
	int initial_file_position = m_file.tellg();
	std::ios::iostate initial_state = m_file.rdstate();
	m_file.seekg( 0 );
	// skip through the file just after first occurence of the start_key
	int iotype;
	iotype = m_common_io.find_file_type(m_file);
	if( iotype != extascii_pdt ) {
	    m_file.seekg( initial_file_position );
	    std::cerr << "IO_ExtendedAscii::fill_particle_data_table start key not  "
		      << "found setting badbit." << std::endl;
	    m_file.clear(std::ios::badbit); 
	    return false;
	}
	//
	pdt->set_description("Read with IO_ExtendedAscii");
	m_common_io.read_io_particle_data_table( &m_file, pdt );
	//
	// check for the end event listing key
	iotype =  m_common_io.find_end_key(m_file);
	if( iotype != extascii_pdt ) {
	    std::cerr << "IO_ExtendedAscii::fill_particle_data_table end key not  "
		      << "found setting badbit." << std::endl;
	    m_file.clear(std::ios::badbit);
	}
	// put the file back into its original state and position
	m_file.clear( initial_state );
	m_file.seekg( initial_file_position );
	return true;
    }

    void IO_ExtendedAscii::write_vertex( GenVertex* v ) {
	// assumes mode has already been checked
	if ( !v || !m_file ) {
	    std::cerr << "IO_ExtendedAscii::write_vertex !v||!m_file, "
		      << "v="<< v << " setting badbit" << std::endl;
	    m_file.clear(std::ios::badbit); 
	    return;
	}
	// First collect info we need
	// count the number of orphan particles going into v
	int num_orphans_in = 0;
	for ( GenVertex::particles_in_const_iterator p1
		  = v->particles_in_const_begin();
	      p1 != v->particles_in_const_end(); ++p1 ) {
	    if ( !(*p1)->production_vertex() ) ++num_orphans_in;
	}
	//
	m_file << 'V';
	output( v->barcode() ); // v's unique identifier
	output( v->id() );
	output( v->position().x() );
	output( v->position().y() );
	output( v->position().z() );
	output( v->position().t() );
	output( num_orphans_in );
	output( (int)v->particles_out_size() );
	output( (int)v->weights().size() );
	for ( WeightContainer::iterator w = v->weights().begin(); 
	      w != v->weights().end(); ++w ) {
	    output( *w );
	}
	output('\n');
	for ( GenVertex::particles_in_const_iterator p2 
		  = v->particles_in_const_begin();
	      p2 != v->particles_in_const_end(); ++p2 ) {
	    if ( !(*p2)->production_vertex() ) {
		write_particle( *p2 );
	    }
	}
	for ( GenVertex::particles_out_const_iterator p3 
		  = v->particles_out_const_begin();
	      p3 != v->particles_out_const_end(); ++p3 ) {
	    write_particle( *p3 );
	}
    }

    void IO_ExtendedAscii::write_beam_particles( 
        std::pair<HepMC::GenParticle *,HepMC::GenParticle *> pr ) {
        GenParticle* p = pr.first;
	//m_file << 'B';
	if(!p) {
	   output( 0 );
	} else {
	   output( p->barcode() );
	}
        p = pr.second;
	if(!p) {
	   output( 0 );
	} else {
	   output( p->barcode() );
	}
    }

    void IO_ExtendedAscii::write_heavy_ion( HeavyIon const * ion ) {
	// assumes mode has already been checked
	if ( !m_file ) {
	    std::cerr << "IO_ExtendedAscii::write_heavy_ion !m_file, "
		      << " setting badbit" << std::endl;
	    m_file.clear(std::ios::badbit); 
	    return;
	}
	m_file << 'H';
	// HeavyIon* is set to 0 by default
	if ( !ion  ) {
	    output( 0 );
	    output( 0 );
	    output( 0 );
	    output( 0 );
	    output( 0 );
	    output( 0 );
	    output( 0 );
	    output( 0 );
	    output( 0 );
	    output( 0. );
	    output( 0. );
	    output( 0. );
	    output( 0. );
	    output('\n');
	    return;
	}
	//
	output( ion->Ncoll_hard() );
	output( ion->Npart_proj() );
	output( ion->Npart_targ() );
	output( ion->Ncoll() );
	output( ion->spectator_neutrons() );
	output( ion->spectator_protons() );
	output( ion->N_Nwounded_collisions() );
	output( ion->Nwounded_N_collisions() );
	output( ion->Nwounded_Nwounded_collisions() );
	output( ion->impact_parameter() );
	output( ion->event_plane_angle() );
	output( ion->eccentricity() );
	output( ion->sigma_inel_NN() );
	output('\n');
    }

    void IO_ExtendedAscii::write_pdf_info( PdfInfo const * pdf ) {
	// assumes mode has already been checked
	if ( !m_file ) {
	    std::cerr << "IO_ExtendedAscii::write_pdf_info !m_file, "
		      << " setting badbit" << std::endl;
	    m_file.clear(std::ios::badbit); 
	    return;
	}
	m_file << 'F';
	// PdfInfo* is set to 0 by default
	if ( !pdf ) {
	    output( 0 );
	    output( 0 );
	    output( 0. );
	    output( 0. );
	    output( 0. );
	    output( 0. );
	    output( 0. );
	    output( 0 );
	    output( 0 );
	    output('\n');
	    return;
	}
	//
	output( pdf->id1() );
	output( pdf->id2() );
	output( pdf->x1() );
	output( pdf->x2() );
	output( pdf->scalePDF() );
	output( pdf->pdf1() );
	output( pdf->pdf2() );
	output( pdf->pdf_id1() );
	output( pdf->pdf_id2() );
	output('\n');
    }

    void IO_ExtendedAscii::write_particle( GenParticle* p ) {
	// assumes mode has already been checked
	if ( !p || !m_file ) {
	    std::cerr << "IO_ExtendedAscii::write_particle !p||!m_file, "
		      << "v="<< p << " setting badbit" << std::endl;
	    m_file.clear(std::ios::badbit); 
	    return;
	}
	m_file << 'P';
	output( p->barcode() );
	output( p->pdg_id() );
	output( p->momentum().px() );
	output( p->momentum().py() );
	output( p->momentum().pz() );
	output( p->momentum().e() );
	output( p->generated_mass() );
	output( p->status() );
	output( p->polarization().theta() );
	output( p->polarization().phi() );
	// since end_vertex is oftentimes null, this CREATES a null vertex
	// in the map
	output(   ( p->end_vertex() ? p->end_vertex()->barcode() : 0 )  );
	m_file << ' ' << p->flow() << "\n";
    }

    void IO_ExtendedAscii::write_particle_data( const ParticleData* pdata ) {
	// assumes mode has already been checked
	if ( !pdata || !m_file ) {
	    std::cerr << "IO_ExtendedAscii::write_particle_data !pdata||!m_file, "
		      << "pdata="<< pdata << " setting badbit" << std::endl;
	    m_file.clear(std::ios::badbit); 
	    return;
	}
	m_file << 'D';
	output( pdata->pdg_id() );
	output( pdata->charge() );
	output( pdata->mass() );
	output( pdata->clifetime() );
	output( (int)(pdata->spin()*2.+.1) );
	// writes the first 21 characters starting with 0
	m_file << " " << pdata->name().substr(0,21) << "\n";
    }

    bool IO_ExtendedAscii::write_end_listing() {
    	if ( m_finished_first_event_io && m_mode&std::ios::out ) {
	    m_common_io.write_IO_ExtendedAscii_End(m_file);
	    m_file << std::flush;
	    m_finished_first_event_io = 0;
	    return true;
	}
	return false;
    }
	
} // HepMC
