# Applicable to each test program:

INCLUDES = -I$(top_builddir) -I$(top_srcdir) 

LDADD = $(top_builddir)/src/libHepMC.la

# Identify executables needed during testing:
check_PROGRAMS = testSimpleVector testUnits testPrintBug \
                 testHepMC testHepMCIteration testMass \
                 testMultipleCopies testStreamIO testFlow

check_SCRIPTS = testHepMC.sh testHepMCIteration.sh testPrintBug.sh \
                testMass.sh testStreamIO.sh testFlow.sh

# Identify test(s) to run when 'make check' is requested:
TESTS = testSimpleVector testUnits \
        testHepMC.sh testHepMCIteration.sh testMass.sh testFlow.sh testStreamIO.sh \
        testPrintBug.sh testMultipleCopies

# Identify the test(s) for which failure is the intended outcome:
XFAIL_TESTS = 

# Identify the dependencies on a per-test basis:
testSimpleVector_SOURCES   = testSimpleVector.cc
testUnits_SOURCES          = testUnits.cc
testFlow_SOURCES           = testFlow.cc
testHepMC_SOURCES          = testHepMC.cc testHepMCMethods.cc
testMass_SOURCES           = testMass.cc
testStreamIO_SOURCES       = testStreamIO.cc testHepMCMethods.cc
testHepMCIteration_SOURCES = testHepMCIteration.cc
testMultipleCopies_SOURCES = testMultipleCopies.cc
testPrintBug_SOURCES       = testPrintBug.cc

# Identify input data file(s) and prototype output file(s):
EXTRA_DIST = testIOGenEvent.input \
             testHepMCVarious.input \
	     testCrossSection.output \
	     testFlow.output \
             testHepMC.output \
	     testHepMCParticle.output \
             testPrintBug.output \
	     testStreamIO.output \
	     testCrossSection.dat \
	     testHepMC.dat \
	     testHepMCIteration.dat \
	     testMass1.dat \
	     testStreamIOVarious.dat

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = testHepMC.cout testStreamIO.cout \
             testHepMCIteration.cout \
             testHepMC.out testHepMCParticle.out \
             testPrintBug.out testMass.out \
	     testAscii.out testHepMCIteration.out testHepMCprecision.out \
             testHepMCIteration.out testMass1.out testMass2.out \
	     testMultipleCopies.out testHepMCVarious.out testIOAscii.out \
	     testMultipleCopies1.out testMultipleCopies2.out \
	     testMultipleOriginals.out testUnits.out \
	     testCrossSection.out testCrossSection2.out testCrossSection.cout \
	     testStreamIO.out testStreamIOParticle.out testStreamIOVarious.out \
	     testStreamIO2.out testStreamIOprecision.out testStreamIO.cout \
	     testFlow.out testFlow.out1 testFlow.out2 testFlow.out3
