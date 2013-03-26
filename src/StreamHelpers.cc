//--------------------------------------------------------------------------
//
// GenEventStreamIO.cc
// Author:  Lynn Garren
//
// helper functions used by streaming IO
//
// ----------------------------------------------------------------------

#include <ostream>
#include <istream>
#include <sstream>

#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"
#include "HepMC/StreamHelpers.h"
#include "HepMC/IO_Exception.h"

namespace HepMC {

namespace detail {


std::istream & find_event_end( std::istream & is ) {
    // since there is no end of event flag, 
    // read one line at time until we find the next event 
    // or the end of event block
    // don't throw until we find the end of the event
    std::string line, firstc;
    while ( is ) { 
	is >> firstc;
        if( firstc=="E" ) {	// next event
	    is.unget();
            throw IO_Exception("input stream encountered invalid data");
	    return is;
	} else if( firstc.size() > 1 ) { // no more events in this block
            throw IO_Exception("input stream encountered invalid data, now at end of event block");
	    return is;
	}
        std::getline(is,line);
    }
    // the stream is bad 
    throw IO_Exception("input stream encountered invalid data, stream is now corrupt");
    return is;
}

} // detail

} // HepMC
