#ifndef HEPMC_GEN_EVENT_ITERATORS_H
#define HEPMC_GEN_EVENT_ITERATORS_H

#include "HepMC/GenEvent.h"

namespace HepMC {

  /// @name Particle and vertex ranges
  /// These ranges behave like collections, for use with Boost foreach and C++11 range-for.
  //@{

  /// GenEventVertexRange acts like a collection of vertices
  struct GenEventVertexRange {
    /// @todo Mk const ref?
    GenEventVertexRange( GenEvent& e ) : m_event(e) {}
    GenEvent::vertex_iterator begin();
    GenEvent::vertex_iterator end();
  private:
    GenEventVertexRange& operator=( GenEventVertexRange & ); //< forbidden
    GenEvent& m_event;
  };


  /// ConstGenEventVertexRange acts like a collection of vertices
  struct ConstGenEventVertexRange {
    /// @todo Mk const ref?
    ConstGenEventVertexRange( GenEvent const & e ) : m_event(e) {}
    GenEvent::vertex_const_iterator begin() const;
    GenEvent::vertex_const_iterator end() const;
  private:
    ConstGenEventVertexRange& operator=( ConstGenEventVertexRange & ); //< forbidden
    GenEvent const & m_event;
  };


  /// GenEventParticleRange acts like a collection of particles
  struct GenEventParticleRange {
    /// @todo Mk const ref?
    GenEventParticleRange( GenEvent & e ) : m_event(e) {}
    GenEvent::particle_iterator begin();
    GenEvent::particle_iterator end();
  private:
    GenEventParticleRange& operator=( GenEventParticleRange & ); //< forbidden
    GenEvent & m_event;
  };


  /// ConstGenEventParticleRange acts like a collection of particles
  struct ConstGenEventParticleRange {
    /// @todo Mk const ref?
    ConstGenEventParticleRange( GenEvent const & e ) : m_event(e) {}
    GenEvent::particle_const_iterator begin() const;
    GenEvent::particle_const_iterator end() const;
  private:
    ConstGenEventParticleRange& operator=( ConstGenEventParticleRange & ); //< forbidden
    GenEvent const & m_event;
  };


  /// GenVertexParticleRange acts like a collection of particles
  struct GenVertexParticleRange {
    GenVertexParticleRange(GenVertex& v, IteratorRange range = relatives ) : m_vertex(v), m_range(range) {}
    typedef GenVertex::particle_iterator iterator;
    GenVertex::particle_iterator begin();
    GenVertex::particle_iterator end();

    /// @note This const iterator type doesn't exist and would be a mess to implement. We give up: use HepMC3 when it's available!
    // typedef GenVertex::particle_const_iterator const_iterator;
    // GenVertex::particle_const_iterator begin() const;
    // GenVertex::particle_const_iterator end() const;
  private:
    GenVertexParticleRange& operator=( GenVertexParticleRange & ); //< forbidden
    GenVertex & m_vertex;
    IteratorRange m_range;
  };


  /// GenParticleProductionRange acts like a collection of particles
  ///
  /// Throws a runtime error if the particle production_vertex is undefined
  struct GenParticleProductionRange {
    GenParticleProductionRange(const GenParticle& p, IteratorRange range = relatives ) : m_particle(p), m_range(range) {}
    GenVertex::particle_iterator begin();
    GenVertex::particle_iterator end();
  private:
    GenParticleProductionRange& operator=( GenParticleProductionRange & ); //< forbidden
    const GenParticle& m_particle;
    IteratorRange m_range;
  };


  /// GenParticleProductionRange acts like a collection of particles
  ///
  /// Throws a runtime error if the particle production_vertex is undefined
  struct ConstGenParticleProductionRange {
    ConstGenParticleProductionRange(const GenParticle& p, IteratorRange range = relatives ) : m_particle(p), m_range(range) {}
    GenVertex::particle_iterator begin();
    GenVertex::particle_iterator end();
  private:
    ConstGenParticleProductionRange& operator=( ConstGenParticleProductionRange & ); //< forbidden
    const GenParticle& m_particle;
    IteratorRange m_range;
  };


  /// GenParticleEndRange acts like a collection of particles
  ///
  /// Throws a runtime error if the particle end_vertex is undefined
  struct GenParticleEndRange {
    GenParticleEndRange(const GenParticle& p, IteratorRange range = relatives ) : m_particle(p), m_range(range) {}
    GenVertex::particle_iterator begin();
    GenVertex::particle_iterator end();
  private:
    GenParticleEndRange& operator=( GenParticleEndRange & ); //< forbidden
    const GenParticle& m_particle;
    IteratorRange m_range;
  };


  /// GenParticleEndRange acts like a collection of particles
  ///
  /// Throws a runtime error if the particle end_vertex is undefined
  struct ConstGenParticleEndRange {
    ConstGenParticleEndRange(const GenParticle& p, IteratorRange range = relatives ) : m_particle(p), m_range(range) {}
    GenVertex::particle_iterator begin();
    GenVertex::particle_iterator end();
  private:
    ConstGenParticleEndRange& operator=( ConstGenParticleEndRange & ); //< forbidden
    const GenParticle& m_particle;
    IteratorRange m_range;
  };



  //@}


}

#endif  // HEPMC_GEN_EVENT_ITERATORS_H
