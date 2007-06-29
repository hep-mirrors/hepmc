//--------------------------------------------------------------------------

#ifndef PYTHIA_WRAPPER_H
#define PYTHIA_WRAPPER_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, December 1999
// Wrapper for FORTRAN version 5.720 of Pythia
// This wrapper is NOT intended as a part of HepMC - it is only supplied
// for your convenience.
// Most everything was written by Silvia Resconi (thanks!)
//////////////////////////////////////////////////////////////////////////
// 
// A simple example of calling Pythia from C++ using this header file is given
// at the bottom of this file.
//
// Note the luhepc routine in Jetset 7.4 which is used by Pythia 5.720 to fill
// the HEPEVT common block uses double precision and 2000 entries.
//

#include <ctype.h>

//--------------------------------------------------------------------------
// Initialization routine

#define initpydata initpydata_
    extern "C" {
	void initpydata(void);
    }

//--------------------------------------------------------------------------
// PYTHIA Common Block Declarations

const int pyjets_maxn =4000;
    extern struct {
	int n, k[5][pyjets_maxn];
	float p[5][pyjets_maxn], v[5][pyjets_maxn];
    } lujets_;
#define lujets lujets_

    extern struct {
        int mstu[200];
        float paru[200];
        int mstj[200];
        float parj[200];
    } ludat1_;
#define ludat1 ludat1_

    extern struct {
	int kchg[3][500];
	float pmas[4][500], parf[2000], vckm[4][4];  
    } ludat2_;
#define ludat2 ludat2_

    extern struct {
	int mdcy[3][500], mdme[2][2000];
	float brat[2000];
	int kfdp[5][2000];
    } ludat3_;
#define ludat3 ludat3_

    extern struct {
	int mrlu[6];
	float rrlu[100];
    } ludatr_;
#define ludatr ludatr_

    extern struct {
	int msel, msub[200], kfin[81][2];
	float ckin[200];
    } pysubs_;
#define pysubs pysubs_

    extern struct {
	int mstp[200];
	float parp[200];
	int msti[200];
	float pari[200];
    } pypars_;
#define pypars pypars_

    extern struct {
	int mint[400];
	float vint[400];
    } pyint1_;
#define pyint1 pyint1_

    extern struct {
	int iset[200], kfpr[2][200];
	float coef[20][200];
	int icol[2][4][40];
    } pyint2_;
#define pyint2 pyint2_

    extern struct {
	int ngen[3][201][3];
	float xsec[3][201];
    } pyint5_;
#define pyint5 pyint5_

//--------------------------------------------------------------------------
// PYTHIA routines declaration

#define luhepc luhepc_ 
#define pyinit pyinit_
#define lulist lulist_
#define pystat pystat_
#define pyevnt pyevnt_
#define ludata ludata_
    extern "C" {
	void luhepc(int*);
	void pyinit(const char*,const char*,const char*,float*,int,int,int);
	void lulist(int*);
	void pystat(int*);
	void pyevnt();
	void ludata(void);
    }

//--------------------------------------------------------------------------
// PYTHIA block data
// ( with gcc it works to initialize the block data by calling 
//   "pydata();" at beginning, but this fails for f77, so the fortran routine
//   initpydata.f is supplied ... call it instead for platform independent
//   behaviour )

#define pydata pydata_
extern "C" {
    void pydata(void);
}

#endif  // PYTHIA_WRAPPER_H
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// A simple example of calling Pythia from C++ using this header file:
//        (compiling instructions are given after the example)
//--------------------------------------------------------------------------
//  #include "HepMC/PythiaWrapper5_720.h"
//  main() {
//      // init the PYDATA block data as external by calling
//      initpydata();
//      //
//      // Turn on process number 20 to generate W+gamma
//      pysubs.msel=0;
//      pysubs.msub[20-1] = 1;  // -1 because arrays in C start at 0, F77 at 1
//      // set random number seed (mandatory)
//      ludatr.mrlu[0]=55122 ;
//      //
//      // call pythia initialization for 14 TeV p+ p+ collisions
//      float r_1 = (float)14000.;
//      const char* cms="CMS";
//      const char* collision_particle="p";
//      pyinit(cms, collision_particle, collision_particle, &r_1, 3, 1, 1);
//      //
//      // EVENT LOOP for 100 events
//      for ( int i = 1; i <= 100; i++ ) {
//  	    pyevnt();
//  	    int c_p1 = 1;  // necessary for C interface to pass by value
//  	    if ( i <= 5 ) lulist(&c_p1); // print first 5 events
//  	    // pythia luhepc routine convert common LUJETS in common HEPEVT
//  	    luhepc(&c_p1);
//      }
//      // print out the statistics
//      int c_p1 = 1; // necessary for C interface to pass by value
//      pystat(&c_p1);
//  }
//
//--------------------------------------------------------------------------
// In order to compile the above example you must link with the CERN library
// (in the order given below), with the g2c library, and with a compiled 
// initpydata.f Fortran routine. Assuming the HepMC include files are 
// in ./HepMC/ and the cern library is in /cern/pro/lib/, you would 
// compile as follows using g++:
// g77 -c initpydata.f
// g++ -c -I. filename.cxx
// g++ *.o -L/cern/pro/lib/ -lpawlib -lgraflib -lgrafX11 -lmathlib \ 
//                          -lkernlib -lpacklib -ljetset74 -lg2c 
//
//--------------------------------------------------------------------------


