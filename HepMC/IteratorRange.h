#ifndef HEPMC_ITERATOR_RANGE_H
#define HEPMC_ITERATOR_RANGE_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, September 1999, refer to:
// M. Dobbs and J.B. Hansen, "The HepMC C++ Monte Carlo Event Record for
// High Energy Physics",  Comput.Phys.Commun. 134 (2001) 41-46.
//
// Iterator ranges used by GenVertex
//////////////////////////////////////////////////////////////////////////


namespace HepMC {

  /// Type of particle relationship over which to iterate
  enum IteratorRange { parents=0, PARENTS=0,
                       children=1, CHILDREN=1,
                       family=2, FAMILY=2,
                       ancestors=3, ANCESTORS=3,
                       descendants=4, DESCENDANTS=4,
                       relatives=5, RELATIVES=5 };

} // HepMC

#endif  // HEPMC_ITERATOR_RANGE_H
