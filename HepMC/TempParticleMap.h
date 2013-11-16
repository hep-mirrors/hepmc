#ifndef HEPMC_TempParticleMap_H
#define HEPMC_TempParticleMap_H

//////////////////////////////////////////////////////////////////////////
// garren@fnal.gov, October 2007
//
// Used by IO classes
//////////////////////////////////////////////////////////////////////////

#include <map>

namespace HepMC {

  class GenParticle;

  //! TempParticleMap is a temporary GenParticle* container used during input.
  /// @todo Hide from public header interface?

  /// \class  TempParticleMap
  /// Used by IO classes for recoverable particle ordering.
  /// Map GenParticle* against both outgoing vertex and particle order.
  ///
  class TempParticleMap {
  public:
    typedef std::map<HepMC::GenParticle*,int> TempMap;
    typedef std::map<int,HepMC::GenParticle*> TempOrderMap;
    typedef TempMap::iterator     TempMapIterator;
    typedef TempOrderMap::iterator  orderIterator;

    TempParticleMap()
      : m_particle_to_end_vertex(), m_particle_order()
    { }

    TempMapIterator begin() { return m_particle_to_end_vertex.begin(); }
    TempMapIterator end() { return m_particle_to_end_vertex.end(); }
    orderIterator order_begin() { return m_particle_order.begin(); }
    orderIterator order_end() { return m_particle_order.end(); }

    int end_vertex( GenParticle* p) {
      return (m_particle_to_end_vertex.find(p) != end()) ? m_particle_to_end_vertex[p] : 0;
    }

    /// @todo Why pass the int by reference?
    void addEndParticle( GenParticle* p, int& end_vtx_code) {
      m_particle_order[p->barcode()] = p;
      m_particle_to_end_vertex[p] = end_vtx_code;
    }

  private:

    TempMap       m_particle_to_end_vertex;
    TempOrderMap  m_particle_order;

  };

} // HepMC

#endif  // HEPMC_TempParticleMap_H
