
include(HepMCBuildTest)

set( HepMC_tests 
		 testHepMCIteration
		 testMass
		 testFlow
                 testPrintBug
		 testPolarization )
set( HepMC_simple_tests testSimpleVector 
                	testUnits
			testMultipleCopies 
			testWeights )

# automake/autoconf variables for *.cc.in 
set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )

FILE( GLOB process_source *.cc.in )
foreach( sfile ${process_source} )
  get_filename_component(basename ${sfile} NAME_WE )
  configure_file( ${sfile}
                  ${CMAKE_CURRENT_BINARY_DIR}/${basename}.cc  @ONLY )
endforeach( sfile ${process_source} )

hepmc_test( testHepMC testHepMCMethods.cc )
hepmc_test( testStreamIO testHepMCMethods.cc )

foreach ( test ${HepMC_tests} )
  hepmc_test( ${test} )
endforeach ( test ${HepMC_tests} )

foreach ( test ${HepMC_simple_tests} )
  hepmc_simple_test( ${test} )
endforeach ( test ${HepMC_simple_tests} )
