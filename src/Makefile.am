## Process this file with automake to produce Makefile.in

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

libHepMC_la_SOURCES = \
	CompareGenEvent.cc	\
	Flow.cc	\
	GenEvent.cc	\
	GenEventStreamIO.cc	\
	GenParticle.cc	\
	GenCrossSection.cc	\
	GenVertex.cc	\
	HeavyIon.cc	\
	IO_AsciiParticles.cc	\
	IO_PDG_ParticleDataTable.cc	\
	IO_GenEvent.cc	\
	ParticleData.cc	\
	PdfInfo.cc	\
	Polarization.cc	\
	SearchVector.cc	\
	StreamHelpers.cc	\
	StreamInfo.cc	\
	Units.cc

lib_LTLIBRARIES = libHepMC.la

if BUILD_VISUAL
  lib_shared = HepMC.$(SHEXT)
else
  lib_shared = libHepMC.so
endif

# shared library versioning
libHepMC_la_LDFLAGS = $(LIBRARY_VERSION)
