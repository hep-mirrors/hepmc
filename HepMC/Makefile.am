## Process this file with automake to produce Makefile.in

if USE_GMAKE
  MAKE = gmake
endif

includedir = $(prefix)/include
COPY_P = @COPY_P@

pkginclude_HEADERS = \
	Flow.h		\
	GenEvent.h	\
	GenParticle.h	\
	GenVertex.h	\
	HeavyIon.h	\
	HEPEVT_Wrapper.h	\
	HerwigWrapper6_4.h	\
	HerwigWrapper.h	\
	IO_Ascii.h	\
	IO_AsciiParticles.h	\
	IO_ExtendedAscii.h	\
	IO_BaseClass.h	\
	IO_HEPEVT.h	\
	IO_HERWIG.h	\
	IO_PDG_ParticleDataTable.h	\
	ParticleData.h	\
	ParticleDataTable.h	\
	PdfInfo.h	\
	Polarization.h	\
	PythiaWrapper5_720.h	\
	PythiaWrapper6_152.h	\
	PythiaWrapper6_152_WIN32.h	\
	PythiaWrapper6_2.h	\
	PythiaWrapper6_2_WIN32.h	\
	PythiaWrapper.h	\
	WeightContainer.h	\
	SearchVector.h	\
	SimpleVector.h	\
	SimpleVector.icc	\
	enable_if.h	\
	is_arithmetic.h	\
	HepMC_CLHEP20.h	\
	TempParticleMap.h	\
	Version.h	\
	defs.h

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = defs.h
