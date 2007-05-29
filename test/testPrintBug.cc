//
// Thanks to Bob McElrath and Frank Siegert for this test
//

#include <fstream>

#include "HepMC/GenEvent.h"
#include "HepMC/SimpleVector.h"

int main(int argc,char* argv[]) 
{
  HepMC::GenEvent* p_event;
  
  p_event = new HepMC::GenEvent();

  // define an output stream
  std::ofstream os( "testPrintBug.out" );
  
  for(int i=0; i<10; i++) {
    HepMC::FourVector vector(1.0,1.0,1.0,1.0);
    HepMC::GenVertex* vertex = new HepMC::GenVertex(vector,i);
    for(int j=0; j<3; j++) {
      HepMC::GenParticle* particle = new HepMC::GenParticle(vector,1,2);
      vertex->add_particle_in(particle);
    }
    for(int j=0; j<3; j++) {
      HepMC::GenParticle* particle = new HepMC::GenParticle(vector,1,2);
      vertex->add_particle_out(particle);
    }
    p_event->add_vertex(vertex);
  }
  p_event->print(os);
  // cleanup
  delete p_event;
  return 0;
}
