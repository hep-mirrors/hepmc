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
      dvips -P pdf -o HepMC2_reference_manual.ps refman
      ps2pdf HepMC2_reference_manual.ps
      mv HepMC2_reference_manual.* ../.
   fi
   cd ..
fi
# now run latex
if [ `whereis latex | wc -w` -gt 1 ]; then
   latex HepMC2_user_manual.tex 
   latex HepMC2_user_manual.tex
   dvips -P pdf -o HepMC2_user_manual.ps HepMC2_user_manual
   ps2pdf HepMC2_user_manual.ps
fi
