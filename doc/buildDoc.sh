#! /bin/sh

# this script builds the documents
# the bootstrap process executes this script

set -x
# run doxygen if present
if [ `whereis doxygen | wc -w` -gt 1 ]; then
   doxygen doxygen.conf
   if [ `whereis latex | wc -w` -gt 1 ]; then
      cd latex
      latex refman.tex
      latex refman.tex
      dvips refman
      dvipdf refman
      mv refman.ps ../HepMC2_reference_manual.ps
      mv refman.pdf ../HepMC2_reference_manual.pdf
   fi
   cd ..
fi
# now run latex
if [ `whereis latex | wc -w` -gt 1 ]; then
   latex HepMC2_user_manual.tex 
   latex HepMC2_user_manual.tex
   dvips HepMC2_user_manual
   dvipdf HepMC2_user_manual
fi
