//--------------------------------------------------------------------------

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, January 2000
// Reads a particle data table from file supplied in PDG format
// For the most recent table see: http://pdg.lbl.gov/computer_read.html
//////////////////////////////////////////////////////////////////////////

#include <ctype.h>      // for isspace() etc.
#include <string>
#include <vector>
#include <cstdlib>      // needed for abort()
#include "HepMC/IO_PDG_ParticleDataTable.h"

namespace HepMC {

    IO_PDG_ParticleDataTable::IO_PDG_ParticleDataTable( const char* filename ) 
        : m_filename(filename), m_file(filename) {}
    
    IO_PDG_ParticleDataTable::~IO_PDG_ParticleDataTable() {
	m_file.close();
    }

    bool IO_PDG_ParticleDataTable::fill_particle_data_table( 
	ParticleDataTable* pdt) 
    {
	//
	// test that pdt pointer is not null
	if ( !pdt ) {
	    std::cerr 
		<< "IO_Ascii::fill_particle_data_table - passed null table." 
		<< std::endl;
	    return 0;
	}
	//
	// test the file is good and advance to keyword
        if (!m_file ) {
	  // return 0;
	  std::cerr << "IO_PDG_ParticleDataTable::fill_particle_data_table "
		    << " ERROR, file " << m_filename << " does not exist, "
		    << " ABORT." << std::endl;
	  abort();
	}	
	m_file.seekg( 0 );
	if ( !search_for_key_end(m_file,"\nM") ) {
	    std::cerr << "IO_PDG_ParticleDataTable: "
		      << "Error advancing to first entry." 
		      << std::endl;
	    return 0;
	}
	// set description
	std::string description = "Read from " + m_filename 
				  + " by IO_PDG_ParticleDataTable";
	pdt->set_description( description );
	m_file.putback('M');
	// read in each entry one by one
	while ( m_file.rdstate() == 0 ) {
	    read_entry( pdt );
	}
	return 1;
    }

    void IO_PDG_ParticleDataTable::read_entry( ParticleDataTable* pdt ) {
	int position = m_file.tellg();
	// variables for input
	char code;
	int id[4] = { 0,0,0,0 };
	int charge[4] = { 999, 999, 999, 999 };
	double value;
	std::string name;

	code = m_file.get();
	if ( code == EOF ) {
	    m_file.clear( std::ios::eofbit );
	    return;
	}

	if ( code == '\n' ) return; // ignore empty lines.

	if ( code!='M' && code!='W' ) {
	    std::cerr << "IO_PDG_ParticleDataTable::read_entry cannot "
		      << "understand syntax, setting badbit."
		      << std::endl;
	    m_file.putback( code );
	    m_file.clear( std::ios::badbit ); 
            return;
        } 
	// read the particle ID's -- here I assume there is atleast one
	// white space b/t the 4 id columns.
	for ( int id_i1 = 0; id_i1 < 4; ++id_i1 ) {
	    m_file.seekg( position+1+(id_i1*8) );
	    for ( int i = 0; i < 8; ++i ) {
		if ( !isspace(m_file.peek()) ) {
		    //m_file.width(8-i); // has no effect
		    // how do I limit to reading only 8-i characters?
		    // setw(8-i) has no effect either.
		    m_file >> id[id_i1];
		    break;
		}
		m_file.ignore(); 
	    }
	}
	//m_file.width(0);
	m_file.seekg(position+33);
	m_file >> value;
	m_file.seekg(position+66);
	m_file >> name;
	// loop over all remaining columns of the name section, where the 
	// charge is stored --- until \n is reached
	// Here I assume there is atleast one white space between the name
	// and the charge. This is true for the 1998 PDG table, and I expect
	// it will always be true.
	int id_i2 = 0;
	while ( m_file.rdstate() == 0 ) {
	    char c = m_file.get();
	    if ( c == '\n' || c == EOF ) break;
	    if ( isspace(c) || c==',' ) {
	    } else if ( c=='-' ) { 
		charge[id_i2] = -1; 
		++id_i2; 
	    } else if ( c=='0' ) {
		charge[id_i2] = 0;
		++id_i2; 
	    } else if ( c=='+' ) {
		charge[id_i2] = 1;
		if ( m_file.peek() == '+' ) {
		    charge[id_i2] = 2;
		    m_file.ignore();
		}
		++id_i2;
	    }
	}
	// add the new information to the particle data table.
	for ( int id_i3 = 0; id_i3 < 4; ++id_i3 ) {
	    if ( id[id_i3] == 0 ) continue;
	    std::string scharge;
	    if ( charge[id_i3] == -1 ) { 
		scharge = "-";
	    } else if ( charge[id_i3] == 1 ) { 
		scharge = "+";
	    } else if ( charge[id_i3] == 2 ) { 
		scharge = "++"; 
	    }
	    ParticleData* pdata = pdt->find( id[id_i3] );
	    if ( pdata ) {
		if ( code == 'M' ) {
		    pdata->set_mass( value );
		} else if ( code == 'W' ) {
		    pdata->set_clifetime( clifetime_from_width(value) );
		}
	    } else if ( code == 'M' ) {
		pdata = new ParticleData( name+scharge, 
					  id[id_i3], (double)charge[id_i3], 
					  value );
		pdt->insert(pdata);
	    } else if ( code == 'W' ) {
		pdata = new ParticleData( name+scharge, 
					  id[id_i3], (double)charge[id_i3],
					  0, clifetime_from_width(value) );
		pdt->insert(pdata);
	    }
	}
    }

    void IO_PDG_ParticleDataTable::add_quarks_to_table( 
	ParticleDataTable& pdt ) {
	// since quarks aren't included in PDG table, this method adds them
	// in
	std::vector<std::string> name(6);
	name[0] = "d";
	name[1] = "u";
	name[2] = "s";
	name[3] = "c";
	name[4] = "b";
	name[5] = "t";
	int id[6] = { 1, 2, 3, 4, 5, 6 };
	double charge[6] = { -1./3., 2./3., -1./3., 2./3., -1./3., 2./3. };
	// take central values from PDG July 1998 for masses
	double mass[6] = { 0.006, 0.00325, 0.115, 1.25, 4.25, 173.8 };
	for ( int i = 0; i<6; ++i ) {
	    // build the particle
	    ParticleData* pdata = pdt.find(id[i]);
	    if ( pdata ) {
		mass[i] = pdata->mass();
		pdt.erase(pdata);
		delete pdata;
	    }
	    pdata = new ParticleData( name[i], id[i], charge[i], mass[i], -1.,
				      .5 );
	    pdt.insert(pdata);
	    // build the antiparticle
	    pdata = pdt.find(-1*id[i]);
	    if ( pdata ) {
		pdt.erase(pdata);
		delete pdata;
	    }
	    pdata = new ParticleData( name[i]+"~", -1*id[i], -1*charge[i], 
				      mass[i], -1., .5 );
	    pdt.insert(pdata);
	}
    }

    bool IO_PDG_ParticleDataTable::search_for_key_end( std::istream& in, 
						       const char* key ) {
	// (this method borrowed from IO_Ascii class)
        // reads characters from in until the string of characters matching
        // key is found (success) or EOF is reached (failure).
        // It stops immediately thereafter. Returns T/F for success/fail
        // 
        char c[1];
        unsigned int index = 0;
        while ( in.get(c[0]) ) {
            if ( c[0] == key[index] ) { 
                ++index;
            } else { index = 0; }
            if ( index == strlen(key) ) return 1;
        }
        return 0;
    }

} // HepMC









