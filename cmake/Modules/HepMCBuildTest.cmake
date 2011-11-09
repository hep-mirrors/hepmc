# Create the tests
#
# if using a shell script:
#  hepmc_test( testname )
#   where testname is the base name of the .cc and .sh.in file
#  hepmc_simple_test( testname )
#   where there is no .sh - just an executable
#

macro( hepmc_test testname )

  message( STATUS "building ${testname} for ${testname}.sh" )

  link_libraries( HepMC )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  ADD_EXECUTABLE(${testname} ${${testname}_source} ${ARGN} )
 
  if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
    message( STATUS "skipping ${testname}.sh on ${CMAKE_SYSTEM_NAME}")
  else()
    # automake/autoconf variables for ${testname}.sh.in 
    set(srcdir ${CMAKE_CURRENT_SOURCE_DIR} )
    set(builddir ${CMAKE_CURRENT_BINARY_DIR} )
    configure_file( ${testname}.sh.in 
                    ${CMAKE_CURRENT_BINARY_DIR}/${testname}.sh  @ONLY )
    add_test( ${testname}.sh ${CMAKE_CURRENT_BINARY_DIR}/${testname}.sh )
  endif()

endmacro( hepmc_test )

macro( hepmc_simple_test testname )

  message( STATUS "building simple test ${testname} " )

  # include test in search path
  include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")

  link_libraries( HepMC )
  find_file( ${testname}_source ${testname}.cc ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )
  #message(STATUS "building simple test ${testname} from ${${testname}_source}")
  ADD_EXECUTABLE(${testname} ${${testname}_source} ${ARGN} )

  add_test( ${testname} ${testname} )

endmacro( hepmc_simple_test )
