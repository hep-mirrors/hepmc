#
# hepmc_parse_version()
# get the components of ${version}

macro( hepmc_parse_version )
  ##message(STATUS "hepmc_parse_version: parsing ${project} version ${version}" )
  STRING( REGEX REPLACE "^([0-9]+)[.][0-9]+[.][0-9]+.*" "\\1" VERSION_MAJOR "${version}" )
  STRING( REGEX REPLACE "^[0-9]+[.]([0-9]+)[.][0-9]+.*" "\\1" VERSION_MINOR "${version}" )
  STRING( REGEX REPLACE "^[0-9]+[.][0-9]+[.]([0-9]+)+.*" "\\1" VERSION_PATCH "${version}" )
  STRING( REGEX REPLACE "^[0-9]+[.][0-9]+[.][0-9]+[.](.*)" "\\1" VERSION_MICRO "${version}" )
  ##message(STATUS "hepmc_parse_version: ${project} version ${version} parses to ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_PATCH} ${VERSION_MICRO}" )
endmacro( hepmc_parse_version )
