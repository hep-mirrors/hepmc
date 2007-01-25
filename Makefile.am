## Process this file with automake to produce Makefile.in

if USE_GMAKE
  MAKE = gmake
endif

includedir = $(prefix)/include

SUBDIRS = HepMC src fio test examples doc
# list all subdirectories - for distribution and cleaning
DIST_SUBDIRS = HepMC src fio test examples doc

# there is no longer a need to call "make docs" explicitly
# these directives remain for convenience
docs:
	cd doc; $(MAKE) $(AM_MAKEFLAGS)

install-docs:
	cd doc; $(MAKE) $(AM_MAKEFLAGS) install-docs
