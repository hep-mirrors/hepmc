
set( example_code 
		  example_BuildEventFromScratch.cc
		    )

CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/GNUmakefile.example.in 
                ${CMAKE_CURRENT_BINARY_DIR}/GNUmakefile  @ONLY )

INSTALL (FILES ${example_code}  
               ${CMAKE_CURRENT_BINARY_DIR}/GNUmakefile
         DESTINATION share/HepMC/examples )
