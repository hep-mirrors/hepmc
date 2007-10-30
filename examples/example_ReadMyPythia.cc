//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, October 2007
//  
// Read back the file written by example_MyPythia
//////////////////////////////////////////////////////////////////////////
// To Compile: go to the example directory and type:
// gmake example_ReadMyPythia.exe
//

#include <iostream>
#include <fstream>
#include "HepMC/IO_GenEvent.h"
#include "HepMC/GenEvent.h"

int main() { 
    //
    //........................................define an input scope
    {
        // open input stream
	std::ifstream istr( "example_MyPythia.dat" );
	if( !istr ) {
	  std::cerr << "example_ReadMyPythia: cannot open example_MyPythia.dat" << std::endl;
	  exit(-1);
	}
	HepMC::IO_GenEvent ascii_in(istr);
        // open output stream (alternate method)
	HepMC::IO_GenEvent ascii_out("example_MyPythia2.dat",std::ios::out);
	// now read the file
	int icount=0;
	HepMC::GenEvent* evt = ascii_in.read_next_event();
	while ( evt ) {
            icount++;
            if ( icount%50==1 ) std::cout << "Processing Event Number " << icount
                                	  << " its # " << evt->event_number() 
                                	  << std::endl;
	    // write the event out to the ascii file
	    ascii_out << evt;
            delete evt;
            ascii_in >> evt;
	}
	//........................................PRINT RESULT
	std::cout << icount << " events found. Finished." << std::endl;
    } // ascii_out and istr destructors are called here

    return 0;
}


 
