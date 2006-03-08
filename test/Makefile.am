# Applicable to each test program:

if USE_GMAKE
  MAKE = gmake
endif

CLHEPdir = @CLHEPdir@

INCLUDES = -I$(top_builddir) -I$(top_srcdir) -I$(CLHEPdir)/include

# libraries to link, and whence
if BUILD_STATIC
  LDADD = $(top_builddir)/src/libHepMC.a \
          $(top_builddir)/fio/libHepMCfio.a \
          $(CLHEPdir)/lib/libCLHEP.a
else
if BUILD_VISUAL
  LDADD = $(top_builddir)/src/HepMC.$(SHEXT) \
          $(top_builddir)/fio/libHepMCfio.$(SHEXT) \
          $(CLHEPdir)/lib/CLHEP.$(SHEXT)
else
  LIBS += $(MY_LD)$(top_builddir)/src \
	  $(MY_LD)$(top_builddir)/fio \
	  $(MY_LD)$(CLHEPdir)/lib
  LDADD = $(top_builddir)/src/libHepMC.$(SHEXT) \
          $(top_builddir)/fio/libHepMCfio.$(SHEXT) \
          $(CLHEPdir)/lib/libCLHEP.$(SHEXT)
endif
endif

# Identify executables needed during testing:
check_PROGRAMS = testHepMC 

#check_SCRIPTS = testHepMC.sh

# Identify test(s) to run when 'make check' is requested:
TESTS = testHepMC.sh

# Identify the test(s) for which failure is the intended outcome:
XFAIL_TESTS = 

# Identify the dependencies on a per-test basis:
testHepMC_SOURCES = testHepMC.cc

# Identify input data file(s) and prototype output file(s):
EXTRA_DIST = testHepMC.input testHepMC.dat

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = testHepMC.out
