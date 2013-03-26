
#include "GenCompress.h"

#include <iostream>

namespace HepMC {

    
GenEvent compress(GenEvent evt, compression_type ct )
{
   switch (ct) {
     case none : std::cout << "no compression" << std::endl;
     case status_1 : 
        std::cout << "select GenParticles with status 1" << std::endl;
     case status_1_2 :  
        std::cout << "select GenParticles with status 1 and 2" << std::endl;
     default  : std::cout << "no compression" << std::endl;
   } 
   return evt;
}

} // HepMC
