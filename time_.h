#include <stdint.h>
#include <time.h>

  #ifdef _WIN32
#include <windows.h>
#define sleep(n) Sleep((n) * 1000)
  #else
#include <unistd.h>
#define Sleep(ms) usleep((ms) * 1000)
  #endif

#if defined (__i386__) || defined( __x86_64__ ) 
  #ifdef _MSC_VER  
#include <intrin.h> // __rdtsc
  #else
#include <x86intrin.h>
  #endif

  #ifdef __corei7__
#define RDTSC_INI(_c_) do { unsigned _cl, _ch;				\
  __asm volatile ("couid\n\t"								\
				"rdtsc\n\t"									\
				"mov %%edx, %0\n"							\
				"mov %%eax, %1\n": "=r" (_ch), "=r" (_cl)::	\
				"%rax", "%rbx", "%rcx", "%rdx");			\
  _c_ = (unsigned long long)_ch << 32 | _cl;				\
} while(0)

#define RDTSC(_c_) do { unsigned _cl, _ch;					\
  __asm volatile("rdtscp\n"									\
               "mov %%edx, %0\n"							\
               "mov %%eax, %1\n"							\
               "cpuid\n\t": "=r" (_ch), "=r" (_cl):: "%rax",\
               "%rbx", "%rcx", "%rdx");\
  _c_ = (unsigned long long)_ch << 32 | _cl;\
} while(0)
  #else
#define RDTSC(_c_) do { unsigned _cl, _ch;\
  __asm volatile ("cpuid \n"\
				"rdtsc"\
				: "=a"(_cl), "=d"(_ch)\
				: "a"(0)\
				: "%ebx", "%ecx");\
  _c_ = (unsigned long long)_ch << 32 | _cl;\
} while(0)
#define RDTSC_INI(_c_) RDTSC(_c_)
  #endif
#else
#define RDTSC_INI(_c_)
#define RDTSC(_c_)	  
#endif

#define tmrdtscini() ({ tm_t _c; __asm volatile("" ::: "memory"); RDTSC_INI(_c); _c; })
#define tmrdtsc()    ({ tm_t _c; RDTSC(_c); _c; })

typedef unsigned long long tm_t;
#ifndef TM_F
#define TM_F 1  // TM_F=4 -> MI/s
#endif

  #ifdef RDTSC_ON
#define tminit() tmrdtscini()
#define tmtime() tmrdtsc()
#define TM_T					CLOCKS_PER_SEC
#define TMBS(__l,__t)       	(double)(__t)/((double)(__l))
#define TM_C 1000
  #else
#define TM_T 1000000.0
#define TM_C 1
#define TMBS(__l,__t)         ((__t)>=0.000001?((double)(__l)/(1000000.0*TM_F))/((__t)/TM_T):0.0)
    #ifdef _WIN32
static LARGE_INTEGER tps;
static tm_t tmtime(void) { LARGE_INTEGER tm; QueryPerformanceCounter(&tm); return (tm_t)((double)tm.QuadPart*1000000.0/tps.QuadPart); }
static tm_t tminit() { QueryPerformanceFrequency(&tps); tm_t t0=tmtime(),ts; while((ts = tmtime())==t0); return ts; } 
    #else
      #ifdef __MACH__
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
static   tm_t tmtime(void)    { struct timespec tm; clock_gettime(CLOCK_MONOTONIC, &tm); return (tm_t)tm.tv_sec*1000000ull + tm.tv_nsec/1000; }
static   tm_t tminit()        { tm_t t0=tmtime(),ts; while((ts = tmtime())==t0); return ts; }
    #endif
static double tmsec( tm_t tm) { return (double)tm/1000000.0; }
static double tmmsec(tm_t tm) { return (double)tm/1000.0; }
#endif
//---------------------------------------- bench ----------------------------------------------------------------------
#define TM_TX TM_T
 
#define TMSLEEP do { tm_T = tmtime(); if(!tm_0) tm_0 = tm_T; else if(tm_T - tm_0 > tm_TX) { if(tm_verbose) { printf("S \b\b");fflush(stdout);} sleep(tm_slp); tm_0=tmtime();} } while(0)

#define TMBEG(_tm_reps_, _tm_Reps_) { unsigned _tm_r,_tm_c=0,_tm_R; tm_t _tm_t0,_tm_t,_tm_ts;\
  for(tm_rm = _tm_reps_, tm_tm = 1ull<<63,_tm_R = 0,_tm_ts=tmtime(); _tm_R < _tm_Reps_; _tm_R++) { tm_t _tm_t0 = tminit();\
    for(_tm_r=0;_tm_r < tm_rm;) {
 
#define TMEND(_len_) _tm_r++; if((_tm_t = tmtime() - _tm_t0) > tm_tx) break; } \
  if(_tm_t < tm_tm) { if(tm_tm == 1ull<<63) { tm_rm = _tm_r; /*printf("reps=%d ", tm_rm);*/ } tm_tm = _tm_t; _tm_c++; } \
  else if(_tm_t>tm_tm*1.2) TMSLEEP;   													if(tm_verbose) printf("%8.2f %.2d_%d\b\b\b\b\b\b\b\b\b\b\b\b\b",TMBS(_len_, (double)tm_tm*TM_C/tm_rm),_tm_R+1,_tm_c),fflush(stdout);\
  if(tmtime()-_tm_ts > tm_TX) break;\
  if((_tm_R & 7)==7) sleep(tm_slp),_tm_ts=tmtime(); } }
 
static unsigned tm_rep = 1<<20, tm_Rep = 3, tm_rep2 = 1<<20, tm_Rep2 = 4, tm_slp = 20, tm_rm;
static tm_t     tm_tx = TM_T, tm_TX = 10*TM_T, tm_0, tm_T, tm_verbose=1, tm_tm;
static void tm_init(int _tm_Rep, int _tm_verbose) { tm_verbose = _tm_verbose; if(_tm_Rep) tm_Rep = _tm_Rep; tm_tx =  tminit(); Sleep(500); tm_tx = tmtime() - tm_tx; /*printf("tm_tx=%llu %lld\n", tm_tx, (long long)CLOCKS_PER_SEC);*/ tm_TX = 10*tm_tx; }

#define TMBENCH(_name_, _func_, _len_)  do { if(tm_verbose) printf("%-32s: ", _name_?_name_:#_func_ ); TMBEG(tm_rep, tm_Rep) _func_; TMEND(_len_); if(tm_verbose) printf("%8.2f           \n", TMBS(_len_, (double)tm_tm*TM_C/(double)tm_rm) ); } while(0)
#define TMBENCHF(_name_,_func_, _len_, _res_)  do { if(tm_verbose) printf("%-32s: ", _name_?_name_:#_func_ ); TMBEG(tm_rep, tm_Rep) if(_func_ != _res_) { printf("ERROR: %lld != %lld", (long long)_func_, (long long)_res_ ); exit(0); }; TMEND(_len_); if(tm_verbose) printf("%8.2f           \n", TMBS(_len_,(double)tm_tm*TM_C/(double)tm_rm) ); } while(0)

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
    case 'b': return 1u << n;
	default:  f = def;
  }
  return n*f;
}

static unsigned long long argtol(char *s) {
  char *p;
  unsigned long long n = strtol(s, &p, 10),f=1;
  switch(*p) {
    case 'K': f = KB; break;
    case 'M': f = MB; break;
    case 'G': f = GB; break;
    case 'k': f = Kb; break;
    case 'm': f = Mb; break;
    case 'g': f = Gb; break;
    case 'b': return 1u << n;
	default:  f = MB;	
  }
  return n*f;
}

static unsigned long long argtot(char *s) {
  char *p;
  unsigned long long n = strtol(s, &p, 10),f=1; 
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
