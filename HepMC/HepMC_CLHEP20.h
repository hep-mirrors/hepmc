//--------------------------------------------------------------------------
#ifndef HEPMC_CLHEP20_H
#define HEPMC_CLHEP20_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, April 2006
//
// allow HepMC to work with CLHEP 2.0 without changing HepMC
// This is a courtesy header that is probably not needed in HepMC 2.x
//////////////////////////////////////////////////////////////////////////

//using namespace CLHEP;
typedef CLHEP::HepLorentzVector HepLorentzVector;

//using namespace HepGeom;
typedef HepGeom::Normal3D<double> HepNormal3D;
typedef HepGeom::Point3D<double> HepPoint3D;

#endif  // HEPMC_CLHEP20_H
//--------------------------------------------------------------------------
