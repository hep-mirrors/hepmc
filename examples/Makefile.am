# This makefile is used to copy files during the "make install" step

if USE_GMAKE
  MAKE = gmake
endif

# files to distribute
EXTRA_DIST = \
    example_BuildEventFromScratch.cc \
    example_EventSelection.cc \
    example_MyHerwig.cc \
    example_MyPythia.cc \
    example_MyPythiaOnlyToHepMC.cc \
    example_MyPythiaWithEventSelection.cc \
    example_ReadPDGtable.cc \
    example_UsingIterators.cc \
    initpydata.f \
    example_UsingIterators.txt \
    PDG_ParticleDataTable.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(prefix)/examples
	$(INSTALL_DATA) GNUmakefile.example $(DESTDIR)$(prefix)/examples/GNUmakefile
	for file in $(EXTRA_DIST); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(DESTDIR)$(prefix)/examples/$$file; done
