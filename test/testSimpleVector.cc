//
// First pass - simply exercise all the vector methods
//
#include <iostream>

#include "HepMC/SimpleVector.h"

int main() 
{
  // ThreeVector
  HepMC::ThreeVector vector3;
  HepMC::ThreeVector v3(1.1,2.2,3.3);
  HepMC::ThreeVector vx(1.34);
  
  HepMC::ThreeVector v3copy( v3 );
  
  double x = v3.x();
  double y = v3.y();
  double z = v3.z();
  double p2 = v3.perp2();
  double pt = v3.perp();
  double l = v3.mag();
  double r = v3.r();
  double th = v3.theta();
  double ph = v3.phi();

  vx.set(1., 2., 3.);
  vx.setX(1.1);
  vx.setY(2.3);
  vx.setZ(4.4);
  vx.setPhi(0.12);
  vx.setTheta(0.54);
  
  vector3 = v3;

  if( v3 == vector3 ) {
  } else {
     std::cout << "vectors are different"<< std::endl;
  }
  if( v3 != v3copy ) {
     std::cout << "vectors are different " << std::endl;
  }
 
  // FourVector
  HepMC::FourVector vector;
  HepMC::FourVector v(1.1,2.2,3.3,4.4);
  HepMC::FourVector vt(1.34);
  
  HepMC::FourVector vectorcopy( v );
  vector = v;
  
  double px = v.px();
  double py = v.py();
  double pz = v.pz();
  double e  = v.e();
   x = vectorcopy.x();
   y = vectorcopy.y();
   z = vectorcopy.z();
  double t = vectorcopy.t();
  
  double masssq = v.m2();
  double mass = v.m();
   p2 = v.perp2();
   pt = v.perp();
   l = v.mag();
   th = v.theta();
   ph = v.phi();
   r = v.rho();
  double pr = v.pseudoRapidity();
  double eta = v.eta();

  vt.set(1., 2., 3., 5.5);
  vt.setX(1.1);
  vt.setY(2.3);
  vt.setZ(4.4);
  vt.setT(6.5);
  vt.setPx(3.1);
  vt.setPy(2.2);
  vt.setPz(-1.1);
  vt.setE(5.4);

  if( px != x ) { 
     std::cout << "different X values: " << px << " " << x << std::endl;
  }
  if( py != y ) { 
     std::cout << "different Y values: " << py << " " << y << std::endl;
  }
  if( pz != z ) { 
     std::cout << "different Z values: " << pz << " " << z << std::endl;
  }
  if( e != t ) { 
     std::cout << "different E values: " << e << " " << t << std::endl;
  }
  if( v == vector ) {
  } else {
     std::cout << "vectors are different" << std::endl;
  }
  if( v != vectorcopy ) {
     std::cout << "vectors are different" << std::endl;
  }
  return 0;
}
