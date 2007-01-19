## Process this file with automake to produce Makefile.in

if USE_GMAKE
  MAKE = gmake
endif

TEX = latex
doc_srcdir = $(top_srcdir)/doc
doc_builddir = $(top_builddir)/doc
doc_installdir = $(prefix)/doc/HepMC

DOC_POSTSCRIPT = HepMC_user_manual.ps
DOC_PDF        = HepMC2_user_manual.pdf HepMC2_user_manual.ps
DOC_INCLUDES = html.sty \
               HepMC2_user_manual.tex

if HAVE_LATEX
  DOCUMENTS = get-includes $(DOC_POSTSCRIPT) $(DOC_PDF)
else
  DOCUMENTS = get-includes $(DOC_POSTSCRIPT)
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

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(doc_installdir)
	$(INSTALL_DATA) $(doc_srcdir)/HepMC_user_manual.ps $(DESTDIR)$(doc_installdir)/HepMC_user_manual.ps
if HAVE_LATEX
	$(INSTALL_DATA) $(doc_builddir)/HepMC2_user_manual.pdf $(DESTDIR)$(doc_installdir)/HepMC2_user_manual.pdf
	$(INSTALL_DATA) $(doc_builddir)/HepMC2_user_manual.ps $(DESTDIR)$(doc_installdir)/HepMC2_user_manual.ps
endif

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = $(DOC_PDF) $(DOC_INCLUDES) *.aux *.log *.toc *.dvi
