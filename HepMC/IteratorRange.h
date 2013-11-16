#ifndef HEPMC_ITERATOR_RANGE_H
#define HEPMC_ITERATOR_RANGE_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics", Computer Physics Communications (to be published).
//
// Iterator ranges used by GenVertex
//////////////////////////////////////////////////////////////////////////


namespace HepMC {

  /// Type of particle relationship over which to iterate
  enum IteratorRange { parents, children, family, ancestors, descendants, relatives };

} // HepMC

#endif  // HEPMC_ITERATOR_RANGE_H
