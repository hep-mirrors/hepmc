#include "HepMC/GenRanges.h"
#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include <iostream>
#include <stdexcept>

namespace HepMC {


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

  /// @note This const iterator type doesn't exist and would be a mess to implement. We give up: use HepMC3 when it's available!
  // GenVertex::particle_const_iterator GenVertexParticleRange::begin() const { return m_vertex.particles_const_begin(m_range); }
  // GenVertex::particle_const_iterator GenVertexParticleRange::end() const   { return m_vertex.particles_const_end(m_range); }


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
