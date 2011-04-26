# This makefile is used to copy files during the "make install" step

INSTALLDIR = $(pkgdatadir)/examples/herwig

# files to distribute
EXTRA_DIST = \
    example_MyHerwig.cc \
    testHerwigCopies.cc

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(INSTALLDIR)
	$(INSTALL_DATA) GNUmakefile.example $(DESTDIR)$(INSTALLDIR)/GNUmakefile
	for file in $(EXTRA_DIST); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(DESTDIR)$(INSTALLDIR)/$$file; done
