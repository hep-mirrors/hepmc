
  HepMCdir             = /local/hepmc/root/hep3
  HepMClib             = -L$(HepMCdir)/lib -lHepMC


  CXX           = g++
  INCLUDES 	= -I$(HepMCdir)/include -I$(ROOTSYS)/include

  UNAME = $(shell uname)

ifeq "$(UNAME)" "Darwin"
else
  LINK_LIBS     += -Wl,-rpath -Wl,$(HepMCdir)/lib
endif

  HDRS          = $(HepMCdir)/include/HepMC/*.h *.h

PLATFORM=$(shell uname)

.SUFFIXES:      .o .cc .exe
.PHONY: dictgen
all:	exampleRootIO.exe

exampleRootIO.exe: dictgen exampleRootIO.o GenEventDict.o
	@echo "Building $@ ..."
	$(CXX) exampleRootIO.o GenEventDict.o \
	       `root-config --libs` \
		$(HepMClib)  -o $@

dictgen: GenEventDict.cc

GenEventDict.cc: LinkDef.h
	rootcint -f GenEventDict.cc -c $(HDRS) LinkDef.h


.cc.o:         $(HDRS) $<
	@echo "Compiling $< with $(CXX) ..."
	@$(CXX) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o
	rm -f *.exe
	rm -f *.dat

