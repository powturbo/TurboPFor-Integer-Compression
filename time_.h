/**
    Copyright (C) powturbo 2013-2019
    GPL v2 License

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    - homepage : https://sites.google.com/site/powturbo/
    - github   : https://github.com/powturbo
    - twitter  : https://twitter.com/powturbo
    - email    : powturbo [_AT_] gmail [_DOT_] com
**/
//      time_.h : parameter free high precision time/benchmark functions
#include <time.h>
#include <float.h>

  #ifdef _WIN32
#include <windows.h>
    #ifndef sleep
#define sleep(n) Sleep((n) * 1000)
    #endif

typedef unsigned __int64 uint64_t;
typedef unsigned __int64 tm_t;

  #else
#include <stdint.h>
#include <unistd.h>
#define Sleep(ms) usleep((ms) * 1000)

typedef struct timespec tm_t;
  #endif

#if defined (__i386__) || defined( __x86_64__ )
  #ifdef _MSC_VER
#include <intrin.h> // __rdtsc
  #else
#include <x86intrin.h>
  #endif

  #ifdef __corei7__
#define RDTSC_INI(_c_) do { unsigned _cl, _ch;              \
  __asm volatile ("couid\n\t"                               \
                "rdtsc\n\t"                                 \
                "mov %%edx, %0\n"                           \
                "mov %%eax, %1\n": "=r" (_ch), "=r" (_cl):: \
                "%rax", "%rbx", "%rcx", "%rdx");            \
  _c_ = (uint64_t)_ch << 32 | _cl;              \
} while(0)

#define RDTSC(_c_) do { unsigned _cl, _ch;                  \
  __asm volatile("rdtscp\n"                                 \
               "mov %%edx, %0\n"                            \
               "mov %%eax, %1\n"                            \
               "cpuid\n\t": "=r" (_ch), "=r" (_cl):: "%rax",\
               "%rbx", "%rcx", "%rdx");\
  _c_ = (uint64_t)_ch << 32 | _cl;\
} while(0)
  #else
#define RDTSC(_c_) do { unsigned _cl, _ch;\
  __asm volatile ("cpuid \n"\
                "rdtsc"\
                : "=a"(_cl), "=d"(_ch)\
                : "a"(0)\
                : "%ebx", "%ecx");\
  _c_ = (uint64_t)_ch << 32 | _cl;\
} while(0)
#define RDTSC_INI(_c_) RDTSC(_c_)
  #endif
#else
#define RDTSC_INI(_c_)
#define RDTSC(_c_)
#endif

#define tmrdtscini() ({ uint64_t _c; __asm volatile("" ::: "memory"); RDTSC_INI(_c); _c; })
#define tmrdtsc()    ({ uint64_t _c; RDTSC(_c); _c; })

#ifndef TM_F
#define TM_F 1.0  // TM_F=4 -> MI/s
#endif

  #ifdef RDTSC_ON
#define tminit() tmrdtscini()
#define tmtime() tmrdtsc()
#define TM_T                    CLOCKS_PER_SEC
static double TMBS(unsigned l, double t) { double dt = t, dl = l; return t/l; }
#define TM_C 1000

  #else
#define TM_C 1
static double TMBS(unsigned l, double t) { return (l/t)/1000000.0; }

    #ifdef _WIN32
static LARGE_INTEGER tps;
static tm_t tmtime(void) {
  LARGE_INTEGER tm;
  tm_t t;
  QueryPerformanceCounter(&tm);
  return tm.QuadPart;
}

static tm_t tminit() { tm_t t0,ts; QueryPerformanceFrequency(&tps); t0 = tmtime(); while((ts = tmtime())==t0) {}; return ts; }
static double tmdiff(tm_t start, tm_t stop) { return (double)(stop - start)/tps.QuadPart; }
static int tmiszero(tm_t t) { return !t; }
    #else
      #ifdef __APPLE__
#include <AvailabilityMacros.h>
        #ifndef MAC_OS_X_VERSION_10_12
#define MAC_OS_X_VERSION_10_12 101200
        #endif
#define CIVETWEB_APPLE_HAVE_CLOCK_GETTIME (defined(__APPLE__) && defined(MAC_OS_X_VERSION_MIN_REQUIRED) && MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_12)
        #if !(CIVETWEB_APPLE_HAVE_CLOCK_GETTIME)
#include <sys/time.h>
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 0
int clock_gettime(int /*clk_id*/, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}
        #endif
      #endif
static   tm_t tmtime()                      { struct timespec tm; clock_gettime(CLOCK_MONOTONIC, &tm); return tm; }
static double tmdiff(tm_t start, tm_t stop) { return (stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec)/1e9f; }
static   tm_t tminit()   { tm_t t0 = tmtime(),t; while(!tmdiff(t = tmtime(),t0)) {}; return t; }
static int tmiszero(tm_t t) { return !(t.tv_sec|t.tv_nsec); }
    #endif
#endif

//---------------------------------------- bench ----------------------------------------------------------------------
// for each a function call is repeated until exceding tm_tx seconds.
// A run duration is always tm_tx seconds
// The number of runs can be set with the program options  -I and -J (specify -I15 -J15 for more precision)

// sleep after each 8 runs to avoid cpu trottling.
#define TMSLEEP do { tm_T = tmtime(); if(tmiszero(tm_0)) tm_0 = tm_T; else if(tmdiff(tm_0, tm_T) > tm_TX) { if(tm_verbose) { printf("S \b\b");fflush(stdout); } sleep(tm_slp); tm_0=tmtime();} } while(0)

// benchmark loop
#define TMBEG(_tm_Reps_) { unsigned _tm_r,_tm_c = 0,_tm_R,_tm_Rx = _tm_Reps_,_tm_Rn = _tm_Reps_; double _tm_t;\
  for(tm_rm = tm_rep, tm_tm = DBL_MAX, _tm_R = 0; _tm_R < _tm_Rn; _tm_R++) { tm_t _tm_t0 = tminit(); /*for each run*/\
    for(_tm_r = 0;_tm_r < tm_rm;) { /*repeat tm_rm times */

#define TMEND(_len_) \
      _tm_r++; if(tm_tm == DBL_MAX && (_tm_t = tmdiff(_tm_t0, tmtime())) > tm_tx) break;\
    }\
    /*1st run: break the loop after tm_tx=1 sec, calculate a new repeats 'tm_rm' to avoid calling time() after each function call*/\
    /*other runs: break the loop only after 'tm_rm' repeats */ \
    _tm_t = tmdiff(_tm_t0, tmtime());\
    /*set min time, recalculte repeats tm_rm based on tm_tx, recalculte number of runs based on tm_TX*/\
    if(_tm_t < tm_tm) { if(tm_tm == DBL_MAX) { tm_rm = _tm_r; _tm_Rn = tm_TX/_tm_t; _tm_Rn = _tm_Rn<_tm_Rx?_tm_Rn:_tm_Rx; /*printf("[%d,%d] ", tm_rm, _tm_Rn);*/ } tm_tm = _tm_t; _tm_c++; }\
    else if(_tm_t > tm_tm*1.15) TMSLEEP;/*force sleep at 15% divergence*/\
    if(tm_verbose) { printf("%8.2f %2d_%.2d\b\b\b\b\b\b\b\b\b\b\b\b\b\b",TMBS(_len_, tm_tm/tm_rm),_tm_R+1,_tm_c),fflush(stdout); }\
    if((_tm_R & 7)==7) sleep(tm_slp); /*pause 20 secs after each 8 runs to avoid cpu trottling*/\
  }\
}

static unsigned tm_rep = 1<<30, tm_Rep = 3, tm_Rep2 = 3, tm_rm, tm_RepMin = 1, tm_slp = 20, tm_verbose = 2;
static tm_t tm_0, tm_T;
static double tm_tm, tm_tx = 1, tm_TX = 60;

static void tm_init(int _tm_Rep, int _tm_verbose) { tm_verbose = _tm_verbose; if(_tm_Rep) tm_Rep = _tm_Rep; }

#define TMBENCH(_name_, _func_, _len_)  do { if(tm_verbose>1) printf("%s ", _name_?_name_:#_func_);\
  TMBEG(tm_Rep) _func_; TMEND(_len_); \
  double dm = tm_tm, dr = tm_rm; if(tm_verbose) printf("%8.2f      \b\b\b\b\b", TMBS(_len_, dm*TM_C/dr) );\
} while(0)

// second TMBENCH. Example: use TMBENCH for encoding and TMBENCH2 for decoding
#define TMBENCH2(_name_, _func_, _len_)  do { \
  TMBEG(tm_Rep2) _func_; TMEND(_len_);\
  double dm = tm_tm, dr = tm_rm; if(tm_verbose) printf("%8.2f      \b\b\b\b\b", TMBS(_len_, dm*TM_C/dr) );\
  if(tm_verbose>1) printf("%s ", _name_?_name_:#_func_);\
} while(0)

// Check
#define TMBENCHT(_name_,_func_, _len_, _res_)  do { \
  TMBEG(tm_Rep) \
  if(_func_ != _res_) { printf("ERROR: %lld != %lld", (long long)_func_, (long long)_res_ ); exit(0); };\
  TMEND(_len_);\
  if(tm_verbose) printf("%8.2f      \b\b\b\b\b", TMBS(_len_,(double)tm_tm*TM_C/(double)tm_rm) );\
  if(tm_verbose) printf("%s ", _name_?_name_:#_func_ );\
} while(0)
//----------------------------------------------------------------------------------------------------------------------------------
#define Kb (1u<<10)
#define Mb (1u<<20)
#define Gb (1u<<30)
#define KB 1000
#define MB 1000000
#define GB 1000000000

static unsigned argtoi(char *s, unsigned def) {
  char *p;
  unsigned n = strtol(s, &p, 10),f = 1;
  switch(*p) {
    case 'K': f = KB; break;
    case 'M': f = MB; break;
    case 'G': f = GB; break;
    case 'k': f = Kb; break;
    case 'm': f = Mb; break;
    case 'g': f = Gb; break;
    case 'B': return n; break;
    case 'b': def = 0;
    default: if(!def) return n>=32?0xffffffffu:(1u << n); f = def;
  }
  return n*f;
}
static uint64_t argtol(char *s) {
  char *p;
  uint64_t n = strtol(s, &p, 10),f=1;
  switch(*p) {
    case 'K': f = KB; break;
    case 'M': f = MB; break;
    case 'G': f = GB; break;
    case 'k': f = Kb; break;
    case 'm': f = Mb; break;
    case 'g': f = Gb; break;
    case 'B': return n; break;
    case 'b': return 1u << n;
    default:  f = MB;
  }
  return n*f;
}

static uint64_t argtot(char *s) {
  char *p;
  uint64_t n = strtol(s, &p, 10),f=1;
  switch(*p) {
    case 'h': f = 3600000; break;
    case 'm': f = 60000;   break;
    case 's': f = 1000;    break;
    case 'M': f = 1;       break;
    default:  f = 1000;
  }
  return n*f;
}

static void memrcpy(unsigned char *out, unsigned char *in, unsigned n) { int i; for(i = 0; i < n; i++) out[i] = ~in[i]; }

