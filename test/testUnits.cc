//
// Test MomentumUnits and PositionUnits
//
#include <fstream>
#include <iostream>
#include <iomanip>

#include "HepMC/MomentumUnits.h"
#include "HepMC/PositionUnits.h"

int testMomentumUnits( std::ostream& );
int testPositionUnits( std::ostream& );

int main()
{
  // define an output stream
  std::ofstream os( "testUnits.out" );
  
  int err = 0;
  err = testMomentumUnits(os);
  if( err != 0 ) return err;
  err = testPositionUnits(os);
  return err;
}

int testMomentumUnits( std::ostream& os )
{
    // use the default constructor
    HepMC::MomentumUnits myUnits;
    // two different but equivalent construction methods
    HepMC::MomentumUnits myGUnits(HepMC::MomentumUnits::GeV);
    HepMC::MomentumUnits myTUnits = HepMC::MomentumUnits::TeV;
    
    os << "Unit enum: "  << std::setw(12) << myUnits.units() 
              << " Unit name: " << myUnits.name() 
              << std::endl;
    os << "Unit enum: " << std::setw(12) << myTUnits.units() 
              << " Unit name: " << myTUnits.name() 
              << std::endl;
    if( myUnits.set_units( HepMC::MomentumUnits::MeV ) ) {
        os << "myUnits has been changed" << std::endl;
	os << "Unit enum: " << std::setw(12) << myUnits.units() 
        	  << " Unit name: " << myUnits.name() 
        	  << std::endl;
    } else {
       std::cerr << "ERROR: myUnits has NOT been changed" << std::endl;
       return -1;
    }
    if( myTUnits.set_units( HepMC::MomentumUnits::MeV ) ) {
       std::cerr << "ERROR: myTUnits has been changed" << std::endl;
       std::cerr << "Unit enum: " << std::setw(12) << myTUnits.units() 
        	 << " Unit name: " << myTUnits.name() 
        	 << std::endl;
       return -2;
    } else {
       os << "myTUnits has NOT been changed" << std::endl;
    }
    
    double cf = myGUnits.conversion_factor(HepMC::MomentumUnits::MeV);
    if( cf == 1000. ) {
       os << "convert from GeV to MeV: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf 
                 << " expected 1000." << std::endl;
       return -3;
    }
    cf = myUnits.conversion_factor(HepMC::MomentumUnits::TeV);
    if( (cf - 0.000001) < 1.e-20 ) {
	os << "convert from MeV to TeV: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf 
                 << " expected 0.000001" << std::endl;
       return -4;
    }
    // assign back to unknown
    myUnits = HepMC::MomentumUnits::unknown;
    cf = myUnits.conversion_factor(HepMC::MomentumUnits::MeV);
    if( cf == 0. ) {
	os << "conversion is undefined: " << cf << std::endl;
    } else {
	std::cerr << "ERROR: incorrect conversion factor " << cf 
                  << " expected 0." << std::endl;
	return -5;
    }
    // convert to the same units
    cf = myGUnits.conversion_factor(HepMC::MomentumUnits::GeV);
    if( cf == 1. ) {
	 os << "convert GeV to GeV: " << cf << std::endl;
    } else {
        std::cerr << "ERROR: incorrect conversion factor " << cf 
	          << " expected 1." << std::endl;
        return -6;
    }
    
    // pass some strings
    std::string st = "junk";
    if( myUnits.set_units(st) ) {
        std::cerr << "ERROR: setting units to junk should fail" << std::endl;
    }
    st = "MeV";
    if( myUnits.set_units(st) ) {
        os << "myUnits has been changed" << std::endl;
	os << "Unit enum: " << std::setw(12) << myUnits.units() 
        	  << " Unit name: " << myUnits.name() 
        	  << std::endl;
    } else {
        std::cerr << "ERROR: setting units to MeV should succeed" << std::endl;
    }

    return 0;
}

int testPositionUnits( std::ostream& os )
{
    // use the default constructor
    HepMC::PositionUnits myUnits;
    // two different but equivalent construction methods
    HepMC::PositionUnits cmUnits(HepMC::PositionUnits::cm);
    HepMC::PositionUnits mUnits = HepMC::PositionUnits::meter;
    
    os << "Unit enum: "  << std::setw(12) << myUnits.units() 
              << " Unit name: " << myUnits.name() 
              << std::endl;
    os << "Unit enum: " << std::setw(12) << mUnits.units() 
              << " Unit name: " << mUnits.name() 
              << std::endl;
    if( myUnits.set_units( HepMC::PositionUnits::mm ) ) {
        os << "myUnits has been changed" << std::endl;
	os << "Unit enum: " << std::setw(12) << myUnits.units() 
        	  << " Unit name: " << myUnits.name() 
        	  << std::endl;
    } else {
       std::cerr << "ERROR: myUnits has NOT been changed" << std::endl;
       return -1;
    }
    if( mUnits.set_units( HepMC::PositionUnits::mm ) ) {
       std::cerr << "ERROR: mUnits has been changed" << std::endl;
       std::cerr << "Unit enum: " << std::setw(12) << mUnits.units() 
        	 << " Unit name: " << mUnits.name() 
        	 << std::endl;
       return -2;
    } else {
       os << "mUnits has NOT been changed" << std::endl;
    }
    
    double cf = cmUnits.conversion_factor(HepMC::PositionUnits::mm);
    if( cf == 10. ) {
       os << "convert from cm to mm: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf << std::endl;
       return -3;
    }
    cf = myUnits.conversion_factor(HepMC::PositionUnits::meter);
    if( (cf - 0.001) < 1.e-20  ) {
	os << "convert from mm to m: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf << std::endl;
       return -4;
    }
    // assign back to unknown
    myUnits = HepMC::PositionUnits::unknown;
    cf = myUnits.conversion_factor(HepMC::PositionUnits::mm);
    if( cf == 0. ) {
	os << "conversion is undefined: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf << std::endl;
       return -5;
    }
    // convert to the same units
    cf = cmUnits.conversion_factor(HepMC::PositionUnits::cm);
    if( cf == 1. ) {
	os << "convert cm to cm: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf << std::endl;
       return -6;
    }
    // pass some strings
    std::string st = "junk";
    if( myUnits.set_units(st) ) {
        std::cerr << "ERROR: setting units to junk should fail" << std::endl;
    }
    st = "mm";
    if( myUnits.set_units(st) ) {
        os << "myUnits has been changed" << std::endl;
	os << "Unit enum: " << std::setw(12) << myUnits.units() 
        	  << " Unit name: " << myUnits.name() 
        	  << std::endl;
    } else {
        std::cerr << "ERROR: setting units to mm should succeed" << std::endl;
    }
    
    return 0;
}
