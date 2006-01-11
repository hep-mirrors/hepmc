//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, Feb 2000
// Example of reading in a computer readable GenParticle Data Table as
// as downloaded from the PDG webpage.
//
// To run this example you will have to download a PDG table from:
// http://pdg.lbl.gov/computer_read.html
// and name it PDG_ParticleDataTable.txt
//////////////////////////////////////////////////////////////////////////
// To Compile: go to the HepMC directory and type:
// gmake examples/example_ReadPDGtable.exe
//

#include "HepMC/IO_PDG_ParticleDataTable.h"

int main() {

    // create an instance of the IO_PDG_ParticleDataTable strategy
    HepMC::IO_PDG_ParticleDataTable pdg_io("PDG_ParticleDataTable.txt");
    if ( pdg_io.rdstate() == std::ios::failbit ) {
	std::cerr << "ERROR input file PDG_ParticleDataTable.txt is needed "
		  << "and does not exist. "
		  << "\n Look for it in HepMC/examples, Exit." << std::endl;
	return 1;
    }

    HepMC::ParticleDataTable pdt = *pdg_io.read_particle_data_table();

    // Note the PDG table does not include antiparticles nor quarks
    //  (except top). You can make anti-particle entries corresponding to 
    //  each charged entry with the method make_antiparticles_from_particles()
    //  and you can make default entries for the 6 quarks + anti-quarks with 
    //  the method add_quarks_to_table()
    pdt.make_antiparticles_from_particles();
    pdg_io.add_quarks_to_table( pdt );

    // print the table
    // note that the PDG files do not contain spin information,
    // so the spin field is left at zero.
    pdt.print();
    
    // now clean-up by deleteing all ParticleData objects in the table
    //
    pdt.delete_all();
    return 0;
}
