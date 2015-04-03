#ifndef HEPMC_DEFS_H
#define HEPMC_DEFS_H
// ----------------------------------------------------------------------
//
// HepMCDefs.h
// Author:  Lynn Garren
//
//  Define various useful macros
//  Most allow users to check for various code features
//
// ----------------------------------------------------------------------

// the HeavyIon class is available in HepMC
#ifndef HEPMC_HAS_HEAVY_ION
#define HEPMC_HAS_HEAVY_ION
#endif

// the IO_Ascii class is NOT available in HepMC
#ifndef HEPMC_IO_ASCII_REMOVED
#define HEPMC_IO_ASCII_REMOVED
#endif

// the ParticleData class is NOT available in HepMC
#ifndef HEPMC_PARTICLE_DATA_REMOVED
#define HEPMC_PARTICLE_DATA_REMOVED
#endif

// the IO_GenEvent class is available in HepMC
#ifndef HEPMC_HAS_IO_GENEVENT
#define HEPMC_HAS_IO_GENEVENT
#endif

// the PdfInfo class is available in HepMC
#ifndef HEPMC_HAS_PDF_INFO
#define HEPMC_HAS_PDF_INFO
#endif

// HepMC uses SimpleVector (FourVector) to store momentum and position
#ifndef HEPMC_HAS_SIMPLE_VECTOR
#define HEPMC_HAS_SIMPLE_VECTOR
#endif

// units are defined in HepMC
#ifndef HEPMC_HAS_UNITS
#define HEPMC_HAS_UNITS
#endif

// the GenCrossSection class is available in HepMC
#ifndef HEPMC_HAS_CROSS_SECTION
#define HEPMC_HAS_CROSS_SECTION
#endif

// the iterator range classes are available in HepMC
#ifndef HEPMC_HAS_ITERATOR_RANGES
#define HEPMC_HAS_ITERATOR_RANGES
#endif

// particles and vertices can be retrieved as vectors of pointers as well as the verbose iterators
#ifndef HEPMC_HAS_SIMPLE_RANGES
#define HEPMC_HAS_SIMPLE_RANGES 1
#endif

// particles and vertices iterators have appropriate constness in method declarations and return types
#ifndef HEPMC_HAS_CONSISTENT_CONST
#define HEPMC_HAS_CONSISTENT_CONST 1
#endif

// the HepMC::WeightContainer class allows named weights (version 2)
#ifndef HEPMC_HAS_NAMED_WEIGHTS
#define HEPMC_HAS_NAMED_WEIGHTS 2
#endif

// The HepMC::GenVertex class has a status() member
#ifndef HEPMC_VERTEX_HAS_STATUS
#define HEPMC_VERTEX_HAS_STATUS
#endif

// the HepMC::HeavyIon class contains centrality
#ifndef HEPMC_HEAVY_ION_HAS_CENTRALITY
#define HEPMC_HEAVY_ION_HAS_CENTRALITY
#endif

// define the version of HepMC.
#ifndef HEPMC_VERSION
#define HEPMC_VERSION "2.07.00.a02"
#endif

// define the integer version of HepMC X.Y.Z = 1000000*X + 1000*Y + Z
#ifndef HEPMC_VERSION_CODE
#define HEPMC_VERSION_CODE 2007000
#endif

#endif  // HEPMC_DEFS_H
