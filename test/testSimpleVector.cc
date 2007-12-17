//
// First pass - simply exercise all the vector methods
//
#include <iostream>

#include "HepMC/SimpleVector.h"

int threeVectorMath();
int fourVectorMath();

int main() 
{
  // ThreeVector
  HepMC::ThreeVector vector3;
  HepMC::ThreeVector v3(1.1,2.2,3.3);
  HepMC::ThreeVector vx(1.34);
  
  HepMC::ThreeVector v3copy( v3 );
   
  int numbad = 0;
 
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
     std::cout << "vectors v3 and vector3 are different" << std::endl;
  }
  if( v3 != v3copy ) {
     std::cout << "vectors v3 and v3copy are different" << std::endl;
  }

  numbad += threeVectorMath();
 
  // FourVector
  HepMC::FourVector vector, v4scale1;
  HepMC::FourVector v4(1.1,2.2,3.3,4.4);
  HepMC::FourVector vt(1.34);
  
  HepMC::FourVector vectorcopy( v4 );
  vector = v4;
  
  double px = v4.px();
  double py = v4.py();
  double pz = v4.pz();
  double e  = v4.e();
   x = vectorcopy.x();
   y = vectorcopy.y();
   z = vectorcopy.z();
  double t = vectorcopy.t();
  
   p2 = v4.perp2();
   pt = v4.perp();
   l = v4.mag();
   th = v4.theta();
   ph = v4.phi();
   r = v4.rho();
  double masssq1 = v4.m2();
  double mass1 = v4.m();
  double pr1 = v4.pseudoRapidity();
  double eta1 = v4.eta();
  double masssq2 = vector.m2();
  double mass2 = vector.m();
  double pr2 = vector.pseudoRapidity();
  double eta2 = vector.eta();

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
     ++numbad;
  }
  if( py != y ) { 
     std::cout << "different Y values: " << py << " " << y << std::endl;
     ++numbad;
  }
  if( pz != z ) { 
     std::cout << "different Z values: " << pz << " " << z << std::endl;
     ++numbad;
  }
  if( e != t ) { 
     std::cout << "different E values: " << e << " " << t << std::endl;
     ++numbad;
  }
  if( masssq1 != masssq2 ) { 
     std::cout << "different mass sq values: " << masssq1 << " " << masssq2 << std::endl;
     ++numbad;
  }
  if( mass1 != mass2 ) { 
     std::cout << "different mass values: " << mass1 << " " << mass2 << std::endl;
     ++numbad;
  }
  if( pr1 != pr2 ) { 
     std::cout << "different pr values: " << pr1 << " " << pr2 << std::endl;
     ++numbad;
  }
  if( eta1 != eta2 ) { 
     std::cout << "different eta values: " << eta1 << " " << eta2 << std::endl;
     ++numbad;
  }
  if( v4 == vector ) {
  } else {
     std::cout << "vectors v and vector are different" << std::endl;
     ++numbad;
  }
  if( v4 != vectorcopy ) {
     std::cout << "vectors v and vectorcopy are different" << std::endl;
     ++numbad;
  }

  numbad += fourVectorMath();

  return numbad;
}

int threeVectorMath()
{
  HepMC::ThreeVector v3(1.1,2.2,3.3);
   
  double wt = 1.2528;
  int n3bad = 0;
  
  // scale
  HepMC::ThreeVector v3scale1 = v3;
  v3scale1 *= wt;
  HepMC::ThreeVector v3scale2(wt*v3.x(),wt*v3.y(),wt*v3.z());
  if( v3scale1 != v3scale2 ) {
     std::cout << "vectors v3scale1 and v3scale2 are different" << std::endl;
     ++n3bad;
  }

  // add
  HepMC::ThreeVector v3add1 = v3;
  v3add1 += v3scale1;
  HepMC::ThreeVector v3add2 = v3 + v3scale1;
  HepMC::ThreeVector v3add3(v3scale1.x()+v3.x(),
			   v3scale1.y()+v3.y(),
			   v3scale1.z()+v3.z());
  if( v3add1 != v3add2 ) {
     std::cout << "vectors v3add1 and v3add2 are different" << std::endl;
     ++n3bad;
  }
  if( v3add3 != v3add2 ) {
     std::cout << "vectors v3add3 and v3add2 are different" << std::endl;
     ++n3bad;
  }

  // subtract
  HepMC::ThreeVector v3sub1 = v3;
  v3sub1 -= v3scale1;
  HepMC::ThreeVector v3sub2 = v3 - v3scale1;
  HepMC::ThreeVector v3sub3(v3.x()-v3scale1.x(),
			   v3.y()-v3scale1.y(),
			   v3.z()-v3scale1.z());
  if( v3sub1 != v3sub2 ) {
     std::cout << "vectors v3sub1 and v3sub2 are different" << std::endl;
     ++n3bad;
  }
  if( v3sub3 != v3sub2 ) {
     std::cout << "vectors v3sub3 and v3sub2 are different" << std::endl;
     ++n3bad;
  }

  // minus
  HepMC::ThreeVector v3minus1 = -v3;
  HepMC::ThreeVector v3minus2(-v3.x(),-v3.y(),-v3.z());
  if( v3minus1 != v3minus2 ) {
     std::cout << "vectors v3minus1 and v3minus2 are different" << std::endl;
     ++n3bad;
  }

  return n3bad;
}

int fourVectorMath()
{
  HepMC::FourVector v4(1.1,2.2,3.3,4.4);
   
  double wt = 1.2528;
  int n4bad = 0;
  
  // scale
  HepMC::FourVector v4scale1 = v4;
  v4scale1 *= wt;
  HepMC::FourVector v4scale2(wt*v4.x(),wt*v4.y(),wt*v4.z(),wt*v4.t());
  if( v4scale1 != v4scale2 ) {
     std::cout << "vectors v4scale1 and v4scale2 are different" << std::endl;
     ++n4bad;
  }

  // add
  HepMC::FourVector v4add1 = v4;
  v4add1 += v4scale1;
  HepMC::FourVector v4add2 = v4 + v4scale1;
  HepMC::FourVector v4add3(v4scale1.x()+v4.x(),
			   v4scale1.y()+v4.y(),
			   v4scale1.z()+v4.z(),
			   v4scale1.t()+v4.t());
  if( v4add1 != v4add2 ) {
     std::cout << "vectors v4add1 and v4add2 are different" << std::endl;
     ++n4bad;
  }
  if( v4add3 != v4add2 ) {
     std::cout << "vectors v4add3 and v4add2 are different" << std::endl;
     ++n4bad;
  }

  // subtract
  HepMC::FourVector v4sub1 = v4;
  v4sub1 -= v4scale1;
  HepMC::FourVector v4sub2 = v4 - v4scale1;
  HepMC::FourVector v4sub3(v4.x()-v4scale1.x(),
			   v4.y()-v4scale1.y(),
			   v4.z()-v4scale1.z(),
			   v4.t()-v4scale1.t());
  if( v4sub1 != v4sub2 ) {
     std::cout << "vectors v4sub1 and v4sub2 are different" << std::endl;
     ++n4bad;
  }
  if( v4sub3 != v4sub2 ) {
     std::cout << "vectors v4sub3 and v4sub2 are different" << std::endl;
     ++n4bad;
  }

  // minus
  HepMC::FourVector v4minus1 = -v4;
  HepMC::FourVector v4minus2(-v4.x(),-v4.y(),-v4.z(),-v4.t());
  if( v4minus1 != v4minus2 ) {
     std::cout << "vectors v4minus1 and v4minus2 are different" << std::endl;
     ++n4bad;
  }

  return n4bad;
}
