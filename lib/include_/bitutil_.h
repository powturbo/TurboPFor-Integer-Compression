/**
    Copyright (C) powturbo 2013-2023
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
//     "Integer Compression: max.bits, delta, zigzag, xor"
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
  #endif
  #if defined(_MSC_VER) && _MSC_VER < 1600
#include "vs/stdint.h"
  #else
#include <stdint.h>
  #endif
#include "../include_/sse_neon.h"

  #ifdef __ARM_NEON
#define PREFETCH(_ip_,_rw_)
  #else
#define PREFETCH(_ip_,_rw_) //__builtin_prefetch(_ip_,_rw_)
  #endif
  
//------------------------ zigzag encoding ----------------------------------------------
static inline unsigned char  zigzagenc8( signed char    x) { return x << 1 ^   x >> 7;  }
static inline          char  zigzagdec8( unsigned char  x) { return x >> 1 ^ -(x &  1); }

static inline unsigned short zigzagenc16(short          x) { return x << 1 ^   x >> 15;  }
static inline          short zigzagdec16(unsigned short x) { return x >> 1 ^ -(x &   1); }

static inline unsigned       zigzagenc32(int      x)       { return x << 1 ^   x >> 31;  }
static inline int            zigzagdec32(unsigned x)       { return x >> 1 ^ -(x &   1); }

static inline uint64_t       zigzagenc64(int64_t  x)       { return x << 1 ^ x >> 63;  }
static inline  int64_t       zigzagdec64(uint64_t x)       { return x >> 1 ^ -(x & 1); }

  #ifdef __AVX2__
#define mm256_srai_epi64_63(v, s) _mm256_srai_epi32(_mm256_shuffle_epi32(v, _MM_SHUFFLE(3, 3, 1, 1)), 31)
static ALWAYS_INLINE __m256i mm256_zzage_epi32(__m256i v) { return _mm256_xor_si256(_mm256_slli_epi32(v,1), _mm256_srai_epi32(   v,31)); }
static ALWAYS_INLINE __m256i mm256_zzage_epi64(__m256i v) { return _mm256_xor_si256(_mm256_slli_epi64(v,1),  mm256_srai_epi64_63(v,63)); }
static ALWAYS_INLINE __m256i mm256_zzagd_epi32(__m256i v) { return _mm256_xor_si256(_mm256_srli_epi32(v,1), _mm256_srai_epi32(   _mm256_slli_epi32(v,31),31) ); }
static ALWAYS_INLINE __m256i mm256_zzagd_epi64(__m256i v) { return _mm256_xor_si256(_mm256_srli_epi64(v,1),  mm256_srai_epi64_63(_mm256_slli_epi64(v,63),63) ); }

//-- AVX2 delta <-> prefix sum (scan) / xor encode <-> xor decode ---------------------------------------------------------------------------------------
static ALWAYS_INLINE __m256i mm256_delta_epi32(__m256i v, __m256i sv) { return _mm256_sub_epi32(v, _mm256_alignr_epi8(v, _mm256_permute2f128_si256(sv, v, _MM_SHUFFLE(0, 2, 0, 1)), 12)); }
static ALWAYS_INLINE __m256i mm256_delta_epi64(__m256i v, __m256i sv) { return _mm256_sub_epi64(v, _mm256_alignr_epi8(v, _mm256_permute2f128_si256(sv, v, _MM_SHUFFLE(0, 2, 0, 1)),  8)); }

static ALWAYS_INLINE __m256i mm256_xore_epi32( __m256i v, __m256i sv) { return _mm256_xor_si256(v, _mm256_alignr_epi8(v, _mm256_permute2f128_si256(sv, v, _MM_SHUFFLE(0, 2, 0, 1)), 12)); }
static ALWAYS_INLINE __m256i mm256_xore_epi64( __m256i v, __m256i sv) { return _mm256_xor_si256(v, _mm256_alignr_epi8(v, _mm256_permute2f128_si256(sv, v, _MM_SHUFFLE(0, 2, 0, 1)),  8)); }

#define MM256_HDEC_EPI32(_v_,_sv_,_ho_) {\
  _v_  = _ho_(_v_, _mm256_slli_si256(_v_, 4));\
  _v_  = _ho_(_v_, _mm256_slli_si256(_v_, 8));\
  return _ho_(       _mm256_permute2x128_si256(                       _mm256_shuffle_epi32(_sv_,_MM_SHUFFLE(3, 3, 3, 3)), _sv_, 0x11),\
           _ho_(_v_, _mm256_permute2x128_si256(_mm256_setzero_si256(),_mm256_shuffle_epi32(_v_, _MM_SHUFFLE(3, 3, 3, 3)),       0x20)));\
}
static ALWAYS_INLINE __m256i mm256_scan_epi32(__m256i v, __m256i sv) { MM256_HDEC_EPI32(v,sv,_mm256_add_epi32); }
static ALWAYS_INLINE __m256i mm256_xord_epi32(__m256i v, __m256i sv) { MM256_HDEC_EPI32(v,sv,_mm256_xor_si256); }

#define MM256_HDEC_EPI64(_v_,_sv_,_ho_) {\
  _v_ = _ho_(_v_, _mm256_alignr_epi8(_v_, _mm256_permute2x128_si256(_v_, _v_, _MM_SHUFFLE(0, 0, 2, 0)), 8));\
  return _ho_(_mm256_permute4x64_epi64(_sv_, _MM_SHUFFLE(3, 3, 3, 3)), _ho_(_mm256_permute2x128_si256(_v_, _v_, _MM_SHUFFLE(0, 0, 2, 0)), _v_) );\
}
static ALWAYS_INLINE __m256i mm256_scan_epi64(__m256i v, __m256i sv) { MM256_HDEC_EPI64(v,sv,_mm256_add_epi64); }
static ALWAYS_INLINE __m256i mm256_xord_epi64(__m256i v, __m256i sv) { MM256_HDEC_EPI64(v,sv,_mm256_xor_si256); }

static ALWAYS_INLINE __m256i mm256_scani_epi32(__m256i v, __m256i sv, __m256i vi) { return _mm256_add_epi32(mm256_scan_epi32(v, sv), vi); }

//-- Horizontal OR ---------------------------------------
static ALWAYS_INLINE unsigned mm256_hor_epi32(__m256i v) {
  v = _mm256_or_si256(v, _mm256_srli_si256(v, 8));
  v = _mm256_or_si256(v, _mm256_srli_si256(v, 4));
  return _mm256_extract_epi32(v,0) | _mm256_extract_epi32(v, 4);
}

static ALWAYS_INLINE uint64_t mm256_hor_epi64(__m256i v) {
  v = _mm256_or_si256(v, _mm256_permute2x128_si256(v, v, _MM_SHUFFLE(2, 0, 0, 1)));
  return _mm256_extract_epi64(v, 1) | _mm256_extract_epi64(v,0);
}
  #endif
 
  #if defined(__SSSE3__) || defined(__ARM_NEON)
#define mm_srai_epi64_63(_v_, _s_) _mm_srai_epi32(_mm_shuffle_epi32(_v_, _MM_SHUFFLE(3, 3, 1, 1)), 31)
  
static ALWAYS_INLINE __m128i mm_zzage_epi16(__m128i v) { return _mm_xor_si128( mm_slli_epi16(v,1),  mm_srai_epi16(   v,15)); }
static ALWAYS_INLINE __m128i mm_zzage_epi32(__m128i v) { return _mm_xor_si128( mm_slli_epi32(v,1),  mm_srai_epi32(   v,31)); }
static ALWAYS_INLINE __m128i mm_zzage_epi64(__m128i v) { return _mm_xor_si128( mm_slli_epi64(v,1),  mm_srai_epi64_63(v,63)); }

static ALWAYS_INLINE __m128i mm_zzagd_epi16(__m128i v) { return _mm_xor_si128( mm_srli_epi16(v,1),  mm_srai_epi16(    mm_slli_epi16(v,15),15)); }
static ALWAYS_INLINE __m128i mm_zzagd_epi32(__m128i v) { return _mm_xor_si128( mm_srli_epi32(v,1),  mm_srai_epi32(    mm_slli_epi32(v,31),31)); }
static ALWAYS_INLINE __m128i mm_zzagd_epi64(__m128i v) { return _mm_xor_si128( mm_srli_epi64(v,1),  mm_srai_epi64_63( mm_slli_epi64(v,63),63)); }

static ALWAYS_INLINE __m128i mm_delta_epi16(__m128i v, __m128i sv) { return _mm_sub_epi16(v, _mm_alignr_epi8(v, sv, 14)); }
static ALWAYS_INLINE __m128i mm_delta_epi32(__m128i v, __m128i sv) { return _mm_sub_epi32(v, _mm_alignr_epi8(v, sv, 12)); }
static ALWAYS_INLINE __m128i mm_delta_epi64(__m128i v, __m128i sv) { return _mm_sub_epi64(v, _mm_alignr_epi8(v, sv,  8)); }

static ALWAYS_INLINE __m128i mm_xore_epi16( __m128i v, __m128i sv) { return _mm_xor_si128(v, _mm_alignr_epi8(v, sv, 14)); }
static ALWAYS_INLINE __m128i mm_xore_epi32( __m128i v, __m128i sv) { return _mm_xor_si128(v, _mm_alignr_epi8(v, sv, 12)); }
static ALWAYS_INLINE __m128i mm_xore_epi64( __m128i v, __m128i sv) { return _mm_xor_si128(v, _mm_alignr_epi8(v, sv,  8)); }

#define MM_HDEC_EPI32(_v_,_sv_,_ho_) { \
  _v_ = _ho_(_v_, _mm_slli_si128(_v_, 4)); \
  _v_ = _ho_(mm_shuffle_nnnn_epi32(_sv_, 3), _ho_(_mm_slli_si128(_v_, 8), _v_));\
}
static ALWAYS_INLINE __m128i mm_scan_epi32(__m128i v, __m128i sv) { MM_HDEC_EPI32(v,sv,_mm_add_epi32); return v; }
static ALWAYS_INLINE __m128i mm_xord_epi32(__m128i v, __m128i sv) { MM_HDEC_EPI32(v,sv,_mm_xor_si128); return v; }

#define MM_HDEC_EPI64(_v_,_sv_,_ho_) { \
  _v_ = _ho_(_v_, _mm_slli_si128(_v_, 8)); \
  _v_ = _ho_(_mm_shuffle_epi8(_sv_, _mm_set_epi8(15,14,13,12,11,10,9,8, 15,14,13,12,11,10,9,8)), _v_);\
}
static ALWAYS_INLINE __m128i mm_scan_epi64(__m128i v, __m128i sv) { MM_HDEC_EPI64(v,sv,_mm_add_epi64); return v; }
static ALWAYS_INLINE __m128i mm_xord_epi64(__m128i v, __m128i sv) { MM_HDEC_EPI64(v,sv,_mm_xor_si128); return v; }

#define MM_HDEC_EPI16(_v_,_sv_,_ho_) {\
  _v_  = _ho_(      _v_, _mm_slli_si128(_v_, 2));\
  _v_  = _ho_(      _v_, _mm_slli_si128(_v_, 4));\
  _v_  = _ho_(_ho_(_v_, _mm_slli_si128(_v_, 8)), _mm_shuffle_epi8(_sv_, _mm_set1_epi16(0x0f0e)));\
}
static ALWAYS_INLINE __m128i mm_scan_epi16(__m128i v, __m128i sv) { MM_HDEC_EPI16(v,sv,_mm_add_epi16); return v; }
static ALWAYS_INLINE __m128i mm_xord_epi16(__m128i v, __m128i sv) { MM_HDEC_EPI16(v,sv,_mm_xor_si128); return v; }

#define MM_HDEC_EPI8(_v_,_sv_,_ho_) {\
  _v_  = _ho_(      _v_, _mm_slli_si128(_v_, 1));\
  _v_  = _ho_(      _v_, _mm_slli_si128(_v_, 2));\
  _v_  = _ho_(      _v_, _mm_slli_si128(_v_, 4));\
  _v_  = _ho_(_ho_(_v_, _mm_slli_si128(_v_, 8)), _mm_shuffle_epi8(_sv_, _mm_set1_epi8(0xfe)));/*TODO: test*/\
}
static ALWAYS_INLINE __m128i mm_scan_epi8(__m128i v, __m128i sv) { MM_HDEC_EPI8(v,sv,_mm_add_epi8);  return v; }
static ALWAYS_INLINE __m128i mm_xord_epi8(__m128i v, __m128i sv) { MM_HDEC_EPI8(v,sv,_mm_xor_si128); return v; }

//-------- scan with vi delta > 0 -----------------------------
static ALWAYS_INLINE __m128i mm_scani_epi16(__m128i v, __m128i sv, __m128i vi) { return _mm_add_epi16(mm_scan_epi16(v, sv), vi); }
static ALWAYS_INLINE __m128i mm_scani_epi32(__m128i v, __m128i sv, __m128i vi) { return _mm_add_epi32(mm_scan_epi32(v, sv), vi); }

#define MM_HOZ_EPI16(v,_ho_) {\
  v = _ho_(v, _mm_srli_si128(v, 8));\
  v = _ho_(v, _mm_srli_si128(v, 6));\
  v = _ho_(v, _mm_srli_si128(v, 4));\
  v = _ho_(v, _mm_srli_si128(v, 2));\
}

#define MM_HOZ_EPI32(v,_ho_) {\
  v = _ho_(v, _mm_srli_si128(v, 8));\
  v = _ho_(v, _mm_srli_si128(v, 4));\
}

static ALWAYS_INLINE uint16_t mm_hor_epi16( __m128i v) { MM_HOZ_EPI16(v,_mm_or_si128);               return (unsigned short)_mm_cvtsi128_si32(v); }
static ALWAYS_INLINE uint32_t mm_hor_epi32( __m128i v) { MM_HOZ_EPI32(v,_mm_or_si128);               return (unsigned      )_mm_cvtsi128_si32(v); }
static ALWAYS_INLINE uint64_t mm_hor_epi64( __m128i v) { v = _mm_or_si128( v, _mm_srli_si128(v, 8)); return (uint64_t      )_mm_cvtsi128_si64(v); }
  
//----------------- sub / add ----------------------------------------------------------
#define SUBI16x8(_v_, _sv_)       _mm_sub_epi16(_v_, _sv_)
#define SUBI32x4(_v_, _sv_)       _mm_sub_epi32(_v_, _sv_)
#define ADDI16x8(_v_, _sv_, _vi_) _sv_ = _mm_add_epi16(_mm_add_epi16(_sv_, _vi_),_v_)
#define ADDI32x4(_v_, _sv_, _vi_) _sv_ = _mm_add_epi32(_mm_add_epi32(_sv_, _vi_),_v_)

//---------------- Convert _mm_cvtsi128_siXX -------------------------------------------
static ALWAYS_INLINE uint8_t   mm_cvtsi128_si8 (__m128i v) { return (uint8_t )_mm_cvtsi128_si32(v); }
static ALWAYS_INLINE uint16_t  mm_cvtsi128_si16(__m128i v) { return (uint16_t)_mm_cvtsi128_si32(v); }
#define mm_cvtsi128_si32(_v_) _mm_cvtsi128_si32(_v_)

  #elif defined(__SSE2__)
static ALWAYS_INLINE __m128i mm_delta_epi16(__m128i v, __m128i sv) { return _mm_sub_epi16(v, _mm_or_si128(_mm_srli_si128(sv, 14), _mm_slli_si128(v, 2))); }
static ALWAYS_INLINE __m128i mm_xore_epi16( __m128i v, __m128i sv) { return _mm_xor_si128(v, _mm_or_si128(_mm_srli_si128(sv, 14), _mm_slli_si128(v, 2))); }
static ALWAYS_INLINE __m128i mm_delta_epi32(__m128i v, __m128i sv) { return _mm_sub_epi32(v, _mm_or_si128(_mm_srli_si128(sv, 12), _mm_slli_si128(v, 4))); }
static ALWAYS_INLINE __m128i mm_xore_epi32( __m128i v, __m128i sv) { return _mm_xor_si128(v, _mm_or_si128(_mm_srli_si128(sv, 12), _mm_slli_si128(v, 4))); }
  #endif

//--------- memset -----------------------------------------
#define BITFORSET_(_out_, _n_, _start_, _mindelta_) do { unsigned _i;\
  for(_i = 0; _i != (_n_&~3); _i+=4) {       \
    _out_[_i+0] = _start_+(_i  )*_mindelta_; \
    _out_[_i+1] = _start_+(_i+1)*_mindelta_; \
    _out_[_i+2] = _start_+(_i+2)*_mindelta_; \
    _out_[_i+3] = _start_+(_i+3)*_mindelta_; \
  }                                          \
  while(_i != _n_)                           \
    _out_[_i] = _start_+_i*_mindelta_, ++_i; \
} while(0)

//--------- SIMD zero -----------------------------------------
  #ifdef __AVX2__
#define BITZERO32(_out_, _n_, _start_) do {\
  __m256i _sv_ = _mm256_set1_epi32(_start_), *_ov = (__m256i *)(_out_), *_ove = (__m256i *)(_out_ + _n_);\
  do _mm256_storeu_si256(_ov++, _sv_); while(_ov < _ove);\
} while(0)

#define BITFORZERO32(_out_, _n_, _start_, _mindelta_) do {\
  __m256i _sv = _mm256_set1_epi32(_start_), *_ov=(__m256i *)(_out_), *_ove = (__m256i *)(_out_ + _n_), _cv = _mm256_set_epi32(7+_mindelta_,6+_mindelta_,5+_mindelta_,4+_mindelta_,3*_mindelta_,2*_mindelta_,1*_mindelta_,0); \
    _sv = _mm256_add_epi32(_sv, _cv);\
    _cv = _mm256_set1_epi32(4);\
  do { _mm256_storeu_si256(_ov++, _sv); _sv = _mm256_add_epi32(_sv, _cv); } while(_ov < _ove);\
} while(0)

#define BITDIZERO32(_out_, _n_, _start_, _mindelta_) do { __m256i _sv = _mm256_set1_epi32(_start_), _cv = _mm256_set_epi32(7+_mindelta_,6+_mindelta_,5+_mindelta_,4+_mindelta_,3+_mindelta_,2+_mindelta_,1+_mindelta_,_mindelta_), *_ov=(__m256i *)(_out_), *_ove = (__m256i *)(_out_ + _n_);\
  _sv = _mm256_add_epi32(_sv, _cv); _cv = _mm256_set1_epi32(4*_mindelta_); do { _mm256_storeu_si256(_ov++, _sv), _sv = _mm256_add_epi32(_sv, _cv); } while(_ov < _ove);\
} while(0)

  #elif defined(__SSE2__) || defined(__ARM_NEON) // -------------
// SIMD set value (memset)
#define BITZERO32(_out_, _n_, _v_) do {\
  __m128i _sv_ = _mm_set1_epi32(_v_), *_ov = (__m128i *)(_out_), *_ove = (__m128i *)(_out_ + _n_);\
  do _mm_storeu_si128(_ov++, _sv_); while(_ov < _ove); \
} while(0)

#define BITFORZERO32(_out_, _n_, _start_, _mindelta_) do {\
  __m128i _sv = _mm_set1_epi32(_start_), *_ov=(__m128i *)(_out_), *_ove = (__m128i *)(_out_ + _n_), _cv = _mm_set_epi32(3*_mindelta_,2*_mindelta_,1*_mindelta_,0); \
    _sv = _mm_add_epi32(_sv, _cv);\
    _cv = _mm_set1_epi32(4);\
  do { _mm_storeu_si128(_ov++, _sv); _sv = _mm_add_epi32(_sv, _cv); } while(_ov < _ove);\
} while(0)

#define BITDIZERO32(_out_, _n_, _start_, _mindelta_) do { __m128i _sv = _mm_set1_epi32(_start_), _cv = _mm_set_epi32(3+_mindelta_,2+_mindelta_,1+_mindelta_,_mindelta_), *_ov=(__m128i *)(_out_), *_ove = (__m128i *)(_out_ + _n_);\
  _sv = _mm_add_epi32(_sv, _cv); _cv = _mm_set1_epi32(4*_mindelta_); do { _mm_storeu_si128(_ov++, _sv), _sv = _mm_add_epi32(_sv, _cv); } while(_ov < _ove);\
} while(0)
  #else
#define BITFORZERO32(_out_, _n_, _start_, _mindelta_) BITFORSET_(_out_, _n_, _start_, _mindelta_)
#define BITZERO32(   _out_, _n_, _start_)             BITFORSET_(_out_, _n_, _start_, 0)
  #endif
#define BITZERO16(   _out_, _n_, _start_)             BITFORSET_(_out_, _n_, _start_, 0)

#define DELTR( _in_, _n_, _start_, _mindelta_,      _out_) { unsigned _v; for(      _v = 0; _v < _n_; _v++) _out_[_v] = _in_[_v] - (_start_) - _v*(_mindelta_) - (_mindelta_); }
#define DELTRB(_in_, _n_, _start_, _mindelta_, _b_, _out_) { unsigned _v; for(_b_=0,_v = 0; _v < _n_; _v++) _out_[_v] = _in_[_v] - (_start_) - _v*(_mindelta_) - (_mindelta_), _b_ |= _out_[_v]; _b_ = bsr32(_b_); }

//----------------------------------------- bitreverse scalar + SIMD -------------------------------------------
  #if __clang__ && defined __has_builtin
    #if __has_builtin(__builtin_bitreverse64)
#define BUILTIN_BITREVERSE      
    #else
#define BUILTIN_BITREVERSE  
    #endif 
  #endif
  #ifdef BUILTIN_BITREVERSE
#define rbit8(x)  __builtin_bitreverse8( x)
#define rbit16(x) __builtin_bitreverse16(x)
#define rbit32(x) __builtin_bitreverse32(x)
#define rbit64(x) __builtin_bitreverse64(x)
  #else

    #if (__CORTEX_M >= 0x03u) || (__CORTEX_SC >= 300u)
static ALWAYS_INLINE uint32_t _rbit_(uint32_t x) { uint32_t rc; __asm volatile ("rbit %0, %1" : "=r" (rc) : "r" (x) ); }
    #endif
static ALWAYS_INLINE uint8_t rbit8(uint8_t x) {
    #if (__CORTEX_M >= 0x03u) || (__CORTEX_SC >= 300u)
  return _rbit_(x) >> 24;
    #elif 0
  x = (x & 0xaa) >> 1 | (x & 0x55) << 1;
  x = (x & 0xcc) >> 2 | (x & 0x33) << 2;
  return x << 4 | x >> 4;
    #else
  return (x * 0x0202020202ull & 0x010884422010ull) % 1023;
    #endif
}

static ALWAYS_INLINE uint16_t rbit16(uint16_t x) {
    #if (__CORTEX_M >= 0x03u) || (__CORTEX_SC >= 300u)
  return _rbit_(x) >> 16;
    #else
  x = (x & 0xaaaa) >> 1 | (x & 0x5555) << 1;
  x = (x & 0xcccc) >> 2 | (x & 0x3333) << 2;
  x = (x & 0xf0f0) >> 4 | (x & 0x0f0f) << 4;
  return x << 8 | x >> 8;
    #endif
}

static ALWAYS_INLINE uint32_t rbit32(uint32_t x) {
    #if (__CORTEX_M >= 0x03u) || (__CORTEX_SC >= 300u)
  return _rbit_(x);
    #else
  x = ((x & 0xaaaaaaaa) >> 1 | (x & 0x55555555) << 1);
  x = ((x & 0xcccccccc) >> 2 | (x & 0x33333333) << 2);
  x = ((x & 0xf0f0f0f0) >> 4 | (x & 0x0f0f0f0f) << 4);
  x = ((x & 0xff00ff00) >> 8 | (x & 0x00ff00ff) << 8);
  return x << 16 | x >> 16;
    #endif
}
static ALWAYS_INLINE uint64_t rbit64(uint64_t x) {
    #if (__CORTEX_M >= 0x03u) || (__CORTEX_SC >= 300u)
  return (uint64_t)_rbit_(x) << 32 | _rbit_(x >> 32);
    #else
  x = (x & 0xaaaaaaaaaaaaaaaa) >>  1 | (x & 0x5555555555555555) <<  1;
  x = (x & 0xcccccccccccccccc) >>  2 | (x & 0x3333333333333333) <<  2;
  x = (x & 0xf0f0f0f0f0f0f0f0) >>  4 | (x & 0x0f0f0f0f0f0f0f0f) <<  4;
  x = (x & 0xff00ff00ff00ff00) >>  8 | (x & 0x00ff00ff00ff00ff) <<  8;
  x = (x & 0xffff0000ffff0000) >> 16 | (x & 0x0000ffff0000ffff) << 16;
  return x << 32 | x >> 32;
    #endif
}
  #endif

  #if defined(__SSSE3__) || defined(__ARM_NEON)
static ALWAYS_INLINE __m128i mm_rbit_epi16(__m128i v) { return mm_rbit_epi8(mm_rev_epi16(v)); }
static ALWAYS_INLINE __m128i mm_rbit_epi32(__m128i v) { return mm_rbit_epi8(mm_rev_epi32(v)); }
static ALWAYS_INLINE __m128i mm_rbit_epi64(__m128i v) { return mm_rbit_epi8(mm_rev_epi64(v)); }
//static ALWAYS_INLINE __m128i mm_rbit_si128(__m128i v) { return mm_rbit_epi8(mm_rev_si128(v)); }
  #endif

  #ifdef __AVX2__
static ALWAYS_INLINE __m256i mm256_rbit_epi8(__m256i v) {
  __m256i fv = _mm256_setr_epi8(0, 8, 4,12, 2,10, 6,14, 1, 9, 5,13, 3,11, 7,15, 0, 8, 4,12, 2,10, 6,14, 1, 9, 5,13, 3,11, 7,15), cv0f_8 = _mm256_set1_epi8(0xf);
  __m256i lv = _mm256_shuffle_epi8(fv,_mm256_and_si256(                  v,     cv0f_8));
  __m256i hv = _mm256_shuffle_epi8(fv,_mm256_and_si256(_mm256_srli_epi64(v, 4), cv0f_8));
  return _mm256_or_si256(_mm256_slli_epi64(lv,4), hv);
}

static ALWAYS_INLINE __m256i mm256_rev_epi16(__m256i v) { return _mm256_shuffle_epi8(v, _mm256_setr_epi8( 1, 0, 3, 2, 5, 4, 7, 6,  9, 8,11,10,13,12,15,14,  1, 0, 3, 2, 5, 4, 7, 6,  9, 8,11,10,13,12,15,14)); }
static ALWAYS_INLINE __m256i mm256_rev_epi32(__m256i v) { return _mm256_shuffle_epi8(v, _mm256_setr_epi8( 3, 2, 1, 0, 7, 6, 5, 4, 11,10, 9, 8,15,14,13,12,  3, 2, 1, 0, 7, 6, 5, 4, 11,10, 9, 8,15,14,13,12)); }
static ALWAYS_INLINE __m256i mm256_rev_epi64(__m256i v) { return _mm256_shuffle_epi8(v, _mm256_setr_epi8( 7, 6, 5, 4, 3, 2, 1, 0, 15,14,13,12,11,10, 9, 8,  7, 6, 5, 4, 3, 2, 1, 0, 15,14,13,12,11,10, 9, 8)); }
static ALWAYS_INLINE __m256i mm256_rev_si128(__m256i v) { return _mm256_shuffle_epi8(v, _mm256_setr_epi8(15,14,13,12,11,10, 9, 8,  7, 6, 5, 4, 3, 2, 1, 0, 15,14,13,12,11,10, 9, 8,  7, 6, 5, 4, 3, 2, 1, 0)); }

static ALWAYS_INLINE __m256i mm256_rbit_epi16(__m256i v) { return mm256_rbit_epi8(mm256_rev_epi16(v)); }
static ALWAYS_INLINE __m256i mm256_rbit_epi32(__m256i v) { return mm256_rbit_epi8(mm256_rev_epi32(v)); }
static ALWAYS_INLINE __m256i mm256_rbit_epi64(__m256i v) { return mm256_rbit_epi8(mm256_rev_epi64(v)); }
static ALWAYS_INLINE __m256i mm256_rbit_si128(__m256i v) { return mm256_rbit_epi8(mm256_rev_si128(v)); }
  #endif

// ------------------ bitio general macros ---------------------------
#define bitdef(     _bw_,_br_)           uint64_t _bw_=0; unsigned _br_=0
#define bitini(     _bw_,_br_)           _bw_=_br_=0
//-- bitput ---------
#define bitput(     _bw_,_br_,_nb_,_x_)  (_bw_) += (uint64_t)(_x_) << (_br_), (_br_) += (_nb_)
#define bitenorm(   _bw_,_br_,_op_)      ctou64(_op_) = _bw_; _op_ += ((_br_)>>3), (_bw_) >>=((_br_)&~7), (_br_) &= 7
#define bitflush(   _bw_,_br_,_op_)      ctou64(_op_) = _bw_, _op_ += ((_br_)+7)>>3, _bw_=_br_=0
//-- bitget ---------
#define bitbw(      _bw_,_br_)           ((_bw_)>>(_br_))
#define bitrmv(     _bw_,_br_,_nb_)      (_br_) += _nb_

#define bitdnorm(   _bw_,_br_,_ip_)      _bw_ = ctou64((_ip_) += ((_br_)>>3)), (_br_) &= 7
#define bitalign(   _bw_,_br_,_ip_)      ((_ip_) += ((_br_)+7)>>3)

#define BITPEEK32(  _bw_,_br_,_nb_)      BZHI32(bitbw(_bw_,_br_), _nb_)
#define BITGET32(   _bw_,_br_,_nb_,_x_)  _x_ = BITPEEK32(_bw_, _br_, _nb_), bitrmv(_bw_, _br_, _nb_)
#define BITPEEK64(  _bw_,_br_,_nb_)      BZHI64(bitbw(_bw_,_br_), _nb_)
#define BITGET64(   _bw_,_br_,_nb_,_x_)  _x_ = BITPEEK64(_bw_, _br_, _nb_), bitrmv(_bw_, _br_, _nb_)

#define bitpeek57(  _bw_,_br_,_nb_)      bzhi64(bitbw(_bw_,_br_), _nb_)
#define bitget57(   _bw_,_br_,_nb_,_x_)  _x_ = bitpeek57(_bw_, _br_, _nb_), bitrmv(_bw_, _br_, _nb_)
#define bitpeek31(  _bw_,_br_,_nb_)      bzhi32(bitbw(_bw_,_br_), _nb_)
#define bitget31(   _bw_,_br_,_nb_,_x_)  _x_ = bitpeek31(_bw_, _br_, _nb_), bitrmv(_bw_, _br_, _nb_)
//------------------ templates -----------------------------------
#define bitput8( _bw_,_br_,_b_,_x_,_op_) bitput(_bw_,_br_,_b_,_x_)
#define bitput16(_bw_,_br_,_b_,_x_,_op_) bitput(_bw_,_br_,_b_,_x_)
#define bitput32(_bw_,_br_,_b_,_x_,_op_) bitput(_bw_,_br_,_b_,_x_)
#define bitput64(_bw_,_br_,_b_,_x_,_op_) if((_b_)>45) { bitput(_bw_,_br_,(_b_)-32, (_x_)>>32); bitenorm(_bw_,_br_,_op_); bitput(_bw_,_br_,32,(unsigned)(_x_)); } else bitput(_bw_,_br_,_b_,_x_)

#define bitget8( _bw_,_br_,_b_,_x_,_ip_) bitget31(_bw_,_br_,_b_,_x_)
#define bitget16(_bw_,_br_,_b_,_x_,_ip_) bitget31(_bw_,_br_,_b_,_x_)
#define bitget32(_bw_,_br_,_b_,_x_,_ip_) bitget57(_bw_,_br_,_b_,_x_)
#define bitget64(_bw_,_br_,_b_,_x_,_ip_) if((_b_)>45) { unsigned _v; bitget57(_bw_,_br_,(_b_)-32,_x_); bitdnorm(_bw_,_br_,_ip_); BITGET64(_bw_,_br_,32,_v); _x_ = _x_<<32|_v; } else bitget57(_bw_,_br_,_b_,_x_)  

//---- Floating point to Integer decomposition ---------------------------------
// seeeeeeee21098765432109876543210 (s:sign, e:exponent, 0-9:mantissa)
#define MANTF32    23
#define MANTF64    52

#define BITFENC(_u_, _sgn_, _expo_, _mant_,      _mantbits_, _one_) _sgn_ = _u_ >> (sizeof(_u_)*8-1); _expo_ = ((_u_ >> (_mantbits_)) & ( (_one_<<(sizeof(_u_)*8 - 1 - _mantbits_)) -1)); _mant_ = _u_ & ((_one_<<_mantbits_)-1);
#define BITFDEC(     _sgn_, _expo_, _mant_, _u_, _mantbits_)        _u_ = (_sgn_) << (sizeof(_u_)*8-1) | (_expo_) << _mantbits_ | (_mant_)
