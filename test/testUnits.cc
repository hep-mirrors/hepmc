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
    HepMC::MomentumUnits myGUnits(HepMC::MomentumUnits::GEV);
    HepMC::MomentumUnits myMUnits = HepMC::MomentumUnits::MEV;
    
    os << "Unit enum: "  << std::setw(12) << myUnits.units() 
              << " Unit name: " << myUnits.name() 
              << std::endl;
    os << "Unit enum: " << std::setw(12) << myMUnits.units() 
              << " Unit name: " << myMUnits.name() 
              << std::endl;
    if( myUnits.set_units( HepMC::MomentumUnits::MEV ) ) {
        os << "myUnits has been changed" << std::endl;
	os << "Unit enum: " << std::setw(12) << myUnits.units() 
        	  << " Unit name: " << myUnits.name() 
        	  << std::endl;
    } else {
       std::cerr << "ERROR: myUnits has NOT been changed" << std::endl;
       return -1;
    }
    if( myMUnits.set_units( HepMC::MomentumUnits::GEV ) ) {
       std::cerr << "ERROR: myMUnits has been changed" << std::endl;
       std::cerr << "Unit enum: " << std::setw(12) << myMUnits.units() 
        	 << " Unit name: " << myMUnits.name() 
        	 << std::endl;
       return -2;
    } else {
       os << "myMUnits has NOT been changed" << std::endl;
    }
    
    double cf = myGUnits.conversion_factor(HepMC::MomentumUnits::MEV);
    if( cf == 1000. ) {
       os << "convert from GeV to MeV: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf 
                 << " expected 1000." << std::endl;
       return -3;
    }
    cf = myUnits.conversion_factor(HepMC::MomentumUnits::GEV);
    if( (cf - 0.001) < 1.e-20 ) {
	os << "convert from MeV to GeV: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf 
                 << " expected 0.001" << std::endl;
       return -4;
    }
    // assign back to UNKNOWN
    myUnits = HepMC::MomentumUnits::UNKNOWN;
    cf = myUnits.conversion_factor(HepMC::MomentumUnits::MEV);
    if( cf == 0. ) {
	os << "conversion is undefined: " << cf << std::endl;
    } else {
	std::cerr << "ERROR: incorrect conversion factor " << cf 
                  << " expected 0." << std::endl;
	return -5;
    }
    // convert to the same units
    cf = myGUnits.conversion_factor(HepMC::MomentumUnits::GEV);
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
    st = "MEV";
    if( myUnits.set_units(st) ) {
        os << "myUnits has been changed" << std::endl;
	os << "Unit enum: " << std::setw(12) << myUnits.units() 
        	  << " Unit name: " << myUnits.name() 
        	  << std::endl;
    } else {
        std::cerr << "ERROR: setting units to MEV should succeed" << std::endl;
    }

    return 0;
}

int testPositionUnits( std::ostream& os )
{
    // use the default constructor
    HepMC::PositionUnits myUnits;
    // two different but equivalent construction methods
    HepMC::PositionUnits cmUnits(HepMC::PositionUnits::CM);
    HepMC::PositionUnits mmUnits = HepMC::PositionUnits::MM;
    
    os << "Unit enum: "  << std::setw(12) << myUnits.units() 
              << " Unit name: " << myUnits.name() 
              << std::endl;
    os << "Unit enum: " << std::setw(12) << mmUnits.units() 
              << " Unit name: " << mmUnits.name() 
              << std::endl;
    if( myUnits.set_units( HepMC::PositionUnits::MM ) ) {
        os << "myUnits has been changed" << std::endl;
	os << "Unit enum: " << std::setw(12) << myUnits.units() 
        	  << " Unit name: " << myUnits.name() 
        	  << std::endl;
    } else {
       std::cerr << "ERROR: myUnits has NOT been changed" << std::endl;
       return -1;
    }
    if( mmUnits.set_units( HepMC::PositionUnits::CM ) ) {
       std::cerr << "ERROR: mmUnits has been changed" << std::endl;
       std::cerr << "Unit enum: " << std::setw(12) << mmUnits.units() 
        	 << " Unit name: " << mmUnits.name() 
        	 << std::endl;
       return -2;
    } else {
       os << "mmUnits has NOT been changed" << std::endl;
    }
    
    double cf = cmUnits.conversion_factor(HepMC::PositionUnits::MM);
    if( cf == 10. ) {
       os << "convert from cm to mm: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf << std::endl;
       return -3;
    }
    cf = myUnits.conversion_factor(HepMC::PositionUnits::CM);
    if( (cf - 0.1) < 1.e-20  ) {
	os << "convert from mm to cm: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf 
	         << " expected 0.1" << std::endl;
       return -4;
    }
    // assign back to UNKNOWN
    myUnits = HepMC::PositionUnits::UNKNOWN;
    cf = myUnits.conversion_factor(HepMC::PositionUnits::MM);
    if( cf == 0. ) {
	os << "conversion is undefined: " << cf << std::endl;
    } else {
       std::cerr << "ERROR: incorrect conversion factor " << cf << std::endl;
       return -5;
    }
    // convert to the same units
    cf = cmUnits.conversion_factor(HepMC::PositionUnits::CM);
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
    st = "MM";
    if( myUnits.set_units(st) ) {
        os << "myUnits has been changed" << std::endl;
	os << "Unit enum: " << std::setw(12) << myUnits.units() 
        	  << " Unit name: " << myUnits.name() 
        	  << std::endl;
    } else {
        std::cerr << "ERROR: setting units to MM should succeed" << std::endl;
    }
    
    return 0;
}
