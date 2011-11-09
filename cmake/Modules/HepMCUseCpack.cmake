# define the environment for cpack
#
include(HepMCFindCompiler)

# note that hepmc_parse_version is used to define VERSION_MAJOR, etc.
set( CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR} )
set( CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR} )
set( CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH} )

set( CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0 )
set( CPACK_GENERATOR TGZ )

hepmc_find_compiler()

if ( ${SLTYPE} MATCHES "noarch" )
  set( PACKAGE_BASENAME ${SLTYPE} )
else ()
  if ( NOT CPack_COMPILER_STRING )
    set( PACKAGE_BASENAME ${SLTYPE}-${CMAKE_SYSTEM_PROCESSOR} )
  else ()
    set( PACKAGE_BASENAME ${SLTYPE}-${CMAKE_SYSTEM_PROCESSOR}${CPack_COMPILER_STRING} )
  endif ()
endif ()
if ( NOT qualifier )
  set( CPACK_SYSTEM_NAME ${PACKAGE_BASENAME} )
else ()
  set( CPACK_SYSTEM_NAME ${PACKAGE_BASENAME}-${qualifier} )
endif ()
# check for extra qualifiers
if( NOT  CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE_TOLOWER default )
else()
   STRING(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
   if( ${CMAKE_BUILD_TYPE_TOLOWER} MATCHES "debug")
      set(CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-debug )
   elseif( ${CMAKE_BUILD_TYPE_TOLOWER} MATCHES "release")
      set(CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-opt )
   elseif( ${CMAKE_BUILD_TYPE_TOLOWER} MATCHES "minsizerel")
      set(CPACK_SYSTEM_NAME ${CPACK_SYSTEM_NAME}-prof )
   endif()   
endif()

message(STATUS "CPACK_SYSTEM_NAME = ${CPACK_SYSTEM_NAME}" )

include(CPack)
