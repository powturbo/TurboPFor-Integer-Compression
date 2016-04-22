/**
    Copyright (C) powturbo 2013-2016
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
//    bitutil.h - "Integer Compression" 
#include <stdint.h>

#define _BITFORZERO(_out_, _n_, _start_, _inc_) do { unsigned _i;\
  for(_i = 0; _i != (_n_&~3); ) {\
    _out_[_i] = _start_+_i*_inc_; _i++;\
    _out_[_i] = _start_+_i*_inc_; _i++;\
    _out_[_i] = _start_+_i*_inc_; _i++;\
    _out_[_i] = _start_+_i*_inc_; _i++;\
  }\
  while(_i != _n_)\
    _out_[_i] = _start_+_i*_inc_, ++_i;\
} while(0)

#define BITSIZE(_in_, _n_, _b_, _usize_) { typeof(_in_[0]) *_ip;\
  for(_b_=0,_ip = _in_; _ip != _in_+(_n_&~(4-1)); )\
    _b_ |= *_ip++ | *_ip++ | *_ip++ | *_ip++;\
  while(_ip != _in_+_n_) \
    _b_ |= *_ip++;\
  _b_ = TEMPLATE(bsr, _usize_)(_b_);\
}

static inline uint64_t       zigzagenc64(int64_t  x) { return x << 1 ^ x >> 63; }
static inline uint64_t       zigzagdec64(uint64_t x) { return x >> 1 ^ -(x & 1); }

static inline unsigned       zigzagenc32(int      x) { return x << 1 ^   x >> 31; }
static inline unsigned       zigzagdec32(unsigned x) { return x >> 1 ^ -(x &   1); }

static inline unsigned       zigzagenc31(int      x) { x = (x << 2 | ((x>>30)&  2)) ^   x >> 31; return x; }
static inline unsigned       zigzagdec31(unsigned x) { return (x >> 2 |  (x&  2)<<30 ) ^ -(x &   1); }

static inline unsigned short zigzagenc16(short          x) { return x << 1 ^   x >> 31; }
static inline unsigned short zigzagdec16(unsigned short x) { return x >> 1 ^ -(x &   1); }

static inline unsigned char zigzagenc8(char          x) { return x << 1 ^   x >> 31; }
static inline unsigned char zigzagdec8(unsigned short x) { return x >> 1 ^ -(x &   1); }

  #ifdef __SSE2__
#include <emmintrin.h>
// SIMD Delta
#define DELTA128_32(_v_, _sv_) _mm_sub_epi32(_v_, _mm_or_si128(_mm_srli_si128(_sv_, 12), _mm_slli_si128(_v_, 4)))

// SIMD Scan ( prefix sum ) 
#define SCAN128_32( _v_, _sv_) _v_ = _mm_add_epi32(_v_, _mm_slli_si128(_v_, 4)); _sv_ = _mm_add_epi32(_mm_shuffle_epi32(_sv_, _MM_SHUFFLE(3, 3, 3, 3)), _mm_add_epi32(_mm_slli_si128(_v_, 8), _v_) )
#define SCANI128_32(_v_, _sv_, _vi_) SCAN128_32(_v_, _sv_); _sv_ = _mm_add_epi32(_sv_, _vi_)

// SIMD ZigZag
#define   ZIGZAG128_32(_v_) _mm_xor_si128(_mm_slli_epi32(_v_,1), _mm_srai_epi32(_v_,31))
#define UNZIGZAG128_32(_v_) _mm_xor_si128(_mm_srli_epi32(_v_,1), _mm_srai_epi32(_mm_slli_epi32(_v_,31),31) ) //_mm_sub_epi32(cz, _mm_and_si128(iv,c1))

// SIMD Horizontal OR
#define HOR128_32(_v_,_b_) _v_ = _mm_or_si128(_v_, _mm_srli_si128(_v_, 8)); _v_ = _mm_or_si128(_v_, _mm_srli_si128(_v_, 4)); _b_ = (unsigned)_mm_cvtsi128_si32(_v_)

#define BITSIZE32(_in_, _n_, _b_) { typeof(_in_[0]) *_ip; __m128i _v = _mm_setzero_si128();\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip+=4)\
    _v = _mm_or_si128(_v, _mm_loadu_si128((__m128i*)_ip));\
  HOR128_32(_v,_b_);\
  while(_ip != _in_+_n_)\
    _b_ |= *_ip++;\
  _b_ = bsr32(_b_);\
}
// SIMD set value
#define BITZERO32(_out_, _n_, _start_) do {\
  __m128i _sv_ = _mm_set1_epi32(_start_), *_ov = (__m128i *)(_out_), *_ove = (__m128i *)(_out_ + _n_);\
  do _mm_storeu_si128(_ov++, _sv_); while(_ov < _ove); \
} while(0)

#define BITFORZERO32(_out_, _n_, _start_, _inc_) do {\
  __m128i _sv = _mm_set1_epi32(_start_), *_ov=(__m128i *)(_out_), *_ove = (__m128i *)(_out_ + _n_), _cv = _mm_set_epi32(3*_inc_,2*_inc_,1*_inc_,0); \
    _sv = _mm_add_epi32(_sv, _cv);\
    _cv = _mm_set1_epi32(4);\
  do { _mm_storeu_si128(_ov++, _sv); _sv = _mm_add_epi32(_sv, _cv); } while(_ov < _ove);\
} while(0)

#define BITDIZERO32(_out_, _n_, _start_, _inc_) do { __m128i _sv = _mm_set1_epi32(_start_), _cv = _mm_set_epi32(3+_inc_,2+_inc_,1+_inc_,_inc_), *_ov=(__m128i *)(_out_), *_ove = (__m128i *)(_out_ + _n_);\
  _sv = _mm_add_epi32(_sv, _cv); _cv = _mm_set1_epi32(4*_inc_); do { _mm_storeu_si128(_ov++, _sv), _sv = _mm_add_epi32(_sv, _cv); } while(_ov < _ove);\
} while(0)

  #else																					
#define BITSIZE32(_in_, _n_, _b_)        BITSIZE(_in_, _n_, _b_, 32)
#define BITFORZERO32(_out_, _n_, _start_, _inc_) _BITFORZERO(_out_, _n_, _start_, _inc_)
#define BITZERO32(_out_, _n_, _start_)         _BITFORZERO(_out_, _n_, _start_, 0)
  #endif

#define DELTR( _in_, _n_, _mode_,      _out_) { unsigned _v; for(      _out_[0]=_in_[0],_v = 1;     _v < _n_; _v++) _out_[_v] = (_in_[_v] - _out_[0]) -   _v*_mode_; }
#define DELTRB(_in_, _n_, _mode_, _b_, _out_) { unsigned _v; for(_b_=0,_out_[0]=_in_[0],_v = 1;     _v < _n_; _v++) _out_[_v] = (_in_[_v] - _out_[0]) -   _v*_mode_, _b_ |= _out_[_v]; _b_ = bsr32(_b_); }

#ifdef __cplusplus
extern "C" {
#endif

//------------- get maximum bit length of the elements in the integer array -----------------------
unsigned bit32(     unsigned *in, unsigned n);  

//------------- Delta for sorted integer array ----------------------------------------------------
//-- transform sorted integer array to delta array. inc = increment: out[i] = in[i] - in[i-1] - inc
unsigned bitdelta32(unsigned *in, unsigned n, unsigned *out, unsigned start, unsigned inc);
unsigned bitdelta64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, unsigned inc);

//-- get delta maximum bit length of the non decreasing integer array. out[i] = in[i] - in[i-1]
unsigned bitd32(    unsigned *in, unsigned n, unsigned start);  

//-- get delta maximum bit length of the non strictly decreasing integer array. out[i] = in[i] - in[i-1] - 1
unsigned bitd132(   unsigned *in, unsigned n, unsigned start);

//-- in-place reverse delta transform 
void bitund32(      unsigned *p, unsigned n, unsigned x);
void bitund64(      uint64_t *p, unsigned n, uint64_t x);

void bitundx32(     unsigned *p, unsigned n, unsigned x, unsigned inc);
void bitundx64(     uint64_t *p, unsigned n, uint64_t x, unsigned inc);

void bitund132(     unsigned *p, unsigned n, unsigned x);

//------------- FOR array bit length: out[i] = in[i] - start -------------------------------------

unsigned bitf32(    unsigned *in, unsigned n, unsigned start);  // sorted
unsigned bitf132(   unsigned *in, unsigned n, unsigned start);
unsigned bitfm32(   unsigned *in, unsigned n, unsigned *pmin);  // unsorted
unsigned bitf1m32(  unsigned *in, unsigned n, unsigned *pmin);

//------------- Zigzag encoding for unsorted integer lists: out[i] = in[i] - in[i-1] -------------

//-- get maximum zigzag bit length integer array
unsigned bitz32(     unsigned *in, unsigned n, unsigned start);

//-- Zigzag transform
unsigned bitzigzag32(unsigned *in, unsigned n, unsigned *out, unsigned start);
unsigned bitzigzag64(uint64_t *in, unsigned n, uint64_t *out, unsigned start);

//-- Zigzag reverse transform
void bitunzigzag32(  unsigned *p,  unsigned n, unsigned start);
void bitunzigzag64(  uint64_t *p,  unsigned n, unsigned start);

//---- Floating point to Integer de-/composition ---------------------------------
#define FMANT_BITS    16
#define DMANT_BITS    32
#define DZMANT_BITS   36


#define FLTEXPO(__u,__mantbits, __one)  ( ((__u) >> __mantbits) & ( (__one<<(sizeof(__u)*8 - __mantbits)) - 1 ) )
#define FLTMANT(__u,__mantbits, __one)    ((__u) & ((__one<<__mantbits)-1)) 

#define BITUNFLOAT(__expo, __mant, __u, __mantbits) __u = ((__expo) << __mantbits) | (__mant)//>>1 | (__mant)<<(sizeof(__u)*8 - 1)

/*#define BITFLOAT(__u, __sgn, __expo, __mant,      __mantbits, __one) __sgn = __u >> (sizeof(__u)*8-1); __expo = EXPO(__u,__mantbits; __mant = __u & ((__one<<__mantbits)-1)
#define BITUNFLOAT(   __sgn, __expo, __mant, __u, __mantbits)        __u = (__sgn) << (sizeof(__u)*8-1) | (__expo) << __mantbits | (__mant) */

// De-/Compose floating point array to/from integer arrays (sign,exponent,mantissa) for using with "Integer Compression" functions ------------
void bitdouble(  double *in, unsigned n, int *expo, uint64_t *mant);
void bitundouble(                        int *expo, uint64_t *mant, unsigned n, double *out);
void bitzdouble( double *in, unsigned n, int *expo, uint64_t *mant);
void bitzundouble(                       int *expo, uint64_t *mant, unsigned n, double *out);
void bitfloat(   float *in,  unsigned n, int *expo, unsigned *mant);
void bitunfloat(                         int *expo, unsigned *mant, unsigned n, float *out);

#ifdef __cplusplus
}
#endif
