################################################################################ Makefile for building a HepMC shared library with the gcc compiler
# Matt.Dobbs@CERN.CH  1.2000
#
# This makefiles also works to compile the example programs.
# I.E.: syntax for compiling the example_MyPythia.cxx example:
#       gmake examples/example_MyPythia.exe
# or simply   gmake examples     to compile all examples.
# This should be run from the main HepMC directory. The executables will
# be placed in the examples directory.
#
# NOTE: The library and examples have been tested using GCC compiler on both
#       Linux (Redhat 5.1) and HPUX (10.2).
#
################################################################################ Define directory paths 
#    You may have to change the CERNLIBdir and CLHEPdir paths.
#    DOXYGENdir, DOXYGEN_REFERENCEdir, LATEX_USER_MANUALdir, DISTRIBUTIONdir
#    are for developers only, ignore them.
#
  HepMCdir             = $(shell pwd)
  HepMClib             = libHepMC.so
  CLHEPdir             = /afs/cern.ch/sw/lhcxx/specific/@sys/CLHEP/1.8.0.0
   # use: /afs/cern.ch/sw/lhcxx/specific/rh73_gcc32/CLHEP/1.8.0.0 for gcc 3.2
  CERNLIBdir           = /cern/2002/lib
  DOXYGENdir           = /usr/bin
  DOXYGEN_REFERENCEdir = $(HepMCdir)/doc/DOxygen_reference
  LATEX_USER_MANUALdir = $(HepMCdir)/doc/latex_user_manual
  DISTRIBUTIONdir      = $(HepMCdir)/../dev_V1.0

################################################################################ Compiler options
#
  F77           = g77
  CXX           = g++
  CXXFLAGS      = -I$(HepMCdir) -I$(CLHEPdir)/include
  FLAGS         = -O3 -fPIC        -ansi -pedantic -Wall 
  LIBFLAGS      = -shared
  CLHEP_LIB     = -L$(CLHEPdir)/lib -lCLHEP
  initpydata_OBJ= src/initpydata.o
  Pythia_LIB	= -L$(CERNLIBdir) -lpythia6205 -lpythiad $(initpydata_OBJ) 
#  Herwig_LIB	= -L. -lHerwig dummy_uproutines.o
  LINK_LIBS     = -lg2c -lnsl
  OBJS	        = $(addsuffix .o,$(basename \
                                 $(shell find ./src/ -name '*.cxx')))
  SRCS	        = src/*.cxx src/*.f
  HDRS          = HepMC/*.h

################################################################################ HP-UX Options and Fixes
# see http://consult.cern.ch/qa/2534 for hint at HP-UX solution
#
PLATFORM=$(shell uname)
ifeq "$(PLATFORM)" "HP-UX"
  LINK_LIBS        = $(LINK_LIBS) -lf
endif

################################################################################ definition of the compiler options
#	-I location of directory containing include files
#	-L location of directory containing libraries
#       -lname include the library from -L location called libname.a
#	   -lg2c is the library containing info on converting fortran to C
#          -lf   is the library containing the intrinsic for HPUX only.
#	-shared make a shared library as output
#	-fPIC produce position independent code
#        necessary on some platforms (including HPUX) for -shared
# 	-fpic ^^ same(?)
#	-O optimizes
#	-g produces output for the debugger
#       -pg produces output for gprof profiler
#       note: if you want to see all warnings and ensure ansi standard 
#             compatibility, use:
#             -pipe -ansi -pedantic -fnonnull-objects \
#             -W -Wall -Wwrite-strings -Wpointer-arith -Wnested-externs \
#             -Woverloaded-virtual -Wbad-function-cast -fnonnull-objects
#       The proper order for cernlib libraries is:
#       -lpawlib -lgraflib -lgrafX11 -lmathlib -lkernlib -lpacklib -ljetset74
#
# makefile syntax:
#        for target thedir/target.suf from source anotherdir/source.suf2
#        ${*D}  = thedir
#        ${*F}  = target
#        $*     = thedir/target
#        $@     = thedir/target.suf
#        $<     = anotherdir/source.suf2
#  

###############################################################################
#
.SUFFIXES:      .o .cxx .f .exe .tex .ps
all:		$(HepMClib)

###############################################################################
# instructions for building the HepMC shared library
#
$(HepMClib):    $(OBJS) $(HDRS) $(initpydata_OBJ)
	@echo "Linking $(HepMClib) ..."
	@$(CXX) $(FLAGS) $(LIBFLAGS) $(OBJS) -o $(HepMClib)
	@echo "done"

###############################################################################
# instructions for building a .o file from a .cxx file
#
.cxx.o:         $(HDRS) $<
	@echo "Compiling $< with $(CXX) ..."
	@$(CXX) $(FLAGS) $(CXXFLAGS) -c $< -o $@

###############################################################################
# instructions for building a .o file from a .f file
#
.f.o:           $<
	@echo "Compiling $< with $(F77) ..."
	@$(F77) $(FLAGS) -c $< -o $@

###############################################################################
# instructions for building one of the example routines
#     (this also compiles and links in initpydata.f)
#
.o.exe:         $(initpydata_OBJ) $<
	$(MAKE) $(HepMClib) --no-print-directory
	@echo "Building $@ ..."
	@$(CXX) $(FLAGS) $< $(HepMCdir)/$(HepMClib) \
	        $(Pythia_LIB) $(Herwig_LIB) $(LINK_LIBS) -o $@

###############################################################################
# instructions for building all of the examples at the same time
#
examples: $(addsuffix .exe,$(basename $(shell find ./examples/ -name '*.cxx')))

###############################################################################
# example_PythiaToLCWrite.exe needs some special instructions:
#     (this also compiles and links in initpydata.f)
#
lcwriteobj = examples/Benchmarks_f77/lcwrite.o \
             examples/Benchmarks_f77/example_PythiaToLCWrite.o \
             $(initpydata_OBJ) 
examples/Benchmarks_f77/example_PythiaToLCWrite.exe: $(lcwriteobj)
	$(MAKE) $(HepMClib) --no-print-directory
	$(CXX) $(FLAGS) $(lcwriteobj) \
               $(HepMCdir)/$(HepMClib) $(Pythia_LIB) $(LINK_LIBS) -o $@

###############################################################################
# instructions for building all benchmark executables 
# (mostly the same as examples)
#
benchmarks: $(examples) examples/Benchmarks_f77/example_PythiaToLCWrite.exe
	find examples/. -name '*.exe' \
             -exec ln -s {} examples/Benchmarks_f77/. \;

###############################################################################
# instructions for building a postscript file from a latex file
.tex.ps:        $<
	@echo "---> Latexing ${*}.tex ..."
	@+cd ${*D};latex ${*F}.tex
	@echo "---> Latexing ${*}.tex again ..."
	@+cd ${*D};latex ${*F}.tex
	@echo "---> Converting ${*}.dvi to ${*}.ps ..."
	@+cd ${*D};dvips ${*F}.dvi -o ${*F}.ps
	+cd ${*D};rm -f ${*F}.dvi ${*F}.log ${*F}.aux

###############################################################################
# gmake clean       removes all garbage from HepMC directories.
#
clean:
	find . \( -name '*.o' -o -name '*~' \) -exec rm -f {} \;

###############################################################################
# gmake distclean       removes all compiled libraries, executables, +garbage
#                       to prepare the package for distribution
distclean: 
	$(MAKE) clean --no-print-directory
	rm -f $(HepMCdir)/$(HepMClib)
	find . \( -name '*.exe' \) -exec rm -f {} \;
	rm -f examples/*.dat

###############################################################################
# generate DOxygen_reference manuals (latex + html)
#
DOxygen_reference:
	mkdir $(DOXYGEN_REFERENCEdir)/tmp_latex_reference
	cp $(DOXYGEN_REFERENCEdir)/fancyheadings.sty \
           $(DOXYGEN_REFERENCEdir)/tmp_latex_reference/.
	$(DOXYGENdir)/doxygen $(DOXYGEN_REFERENCEdir)/DOxygen_HepMC.config
	$(MAKE) -C$(DOXYGEN_REFERENCEdir)/tmp_latex_reference refman.ps
	mv $(DOXYGEN_REFERENCEdir)/tmp_latex_reference/refman.ps \
           $(DOXYGEN_REFERENCEdir)/$@.ps
	rm -rf $(DOXYGEN_REFERENCEdir)/tmp_latex_reference


###############################################################################
# generate latex user manual
#
latex_user_manual: $(LATEX_USER_MANUALdir)/HepMC_user_manual.ps

###############################################################################
# generate a distribution 
# Remember also to:
#   1. move the html & user manual to public location
#   2. make an html version of the user manual with ps2www
#   3. update the version number on the webpage.
#
distribution: distclean latex_user_manual DOxygen_reference
	cp -r $(HepMCdir) $(DISTRIBUTIONdir)
	rm -fr $(DISTRIBUTIONdir)/test
	rm -fr $(DISTRIBUTIONdir)/doc/DOxygen_reference














