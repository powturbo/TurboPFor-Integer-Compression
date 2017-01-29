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
//     "Integer Compression"
#include <stdint.h>

#define BITFORSET_(_out_, _n_, _start_, _inc_) do { unsigned _i;\
  for(_i = 0; _i != (_n_&~3); _i+=4) {\
    _out_[_i+0] = _start_+(_i  )*_inc_;\
    _out_[_i+1] = _start_+(_i+1)*_inc_;\
    _out_[_i+2] = _start_+(_i+2)*_inc_;\
    _out_[_i+3] = _start_+(_i+3)*_inc_;\
  }\
  while(_i != _n_)\
    _out_[_i] = _start_+_i*_inc_, ++_i;\
} while(0)

#define BITSIZE_(_in_, _n_, _b_, _usize_) { typeof(_in_[0]) *_ip;\
  for(_b_=0,_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip+=4)\
    _b_ |= _ip[0] | _ip[1] | _ip[2] | _ip[3];\
  while(_ip != _in_+_n_) \
    _b_ |= *_ip++;\
  _b_ = TEMPLATE2(bsr, _usize_)(_b_);\
}

#define BITSIZE8( _in_, _n_, _b_) BITSIZE_(_in_, _n_, _b_,  8)
#define BITSIZE16(_in_, _n_, _b_) BITSIZE_(_in_, _n_, _b_, 16)
#define BITSIZE64(_in_, _n_, _b_) BITSIZE_(_in_, _n_, _b_, 64)

static inline unsigned char  zigzagenc8( char           x) { return x << 1 ^   x >> 7; }
static inline unsigned char  zigzagdec8( unsigned short x) { return x >> 1 ^ -(x &   1); }

static inline unsigned short zigzagenc16(short          x) { return x << 1 ^   x >> 15; }
static inline unsigned short zigzagdec16(unsigned short x) { return x >> 1 ^ -(x &   1); }

static inline unsigned       zigzagenc31(int      x)       { x = (x << 2 | ((x>>30)&  2)) ^   x >> 31; return x; } // for signed x
static inline unsigned       zigzagdec31(unsigned x)       { return (x >> 2 |  (x&  2)<<30 ) ^ -(x &   1); }

static inline unsigned       zigzagenc32(int      x)       { return x << 1 ^   x >> 31; }
static inline unsigned       zigzagdec32(unsigned x)       { return x >> 1 ^ -(x &   1); }

static inline uint64_t       zigzagenc64(int64_t  x)       { return x << 1 ^ x >> 63; }
static inline uint64_t       zigzagdec64(uint64_t x)       { return x >> 1 ^ -(x & 1); }

  #ifdef __AVX2__
#include <emmintrin.h>
#include <stdio.h>
//#define DELTA256x32(_v_, _sv_,_iv_) ?

#define SCAN256x32( _v_, _sv_) {\
  _v_  = _mm256_add_epi32(_v_, _mm256_slli_si256(_v_, 4));\
  _v_  = _mm256_add_epi32(_v_, _mm256_slli_si256(_v_, 8));\
  _sv_ = _mm256_add_epi32(     _mm256_permute2x128_si256(   _mm256_shuffle_epi32(_sv_,_MM_SHUFFLE(3, 3, 3, 3)), _sv_, 0x11), \
         _mm256_add_epi32(_v_, _mm256_permute2x128_si256(zv,_mm256_shuffle_epi32(_v_, _MM_SHUFFLE(3, 3, 3, 3)),       0x20)));\
}

#define SCANI256x32(_v_, _sv_, _vi_) SCAN256x32(_v_, _sv_); _sv_ = _mm256_add_epi32(_sv_, _vi_)

#define   ZIGZAG256x32(_v_) _mm256_xor_si256(_mm256_slli_epi32(_v_,1), _mm256_srai_epi32(_v_,31))
#define UNZIGZAG256x32(_v_) _mm256_xor_si256(_mm256_srli_epi32(_v_,1), _mm256_srai_epi32(_mm256_slli_epi32(_v_,31),31) )

#define HOR256x32(_v_,_b_) _v_ = _mm256_or_si256(_v_, _mm256_srli_si256(_v_, 8)); _v_ = _mm256_or_s256(_v_, _mm256_srli_si256(_v_, 4));\
   _b_ = _mm256_extract_epi32(_v_,0) | _mm256_extract_epi32(_v_, 4)
  #endif 

  #ifdef __SSE2__
#include <emmintrin.h>

#define DELTA128x32(_v_, _sv_) _mm_sub_epi32(_v_, _mm_or_si128(_mm_srli_si128(_sv_, 12), _mm_slli_si128(_v_, 4)))
// SIMD Scan ( prefix sum ) 
#define SCAN128x32( _v_, _sv_) _v_ = _mm_add_epi32(_v_, _mm_slli_si128(_v_, 4)); _sv_ = _mm_add_epi32(_mm_shuffle_epi32(_sv_, _MM_SHUFFLE(3, 3, 3, 3)), _mm_add_epi32(_mm_slli_si128(_v_, 8), _v_) )
#define SCANI128x32(_v_, _sv_, _vi_) SCAN128x32(_v_, _sv_); _sv_ = _mm_add_epi32(_sv_, _vi_)

#define   ZIGZAG128x32(_v_) _mm_xor_si128(_mm_slli_epi32(_v_,1), _mm_srai_epi32(_v_,31))
#define UNZIGZAG128x32(_v_) _mm_xor_si128(_mm_srli_epi32(_v_,1), _mm_srai_epi32(_mm_slli_epi32(_v_,31),31) ) //_mm_sub_epi32(cz, _mm_and_si128(iv,c1))
// SIMD Horizontal OR
#define HOR128x32(_v_,_b_) _v_ = _mm_or_si128(_v_, _mm_srli_si128(_v_, 8)); _v_ = _mm_or_si128(_v_, _mm_srli_si128(_v_, 4)); _b_ = (unsigned)_mm_cvtsi128_si32(_v_)
  #endif 

  #if 0 //def __AVX2__
#define BITSIZE32(_in_, _n_, _b_) { typeof(_in_[0]) *_ip; __m256i _v = _mm256_setzero_si256();\
  for(_ip = _in_; _ip != _in_+(_n_&~(8-1)); _ip+=8)\
    _v = _mm256_or_si256(_v, _mm256_loadu_si256((__m256i*)_ip));\
  HOR256x32(_v,_b_);\
  while(_ip != _in_+_n_)\
    _b_ |= *_ip++;\
  _b_ = bsr32(_b_);\
}
#define BITZERO32(_out_, _n_, _start_) do {\
  __m256i _sv_ = _mm256_set1_epi32(_start_), *_ov = (__m256i *)(_out_), *_ove = (__m256i *)(_out_ + _n_);\
  do _mm256_storeu_si256(_ov++, _sv_); while(_ov < _ove);\
} while(0)

#define BITFORZERO32(_out_, _n_, _start_, _inc_) do {\
  __m256i _sv = _mm256_set1_epi32(_start_), *_ov=(__m256i *)(_out_), *_ove = (__m256i *)(_out_ + _n_), _cv = _mm256_set_epi32(7+_inc_,6+_inc_,5+_inc_,4+_inc_,3*_inc_,2*_inc_,1*_inc_,0); \
    _sv = _mm256_add_epi32(_sv, _cv);\
    _cv = _mm256_set1_epi32(4);\
  do { _mm256_storeu_si256(_ov++, _sv); _sv = _mm256_add_epi32(_sv, _cv); } while(_ov < _ove);\
} while(0)

#define BITDIZERO32(_out_, _n_, _start_, _inc_) do { __m256i _sv = _mm256_set1_epi32(_start_), _cv = _mm256_set_epi32(7+_inc_,6+_inc_,5+_inc_,4+_inc_,3+_inc_,2+_inc_,1+_inc_,_inc_), *_ov=(__m256i *)(_out_), *_ove = (__m256i *)(_out_ + _n_);\
  _sv = _mm256_add_epi32(_sv, _cv); _cv = _mm256_set1_epi32(4*_inc_); do { _mm256_storeu_si256(_ov++, _sv), _sv = _mm256_add_epi32(_sv, _cv); } while(_ov < _ove);\
} while(0)

  #elif defined(__SSE2__) 
#define BITSIZE32(_in_, _n_, _b_) { typeof(_in_[0]) *_ip; __m128i _v = _mm_setzero_si128();\
  for(_ip = _in_; _ip != _in_+(_n_&~(4-1)); _ip+=4)\
    _v = _mm_or_si128(_v, _mm_loadu_si128((__m128i*)_ip));\
  HOR128x32(_v,_b_);\
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
#define BITSIZE32(   _in_,  _n_, _b_)            BITSIZE_(_in_, _n_, _b_, 32)
#define BITFORZERO32(_out_, _n_, _start_, _inc_) BITFORSET_(_out_, _n_, _start_, _inc_)
#define BITZERO32(   _out_, _n_, _start_)        BITFORSET_(_out_, _n_, _start_, 0)
  #endif

#define DELTR( _in_, _n_, _start_, _inc_,      _out_) { unsigned _v; for(      _v = 0; _v < _n_; _v++) _out_[_v] = _in_[_v] - (_start_) - _v*(_inc_) - (_inc_); }
#define DELTRB(_in_, _n_, _start_, _inc_, _b_, _out_) { unsigned _v; for(_b_=0,_v = 0; _v < _n_; _v++) _out_[_v] = _in_[_v] - (_start_) - _v*(_inc_) - (_inc_), _b_ |= _out_[_v]; _b_ = bsr32(_b_); }

#ifdef __cplusplus
extern "C" {
#endif

//------------- get maximum bit length of the elements in the integer array -----------------------
unsigned bit8(      uint8_t  *in, unsigned n);  
unsigned bit16(     uint16_t *in, unsigned n);  
unsigned bit32(     uint32_t *in, unsigned n);  
unsigned bit64(     uint64_t *in, unsigned n);  

//------------- Delta for sorted integer array ----------------------------------------------------
//-- get delta maximum bit length of the non decreasing integer array. out[i] = in[i] - in[i-1]
unsigned bitd8(     uint8_t  *in, unsigned n, uint8_t start);  
unsigned bitd16(    uint16_t *in, unsigned n, uint16_t start);  
unsigned bitd32(    uint32_t *in, unsigned n, uint32_t start);  
unsigned bitd64(    uint64_t *in, unsigned n, uint64_t start);  

//-- get delta maximum bit length of the non strictly decreasing integer array. out[i] = in[i] - in[i-1] - 1
unsigned bitd18(    uint8_t  *in, unsigned n, uint8_t  start);
unsigned bitd116(   uint16_t *in, unsigned n, uint16_t start);
unsigned bitd132(   uint32_t *in, unsigned n, uint32_t start);
unsigned bitd164(   uint64_t *in, unsigned n, uint64_t start);

//-- transform sorted integer array to delta array. inc = increment: out[i] = in[i] - in[i-1] - inc
unsigned bitdelta8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, unsigned inc);
unsigned bitdelta16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, unsigned inc);
unsigned bitdelta32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, unsigned inc);
unsigned bitdelta64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, unsigned inc);

//-- in-place reverse delta transform 
void bitund8(   	 uint8_t  *p, unsigned n, uint8_t start); // non decreasing 
void bitund16(  	 uint16_t *p, unsigned n, uint16_t start);
void bitund32(  	 uint32_t *p, unsigned n, uint32_t start);
void bitund64(       uint64_t *p, unsigned n, uint64_t start);

void bitund18(       uint8_t  *p, unsigned n, uint8_t  start); // non strictly decreasing
void bitund116(      uint16_t *p, unsigned n, uint16_t start);
void bitund132(      uint32_t *p, unsigned n, uint32_t start);
void bitund164(      uint64_t *p, unsigned n, uint64_t start);

void bitundn8( 		 uint8_t  *p, unsigned n, uint8_t  start, uint8_t  inc); // increment
void bitundn16(		 uint16_t *p, unsigned n, uint16_t start, uint16_t inc);
void bitundn32(		 uint32_t *p, unsigned n, uint32_t start, uint32_t inc);
void bitundn64(		 uint64_t *p, unsigned n, uint64_t start, uint64_t inc);

//------------- FOR array bit length: out[i] = in[i] - start -------------------------------------
unsigned bitf32(     uint32_t *in, unsigned n, uint32_t start);  // sorted
unsigned bitf132(    uint32_t *in, unsigned n, uint32_t start);

unsigned bitfm8(     uint8_t  *in, unsigned n, uint8_t  *pmin);  // unsorted
unsigned bitfm16(    uint16_t *in, unsigned n, uint16_t *pmin);  
unsigned bitfm32(    uint32_t *in, unsigned n, uint32_t *pmin);  
unsigned bitfm64(    uint64_t *in, unsigned n, uint64_t *pmin);  

//------------- Zigzag encoding for unsorted integer lists: out[i] = in[i] - in[i-1] -------------

//-- get maximum zigzag bit length integer array
unsigned bitz8(      uint8_t  *in, unsigned n, uint8_t  start);
unsigned bitz16(     uint16_t *in, unsigned n, uint16_t start);
unsigned bitz32(     uint32_t *in, unsigned n, uint32_t start);
unsigned bitz64(     uint64_t *in, unsigned n, uint64_t start);

//-- Zigzag transform
unsigned bitzigzag8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start);
unsigned bitzigzag16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start);
unsigned bitzigzag32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start);
unsigned bitzigzag64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start);

//-- Zigzag reverse transform
void bitunzigzag8(   uint8_t  *p,  unsigned n, uint8_t  start);
void bitunzigzag16(  uint16_t *p,  unsigned n, uint16_t start);
void bitunzigzag32(  uint32_t *p,  unsigned n, uint32_t start);
void bitunzigzag64(  uint64_t *p,  unsigned n, uint64_t start);

//---- Floating point to Integer de-/composition ---------------------------------
#define FMANT_BITS    16
#define DMANT_BITS    32
#define DZMANT_BITS   36


#define FLTEXPO(_u_,_mantbits_, _one_)  ( ((_u_) >> _mantbits_) & ( (_one_<<(sizeof(_u_)*8 - _mantbits_)) - 1 ) )
#define FLTMANT(_u_,_mantbits_, _one_)    ((_u_) & ((_one_<<_mantbits_)-1)) 

#define BITUNFLOAT(_expo_, _mant_, _u_, _mantbits_) _u_ = ((_expo_) << _mantbits_) | (_mant_)//>>1 | (_mant_)<<(sizeof(_u_)*8 - 1)

/*#define BITFLOAT(_u_, _sgn_, _expo_, _mant_,      _mantbits_, _one_) _sgn_ = _u_ >> (sizeof(_u_)*8-1); _expo_ = EXPO(_u_,_mantbits_; _mant_ = _u_ & ((_one_<<_mantbits_)-1)
#define BITUNFLOAT(   _sgn_, _expo_, _mant_, _u_, _mantbits_)        _u_ = (_sgn_) << (sizeof(_u_)*8-1) | (_expo_) << _mantbits_ | (_mant_) */

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
