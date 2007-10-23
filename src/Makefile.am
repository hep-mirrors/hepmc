## Process this file with automake to produce Makefile.in

if USE_GMAKE
  MAKE = gmake
endif

INCLUDES = -I$(top_builddir) -I$(top_srcdir)

libHepMC_a_SOURCES = \
	Flow.cc	\
	GenEvent.cc	\
	GenParticle.cc	\
	GenVertex.cc	\
	IO_Ascii.cc	\
	IO_AsciiParticles.cc	\
	IO_ExtendedAscii.cc	\
	IO_PDG_ParticleDataTable.cc	\
	ParticleData.cc	\
	Polarization.cc	\
	SearchVector.cc

libHepMC_so_OBJECTS = $(patsubst %.cc,$(shareddir)/%.$(OBJEXT),$(libHepMC_a_SOURCES))

# these if/else blocks will be parsed by automake and translated
if BUILD_SHARED
  shareddir = shared
if BUILD_VISUAL
  lib_shared = HepMC.$(SHEXT)
else
  lib_shared = libHepMC.$(SHEXT)
endif
  SHFLAGS = @MY_SHFLAGS@
  SHLINK = @MY_SHLINK@
  SHNAME = @MY_SHNAME@
endif
if BUILD_STATIC
  lib_LIBRARIES = libHepMC.a
endif

CLEANFILES =  $(lib_shared)

all-local: $(shareddir) $(lib_shared)

$(shareddir):
	test -d $(shareddir) || mkdir $(shareddir)

if BUILD_VISUAL
$(lib_shared): $(libHepMC_so_OBJECTS)
	$(SHLINK) $(SHNAME)$@ $(libHepMC_so_OBJECTS)

$(shareddir)/%.$(OBJEXT): %.cc
	$(CXXCOMPILE) $(SHFLAGS) /c /Fo$@ $<
else
$(lib_shared): $(libHepMC_so_OBJECTS)
	$(CXXLINK) $(SHLINK) $(SHNAME)$@ $(libHepMC_so_OBJECTS) -o $@

$(shareddir)/%.$(OBJEXT): %.cc
	$(CXXCOMPILE) $(SHFLAGS) -c -o $@ $<
endif

install-exec-local: $(lib_shared)
	@$(NORMAL_INSTALL)
	test -z "$(libdir)" || $(mkdir_p) "$(DESTDIR)$(libdir)"
	@list='$(lib_shared)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL) '$$p' '$(DESTDIR)$(libdir)/$$p'"; \
	    $(INSTALL) "$$p" "$(DESTDIR)$(libdir)/$$f"; \
	  else :; fi; \
	done

clean-local:
	-test -d $(shareddir) && rm -f $(shareddir)/*.$(OBJEXT)

distclean-local:
	-test -d $(shareddir) && rm -rf ./$(shareddir)
