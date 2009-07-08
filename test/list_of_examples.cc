/// The HepMC tests can also serve as useful examples

	/// This program checks the deprecated IO_Ascii class.
	/// IO_Ascii is slated for removal in the 2.05 release.
	/// \example testDeprecated.cc.in

	/// based on example_EventSelection
	/// Apply an event selection to the events in testHepMC.input
	/// Events containing a photon of pT > 25 GeV pass the selection and are
	/// written to "testHepMC.out"
	/// Add arbitrary PDF information to the good events
	/// Also write events using IO_AsciiParticles 
	/// \example testHepMC.cc.in

	/// Use Matt's example_EventSelection along with example_UsingIterators 
	/// to check HepMC iteration.
	/// Apply an event selection to the events in testHepMC.input
	/// Events containing a photon of pT > 25 GeV pass the selection.
	/// Use iterators on these events.
	/// \example testHepMCIteration.cc.in

	/// Read events from testIOGenEvent.input
	/// Select events containing a photon of pT > 25 GeV 
	/// Add arbitrary PDF information to one of the good events
	/// Write the selected events and read them back in using an istream
	/// \example testMass.cc.in

	/// Multiple events in memory at the same time
	/// run with valgrind or some other leak checker
	/// \example testMultipleCopies.cc.in

        /// Thanks to Bob McElrath and Frank Siegert for this test
	/// \example testPrintBug.cc

        /// Exercise all the vector methods
	/// \example testSimpleVector.cc

	/// Test MomentumUnits and PositionUnits
	/// Make sure set and change methods work as expected.
	/// \example testUnits.cc

	/// Use a modified example_BuildEventFromScratch to test Flow
	/// \example testFlow.cc


// This file is used by Doxygen when generating the documentation.
