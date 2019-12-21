/**
    Copyright (C) powturbo 2015-2019
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

    TurboRLE - "Most efficient and fastest Run Length Encoding"
**/
  #ifndef USIZE
#include <string.h>
  #ifdef __AVX2__
#include <immintrin.h>
  #elif defined(__AVX__)
#include <immintrin.h>
  #elif defined(__SSE4_1__)
#include <smmintrin.h>
  #elif defined(__SSSE3__)
    #ifdef __powerpc64__
#define __SSE__   1
#define __SSE2__  1
#define __SSE3__  1
#define NO_WARN_X86_INTRINSICS 1
    #endif
#include <tmmintrin.h>
  #elif defined(__SSE2__)
#include <emmintrin.h>
  #elif defined(__ARM_NEON)
#include <arm_neon.h>
#include "sse_neon.h"
  #endif

  #ifdef __ARM_NEON
#define PREFETCH(_ip_,_rw_)
  #else
#define PREFETCH(_ip_,_rw_) __builtin_prefetch(_ip_,_rw_)
  #endif

#include "trle.h"
#include "trle_.h"
//------------------------------------- RLE 8 with Escape char ------------------------------------------------------------------
//#define MEMSAFE
#define SRLE8 32
#define USIZE 8
#include "trled.c"

  #if SRLE8
#define rmemset8(_op_, _c_, _i_) while(_i_--) *_op_++ = _c_
unsigned _srled8(const unsigned char *__restrict in, unsigned char *__restrict out, unsigned outlen, unsigned char e) {
  uint8_t *ip = in, *op = out, c, *oe = out+outlen;
  uint32_t r;

    #ifdef __AVX2__
  __m256i ev = _mm256_set1_epi8(e);
    #elif defined(__SSE__)
  __m128i ev = _mm_set1_epi8(e);
    #endif
  if(outlen >= SRLE8)
    while(op < out+(outlen-SRLE8)) {
        #if defined(__AVX2__) || defined(__SSE__) //|| defined(__ARM_NEON)
      uint32_t mask;
          #ifdef __AVX2__
      __m256i v = _mm256_loadu_si256((__m256i*)ip); _mm256_storeu_si256((__m256i *)op, v); mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(v, ev)); if(mask) goto a; op += 32; ip += 32;
          #elif defined(__SSE__)
      __m128i v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, ev)); if(mask) goto a; ip += 16; op += 16;
            #if SRLE8 >= 32
              v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, ev)); if(mask) goto a; ip += 16; op += 16;
            #endif
          #endif
                                                                                PREFETCH(ip+512, 0);
      continue;
      a: r = ctz32(mask); ip += r+1;                                            PREFETCH(ip+512, 0);
      op += r;
        #else
      if(likely((c = *ip++) != e)) { *op++ = c; continue; }
        #endif
      vlget32(ip, r);
      if(likely(r) >= 3) {
        uint8_t c = *ip++;
        r = r-3+4;
        rmemset(op, c, r);
      } else { r++;
        rmemset8(op, e, r);
      }
    }

  while(op < out+outlen)
    if(likely((c = *ip++) != e)) {
      *op++ = c;
    } else {
      vlget32(ip, r);
      if(likely(r) >= 3) {
        c   = *ip++;
        r  = r-3+4;
        rmemset8(op, c, r);
      } else { r++;
        rmemset8(op, e, r);
      }
    }
  return ip - in;
}

static inline unsigned _srled8x(const unsigned char *__restrict in, unsigned char *__restrict out, unsigned outlen, unsigned char e, unsigned char ix) {
  uint8_t *ip = in, *op = out, c, *oe = out+outlen;
  uint32_t r;

    #ifdef __AVX2__
  __m256i ev = _mm256_set1_epi8(e);
    #elif defined(__SSE__)
  __m128i ev = _mm_set1_epi8(e);
    #elif  defined(__ARM_NEON)
  uint8x8_t ev = vdup_n_u8(e);
    #endif
  if(outlen >= SRLE8)
    while(op < out+(outlen-SRLE8)) {
        #if defined(__AVX2__) || defined(__SSE__) //|| defined(__ARM_NEON)
      uint32_t mask;
          #ifdef __AVX2__
      __m256i v = _mm256_loadu_si256((__m256i*)ip); _mm256_storeu_si256((__m256i *)op, v); mask = _mm256_movemask_epi8(_mm256_cmpeq_epi8(v, ev)); if(mask) goto a; op += 32; ip += 32;
          #elif defined(__SSE__)
      __m128i v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, ev)); if(mask) goto a; ip += 16; op += 16;
            #if SRLE8 >= 32
              v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(_mm_cmpeq_epi8(v, ev)); if(mask) goto a; ip += 16; op += 16;
            #endif
          #endif
                                                                                PREFETCH(ip+512, 0);
      continue;
      a: r = ctz32(mask); ip += r+1;                                            PREFETCH(ip+512, 0);
      op += r;
        #else
      if(likely((c = *ip++) != e)) { *op++ = c; continue; }
        #endif
      vlget32(ip, r);

      int f = r&1; r >>= 1;
      if(likely(r) >= 3) {
        uint8_t y=ip[0],c = f?y:ix; ip+=f;
        r = r-3+4;
        rmemset(op, c, r);
      } else { r++;  rmemset8(op, e, r);
        /*switch(r) {
          case 3: *op++ = c;
          case 2: *op++ = c;
          case 1: *op++ = c;
          case 0: *op++ = c;
        }*/
      }
    }

  while(op < out+outlen)
    if(likely((c = *ip++) != e)) {
      *op++ = c;
    } else {
      vlget32(ip, r);
      int f = r&1; r >>= 1;
      if(likely(r) >= 3) {
        uint8_t c = f?ip[0]:ix; ip+=f;
        r = r-3+4;
        rmemset(op, c, r);
      } else { r++;
        rmemset8(op, e, r);
      }
    }
  return ip - in;
}
  #endif

unsigned _srled(const unsigned char *__restrict in, unsigned char *__restrict out, unsigned outlen) {
  return _srled8(in+1, out, outlen, *in);
}

unsigned srled(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen) { unsigned l;
  if(inlen == outlen)
    memcpy(out, in, outlen);                                //No compression
  else if(inlen == 1)
    memset(out, in[0], outlen);                             //Only 1 char
  else _srled8x(in+2, out, outlen, in[0], in[1]);                           //AS((ip-in) == inlen,"FatalI l>inlen %d ", (ip-in) - inlen);
  return inlen;
}
//------------------------------------- TurboRLE ------------------------------------------
unsigned _trled(const unsigned char *__restrict in, unsigned char *__restrict out, unsigned outlen) {
  uint8_t rmap[256] = {0}, *op = out, *ip = in;
  unsigned m = 0, i, c;

  if(!outlen)
    return 0;

  if(!(c = *ip++))
    return _srled8(ip+1, out, outlen, *ip)+2;

  for(i = 0; i != c; i++) { uint8_t *pb = &rmap[i<<3]; unsigned u = ip[i],v;
    v = (u >> 0) & 1; m += v; pb[0] = v?m:0;
    v = (u >> 1) & 1; m += v; pb[1] = v?m:0;
    v = (u >> 2) & 1; m += v; pb[2] = v?m:0;
    v = (u >> 3) & 1; m += v; pb[3] = v?m:0;
    v = (u >> 4) & 1; m += v; pb[4] = v?m:0;
    v = (u >> 5) & 1; m += v; pb[5] = v?m:0;
    v = (u >> 6) & 1; m += v; pb[6] = v?m:0;
    v = (u >> 7) & 1; m += v; pb[7] = v?m:0;
  }
  ip += c;
  for(i = c*8; i != 256; i++) rmap[i] = ++m;

  m--;
  unsigned char ix = *ip++;

  if(outlen >= 32)
    while(op < out+(outlen-32)) {
        #if __WORDSIZE == 64
      uint64_t z = (uint64_t)rmap[ip[7]]<<56 | (uint64_t)rmap[ip[6]] << 48 | (uint64_t)rmap[ip[5]] << 40 | (uint64_t)rmap[ip[4]] << 32 | (uint32_t)rmap[ip[3]] << 24 | (uint32_t)rmap[ip[2]] << 16| (uint32_t)rmap[ip[1]] << 8| rmap[ip[0]];
      ctou64(op) = ctou64(ip); if(z) goto a; ip += 8; op += 8;
      continue;
      a: z = ctz64(z)>>3;
        #else
      uint32_t z = (uint32_t)rmap[ip[3]] << 24 | (uint32_t)rmap[ip[2]] << 16| (uint32_t)rmap[ip[1]] << 8| rmap[ip[0]];
      ctou32(op) = ctou32(ip); if(z) goto a; ip += 4; op += 4;
      continue;
      a: z = ctz32(z)>>3;
        #endif

      ip += z; op += z;
      c = rmap[*ip++];
      vlzget(ip, i, m, c-1);
      if(i&1) c = ix; else c = *ip++; i = (i>>1) + TMIN;
      rmemset(op,c,i);                                                      PREFETCH(ip+512, 0);
    }

  while(op < out+outlen) {
    if(likely(!(c = rmap[*ip]))) *op++ = *ip++;
    else {
      ip++;
      vlzget(ip, i, m, c-1);
      if(i&1) c = ix; else c = *ip++; i = (i>>1) + TMIN;
      rmemset8(op,c,i);
    }
  }
  return ip - in;
}

unsigned trled(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen) {
  if(inlen == outlen)
    memcpy(out, in, outlen);
  else if(inlen == 1)
    memset(out, in[0], outlen);
  else {
    unsigned l = _trled(in, out, outlen);                                       //AS(l == inlen,"FatalI l>inlen %d ", l - inlen);
  }
  return inlen;
}

#undef USIZE
#undef rmemset
#undef SRLE8

//------------------------------------- RLE 16, 32, 64 --------------------------------------------------
#define USIZE 16
#include "trled.c"
#undef rmemset
#undef USIZE
#undef runcpy

#define USIZE 32
#include "trled.c"
#undef rmemset
#undef USIZE
#undef runcpy

#define USIZE 64
#include "trled.c"
#undef rmemset
#undef USIZE

 #else // ---------------------------- include 16, 32, 64----------------------------------------------
  #ifdef MEMSAFE
#define rmemset(_op_, _c_, _i_) while(_i_--) *_op_++ = _c_
  #elif (__AVX2__ != 0) && USIZE < 64
#define rmemset(_op_, _c_, _i_) do {\
  __m256i cv = TEMPLATE2(_mm256_set1_epi, USIZE)(_c_); unsigned char *_p = _op_; _op_ += _i_;\
  do _mm256_storeu_si256((__m256i *)_p, cv),_p+=32; while(_p < _op_);\
} while(0)
  #elif (__SSE__ != 0 || __ARM_NEON != 0) && USIZE < 64
#define rmemset(_op_, _c_, _i_) do { \
  __m128i *_up = (__m128i *)_op_, cv = TEMPLATE2(_mm_set1_epi, USIZE)(_c_);\
  _op_ += _i_;\
  do { _mm_storeu_si128(  _up, cv); _mm_storeu_si128(_up+1, cv); _up+=2; } while(_up < (__m128i *)_op_);\
} while(0)
  #else
#define _cset64(_cc,_c_) _cc = _c_
#define _cset32(_cc,_c_) _cc = _c_; _cc = _cc<<32|_cc
#define _cset16(_cc,_c_) _cc = _c_; _cc = _cc<<48|_cc<<32|_cc<<16|_cc
#define _cset8( _cc,_c_) _cc = (uint32_t)_c_<<24 | (uint32_t)_c_<<16 | (uint32_t)_c_<<8 | (uint32_t)_c_; _cc = _cc<<32|_cc

#define rmemset(_op_, _c_, _i_) do {  uint64_t _cc; uint8_t *_up = (uint8_t *)_op_; _op_ +=_i_;\
 TEMPLATE2(_cset, USIZE)(_cc,_c_);\
  do {\
    TEMPLATE2(ctou, USIZE)(_up) = _c_; _up += USIZE/8;\
    TEMPLATE2(ctou, USIZE)(_up) = _c_; _up += USIZE/8;\
  } while(_up < (uint8_t *)_op_);\
} while(0)
  #endif

#define uint_t TEMPLATE3(uint, USIZE, _t)
#define ctout(_x_) *(uint_t *)(_x_)
  #if !SRLE8
unsigned TEMPLATE2(_srled, USIZE)(const unsigned char *__restrict in, unsigned char *__restrict cout, unsigned outlen, uint_t e) {
  uint_t *out = (uint_t *)cout, *op = out, c;
  const unsigned char *ip = in;

    #ifdef __AVX2__
  #define _mm256_set1_epi64 _mm256_set1_epi64x
  __m256i ev = TEMPLATE2(_mm256_set1_epi, USIZE)(e);
    #elif (defined(__SSE__) /*|| defined(__ARM_NEON)*/)
       // #if USIZE != 64
  #define _mm_set1_epi64 _mm_set1_epi64x
  __m128i ev = TEMPLATE2(_mm_set1_epi, USIZE)(e);
       // #endif
    #endif

  if(outlen >= sizeof(uint_t)*8)
    while(op < out+outlen/sizeof(uint_t)-sizeof(uint_t)*8) { int r;
        #if __AVX2__ != 0 && USIZE != 64
      uint32_t mask;
      __m256i v = _mm256_loadu_si256((__m256i*)ip); _mm256_storeu_si256((__m256i *)op, v); mask = _mm256_movemask_epi8(TEMPLATE2(_mm256_cmpeq_epi,USIZE)(v, ev)); if(mask) goto a; ip += 32; op += 256/USIZE;
              v = _mm256_loadu_si256((__m256i*)ip); _mm256_storeu_si256((__m256i *)op, v); mask = _mm256_movemask_epi8(TEMPLATE2(_mm256_cmpeq_epi,USIZE)(v, ev)); if(mask) goto a; ip += 32; op += 256/USIZE;
                                                                                PREFETCH(ip+512, 0);
      continue;
      a: r = ctz32(mask)/(USIZE/8);
      op += r;
      ip += (r+1)*sizeof(uint_t);                                               PREFETCH(ip+512, 0);
        #elif (__SSE__ != 0 /*|| __ARM_NEON != 0*/) && USIZE != 64
      uint32_t mask;
    __m128i v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(TEMPLATE2(_mm_cmpeq_epi,USIZE)(v, ev)); if(mask) goto a; ip += 16; op += 128/USIZE;
            v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(TEMPLATE2(_mm_cmpeq_epi,USIZE)(v, ev)); if(mask) goto a; ip += 16; op += 128/USIZE;
            v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(TEMPLATE2(_mm_cmpeq_epi,USIZE)(v, ev)); if(mask) goto a; ip += 16; op += 128/USIZE;
            v = _mm_loadu_si128((__m128i*)ip); _mm_storeu_si128((__m128i *)op, v); mask = _mm_movemask_epi8(TEMPLATE2(_mm_cmpeq_epi,USIZE)(v, ev)); if(mask) goto a; ip += 16; op += 128/USIZE;
                                                                                PREFETCH(ip+512, 0);
      continue;
      a: r = ctz32(mask)/(USIZE/8);
      op += r;
      ip += (r+1)*sizeof(uint_t);                                               PREFETCH(ip+512, 0);
       #else
    if(((c = ctout(ip)) == e)) goto a; ip += sizeof(uint_t); *op++ = c;
    if(((c = ctout(ip)) == e)) goto a; ip += sizeof(uint_t); *op++ = c;
    if(((c = ctout(ip)) == e)) goto a; ip += sizeof(uint_t); *op++ = c;
    if(((c = ctout(ip)) == e)) goto a; ip += sizeof(uint_t); *op++ = c;
    if(((c = ctout(ip)) == e)) goto a; ip += sizeof(uint_t); *op++ = c;
    if(((c = ctout(ip)) == e)) goto a; ip += sizeof(uint_t); *op++ = c;
    if(((c = ctout(ip)) == e)) goto a; ip += sizeof(uint_t); *op++ = c;
    if(((c = ctout(ip)) == e)) goto a; ip += sizeof(uint_t); *op++ = c;          PREFETCH(ip +512, 0);
    continue;
    a: ip += sizeof(uint_t);                                                    PREFETCH(ip +512, 0);
       #endif
      vlget32(ip, r);
      if(likely(r) >= 3) {
        c   = ctout(ip); ip += sizeof(uint_t);
        r = r-3+4;
        rmemset(op, c, r);
      } else { r++;
        rmemset(op, e, r);
      }
  }

  while(op < out+outlen/sizeof(uint_t)) {
    c = ctout(ip); ip += sizeof(uint_t);
    if(likely(c != e)) {
      *op++ = c;
    } else {
      int r;
      vlget32(ip, r);
      if(likely(r) >= 3) {
        c   = ctout(ip); ip += sizeof(uint_t);
        r = r-3+4;
        rmemset(op, c, r);
      } else {
        r++;
        rmemset(op, e, r);
      }
    }
  }
  { unsigned char *p = (unsigned char *)op;
    while(p < cout+outlen) *p++ = *ip++;
  }
  return ip - in;
}
  #endif

unsigned TEMPLATE2(srled, USIZE)(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint_t e) {
  if(inlen == outlen)
    memcpy(out, in, outlen);
  else if(inlen == 1)
    memset(out, in[0], outlen);
  else
    return TEMPLATE2(_srled, USIZE)(in, out, outlen, e);
  return inlen;
}
 #endif

