## Process this file with automake to produce Makefile.in

if USE_GMAKE
  MAKE = gmake
endif

includedir = $(prefix)/include

##SUBDIRS = HepMC src fio tests examples
SUBDIRS = HepMC src fio
# list all subdirectories - for distribution and cleaning
##DIST_SUBDIRS = HepMC src fio doc tests examples
DIST_SUBDIRS = HepMC src fio doc

docs:
	cd doc; $(MAKE) $(AM_MAKEFLAGS)

install-docs:
	cd doc; $(MAKE) $(AM_MAKEFLAGS) install-docs
