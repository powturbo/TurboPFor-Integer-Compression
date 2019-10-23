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
// intel sse to arm neon 

#ifndef _SSE_NEON_H_
#define _SSE_NEON_H_
#include "conf.h"

  #ifdef __ARM_NEON  //--------------------------------------------------------------------------------------------------
#include <arm_neon.h>
#define __m128i uint32x4_t

//#define USE_MACROS
#define uint8x16_to_8x8x2(_a_)                  ((uint8x8x2_t) { vget_low_u8(_a_), vget_high_u8(_a_) })

#ifdef USE_MACROS //---------------------------- Set : _mm_set_epi/_mm_set1_epi ----------------------------------------------------------
#define _mm_set_epi8(u15,u14,u13,u12,\
                     u11,u10, u9, u8,\
					     u7,u6,u5,u4,\
					     u3,u2,u1,u0) 			({ uint8_t  __attribute__((aligned(16))) _u[16] = { u0,u1,u2,u3,u4,u5,u6,u7,u8,u9,u10,u11,u12,u13,u14,u15 }; (uint32x4_t)vld1q_u8( _u);})
#define _mm_set_epi16(   u7,u6,u5,u4,\
                         u3,u2,u1,u0)           ({ uint16_t __attribute__((aligned(16))) _u[ 8] = { u0,u1,u2,u3,u4,u5,u6,u7 }; 					             (uint32x4_t)vld1q_u16(_u);})
#define _mm_set_epi32(   u3,u2,u1,u0) 			({ uint32_t __attribute__((aligned(16))) _u[ 4] = { u0,u1,u2,u3 }; 									                    vld1q_u32(_u);})
#define _mm_set_epi64x(        u1,u0) 			({ uint64_t __attribute__((aligned(16))) _u[ 2] = { u0,u1 };   									             (uint32x4_t)vld1q_u64(_u);})
#define _mm_set_epi32(u3, u2, u1, u0)           vcombine_u32(vcreate_u32((uint64_t)u1 << 32 | u0), vcreate_u32((uint64_t)u3 << 32 | u2))
#define _mm_set_epi64x(u1, u0)                  (__m128i)vcombine_u64(vcreate_u64(u0), vcreate_u64(u1))
#else
static ALWAYS_INLINE __m128i _mm_set_epi8(      uint8_t u15, uint8_t u14, uint8_t u13, uint8_t u12, uint8_t u11, uint8_t u10, uint8_t  u9, uint8_t  u8,
                                                uint8_t  u7, uint8_t  u6, uint8_t  u5, uint8_t  u4, 
                                                uint8_t  u3, uint8_t  u2, uint8_t  u1, uint8_t  u0) {
                                                  uint8_t  __attribute__((aligned(16))) u[16] = { u0,u1,u2,u3,u4,u5,u6,u7,u8,u9,u10,u11,u12,u13,u14,u15 };                       return (uint32x4_t)vld1q_u8( u); }
static ALWAYS_INLINE __m128i _mm_set_epi16(     uint16_t u7, uint16_t u6, uint16_t u5, uint16_t u4, 
                                                uint16_t u3, uint16_t u2, uint16_t u1, uint16_t u0) { uint16_t __attribute__((aligned(16))) u[ 8] = { u0,u1,u2,u3,u4,u5,u6,u7 }; return (uint32x4_t)vld1q_u16(u); }
static ALWAYS_INLINE __m128i _mm_set_epi32(     uint32_t u3, uint32_t u2, uint32_t u1, uint32_t u0) { uint32_t __attribute__((aligned(16))) u[ 4] = { u0,u1,u2,u3 }; 			 return             vld1q_u32(u); }
static ALWAYS_INLINE __m128i _mm_set_epi64x(    uint64_t u1, uint64_t u0) { uint64_t __attribute__((aligned(16))) u[ 2] = { u0,u1 };   									     return (uint32x4_t)vld1q_u64(u); }
#endif

#define _mm_set1_epi8(  _u8_ )					(__m128i)vdupq_n_u8( _u8_ )
#define _mm_set1_epi16( _u16_)					(__m128i)vdupq_n_u16(_u16_)
#define _mm_set1_epi32( _u32_)							 vdupq_n_u32(_u32_)
#define _mm_set1_epi64x(_u64_)     				(__m128i)vdupq_n_u64(_u64_)
#define _mm_setzero_si128()                              vdupq_n_u32( 0   )
//---------------------------------------------- Arithmetic -----------------------------------------------------------------------
#define _mm_add_epi8(  _a_,_b_)   				(__m128i)vaddq_u8((uint8x16_t)(_a_), (uint8x16_t)(_b_)) 
#define _mm_add_epi16(  _a_,_b_)   				(__m128i)vaddq_u16((uint16x8_t)(_a_), (uint16x8_t)(_b_)) 
#define _mm_add_epi32(  _a_,_b_)   				            vaddq_u32(             _a_,               _b_ ) 
#define _mm_sub_epi16(  _a_,_b_)   				(__m128i)vsubq_u16((uint16x8_t)(_a_), (uint16x8_t)(_b_)) 
#define _mm_sub_epi32(  _a_,_b_)   				(__m128i)vsubq_u32((uint32x4_t)(_a_), (uint32x4_t)(_b_)) 
#define _mm_subs_epu8(   _a_,_b_)   			(__m128i)vqsubq_u8((uint8x16_t)(_a_), (uint8x16_t)(_b_)) 

#define _mm_mullo_epi32(_a_,_b_)   				(__m128i)vmulq_s32(( int32x4_t)(_a_), ( int32x4_t)(_b_))
#define  mm_mullo_epu32(_a_,_b_)   				         vmulq_u32(_a_,_b_)
#define _mm_mul_epu32(  _a_,_b_)     			(__m128i)vmull_u32(vget_low_u32(_a_),vget_low_u32(_b_))
#define _mm_adds_epu16( _a_,_b_)                (__m128i)vqaddq_u16((uint16x8_t)(_a_),(uint16x8_t)(_b_))
static ALWAYS_INLINE __m128i _mm_madd_epi16(__m128i a, __m128i b) { 
  int32x4_t mlo = vmull_s16(vget_low_s16( (int16x8_t)a), vget_low_s16( (int16x8_t)b));
  int32x4_t mhi = vmull_s16(vget_high_s16((int16x8_t)a), vget_high_s16((int16x8_t)b));
  int32x2_t alo = vpadd_s32(vget_low_s32(mlo), vget_high_s32(mlo));
  int32x2_t ahi = vpadd_s32(vget_low_s32(mhi), vget_high_s32(mhi));
  return (__m128i)vcombine_s32(alo, ahi);
}
//---------------------------------------------- Special math functions -----------------------------------------------------------
#define _mm_min_epu8(   _a_,_b_)                (__m128i)vminq_u8((uint8x16_t)(_a_), (uint8x16_t)(_b_))
#define _mm_min_epu16(  _a_,_b_)                (__m128i)vminq_u16((uint16x8_t)(_a_), (uint16x8_t)(_b_))
#define _mm_min_epi16(  _a_,_b_)                (__m128i)vminq_s16((int16x8_t)(_a_), (int16x8_t)(_b_))
//---------------------------------------------- Logical --------------------------------------------------------------------------
#define  mm_testnz_epu32(_a_)   				vmaxvq_u32(_a_) //vaddvq_u32(_a_)
#define  mm_testnz_epu8(_a_)   				    vmaxv_u8(_a_)
#define _mm_or_si128(   _a_,_b_)    			(__m128i)vorrq_u32(  (uint32x4_t)(_a_), (uint32x4_t)(_b_))
#define _mm_and_si128(  _a_,_b_)    			(__m128i)vandq_u32(  (uint32x4_t)(_a_), (uint32x4_t)(_b_))
#define _mm_xor_si128(  _a_,_b_)    			(__m128i)veorq_u32(  (uint32x4_t)(_a_), (uint32x4_t)(_b_))
//---------------------------------------------- Shift ----------------------------------------------------------------------------
#define _mm_slli_epi16( _a_,_m_)    			(__m128i)vshlq_n_u16((uint16x8_t)(_a_), _m_)
#define _mm_slli_epi32( _a_,_m_)    			(__m128i)vshlq_n_u32((uint32x4_t)(_a_), _m_)
#define _mm_slli_epi64( _a_,_m_)    			(__m128i)vshlq_n_u64((uint64x2_t)(_a_), _m_)
#define _mm_slli_si128( _a_,_m_)                (__m128i)vextq_u8(vdupq_n_u8(0), (uint8x16_t)(_a_), 16 - (_m_) ) // _m_: 1 - 15
 
#define _mm_srli_epi16( _a_,_m_)    			(__m128i)vshrq_n_u16((uint16x8_t)(_a_), _m_)
#define _mm_srli_epi32( _a_,_m_)    			(__m128i)vshrq_n_u32((uint32x4_t)(_a_), _m_)
#define _mm_srli_epi64( _a_,_m_)    			(__m128i)vshlq_n_u64((uint64x2_t)(_a_), _m_) 
#define _mm_srli_si128( _a_,_m_) 			    (__m128i)vextq_s8((int8x16_t)(_a_), vdupq_n_s8(0), (_m_))

#define _mm_srai_epi16( _a_,_m_)			    (__m128i)vshrq_n_s16((int16x8_t)(_a_), _m_)  
#define _mm_srai_epi32( _a_,_m_)			    (__m128i)vshrq_n_s32((int32x4_t)(_a_), _m_) 
#define _mm_srai_epi64( _a_,_m_)			    (__m128i)vshrq_n_s64((int64x2_t)(_a_), _m_) 

#define _mm_sllv_epi32( _a_,_b_)    			(__m128i)vshlq_u32((uint32x4_t)(_a_), (uint32x4_t)(_b_))									
#define _mm_srlv_epi32( _a_,_b_)                (__m128i)vshlq_u32((uint32x4_t)(_a_), vnegq_s32((int32x4_t)(_b_)))
//---------------------------------------------- Compare --------- true/false->1/0 (all bits set) ---------------------------------
#define _mm_cmpeq_epi8( _a_,_b_)   				(__m128i)vceqq_s8( ( int8x16_t)(_a_), ( int8x16_t)(_b_))
#define _mm_cmpeq_epi16(_a_,_b_)   				(__m128i)vceqq_s16(( int16x8_t)(_a_), ( int16x8_t)(_b_))
#define _mm_cmpeq_epi32(_a_,_b_)   				(__m128i)vceqq_s32(( int32x4_t)(_a_), ( int32x4_t)(_b_))

#define _mm_cmpgt_epi16(_a_,_b_)   				(__m128i)vcgtq_s16(( int16x8_t)(_a_), ( int16x8_t)(_b_)) 
#define _mm_cmpgt_epi32(_a_,_b_)   				(__m128i)vcgtq_s32(( int32x4_t)(_a_), ( int32x4_t)(_b_))

#define _mm_cmpgt_epu16(_a_,_b_)   				(__m128i)vcgtq_u16((uint16x8_t)(_a_), (uint16x8_t)(_b_)) 
#define  mm_cmpgt_epu32(_a_,_b_)   				(__m128i)vcgtq_u32(             _a_,               _b_) 
//---------------------------------------------- Load -----------------------------------------------------------------------------
#define _mm_loadl_epi64(   _u64p_)              (__m128i)vcombine_s32(vld1_s32((int32_t const *)(_u64p_)), vcreate_s32(0))
#define  mm_loadu_epi64p(  _u64p_,_a_) 			(__m128i)vld1q_lane_u64((uint64_t *)(_u64p_), (uint64x2_t)(_a_), 0)
#define _mm_loadu_si128(  _ip_)     			vld1q_u32(_ip_)
#define _mm_load_si128(   _ip_)      			vld1q_u32(_ip_)
//---------------------------------------------- Store ----------------------------------------------------------------------------
#define _mm_storel_epi64(_ip_,_a_)   			vst1q_lane_u64((uint64_t *)(_ip_), (uint64x2_t)(_a_), 0)
#define _mm_storeu_si128(_ip_,_a_)  			vst1q_u32((__m128i *)(_ip_),_a_)
//---------------------------------------------- Convert --------------------------------------------------------------------------
#define  mm_cvtsi64_si128p(_u64p_,_a_)          mm_loadu_epi64p(_u64p_,_a_)
#define _mm_cvtsi64_si128(_a_) 				    (__m128i)vdupq_n_u64(_a_)  //vld1q_s64(_a_)
//---------------------------------------------- Reverse bits/bytes ---------------------------------------------------------------
#define mm_rbit_epi8(a)                         (__m128i)vrbitq_u8( (uint8x16_t)(a)) // reverse bits
#define mm_rev_epi16(a)                         vrev16q_u8((uint8x16_t)(a))           // reverse bytes
#define mm_rev_epi32(a)                         vrev32q_u8((uint8x16_t)(a))
#define mm_rev_epi64(a)                         vrev64q_u8((uint8x16_t)(a))
//--------------------------------------------- Insert/extract --------------------------------------------------------------------
#define  mm_extract_epi32x(_a_,_u32_,_id_) 	    vst1q_lane_u32((uint32_t *)&(_u32_),              _a_,  _id_)
#define _mm_extract_epi64x(_a_,_u64_,_id_)      vst1q_lane_u64((uint64_t *)&(_u64_), (uint64x2_t)(_a_), _id_)

#define _mm_extract_epi8(_a_,        _id_)      vgetq_lane_u8( (uint8x16_t)(_a_), _id_)
#define _mm_extract_epi16(_a_,       _id_)      vgetq_lane_u16(_a_, _id_)
#define _mm_extract_epi32(_a_,       _id_)      vgetq_lane_u32(_a_, _id_)
#define  mm_extract_epu32(_a_,       _id_)      vgetq_lane_u32(_a_, _id_)
#define _mm_cvtsi128_si32(_a_)  				vgetq_lane_u32((uint32x4_t)(_a_),0)
#define _mm_cvtsi128_si64(_a_)  				vgetq_lane_u64((uint64x2_t)(_a_),0)

#define _mm_insert_epu32p(_a_,_u32p_,_id_)      vsetq_lane_u32(_x_, _a_, _id_)
#define  mm_insert_epi32p(_a_,_u32p_,_id_) 	    vld1q_lane_u32(_u32p_, (uint32x4_t)(_a_), _id_)
#define _mm_cvtsi32_si128(_a_) 					(__m128i)vsetq_lane_s32(_a_, vdupq_n_s32(0), 0)

#define _mm_blendv_epi8(_a_,_b_,_m_) 			vbslq_u32(_m_,_b_,_a_)
//---------------------------------------------- Miscellaneous --------------------------------------------------------------------
#define _mm_alignr_epi8(_a_,_b_,_m_)            (__m128i)vextq_u8(  (uint8x16_t)(_b_), (uint8x16_t)(_a_), _m_)                   
#define _mm_packs_epi16(   _a_,_b_) 		    (__m128i)vcombine_s8( vqmovn_s16((int16x8_t)(_a_)), vqmovn_s16((int16x8_t)(_b_)))
#define _mm_packs_epi32(   _a_,_b_) 		    (__m128i)vcombine_s16(vqmovn_s32((int32x4_t)(_a_)), vqmovn_s32((int32x4_t)(_b_)))

#define _mm_packs_epu16(   _a_,_b_) 			(__m128i)vcombine_u8((uint16x8_t)(_a_), (uint16x8_t)(_b_))
#define _mm_packus_epi16(  _a_,_b_) 			(__m128i)vcombine_u8(vqmovun_s16((int16x8_t)(_a_)), vqmovun_s16((int16x8_t)(_b_)))

static ALWAYS_INLINE uint16_t _mm_movemask_epi8(__m128i v) {
  const uint8x16_t __attribute__ ((aligned (16))) m = {1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7, 1, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6, 1<<7};
  uint8x16_t mv = (uint8x16_t)vpaddlq_u32(vpaddlq_u16(vpaddlq_u8(vandq_u8(vcltq_s8((int8x16_t)v, vdupq_n_s8(0)), m))));
  return vgetq_lane_u8(mv, 8) << 8 | vgetq_lane_u8(mv, 0);
}
//-------- Neon movemask ------ All lanes must be 0 or -1 (=0xff, 0xffff or 0xffffffff)
    #ifdef __aarch64__
static ALWAYS_INLINE uint8_t   mm_movemask_epi8s(uint8x8_t sv) { const uint8x8_t  m = { 1, 1<<1, 1<<2, 1<<3, 1<<4, 1<< 5, 1<< 6, 1<<7 }; return vaddv_u8(  vand_u8(             sv, m)); } // short only ARM
//static ALWAYS_INLINE uint16_t  mm_movemask_epu16(uint32x4_t v) { const uint16x8_t m = { 1, 1<<2, 1<<4, 1<<6, 1<<8, 1<<10, 1<<12, 1<<14}; return vaddvq_u16(vandq_u16((uint16x8_t)v, m)); }
static ALWAYS_INLINE uint16_t  mm_movemask_epu16(__m128i v) { const uint16x8_t m = { 1, 1<<1, 1<<2, 1<<3, 1<<4, 1<< 5, 1<< 6, 1<<7 }; return vaddvq_u16(vandq_u16((uint16x8_t)v, m)); } 
static ALWAYS_INLINE uint32_t  mm_movemask_epu32(__m128i v) { const uint32x4_t m = { 1, 1<<1, 1<<2, 1<<3 };                           return vaddvq_u32(vandq_u32((uint32x4_t)v, m)); } 
static ALWAYS_INLINE uint64_t  mm_movemask_epu64(__m128i v) { const uint64x2_t m = { 1, 1<<1 };                                       return vaddvq_u64(vandq_u64((uint64x2_t)v, m)); }
    #else
static ALWAYS_INLINE uint32_t  mm_movemask_epu32(uint32x4_t v) { const uint32x4_t mask = {1,2,4,8}, av = vandq_u32(v, mask), xv = vextq_u32(av, av, 2), ov = vorrq_u32(av, xv); return vgetq_lane_u32(vorrq_u32(ov, vextq_u32(ov, ov, 3)), 0); }
    #endif
// --------------------------------------------- Swizzle : _mm_shuffle_epi8 / _mm_shuffle_epi32 / Pack/Unpack -----------------------------------------
#define _MM_SHUFFLE(u3,u2,u1,u0) 				((u3) << 6 | (u2) << 4 | (u1) << 2 | (u0))

#define _mm_shuffle_epi8(_a_, _b_) 				(__m128i)vqtbl1q_u8((uint8x16_t)(_a_), (uint8x16_t)(_b_))
    #if defined(__aarch64__)
#define  mm_shuffle_nnnn_epi32(_a_,_m_)     	(__m128i)vdupq_laneq_u32(_a_, _m_)
    #else
#define  mm_shuffle_nnnn_epi32(_a_,_m_)         (__m128i)vdupq_n_u32(vgetq_lane_u32(_a_, _m_)
    #endif

  #ifdef USE_MACROS
#define mm_shuffle_2031_epi32(_a_) 				({ uint32x4_t _zv = (uint32x4_t)vrev64q_u32(_a_); uint32x2x2_t _zv = vtrn_u32(vget_low_u32(_zv), vget_high_u32(_zv)); vcombine_u32(_zv.val[0], _zv.val[1]);})
#define mm_shuffle_3120_epi32(_a_) 				({ uint32x4_t _zv = _a_;                                       _zv = vtrn_u32(vget_low_u32(_zv), vget_high_u32(_zv)); vcombine_u32(_zv.val[0], _zv.val[1]);})
  #else
static ALWAYS_INLINE __m128i mm_shuffle_2031_epi32(__m128i a) { uint32x4_t   v = (uint32x4_t)vrev64q_u32(a); uint32x2x2_t z = vtrn_u32(vget_low_u32(v), vget_high_u32(v)); return vcombine_u32(z.val[0], z.val[1]);}
static ALWAYS_INLINE __m128i mm_shuffle_3120_epi32(__m128i a) {                                              uint32x2x2_t z = vtrn_u32(vget_low_u32(a), vget_high_u32(a)); return vcombine_u32(z.val[0], z.val[1]);}
  #endif

  #if defined(USE_MACROS) || defined(__clang__)
#define _mm_shuffle_epi32(_a_, _m_)             ({ const uint32x4_t _av =_a_;\
                                                   uint32x4_t _v =    vmovq_n_u32(vgetq_lane_u32(_av, (_m_)        & 0x3));\
                                                              _v = vsetq_lane_u32(vgetq_lane_u32(_av, ((_m_) >> 2) & 0x3), _v, 1);\
                                                              _v = vsetq_lane_u32(vgetq_lane_u32(_av, ((_m_) >> 4) & 0x3), _v, 2);\
                                                              _v = vsetq_lane_u32(vgetq_lane_u32(_av, ((_m_) >> 6) & 0x3), _v, 3); _v;\
                                                })
#define _mm_shuffle_epi32s(_a_, _m_) 			_mm_set_epi32(vgetq_lane_u32(_a_, ((_m_)     ) & 0x3),\
															  vgetq_lane_u32(_a_, ((_m_) >> 2) & 0x3),\
															  vgetq_lane_u32(_a_, ((_m_) >> 4) & 0x3),\
															  vgetq_lane_u32(_a_, ((_m_) >> 6) & 0x3))
  #else
static ALWAYS_INLINE __m128i _mm_shuffle_epi32(__m128i _a_, const unsigned _m_) { const uint32x4_t _av =_a_;
  uint32x4_t _v =    vmovq_n_u32(vgetq_lane_u32(_av, (_m_)        & 0x3));
  _v = vsetq_lane_u32(vgetq_lane_u32(_av, ((_m_) >> 2) & 0x3), _v, 1);
  _v = vsetq_lane_u32(vgetq_lane_u32(_av, ((_m_) >> 4) & 0x3), _v, 2);
  _v = vsetq_lane_u32(vgetq_lane_u32(_av, ((_m_) >> 6) & 0x3), _v, 3); 
  return _v;
}
static ALWAYS_INLINE __m128i _mm_shuffle_epi32s(__m128i _a_, const unsigned _m_) { 
  return _mm_set_epi32(vgetq_lane_u32(_a_, ((_m_)     ) & 0x3),
	                   vgetq_lane_u32(_a_, ((_m_) >> 2) & 0x3),
		 	           vgetq_lane_u32(_a_, ((_m_) >> 4) & 0x3),
			           vgetq_lane_u32(_a_, ((_m_) >> 6) & 0x3));
}
  #endif
#ifdef USE_MACROS
#define _mm_unpacklo_epi8( _a_,_b_)            	({ uint8x8x2_t  _zv = vzip_u8 ( vget_low_u8( (uint8x16_t)(_a_)), vget_low_u8 ((uint8x16_t)(_b_))); (uint32x4_t)vcombine_u8( _zv.val[0], _zv.val[1]);})
#define _mm_unpacklo_epi16(_a_,_b_)            	({ uint16x4x2_t _zv = vzip_u16( vget_low_u16((uint16x8_t)(_a_)), vget_low_u16((uint16x8_t)(_b_))); (uint32x4_t)vcombine_u16(_zv.val[0], _zv.val[1]);})
#define _mm_unpacklo_epi32(_a_,_b_)             ({ uint32x2x2_t _zv = vzip_u32( vget_low_u32(             _a_ ), vget_low_u32(             _b_ ));             vcombine_u32(_zv.val[0], _zv.val[1]);})
#define _mm_unpacklo_epi64(_a_,_b_)             (uint32x4_t)vcombine_u64(vget_low_u64((uint64x2_t)(_a_)), vget_low_u64((uint64x2_t)(_b_)))

#define _mm_unpackhi_epi8( _a_,_b_)            	({ uint8x8x2_t  _zv = vzip_u8 (vget_high_u8( (uint8x16_t)(_a_)), vget_high_u8( (uint8x16_t)(_b_))); (uint32x4_t)vcombine_u8( _zv.val[0], _zv.val[1]);})
#define _mm_unpackhi_epi16(_a_,_b_)            	({ uint16x4x2_t _zv = vzip_u16(vget_high_u16((uint16x8_t)(_a_)), vget_high_u16((uint16x8_t)(_b_))); (uint32x4_t)vcombine_u16(_zv.val[0], _zv.val[1]);})
#define _mm_unpackhi_epi32(_a_,_b_)             ({ uint32x2x2_t _zv = vzip_u32(vget_high_u32(             _a_ ), vget_high_u32(             _b_ ));             vcombine_u32(_zv.val[0], _zv.val[1]);})
#define _mm_unpackhi_epi64(_a_,_b_)             (uint32x4_t)vcombine_u64(vget_high_u64((uint64x2_t)(_a_)), vget_high_u64((uint64x2_t)(_b_)))
#else
static ALWAYS_INLINE __m128i _mm_unpacklo_epi8( __m128i _a_, __m128i _b_) { uint8x8x2_t  _zv = vzip_u8 ( vget_low_u8( (uint8x16_t)(_a_)), vget_low_u8 ((uint8x16_t)(_b_)));  return (uint32x4_t)vcombine_u8( _zv.val[0], _zv.val[1]);}
static ALWAYS_INLINE __m128i _mm_unpacklo_epi16(__m128i _a_, __m128i _b_) { uint16x4x2_t _zv = vzip_u16( vget_low_u16((uint16x8_t)(_a_)), vget_low_u16((uint16x8_t)(_b_)));  return (uint32x4_t)vcombine_u16(_zv.val[0], _zv.val[1]);}
static ALWAYS_INLINE __m128i _mm_unpacklo_epi32(__m128i _a_, __m128i _b_) { uint32x2x2_t _zv = vzip_u32( vget_low_u32(             _a_ ), vget_low_u32(             _b_ ));  return             vcombine_u32(_zv.val[0], _zv.val[1]);}
static ALWAYS_INLINE __m128i _mm_unpacklo_epi64(__m128i _a_, __m128i _b_) {                                                                                                  return (uint32x4_t)vcombine_u64(vget_low_u64((uint64x2_t)(_a_)), vget_low_u64((uint64x2_t)(_b_))); }

static ALWAYS_INLINE __m128i _mm_unpackhi_epi8( __m128i _a_, __m128i _b_) { uint8x8x2_t  _zv = vzip_u8 (vget_high_u8( (uint8x16_t)(_a_)), vget_high_u8( (uint8x16_t)(_b_))); return (uint32x4_t)vcombine_u8( _zv.val[0], _zv.val[1]); }
static ALWAYS_INLINE __m128i _mm_unpackhi_epi16(__m128i _a_, __m128i _b_) { uint16x4x2_t _zv = vzip_u16(vget_high_u16((uint16x8_t)(_a_)), vget_high_u16((uint16x8_t)(_b_))); return (uint32x4_t)vcombine_u16(_zv.val[0], _zv.val[1]); }
static ALWAYS_INLINE __m128i _mm_unpackhi_epi32(__m128i _a_, __m128i _b_) { uint32x2x2_t _zv = vzip_u32(vget_high_u32(             _a_ ), vget_high_u32(             _b_ )); return             vcombine_u32(_zv.val[0], _zv.val[1]); }
static ALWAYS_INLINE __m128i _mm_unpackhi_epi64(__m128i _a_, __m128i _b_) {                                                                                                  return (uint32x4_t)vcombine_u64(vget_high_u64((uint64x2_t)(_a_)), vget_high_u64((uint64x2_t)(_b_))); }
#endif

  #else //------------------------------------- intel SSE2/SSSE3 --------------------------------------------------------------
#define mm_movemask_epu32(_a_)                  _mm_movemask_ps(_mm_castsi128_ps(_a_))
#define mm_movemask_epu16(_a_)                  _mm_movemask_epi8(_a_)
#define mm_loadu_epi64p( _u64p_,_a_)            _a_ = _mm_cvtsi64_si128(ctou64(_u64p_))

#define mm_extract_epu32( _a_,       _id_)      _mm_extract_epi32(_a_, _id_) 
#define mm_extract_epi32x(_a_,_u32_, _id_)      _u32_ = _mm_extract_epi32(_a_, _id_) 
#define mm_extract_epi64x(_a_,_u64_, _id_)      _u64_ = _mm_extract_epi64(_a_, _id_) 
#define mm_insert_epi32p( _a_,_u32p_,_c_)       _mm_insert_epi32( _a_,ctou32(_u32p_),_c_)  

#define mm_mullo_epu32(   _a_,_b_)   			_mm_mullo_epi32(_a_,_b_)
#define mm_cvtsi64_si128p(_u64p_,_a_)           _a_ = _mm_cvtsi64_si128(ctou64(_u64p_))

#define mm_cmpgt_epu32(   _a_, _b_)             _mm_cmpgt_epi32(_mm_xor_si128(_a_, cv80000000), _mm_xor_si128(_b_, cv80000000))

#define mm_shuffle_nnnn_epi32(_a_, _n_)         _mm_shuffle_epi32(_a_, _MM_SHUFFLE(_n_,_n_,_n_,_n_))
#define mm_shuffle_2031_epi32(_a_)              _mm_shuffle_epi32(_a_, _MM_SHUFFLE(2,0,3,1))
#define mm_shuffle_3120_epi32(_a_)              _mm_shuffle_epi32(_a_, _MM_SHUFFLE(3,1,2,0))

static ALWAYS_INLINE __m128i mm_rbit_epi8(__m128i v) { // reverse bits in bytes
  __m128i fv     = _mm_set_epi8(15, 7,11, 3,13, 5, 9, 1,14, 6,10, 2,12, 4, 8, 0), cv0f_8 = _mm_set1_epi8(0xf);	  
  __m128i lv = _mm_shuffle_epi8(fv,_mm_and_si128(               v,     cv0f_8));
  __m128i hv = _mm_shuffle_epi8(fv,_mm_and_si128(_mm_srli_epi64(v, 4), cv0f_8));
  return _mm_or_si128(_mm_slli_epi64(lv,4), hv);
}

static ALWAYS_INLINE __m128i mm_rev_epi16(__m128i v) { return _mm_shuffle_epi8(v, _mm_set_epi8(14,15,12,13,10,11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1)); } // reverse vector bytes in uint??_t
static ALWAYS_INLINE __m128i mm_rev_epi32(__m128i v) { return _mm_shuffle_epi8(v, _mm_set_epi8(12,13,14,15, 8, 9,10,11, 4, 5, 6, 7, 0, 1, 2, 3)); }
static ALWAYS_INLINE __m128i mm_rev_epi64(__m128i v) { return _mm_shuffle_epi8(v, _mm_set_epi8( 8, 9,10,11,12,13,14,15, 0, 1, 2, 3, 4, 5, 6, 7)); }
static ALWAYS_INLINE __m128i mm_rev_si128(__m128i v) { return _mm_shuffle_epi8(v, _mm_set_epi8( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15)); }
  #endif
#endif
