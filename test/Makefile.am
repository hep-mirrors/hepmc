# Applicable to each test program:

if USE_GMAKE
  MAKE = gmake
endif

INCLUDES = -I$(top_builddir) -I$(top_srcdir) 

# libraries to link, and whence
if BUILD_STATIC
  LDADD = $(top_builddir)/src/libHepMC.a \
          $(top_builddir)/fio/libHepMCfio.a 
else
if BUILD_VISUAL
  LDADD = $(top_builddir)/src/HepMC.$(SHEXT) \
          $(top_builddir)/fio/HepMCfio.$(SHEXT) 
else
  LIBS += $(MY_LD)$(top_builddir)/src \
	  $(MY_LD)$(top_builddir)/fio 
  LDADD = $(top_builddir)/src/libHepMC.$(SHEXT) \
          $(top_builddir)/fio/libHepMCfio.$(SHEXT) 
endif
endif

# Identify executables needed during testing:
check_PROGRAMS = testHepMC testHepMCIteration testMass \
                 testPrintBug testSimpleVector

check_SCRIPTS = testHepMC.sh testHepMCIteration.sh testPrintBug.sh testMass.sh

# Identify test(s) to run when 'make check' is requested:
TESTS = testHepMC.sh testHepMCIteration.sh testMass.sh \
        testPrintBug.sh testSimpleVector

# Identify the test(s) for which failure is the intended outcome:
XFAIL_TESTS = 

# Identify the dependencies on a per-test basis:
testHepMC_SOURCES          = testHepMC.cc
testMass_SOURCES           = testMass.cc
testHepMCIteration_SOURCES = testHepMCIteration.cc
testPrintBug_SOURCES       = testPrintBug.cc
testSimpleVector_SOURCES   = testSimpleVector.cc

# Identify input data file(s) and prototype output file(s):
EXTRA_DIST = testHepMC.input testHepMC.output testHepMCParticle.output \
             testPrintBug.output testHepMCExtended.dat \
	     testHepMCIteration.output testHepMCIterationExtended.output

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = testHepMC.out testHepMCParticle.out testHepMCExtended.out \
             testPrintBug.out testHepMCIterationExtended.out testMass.out
