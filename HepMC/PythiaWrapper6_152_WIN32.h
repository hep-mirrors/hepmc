//--------------------------------------------------------------------------
#ifdef  _WIN32  // This version is for Windows MS Visual C++ only.
#ifndef PYTHIA_WRAPPER_H
#define PYTHIA_WRAPPER_H

//////////////////////////////////////////////////////////////////////////
// Matt.Dobbs@Cern.CH, November 2000
// Wrapper for FORTRAN version 6.152 of Pythia 
// (which is the version in CERNlib 2001)
// The _WIN32 version is provided by Witold Pokorski 
//       <Witold.Pokorski@Cern.CH>, 2002-01-22, and is the version 
//       which should be used when compiling on Windows MS Visual C++.
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

//--------------------------------------------------------------------------
// Initialization routine

    extern "C" {
	void INITPYDATA(void);
    }
#define initpydata INITPYDATA

//--------------------------------------------------------------------------
// PYTHIA Common Block Declarations

const int pyjets_maxn =4000;

struct PYSUBS_DEF {
	int msel, mselpd, msub[500], kfin[81][2];
	double ckin[200];
};

struct PYJETS_DEF {
	int n, npad, k[5][pyjets_maxn];
	double p[5][pyjets_maxn], v[5][pyjets_maxn];
    };

struct PYDAT1_DEF {
        int mstu[200];
        double paru[200];
        int mstj[200];
        double parj[200];
    };

struct PYDAT2_DEF {
	int kchg[4][500];
	double pmas[4][500], parf[2000], vckm[4][4];  
   };

struct PYPARS_DEF{
	int mstp[200];
	double parp[200];
	int msti[200];
	double pari[200];
    };

struct PYDATR_DEF{
	int mrpy[6];
	double rrpy[100];
    };

struct PYDAT3_DEF{
	int mdcy[3][500], mdme[2][4000];
	double brat[4000];
	int kfdp[5][4000];
    };


struct PYINT1_DEF{
	int mint[400];
	double vint[400];
    };

struct PYINT2_DEF{
	int iset[500], kfpr[2][500];
	double coef[20][500];
	int icol[2][4][40];       // was [320] was [40][4][2]
    };

struct PYINT5_DEF{
	int ngenpd, ngen[3][501];
	double xsec[3][501];
    };

struct PROCESS_DEF{
	int iprocess;
    };// This common is for ATLfast.


extern "C" PYSUBS_DEF PYSUBS;
extern "C" PYJETS_DEF PYJETS;
extern "C" PYDAT1_DEF PYDAT1;
extern "C" PYDAT2_DEF PYDAT2;
extern "C" PYPARS_DEF PYPARS;
extern "C" PYDATR_DEF PYDATR;
extern "C" PYDAT3_DEF PYDAT3;
extern "C" PYINT1_DEF PYINT1;
extern "C" PYINT2_DEF PYINT2;
extern "C" PYINT5_DEF PYINT5;
extern "C" PROCESS_DEF PROCESS;


#define pysubs PYSUBS
#define pyjets PYJETS
#define pydat1 PYDAT1
#define pydat2 PYDAT2
#define pypars PYPARS
#define pydatr PYDATR
#define pydat3 PYDAT3
#define pyint1 PYINT1
#define pyint2 PYINT2
#define pyint5 PYINT5
#define process PROCESS



//--------------------------------------------------------------------------
// PYTHIA routines declaration


extern "C" {
  void __stdcall PYSTAT(int*);
  void __stdcall PYHEPC(int*);
  void __stdcall PYLIST(int*);
  void __stdcall PYEVNT(void);

//--------------------------------------------------------------------------
// PYTHIA block data
// ( with gcc it works to initialize the block data by calling 
//   "pydata();" at beginning, but this fails for f77, so the fortran routine
//   initpydata.f is supplied ... call it instead for platform independent
//   behaviour )
  void __stdcall PYDATA(void);

  void __stdcall PYINIT(const char*, int, const char*, int, const char*, int, double*);
}




// define methods to hide the subtle syntax necessary to call fortran from C++

inline void call_pyhepc( int mode ){ PYHEPC( &mode ); }
inline void call_pyinit( const char* frame, const char* beam, const char* target,
		  double win ) 
{ PYINIT( frame,strlen(frame),beam,strlen(beam),target,strlen(target),&win); }
inline void call_pylist( int mode ){ PYLIST( &mode ); }
inline void call_pystat( int mode ){ PYSTAT( &mode ); }
inline void call_pyevnt(){ PYEVNT(); }


#endif  // PYTHIA_WRAPPER_H
#endif  // _WIN32
//--------------------------------------------------------------------------
