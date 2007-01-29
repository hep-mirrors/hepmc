## Process this file with automake to produce Makefile.in

if USE_GMAKE
  MAKE = gmake
endif

TEX = @LATEXPATH@
doc_srcdir = $(top_srcdir)/doc
doc_builddir = $(top_builddir)/doc
doc_installdir = $(prefix)/doc/HepMC

DOXYGEN_REFERENCEdir = $(doc_srcdir)/latex

HEPMC1_DOC   = HepMC_user_manual.ps
HEPMC2_DOCS  = HepMC2_user_manual.pdf HepMC2_user_manual.ps
HEPMC2_REF   = HepMC2_reference_manual.pdf HepMC2_reference_manual.ps
DOC_INCLUDES = html.sty \
               HepMC2_user_manual.tex

if HAVE_LATEX
  DOCUMENTS = get-includes $(HEPMC1_DOC) $(HEPMC2_DOCS) $(HEPMC2_REF)
else
  DOCUMENTS = get-includes $(HEPMC1_DOC)
endif

SUFFIXES = .pdf .ps .dvi .tex

# latex twice to get references
.tex.dvi:        $<
	$(TEX) ${*F}.tex
	$(TEX) ${*F}.tex

.ps.pdf: $<
	ps2pdf $<

.dvi.ps:
	dvips -P pdf -o $@ $(@:.ps=.dvi)

all: $(DOCUMENTS)

get-includes: 
	list='$(DOC_INCLUDES)'; for file in $$list; do \
	  if test ! -f $$file; then  \
	  (cp $(srcdir)/$$file $(doc_builddir)/$$file); fi; done

HepMC2_reference_manual.pdf:
	if test -d $(DOXYGEN_REFERENCEdir); then \
	( $(MAKE) -C$(DOXYGEN_REFERENCEdir) refman.pdf; \
	  mv $(DOXYGEN_REFERENCEdir)/refman.ps $(doc_builddir)/HepMC2_reference_manual.ps; \
	  mv $(DOXYGEN_REFERENCEdir)/refman.pdf $(doc_builddir)/HepMC2_reference_manual.pdf); \
	fi
	  
HepMC2_reference_manual.ps:
	if test -d $(DOXYGEN_REFERENCEdir); then \
	( $(MAKE) -C$(DOXYGEN_REFERENCEdir) refman.ps; \
	  mv $(DOXYGEN_REFERENCEdir)/refman.ps $(doc_builddir)/HepMC2_reference_manual.ps); \
	fi

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(doc_installdir)
	$(INSTALL_DATA) $(doc_srcdir)/HepMC_user_manual.ps $(DESTDIR)$(doc_installdir)/HepMC_user_manual.ps
if HAVE_LATEX
	list='$(HEPMC2_DOCS) $(HEPMC2_REF)'; for file in $$list; do \
	if test -f $$file; then  \
	($(INSTALL_DATA) $(doc_builddir)/$$file $(DESTDIR)$(doc_installdir)/$$file); \
	fi; done
endif

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = $(HEPMC2_DOCS) $(HEPMC2_REF) $(DOC_INCLUDES) *.aux *.log *.toc *.dvi
