
set( mock_root_source 
		      IO_MockRoot.cc
		      IO_MockRoot.h
		      testMockRoot.dat
		      testMockRoot.output
		  )

CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/GNUmakefile.example.in 
                ${CMAKE_CURRENT_BINARY_DIR}/GNUmakefile  @ONLY )
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/testMockRoot.cc.in
                ${CMAKE_CURRENT_BINARY_DIR}/testMockRoot.cc  @ONLY )

INSTALL (FILES ${mock_root_source}  
               ${CMAKE_CURRENT_BINARY_DIR}/GNUmakefile
               ${CMAKE_CURRENT_BINARY_DIR}/testMockRoot.cc
         DESTINATION share/HepMC/examples/pythia8 )
