#ifndef HEPMC_STREAM_HELPERS_H
#define HEPMC_STREAM_HELPERS_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, March 2009
//
// This header contains helper functions used by streaming IO
//////////////////////////////////////////////////////////////////////////

#include <ostream>
#include <istream>

#include "HepMC/GenEvent.h"
#include "HepMC/TempParticleMap.h"

namespace HepMC {

  namespace detail {

    /// Used by IO_GenEvent constructor
    std::ostream & establish_output_stream_info( std::ostream & );
    /// Used by IO_GenEvent constructor
    std::istream & establish_input_stream_info( std::istream & );

    /// Get a GenVertex from ASCII input
    ///
    /// TempParticleMap is used to track the associations of particles with vertices
    std::istream & read_vertex( std::istream &, TempParticleMap &, GenVertex * );

    /// Get a GenParticle from ASCII input
    ///
    /// TempParticleMap is used to track the associations of particles with vertices
    std::istream & read_particle( std::istream&, TempParticleMap &, GenParticle * );

    /// Write a double - for internal use by streaming IO
    inline std::ostream & output( std::ostream & os, const double& d ) {
      if ( os  ) {
        if ( d == 0. ) {
          os << ' ' << (int)0;
        } else {
          os << ' ' << d;
        }
      }
      return os;
    }

    /// Write a float - for internal use by streaming IO
    inline std::ostream & output( std::ostream & os, const float& d ) {
      if ( os  ) {
        if ( d == 0. ) {
          os << ' ' << (int)0;
        } else {
          os << ' ' << d;
        }
      }
      return os;
    }

    /// Write an int - for internal use by streaming IO
    inline std::ostream & output( std::ostream & os, const int& i ) {
      if ( os  ) {
        if ( i == 0. ) {
          os << ' ' << (int)0;
        } else {
          os << ' ' << i;
        }
      }
      return os;
    }

    /// Write a long int - for internal use by streaming IO
    inline std::ostream & output( std::ostream & os, const long& i ) {
      if ( os  ) {
        if ( i == 0. ) {
          os << ' ' << (int)0;
        } else {
          os << ' ' << i;
        }
      }
      return os;
    }

    /// Write a single char - for internal use by streaming IO
    inline std::ostream & output( std::ostream & os, const char& c ) {
      if ( os  ) {
        if ( c ) {
          os << c;
        } else {
          os << ' ' ;
        }
      }
      return os;
    }

    /// Used to read to the end of a bad event
    std::istream & find_event_end( std::istream & );

  } // detail

} // HepMC

#endif  // HEPMC_STREAM_HELPERS_H
