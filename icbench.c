/**
    Copyright (C) powturbo 2013-2014
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

    - email    : powturbo [AT] gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo

    icbench.c - "Integer Compression" benchmark program
**/
   
#define _LARGEFILE64_SOURCE 1 
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
 
#include <getopt.h>
#include <sys/stat.h>
#include <x86intrin.h>

// simple-8b simple16 and optpfd don't work with all interger lists.
// Enable if you to want to test
#define USE_SIMPLE_8B  // crashs on some lists 
#define USE_SIMPLE16   // limited to 28 bits
#define USE_OPTPFD     // compression too slow and limited to 28 bits. crashs on some lists
#define STATS
//---------------------------------------- Platform ------------------------
  #ifdef _WIN32
#define srand48(x) srand(x)
#define drand48() ((double)(rand()) / RAND_MAX)
#define __off64_t _off64_t  
  #endif
//---------------------------------------- Time ---------------------------------------------------------------------
typedef unsigned long long tm_t;
#define TM_TMAX (1ull<<63)

#include <sys/time.h>
#define TM_T 1000000.0
static   tm_t tmtime(void)    { struct timeval tm; gettimeofday(&tm, NULL); return (tm_t)tm.tv_sec*1000000ull + tm.tv_usec; }
static   tm_t tminit()        { tm_t t0=tmtime(),ts; while((ts = tmtime())==t0); return ts; }
static double tmsec( tm_t tm) { return (double)tm/1000000.0; }
static double tmmsec(tm_t tm) { return (double)tm/1000.0; }

//-------------------------------------- TurboPFor ------------------------------------------------------------------
#include "vint.h"      
#include "vsimple.h"

#include "bitpack.h"
#include "bitunpack.h"

#include "vp4dc.h"
#include "vp4dd.h"

unsigned char *u32enc(unsigned *__restrict in, int n, unsigned *__restrict out) { unsigned *in_   = in +n; while(in  <  in_) *out++ = *in++; return (unsigned char *)out;}
unsigned char *u32dec(unsigned *__restrict in, int n, unsigned *__restrict out) { unsigned *out_  = out+n; while(out < out_) *out++ = *in++; return (unsigned char *)in; }

#define PAD8(__x) (((__x)+7)/8)
unsigned char *_bitunpackx32( unsigned char *__restrict in, unsigned n, unsigned b,            unsigned *__restrict out) { unsigned i,k=0; for(i=0; i < n; i++,k+=b ) *out++ = _bitgetx32(in, b, k); return in + PAD8(n*b); }

unsigned char *bitdunpackx32( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned *__restrict out) { int i; for(i = 0; i < n; i++) out[i] = (start += bitgetx32(in, b, i)+1); return in + PAD8(n*b); }
unsigned char *bitd0unpackx32(unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned *__restrict out) { int i; for(i = 0; i < n; i++) out[i] = (start += bitgetx32(in, b, i)+1); return in + PAD8(n*b); }
unsigned char *bitfunpackx32( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned *__restrict out) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, b, i)+start+i+1; return in + PAD8(n*b); }
unsigned char *bitf0unpackx32(unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned *__restrict out) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, b, i)+start; return in + PAD8(n*b); }
//-------------------------------------- External functions for comparison ------------------------------------------------------------------------
#include "ext/vabyte.h"
#include "ext/varintg8iu.h"
#include "ext/varintg8iu.h"
#include "ext/simdcomp/include/simdbitpacking.h"

  #ifdef USE_SIMPLE16
#include "ext/vas16c.h"
#include "ext/vas16d.h"
  #endif
  #ifdef USE_OPTPFD
#include "ext/OPT_PFD/opt_p4.h"
  #endif
  #ifdef USE_SIMPLE_8B
#include "ext/simple8b.h"
  #endif

unsigned char *simdpackwn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpackwithoutmask(in, (__m128i *)out, b);
  return (unsigned char *)out;
}

unsigned char *simdpackn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpack(in, (__m128i *)out, b);
  return (unsigned char *)out;
}

unsigned char *simdunpackn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {  
  uint32_t k, *out_; 
  for(out_ = out + n; out + 128 <= out_; out += 128, in += 4 * b) simdunpack((const __m128i *)in, out, b); 
  return (unsigned char *)in;
}

unsigned char *simdpackwn1(uint32_t *in, uint32_t n, uint32_t b, uint32_t start, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpackwithoutmaskd1(start, in, (__m128i *)out, b); //simdpackwithoutmaskd1(x, ip+1, (__m128i *)out, b); 
  return (unsigned char *)out;
}

unsigned char *simdunpackn1(uint32_t *in, uint32_t n, uint32_t b, uint32_t start, uint32_t *out) {
  uint32_t k, *out_; 
  for(out_ = out + n; out + 128 <= out_; out += 128, in += 4 * b) simdunpackd1(start, in, out, b); 
  return (unsigned char *)in;
}

#include "ext/vbyte_poly.h"
unsigned char *vbpolyenc(int *in, int n, unsigned char *out) {
  int i; for(i = 0; i < n; i++) { unsigned x = in[i]; VBYTE_ENC(out, x); } return out;
}
unsigned char *vbpolydec(unsigned char *in, int n, int *out) {
  int i; for(i = 0; i < n; i++) { unsigned x; VBYTE_DEC(in, x); out[i] = x; } return in;
}
//-------------------------------------------------------------------------------------------------------------------
#define BLK_SIZE  (64*1024)
#define PACK_SIZE 128
enum { 
  P_CPY, 
  P_VB, 		P_VBL, P_VG8, P_VBP, P_VBI,		
  P_PCK, P_PCKR, 	P_SIMDH,
  P_SV, 		P_S16, P_S64, 
  P_P4D, P_P4DR, 	P_OPTP4, 													
}; 

//------------------------------------------------ random integer array (not sorted) ---------------------------------------------------------------------------
unsigned char *beenc(unsigned *in, size_t n, unsigned char *out, int id, int b) { int i;
  switch(id) {
    case P_CPY:   return u32enc(   in, n, (unsigned *)out);

      // --------- variable byte -------------------------------------------
    case P_VB:    return vbenc(    in, n, out);

    case P_VBL:   return vbyteenc( in, n, (unsigned *)out);
    case P_VG8:   return vintg8enc(in, n, out);
    case P_VBP:   return vbpolyenc(in, n, out);
      // --------- simple family: simple16, simpleV, simple64 ---------------
    case P_SV:    return vsenc32(  in, n, out);

    case P_S16:   return vs16enc(  in, n, (unsigned *)out); 
    case P_S64:   return vs8benc(  in, n, out); 
      // --------- PFor -----------------------------------------------------
    case P_P4DR:
    case P_P4D:   return p4denc32(in, n, out);

    case P_OPTP4: if(n < 128) return vbyteenc(in, n, (unsigned *)out); 
                  else { unsigned tmp[2048]; for(i = 0; i < n; i++) tmp[i] = in[i]; return out +  OPT4(tmp, n, (unsigned *)out); }
      // --------- bit packing -------------------------------------------
    case P_PCKR: 
    case P_PCK:   if(b < 0) { for(b=0,i=0; i < n; i++) b |= in[i]; b = bsr32(b); *out++ = b; } return bitpack32(in, n, b, out); 

    case P_SIMDH: // bit packing from FastPFor
      if(n < 128) return vbyteenc(in, n, (unsigned *)out); 
      else {      if(b < 0) { for(b=0,i=0; i < n; i++) b |= in[i]; b = bsr32(b); *out++ = b; } return simdpackwn(in, n, b, (unsigned *)out); } 
    default: die("Fatal- Not entry %d", id);
  } 
}

unsigned char *bedec(unsigned char *in, size_t n, unsigned *out, int id, int b) {
  switch(id) {         
    case P_CPY:    return u32dec(   (unsigned *)in, n, out);
      // --------- variable byte -------------------------------------------
    case P_VB:     return vbdec(    in, n, out); 

    case P_VBL:    return vbytedec( in, n, out); 
    case P_VG8:    return vintg8dec(in, n, out);
    case P_VBP:    return vbpolydec(in, n, out);
      
      // --------- simple family: simple16, simpleV, simple64 ---------------
    case P_SV:     return vsdec32(  in, n, out); 

    case P_S16:    return vs16dec(  (unsigned *)in, n, out);  
    case P_S64:    return vs8bdec(  in, n, out); 

      // --------- PFor -----------------------------------------------------
    case P_OPTP4 : if(n < 128) return vbytedec(in, n, out); else { unsigned all_array[2048]; return (unsigned char *)detailed_p4_decode(out, (unsigned *)in, all_array); }
    case P_P4D :   return p4ddec32(  in, n, out); 
    case P_P4DR :  return p4ddecx32( in, n, out);

      // --------- bit packing -------------------------------------------
    case P_PCK:    if(b < 0) b = *in++; return bitunpack32(             in, n, b, out);
    case P_PCKR:   if(b < 0) b = *in++; return _bitunpackx32(           in, n, b, out);

    case P_SIMDH:  
      if(n < 128)  return vbytedec(in, n, out);
      else       { if(b < 0) b = *in++; return simdunpackn( (unsigned *)in, n, b, out); }
    default:       die("Fatal- Not entry %d", id);
  }
  return in;
}

//------------------------------------------------- Sorted integer array : Delta/Differential compression ------------------------------------------------
//#define DELTA(in, n, mode, pa) for(pa[0]=in[0],v = 1; v < n; v++) pa[v] = in[v] - in[v-1] - mode
#define DELTA( __in, __n, __mode, __pa) {      unsigned _v; for(      __pa[0]=__in[0],_v = __n-1; _v >   0; --_v) __pa[_v] = (__in[_v] - __in[_v-1]) - __mode; }
#define DELTAB(__in, __n, __mode, __b, __pa) { unsigned _v; for(__b=0,__pa[0]=__in[0],_v = __n-1; _v >   0; --_v) __pa[_v] = (__in[_v] - __in[_v-1]) - __mode, __b |= __pa[_v]; __b = bsr32(__b); }

#define DELTR( __in, __n, __mode, __pa) {      unsigned _v; for(      __pa[0]=__in[0],_v = 1;     _v < __n; _v++) __pa[_v] = (__in[_v] - __pa[0]) -   _v*__mode; }
#define DELTRB(__in, __n, __mode, __b, __pa) { unsigned _v; for(__b=0,__pa[0]=__in[0],_v = 1;     _v < __n; _v++) __pa[_v] = (__in[_v] - __pa[0]) -   _v*__mode, __b |= __pa[_v]; __b = bsr32(__b); }

unsigned char *besenc(unsigned *in, size_t n, unsigned char *out, int id, int mode) { 
  unsigned pa[BLK_SIZE+2048],x; unsigned b;

  switch(id) {
    case P_CPY:       										     return u32enc( in, n, (unsigned *)out);
	  //----------- Variable byte ----------------------------------------------------------------------------------------
    case P_VB:    DELTA( in, n, mode, pa); 					     return vbenc(    pa, n, out);

    case P_VBL:   DELTA( in, n, mode, pa); 					     return vbyteenc( pa, n, (unsigned *)out); 
    case P_VBP:   DELTA( in, n, mode, pa); 					     return vbpolyenc(pa, n, out);
    case P_VG8:   DELTA( in, n, mode, pa); 		                 return vintg8enc(pa, n, out);
       // --------- Simple family ---------
	case P_SV:    DELTA( in, n, mode, pa);    vbput(out, pa[0]); return vsenc32(  pa+1, n-1, out); 

    case P_S16:   DELTAB(in, n, mode, b, pa); 							          										if(b>28) die("simple16 overflow.bits size>28\n"); 
                   						      vbput(out, pa[0]); return vs16enc(  pa+1, n-1, (unsigned *)out); 
	case P_S64:   DELTA( in, n, mode, pa); 																				if(b>28) die("simple-8b overflow.bits size>28\n");
                  vbput(out, pa[0]); return vs8benc(  pa+1, n-1, out);
	   // --------- PFor -------------------------------------------------------------------------------------------------
    case P_P4D:   DELTA( in, n, mode, pa);    vbput(out, pa[0]); return p4denc32(  pa+1, n-1,             out);
	case P_P4DR:  DELTR( in, n, mode, pa);    vbput(out, pa[0]); return p4denc32(  pa+1, n-1,             out);
    
    case P_OPTP4: DELTAB(in, n, mode, b, pa); 																			if(b>28) die("optp4 overflow.bits size>28\n");   
                  if(n < 129) {                                  return vbenc(pa, n, out); } 
	              else {	                  vbput(out, pa[0]); return out + OPT4(pa+1, n-1, (unsigned *)out); }
      // --------- bit packing -----------------------------------------------------------------------------------------------
    case P_PCK:   DELTAB(in, n, mode, b, pa); vbput(out, pa[0]); *out++=b; return bitpack32(pa+1, n-1, b, out);		
    case P_PCKR:  DELTRB(in, n, mode, b, pa); vbput(out, pa[0]); *out++=b; return bitpack32(pa+1, n-1, b, out);
    
    case P_SIMDH: 																					
      if(n < 129) { DELTA(in, n, mode, pa);                                return vbyteenc((unsigned *)pa, n, (unsigned *)out); } 
	  else { b = simdmaxbitsd1(in[0], in+1);  vbput(out, in[0]); *out++=b; return simdpackwn1((unsigned *)(in+1), n-1, b, in[0], (unsigned *)out); } 
  }
} 

#define UNDELTA(__out, __n, __mode) { unsigned _x,_v;\
  for(_x = __out[0],_v=1;_v<__n;_v+=4) {\
    __out[_v  ] = (_x += __out[_v  ] + __mode);\
    __out[_v+1] = (_x += __out[_v+1] + __mode);\
    __out[_v+2] = (_x += __out[_v+2] + __mode);\
    __out[_v+3] = (_x += __out[_v+3] + __mode);\
  }\
}

unsigned char *besdec(unsigned char *in, size_t n, unsigned *out, int id, int mode) { unsigned b,x,v;
  switch(id) {
    case P_CPY: 		                   in = u32dec( (unsigned *)in, n,   out);   						break;
      //------------- Variable byte ---------------------------------------------- 
    case P_VB: 			                   in = vbdec(              in, n,   out);   UNDELTA(out, n, mode); break;

    case P_VBL: 		                   in = vbytedec(           in, n,   out);   UNDELTA(out, n, mode); break;
    case P_VBP: 	                       in = vbpolydec(          in, n,   out);   UNDELTA(out, n, mode); break;
    case P_VG8:		                       in = vintg8dec(          in, n,   out);   UNDELTA(out, n, mode); break;    
      //------------- Simple family ----------------------------------------------
    case P_SV:    vbgeta(in, x, *out = x); in = vsdec32(            in, n-1, out+1); UNDELTA(out, n, mode); break;  

    case P_S16:   vbgeta(in, x, *out = x); in = vs16dec((unsigned *)in, n-1, out+1); UNDELTA(out, n, mode); break;
	case P_S64:   vbgeta(in, x, *out = x); in = vs8bdec(            in, n-1, out+1); UNDELTA(out, n, mode); break;
      // ------------ PFor -------------------------------------------------------
    case P_P4D:   vbgeta(in, x, *out = x); in = p4ddec32(           in, n-1, out+1); UNDELTA(out, n, mode); break;
    case P_P4DR:  vbgeta(in, x, *out = x); return mode?p4dfdecx32(in, n-1, x, out+1):p4df0decx32( in, n-1, x, out+1);

    case P_OPTP4: 
      if(n < 129)                          in = vbdec(in, n, out); 			              
      else {      vbgeta(in, x, *out = x); unsigned all_array[2048]; in = (unsigned char *)detailed_p4_decode(out+1, (unsigned *)in, all_array); }
       																								   UNDELTA(out, n, mode);
      break;
      // --------- bit packing ----------------------------------------
    case P_PCK:   vbgeta(in, x, *out = x); b = *in++; return mode?bitdunpack32( in, n-1, b, x, out+1):bitd0unpack32( in, n-1, b, x, out+1);
    case P_PCKR:  vbgeta(in, x, *out = x); b = *in++; return mode?bitfunpackx32(in, n-1, b, x, out+1):bitf0unpackx32(in, n-1, b, x, out+1);

    case P_SIMDH: 
      if(n < 129) { in = vbytedec(in, n, out); 									               UNDELTA(out, n, mode); }
      else {    vbgeta(in, x, *out = x);  b = *in++; in = simdunpackn1((uint32_t *)in, n-1, b, out[0], out+1); }
      break;
  }
  return in;
}

//--------------------------------------- Zipfian generator --------------------------------------------------------
int z_cmp(double **a, double **b) {
  if(*a < *b) return -1;
  if(*a > *b) return  1;
  return 0;
}

void zipfgen(unsigned *a, double alpha, unsigned x1, unsigned x2, int n) { 
  int    i,m = x2 - x1 + 1; 
  double prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error\n"); 

  // generate initial set (slow) 
  srand48(1); 
  for(cum = 0.0,i = 0; i < m; i++) 
    cum += 1.0 / pow(i+1, alpha); 
  cum = 1.0 / cum;
  for(prob = 0.0,i = 0; i < m; i++) 
    zmap[i] = prob += cum / pow(i+1, alpha);
	
  // use binary search to speed up zipfgen
  qsort(zmap, m, sizeof(zmap[0]), (int(*)(const void*,const void*))z_cmp);
  for(i = 0; i < n; i++) { 
    double r = drand48(); 
    int    l = 0, h = m-1; 
    while(l < h) { 
      int k = (l + h) >> 1; 
      if(r > zmap[k]) l = k + 1; 
      else h = k; 
    }
    a[i] = x1 + l;	 
  } 
  free(zmap);
}
 
//----------------------------------------------- Benchmark -------------------------------------------------------------------
struct libss { int id; char *s;int size; };

struct libss libss[] = { 
  { P_CPY,   "Copy", 0       },
  //---------------- Variable byte ---------------------------------
  { P_VB,    "TurboVbyte", 0	},
  { P_VBL,   "VbyteFPF", 0 	},
  { P_VG8,   "VarintG8IU",0 	}, 
//{ P_VBP,   "VBytePoly"	},
  // -------------- Simple family ---------------------------------- 
  { P_SV,    "SimpleV",  0 	},
    #ifdef USE_SIMPLE_8B
  { P_S64,   "Simple-8b",SIMPLE8BMAX 	},  //crash on 32 bits?
    #endif
    #ifdef USE_SIMPLE16
  { P_S16,   "Simple16", 0 	},  //max. 28 bits
    #endif
  //--------------- PFor ------------------------------------------
    #ifndef _WIN32    
  { P_P4DR,  "TurboPForDA", 128 }, // actually not working w. mingw 
    #endif
  { P_P4D,   "TurboPFor",   128	}, 

    #ifdef USE_OPTPFD
  { P_OPTP4, "OptPFD", 128 }, //max. 28 bits
    #endif
  //-------------- Bit Packing ------------------------------ 
  { P_PCK,   "TurboPack",   PACK_SIZE	},
  { P_PCKR,  "TurboPackDA", PACK_SIZE	},
  { P_SIMDH, "SIMDPackFPF", 128	},

  { -1, "" },
};

#define MB 1000000
int verb = 0, reps = 1<<24, trips = 1, xcheck=1;
unsigned xbits[33];
enum { T_DUP, T_UNI, T_TXT, T_BYTE, T_TST  };

struct libs { int id,err,size; char *s,*v; unsigned long long l, c[33]; double tc,td; };
struct libs libs[64];
int libini() { int m; for(m = 0; libs[m].id >= 0; m++) libs[m].l = libs[m].tc = libs[m].td = 0; }

int l_cmp(struct libs *a, struct libs *b) {
  if(a->l  < b->l || a->l == b->l && a->td < b->td) return -1;
  if(a->l  > b->l || a->l == b->l && a->td > b->td) return  1;
  return 0;
}

void check(unsigned *in, unsigned n, unsigned *out, char *s) {
  unsigned k,j; 
  for(k = 0; k < n; k++) 
    if(in[k] != out[k]) { 
      printf("\nFATAL in check %x,%x at %u[%u] in %s\n", in[k], out[k], k, n, s); 
   for(j=k & 0xffffff80u; j < k+128;j++) 
     printf("%d:%x,%x ", j, in[j], out[j]  );printf("\n");
   exit(0); 
  } 
}

void stprint() {
  int m;
  unsigned long long t=0; 
  for(m = 0; m < 33; m++) 
    t += xbits[m];
  printf("\nbit size histogramm:"); 
  for(m = 0; m < 33; m++) 
    if(xbits[m]) printf("%d:%.2f%% ", m, (double)xbits[m]*100/t); printf("\n");
}

void print(unsigned long long n, char *s, unsigned long long *u) { 
  int m, k; 
  for(k = 0; libs[k].id >= 0; k++);
    qsort(libs, k, sizeof(libs[0]), (int(*)(const void*,const void*))l_cmp);	
  char *prtname = s?s:""; { unsigned char *p; if((p = strrchr(prtname, '/')) || (p = strrchr(prtname, '\\'))) prtname = p+1;} 
  for(m = 0; m < k; m++) 
    if(libs[m].l) { 
      struct libs *lb = &libs[m]; 
      printf("%-16s%12llu\t%5.2f\t%5.2f\t%8.2f\t%8.2f\t%s\n", prtname, lb->l, (double)lb->l*100.0/((double)n*4.0), (double)lb->l*8.0/(double)n, 
        lb->tc>=0.000001?((double)n/1000000.0) / (lb->tc/TM_T):0.0,
        lb->td>=0.000001?((double)n/1000000.0) / (lb->td/TM_T):0.0,
        lb->s ); 
      if(u && verb>3) { printf("\n");for(k = 0; k < 33; k++) if(u[k]) printf("%d:%.1f\t", k, (double)lb->c[k]*100/u[k]); printf("\n"); }
    }
}

unsigned bench(unsigned *__restrict _in, unsigned _inlen, int blksize, unsigned char *__restrict _out, unsigned long long outsize, char *inname, tm_t tx, unsigned *__restrict cpy, int bb, int mode ) { int m,id,b=bb,i; 
  if(!_inlen) return 0;													if(verb>1) { printf(":%d,", _inlen);  fflush(stdout); }
  unsigned cn; 															tm_t tt0 = tminit();
  for(i = 0; i < 10; i++) memcpy(_out, _in, _inlen); 																																			
  for(m = 0; (id=libs[m].id) >= 0; m++) { 
    blksize = libs[m].size?libs[m].size:blksize;
    int r,insize=(mode>=0)?blksize+1:blksize; 
    struct libs *lb = &libs[m]; 
    unsigned cl,cc[33];													if(verb) printf("%s,%d", libs[m].s, blksize);
    int t,tj; 															tm_t t0,tc=TM_TMAX,td=TM_TMAX,tt;
    for(t = 0; t < trips; t++) { 										t0 = tminit();
      for(r = 0; r < reps; ) { 
        for(cl=0; cl<33; cl++) cc[cl]=0; cn=cl=0;
        unsigned *in; 													
        unsigned char *out; 					        				
        for(out = _out, in = _in; in < _in+_inlen; ) { 
          unsigned n,inlen = *in++,*ip=in; in += inlen; 
          *(unsigned *)out = inlen; out += 4; unsigned char *sout = out;					
          for(;ip < in; ip += n) { n = ip+insize <= in?insize:in-ip; 	cn += n;  if(out+5*n > _out+outsize) die("Overflow error %llu, %u in %s\n", outsize, (int)(ptrdiff_t)(out - _out), lb->s);
            out = mode >= 0?besenc(ip, n, out, id, mode):beenc(ip, n, out, id, bb);            	
          } 															
          cl += out - sout; cc[bsr32(inlen)] += out - sout;
        } 
		r++;															if((tt = tmtime() - t0) > tx) break;
      } 																if(tt < tc) tc = tt, tj = r;								//if(tmtime() - tt0 > tx*trips) { sleelp(5); tt0 = tminit(); }
    }
      
    for(t=0; t < 33; ++t) lb->c[t] += cc[t]; 

    lb->l += cl; lb->tc += (double)tc/tj; memset(cpy, 0xf, _inlen*4);			if(verb) { printf("/");fflush(stdout);}    
																		tt0 = tminit();    
    for(t = 0; t < trips; t++) {										t0 = tminit();
      for(r = 0; r < reps; ) { 
        unsigned *out; unsigned char *in;
        for(out = cpy, in = _out; out < cpy+_inlen;) { 
          unsigned n,*op, outlen=*(unsigned *)in; in+=4; 
          *out++ = outlen;
          for(op=out,out += outlen; op < out; op += n) { 
            n = op + insize<=out?insize:out-op;
            in = mode>=0?besdec(in,n,op,id, mode):bedec(in,n,op,id,bb);
          }
        }
	    r++;															if((tt = tmtime() - t0) > tx) break;
      }																	if(tt < td) td = tt, tj = r;
																		//if(tmtime() - tt0 > tx*trips) tt0 = tminit();      
    } 
    lb->td += (double)td/tj; 
    if(xcheck) check(_in, _inlen, cpy, lb->s);
  }
  return cn;
}

void usage() {
  fprintf(stderr, "\nTurboPFor Copyright (c) 2013-2015 Powturbo  %s\n", __DATE__);
  fprintf(stderr, "Usage: icbench [options] [file]\n");
  fprintf(stderr, "Use zipfian generator when no file specified\n");
  fprintf(stderr, "<options>\n");
  fprintf(stderr, " -bNm    N = blocksize (default 128) m=k kilobyte ex. -b64k\n");
  fprintf(stderr, " -cN     N = format ordered(0:delta+0,1:delta+1),2=convert text to integer format\n");
  fprintf(stderr, " -eS     S = encoder schemes sparated by / (default all)\n");
  fprintf(stderr, " -tN     N = time in seconds per interation\n");
  fprintf(stderr, " -TN     N = Iterations (default 3)\n");
  fprintf(stderr, " -vN     N = verbosity 1..3\n");
  fprintf(stderr, "----- file specified --------------\n");
  fprintf(stderr, " -rN     N = max. file size to read\n");
  fprintf(stderr, "Ex. ./icbench -c1 gov2.sorted -eturbopack/turbopfor\n");
  fprintf(stderr, "----- file not specified --------------\n");
  fprintf(stderr, " -aF     F = zipfian distribution alpha ex. -a1.0 uniform -a1.5 skewed\n");
  fprintf(stderr, " -mN     N = minimum integer generated in bits\n");
  fprintf(stderr, " -MN     N = maximum integer generated in bits\n");
  fprintf(stderr, " -nN     N = number of integers to generate\n");
  fprintf(stderr, "Ex. ./icbench -a1.0 -m0 -x8 -n100000000\n");
  exit(0);
}

#define OVD (10*MB)
int main(int argc, char *argv[]) { int r;
  char      fname[0x100], *cmd=NULL;
  unsigned  xbp=0, rm=0,rx=30,n=0;
  int       mode = -1;  
  long long rdmax = 1ull<<32; 
  double    a = 1.5;
  tm_t      tx=1*1000000; 
  unsigned  blksize = PACK_SIZE;
  tminit();
  VarIntG8IU();  
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"repeat", 	0, 0, 'r'}, {0,0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "BshHa:b:c:e:f:m:n:r:R:T:v:M:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;								
      case 'a': a = strtod(optarg, NULL); 	break;
      case 'b': { char *p; blksize = strtol(optarg, &p, 10); if(*p == 'k' || *p == 'K') blksize *= 1024; if(blksize>BLK_SIZE) blksize = BLK_SIZE; } break;
      case 'c': mode = atoi(optarg); 		break;
      case 'f': rdmax = atoi(optarg)*MB; 	break;
      case 'h': usage(); 		            break;
      case 'H': xcheck=0; 		            break;
      case 'e': cmd = optarg; 				break;
      case 'm': rm = atoi(optarg); 			break;
      case 'n': { char *p; n = strtol(optarg, &p, 10); if(*p == 'k' || *p == 'K') n *= 1000; else if(*p == 'b' || *p == 'B') n *= 1000000000; else n *= 1000000; } break;
      case 'r': reps = atoi(optarg); 		break;
      case 'R': trips = atoi(optarg); 	   	break;
      case 't': tx = atoi(optarg)*1000000;	break;
      case 'v': verb = atoi(optarg); 		break;
      case 'M': rx = atoi(optarg); 	   		break;
      default:  usage();
    }
  }																														
  int fno,i=0; 
  if(!xbp) { rm = (1<<rm)-1; rx = (1ull<<rx)-1; if(n && rx > n) rx = n; } else if(!rm) rm = 1;   //printf("range=(min=%u, max=%u)\n", rm, rx);fflush(stdout);
  // build the test functions set
  struct libss *ls; 
  if(cmd) {						
    char *q = NULL;
    for(i=0,libs[0].id = -1;;) { 
      if(cmd) { 
        if(!*cmd) break; 							
        q = strchr(cmd,','); 
        if(q) *q=' '; 
        if(q = strchr(cmd,'/')) 
          *q = '\0';
        for(ls = libss; ls->id >= 0; ls++) 
          if(!strcasecmp(ls->s, cmd)) { 
            memset(&libs[i], 0, sizeof(struct libs)); 
            libs[i].id     = ls->id; 
            libs[i].err    = 0; 
            libs[i].s      = ls->s; 
            libs[i++].size = ls->size; 										if(verb) printf("%s/", ls->s);fflush(stdout);
            break; 
          }
        if(ls->id < 0) die("library: '%s' not found\n", cmd); 
        cmd = q?(q+1):"";
      }
    }
  } else for(ls = libss; ls->id >= 0; ls++) { 
    libs[i].id     = ls->id; 
    libs[i].err    = 0; 
    libs[i].s      = ls->s;  								    			if(verb) printf("%s/", ls->s);fflush(stdout);
    libs[i++].size = ls->size;    
  }
  libs[i].id = -1;															if(verb) printf("\n");
  
  if(argc <= optind) { // No file specified
    if(!n) n = 100000000; if(rx > n) rx = n;
    unsigned *in, *cpy,*ip; unsigned char *out; unsigned long long totlen=0; 
    in  = malloc(n*4+OVD);     if(!in)  die("malloc err=%u", n); 
    out = malloc(n*4+OVD);     if(!out) die("malloc err=%u", n); 
    cpy = malloc(n*4+OVD);     if(!cpy) die("malloc err=%u", n); 
    char s[33]; s[0]=0; 
    if(mode == T_TST) { // Unit test for fixed bit sizes
      int b; 
      printf("bittest\n"); fflush(stdout);
      for(b = rm; b <= rx; b++) { 
        sprintf(s,"b=%d", b);        
        *in = n; 
        for(i = 1; i <= n; i++) 
          in[i] = (1ull << b)-1;
        totlen = bench(in+1, n, blksize, out, n*4+OVD, s, tx, cpy, b, mode);
        print(totlen, s, NULL);
      }
    } else { 			// Benchmark w. generated data
      printf("zipf alpha=%3.1f range[%u..%u].\n ", a, rm, rx); 
      *in = n; 
      zipfgen(in+1, a, rm, rx, n);   							for(i = 1; i <= n; i++) xbits[bsr32(in[i])]++; stprint();
      if(mode>=0) { unsigned *ip=in+1; int v; for(v = 1; v < n; v++) { ip[v] += ip[v-1] + mode; if(ip[v]>(1u<<28)) die("overflow generating sorted array\n" );  } }
      totlen = bench(in, n+1, blksize, out, n*4+OVD, s, tx, cpy, -1, mode);
      print(totlen, s, NULL); 
    }
    free(in);
    free(cpy);
    free(out);
  } else for(fno = optind; fno < argc; fno++) { // Benchmark w. specified data files
    libini();
    char *inname = argv[fno]; 
    if(mode == T_TXT || mode == T_BYTE) { //------------ convert text file to integer array format
      FILE *fi = fopen(inname, "r"); if(!fi) { fprintf(stderr, "open error '%s'", inname); perror(inname); exit(-1); } 
      char outname[257]; strcpy(outname, inname); strcat(outname, ".dat");
      FILE *fo = fopen(outname, "wb"); if(!fo) { fprintf(stderr, "open error '%s'", inname); perror(inname); exit(-1); } 
	  #define LSIZE 16
      char s[LSIZE+1];
      unsigned num = 0;
      fwrite(&num, 1, 4, fo);
      if(mode == T_TXT) { 
	    while(fgets(s, LSIZE, fi)) {
          s[strlen(s) - 1] = 0;
		  unsigned i = strtoul(s, NULL, 10);
          fwrite(&i, 1, 4, fo);
		  num++;
        }
      } else {
        unsigned u;
        unsigned char c;
        while(fread(&c, 1, 1, fi)>0){
          u = c;
          fwrite(&u, 1, 4, fo);
          num++;
        }
      }

	  fseeko(fo, 0, SEEK_SET);
      fwrite(&num, 1, 4, fo);											 					printf("num=%u\n", num);
	  fclose(fo);
	  fclose(fi);
	  continue; 
    }   
	// process integer array file 
    FILE *fi = fopen64(inname, "rb"); 
    if(!fi) { fprintf(stderr, "open error '%s'", inname); perror(inname); exit(-1); } 
    fseeko(fi, 0, SEEK_END); 
    unsigned long long fisize = ftello(fi); fseeko(fi, 0, SEEK_SET); 						//printf("fisize=%llu\n", fisize);
    if(fisize > rdmax) fisize = rdmax; 
    fisize /= 4; 			
				
    unsigned *in, *cpy,*ip,num;  unsigned char *out; 
    unsigned long long outsize=fisize*5+OVD,totlen=0,bitslen[33]={0}; 
    out = malloc(outsize);   if(!out) die("malloc err=%llu", fisize); 
    cpy = malloc(fisize*4+OVD);   if(!cpy) die("malloc err=%llu", fisize);												
    in  = malloc(fisize*4+1024);  if(!in)  die("malloc err=%llu", fisize); 	
    
	ip = in;
    while(fread(&num, 1, 4, fi) == 4 && num) {  											//printf("?");fflush(stdout);						
      if(num < rm || num > rx) { fseeko(fi, num*4, SEEK_CUR); continue; }	
	  if(ip+num > in+fisize) { 
	    totlen += bench(in, ip-in, blksize, out, outsize, inname, tx, cpy, -1, mode); printf("#%u", (unsigned)(totlen/1000000));fflush(stdout);
          if(n && totlen > n) 
            break;
          ip = in;
	  }												
      *ip++ = num; if(fread(ip, 4, num, fi) != num) break;
	  bitslen[bsr32(num)] += num*4; 
        #ifdef STATS
	  unsigned *ep = ip+num,insize=(mode>=0)?blksize+1:blksize;
      while(ip < ep) {
        unsigned m = min(ep-ip, insize),i;
        if(mode >= 0) {
          for(i = 1; i < m; i++) {	 							
            if(verb>3) printf(":%u ", ip[i]);fflush(stdout); 
            xbits[bsr32((ip[i] - ip[i-1]) - mode)]++; 
            if(ip[i] < ip[i-1]+mode) die("IDs in '%s' not sorted.[did=%u,%u] at line=%d\n", inname, ip[i], ip[i-1], (int)(ip-in));
          }
        } else for(i = 0; i < m; i++) xbits[bsr32(ip[i])]++;
        ip += m;									
      }
        #else
      ip += num;
        #endif
    }
	a:fclose(fi);
	totlen += bench(in, ip-in, blksize, out, outsize, inname, tx, cpy, -1, mode);	
    printf("#%u", (unsigned)(totlen/1000000));
	free(in); free(cpy); free(out);													
    stprint(); print(totlen,inname, bitslen);	
  }			
}
