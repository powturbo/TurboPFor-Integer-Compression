/**
    Copyright (C) powturbo 2013-2017
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
//   Nibble/Byte transpose

#ifndef ESIZE
#include <string.h>
  #ifdef __AVX2__
#include <immintrin.h>
  #elif defined(__SSSE3__)
#include <tmmintrin.h>
  #elif defined(__SSE2__)
#include <emmintrin.h>
  #endif
#include "conf.h"
#include "transpose.h"

#define powof2(n) !((n)&((n)-1))

#define TPENC tpenc
#define TPDEC tpdec

#define ESIZE 3
#define STRIDE ESIZE
#include "transpose.c"

#define ESIZE 16
#define STRIDE ESIZE
#include "transpose.c"

#define ESIZE 2

#define STRIDE ESIZE
#define TPENC128V   tpenc128v
#define TPDEC128V   tpdec128v
#define TPENC256V   tpenc256v
#define TPDEC256V 	tpdec256v
#include "transpose.c"

#define STRIDE 4
#define TPENC128V   tp4enc128v
#define TPDEC128V   tp4dec128v
#define TPENC256V   tp4enc256v
#define TPDEC256V 	tp4dec256v
#include "transpose.c"

#define ESIZE 4

#define STRIDE ESIZE
#define TPENC128V   tpenc128v
#define TPDEC128V   tpdec128v
#define TPENC256V   tpenc256v
#define TPDEC256V 	tpdec256v
#include "transpose.c"

#define STRIDE 8
#define TPENC128V   tp4enc128v
#define TPDEC128V   tp4dec128v
#define TPENC256V   tp4enc256v
#define TPDEC256V 	tp4dec256v
#include "transpose.c"

#define ESIZE 8

#define STRIDE ESIZE
#define TPENC128V   tpenc128v
#define TPDEC128V   tpdec128v
#define TPENC256V   tpenc256v
#define TPDEC256V 	tpdec256v
#include "transpose.c"

#define STRIDE 16
#define TPENC128V   tp4enc128v
#define TPDEC128V   tp4dec128v
#define TPENC256V   tp4enc256v
#define TPDEC256V 	tp4dec256v
#include "transpose.c"

//--------------------- CPU detection -------------------------------------------
#if defined(_MSC_VER) || defined (__INTEL_COMPILER)
#include <intrin.h>
#endif

#if !defined(SSE2_ON) && !defined(AVX2_ON) 
static inline void cpuid(int reg[4], int id) {	
    #if defined (_MSC_VER) || defined (__INTEL_COMPILER)       
  __cpuidex(reg, id, 0);                   
    #elif defined(__i386__) || defined(__x86_64__) 
  __asm("cpuid" : "=a"(reg[0]),"=b"(reg[1]),"=c"(reg[2]),"=d"(reg[3]) : "a"(id),"c"(0) : );
    #endif
}

static inline unsigned long long xgetbv (int ctr) {	
    #if(defined _MSC_VER || defined __INTEL_COMPILER)
  return _xgetbv(ctr);                                  
    #elif defined(__i386__) || defined(__x86_64__)
  unsigned a, d;
  __asm("xgetbv" : "=a"(a),"=d"(d) : "c"(ctr) : );
    #else  
  unsigned a=0, d=0;
    #endif
  return (unsigned long long)d << 32 | a;
}

static int _cpuiset;                                  
int cpuini(int cpuiset) { if(cpuiset) _cpuiset = cpuiset; return _cpuiset; }

int cpuiset(void) {
  int c[4] = {0};                               
  if(_cpuiset) return _cpuiset;                                   
  _cpuiset++;                                              
  cpuid(c, 0);                                        
  if(c[0]) {              
    cpuid(c, 1);                                       
    if( c[3] & (1 << 25)) { _cpuiset = 10; // SSE
    if( c[3] & (1 << 26)) { _cpuiset = 20; // SSE2
    if( c[2] & (1 <<  0)) { _cpuiset = 30; // SSE3                                          
    if( c[2] & (1 <<  9)) { _cpuiset = 31; // SSSE3
    if( c[2] & (1 << 19)) { _cpuiset = 40; // SSE4.1 
    if( c[2] & (1 << 23)) { _cpuiset = 41; // +popcount       
    if( c[2] & (1 << 20)) { _cpuiset = 42; 
    if((c[2] & (1 << 28)) &&         
       (c[2] & (1 << 27)) && // OSXSAVE 
	   (c[2] & (1 << 26)) && // XSAVE
	   (xgetbv(0) & 6)==6){ _cpuiset = 50; // AVX
	  if(c[2]& (1 << 25))   _cpuiset = 51; // +AES
      cpuid(c, 7);                                    
      if(c[1] & (1 << 5))   _cpuiset = 52; // AVX2
	}}}}}}}}} 
  return _cpuiset;
}

//---------------------------------------------------------------------------------
typedef void (*TPFUNC)( unsigned char *in, unsigned n, unsigned char *out);

static TPFUNC _tpe[]  = { 0, 0, tpenc2, 	 tpenc3, tpenc4,      0, 0, 0, tpenc8, 	    0, 0, 0, 0, 0, 0, 0, tpenc16 };
static TPFUNC _tpd[]  = { 0, 0, tpdec2, 	 tpdec3, tpdec4,      0, 0, 0, tpdec8, 	    0, 0, 0, 0, 0, 0, 0, tpdec16 };
static TPFUNC _tp4e[] = { 0, 0, tp4enc128v2, tpenc3, tp4enc128v4, 0, 0, 0, tp4enc128v8, 0, 0, 0, 0, 0, 0, 0, tpenc16 }; // Nibble
static TPFUNC _tp4d[] = { 0, 0, tp4enc128v2, tpdec3, tp4dec128v4, 0, 0, 0, tp4enc128v8, 0, 0, 0, 0, 0, 0, 0, tpdec16 };
static int tpset;
 
void tpini(int id) {
  if(tpset) return; 
  tpset++;   
  int i = id?id:cpuiset();
  #if defined(__AVX2__) && defined(AVX2_ON)
  if(i >= 52) {  
    _tpe[2] = tpenc256v2; _tpd[2] = tpdec256v2; _tp4e[2] = tp4enc256v2; _tp4d[2] = tp4dec256v2; 
    _tpe[4] = tpenc256v4; _tpd[4] = tpdec256v4; _tp4e[4] = tp4enc256v4; _tp4d[4] = tp4dec256v4; 
    _tpe[8] = tpenc256v8; _tpd[8] = tpdec256v8; _tp4e[8] = tp4enc256v8; _tp4d[8] = tp4dec256v8; 
  } else 
  #endif
  if(i >= 20) {
    _tpe[2] = tpenc128v2; _tpd[2] = tpdec128v2; _tp4e[2] = tp4enc128v2; _tp4d[2] = tp4dec128v2;
    _tpe[4] = tpenc128v4; _tpd[4] = tpdec128v4; _tp4e[4] = tp4enc128v4; _tp4d[4] = tp4dec128v4;
    _tpe[8] = tpenc128v8; _tpd[8] = tpdec128v8; _tp4e[8] = tp4enc128v8; _tp4d[8] = tp4dec128v8;
  }
}

void tpenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) { if(!tpset) tpini(0);                                   
  TPFUNC f;	
  if(esize <= 16 && (f = _tpe[esize])) f(in,n,out);
  else {
    unsigned bsize = n/esize,i;
    unsigned char *op,*ip;
    for(ip = in,op = out; ip < in+bsize*esize; op++)
      for(i = 0; i < esize; i++)
	    op[i*bsize] = *ip++; 
    for(op = out + esize*bsize; ip < in+n;) 
      *op++ = *ip++;
  }
}  

void tpdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) { if(!tpset) tpini(0); 
  TPFUNC f;	
  if(esize <= 16 && (f = _tpd[esize])) f(in,n,out);	
  else {
    unsigned bsize = n/esize,i;
    unsigned char *op,*ip; 
    for(op = out,ip = in; op < out+bsize*esize; ip++)
	  for(i = 0; i < esize; i++) 
	    *op++ = ip[i*bsize];
    for(ip = in+esize*bsize; op < out+n;) 
	  *op++ = *ip++;
  }
}  

void tp4enc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) { if(!tpset) tpini(0); 
  TPFUNC f;
  if(esize <= 16 && (f = _tp4e[esize])) f(in,n,out);
  else tpenc(in,n,out,esize);
}  

void tp4dec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) { if(!tpset) tpini(0); 
  TPFUNC f;
  if(esize <= 16 && (f = _tp4d[esize])) f(in,n,out);
  else tpdec(in,n,out,esize);  
}
#endif

#else

#if !defined(SSE2_ON) && !defined(AVX2_ON)
  #if STRIDE == ESIZE
void TEMPLATE2(TPENC, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned bsize = n/ESIZE,i;
  unsigned char *op,*ip,*e;
    #if powof2(ESIZE)
  e = in+(n&~(ESIZE-1));
	#else      
  e = in+bsize*ESIZE;		
    #endif
	
  for(ip = in,op = out; ip < e; op++) {
	op[0]        = *ip++; 
	op[i =bsize] = *ip++;
	  #if ESIZE > 2
	op[i+=bsize] = *ip++; 
	    #if ESIZE > 3
	op[i+=bsize] = *ip++; 
	      #if ESIZE > 4
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	        #if ESIZE > 8
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	        #endif	
	      #endif
	    #endif
	  #endif
  }	
  op = out+bsize*ESIZE; while(ip < in+n) *op++ = *ip++;
}  

void TEMPLATE2(TPDEC, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned bsize = n/ESIZE,i;
  unsigned char *op,*ip,*e; 
  
    #if powof2(ESIZE)
  e = out+(n&~(ESIZE-1));
	#else      
  e = out+bsize*ESIZE;		
    #endif
  for(op = out,ip = in; op < e; ip++) {
	*op++ = ip[0];
	*op++ = ip[i =bsize];
	  #if ESIZE > 2
	*op++ = ip[i+=bsize];
	    #if ESIZE > 3	
	*op++ = ip[i+=bsize];
	      #if ESIZE > 4
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	        #if ESIZE > 8
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	        #endif
	      #endif
	    #endif
	  #endif
  } 	
  ip = in+bsize*ESIZE; 
  while(op < out+n) 
	*op++ = *ip++;
}  
  #endif // STRIDE
#endif

#if ESIZE == 2 || ESIZE == 4 || ESIZE == 8

  #if defined(__SSE2__) && defined(SSE2_ON)
void TEMPLATE2(TPENC128V, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {   
  unsigned       v = n&~(ESIZE*16-1), bsize = v/STRIDE;
  unsigned char *op,*ip;
    #ifdef __SSE3__
	  #if ESIZE == 2
  __m128i sv = _mm_set_epi8(15, 13, 11, 9, 7, 5, 3, 1, 
                            14, 12, 10, 8, 6, 4, 2, 0);
      #elif ESIZE == 4
  __m128i sv = _mm_set_epi8(15, 11, 7,3, 
                            14, 10, 6,2, 
							13,  9, 5,1,
							12,  8, 4,0);
      #else
  __m128i sv = _mm_set_epi8(15,  7,  
                            14,  6,  
							13,  5, 
							12,  4, 
							11,  3, 
							10,  2, 
							 9,  1, 
							 8,  0	);
	  #endif
    #endif

      #if STRIDE > ESIZE
  __m128i cl = _mm_set1_epi8(0x0f), ch=_mm_set1_epi8(0xf0), cb = _mm_set1_epi16(0xff);
      #endif

  for(ip = in, op = out; ip != in+v; op += ESIZE*16/STRIDE) { unsigned char *p = op;
    __m128i iv[ESIZE],ov[ESIZE];
      #ifdef __SSSE3__
        #if   ESIZE == 2
    ov[0] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)ip), sv); ip += 16; 
    ov[1] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)ip), sv); ip += 16; 	__builtin_prefetch(ip+512);
	iv[0] = _mm_unpacklo_epi64(ov[0], ov[1]);
    iv[1] = _mm_unpackhi_epi64(ov[0], ov[1]);
        #elif ESIZE == 4
    iv[0] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *) ip    ), sv);
    iv[1] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+16)), sv); 
    iv[2] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+32)), sv); 
    iv[3] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+48)), sv); ip += 64; __builtin_prefetch(ip+512);

	ov[0] = _mm_unpacklo_epi32(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi32(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi32(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi32(iv[2], iv[3]);

    iv[0] = _mm_unpacklo_epi64(ov[0], ov[2]); 
    iv[1] = _mm_unpackhi_epi64(ov[0], ov[2]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[3]);
    iv[3] = _mm_unpackhi_epi64(ov[1], ov[3]);
        #elif ESIZE == 8
    ov[0] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *) ip    ), sv); 
    ov[1] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+16)), sv); 
    ov[2] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+32)), sv); 
    ov[3] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+48)), sv); ip += 64;	
	
	iv[0] = _mm_unpacklo_epi16(ov[0], ov[1]); 
    iv[1] = _mm_unpackhi_epi16(ov[0], ov[1]); 
    iv[2] = _mm_unpacklo_epi16(ov[2], ov[3]); 
    iv[3] = _mm_unpackhi_epi16(ov[2], ov[3]);  
	
    ov[0] = _mm_unpacklo_epi32(iv[0], iv[2]);
    ov[1] = _mm_unpackhi_epi32(iv[0], iv[2]);
    ov[2] = _mm_unpacklo_epi32(iv[1], iv[3]);
    ov[3] = _mm_unpackhi_epi32(iv[1], iv[3]);

	
    ov[4] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *) ip    ), sv); 
    ov[5] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+16)), sv);  
    ov[6] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+32)), sv); 
    ov[7] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)(ip+48)), sv); ip += 64; __builtin_prefetch(ip+512);

	iv[4] = _mm_unpacklo_epi16(ov[4], ov[5]);
    iv[5] = _mm_unpackhi_epi16(ov[4], ov[5]);
    iv[6] = _mm_unpacklo_epi16(ov[6], ov[7]);
    iv[7] = _mm_unpackhi_epi16(ov[6], ov[7]);

    ov[4] = _mm_unpacklo_epi32(iv[4], iv[6]);
    ov[5] = _mm_unpackhi_epi32(iv[4], iv[6]);
    ov[6] = _mm_unpacklo_epi32(iv[5], iv[7]);
    ov[7] = _mm_unpackhi_epi32(iv[5], iv[7]);

	iv[0] = _mm_unpacklo_epi64(ov[0], ov[4]);
    iv[1] = _mm_unpackhi_epi64(ov[0], ov[4]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[5]);
    iv[3] = _mm_unpackhi_epi64(ov[1], ov[5]);
	
	iv[4] = _mm_unpacklo_epi64(ov[2], ov[6]);
    iv[5] = _mm_unpackhi_epi64(ov[2], ov[6]);
    iv[6] = _mm_unpacklo_epi64(ov[3], ov[7]);
    iv[7] = _mm_unpackhi_epi64(ov[3], ov[7]);
        #endif        
																			
      #elif defined(__SSE2__)
        #if ESIZE == 2
    iv[0] = _mm_loadu_si128((__m128i *)ip); ip += 16;
    iv[1] = _mm_loadu_si128((__m128i *)ip); ip += 16;		__builtin_prefetch(ip+512);
	
    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
	
    iv[0] = _mm_unpacklo_epi8(ov[0], ov[1]);
    iv[1] = _mm_unpackhi_epi8(ov[0], ov[1]);
	
    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
	
    iv[0] = _mm_unpacklo_epi8(ov[0], ov[1]);
    iv[1] = _mm_unpackhi_epi8(ov[0], ov[1]);
        #elif ESIZE == 4
    iv[0] = _mm_loadu_si128((__m128i *) ip   ); 
    iv[1] = _mm_loadu_si128((__m128i *)(ip+16)); 
    iv[2] = _mm_loadu_si128((__m128i *)(ip+32)); 
    iv[3] = _mm_loadu_si128((__m128i *)(ip+48)); ip += 64;	__builtin_prefetch(ip+512);				

    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    iv[0] = _mm_unpacklo_epi8(ov[0], ov[1]);
    iv[1] = _mm_unpackhi_epi8(ov[0], ov[1]);

    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);
    
	iv[2] = _mm_unpacklo_epi8(ov[2], ov[3]);
    iv[3] = _mm_unpackhi_epi8(ov[2], ov[3]);
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);
	
    iv[0] = _mm_unpacklo_epi64(ov[0], ov[2]);
    iv[1] = _mm_unpackhi_epi64(ov[0], ov[2]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[3]);
    iv[3] = _mm_unpackhi_epi64(ov[1], ov[3]);
        #elif ESIZE == 8
    iv[0] = _mm_loadu_si128((__m128i *) ip    );
    iv[1] = _mm_loadu_si128((__m128i *)(ip+ 16));
    iv[2] = _mm_loadu_si128((__m128i *)(ip+ 32));
    iv[3] = _mm_loadu_si128((__m128i *)(ip+ 48)); 					
    iv[4] = _mm_loadu_si128((__m128i *)(ip+ 64)); 
    iv[5] = _mm_loadu_si128((__m128i *)(ip+ 80)); 					
    iv[6] = _mm_loadu_si128((__m128i *)(ip+ 96)); 
    iv[7] = _mm_loadu_si128((__m128i *)(ip+112)); ip += 128; __builtin_prefetch(ip+512);

	ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]); 
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]); 
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]); 
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);  
	ov[4] = _mm_unpacklo_epi8(iv[4], iv[5]);
    ov[5] = _mm_unpackhi_epi8(iv[4], iv[5]);
    ov[6] = _mm_unpacklo_epi8(iv[6], iv[7]);
    ov[7] = _mm_unpackhi_epi8(iv[6], iv[7]);

	iv[0] = _mm_unpacklo_epi8(ov[0], ov[1]); 
    iv[1] = _mm_unpackhi_epi8(ov[0], ov[1]); 
    iv[2] = _mm_unpacklo_epi8(ov[2], ov[3]); 
    iv[3] = _mm_unpackhi_epi8(ov[2], ov[3]);  
	iv[4] = _mm_unpacklo_epi8(ov[4], ov[5]);
    iv[5] = _mm_unpackhi_epi8(ov[4], ov[5]);
    iv[6] = _mm_unpacklo_epi8(ov[6], ov[7]);
    iv[7] = _mm_unpackhi_epi8(ov[6], ov[7]);
	
    ov[0] = _mm_unpacklo_epi32(iv[0], iv[2]);
    ov[1] = _mm_unpackhi_epi32(iv[0], iv[2]);
    ov[2] = _mm_unpacklo_epi32(iv[1], iv[3]);
    ov[3] = _mm_unpackhi_epi32(iv[1], iv[3]);
    ov[4] = _mm_unpacklo_epi32(iv[4], iv[6]);
    ov[5] = _mm_unpackhi_epi32(iv[4], iv[6]);
    ov[6] = _mm_unpacklo_epi32(iv[5], iv[7]);
    ov[7] = _mm_unpackhi_epi32(iv[5], iv[7]);

	iv[0] = _mm_unpacklo_epi64(ov[0], ov[4]);
    iv[1] = _mm_unpackhi_epi64(ov[0], ov[4]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[5]);
    iv[3] = _mm_unpackhi_epi64(ov[1], ov[5]);
	iv[4] = _mm_unpacklo_epi64(ov[2], ov[6]);
    iv[5] = _mm_unpackhi_epi64(ov[2], ov[6]);
    iv[6] = _mm_unpacklo_epi64(ov[3], ov[7]);
    iv[7] = _mm_unpackhi_epi64(ov[3], ov[7]);
	    #endif
      #endif

      #if STRIDE <= ESIZE
    _mm_storeu_si128((__m128i *) p,         iv[0]);
    _mm_storeu_si128((__m128i *)(p+=bsize), iv[1]);
        #if ESIZE > 2
    _mm_storeu_si128((__m128i *)(p+=bsize), iv[2]);
    _mm_storeu_si128((__m128i *)(p+=bsize), iv[3]); 
          #if ESIZE > 4
    _mm_storeu_si128((__m128i *)(p+=bsize), iv[4]);
    _mm_storeu_si128((__m128i *)(p+=bsize), iv[5]); 
    _mm_storeu_si128((__m128i *)(p+=bsize), iv[6]);
    _mm_storeu_si128((__m128i *)(p+=bsize), iv[7]); 
          #endif															
        #endif

      #else // Nibble
    ov[0] = _mm_and_si128(iv[0], cl);                             					
    ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(  ov[0],2));    				      					

    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[0], ch), 4);            					
    ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128( ov[1],2));    				      					

    ov[2] = _mm_and_si128(iv[1], cl);                             					
    ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(  ov[2],2));    				      					

    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[1], ch),4);            					
    ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(  ov[3],2));    				      					

    _mm_storel_epi64((__m128i *) p,         ov[0]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[1]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[2]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[3]); 
        #if ESIZE > 2
    ov[0] = _mm_and_si128(iv[2], cl);                             					
    ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(  ov[0],2));    				      					

    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[2], ch),4);            					
    ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128(  ov[1],2));    				      					

    ov[2] = _mm_and_si128(iv[3], cl);                             					
    ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(  ov[2],2));    				      					

    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[3], ch),4);            					
    ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(  ov[3],2));    				      					

    _mm_storel_epi64((__m128i *)(p+=bsize), ov[0]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[1]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[2]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[3]);
          #if ESIZE > 4
    ov[0] = _mm_and_si128(iv[4], cl);                             					
    ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(  ov[0],2));    				      					

    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[4], ch), 4);            					
    ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128( ov[1],2));    				      					

    ov[2] = _mm_and_si128(iv[5], cl);                             					
    ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(  ov[2],2));    				      					

    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[5], ch),4);            					
    ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(  ov[3],2));    				      					

    _mm_storel_epi64((__m128i *)(p+=bsize), ov[0]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[1]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[2]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[3]); 
	
    ov[0] = _mm_and_si128(iv[6], cl);                             					
    ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(  ov[0],2));    				      					

    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[6], ch),4);            					
    ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128(  ov[1],2));    				      					

    ov[2] = _mm_and_si128(iv[7], cl);                             					
    ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(  ov[2],2));    				      					

    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[7], ch),4);            					
    ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(  ov[3],2));    				      					

    _mm_storel_epi64((__m128i *)(p+=bsize), ov[0]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[1]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[2]);
    _mm_storel_epi64((__m128i *)(p+=bsize), ov[3]);		
	      #endif
        #endif															
      #endif															
  }
  TEMPLATE2(tpenc,ESIZE)(in+v, n-v, out+v);
}

void TEMPLATE2(TPDEC128V, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned       v = n&~(ESIZE*16-1), bsize = v/STRIDE,i;
  unsigned char *op,*ip; 

    #if STRIDE > ESIZE
  __m128i cl = _mm_set1_epi8(0x0f), ch=_mm_set1_epi8(0xf0), cb = _mm_set1_epi16(0xff);
    #endif

  for(op = out,ip = in; op != out+v; ip += ESIZE*16/STRIDE) { unsigned char *p=ip;
    __m128i iv[ESIZE], ov[ESIZE];

      #if STRIDE > ESIZE
    ov[0] = _mm_loadl_epi64((__m128i *) p        );   
    ov[1] = _mm_loadl_epi64((__m128i *)(p+=bsize));   

    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); 						
    ov[0] = _mm_and_si128(ov[0], cl);												
    
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); 						
    ov[1] = _mm_and_si128(ov[1], cl);												
    iv[0] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);					

 
    ov[2] = _mm_loadl_epi64((__m128i *)(p+=bsize));   
    ov[3] = _mm_loadl_epi64((__m128i *)(p+=bsize));  
 
    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); 						
    ov[2] = _mm_and_si128(ov[2], cl);												
    
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); 						
    ov[3] = _mm_and_si128(ov[3], cl);												
    iv[1] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);	

	    #if ESIZE > 2
    ov[0] = _mm_loadl_epi64((__m128i *)(p+=bsize));   
    ov[1] = _mm_loadl_epi64((__m128i *)(p+=bsize));  
 
    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); 						
    ov[0] = _mm_and_si128(ov[0], cl);												
    
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); 						
    ov[1] = _mm_and_si128(ov[1], cl);												
    iv[2] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);					


    ov[2] = _mm_loadl_epi64((__m128i *)(p+=bsize));   
    ov[3] = _mm_loadl_epi64((__m128i *)(p+=bsize));  
 
    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); 						
    ov[2] = _mm_and_si128(ov[2], cl);												
    
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); 						
    ov[3] = _mm_and_si128(ov[3], cl);												
    iv[3] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);	
        #endif
        #if ESIZE > 4
    ov[0] = _mm_loadl_epi64((__m128i *)(p+=bsize));   
    ov[1] = _mm_loadl_epi64((__m128i *)(p+=bsize));   

    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); 						
    ov[0] = _mm_and_si128(ov[0], cl);												
    
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); 						
    ov[1] = _mm_and_si128(ov[1], cl);												
    iv[4] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);					

 
    ov[2] = _mm_loadl_epi64((__m128i *)(p+=bsize));   
    ov[3] = _mm_loadl_epi64((__m128i *)(p+=bsize));  
 
    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); 						
    ov[2] = _mm_and_si128(ov[2], cl);												
    
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); 						
    ov[3] = _mm_and_si128(ov[3], cl);												
    iv[5] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);	


    ov[0] = _mm_loadl_epi64((__m128i *)(p+=bsize));   
    ov[1] = _mm_loadl_epi64((__m128i *)(p+=bsize));  
 
    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); 						
    ov[0] = _mm_and_si128(ov[0], cl);												
    
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); 						
    ov[1] = _mm_and_si128(ov[1], cl);												
    iv[6] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);					


    ov[2] = _mm_loadl_epi64((__m128i *)(p+=bsize));   
    ov[3] = _mm_loadl_epi64((__m128i *)(p+=bsize));  
 
    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); 						
    ov[2] = _mm_and_si128(ov[2], cl);												
    
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); 						
    ov[3] = _mm_and_si128(ov[3], cl);												
    iv[7] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);	
        #endif	
      #else
    iv[0] = _mm_loadu_si128((__m128i *) p        );   
    iv[1] = _mm_loadu_si128((__m128i *)(p+=bsize));   
        #if ESIZE > 2
    iv[2] = _mm_loadu_si128((__m128i *)(p+=bsize));   
    iv[3] = _mm_loadu_si128((__m128i *)(p+=bsize));	
          #if ESIZE > 4
    iv[4] = _mm_loadu_si128((__m128i *)(p+=bsize));   
    iv[5] = _mm_loadu_si128((__m128i *)(p+=bsize));	
    iv[6] = _mm_loadu_si128((__m128i *)(p+=bsize));   
    iv[7] = _mm_loadu_si128((__m128i *)(p+=bsize));	
          #endif
        #endif
      #endif
																	__builtin_prefetch(ip+(ESIZE*16/STRIDE)+512);
        #if ESIZE == 2
    _mm_storeu_si128((__m128i *)op, ov[0] = _mm_unpacklo_epi8(iv[0], iv[1])); op += 16;
    _mm_storeu_si128((__m128i *)op, ov[1] = _mm_unpackhi_epi8(iv[0], iv[1])); op += 16;
        #elif ESIZE == 4
    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);
    _mm_storeu_si128((__m128i *) op,     _mm_unpacklo_epi16(ov[0], ov[2])); 
    _mm_storeu_si128((__m128i *)(op+16), _mm_unpackhi_epi16(ov[0], ov[2])); 
    _mm_storeu_si128((__m128i *)(op+32), _mm_unpacklo_epi16(ov[1], ov[3])); 
    _mm_storeu_si128((__m128i *)(op+48), _mm_unpackhi_epi16(ov[1], ov[3])); op += 64;
        #else
    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);
    iv[0] = _mm_unpacklo_epi16(ov[0], ov[2]);
    iv[1] = _mm_unpackhi_epi16(ov[0], ov[2]);
    iv[2] = _mm_unpacklo_epi16(ov[1], ov[3]);
    iv[3] = _mm_unpackhi_epi16(ov[1], ov[3]);

    ov[4] = _mm_unpacklo_epi8(iv[4], iv[5]);
    ov[5] = _mm_unpackhi_epi8(iv[4], iv[5]);
    ov[6] = _mm_unpacklo_epi8(iv[6], iv[7]);
    ov[7] = _mm_unpackhi_epi8(iv[6], iv[7]);	
    iv[4] = _mm_unpacklo_epi16(ov[4], ov[6]);
    iv[5] = _mm_unpackhi_epi16(ov[4], ov[6]);
    iv[6] = _mm_unpacklo_epi16(ov[5], ov[7]);
    iv[7] = _mm_unpackhi_epi16(ov[5], ov[7]);	
 			
    _mm_storeu_si128((__m128i *) op,      _mm_unpacklo_epi32(iv[0], iv[4])); 
    _mm_storeu_si128((__m128i *)(op+ 16), _mm_unpackhi_epi32(iv[0], iv[4])); 
    _mm_storeu_si128((__m128i *)(op+ 32), _mm_unpacklo_epi32(iv[1], iv[5])); 
    _mm_storeu_si128((__m128i *)(op+ 48), _mm_unpackhi_epi32(iv[1], iv[5]));   
    _mm_storeu_si128((__m128i *)(op+ 64), _mm_unpacklo_epi32(iv[2], iv[6])); 
    _mm_storeu_si128((__m128i *)(op+ 80), _mm_unpackhi_epi32(iv[2], iv[6])); 
    _mm_storeu_si128((__m128i *)(op+ 96), _mm_unpacklo_epi32(iv[3], iv[7])); 
    _mm_storeu_si128((__m128i *)(op+112), _mm_unpackhi_epi32(iv[3], iv[7])); op += 128;  
        #endif
  }
  TEMPLATE2(tpdec,ESIZE)(in+v, n-v, out+v);
}
  #endif

  #if defined(__AVX2__) && defined(AVX2_ON)
void TEMPLATE2(TPENC256V, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned v = n&~(ESIZE*32-1), bsize = v/STRIDE;
  unsigned char *op,*ip;

    #if ESIZE == 2
  __m256i sv = _mm256_set_epi8( 15, 13, 11, 9, 7, 5, 3, 1, 
                            	14, 12, 10, 8, 6, 4, 2, 0,
								15, 13, 11, 9, 7, 5, 3, 1, 
                            	14, 12, 10, 8, 6, 4, 2, 0);
    #else
  __m256i pv = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0),
      #if ESIZE == 4
          sv = _mm256_set_epi8(15, 11, 7, 3, 
                               14, 10, 6, 2, 
							   13,  9, 5, 1,
							   12,  8, 4, 0,
							   15, 11, 7, 3, 
                               14, 10, 6, 2, 
							   13,  9, 5, 1,
							   12,  8, 4, 0);
      #else
          sv = _mm256_set_epi8(15,  7,  
                               14,  6,  
							   13,  5, 
							   12,  4, 
							   11,  3, 
							   10,  2, 
							    9,  1, 
							    8,  0,
                               15,  7,  
                               14,  6,  
							   13,  5, 
							   12,  4, 
							   11,  3, 
							   10,  2, 
							    9,  1, 
							    8,  0 ),
          tv = _mm256_set_epi8(15, 14, 11, 10, 13, 12,  9,  8, 
						        7,  6,  3,  2,  5,  4,  1,  0,
							   15, 14, 11, 10, 13, 12,  9,  8, 
						        7,  6,  3,  2,  5,  4,  1,  0);
	  #endif
	#endif
  
    #if STRIDE > ESIZE
  __m256i cl = _mm256_set1_epi8(0x0f), ch=_mm256_set1_epi8(0xf0), cb = _mm256_set1_epi16(0xff);
    #endif

  for(ip = in,op = out; ip != in+v; op += ESIZE*32/STRIDE) { unsigned char *p=op;
    __m256i iv[ESIZE],ov[ESIZE];														

	  #if   ESIZE == 2
    ov[0] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)ip), sv); ip += 32; 
    ov[1] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)ip), sv); ip += 32; 		__builtin_prefetch(ip+512);	
	iv[0] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi64(ov[0], ov[1]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[1] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi64(ov[0], ov[1]), _MM_SHUFFLE(3, 1, 2, 0));
	  #elif ESIZE == 4
    iv[0] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *) ip),     sv);
    iv[1] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+32)), sv); 
    iv[2] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+64)), sv); 
    iv[3] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+96)), sv); ip += 128; __builtin_prefetch(ip+512);	

	ov[0] = _mm256_unpacklo_epi32(iv[0], iv[1]);
    ov[1] = _mm256_unpackhi_epi32(iv[0], iv[1]);
    ov[2] = _mm256_unpacklo_epi32(iv[2], iv[3]);
    ov[3] = _mm256_unpackhi_epi32(iv[2], iv[3]);

    iv[0] = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(ov[0], ov[2]), pv); 
    iv[1] = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(ov[0], ov[2]), pv);
    iv[2] = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(ov[1], ov[3]), pv);
    iv[3] = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(ov[1], ov[3]), pv);
	  #else
    ov[0] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *) ip    ), sv); 
    ov[1] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+32)), sv); 
    ov[2] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+64)), sv); 
    ov[3] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+96)), sv); ip += 128; 
	
	iv[0] = _mm256_unpacklo_epi16(ov[0], ov[1]); 
    iv[1] = _mm256_unpackhi_epi16(ov[0], ov[1]); 
    iv[2] = _mm256_unpacklo_epi16(ov[2], ov[3]); 
    iv[3] = _mm256_unpackhi_epi16(ov[2], ov[3]);  
	
    ov[0] = _mm256_unpacklo_epi32(iv[0], iv[2]);
    ov[1] = _mm256_unpackhi_epi32(iv[0], iv[2]);
    ov[2] = _mm256_unpacklo_epi32(iv[1], iv[3]);
    ov[3] = _mm256_unpackhi_epi32(iv[1], iv[3]);
	

    ov[4] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *) ip    ), sv); 
    ov[5] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+32)), sv);  
    ov[6] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+64)), sv); 
    ov[7] = _mm256_shuffle_epi8(_mm256_loadu_si256((__m256i *)(ip+96)), sv); ip += 128; __builtin_prefetch(ip+512);	

	iv[4] = _mm256_unpacklo_epi16(ov[4], ov[5]);
    iv[5] = _mm256_unpackhi_epi16(ov[4], ov[5]);
    iv[6] = _mm256_unpacklo_epi16(ov[6], ov[7]);
    iv[7] = _mm256_unpackhi_epi16(ov[6], ov[7]);

    ov[4] = _mm256_unpacklo_epi32(iv[4], iv[6]);
    ov[5] = _mm256_unpackhi_epi32(iv[4], iv[6]);
    ov[6] = _mm256_unpacklo_epi32(iv[5], iv[7]);
    ov[7] = _mm256_unpackhi_epi32(iv[5], iv[7]);
    
	iv[0] = _mm256_shuffle_epi8(_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(ov[0], ov[4]), pv), tv);
    iv[1] = _mm256_shuffle_epi8(_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(ov[0], ov[4]), pv), tv);
    iv[2] = _mm256_shuffle_epi8(_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(ov[1], ov[5]), pv), tv);
    iv[3] = _mm256_shuffle_epi8(_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(ov[1], ov[5]), pv), tv);
	
	iv[4] = _mm256_shuffle_epi8(_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(ov[2], ov[6]), pv), tv);
    iv[5] = _mm256_shuffle_epi8(_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(ov[2], ov[6]), pv), tv);
    iv[6] = _mm256_shuffle_epi8(_mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(ov[3], ov[7]), pv), tv);
    iv[7] = _mm256_shuffle_epi8(_mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(ov[3], ov[7]), pv), tv);
	  #endif
	  																											
      #if STRIDE <= ESIZE 
    _mm256_storeu_si256((__m256i *) p,         iv[0]);  
    _mm256_storeu_si256((__m256i *)(p+=bsize), iv[1]);
	    #if ESIZE > 2
    _mm256_storeu_si256((__m256i *)(p+=bsize), iv[2]);
    _mm256_storeu_si256((__m256i *)(p+=bsize), iv[3]); 
	      #if ESIZE > 4
    _mm256_storeu_si256((__m256i *)(p+=bsize), iv[4]);
    _mm256_storeu_si256((__m256i *)(p+=bsize), iv[5]);
    _mm256_storeu_si256((__m256i *)(p+=bsize), iv[6]);
    _mm256_storeu_si256((__m256i *)(p+=bsize), iv[7]);
          #endif
        #endif

      #else // Nibble Transpose
    #define mm256_packus_epi16(a, b) _mm256_permute4x64_epi64(_mm256_packus_epi16(a, b), _MM_SHUFFLE(3, 1, 2, 0))

    ov[0] = _mm256_and_si256(iv[0], cl);                             					
    ov[0] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] =  mm256_packus_epi16(ov[0], _mm256_srli_si256(  ov[0],2));    				      					

    ov[1] = _mm256_srli_epi16(_mm256_and_si256(iv[0], ch),4);            					
    ov[1] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] =  mm256_packus_epi16(ov[1], _mm256_srli_si256(  ov[1],2));    				      					

    ov[2] = _mm256_and_si256(iv[1], cl);                             					
    ov[2] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] =  mm256_packus_epi16(ov[2], _mm256_srli_si256(  ov[2],2));    				      					

    ov[3] = _mm256_srli_epi16(_mm256_and_si256(iv[1], ch),4);            					
    ov[3] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] =  mm256_packus_epi16(ov[3], _mm256_srli_si256(  ov[3],2));    				      					

    _mm_storeu_si128((__m256i *) p        , _mm256_castsi256_si128(ov[0]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[1]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[2]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[3])); 

        #if ESIZE > 2
    ov[0] = _mm256_and_si256(iv[2], cl);                             					
    ov[0] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] =  mm256_packus_epi16(ov[0], _mm256_srli_si256(  ov[0],2));    				      					

    ov[1] = _mm256_srli_epi16(_mm256_and_si256(iv[2], ch),4);            					
    ov[1] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] =  mm256_packus_epi16(ov[1], _mm256_srli_si256(  ov[1],2));    				      					

    ov[2] = _mm256_and_si256(iv[3], cl);                             					
    ov[2] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] =  mm256_packus_epi16(ov[2], _mm256_srli_si256(  ov[2],2));    				      					

    ov[3] = _mm256_srli_epi16(_mm256_and_si256(iv[3], ch),4);            					
    ov[3] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] =  mm256_packus_epi16(ov[3], _mm256_srli_si256(  ov[3],2));    				      					

    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[0]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[1]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[2]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[3]));
	
          #if ESIZE > 4
    ov[0] = _mm256_and_si256(iv[4], cl);                             					
    ov[0] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] =  mm256_packus_epi16(ov[0], _mm256_srli_si256(  ov[0],2));    				      					

    ov[1] = _mm256_srli_epi16(_mm256_and_si256(iv[4], ch),4);            					
    ov[1] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] =  mm256_packus_epi16(ov[1], _mm256_srli_si256(  ov[1],2));    				      					

    ov[2] = _mm256_and_si256(iv[5], cl);                             					
    ov[2] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] =  mm256_packus_epi16(ov[2], _mm256_srli_si256(  ov[2],2));    				      					

    ov[3] = _mm256_srli_epi16(_mm256_and_si256(iv[5], ch),4);            					
    ov[3] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] =  mm256_packus_epi16(ov[3], _mm256_srli_si256(  ov[3],2));    				      					

    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[0]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[1]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[2]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[3])); 


    ov[0] = _mm256_and_si256(iv[6], cl);                             					
    ov[0] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] =  mm256_packus_epi16(ov[0], _mm256_srli_si256(  ov[0],2));    				      					

    ov[1] = _mm256_srli_epi16(_mm256_and_si256(iv[6], ch),4);            					
    ov[1] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] =  mm256_packus_epi16(ov[1], _mm256_srli_si256(  ov[1],2));    				      					

    ov[2] = _mm256_and_si256(iv[7], cl);                             					
    ov[2] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] =  mm256_packus_epi16(ov[2], _mm256_srli_si256(  ov[2],2));    				      					

    ov[3] = _mm256_srli_epi16(_mm256_and_si256(iv[7], ch),4);            					
    ov[3] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] =  mm256_packus_epi16(ov[3], _mm256_srli_si256(  ov[3],2));    				      					

    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[0]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[1]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[2]));
    _mm_storeu_si128((__m256i *)(p+=bsize), _mm256_castsi256_si128(ov[3]));
          #endif
        #endif		
      #endif
  } 														  
  TEMPLATE2(tpenc128v,ESIZE)(in+v, n-v, out+v);
}

#define NBL0(x,y) ov[x] = _mm256_permute4x64_epi64(_mm256_castsi128_si256(_mm_loadu_si128((__m128i *)(p       ))),_MM_SHUFFLE(3, 1, 2, 0));\
                  ov[y] = _mm256_permute4x64_epi64(_mm256_castsi128_si256(_mm_loadu_si128((__m128i *)(p+=bsize))),_MM_SHUFFLE(3, 1, 2, 0));

#define NBL(x,y)  ov[x] = _mm256_permute4x64_epi64(_mm256_castsi128_si256(_mm_loadu_si128((__m128i *)(p+=bsize))),_MM_SHUFFLE(3, 1, 2, 0));\
                  ov[y] = _mm256_permute4x64_epi64(_mm256_castsi128_si256(_mm_loadu_si128((__m128i *)(p+=bsize))),_MM_SHUFFLE(3, 1, 2, 0));

#define NB(x,y,_iv_) {\
  ov[x] = _mm256_and_si256(_mm256_unpacklo_epi8(ov[x], _mm256_srli_epi16(ov[x],4)), cl);\
  ov[y] = _mm256_and_si256(_mm256_unpacklo_epi8(ov[y], _mm256_srli_epi16(ov[y],4)), cl);\
  _iv_  = _mm256_or_si256(_mm256_slli_epi16(ov[y],4), ov[x]); \
} 

void TEMPLATE2(TPDEC256V, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned char *op,*ip; 
  unsigned v = n&~(ESIZE*32-1), bsize = v/STRIDE;

    #if STRIDE > ESIZE
  __m256i cl = _mm256_set1_epi8(0x0f), ch=_mm256_set1_epi8(0xf0), cb = _mm256_set1_epi16(0xff);
    #endif

  for(op = out,ip = in; op != out+v; ip += ESIZE*32/STRIDE) { unsigned char *p = ip;
    __m256i iv[ESIZE], ov[ESIZE];
  
      #if STRIDE > ESIZE
	NBL0(0,1); NBL( 2,3); NB(0,1,iv[0]); NB(2,3,iv[1]);
        #if ESIZE > 2
	NBL( 0,1); NBL( 2,3); NB(0,1,iv[2]); NB(2,3,iv[3]);
          #if ESIZE > 4
	NBL(4,5); NBL( 6,7); NB(4,5,iv[4]); NB(6,7,iv[5]);
	NBL(4,5); NBL( 6,7); NB(4,5,iv[6]); NB(6,7,iv[7]);
	      #endif
	    #endif
      #else
    iv[0] = _mm256_loadu_si256((__m256i *) p)        ;    
    iv[1] = _mm256_loadu_si256((__m256i *)(p+=bsize));   
        #if ESIZE > 2
    iv[2] = _mm256_loadu_si256((__m256i *)(p+=bsize));  
    iv[3] = _mm256_loadu_si256((__m256i *)(p+=bsize));
          #if ESIZE > 4
    iv[4] = _mm256_loadu_si256((__m256i *)(p+=bsize));  
    iv[5] = _mm256_loadu_si256((__m256i *)(p+=bsize));          	
    iv[6] = _mm256_loadu_si256((__m256i *)(p+=bsize));  
    iv[7] = _mm256_loadu_si256((__m256i *)(p+=bsize));          	
          #endif
        #endif
      #endif
														__builtin_prefetch(ip+ESIZE*32/STRIDE+512);
      #if ESIZE == 2
	ov[0] = _mm256_permute4x64_epi64(iv[0], _MM_SHUFFLE(3, 1, 2, 0));
	ov[1] = _mm256_permute4x64_epi64(iv[1], _MM_SHUFFLE(3, 1, 2, 0));
	_mm256_storeu_si256((__m256i *)op, _mm256_unpacklo_epi8(ov[0], ov[1])); op += 32;
	_mm256_storeu_si256((__m256i *)op, _mm256_unpackhi_epi8(ov[0], ov[1])); op += 32;
      #elif ESIZE == 4	  
    ov[0] = _mm256_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm256_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm256_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm256_unpackhi_epi8(iv[2], iv[3]);

    iv[0] = _mm256_unpacklo_epi16(ov[0], ov[2]);
    iv[1] = _mm256_unpackhi_epi16(ov[0], ov[2]);
    iv[2] = _mm256_unpacklo_epi16(ov[1], ov[3]);
    iv[3] = _mm256_unpackhi_epi16(ov[1], ov[3]);

	_mm256_storeu_si256((__m256i *) op,     _mm256_permute2x128_si256(iv[0], iv[1], (2 << 4) | 0)); 
    _mm256_storeu_si256((__m256i *)(op+32), _mm256_permute2x128_si256(iv[2], iv[3], (2 << 4) | 0)); 
    _mm256_storeu_si256((__m256i *)(op+64), _mm256_permute2x128_si256(iv[0], iv[1], (3 << 4) | 1)); 
    _mm256_storeu_si256((__m256i *)(op+96), _mm256_permute2x128_si256(iv[2], iv[3], (3 << 4) | 1)); op += 128;
      #else
    ov[0] = _mm256_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm256_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm256_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm256_unpackhi_epi8(iv[2], iv[3]);
    iv[0] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi16(ov[0], ov[2]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[1] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi16(ov[0], ov[2]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[2] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi16(ov[1], ov[3]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[3] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi16(ov[1], ov[3]), _MM_SHUFFLE(3, 1, 2, 0));

    ov[4] = _mm256_unpacklo_epi8(iv[4], iv[5]);
    ov[5] = _mm256_unpackhi_epi8(iv[4], iv[5]);
    ov[6] = _mm256_unpacklo_epi8(iv[6], iv[7]);
    ov[7] = _mm256_unpackhi_epi8(iv[6], iv[7]);
    iv[4] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi16(ov[4], ov[6]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[5] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi16(ov[4], ov[6]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[6] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi16(ov[5], ov[7]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[7] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi16(ov[5], ov[7]), _MM_SHUFFLE(3, 1, 2, 0));

    _mm256_storeu_si256((__m256i *) op,      _mm256_unpacklo_epi32(iv[0], iv[4])); 
    _mm256_storeu_si256((__m256i *)(op+ 32), _mm256_unpacklo_epi32(iv[1], iv[5])); 
    _mm256_storeu_si256((__m256i *)(op+ 64), _mm256_unpacklo_epi32(iv[2], iv[6])); 
    _mm256_storeu_si256((__m256i *)(op+ 96), _mm256_unpacklo_epi32(iv[3], iv[7])); 
    _mm256_storeu_si256((__m256i *)(op+128), _mm256_unpackhi_epi32(iv[0], iv[4])); 
    _mm256_storeu_si256((__m256i *)(op+160), _mm256_unpackhi_epi32(iv[1], iv[5]));   
    _mm256_storeu_si256((__m256i *)(op+192), _mm256_unpackhi_epi32(iv[2], iv[6])); 
    _mm256_storeu_si256((__m256i *)(op+224), _mm256_unpackhi_epi32(iv[3], iv[7])); op += 256;  
      #endif
  }
  TEMPLATE2(tpdec128v,ESIZE)(in+v, n-v, out+v);
}
  #endif
  
#endif
#endif

