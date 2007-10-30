//--------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, July 2006
// event input/output in ascii format for machine reading
// extended format contains HeavyIon and PdfInfo classes
//////////////////////////////////////////////////////////////////////////

#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"
#include "HepMC/ParticleDataTable.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/PdfInfo.h"
#include "HepMC/Version.h"

namespace HepMC {

    IO_GenEvent::IO_GenEvent( const char* filename, std::ios::openmode mode ) 
    : m_mode(mode), 
      m_file(filename, mode), 
      m_ostr(0),
      m_istr(0),
      m_iostr(0),
      m_finished_first_event_io(false),
      m_have_file(false)
    {
	if ( (m_mode&std::ios::out && m_mode&std::ios::in) ||
	     (m_mode&std::ios::app && m_mode&std::ios::in) ) {
	    std::cerr << "IO_GenEvent::IO_GenEvent Error, open of file requested "
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
	// now we set the streams
	m_iostr = &m_file;
	if ( m_mode&std::ios::in ) {
	    m_istr = &m_file;
	    m_ostr = NULL;
	}
	if ( m_mode&std::ios::out ) {
	    m_ostr = &m_file;
	    m_istr = NULL;
	}
	m_have_file = true;
    }

    IO_GenEvent::IO_GenEvent( std::istream & istr ) 
    : m_ostr(0),
      m_istr(&istr),
      m_iostr(&istr),
      m_finished_first_event_io(false),
      m_have_file(false)
    { }

    IO_GenEvent::IO_GenEvent( std::ostream & ostr )
    : m_ostr(&ostr),
      m_istr(0),
      m_iostr(&ostr),
      m_finished_first_event_io(false),
      m_have_file(false)
    {
	// precision 16 (# digits following decimal point) is the minimum that
	//  will capture the full information stored in a double
	m_ostr->precision(16);
	// we use decimal to store integers, because it is smaller than hex!
	m_ostr->setf(std::ios::dec,std::ios::basefield);
	m_ostr->setf(std::ios::scientific,std::ios::floatfield);
    }

    IO_GenEvent::~IO_GenEvent() {
	write_end_listing();
	if(m_have_file) m_file.close();
    }

    void IO_GenEvent::print( std::ostream& ostr ) const { 
	ostr << "IO_GenEvent: unformated ascii file IO for machine reading.\n"; 
	if(m_have_file)    ostr  << "\tFile openmode: " << m_mode ;
	ostr << " stream state: " << m_ostr->rdstate()
	     << " bad:" << (m_ostr->rdstate()&std::ios::badbit)
	     << " eof:" << (m_ostr->rdstate()&std::ios::eofbit)
	     << " fail:" << (m_ostr->rdstate()&std::ios::failbit)
	     << " good:" << (m_ostr->rdstate()&std::ios::goodbit) << std::endl;
    }

    void IO_GenEvent::write_event( const GenEvent* evt ) {
	/// Writes evt to output stream. It does NOT delete the event after writing.
	//
	// make sure the state is good, and that it is in output mode
	if ( !evt  ) return;
	if ( m_ostr == NULL ) {
	    std::cerr << "HepMC::IO_GenEvent::write_event "
		      << " attempt to write to input file." << std::endl;
	    return;
	}
	//
	// write event listing key before first event only.
	if ( !m_finished_first_event_io ) {
	    m_finished_first_event_io = 1;
	    *m_ostr << "\n" << "HepMC::Version " << versionName();
	    *m_ostr << "\n" << "HepMC::IO_GenEvent-START_EVENT_LISTING\n";
	}
	//
	// output the event data including the number of primary vertices
	//  and the total number of vertices
	std::vector<long int> random_states = evt->random_states();
	*m_ostr << 'E';
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

    bool IO_GenEvent::fill_next_event( GenEvent* evt ){
	//
	//
	// test that evt pointer is not null
	if ( !evt ) {
	    std::cerr 
		<< "IO_GenEvent::fill_next_event error - passed null event." 
		<< std::endl;
	    return false;
	}
	// make sure the stream is good, and that it is in input mode
	if ( !(*m_istr) ) return false;
	if ( !m_istr ) {
	    std::cerr << "HepMC::IO_GenEvent::fill_next_event "
		      << " attempt to read from output file." << std::endl;
	    return false;
	}
	//
	// search for event listing key before first event only.
	//
	// skip through the file just after first occurence of the start_key
	if ( !m_finished_first_event_io ) {
	    //m_file.seekg( 0 ); // go to position zero in the file.
	    if (!search_for_key_end( *m_istr,
				     "HepMC::IO_GenEvent-START_EVENT_LISTING\n")){
		std::cerr << "IO_GenEvent::fill_next_event start key not found "
			  << "setting badbit." << std::endl;
		m_istr->clear(std::ios::badbit); 
		return false;
	    }
	    m_finished_first_event_io = 1;
	}
	//
	// test to be sure the next entry is of type "E" then ignore it
	if ( !(*m_istr) || m_istr->peek()!='E' ) { 
	    // if the E is not the next entry, then check to see if it is
	    // the end event listing key - if yes, search for another start key
	    if ( eat_key(*m_istr, "HepMC::IO_GenEvent-END_EVENT_LISTING\n") ) {
		bool search_result = search_for_key_end(*m_istr,
				      "HepMC::IO_GenEvent-START_EVENT_LISTING\n");
		if ( !search_result ) {
		    // this is the only case where we set an EOF state
		    m_istr->clear(std::ios::eofbit);
		    return false;
		}
	    } else {
		std::cerr << "IO_GenEvent::fill_next_event end key not found "
			  << "setting badbit." << std::endl;
		m_istr->clear(std::ios::badbit); 
		return false;
	    }
	} 
	m_istr->ignore();
	// read values into temp variables, then create a new GenEvent
	int event_number = 0, signal_process_id = 0, signal_process_vertex = 0,
	    num_vertices = 0, random_states_size = 0, weights_size = 0,
	    nmpi = 0, bp1 = 0, bp2 = 0;
	double eventScale = 0, alpha_qcd = 0, alpha_qed = 0;
	*m_istr >> event_number >> nmpi >> eventScale >> alpha_qcd >> alpha_qed
	       >> signal_process_id >> signal_process_vertex
	       >> num_vertices >> bp1 >> bp2 >> random_states_size;
	std::vector<long int> random_states(random_states_size);
	for ( int i = 0; i < random_states_size; ++i ) {
	    *m_istr >> random_states[i];
	}
	*m_istr >> weights_size;
	WeightContainer weights(weights_size);
	for ( int ii = 0; ii < weights_size; ++ii ) *m_istr >> weights[ii];
	m_istr->ignore(2,'\n');
	// 
	// fill signal_process_id, event_number, weights, random_states
	evt->set_signal_process_id( signal_process_id );
	evt->set_event_number( event_number );
	evt->set_mpi( nmpi );
	evt->weights() = weights;
	evt->set_random_states( random_states );
	// get HeavyIon and PdfInfo
	HeavyIon* ion = read_heavy_ion();
	if(ion) evt->set_heavy_ion( *ion );
	PdfInfo* pdf = read_pdf_info();
	if(pdf) evt->set_pdf_info( *pdf );
	//
	// the end vertices of the particles are not connected until
	//  after the event is read --- we store the values in a map until then
       	TempParticleMap particle_to_end_vertex;
	//
	// read in the vertices
	for ( int iii = 1; iii <= num_vertices; ++iii ) {
	    GenVertex* v = read_vertex(particle_to_end_vertex);
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
		std::cerr << "IO_GenEvent::fill_next_event ERROR particle points"
			  << "\n to null end vertex. " <<std::endl;
	    }
	    // also look for the beam particles
	    if( p->barcode() == bp1 ) beam1 = p;
	    if( p->barcode() == bp2 ) beam2 = p;
	}
	evt->set_beam_particles(beam1,beam2);
	return true;
    }

    void IO_GenEvent::write_comment( const std::string comment ) {
	// make sure the stream is good, and that it is in output mode
	if ( !(*m_ostr) ) return;
	if ( m_ostr == NULL ) {
	    std::cerr << "HepMC::IO_GenEvent::write_comment "
		      << " attempt to write to input file." << std::endl;
	    return;
	}
	// write end of event listing key if events have already been written
	write_end_listing();
	// insert the comment key before the comment
	*m_ostr << "\n" << "HepMC::IO_GenEvent-COMMENT\n";
	*m_ostr << comment << std::endl;
    }

    void IO_GenEvent::write_particle_data_table( const ParticleDataTable* pdt) {
	//
	// make sure the stream is good, and that it is in output mode
	if ( !(*m_ostr) ) return;
	if ( m_ostr == NULL ) {
	    std::cerr << "HepMC::IO_GenEvent::write_particle_data_table "
		      << " attempt to write to input file." << std::endl;
	    return;
	}
	// write end of event listing key if events have already been written
	write_end_listing();
	//
	*m_ostr << "\n" << "HepMC::IO_GenEvent-START_PARTICLE_DATA\n";
        for ( ParticleDataTable::const_iterator pd = pdt->begin(); 
	      pd != pdt->end(); pd++ ) {
	    write_particle_data( pd->second );
        }
	*m_ostr << "HepMC::IO_GenEvent-END_PARTICLE_DATA\n" << std::flush;
    }

    bool IO_GenEvent::fill_particle_data_table( ParticleDataTable* pdt ) {
	//
	// test that pdt pointer is not null
	if ( !pdt ) {
	    std::cerr 
		<< "IO_GenEvent::fill_particle_data_table - passed null table." 
		<< std::endl;
	    return false;
	}
	//
	// check the state of m_istr is good
	if ( !(*m_istr) ) return false;
	if ( m_istr == NULL ) {
	    std::cerr << "HepMC::IO_GenEvent::fill_particle_data_table "
		      << " attempt to read from output file." << std::endl;
	    return false;
	}
	// position to beginning of file
	int initial_file_position = m_istr->tellg();
	std::ios::iostate initial_state = m_istr->rdstate();
	m_istr->seekg( 0 );
	// skip through the file just after first occurence of the start_key
	if (!search_for_key_end( *m_istr,
				 "HepMC::IO_GenEvent-START_PARTICLE_DATA\n")) {
	    m_istr->seekg( initial_file_position );
	    std::cerr << "IO_GenEvent::fill_particle_data_table start key not  "
		      << "found setting badbit." << std::endl;
	    m_istr->clear(std::ios::badbit); 
	    return false;
	}
	//
	pdt->set_description("Read with IO_GenEvent");
	// 
	// read Individual GenParticle data entries
	while ( read_particle_data( pdt ) );
	//
	// eat end_key
	if ( !eat_key(*m_istr,"HepMC::IO_GenEvent-END_PARTICLE_DATA\n") ){
	    std::cerr << "IO_GenEvent::fill_particle_data_table end key not  "
		      << "found setting badbit." << std::endl;
	    m_istr->clear(std::ios::badbit);
	}
	// put the file back into its original state and position
	m_istr->clear( initial_state );
	m_istr->seekg( initial_file_position );
	return true;
    }

    void IO_GenEvent::write_vertex( GenVertex* v ) {
	// assumes mode has already been checked
	if ( !v || !(*m_ostr) ) {
	    std::cerr << "IO_GenEvent::write_vertex !v||!(*m_ostr), "
		      << "v="<< v << " setting badbit" << std::endl;
	    m_ostr->clear(std::ios::badbit); 
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
	*m_ostr << 'V';
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

    void IO_GenEvent::write_beam_particles( 
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

    void IO_GenEvent::write_heavy_ion( HeavyIon* ion ) {
	// assumes mode has already been checked
	if ( !(*m_ostr) ) {
	    std::cerr << "IO_GenEvent::write_heavy_ion !(*m_ostr), "
		      << " setting badbit" << std::endl;
	    m_ostr->clear(std::ios::badbit); 
	    return;
	}
	*m_ostr << 'H';
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

    void IO_GenEvent::write_pdf_info( PdfInfo* pdf ) {
	// assumes mode has already been checked
	if ( !(*m_ostr) ) {
	    std::cerr << "IO_GenEvent::write_pdf_info !(*m_ostr), "
		      << " setting badbit" << std::endl;
	    m_ostr->clear(std::ios::badbit); 
	    return;
	}
	*m_ostr << 'F';
	// PdfInfo* is set to 0 by default
	if ( !pdf ) {
	    output( 0 );
	    output( 0 );
	    output( 0. );
	    output( 0. );
	    output( 0. );
	    output( 0. );
	    output( 0. );
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
	output('\n');
    }

    void IO_GenEvent::write_particle( GenParticle* p ) {
	// assumes mode has already been checked
	if ( !p || !(*m_ostr) ) {
	    std::cerr << "IO_GenEvent::write_particle !p||!(*m_ostr), "
		      << "v="<< p << " setting badbit" << std::endl;
	    m_ostr->clear(std::ios::badbit); 
	    return;
	}
	*m_ostr << 'P';
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
	*m_ostr << ' ' << p->flow() << "\n";
    }

    void IO_GenEvent::write_particle_data( const ParticleData* pdata ) {
	// assumes mode has already been checked
	if ( !pdata || !(*m_ostr) ) {
	    std::cerr << "IO_GenEvent::write_particle_data !pdata||!(*m_ostr), "
		      << "pdata="<< pdata << " setting badbit" << std::endl;
	    m_ostr->clear(std::ios::badbit); 
	    return;
	}
	*m_ostr << 'D';
	output( pdata->pdg_id() );
	output( pdata->charge() );
	output( pdata->mass() );
	output( pdata->clifetime() );
	output( (int)(pdata->spin()*2.+.1) );
	// writes the first 21 characters starting with 0
	*m_ostr << " " << pdata->name().substr(0,21) << "\n";
    }

    GenVertex* IO_GenEvent::read_vertex
    ( TempParticleMap& particle_to_end_vertex )
    {
	// assumes mode has already been checked
	//
	// test to be sure the next entry is of type "V" then ignore it
	if ( !(*m_istr) || m_istr->peek()!='V' ) {
	    std::cerr << "IO_GenEvent::read_vertex setting badbit." << std::endl;
	    m_istr->clear(std::ios::badbit); 
	    return false;
	} 
	m_istr->ignore();
	// read values into temp variables, then create a new GenVertex object
	int identifier =0, id =0, num_orphans_in =0, 
            num_particles_out = 0, weights_size = 0;
	double x = 0., y = 0., z = 0., t = 0.; 
	*m_istr >> identifier >> id >> x >> y >> z >> t
	       >> num_orphans_in >> num_particles_out >> weights_size;
	WeightContainer weights(weights_size);
	for ( int i1 = 0; i1 < weights_size; ++i1 ) *m_istr >> weights[i1];
	m_istr->ignore(2,'\n');
	GenVertex* v = new GenVertex( FourVector(x,y,z,t),
				id, weights);
	v->suggest_barcode( identifier );
	//
	// read and create the associated particles. outgoing particles are
	//  added to their production vertices immediately, while incoming
	//  particles are added to a map and handles later.
	for ( int i2 = 1; i2 <= num_orphans_in; ++i2 ) {
	    read_particle(particle_to_end_vertex);
	}
	for ( int i3 = 1; i3 <= num_particles_out; ++i3 ) {
	    v->add_particle_out( read_particle(particle_to_end_vertex) );
	}
	return v;
    }

    HeavyIon* IO_GenEvent::read_heavy_ion()
    {
	// assumes mode has already been checked
	//
	// test to be sure the next entry is of type "H" then ignore it
	if ( !(*m_istr) || m_istr->peek()!='H' ) {
	    std::cerr << "IO_GenEvent::read_heavy_ion setting badbit." << std::endl;
	    m_istr->clear(std::ios::badbit); 
	    return false;
	} 
	m_istr->ignore();
	// read values into temp variables, then create a new HeavyIon object
	int nh =0, np =0, nt =0, nc =0, 
            neut = 0, prot = 0, nw =0, nwn =0, nwnw =0;
	float impact = 0., plane = 0., xcen = 0., inel = 0.; 
	*m_istr >> nh >> np >> nt >> nc >> neut >> prot
	       >> nw >> nwn >> nwnw >> impact >> plane >> xcen >> inel;
	m_istr->ignore(2,'\n');
	if( nh == 0 ) return false;
	HeavyIon* ion = new HeavyIon(nh, np, nt, nc, neut, prot,
	                             nw, nwn, nwnw, 
				     impact, plane, xcen, inel );
	//
	return ion;
    }

    PdfInfo* IO_GenEvent::read_pdf_info()
    {
	// assumes mode has already been checked
	//
	// test to be sure the next entry is of type "F" then ignore it
	if ( !(*m_istr) || m_istr->peek() !='F') {
	    std::cerr << "IO_GenEvent::read_pdf_info setting badbit." << std::endl;
	    m_istr->clear(std::ios::badbit); 
	    return false;
	} 
	m_istr->ignore();
	// read values into temp variables, then create a new PdfInfo object
	int id1 =0, id2 =0;
	double  x1 = 0., x2 = 0., scale = 0., pdf1 = 0., pdf2 = 0.; 
	*m_istr >> id1 >> id2 >> x1 >> x2 >> scale >> pdf1 >> pdf2;
	m_istr->ignore(2,'\n');
	if( id1 == 0 ) return false;
	PdfInfo* pdf = new PdfInfo( id1, id2, x1, x2, scale, pdf1, pdf2);
	//
	return pdf;
    }

    GenParticle* IO_GenEvent::read_particle(
	TempParticleMap& particle_to_end_vertex ){
	// assumes mode has already been checked
	//
	// test to be sure the next entry is of type "P" then ignore it
	if ( !(*m_istr) || m_istr->peek()!='P' ) { 
	    std::cerr << "IO_GenEvent::read_particle setting badbit." 
		      << std::endl;
	    m_istr->clear(std::ios::badbit); 
	    return false;
	} 
	m_istr->ignore();
	//
	// declare variables to be read in to, and read everything except flow
	double px = 0., py = 0., pz = 0., e = 0., m = 0., theta = 0., phi = 0.;
	int bar_code = 0, id = 0, status = 0, end_vtx_code = 0, flow_size = 0;
	*m_istr >> bar_code >> id >> px >> py >> pz >> e >> m >> status 
	       >> theta >> phi >> end_vtx_code >> flow_size;
	//
	// read flow patterns if any exist
	Flow flow;
	int code_index, code;
	for ( int i = 1; i <= flow_size; ++i ) {
	    *m_istr >> code_index >> code;
	    flow.set_icode( code_index,code);
	}
	m_istr->ignore(2,'\n'); // '\n' at end of entry
	GenParticle* p = new GenParticle( FourVector(px,py,pz,e), 
				    id, status, flow, 
				    Polarization(theta,phi) );
        p->set_generated_mass( m );
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

    ParticleData* IO_GenEvent::read_particle_data( ParticleDataTable* pdt ) {
	// assumes mode has already been checked
	//
	// test to be sure the next entry is of type "D" then ignore it
	if ( !(*m_istr) || m_istr->peek()!='D' ) return false;
	m_istr->ignore();
	//
	// read values into temp variables then create new ParticleData object
	char its_name[22];
	int its_id = 0, its_spin = 0;  
	double its_charge = 0, its_mass = 0, its_clifetime = 0;
	*m_istr >> its_id >> its_charge >> its_mass 
	       >> its_clifetime >> its_spin;
	m_istr->ignore(1); // eat the " "
	m_istr->getline( its_name, 22, '\n' );
	ParticleData* pdata = new ParticleData( its_name, its_id, its_charge, 
						its_mass, its_clifetime, 
						double(its_spin)/2.);
	pdt->insert(pdata);
	return pdata;
    }

    bool IO_GenEvent::write_end_listing() {
    	if ( m_finished_first_event_io && (m_ostr != NULL) ) {
	    *m_ostr << "HepMC::IO_GenEvent-END_EVENT_LISTING\n" << std::flush;
	    m_finished_first_event_io = 0;
	    return true;
	}
	return false;
    }

    bool IO_GenEvent::search_for_key_end( std::istream& in, const char* key ) {
	/// reads characters from in until the string of characters matching
	/// key is found (success) or EOF is reached (failure).
	/// It stops immediately thereafter. Returns T/F for success/fail
	// 
	char c[1];
	unsigned int index = 0;
	while ( in.get(c[0]) ) {
	    if ( c[0] == key[index] ) { 
		++index;
	    } else { index = 0; }
	    if ( index == strlen(key) ) return true;
	}
	return false;
    }

    bool IO_GenEvent::search_for_key_beginning( std::istream& in,
					     const char* key ) {
	/// not tested and NOT used anywhere!
	if ( search_for_key_end( in, key) ) {
	    int i = strlen(key);
	    while ( i>=0 ) in.putback(key[i--]); 
            return true; 
	} else {
	    in.putback(EOF);
	    in.clear();
            return false;
	}
    }

    bool IO_GenEvent::eat_key( std::istream& in, const char* key ) {
	/// eats the character string key from istream in - only if the key
	/// is the very next occurence in the stream
	/// if the key is not the next occurence, it eats nothing ... i.e.
	///  it puts back whatever it would have eaten.
	int key_length = strlen(key);
	// below is the only way I know of to get a variable length string
	//  conforming to ansi standard.
	char* c = new char[key_length +1];
	int i=0;
	// read the stream until get fails (because of EOF), a character
	//  doesn't match a character in the string, or all characters in
	//  the string have been checked and match.
	while ( in.get(c[i]) && c[i]==key[i] && i<key_length ) {
	    ++i;
	}
	if ( i == key_length ) {
	    delete [] c;
	    return true;
	}
	//
	// if we get here, then we have eaten the wrong this and we must put it
	// back
	while ( i>=0 ) in.putback(c[i--]); 
	delete c;
	return false;
    }

    int IO_GenEvent::find_in_map( const std::map<HepMC::GenVertex*,int>& m, 
			       GenVertex* v ) const {
	std::map<GenVertex*,int>::const_iterator iter = m.find(v);
	if ( iter == m.end() ) return false;
	return iter->second;
    }
	
} // HepMC
