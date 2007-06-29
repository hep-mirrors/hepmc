//--------------------------------------------------------------------------

#ifndef PYTHIA_WRAPPER_H
#define PYTHIA_WRAPPER_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, November 2000
// Wrapper for FORTRAN version 6.152 of Pythia 
// (which is the version in CERNlib 2001)
// This wrapper is NOT intended as a part of HepMC - it is only supplied
// for your convenience.
//////////////////////////////////////////////////////////////////////////
// 
// A simple example of calling Pythia from C++ using this header file is
// given in test/test_PythiaWrapper.cxx
//
// Note the pyhepc routine is used by Pythia to fill
// the HEPEVT common block uses double precision and 4000 entries.
//

#include <ctype.h>
#include <cstring>

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
	int n, npad, k[5][pyjets_maxn];
	double p[5][pyjets_maxn], v[5][pyjets_maxn];
    } pyjets_;
#define pyjets pyjets_

    extern struct {
        int mstu[200];
        double paru[200];
        int mstj[200];
        double parj[200];
    } pydat1_;
#define pydat1 pydat1_

    extern struct {
	int kchg[4][500];
	double pmas[4][500], parf[2000], vckm[4][4];  
    } pydat2_;
#define pydat2 pydat2_

    extern struct {
	int mdcy[3][500], mdme[2][4000];
	double brat[4000];
	int kfdp[5][4000];
    } pydat3_;
#define pydat3 pydat3_

    extern struct {
	int mrpy[6];
	double rrpy[100];
    } pydatr_;
#define pydatr pydatr_

    extern struct {
	int msel, mselpd, msub[500], kfin[81][2];
	double ckin[200];
    } pysubs_;
#define pysubs pysubs_

    extern struct {
	int mstp[200];
	double parp[200];
	int msti[200];
	double pari[200];
    } pypars_;
#define pypars pypars_

    extern struct {
	int mint[400];
	double vint[400];
    } pyint1_;
#define pyint1 pyint1_

    extern struct {
	int iset[500], kfpr[2][500];
	double coef[20][500];
	int icol[2][4][40];       // was [320] was [40][4][2]
    } pyint2_;
#define pyint2 pyint2_

    extern struct {
	int ngenpd, ngen[3][501];
	double xsec[3][501];
    } pyint5_;
#define pyint5 pyint5_

//--------------------------------------------------------------------------
// PYTHIA routines declaration

#define pyhepc pyhepc_ 
#define pyinit pyinit_
#define pylist pylist_
#define pystat pystat_
#define pyevnt pyevnt_
    extern "C" {
	void pyhepc(int*);
	void pyinit(const char*,const char*,const char*,double*,int,int,int);
	void pylist(int*);
	void pystat(int*);
	void pyevnt();
    }

// define methods to hide the subtle syntax necessary to call fortran from C++
inline void call_pyhepc( int mode ){ pyhepc( &mode ); }
inline void call_pyinit( const char* frame, const char* beam, const char* target,
		  double win ) 
{ pyinit( frame,beam,target,&win,strlen(frame),strlen(beam),strlen(target) ); }
inline void call_pylist( int mode ){ pylist( &mode ); }
inline void call_pystat( int mode ){ pystat( &mode ); }
inline void call_pyevnt(){ pyevnt(); }


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
