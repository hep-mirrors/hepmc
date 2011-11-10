#------------------------------------------------------------------------------
# Top Level CMakeLists.txt for HepMC
#  cmake -Dmomentum:STRING=[MEV|GEV] -Dlength:STRING=[MM|CM]
#        [-DCMAKE_INSTALL_PREFIX=/install/path] 
#        [-DCMAKE_BUILD_TYPE=Debug|Release|RelWithDebInfo|MinSizeRel]
#        [-Dbuild_docs:BOOL=ON] 
#        /path/to/source
#  make
#  make test
#  make install
#
# The default HepMC build type is CMAKE_BUILD_TYPE=RelWithDebInfo,
# which matches the default HepMC autoconf flags
#------------------------------------------------------------------------------

# use cmake 2.6 or later
cmake_minimum_required (VERSION 2.6)

# project name and version
project(HepMC)
set( version 2.06.05 )

if ( momentum )
  set( HEPMC_DEFAULT_MOM_UNIT ${momentum} )
else()
  message(FATAL_ERROR "You must specify the momentum units with -Dmomentum:STRING=[MEV|GEV]")
endif()

if ( length )
  set( HEPMC_DEFAULT_LEN_UNIT ${length} )
else()
  message(FATAL_ERROR "You must specify the length units with -Dlength:STRING=[MM|CM]")
endif()

# find the HepMC cmake modules
#set(CMAKE_MODULE_PATH "${CMAKE_ROOT}/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH})
message(STATUS "module path is ${CMAKE_MODULE_PATH}" )

# make sure we are not building from within the source code directory
include(HepMCOutOfSourceBuild)
hepmc_ensure_out_of_source_build()

# build_docs is OFF (false) by default
if ( build_docs )
   message(STATUS "documents WILL be built and installed" )
else()
   message(STATUS "documents WILL NOT be installed" )
endif()

# various handy macros
include(HepMCVariables)
include(HepMCParseVersion)

include(CMakeDetermineFortranCompiler)

# because we want to move these libraries about,
# do not embed full path in shared libraries or executables
set (CMAKE_SKIP_RPATH)

ENABLE_TESTING()

# include search path
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")

#build all libraries in a single directory to facilitate testing
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# set our preferred compiler flags and other variables
hepmc_parse_version()
hepmc_set_compiler_flags()

add_subdirectory(HepMC) 
add_subdirectory(src) 
add_subdirectory(fio) 
add_subdirectory(test) 
add_subdirectory(examples) 
add_subdirectory(doc)

# Packaging utility
include(HepMCUseCpack)

