## Process this file with automake to produce Makefile.in

# documents are already built - this is just used to install them

if USE_GMAKE
  MAKE = gmake
endif

doc_srcdir = $(top_srcdir)/doc
doc_installdir = $(prefix)/doc/HepMC

HEPMC1_DOC   = HepMC_user_manual.ps
HEPMC2_DOCS  = HepMC2_user_manual.pdf HepMC2_user_manual.ps
HEPMC2_REF   = HepMC2_reference_manual.pdf HepMC2_reference_manual.ps

DOCUMENTS = $(HEPMC1_DOC) $(HEPMC2_DOCS) $(HEPMC2_REF)

EXTRA_DIST = $(DOCUMENTS)

#all: $(DOCUMENTS)


install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(doc_installdir)
	list='$(DOCUMENTS)'; for file in $$list; do \
	if test -f $(doc_srcdir)/$$file; then  \
	($(INSTALL_DATA) $(doc_srcdir)/$$file $(DESTDIR)$(doc_installdir)/$$file); \
	fi; done

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = *.aux *.log *.toc *.dvi
