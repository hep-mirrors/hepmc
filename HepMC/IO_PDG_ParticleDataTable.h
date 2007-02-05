//--------------------------------------------------------------------------
#ifndef HEPMC_IO_PDG_PARTICLEDATATABLE_H
#define HEPMC_IO_PDG_PARTICLEDATATABLE_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, November 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// Reads a particle data table from file supplied in PDG format
// For the most recent table see: http://pdg.lbl.gov/computer_read.html
//////////////////////////////////////////////////////////////////////////
//
// Example of reading from file PDG98_ParticleDataTable.txt
//
//	IO_PDG_ParticleDataTable pdg_io("PDG98_ParticleDataTable.txt");
// 	ParticleDataTable pdt = *pdg_io.read_particle_data_table();
//      (note the read_particle_data_table method comes from the IO_Baseclass)
//  or
//	IO_PDG_ParticleDataTable pdg_io("PDG98_ParticleDataTable.txt");
// 	ParticleDataTable* pdt;
//      pdg_io >> pdt;       
//  in both cases IO_PDG_ParticleDataTable creates an instance of the 
//  ParticleDataTable which the user becomes owner of (i.e. user is 
//  responsible for deleting)
//
// Note the PDG table does not include antiparticles nor quarks (except top)
// You can make anti-particle entries corresponding to each charged entry with
// the method make_antiparticles_from_particles() and you can make default
// entries for the 6 quarks + anti-quarks with the method add_quarks_to_table()
// Continuing with the above example:
//      pdt->make_antiparticles_from_particles();
//      pdg_io.add_quarks_to_table( *pdt );
//

#include "HepMC/IO_BaseClass.h"
#include <fstream>

namespace HepMC {

    class GenEvent;

    //! an example ParticleDataTable IO method
    
    ///
    /// \class IO_PDG_ParticleDataTable
    /// Example of reading from file PDG98_ParticleDataTable.txt
    ///
    class IO_PDG_ParticleDataTable : public IO_BaseClass {
    public:
        /// constructor using filename
	IO_PDG_ParticleDataTable( const char* filename
				  ="PDG98_ParticleDataTable.txt" );
	virtual ~IO_PDG_ParticleDataTable();
	/// read the input and fill the table
	bool    fill_particle_data_table( ParticleDataTable* );
	/// add u, d, s, c, b, and t
	void    add_quarks_to_table( ParticleDataTable& );
	/// write to ostr
	void    print( std::ostream& ostr = std::cout ) const;

        /// check the IO state
	int     rdstate() const { return (int)m_file.rdstate(); }
    protected: // for internal use only
        /// for internal use
	bool search_for_key_end( std::istream& in, const char* key );
	/// read a line
	void read_entry( ParticleDataTable* );
    private: // following are not implemented
	void write_event( const GenEvent* ){}
	bool fill_next_event( GenEvent* ){ return 0; }
	void write_particle_data_table( const ParticleDataTable* ){}
    private: // use of copy constructor is not allowed
	IO_PDG_ParticleDataTable( const IO_PDG_ParticleDataTable& ) : 
	    IO_BaseClass() {}
    private: // member data
	std::string   m_filename;
	std::ifstream m_file;
    };

    //////////////
    // Inlines  //
    //////////////

    inline void IO_PDG_ParticleDataTable::print( std::ostream& ostr ) const { 
	ostr << "IO_PDG_ParticleDataTable: for computer readable PDG tables.\n"
             << " file state: " << m_file.rdstate()
             << " bad:" << (m_file.rdstate()&std::ios::badbit)
             << " eof:" << (m_file.rdstate()&std::ios::eofbit)
             << " fail:" << (m_file.rdstate()&std::ios::failbit)
             << " good:" << (m_file.rdstate()&std::ios::goodbit) << std::endl;
    }

} // HepMC

#endif  // HEPMC_IO_BASECLASS_H
//--------------------------------------------------------------------------







