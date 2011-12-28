#ifndef HEPMC_MOCK_STREAM_IO_H
#define HEPMC_MOCK_STREAM_IO_H

namespace HepMC {

	std::ostream& writeMockRoot(std::ostream&);
	std::istream& readMockRoot(std::istream&);
    /// Explicitly write the begin block lines that IO_MockRoot uses
    std::ostream & write_HepMC_MockRoot_block_begin(std::ostream & );
    /// Explicitly write the end block line that IO_MockRoot uses
    std::ostream & write_HepMC_MockRoot_block_end(std::ostream & );

} // HepMC

#endif  // HEPMC_MOCK_STREAM_IO_H
