# parse a version string and set the cmake project subversions
#
# hepmc_parse_version(  )

macro( hepmc_parse_version )

  STRING( REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+\\.[0-9]+.*" "\\1" VERSION_MAJOR "${version}" )
  STRING( REGEX REPLACE "^[0-9]+\\.([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" VERSION_MINOR "${version}" )
  STRING( REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)\\.[0-9]+.*" "\\1" VERSION_PATCH "${version}" )
  message(STATUS "${project} version ${version} parses to ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_PATCH}" )

endmacro( hepmc_parse_version )
