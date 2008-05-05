# ----------------------------------------------------------------------
# This is the HepMC configuration input file
# Process this file with autoconf to produce a configure script.
# ----------------------------------------------------------------------

AC_PREREQ(2.59)
AC_INIT([HepMC],[2.04.00.beta],[https://savannah.cern.ch/projects/hepmc/],[HepMC])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE(1.9 foreign)
AC_CONFIG_SRCDIR([src/CommonIO.cc])
AC_CONFIG_HEADER([HepMC/defs.h])

# ----------------------------------------------------------------------
# define --enable-visual
# ----------------------------------------------------------------------
AC_ARG_ENABLE(visual,
   AC_HELP_STRING([--enable-visual],[on by default when using Visual C++]),
        [case "${enableval}" in
          yes) build_visual="yes";;
          no)  build_visual="no";;
          *)   build_visual="no";;
        esac],
        [build_visual="no"])

# ----------------------------------------------------------------------
# find CLHEP and/or GENSER:
# these are used ONLY in the examples
# ----------------------------------------------------------------------

AC_ARG_WITH(CLHEP,
   AC_HELP_STRING([--with-CLHEP],[--with-CLHEP=$CLHEP_DIR: CLHEP is used in the examples]),
   [CLHEPdir=${withval}],[CLHEPdir=" "])
AC_SUBST(CLHEPdir)

AC_ARG_WITH(GENSER,
   AC_HELP_STRING([--with-GENSER],[--with-GENSER=$GENSER_DIR: GENSER is used in the examples]),
   [GENSERdir=${withval}],[GENSERdir=" "])
AC_SUBST(GENSERdir)

# ----------------------------------------------------------------------
# Checks for programs.
# ----------------------------------------------------------------------
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET

# special libtool rules for Windows - work with gcc and cygwin
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# ----------------------------------------------------------------------
# Checks for libraries.
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
# Checks for header files.
# ----------------------------------------------------------------------
AC_CHECK_HEADERS([stdint.h])

# ----------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# ----------------------------------------------------------------------
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_CHECK_TYPES([ptrdiff_t])

# ----------------------------------------------------------------------
# Checks for library functions.
# ----------------------------------------------------------------------
AC_HEADER_STDC

# ----------------------------------------------------------------------
# OS dependent differences:
# ----------------------------------------------------------------------

# copy
case "$target" in
*-*-win32*)
  COPY_P="copy -p"
  ;;
*)
  COPY_P="cp -p"
esac

# diff
DIFF_Q="diff -q -b"

AC_SUBST(COPY_P)
AC_SUBST(DIFF_Q)

# ----------------------------------------------------------------------
# compiler dependent differences
# notice that the default is presumed to be some flavor of g++
# ----------------------------------------------------------------------
case "$CXX" in
cl) 
  AM_CXXFLAGS="-EHsc -nologo -GR -MD"
  AM_MAKEFLAGS="-f VCMakefile"
  AC_SUBST(AM_MAKEFLAGS)
  CXXFLAGS=" "
  AC_SUBST(CXXFLAGS)
  build_visual="yes"
  ;;
*)  
  AM_CXXFLAGS="-ansi -pedantic -Wall"
esac

AC_SUBST(AM_CXXFLAGS)

AM_CONDITIONAL(BUILD_VISUAL, test x$build_visual = xyes)

# ----------------------------------------------------------------------
# process Makefile.in and other *.in files
# ----------------------------------------------------------------------
AC_CONFIG_FILES([Makefile
		 VCMakefile
		 HepMC/Makefile
		 HepMC/VCMakefile
                 doc/Makefile
                 doc/VCMakefile
                 examples/Makefile
                 examples/VCMakefile
                 fio/Makefile
                 fio/VCMakefile
                 src/Makefile
                 src/VCMakefile
                 test/Makefile
                 test/VCMakefile
                 test/testHepMC.cc
                 test/testMass.cc
                 test/testHepMCIteration.cc
                 test/testMultipleCopies.cc
                 test/testDeprecated.cc
                 examples/GNUmakefile.example])

AC_CONFIG_FILES([test/testHepMC.sh], [chmod +x test/testHepMC.sh])
AC_CONFIG_FILES([test/testMass.sh], [chmod +x test/testMass.sh])
AC_CONFIG_FILES([test/testHepMCIteration.sh], [chmod +x test/testHepMCIteration.sh])
AC_CONFIG_FILES([test/testPrintBug.sh], [chmod +x test/testPrintBug.sh])
AC_CONFIG_FILES([test/testDeprecated.sh], [chmod +x test/testDeprecated.sh])

# ----------------------------------------------------------------------
# Finish up:
# ----------------------------------------------------------------------
AC_OUTPUT