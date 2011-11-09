# determine the compiler and version
# this code is more or less lifted from FindBoost

#-------------------------------------------------------------------------------

#
# Runs compiler with "-dumpversion" and parses major/minor
# version with a regex.
#
FUNCTION(_My_COMPILER_DUMPVERSION _OUTPUT_VERSION)

  EXEC_PROGRAM(${CMAKE_CXX_COMPILER}
    ARGS ${CMAKE_CXX_COMPILER_ARG1} -dumpversion
    OUTPUT_VARIABLE _my_COMPILER_VERSION
  )
  set( COMPILER_VERSION ${_my_COMPILER_VERSION} PARENT_SCOPE)
  STRING(REGEX REPLACE "([0-9])\\.([0-9])(\\.[0-9])?" "\\1\\2"
    _my_COMPILER_VERSION ${_my_COMPILER_VERSION})

  SET(${_OUTPUT_VERSION} ${_my_COMPILER_VERSION} PARENT_SCOPE)
ENDFUNCTION()

#
# End functions/macros
#
#-------------------------------------------------------------------------------


macro( hepmc_find_compiler )
  if (My_COMPILER)
      SET (CPack_COMPILER_STRING ${My_COMPILER})
      message(STATUS "[ ${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE} ] "
                     "using user-specified My_COMPILER = ${CPack_COMPILER_STRING}")
  else(My_COMPILER)
    # Attempt to guess the compiler suffix
    # NOTE: this is not perfect yet, if you experience any issues
    # please report them and use the My_COMPILER variable
    # to work around the problems.
    if (MSVC90)
      SET (CPack_COMPILER_STRING "-vc90")
    elseif (MSVC80)
      SET (CPack_COMPILER_STRING "-vc80")
    elseif (MSVC71)
      SET (CPack_COMPILER_STRING "-vc71")
    elseif (MSVC70) # Good luck!
      SET (CPack_COMPILER_STRING "-vc7") # yes, this is correct
    elseif (MSVC60) # Good luck!
      SET (CPack_COMPILER_STRING "-vc6") # yes, this is correct
    elseif (BORLAND)
      SET (CPack_COMPILER_STRING "-bcb")
    elseif("${CMAKE_CXX_COMPILER}" MATCHES "icl"
        OR "${CMAKE_CXX_COMPILER}" MATCHES "icpc")
      if(WIN32)
        set (CPack_COMPILER_STRING "-iw")
      else()
        set (CPack_COMPILER_STRING "-il")
      endif()
    elseif (MINGW)
        _My_COMPILER_DUMPVERSION(CPack_COMPILER_STRING_VERSION)
        SET (CPack_COMPILER_STRING "-mgw${CPack_COMPILER_STRING_VERSION}")
    elseif (UNIX)
      if (CMAKE_COMPILER_IS_GNUCXX)
          _My_COMPILER_DUMPVERSION(CPack_COMPILER_STRING_VERSION)
          # Determine which version of GCC we have.
	  if(APPLE)
              SET (CPack_COMPILER_STRING "-xgcc${CPack_COMPILER_STRING_VERSION}")
	  else()
              SET (CPack_COMPILER_STRING "-gcc${CPack_COMPILER_STRING_VERSION}")
	  endif()
      endif (CMAKE_COMPILER_IS_GNUCXX)
    endif()
    message(STATUS "Using compiler ${CPack_COMPILER_STRING}")
  endif(My_COMPILER)
endmacro( hepmc_find_compiler )


macro( compiler_status )
    hepmc_find_compiler()
    message(STATUS " ")
    message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
    message(STATUS "Compiler version: ${COMPILER_VERSION}")
    message(STATUS "Compiler string for cpack: ${CPack_COMPILER_STRING}")
    message(STATUS " ")
endmacro( compiler_status )
