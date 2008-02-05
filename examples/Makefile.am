# This makefile is used to copy files during the "make install" step

if USE_GMAKE
  MAKE = gmake
endif

INSTALLDIR = @prefix@/examples/@PACKAGE@

# files to distribute
EXTRA_DIST = \
    example_BuildEventFromScratch.cc \
    example_EventSelection.cc \
    example_MyHerwig.cc \
    example_MyPythia.cc \
    example_MyPythiaRead.cc \
    example_MyPythiaOnlyToHepMC.cc \
    example_MyPythiaWithEventSelection.cc \
    example_PythiaParticle.cc	\
    example_ReadPDGtable.cc \
    example_UsingIterators.cc \
    initPythia.cc \
    PythiaHelper.h \
    initpydata.f \
    VectorConversion.h \
    example_UsingIterators.txt \
    PDG_ParticleDataTable.txt

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(INSTALLDIR)
	$(INSTALL_DATA) GNUmakefile.example $(DESTDIR)$(INSTALLDIR)/GNUmakefile
	for file in $(EXTRA_DIST); do \
	  $(INSTALL_DATA) $(srcdir)/$$file $(DESTDIR)$(INSTALLDIR)/$$file; done
