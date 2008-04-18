# Applicable to each test program:

INCLUDES = -I$(top_builddir) -I$(top_srcdir) 

# libraries to link, and whence
if BUILD_VISUAL
 HepMCLIB    = $(top_builddir)/src/.libs/HepMC.$(SHEXT)
 HepMCfioLIB = $(top_builddir)/fio/.libs/HepMCfio.$(SHEXT) 
else
 HepMCLIB    = $(top_builddir)/src/.libs/libHepMC.a
 HepMCfioLIB = $(top_builddir)/fio/.libs/libHepMCfio.a
endif

#LDADD = $(top_builddir)/src/libHepMC.la \
#        $(top_builddir)/fio/libHepMCfio.la 

# Identify executables needed during testing:
check_PROGRAMS = testSimpleVector testUnits testPrintBug \
                 testHepMC testHepMCIteration testMass \
                 testMultipleCopies testDeprecated

check_SCRIPTS = testHepMC.sh testHepMCIteration.sh testPrintBug.sh \
                testMass.sh testDeprecated.sh

# Identify test(s) to run when 'make check' is requested:
TESTS = testSimpleVector testUnits \
        testHepMC.sh testHepMCIteration.sh testMass.sh \
        testPrintBug.sh testMultipleCopies testDeprecated.sh

# Identify the test(s) for which failure is the intended outcome:
XFAIL_TESTS = 

# Identify the dependencies on a per-test basis:
testSimpleVector_SOURCES   = testSimpleVector.cc
testUnits_SOURCES          = testUnits.cc
testHepMC_SOURCES          = testHepMC.cc
testMass_SOURCES           = testMass.cc
testHepMCIteration_SOURCES = testHepMCIteration.cc
testMultipleCopies_SOURCES = testMultipleCopies.cc
testPrintBug_SOURCES       = testPrintBug.cc
testDeprecated_SOURCES     = testDeprecated.cc

# Identify the link libraries and flags on a per-test basis:
testSimpleVector_LDADD   = $(top_builddir)/src/libHepMC.la
testUnits_LDADD          = $(top_builddir)/src/libHepMC.la
testHepMC_LDADD          = $(top_builddir)/src/libHepMC.la
testMass_LDADD           = $(top_builddir)/src/libHepMC.la
testHepMCIteration_LDADD = $(top_builddir)/src/libHepMC.la
testMultipleCopies_LDADD = $(top_builddir)/src/libHepMC.la
testPrintBug_LDADD       = $(top_builddir)/src/libHepMC.la
testDeprecated_LDADD     = $(top_builddir)/src/libHepMC.la

#testHepMC_LDFLAGS          = -Wl,--rpath -Wl,$(top_builddir)/src/.libs \
#                             -Wl,--rpath -Wl,$(top_builddir)/fio/.libs
#testMass_LDFLAGS           = -Wl,$(top_builddir)/fio/.libs
#testHepMCIteration_LDFLAGS = -Wl,$(top_builddir)/fio/.libs
#testMultipleCopies_LDFLAGS = -Wl,$(top_builddir)/fio/.libs
#testDeprecated_LDFLAGS     = -Wl,$(top_builddir)/fio/.libs

# Identify input data file(s) and prototype output file(s):
EXTRA_DIST = testHepMC.input testHepMC.output testHepMCParticle.output \
             testPrintBug.output testHepMCExtended.dat \
	     testHepMCIteration.output testHepMCIterationExtended.output \
	     testAscii.input testAscii.dat

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = testHepMC.out testHepMCParticle.out testHepMCExtended.out \
             testPrintBug.out testHepMCIterationExtended.out testMass.out \
	     testAscii.dat testHepMCExtended.dat testHepMCIteration.out \
             testHepMCIteration.dat testMass1.dat testMass2.dat \
             testDeprecated.cout testHepMC.cout testHepMCIteration.cout \
	     testMultipleCopies.out testHepMCVarious.out testIOAscii.dat \
	     testMultipleCopies1.dat testMultipleCopies2.dat \
	     testMultipleOriginals.dat testIOHepMCExtended.dat \
	     testUnits.out
