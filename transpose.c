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
//   Nibble/Byte transpose
#ifndef ESIZE
#include <string.h>
  #ifdef __AVX2__
#include <immintrin.h>
  #elif defined(__SSE4_1__)
#include <smmintrin.h>
  #elif defined(__SSSE3__)
#include <tmmintrin.h>
  #elif defined(__SSE3__)
#include <pmmintrin.h>
  #elif defined(__SSE2__)
#include <emmintrin.h>
  #elif defined(__ARM_NEON)
#include <arm_neon.h>
#include "sse_neon.h"
  #endif
#pragma warning( disable : 4005) 
  
#include "conf.h"
#include "transpose.h"

  #ifdef __ARM_NEON
#define PREFETCH(_ip_,_rw_)
  #else
#define PREFETCH(_ip_,_rw_) __builtin_prefetch(_ip_,_rw_)
  #endif

#define powof2(n) !((n)&((n)-1))
 
#define TPENC tpenc
#define TPDEC tpdec

#define ESIZE 3
#define STRIDE ESIZE
#include "transpose.c"
#undef ESIZE

#define ESIZE 16
#define STRIDE ESIZE
#include "transpose.c"
#undef ESIZE

#define ESIZE 2

#define STRIDE ESIZE
#define LD128(_ip_)   _mm_loadu_si128((__m128i *)(_ip_))
#define ST128(_op_,_v_) _mm_storeu_si128((__m128i *)(_op_),_v_)
#define TPENC128V   tpenc128v
#define TPDEC128V   tpdec128v

#define LD256(ip)   _mm256_loadu_si256(ip)
#define ST256(op,v) _mm256_storeu_si256(op,v)
#define TPENC256V   tpenc256v
#define TPDEC256V   tpdec256v
#include "transpose.c"
#undef STRIDE

#define STRIDE 4
#define TPENC128V   tp4enc128v
#define TPDEC128V   tp4dec128v
#define TPENC256V   tp4enc256v
#define TPDEC256V   tp4dec256v
#include "transpose.c"
#undef ESIZE

#define ESIZE 4

#define STRIDE ESIZE
#define TPENC128V   tpenc128v
#define TPDEC128V   tpdec128v
#define TPENC256V   tpenc256v
#define TPDEC256V   tpdec256v
#include "transpose.c"
#undef STRIDE

#define STRIDE 8
#define TPENC128V   tp4enc128v
#define TPDEC128V   tp4dec128v
#define TPENC256V   tp4enc256v
#define TPDEC256V   tp4dec256v
#include "transpose.c"
#undef ESIZE
#undef STRIDE

#define ESIZE 8

#define STRIDE ESIZE
#define TPENC128V   tpenc128v
#define TPDEC128V   tpdec128v
#define TPENC256V   tpenc256v
#define TPDEC256V   tpdec256v
#include "transpose.c"
#undef STRIDE

#define STRIDE 16
#define TPENC128V   tp4enc128v
#define TPDEC128V   tp4dec128v
#define TPENC256V   tp4enc256v
#define TPDEC256V   tp4dec256v
#include "transpose.c"

//--------------------- CPU detection -------------------------------------------
  #if _MSC_VER >=1300
#include <intrin.h>
  #elif defined (__INTEL_COMPILER)
#include <x86intrin.h>
  #endif

  #if !defined(SSE2_ON) && !defined(AVX2_ON)
    #if defined(__i386__) || defined(__x86_64__)
static inline void cpuid(int reg[4], int id) {  
    #if defined (_MSC_VER) //|| defined (__INTEL_COMPILER)       
  __cpuidex(reg, id, 0);                   
    #elif defined(__i386__) || defined(__x86_64__) 
  __asm("cpuid" : "=a"(reg[0]),"=b"(reg[1]),"=c"(reg[2]),"=d"(reg[3]) : "a"(id),"c"(0) : );
    #endif
}

static inline uint64_t xgetbv (int ctr) {   
    #if(defined _MSC_VER && (_MSC_FULL_VER >= 160040219) || defined __INTEL_COMPILER)
  return _xgetbv(ctr);                                  
    #elif defined(__i386__) || defined(__x86_64__)
  unsigned a, d;
  __asm("xgetbv" : "=a"(a),"=d"(d) : "c"(ctr) : );
  return (uint64_t)d << 32 | a;
    #else  
  unsigned a=0, d=0;
  return (uint64_t)d << 32 | a;
    #endif
}
  #endif

static int _cpuiset;                                  
int cpuini(int cpuiset) { if(cpuiset) _cpuiset = cpuiset; return _cpuiset; }

char *cpustr(int cpuiset) {
       if(cpuiset >= 52) return "avx2";
  else if(cpuiset >= 50) return "avx";
  else if(cpuiset >= 41) return "sse4.1";
  else if(cpuiset >= 35) return "arm_neon";
  else if(cpuiset >= 31) return "sse3";
  else if(cpuiset >= 20) return "sse2";
  else return "none";
}

int cpuiset(void) {
  int c[4] = {0};                               
  if(_cpuiset) return _cpuiset;                                   
  _cpuiset++;      
    #ifdef __ARM_NEON
  _cpuiset = 35; // ARM_NEON
    #elif defined(__i386__) || defined(__x86_64__)                                 
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
    #endif 
  return _cpuiset;
}

//---------------------------------------------------------------------------------
typedef void (*TPFUNC)( unsigned char *in, unsigned n, unsigned char *out);

                       // 0  1       2       3       4      5  6  7       8  9                    16 
static TPFUNC _tpe[]  = { 0, 0, tpenc2, tpenc3, tpenc4,     0, 0, 0, tpenc8, 0, 0, 0, 0, 0, 0, 0, tpenc16 };
static TPFUNC _tpd[]  = { 0, 0, tpdec2, tpdec3, tpdec4,     0, 0, 0, tpdec8, 0, 0, 0, 0, 0, 0, 0, tpdec16 };
 
  #ifdef USE_SSE
static TPFUNC _tp4e[] = { 0, 0, tpenc2, tpenc3, tpenc4,     0, 0, 0, tpenc8, 0, 0, 0, 0, 0, 0, 0, tpenc16 }; // Nibble
static TPFUNC _tp4d[] = { 0, 0, tpdec2, tpdec3, tpdec4,     0, 0, 0, tpdec8, 0, 0, 0, 0, 0, 0, 0, tpdec16 };
  #endif

static int tpset;
 
void tpini(int id) { 
  int i; 
  if(tpset) return; 
  tpset++;   
  i = id?id:cpuiset();
    #ifdef USE_AVX2
  if(i >= 52) {  
    _tpe[2] = tpenc128v2; _tpd[2] = tpdec256v2; _tp4e[2] = tp4enc256v2; _tp4d[2] = tp4dec256v2; //SSE encoding _tpe[2] is faster (skylake)
    _tpe[4] = tpenc128v4; _tpd[4] = tpdec256v4; _tp4e[4] = tp4enc256v4; _tp4d[4] = tp4dec256v4; //SSE encoding _tpe[4] is faster (skylake)
    _tpe[8] = tpenc256v8; _tpd[8] = tpdec256v8; _tp4e[8] = tp4enc256v8; _tp4d[8] = tp4dec256v8; 
  } else 
    #endif
    #ifdef USE_SSE
  if(i >= 20) {  
    _tpe[2] = tpenc128v2; _tpd[2] = tpdec128v2; _tp4e[2] = tp4enc128v2; _tp4d[2] = tp4dec128v2;
    _tpe[4] = tpenc128v4; _tpd[4] = tpdec128v4; _tp4e[4] = tp4enc128v4; _tp4d[4] = tp4dec128v4;
    _tpe[8] = tpenc128v8; _tpd[8] = tpdec128v8; _tp4e[8] = tp4enc128v8; _tp4d[8] = tp4dec128v8;
    if(i == 35) _tpd[8] = tpdec8; // ARM NEON scalar is faster 
  }
    #endif
}

void tpenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) { 
  TPFUNC f;  
  if(!tpset) tpini(0);                                   
  if(esize <= 16 && (f = _tpe[esize])) f(in,n,out);
  else {
    unsigned i, stride=n/esize; 
    unsigned char *op,*ip;
    for(ip = in,op = out; ip < in+stride*esize; op++)
      for(i = 0; i < esize; i++)
        op[i*stride] = *ip++; 
    for(op = out + esize*stride; ip < in+n;) 
      *op++ = *ip++;
  }
}  

void tpdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) { 
  TPFUNC f; 
  if(!tpset) tpini(0); 
  if(esize <= 16 && (f = _tpd[esize])) f(in,n,out); 
  else {
    unsigned i, stride = n/esize; 
    unsigned char *op,*ip; 
    for(op = out,ip = in; op < out+stride*esize; ip++)
      for(i = 0; i < esize; i++) 
        *op++ = ip[i*stride];
    for(ip = in+esize*stride; op < out+n;) 
      *op++ = *ip++;
  }
}  

#define ODX2 e + (_x + _y * x) * esize  
void tp2denc(unsigned char *in, unsigned x, unsigned y, unsigned char *out, unsigned esize) { 
  unsigned _x,_y; int e; uint8_t *op = out, *ip = in;
  for(e = esize-1; e >= 0; e--)
    for(  _x = 0; _x < x; _x++)
      for(_y = 0; _y < y; _y++) op[ODX2] = *ip++; 
}  

void tp2ddec(unsigned char *in, unsigned x, unsigned y, unsigned char *out, unsigned esize) { 
  unsigned _x,_y; int e; uint8_t *op=out,*ip=in;
  for(e = esize-1; e >= 0; e--)
    for(  _x = 0; _x < x; _x++)
      for(_y = 0; _y < y; _y++) *op++ = ip[ODX2];
}  

#define ODX3 e + (_x + _y * x + _z * y * x) * esize
void tp3denc(unsigned char *in, unsigned x, unsigned y, unsigned z, unsigned char *out, unsigned esize) {
  unsigned _x,_y,_z; int e; uint8_t *op = out, *ip=in;
  for(e = esize-1; e >= 0; e--)
    for(    _x = 0; _x < x; _x++)
      for(  _y = 0; _y < y; _y++)
        for(_z = 0; _z < z; _z++) op[ODX3] = *ip++; 
}

void tp3ddec(unsigned char *in, unsigned x, unsigned y, unsigned z, unsigned char *out, unsigned esize) {
  unsigned _x,_y,_z; int e; uint8_t *op=out,*ip=in;
  for(e = esize-1; e >= 0; e--)
    for(_x = 0; _x < x; ++_x)
      for(_y = 0; _y < y; ++_y)
        for(_z = 0; _z < z; ++_z) *op++= ip[ODX3];
}  

#define ODX4 e + (_w + _x * w + _y * x * w + _z * x * y * w) * esize
void tp4denc(unsigned char *in, unsigned w, unsigned x, unsigned y, unsigned z, unsigned char *out, unsigned esize) { 
  unsigned _w,_x,_y,_z; int e; uint8_t *op = out, *ip=in;
  for(e = esize-1; e >= 0; e--)
    for(      _w = 0; _w < w; _w++)
      for(    _x = 0; _x < x; _x++)
        for(  _y = 0; _y < y; _y++)
          for(_z = 0; _z < z; _z++) op[ODX4] = *ip++; 
}

void tp4ddec(unsigned char *in, unsigned w, unsigned x, unsigned y, unsigned z, unsigned char *out, unsigned esize) {  
  unsigned _w,_x,_y,_z; int e; uint8_t *op=out,*ip=in;
  for(e = esize-1; e >= 0; e--)
    for(      _w = 0; _w < w; _w++)
      for(    _x = 0; _x < x; ++_x)
        for(  _y = 0; _y < y; ++_y)
          for(_z = 0; _z < z; ++_z) *op++= ip[ODX4];
}  

  #ifdef USE_SSE
void tp4enc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) { 
  TPFUNC f;
  if(!tpset) tpini(0); 
  if(esize <= 16 && (f = _tp4e[esize])) f(in,n,out);
  else tpenc(in,n,out,esize);
}  

void tp4dec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) { 
  TPFUNC f;
  if(!tpset) tpini(0); 
  if(esize <= 16 && (f = _tp4d[esize])) f(in,n,out);
  else tpdec(in,n,out,esize);  
}
  #endif
#endif


#else

#define SIE(p,i)  (p+=stride) //faster on ARM
//#define SIE(_p_,_i_)  (_p_+ _i_*stride)

#define SID(p,i)  (p+=stride)
//#define SID(_p_,_i_)  (_p_+ _i_*stride)

#if !defined(SSE2_ON) && !defined(AVX2_ON)

  #if STRIDE == ESIZE
void TEMPLATE2(TPENC, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) { 
  unsigned char *op,*ip,*e;
  unsigned stride = n/STRIDE; 

    #if powof2(ESIZE)
  e = in+(n&~(ESIZE-1));
    #else      
  e = in+stride*ESIZE;      
    #endif
    
  for(ip = in,op = out; ip < e; op++, ip+=ESIZE) { unsigned char *p = op;
    p[0]      = ip[ 0]; 
    *SIE(p, 1) = ip[ 1];
      #if ESIZE > 2
    *SIE(p, 2) = ip[ 2];
        #if ESIZE > 3
    *SIE(p, 3) = ip[ 3];
          #if ESIZE > 4
    uint32_t u = ctou32(p);
    *SIE(p, 4) = ip[ 4];
    *SIE(p, 5) = ip[ 5];
    *SIE(p, 6) = ip[ 6];
    *SIE(p, 7) = ip[ 7];
            #if ESIZE > 8
    *SIE(p, 8) = ip[ 8];
    *SIE(p, 9) = ip[ 9];
    *SIE(p,10) = ip[10];
    *SIE(p,11) = ip[11];
    *SIE(p,12) = ip[12];
    *SIE(p,13) = ip[13];
    *SIE(p,14) = ip[14];
    *SIE(p,15) = ip[15];
            #endif
          #endif
        #endif
      #endif                        
  }
  for(op = out+stride*ESIZE;ip < in+n;) 
    *op++ = *ip++;
} 

void TEMPLATE2(TPDEC, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) { 
  unsigned char *op,*ip,*e; 
  unsigned      stride = n/STRIDE; 
  
    #if powof2(ESIZE)
  e = out+(n&~(ESIZE-1));
    #else      
  e = out+stride*ESIZE;     
    #endif
  for(op = out,ip = in; op < e; ip++,op+=ESIZE) { unsigned char *p = ip; 
    op[ 0] = *p;
    op[ 1] = *SID(p,1); 
      #if ESIZE > 2
    op[ 2] = *SID(p,2);
        #if ESIZE > 3   
    op[ 3] = *SID(p,3);
          #if ESIZE > 4
    op[ 4] = *SID(p,4);
    op[ 5] = *SID(p,5);
    op[ 6] = *SID(p,6);
    op[ 7] = *SID(p,7);
            #if ESIZE > 8
    op[ 8] = *SID(p,8);
    op[ 9] = *SID(p,9);
    op[10] = *SID(p,10);
    op[11] = *SID(p,11);
    op[12] = *SID(p,12);
    op[13] = *SID(p,13);
    op[14] = *SID(p,14);
    op[15] = *SID(p,15);
            #endif
          #endif
        #endif
      #endif
  }
  for(ip = in+stride*ESIZE; op < out+n; ) 
    *op++ = *ip++;
}  
  #endif // STRIDE
#endif

#if ESIZE == 2 || ESIZE == 4 || ESIZE == 8


  #if (defined(__SSE2__) || defined(__ARM_NEON)) && defined(SSE2_ON)  
#define ST(_p_,_v_,_i_)  _mm_storeu_si128((__m128i *)SIE(_p_,_i_), _v_)
#define ST0(_p_,_v_)  _mm_storeu_si128((__m128i *)(_p_), _v_)

void TEMPLATE2(TPENC128V, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned           v = n&~(ESIZE*32-1); 
  unsigned      stride = v/STRIDE; 
  unsigned char *op,*ip;

    #if defined(__SSE3__) || defined(__ARM_NEON)
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
                             8,  0  );
      #endif
    #endif

      #if STRIDE > ESIZE
  __m128i cl = _mm_set1_epi8(0x0f), ch=_mm_set1_epi8(0xf0), cb = _mm_set1_epi16(0xff);
      #endif

  for(ip = in, op = out; ip != in+v; ip+=ESIZE*16,op += ESIZE*16/STRIDE) { unsigned char *p = op;   PREFETCH(ip+(ESIZE*16)*ESIZE,0);
    __m128i iv[ESIZE],ov[ESIZE];
      #if defined(__SSSE3__) || defined(__ARM_NEON)
        #if   ESIZE == 2
          #ifdef __ARM_NEON
    uint8x16x2_t w = vld2q_u8(ip);
            #if STRIDE <= ESIZE
    ST0(p,(__m128i)w.val[0]); ST(p,(__m128i)w.val[1],1);
            #else
    iv[0] = (__m128i)w.val[0]; iv[1] = (__m128i)w.val[1];
            #endif
          #else
    ov[0] = LD128(ip);    ov[0] = _mm_shuffle_epi8(ov[0], sv); 
    ov[1] = LD128(ip+16); ov[1] = _mm_shuffle_epi8(ov[1], sv); 

    iv[0] = _mm_unpacklo_epi64(ov[0], ov[1]); iv[1] = _mm_unpackhi_epi64(ov[0], ov[1]);
            #if STRIDE <= ESIZE
    ST0(p,iv[0]); ST(p,iv[1],1);
            #endif
          #endif
                                                                                            
        #elif ESIZE == 4
          #ifdef __ARM_NEON
    uint8x16x4_t w = vld4q_u8(ip); 
            #if STRIDE <= ESIZE
    ST0(p,(__m128i)w.val[0]); ST(p,(__m128i)w.val[1],1); ST(p,(__m128i)w.val[2],2); ST(p,(__m128i)w.val[3],3); 
            #else
    iv[0] = (__m128i)w.val[0]; iv[1] = (__m128i)w.val[1]; iv[2] = (__m128i)w.val[2]; iv[3] = (__m128i)w.val[3];
            #endif
          #else
    iv[0] = LD128(ip   ); iv[0] = _mm_shuffle_epi8(iv[0], sv);
    iv[1] = LD128(ip+16); iv[1] = _mm_shuffle_epi8(iv[1], sv); 
    iv[2] = LD128(ip+32); iv[2] = _mm_shuffle_epi8(iv[2], sv); 
    iv[3] = LD128(ip+48); iv[3] = _mm_shuffle_epi8(iv[3], sv); 

    ov[0] = _mm_unpacklo_epi32(iv[0], iv[1]); ov[1] = _mm_unpackhi_epi32(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi32(iv[2], iv[3]); ov[3] = _mm_unpackhi_epi32(iv[2], iv[3]);

    iv[0] = _mm_unpacklo_epi64(ov[0], ov[2]); iv[1] = _mm_unpackhi_epi64(ov[0], ov[2]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[3]); iv[3] = _mm_unpackhi_epi64(ov[1], ov[3]);
            #if STRIDE <= ESIZE
    ST0(p,iv[0]); ST(p,iv[1],1); ST(p,iv[2],2); ST(p,iv[3],3);
            #endif
          #endif
                                                                                            
        #elif ESIZE == 8
          #ifdef __ARM_NEON
#define vzipl_u16(_a_,_b_) vzip_u16(vget_low_u16((uint16x8_t)(_a_)), vget_low_u16((uint16x8_t)(_b_)))
#define vziph_u16(_a_,_b_) vzip_u16(vget_high_u16((uint16x8_t)(_a_)), vget_high_u16((uint16x8_t)(_b_)))
//#define VQ
            #ifndef VQ
    uint16x4x2_t v16[8]; 
    uint32x2x2_t v32[8]; 
            #else
    uint8x16x2_t v8[4]; 
    uint16x8x2_t v16[4]; 
    uint32x4x2_t v32[4];   //uint64x2x2_t v64[4]; 
            #endif
            #ifdef VQ  
    ov[0] = LD128(ip    ); //ov[0] = _mm_shuffle_epi8(ov[0], sv); 
    ov[1] = LD128(ip+ 16); //ov[1] = _mm_shuffle_epi8(ov[1], sv); 
    ov[2] = LD128(ip+ 32); //ov[2] = _mm_shuffle_epi8(ov[2], sv); 
    ov[3] = LD128(ip+ 48); //ov[3] = _mm_shuffle_epi8(ov[3], sv);    
    ov[4] = LD128(ip+ 64); //ov[4] = _mm_shuffle_epi8(ov[4], sv); 
    ov[5] = LD128(ip+ 80); //ov[5] = _mm_shuffle_epi8(ov[5], sv);  
    ov[6] = LD128(ip+ 96); //ov[6] = _mm_shuffle_epi8(ov[6], sv); 
    ov[7] = LD128(ip+112); //ov[7] = _mm_shuffle_epi8(ov[7], sv); 
                                                                                            
    v8[0]  = vzipq_u8((uint8x16_t)ov[0], (uint8x16_t)ov[1]); 
    v8[1]  = vzipq_u8((uint8x16_t)ov[2], (uint8x16_t)ov[3]); 
    v8[2]  = vzipq_u8((uint8x16_t)ov[4], (uint8x16_t)ov[5]); 
    v8[3]  = vzipq_u8((uint8x16_t)ov[6], (uint8x16_t)ov[7]);  

/*    v16[0] = vzipq_u16((uint16x8_t)ov[0], (uint16x8_t)ov[1]);
    v16[1] = vzipq_u16((uint16x8_t)ov[2], (uint16x8_t)ov[3]);
    v16[2] = vzipq_u16((uint16x8_t)ov[4], (uint16x8_t)ov[5]);
    v16[3] = vzipq_u16((uint16x8_t)ov[6], (uint16x8_t)ov[7]);*/
    v16[0] = vzipq_u16(vreinterpretq_u16_u8( v8[0].val[0]),  vreinterpretq_u16_u8(v8[1].val[0]));
    v16[1] = vzipq_u16(vreinterpretq_u16_u8( v8[0].val[1]),  vreinterpretq_u16_u8(v8[1].val[1]));
    v16[2] = vzipq_u16(vreinterpretq_u16_u8( v8[2].val[0]),  vreinterpretq_u16_u8(v8[3].val[0]));
    v16[3] = vzipq_u16(vreinterpretq_u16_u8( v8[2].val[1]),  vreinterpretq_u16_u8(v8[3].val[1]));

    v32[0] = vzipq_u32(vreinterpretq_u32_u16(v16[0].val[0]), vreinterpretq_u32_u16(v16[2].val[0]));
    v32[1] = vzipq_u32(vreinterpretq_u32_u16(v16[0].val[1]), vreinterpretq_u32_u16(v16[2].val[1]));
    v32[2] = vzipq_u32(vreinterpretq_u32_u16(v16[1].val[0]), vreinterpretq_u32_u16(v16[3].val[0]));
    v32[3] = vzipq_u32(vreinterpretq_u32_u16(v16[1].val[1]), vreinterpretq_u32_u16(v16[3].val[1]));

    iv[0] = _mm_unpacklo_epi64(v32[0].val[0], v32[2].val[0]); iv[1] = _mm_unpackhi_epi64(v32[0].val[0], v32[2].val[0]);
    iv[2] = _mm_unpacklo_epi64(v32[0].val[1], v32[2].val[1]); iv[3] = _mm_unpackhi_epi64(v32[0].val[1], v32[2].val[1]);
    iv[4] = _mm_unpacklo_epi64(v32[1].val[0], v32[3].val[0]); iv[5] = _mm_unpackhi_epi64(v32[1].val[0], v32[3].val[0]);
    iv[6] = _mm_unpacklo_epi64(v32[1].val[1], v32[3].val[1]); iv[7] = _mm_unpackhi_epi64(v32[1].val[1], v32[3].val[1]);
            #else
    ov[0] = LD128(ip    ); ov[0] = _mm_shuffle_epi8(ov[0], sv); 
    ov[1] = LD128(ip+ 16); ov[1] = _mm_shuffle_epi8(ov[1], sv); 
    ov[2] = LD128(ip+ 32); ov[2] = _mm_shuffle_epi8(ov[2], sv); 
    ov[3] = LD128(ip+ 48); ov[3] = _mm_shuffle_epi8(ov[3], sv);         
    ov[4] = LD128(ip+ 64); ov[4] = _mm_shuffle_epi8(ov[4], sv); 
    ov[5] = LD128(ip+ 80); ov[5] = _mm_shuffle_epi8(ov[5], sv);  
    ov[6] = LD128(ip+ 96); ov[6] = _mm_shuffle_epi8(ov[6], sv); 
    ov[7] = LD128(ip+112); ov[7] = _mm_shuffle_epi8(ov[7], sv);                 
    v16[0] = vzipl_u16(ov[0], ov[1]); v16[1] = vziph_u16(ov[0], ov[1]); 
    v16[2] = vzipl_u16(ov[2], ov[3]); v16[3] = vziph_u16(ov[2], ov[3]);  
    v16[4] = vzipl_u16(ov[4], ov[5]); v16[5] = vziph_u16(ov[4], ov[5]);
    v16[6] = vzipl_u16(ov[6], ov[7]); v16[7] = vziph_u16(ov[6], ov[7]);

    v32[0] = vzip_u32(vreinterpret_u32_u16(v16[0].val[0]), vreinterpret_u32_u16(v16[2].val[0]) );
    v32[1] = vzip_u32(vreinterpret_u32_u16(v16[0].val[1]), vreinterpret_u32_u16(v16[2].val[1]) );
    v32[2] = vzip_u32(vreinterpret_u32_u16(v16[1].val[0]), vreinterpret_u32_u16(v16[3].val[0]) );
    v32[3] = vzip_u32(vreinterpret_u32_u16(v16[1].val[1]), vreinterpret_u32_u16(v16[3].val[1]) );
    v32[4] = vzip_u32(vreinterpret_u32_u16(v16[4].val[0]), vreinterpret_u32_u16(v16[6].val[0]) );
    v32[5] = vzip_u32(vreinterpret_u32_u16(v16[4].val[1]), vreinterpret_u32_u16(v16[6].val[1]) );
    v32[6] = vzip_u32(vreinterpret_u32_u16(v16[5].val[0]), vreinterpret_u32_u16(v16[7].val[0]) );
    v32[7] = vzip_u32(vreinterpret_u32_u16(v16[5].val[1]), vreinterpret_u32_u16(v16[7].val[1]) );

    iv[0] = (__m128i)vcombine_u64(vreinterpret_u64_u32(v32[0].val[0]), vreinterpret_u64_u32(v32[4].val[0]) );
    iv[1] = (__m128i)vcombine_u64(vreinterpret_u64_u32(v32[0].val[1]), vreinterpret_u64_u32(v32[4].val[1]) );
    iv[2] = (__m128i)vcombine_u64(vreinterpret_u64_u32(v32[1].val[0]), vreinterpret_u64_u32(v32[5].val[0]) );
    iv[3] = (__m128i)vcombine_u64(vreinterpret_u64_u32(v32[1].val[1]), vreinterpret_u64_u32(v32[5].val[1]) );

    iv[4] = (__m128i)vcombine_u64(vreinterpret_u64_u32(v32[2].val[0]), vreinterpret_u64_u32(v32[6].val[0]) );
    iv[5] = (__m128i)vcombine_u64(vreinterpret_u64_u32(v32[2].val[1]), vreinterpret_u64_u32(v32[6].val[1]) );
    iv[6] = (__m128i)vcombine_u64(vreinterpret_u64_u32(v32[3].val[0]), vreinterpret_u64_u32(v32[7].val[0]) );
    iv[7] = (__m128i)vcombine_u64(vreinterpret_u64_u32(v32[3].val[1]), vreinterpret_u64_u32(v32[7].val[1]) );
            #endif
            #if STRIDE <= ESIZE
    ST0(p,iv[0]); ST(p,iv[1],1); ST(p,iv[2],2); ST(p,iv[3],3); ST(p,iv[4],4); ST(p,iv[5],5); ST(p,iv[6],6); ST(p,iv[7],7);
            #endif
          #else // SSE
    ov[0] = LD128(ip   ); ov[0] = _mm_shuffle_epi8(ov[0], sv); 
    ov[1] = LD128(ip+16); ov[1] = _mm_shuffle_epi8(ov[1], sv); 
    ov[2] = LD128(ip+32); ov[2] = _mm_shuffle_epi8(ov[2], sv); 
    ov[3] = LD128(ip+48); ov[3] = _mm_shuffle_epi8(ov[3], sv);  
    
    iv[0] = _mm_unpacklo_epi16(ov[0], ov[1]); iv[1] = _mm_unpackhi_epi16(ov[0], ov[1]); 
    iv[2] = _mm_unpacklo_epi16(ov[2], ov[3]); iv[3] = _mm_unpackhi_epi16(ov[2], ov[3]);  
    
    ov[0] = _mm_unpacklo_epi32(iv[0], iv[2]); ov[1] = _mm_unpackhi_epi32(iv[0], iv[2]);
    ov[2] = _mm_unpacklo_epi32(iv[1], iv[3]); ov[3] = _mm_unpackhi_epi32(iv[1], iv[3]);
    
    ov[4] = LD128(ip+ 64); ov[4] = _mm_shuffle_epi8(ov[4], sv); 
    ov[5] = LD128(ip+ 80); ov[5] = _mm_shuffle_epi8(ov[5], sv);  
    ov[6] = LD128(ip+ 96); ov[6] = _mm_shuffle_epi8(ov[6], sv); 
    ov[7] = LD128(ip+112); ov[7] = _mm_shuffle_epi8(ov[7], sv);                 

    iv[4] = _mm_unpacklo_epi16(ov[4], ov[5]); iv[5] = _mm_unpackhi_epi16(ov[4], ov[5]);
    iv[6] = _mm_unpacklo_epi16(ov[6], ov[7]); iv[7] = _mm_unpackhi_epi16(ov[6], ov[7]);

    ov[4] = _mm_unpacklo_epi32(iv[4], iv[6]); ov[5] = _mm_unpackhi_epi32(iv[4], iv[6]);
    ov[6] = _mm_unpacklo_epi32(iv[5], iv[7]); ov[7] = _mm_unpackhi_epi32(iv[5], iv[7]);

    iv[0] = _mm_unpacklo_epi64(ov[0], ov[4]); iv[1] = _mm_unpackhi_epi64(ov[0], ov[4]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[5]); iv[3] = _mm_unpackhi_epi64(ov[1], ov[5]);
    
    iv[4] = _mm_unpacklo_epi64(ov[2], ov[6]); iv[5] = _mm_unpackhi_epi64(ov[2], ov[6]);
    iv[6] = _mm_unpacklo_epi64(ov[3], ov[7]); iv[7] = _mm_unpackhi_epi64(ov[3], ov[7]);
             #if STRIDE <= ESIZE
    ST0(p,iv[0]); ST(p,iv[1],1); ST(p,iv[2],2); ST(p,iv[3],3); ST(p,iv[4],4); ST(p,iv[5],5); ST(p,iv[6],6); ST(p,iv[7],7);
            #endif
          #endif
        #endif        
                                                                            
      #elif defined(__SSE2__) 
        #if ESIZE == 2
    iv[0] = LD128(ip   );  iv[1] = LD128(ip+16));                                                       
    
    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]); ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);   
    iv[0] = _mm_unpacklo_epi8(ov[0], ov[1]); iv[1] = _mm_unpackhi_epi8(ov[0], ov[1]);
    
    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]); ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    iv[0] = _mm_unpacklo_epi8(ov[0], ov[1]); iv[1] = _mm_unpackhi_epi8(ov[0], ov[1]);
    ST0(p,iv[0]); ST(p,iv[1],1);
        #elif ESIZE == 4
    iv[0] = LD128(ip   ); iv[1] = LD128(ip+16); iv[2] = LD128(ip+32); iv[3] = LD128(ip+48);                                                                         

    ov[0] = _mm_unpacklo_epi8( iv[0], iv[1]); ov[1] = _mm_unpackhi_epi8( iv[0], iv[1]);
    iv[0] = _mm_unpacklo_epi8( ov[0], ov[1]); iv[1] = _mm_unpackhi_epi8( ov[0], ov[1]);

    ov[0] = _mm_unpacklo_epi8( iv[0], iv[1]); ov[1] = _mm_unpackhi_epi8( iv[0], iv[1]);
    iv[0] = _mm_unpacklo_epi64(ov[0], ov[2]); iv[1] = _mm_unpackhi_epi64(ov[0], ov[2]);

    ov[2] = _mm_unpacklo_epi8( iv[2], iv[3]); ov[3] = _mm_unpackhi_epi8( iv[2], iv[3]);
    iv[2] = _mm_unpacklo_epi8( ov[2], ov[3]); iv[3] = _mm_unpackhi_epi8( ov[2], ov[3]);
    ov[2] = _mm_unpacklo_epi8( iv[2], iv[3]); ov[3] = _mm_unpackhi_epi8( iv[2], iv[3]);
    
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[3]); iv[3] = _mm_unpackhi_epi64(ov[1], ov[3]);
    ST0(p,iv[0]); ST(p,iv[1],1); ST(p,iv[2],2); ST(p,iv[3],3);
        #elif ESIZE == 8
    iv[0] = LD128(ip   ); iv[1] = LD128(ip+16); iv[2] = LD128(ip+32); iv[3] = LD128(ip+48); 
    iv[4] = LD128(ip+64); iv[5] = LD128(ip+80); iv[6] = LD128(ip+96); iv[7] = LD128(ip+112); 

    ov[0] = _mm_unpacklo_epi8( iv[0], iv[1]); ov[1] = _mm_unpackhi_epi8( iv[0], iv[1]); 
    ov[2] = _mm_unpacklo_epi8( iv[2], iv[3]); ov[3] = _mm_unpackhi_epi8( iv[2], iv[3]);  
    ov[4] = _mm_unpacklo_epi8( iv[4], iv[5]); ov[5] = _mm_unpackhi_epi8( iv[4], iv[5]);
    ov[6] = _mm_unpacklo_epi8( iv[6], iv[7]); ov[7] = _mm_unpackhi_epi8( iv[6], iv[7]);

    iv[0] = _mm_unpacklo_epi8( ov[0], ov[1]); iv[1] = _mm_unpackhi_epi8( ov[0], ov[1]); 
    iv[2] = _mm_unpacklo_epi8( ov[2], ov[3]); iv[3] = _mm_unpackhi_epi8( ov[2], ov[3]);  
    iv[4] = _mm_unpacklo_epi8( ov[4], ov[5]); iv[5] = _mm_unpackhi_epi8( ov[4], ov[5]);
    iv[6] = _mm_unpacklo_epi8( ov[6], ov[7]); iv[7] = _mm_unpackhi_epi8( ov[6], ov[7]);
    
    ov[0] = _mm_unpacklo_epi32(iv[0], iv[2]); ov[1] = _mm_unpackhi_epi32(iv[0], iv[2]);
    ov[2] = _mm_unpacklo_epi32(iv[1], iv[3]); ov[3] = _mm_unpackhi_epi32(iv[1], iv[3]);
    ov[4] = _mm_unpacklo_epi32(iv[4], iv[6]); ov[5] = _mm_unpackhi_epi32(iv[4], iv[6]);
    ov[6] = _mm_unpacklo_epi32(iv[5], iv[7]); ov[7] = _mm_unpackhi_epi32(iv[5], iv[7]);
    ST0(p,iv[0]); ST(p,iv[1],1); ST(p,iv[2],2); ST(p,iv[3],3); 

    iv[0] = _mm_unpacklo_epi64(ov[0], ov[4]); iv[1] = _mm_unpackhi_epi64(ov[0], ov[4]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[5]); iv[3] = _mm_unpackhi_epi64(ov[1], ov[5]);
    iv[4] = _mm_unpacklo_epi64(ov[2], ov[6]); iv[5] = _mm_unpackhi_epi64(ov[2], ov[6]);
    iv[6] = _mm_unpacklo_epi64(ov[3], ov[7]); iv[7] = _mm_unpackhi_epi64(ov[3], ov[7]);
    ST(p,iv[4],4); ST(p,iv[5],5); ST(p,iv[6],6); ST(p,iv[7],7);
        #endif
      #endif

      #if STRIDE > ESIZE // ---------------------- Nibble -------------------------------------------
    #define STL(_p_,_v_,_i_)  _mm_storel_epi64((__m128i *)SIE(_p_,_i_), _v_)
    #define STL0(_p_,_v_)     _mm_storel_epi64((__m128i *)(_p_), _v_)

    ov[0] = _mm_and_si128(iv[0], cl);                   ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb); ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(ov[0],2));                                           
    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[0], ch),4); ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb); ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128(ov[1],2));                                           
    ov[2] = _mm_and_si128(iv[1], cl);                   ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb); ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(ov[2],2));                                           
    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[1], ch),4); ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb); ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(ov[3],2));                                           
    STL0(p,ov[0]); STL(p,ov[1],1);STL(p,ov[2],2);STL(p,ov[3],3);
        #if ESIZE > 2
    ov[0] = _mm_and_si128(iv[2], cl);                   ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb); ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(ov[0],2));                                           
    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[2], ch),4); ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb); ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128(ov[1],2));                                           
    ov[2] = _mm_and_si128(iv[3], cl);                   ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb); ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(ov[2],2));                                           
    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[3], ch),4); ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb); ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(ov[3],2));                                           
    STL(p,ov[0],4); STL(p,ov[1],5);STL(p,ov[2],6);STL(p,ov[3],7);
          #if ESIZE > 4
    ov[0] = _mm_and_si128(iv[4], cl);                   ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb); ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(ov[0],2));                                           
    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[4], ch),4); ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb); ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128(ov[1],2));                                           
    ov[2] = _mm_and_si128(iv[5], cl);                   ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb); ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(ov[2],2));                                           
    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[5], ch),4); ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb); ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(ov[3],2));                                           
    STL(p,ov[0],8); STL(p,ov[1],9);STL(p,ov[2],10);STL(p,ov[3],11);
    
    ov[4] = _mm_and_si128(iv[6], cl);                   ov[4] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[4],4), ov[4]),cb); ov[4] = _mm_packus_epi16(ov[4], _mm_srli_si128(ov[4],2));                                           
    ov[5] = _mm_srli_epi16(_mm_and_si128(iv[6], ch),4); ov[5] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[5],4), ov[5]),cb); ov[5] = _mm_packus_epi16(ov[5], _mm_srli_si128(ov[5],2));                                           
    ov[6] = _mm_and_si128(iv[7], cl);                   ov[6] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[6],4), ov[6]),cb); ov[6] = _mm_packus_epi16(ov[6], _mm_srli_si128(ov[6],2));                                           
    ov[7] = _mm_srli_epi16(_mm_and_si128(iv[7], ch),4); ov[7] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[7],4), ov[7]),cb); ov[7] = _mm_packus_epi16(ov[7], _mm_srli_si128(ov[7],2));                                           
    STL(p,ov[4],12); STL(p,ov[5],13);STL(p,ov[6],14);STL(p,ov[7],15);
          #endif
        #endif                                              
      #endif                                                            
  }
  TEMPLATE2(tpenc,ESIZE)(in+v, n-v, out+v);
}

void TEMPLATE2(TPDEC128V, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned           v = n&~(ESIZE*32-1); // binary compatible with AVX2 shuffle
  unsigned      stride = v/STRIDE; 
  unsigned char *op, *ip; 
  
    #if STRIDE > ESIZE
  __m128i cl = _mm_set1_epi8(0x0f), ch=_mm_set1_epi8(0xf0), cb = _mm_set1_epi16(0xff);
    #endif

  for(op = out,ip = in; op != out+v; op+=ESIZE*16,ip += ESIZE*16/STRIDE) { 
    unsigned char *p = ip;                                                        PREFETCH(ip+(ESIZE*16/STRIDE)*ESIZE,0);
    __m128i iv[ESIZE], ov[ESIZE];

      #if STRIDE > ESIZE //------------ Nibble transpose -------------------
    ov[0] = _mm_loadl_epi64((__m128i *)    p   );
    ov[1] = _mm_loadl_epi64((__m128i *)SID(p,1));   
    ov[2] = _mm_loadl_epi64((__m128i *)SID(p,2));
    ov[3] = _mm_loadl_epi64((__m128i *)SID(p,3));

    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); ov[0] = _mm_and_si128(ov[0], cl); // 0,1->0
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); ov[1] = _mm_and_si128(ov[1], cl);                                                
    iv[0] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);                   
 
    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); ov[2] = _mm_and_si128(ov[2], cl); // 2,3->1
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); ov[3] = _mm_and_si128(ov[3], cl);                                                
    iv[1] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);                                        
        #if ESIZE > 2
    ov[0] = _mm_loadl_epi64((__m128i *)SID(p,4));   
    ov[1] = _mm_loadl_epi64((__m128i *)SID(p,5));  
    ov[2] = _mm_loadl_epi64((__m128i *)SID(p,6));   
    ov[3] = _mm_loadl_epi64((__m128i *)SID(p,7));  

    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); ov[0] = _mm_and_si128(ov[0], cl); // 0,1->2                                              
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); ov[1] = _mm_and_si128(ov[1], cl);                                                
    iv[2] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);                   

    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); ov[2] = _mm_and_si128(ov[2], cl); // 2,3->3                                              
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); ov[3] = _mm_and_si128(ov[3], cl);                                                
    iv[3] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);                                   
        #endif
        #if ESIZE > 4
    ov[0] = _mm_loadl_epi64((__m128i *)SID(p,8));   
    ov[1] = _mm_loadl_epi64((__m128i *)SID(p,9));   
    ov[2] = _mm_loadl_epi64((__m128i *)SID(p,10));   
    ov[3] = _mm_loadl_epi64((__m128i *)SID(p,11));  

    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); ov[0] = _mm_and_si128(ov[0], cl); // 0,1->4                                              
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); ov[1] = _mm_and_si128(ov[1], cl);                                                
    iv[4] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);                   

    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); ov[2] = _mm_and_si128(ov[2], cl); // 2,3->5                                              
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4));                      
    ov[3] = _mm_and_si128(ov[3], cl);                                               
    iv[5] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);   

    ov[0] = _mm_loadl_epi64((__m128i *)SID(p,12));   
    ov[1] = _mm_loadl_epi64((__m128i *)SID(p,13));  
    ov[2] = _mm_loadl_epi64((__m128i *)SID(p,14));   
    ov[3] = _mm_loadl_epi64((__m128i *)SID(p,15));   

    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); ov[0] = _mm_and_si128(ov[0], cl); // 0,1->6                                              
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); ov[1] = _mm_and_si128(ov[1], cl);                                                
    iv[6] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);                   

    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); ov[2] = _mm_and_si128(ov[2], cl); // 2,3->7                                              
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); ov[3] = _mm_and_si128(ov[3], cl);                                                
    iv[7] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);                                           
        #endif  
      #else // --------------------------- Byte transpose -------------------
    iv[0] = _mm_loadu_si128((__m128i *)    p   );   
    iv[1] = _mm_loadu_si128((__m128i *)SID(p,1));   
        #if ESIZE > 2
    iv[2] = _mm_loadu_si128((__m128i *)SID(p,2));   
    iv[3] = _mm_loadu_si128((__m128i *)SID(p,3));   
          #if ESIZE > 4
    iv[4] = _mm_loadu_si128((__m128i *)SID(p,4));   
    iv[5] = _mm_loadu_si128((__m128i *)SID(p,5));   
    iv[6] = _mm_loadu_si128((__m128i *)SID(p,6));   
    iv[7] = _mm_loadu_si128((__m128i *)SID(p,7));  
          #endif
        #endif
      #endif
      #if ESIZE == 2
        #ifdef __ARM_NEON
    uint8x16x2_t w; w.val[0] = (uint8x16_t)iv[0]; 
                    w.val[1] = (uint8x16_t)iv[1]; vst2q_u8(op, w);
        #else
    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]); ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);//i(0,1)->o(0,1)  
    ST128(op,   ov[0]); ST128(op+16, ov[1]);
        #endif
      #elif ESIZE == 4
        #ifdef __ARM_NEON
    uint8x16x4_t w; w.val[0] = (uint8x16_t)iv[0]; 
                    w.val[1] = (uint8x16_t)iv[1];
                    w.val[2] = (uint8x16_t)iv[2]; 
                    w.val[3] = (uint8x16_t)iv[3]; vst4q_u8(op,w);
        #else
    ov[0] = _mm_unpacklo_epi8( iv[0], iv[1]); ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]); //i(0,1)->o(0,1)
    ov[2] = _mm_unpacklo_epi8( iv[2], iv[3]); ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]); //i(2,3)->o(2,3)
    
    iv[0] = _mm_unpacklo_epi16(ov[0], ov[2]); iv[1] = _mm_unpackhi_epi16(ov[0], ov[2]);//o(0,2)->i(0,1)
    iv[2] = _mm_unpacklo_epi16(ov[1], ov[3]); iv[3] = _mm_unpackhi_epi16(ov[1], ov[3]);//o(1,3)->i(2,3)
    ST128(op,   iv[0]); ST128(op+16,iv[1]); ST128(op+32,iv[2]); ST128(op+48,iv[3]); 
        #endif 
      #else
    ov[0] = _mm_unpacklo_epi8( iv[0], iv[1]); ov[1] = _mm_unpackhi_epi8( iv[0], iv[1]);//i(0,1)->o(0,1)
    ov[2] = _mm_unpacklo_epi8( iv[2], iv[3]); ov[3] = _mm_unpackhi_epi8( iv[2], iv[3]);//i(2,3)->o(2,3)
    ov[4] = _mm_unpacklo_epi8( iv[4], iv[5]); ov[5] = _mm_unpackhi_epi8( iv[4], iv[5]);//i(4,5)->o(4,5)
    ov[6] = _mm_unpacklo_epi8( iv[6], iv[7]); ov[7] = _mm_unpackhi_epi8( iv[6], iv[7]);//i(6,7)->o(6,7)

    iv[0] = _mm_unpacklo_epi16(ov[0], ov[2]); iv[1] = _mm_unpackhi_epi16(ov[0], ov[2]);
    iv[2] = _mm_unpacklo_epi16(ov[1], ov[3]); iv[3] = _mm_unpackhi_epi16(ov[1], ov[3]);
    iv[4] = _mm_unpacklo_epi16(ov[4], ov[6]); iv[5] = _mm_unpackhi_epi16(ov[4], ov[6]);
    iv[6] = _mm_unpacklo_epi16(ov[5], ov[7]); iv[7] = _mm_unpackhi_epi16(ov[5], ov[7]); 
            
    ov[0] = _mm_unpacklo_epi32(iv[0], iv[4]); ov[1] = _mm_unpackhi_epi32(iv[0], iv[4]); 
    ov[2] = _mm_unpacklo_epi32(iv[1], iv[5]); ov[3] = _mm_unpackhi_epi32(iv[1], iv[5]);  
    ov[4] = _mm_unpacklo_epi32(iv[2], iv[6]); ov[5] = _mm_unpackhi_epi32(iv[2], iv[6]); 
    ov[6] = _mm_unpacklo_epi32(iv[3], iv[7]); ov[7] = _mm_unpackhi_epi32(iv[3], iv[7]);

    ST128(op,   ov[0]); ST128(op+16,ov[1]); ST128(op+32,ov[2]); ST128(op+48, ov[3]); 
    ST128(op+64,ov[4]); ST128(op+80,ov[5]); ST128(op+96,ov[6]); ST128(op+112,ov[7]); 
      #endif
  }
  TEMPLATE2(tpdec,ESIZE)(in+v, n-v, out+v);
}
  #endif

  #if defined(__AVX2__) && defined(AVX2_ON)
void TEMPLATE2(TPENC256V, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned      v      = n&~(ESIZE*32-1);
  unsigned      stride = v/STRIDE; 
  unsigned char *op, *ip;
  
    #if ESIZE == 2
  __m256i sv = _mm256_set_epi8( 15, 13, 11, 9, 7, 5, 3, 1, 
                                14, 12, 10, 8, 6, 4, 2, 0,
                                15, 13, 11, 9, 7, 5, 3, 1, 
                                14, 12, 10, 8, 6, 4, 2, 0);
  __m256i sv0 = _mm256_set_epi8(15, 13, 11, 9,  
                                 7,  5,  3, 1,
                                14, 12, 10, 8, 
                                 6,  4,  2, 0,
                                15, 13, 11, 9,  
                                 7,  5,  3, 1,
                                14, 12, 10, 8, 
                                 6,  4,  2, 0);
  __m256i sv1 = _mm256_set_epi8(14, 12, 10, 8, 
                                 6,  4,  2, 0,
                                15, 13, 11, 9,
                                 7,  5,  3, 1,
                                14, 12, 10, 8, 
                                 6,  4,  2, 0,
                                15, 13, 11, 9,
                                 7,  5,  3, 1);
   #else
  __m256i pv = _mm256_set_epi32( 7, 3, 6, 2, 5, 1, 4, 0),
      #if ESIZE == 4
          sv0 = _mm256_set_epi8(15, 11, 7, 3, 
                                13,  9, 5, 1,
                                14, 10, 6, 2, 
                                12,  8, 4, 0,
                                15, 11, 7, 3, 
                                13,  9, 5, 1,
                                14, 10, 6, 2, 
                                12,  8, 4, 0),
           sv1= _mm256_set_epi8(13,  9, 5, 1,
                                15, 11, 7, 3, 
                                12,  8, 4, 0,
                                14, 10, 6, 2, 
                                13,  9, 5, 1,
                                15, 11, 7, 3, 
                                12,  8, 4, 0,
                                14, 10, 6, 2);
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
  
    #if STRIDE > ESIZE // ------------------ byte transpose ----------------------------------
  __m256i cl = _mm256_set1_epi8( 0x0f), 
          ch = _mm256_set1_epi8( 0xf0), 
          cb = _mm256_set1_epi16(0xff);
    #endif

  for(ip = in,op = out; ip != in+v; ip += ESIZE*32, op += ESIZE*32/STRIDE) {  	
    unsigned char *p = op;   													PREFETCH(ip+ESIZE*192,0);   	                                                   
    __m256i iv[ESIZE],ov[ESIZE];                                                        
      #if   ESIZE == 2
    ov[0] = _mm256_shuffle_epi8(LD256((__m256i *) ip    ), sv0);       
    ov[1] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+32)), sv1);                                            
    iv[0] = _mm256_permute4x64_epi64(_mm256_blend_epi32(ov[0], ov[1],0b11001100),_MM_SHUFFLE(3, 1, 2, 0));
    iv[1] = _mm256_blend_epi32(ov[0], ov[1],0b00110011); 
    iv[1] = _mm256_permute4x64_epi64(_mm256_shuffle_epi32(iv[1],_MM_SHUFFLE(1, 0, 3, 2)),_MM_SHUFFLE(3, 1, 2, 0));
      #elif ESIZE == 4
    iv[0] = _mm256_shuffle_epi8(LD256((__m256i *) ip    ), sv0);      
    iv[1] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+32)), sv1);  
    iv[2] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+64)), sv0);       
    iv[3] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+96)), sv1);                                            

    ov[0] = _mm256_blend_epi32(iv[0], iv[1],0b10101010); 
    ov[1] = _mm256_shuffle_epi32(_mm256_blend_epi32(iv[0], iv[1],0b01010101),_MM_SHUFFLE(2, 3, 0, 1));

    ov[2] = _mm256_blend_epi32(iv[2], iv[3],0b10101010); 
    ov[3] = _mm256_shuffle_epi32(_mm256_blend_epi32(iv[2], iv[3],0b01010101),_MM_SHUFFLE(2, 3, 0, 1));

    iv[0] = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(ov[0], ov[2]), pv); 
    iv[1] = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(ov[0], ov[2]), pv);
    iv[2] = _mm256_permutevar8x32_epi32(_mm256_unpacklo_epi64(ov[1], ov[3]), pv);
    iv[3] = _mm256_permutevar8x32_epi32(_mm256_unpackhi_epi64(ov[1], ov[3]), pv);   
      #else
    ov[0] = _mm256_shuffle_epi8(LD256((__m256i *) ip    ), sv); 
    ov[1] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+32)), sv); 
    ov[2] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+64)), sv); 
    ov[3] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+96)), sv); 								 
    
    iv[0] = _mm256_unpacklo_epi16(ov[0], ov[1]); iv[1] = _mm256_unpackhi_epi16(ov[0], ov[1]); 
    iv[2] = _mm256_unpacklo_epi16(ov[2], ov[3]); iv[3] = _mm256_unpackhi_epi16(ov[2], ov[3]);  
    
    ov[0] = _mm256_unpacklo_epi32(iv[0], iv[2]); ov[1] = _mm256_unpackhi_epi32(iv[0], iv[2]);
    ov[2] = _mm256_unpacklo_epi32(iv[1], iv[3]); ov[3] = _mm256_unpackhi_epi32(iv[1], iv[3]);
    
    ov[4] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+128)), sv); 
    ov[5] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+160)), sv);  
    ov[6] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+192)), sv); 
    ov[7] = _mm256_shuffle_epi8(LD256((__m256i *)(ip+224)), sv);                                            

    iv[4] = _mm256_unpacklo_epi16(ov[4], ov[5]); iv[5] = _mm256_unpackhi_epi16(ov[4], ov[5]);
    iv[6] = _mm256_unpacklo_epi16(ov[6], ov[7]); iv[7] = _mm256_unpackhi_epi16(ov[6], ov[7]);

    ov[4] = _mm256_unpacklo_epi32(iv[4], iv[6]); ov[5] = _mm256_unpackhi_epi32(iv[4], iv[6]);
    ov[6] = _mm256_unpacklo_epi32(iv[5], iv[7]); ov[7] = _mm256_unpackhi_epi32(iv[5], iv[7]);
    
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
    _mm256_storeu_si256((__m256i *) p,          iv[0]);  
    _mm256_storeu_si256((__m256i *)(p+=stride), iv[1]);
        #if ESIZE > 2
    _mm256_storeu_si256((__m256i *)(p+=stride), iv[2]);
    _mm256_storeu_si256((__m256i *)(p+=stride), iv[3]); 
          #if ESIZE > 4
    _mm256_storeu_si256((__m256i *)(p+=stride), iv[4]);
    _mm256_storeu_si256((__m256i *)(p+=stride), iv[5]);
    _mm256_storeu_si256((__m256i *)(p+=stride), iv[6]);
    _mm256_storeu_si256((__m256i *)(p+=stride), iv[7]);
          #endif
        #endif

      #else //---------------------- Nibble Transpose ------------------------
    #define mm256_packus_epi16(a, b) _mm256_permute4x64_epi64(_mm256_packus_epi16(a, b), _MM_SHUFFLE(3, 1, 2, 0))
    #define ST128(_p_,_v_,_i_) _mm_storeu_si128((__m256i *)SIE(_p_,_i_), _mm256_castsi256_si128(_v_))
    #define ST1280(_p_,_v_)    _mm_storeu_si128((__m256i *)(_p_), _mm256_castsi256_si128(_v_))

    ov[0] = _mm256_and_si256(iv[0], cl);                      ov[0] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[0],4), ov[0]),cb); ov[0] = mm256_packus_epi16(ov[0], _mm256_srli_si256( ov[0],2));                                          
    ov[1] = _mm256_srli_epi16(_mm256_and_si256(iv[0], ch),4); ov[1] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[1],4), ov[1]),cb); ov[1] = mm256_packus_epi16(ov[1], _mm256_srli_si256( ov[1],2));                                          
    ov[2] = _mm256_and_si256(iv[1], cl);                      ov[2] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[2],4), ov[2]),cb); ov[2] = mm256_packus_epi16(ov[2], _mm256_srli_si256( ov[2],2));                                          
    ov[3] = _mm256_srli_epi16(_mm256_and_si256(iv[1], ch),4); ov[3] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[3],4), ov[3]),cb); ov[3] = mm256_packus_epi16(ov[3], _mm256_srli_si256( ov[3],2));
    ST1280(p,ov[0]);  ST128(p,ov[1],1); ST128(p,ov[2],2); ST128(p,ov[3],3);
        #if ESIZE > 2
    ov[0] = _mm256_and_si256(iv[2], cl);                      ov[0] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[0],4), ov[0]),cb); ov[0] = mm256_packus_epi16(ov[0], _mm256_srli_si256( ov[0],2));
    ov[1] = _mm256_srli_epi16(_mm256_and_si256(iv[2], ch),4); ov[1] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[1],4), ov[1]),cb); ov[1] = mm256_packus_epi16(ov[1], _mm256_srli_si256( ov[1],2));
    ov[2] = _mm256_and_si256(iv[3], cl);                      ov[2] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[2],4), ov[2]),cb); ov[2] = mm256_packus_epi16(ov[2], _mm256_srli_si256( ov[2],2));
    ov[3] = _mm256_srli_epi16(_mm256_and_si256(iv[3], ch),4); ov[3] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[3],4), ov[3]),cb); ov[3] = mm256_packus_epi16(ov[3], _mm256_srli_si256( ov[3],2));  
    ST128(p,ov[0],4); ST128(p,ov[1],5); ST128(p,ov[2],6); ST128(p,ov[3],7);
          #if ESIZE > 4
    ov[0] = _mm256_and_si256(iv[4], cl);                      ov[0] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[0],4), ov[0]),cb); ov[0] = mm256_packus_epi16(ov[0], _mm256_srli_si256(  ov[0],2));                                         
    ov[1] = _mm256_srli_epi16(_mm256_and_si256(iv[4], ch),4); ov[1] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[1],4), ov[1]),cb); ov[1] = mm256_packus_epi16(ov[1], _mm256_srli_si256(  ov[1],2));                                         
    ov[2] = _mm256_and_si256(iv[5], cl);                      ov[2] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[2],4), ov[2]),cb); ov[2] = mm256_packus_epi16(ov[2], _mm256_srli_si256(  ov[2],2));                                         
    ov[3] = _mm256_srli_epi16(_mm256_and_si256(iv[5], ch),4); ov[3] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[3],4), ov[3]),cb); ov[3] = mm256_packus_epi16(ov[3], _mm256_srli_si256(  ov[3],2));                                         
    ST128(p,ov[0],8); ST128(p,ov[1],9); ST128(p,ov[2],10); ST128(p,ov[3],11);

    ov[0] = _mm256_and_si256(iv[6], cl);                      ov[0] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[0],4), ov[0]),cb); ov[0] =  mm256_packus_epi16(ov[0], _mm256_srli_si256(  ov[0],2));                                            
    ov[1] = _mm256_srli_epi16(_mm256_and_si256(iv[6], ch),4); ov[1] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[1],4), ov[1]),cb); ov[1] =  mm256_packus_epi16(ov[1], _mm256_srli_si256(  ov[1],2));                                            
    ov[2] = _mm256_and_si256(iv[7], cl);                      ov[2] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[2],4), ov[2]),cb); ov[2] =  mm256_packus_epi16(ov[2], _mm256_srli_si256(  ov[2],2));                                            
    ov[3] = _mm256_srli_epi16(_mm256_and_si256(iv[7], ch),4); ov[3] = _mm256_and_si256(_mm256_or_si256(_mm256_srli_epi16(ov[3],4), ov[3]),cb); ov[3] =  mm256_packus_epi16(ov[3], _mm256_srli_si256(  ov[3],2));                                            
    ST128(p,ov[0],12); ST128(p,ov[1],13); ST128(p,ov[2],14); ST128(p,ov[3],15);
          #endif
        #endif      
      #endif																							 
  }                                                           
  TEMPLATE2(tpenc,ESIZE)(in+v, n-v, out+v);
}

#define NBL0(x,y) ov[x] = _mm256_permute4x64_epi64(_mm256_castsi128_si256(_mm_loadu_si128((__m128i *)(p       ))),_MM_SHUFFLE(3, 1, 2, 0));\
                  ov[y] = _mm256_permute4x64_epi64(_mm256_castsi128_si256(_mm_loadu_si128((__m128i *)(p+=stride))),_MM_SHUFFLE(3, 1, 2, 0));

#define NBL(x,y)  ov[x] = _mm256_permute4x64_epi64(_mm256_castsi128_si256(_mm_loadu_si128((__m128i *)(p+=stride))),_MM_SHUFFLE(3, 1, 2, 0));\
                  ov[y] = _mm256_permute4x64_epi64(_mm256_castsi128_si256(_mm_loadu_si128((__m128i *)(p+=stride))),_MM_SHUFFLE(3, 1, 2, 0));

#define NB(x,y,_iv_) {\
  ov[x] = _mm256_and_si256(_mm256_unpacklo_epi8(ov[x], _mm256_srli_epi16(ov[x],4)), cl);\
  ov[y] = _mm256_and_si256(_mm256_unpacklo_epi8(ov[y], _mm256_srli_epi16(ov[y],4)), cl);\
  _iv_  = _mm256_or_si256(_mm256_slli_epi16(ov[y],4), ov[x]); \
} 

void TEMPLATE2(TPDEC256V, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned      v      = n&~(ESIZE*32-1);
  unsigned      stride = v/STRIDE; 
  unsigned char *op, *ip; 

    #if STRIDE > ESIZE
  __m256i cl = _mm256_set1_epi8(0x0f), ch=_mm256_set1_epi8(0xf0), cb = _mm256_set1_epi16(0xff);
    #endif

  for(op = out,ip = in; op != out+v; ip += ESIZE*32/STRIDE, op += ESIZE*32) {   
    unsigned char *p = ip;    													PREFETCH(ip+ESIZE*192,0);
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
    iv[0] = _mm256_loadu_si256((__m256i *) p        );    
    iv[1] = _mm256_loadu_si256((__m256i *)(p+=stride));   
        #if ESIZE > 2
    iv[2] = _mm256_loadu_si256((__m256i *)(p+=stride));  
    iv[3] = _mm256_loadu_si256((__m256i *)(p+=stride));
          #if ESIZE > 4
    iv[4] = _mm256_loadu_si256((__m256i *)(p+=stride));  
    iv[5] = _mm256_loadu_si256((__m256i *)(p+=stride));             
    iv[6] = _mm256_loadu_si256((__m256i *)(p+=stride));  
    iv[7] = _mm256_loadu_si256((__m256i *)(p+=stride));             
          #endif
        #endif
      #endif
                                                                                                
      #if ESIZE == 2 
    ov[0] = _mm256_permute4x64_epi64(iv[0], _MM_SHUFFLE(3, 1, 2, 0));
    ov[1] = _mm256_permute4x64_epi64(iv[1], _MM_SHUFFLE(3, 1, 2, 0));
    _mm256_storeu_si256((__m256i *)op,      _mm256_unpacklo_epi8(ov[0], ov[1])); 
    _mm256_storeu_si256((__m256i *)(op+32), _mm256_unpackhi_epi8(ov[0], ov[1])); 
      #elif ESIZE == 4    
    ov[0] = _mm256_unpacklo_epi8( iv[0], iv[1]); ov[1] = _mm256_unpackhi_epi8( iv[0], iv[1]);
    ov[2] = _mm256_unpacklo_epi8( iv[2], iv[3]); ov[3] = _mm256_unpackhi_epi8( iv[2], iv[3]);

    iv[0] = _mm256_unpacklo_epi16(ov[0], ov[2]); iv[1] = _mm256_unpackhi_epi16(ov[0], ov[2]);
    iv[2] = _mm256_unpacklo_epi16(ov[1], ov[3]); iv[3] = _mm256_unpackhi_epi16(ov[1], ov[3]);

    ov[0] = _mm256_permute2x128_si256(iv[0], iv[1], (2 << 4) | 0); 
    ov[1] = _mm256_permute2x128_si256(iv[2], iv[3], (2 << 4) | 0); 
    ov[2] = _mm256_permute2x128_si256(iv[0], iv[1], (3 << 4) | 1); 
    ov[3] = _mm256_permute2x128_si256(iv[2], iv[3], (3 << 4) | 1); 
    _mm256_storeu_si256((__m256i *) op,     ov[0]); 
    _mm256_storeu_si256((__m256i *)(op+32), ov[1]); 
    _mm256_storeu_si256((__m256i *)(op+64), ov[2]); 
    _mm256_storeu_si256((__m256i *)(op+96), ov[3]); 
      #else
    ov[0] = _mm256_unpacklo_epi8(iv[0], iv[1]); ov[1] = _mm256_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm256_unpacklo_epi8(iv[2], iv[3]); ov[3] = _mm256_unpackhi_epi8(iv[2], iv[3]);
    iv[0] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi16(ov[0], ov[2]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[1] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi16(ov[0], ov[2]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[2] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi16(ov[1], ov[3]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[3] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi16(ov[1], ov[3]), _MM_SHUFFLE(3, 1, 2, 0));

    ov[4] = _mm256_unpacklo_epi8(iv[4], iv[5]); ov[5] = _mm256_unpackhi_epi8(iv[4], iv[5]);
    ov[6] = _mm256_unpacklo_epi8(iv[6], iv[7]); ov[7] = _mm256_unpackhi_epi8(iv[6], iv[7]);
    iv[4] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi16(ov[4], ov[6]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[5] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi16(ov[4], ov[6]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[6] = _mm256_permute4x64_epi64(_mm256_unpacklo_epi16(ov[5], ov[7]), _MM_SHUFFLE(3, 1, 2, 0));
    iv[7] = _mm256_permute4x64_epi64(_mm256_unpackhi_epi16(ov[5], ov[7]), _MM_SHUFFLE(3, 1, 2, 0));

    ov[0] = _mm256_unpacklo_epi32(iv[0], iv[4]); 
    ov[1] = _mm256_unpacklo_epi32(iv[1], iv[5]); 
    ov[2] = _mm256_unpacklo_epi32(iv[2], iv[6]); 
    ov[3] = _mm256_unpacklo_epi32(iv[3], iv[7]); 
    ov[4] = _mm256_unpackhi_epi32(iv[0], iv[4]); 
    ov[5] = _mm256_unpackhi_epi32(iv[1], iv[5]);   
    ov[6] = _mm256_unpackhi_epi32(iv[2], iv[6]); 
    ov[7] = _mm256_unpackhi_epi32(iv[3], iv[7]); 

    ST256((__m256i *) op,      ov[0] ); 
    ST256((__m256i *)(op+ 32), ov[1] ); 
    ST256((__m256i *)(op+ 64), ov[2] ); 
    ST256((__m256i *)(op+ 96), ov[3] ); 
    ST256((__m256i *)(op+128), ov[4] ); 
    ST256((__m256i *)(op+160), ov[5] );   
    ST256((__m256i *)(op+192), ov[6] ); 
    ST256((__m256i *)(op+224), ov[7] );
      #endif
  }
  if(n-v) TEMPLATE2(tpdec,ESIZE)(in+v, n-v, out+v);
}
  #endif
  
#endif
#endif

