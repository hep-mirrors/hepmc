# Applicable to each test program:

INCLUDES = -I$(top_builddir) -I$(top_srcdir) 

LDADD = $(top_builddir)/src/libHepMC.la

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

# Identify input data file(s) and prototype output file(s):
EXTRA_DIST = testHepMC.input testHepMC.output testHepMCParticle.output \
             testPrintBug.output \
	     testHepMCIteration.output \
	     testAscii.input testAscii.dat

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = testHepMC.out testHepMCParticle.out \
             testPrintBug.out testMass.out \
	     testAscii.dat testHepMCIteration.out \
             testHepMCIteration.dat testMass1.dat testMass2.dat \
             testDeprecated.cout testHepMC.cout testHepMCIteration.cout \
	     testMultipleCopies.out testHepMCVarious.out testIOAscii.dat \
	     testMultipleCopies1.dat testMultipleCopies2.dat \
	     testMultipleOriginals.dat testUnits.out
