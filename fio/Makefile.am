## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)

libHepMCfio_la_SOURCES = \
	HEPEVT_Wrapper.cc \
	IO_HEPEVT.cc \
	IO_HERWIG.cc \
	HerwigWrapper.cc

lib_LTLIBRARIES = libHepMCfio.la

libHepMCfio_la_LIBADD = $(top_builddir)/src/libHepMC.la

if BUILD_VISUAL
  lib_shared = HepMC.$(SHEXT)
else
  lib_shared = libHepMC.so
endif

# shared library versioning
libHepMCfio_la_LDFLAGS = $(LIBRARY_VERSION)
