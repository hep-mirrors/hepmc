
set ( fio_source_list    
			 HEPEVT_Wrapper.cc
			 HerwigWrapper.cc
			 IO_HEPEVT.cc
			 IO_HERWIG.cc
			  )

# this library is meant to interact with Fortran and needs extra flags
    if(APPLE)
      set( CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS 
           "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -Wl,-undefined -Wl,dynamic_lookup -Wl,-single_module")
    endif(APPLE)

ADD_LIBRARY (HepMCfio  SHARED ${fio_source_list})
ADD_LIBRARY (HepMCfioS STATIC ${fio_source_list})
SET_TARGET_PROPERTIES (HepMCfio  PROPERTIES OUTPUT_NAME HepMCfio )
SET_TARGET_PROPERTIES (HepMCfio  PROPERTIES VERSION 4.0.0 SOVERSION 4 )
SET_TARGET_PROPERTIES (HepMCfioS PROPERTIES OUTPUT_NAME HepMCfio )
SET_TARGET_PROPERTIES(HepMCfio  PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(HepMCfioS PROPERTIES CLEAN_DIRECT_OUTPUT 1)

INSTALL (TARGETS HepMCfio HepMCfioS
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    ) 
