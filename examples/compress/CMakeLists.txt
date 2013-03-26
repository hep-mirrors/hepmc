# we build a library and executable here

set(srcdir ${CMAKE_CURRENT_SOURCE_DIR}/../../test )

set ( compress_source CleverStream.cc
                      GenCompress.cc
		      )

link_libraries( HepMC )

ADD_LIBRARY (HepMCcomp SHARED ${compress_source})
SET_TARGET_PROPERTIES (HepMCcomp  PROPERTIES VERSION 4.0.0 SOVERSION 4 )

INSTALL (TARGETS HepMCcomp
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    ) 

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/testHepMCcomp.cc.in
                ${CMAKE_CURRENT_BINARY_DIR}/testHepMCcomp.cc  @ONLY )
ADD_EXECUTABLE(testHepMCcomp ${CMAKE_CURRENT_BINARY_DIR}/testHepMCcomp.cc)

