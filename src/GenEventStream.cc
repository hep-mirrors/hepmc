
#include <iostream>

#include "HepMC/GenEvent.h"
#include "HepMC/Version.h"

namespace HepMC {

void GenEvent::write_units( std::ostream & os ) const {} 
void GenEvent::write_cross_section( std::ostream& ostr ) const {}

std::ostream& GenEvent::write( std::ostream& os ) {
  return os;
}
std::istream& GenEvent::read( std::istream& is ) {
  return is;
}

void GenEvent::print( std::ostream& ostr ) const {}
void GenEvent::print_version( std::ostream& ostr ) const {}

// ----------- internal I/O methods ------------------
std::ostream & GenEvent::write_beam_particles( std::ostream& os, 
                   std::pair<HepMC::GenParticle *,HepMC::GenParticle *> ) {
  return os;
}

std::ostream & GenEvent::write_vertex( std::ostream& os, GenVertex const * ) {
  return os;
}
std::ostream & GenEvent::write_particle( std::ostream& os, GenParticle const * ) {
  return os;
}
std::istream & GenEvent::find_file_type( std::istream& is ) {
  return is;
}
std::istream & GenEvent::find_end_key( std::istream& is, int & ) {
  return is;
}
std::istream & GenEvent::read_units( std::istream& is ) {
  return is;
}
std::istream & GenEvent::read_weight_names( std::istream& is ) {
  return is;
}
std::istream & GenEvent::process_event_line( std::istream& is, 
                                             int &, int &, int &, int & ) {
  return is;
}


} // HepMC
