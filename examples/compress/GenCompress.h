#ifndef HEPMC_GEN_COMPRESS_H
#define HEPMC_GEN_COMPRESS_H

#include "HepMC/GenEvent.h"

namespace HepMC {

// the default compression is none
enum compression_type { none=1, status_1, status_1_2 };
    
GenEvent compress(GenEvent, compression_type ct = none );

} // HepMC

#endif  // HEPMC_GEN_COMPRESS_H
