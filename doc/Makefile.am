## Process this file with automake to produce Makefile.in

if USE_GMAKE
  MAKE = gmake
endif

TEX = latex
doc_srcdir = $(top_srcdir)/doc
doc_builddir = $(top_builddir)/doc
doc_installdir = $(prefix)/doc

DOC_POSTSCRIPT = HepMC2_user_manual.pdf HepMC2_user_manual.ps HepMC_user_manual.ps
DOC_INCLUDES = html.sty \
               HepMC2_user_manual.tex

SUFFIXES = .pdf .ps .dvi .tex

# latex twice to get references
.tex.dvi:        $<
	latex ${*F}.tex
	latex ${*F}.tex

.ps.pdf: $<
	ps2pdf $<

.dvi.ps:
	dvips -P pdf -o $@ $(@:.ps=.dvi)

all: docs

docs: get-includes $(DOC_POSTSCRIPT)

get-includes: 
	list='$(DOC_INCLUDES)'; for file in $$list; do \
	  if test ! -f $$file; then  \
	  (cp $(srcdir)/$$file $(doc_builddir)/$$file); fi; done

install-docs:
	$(mkinstalldirs) $(DESTDIR)$(doc_installdir)
	$(INSTALL_DATA) $(doc_srcdir)/HepMC_user_manual.ps $(DESTDIR)$(doc_installdir)/HepMC_user_manual.ps
	$(INSTALL_DATA) $(doc_builddir)/HepMC2_user_manual.pdf $(DESTDIR)$(doc_installdir)/HepMC2_user_manual.pdf
	$(INSTALL_DATA) $(doc_builddir)/HepMC2_user_manual.ps $(DESTDIR)$(doc_installdir)/HepMC2_user_manual.ps

# Identify generated file(s) to be removed when 'make clean' is requested:
CLEANFILES = $(DOC_POSTSCRIPT) $(DOC_INCLUDES) *.aux *.log *.toc *.dvi
