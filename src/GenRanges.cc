#include "HepMC/GenRanges.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include <iostream>
#include <stdexcept>

namespace HepMC {


  GenEventVertexRange GenEvent::vertex_range() { return GenEventVertexRange(*this); }
  ConstGenEventVertexRange GenEvent::vertex_range() const { return ConstGenEventVertexRange(*this); }
  GenEventVertexRange GenEvent::vertices() { return vertex_range(); }
  ConstGenEventVertexRange GenEvent::vertices() const { return vertex_range(); }

  GenEventParticleRange GenEvent::particle_range() { return GenEventParticleRange(*this); }
  ConstGenEventParticleRange GenEvent::particle_range() const { return ConstGenEventParticleRange(*this); }
  GenEventParticleRange GenEvent::particles() { return particle_range(); }
  ConstGenEventParticleRange GenEvent::particles() const { return particle_range(); }

  GenVertexParticleRange GenVertex::particles( IteratorRange range ) { return GenVertexParticleRange(*this, range); }
  GenParticleProductionRange GenVertex::particles_in( GenParticle& p, IteratorRange range ) { return GenParticleProductionRange(p,range); }
  ConstGenParticleProductionRange GenVertex:: particles_in( GenParticle const & p, IteratorRange range ) const { return ConstGenParticleProductionRange(p,range); }
  GenParticleEndRange GenVertex::particles_out( GenParticle& p, IteratorRange range ) { return GenParticleEndRange(p,range); }
  ConstGenParticleEndRange GenVertex::particles_out( GenParticle const & p, IteratorRange range ) const { return ConstGenParticleEndRange(p,range); }

  /// @todo Add a particles(p, itrange)
  GenParticleProductionRange GenParticle::particles_in( IteratorRange range ) { return GenParticleProductionRange(*this,range); }
  ConstGenParticleProductionRange GenParticle::particles_in( IteratorRange range ) const { return ConstGenParticleProductionRange(*this,range); }
  GenParticleEndRange GenParticle::particles_out( IteratorRange range ) { return GenParticleEndRange(*this,range); }
  ConstGenParticleEndRange GenParticle::particles_out( IteratorRange range ) const { return ConstGenParticleEndRange(*this,range); }

  //////////////////

  GenEvent::vertex_iterator GenEventVertexRange::begin() { return m_event.vertices_begin(); }
  GenEvent::vertex_iterator GenEventVertexRange::end()   { return m_event.vertices_end(); }

  GenEvent::vertex_const_iterator  ConstGenEventVertexRange::begin() const { return m_event.vertices_begin(); }
  GenEvent::vertex_const_iterator  ConstGenEventVertexRange::end()   const { return m_event.vertices_end(); }

  GenEvent::particle_iterator GenEventParticleRange::begin() { return m_event.particles_begin(); }
  GenEvent::particle_iterator GenEventParticleRange::end()   { return m_event.particles_end(); }

  GenEvent::particle_const_iterator ConstGenEventParticleRange::begin() const { return m_event.particles_begin(); }
  GenEvent::particle_const_iterator ConstGenEventParticleRange::end()   const { return m_event.particles_end(); }

  GenVertex::particle_iterator GenVertexParticleRange::begin() { return m_vertex.particles_begin(m_range); }
  GenVertex::particle_iterator GenVertexParticleRange::end()   { return m_vertex.particles_end(m_range); }


  GenVertex::particle_iterator GenParticleProductionRange::begin() {
    if ( ! m_particle.production_vertex() ) throw(std::range_error("GenParticleProductionRange: GenParticle has no production_vertex"));
    return m_particle.production_vertex()->particles_begin(m_range);
  }
  GenVertex::particle_iterator GenParticleProductionRange::end() {
    if ( ! m_particle.production_vertex() ) throw(std::range_error("GenParticleProductionRange: GenParticle has no production_vertex"));
    return m_particle.production_vertex()->particles_end(m_range);
  }

  GenVertex::particle_iterator ConstGenParticleProductionRange::begin() {
    if ( ! m_particle.production_vertex() ) throw(std::range_error("ConstGenParticleProductionRange: GenParticle has no production_vertex"));
    return m_particle.production_vertex()->particles_begin(m_range);
  }
  GenVertex::particle_iterator ConstGenParticleProductionRange::end() {
    if ( ! m_particle.production_vertex() ) throw(std::range_error("ConstGenParticleProductionRange: GenParticle has no production_vertex"));
    return m_particle.production_vertex()->particles_end(m_range);
  }

  GenVertex::particle_iterator GenParticleEndRange::begin() {
    if ( ! m_particle.end_vertex() ) throw(std::range_error("GenParticleEndRange: GenParticle has no end_vertex"));
    return m_particle.end_vertex()->particles_begin(m_range);
  }
  GenVertex::particle_iterator GenParticleEndRange::end() {
    if ( ! m_particle.end_vertex() ) throw(std::range_error("GenParticleEndRange: GenParticle has no end_vertex"));
    return m_particle.end_vertex()->particles_end(m_range);
  }

  GenVertex::particle_iterator ConstGenParticleEndRange::begin() {
    if ( ! m_particle.end_vertex() ) throw(std::range_error("ConstGenParticleEndRange: GenParticle has no end_vertex"));
    return m_particle.end_vertex()->particles_begin(m_range);
  }
  GenVertex::particle_iterator ConstGenParticleEndRange::end() {
    if ( ! m_particle.end_vertex() ) throw(std::range_error("ConstGenParticleEndRange: GenParticle has no end_vertex"));
    return m_particle.end_vertex()->particles_end(m_range);
  }


}
