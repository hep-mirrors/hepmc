#ifndef HEPMC_IO_EXCEPTION_H
#define HEPMC_IO_EXCEPTION_H
// ----------------------------------------------------------------------
//
// IO_Exception.h
// Author:  Lynn Garren
//
// ----------------------------------------------------------------------

#include <stdexcept>
 
namespace HepMC {

class IO_Exception : public std::runtime_error {
public:
  IO_Exception(const std::string & msg) 
  : std::runtime_error(msg) { }
};

}	// namespace HepMC

#endif // HEPMC_IO_EXCEPTION_H
