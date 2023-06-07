/**
    Copyright (C) powturbo 2016-2023
    GPL v3 License

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
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

// conf.h - config & common
#ifndef CONF_H_
#define CONF_H_
#if defined(_MSC_VER) && (_MSC_VER < 1600)
  #if !defined(_STDINT) && !defined(_MSC_STDINT_H_)
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
  #endif
#else
#include <stdint.h>
#endif
#include <stddef.h>

//------------------------- Compiler ------------------------------------------
  #if defined(__GNUC__)
#include <stdint.h>
#define ALIGNED(t,v,n)  t v __attribute__ ((aligned (n)))
#define ALWAYS_INLINE   inline __attribute__((always_inline))
#define NOINLINE        __attribute__((noinline))
#define _PACKED         __attribute__ ((packed))
#define likely(x)       __builtin_expect((x),1)
#define unlikely(x)     __builtin_expect((x),0)

//#define bswap8(x)    (x)
    #if __GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 8
#define bswap16(x) __builtin_bswap16(x)
    #else
static ALWAYS_INLINE unsigned short bswap16(unsigned short x) { return __builtin_bswap32(x << 16); }
    #endif
#define bswap32(x) __builtin_bswap32(x)
#define bswap64(x) __builtin_bswap64(x)

#define popcnt32(_x_)   __builtin_popcount(_x_)
#define popcnt64(_x_)   __builtin_popcountll(_x_)

    #if defined(__i386__) || defined(__x86_64__)
//x,__bsr32:     1:0,2:1,3:1,4:2,5:2,6:2,7:2,8:3,9:3,10:3,11:3,12:3,13:3,14:3,15:3,16:4,17:4,18:4,19:4,20:4,21:4,22:4,23:4,24:4,25:4,26:4,27:4,28:4,29:4,30:4,31:4,32:5,...
//x,  bsr32: 0:0,1:1,2:2,3:2,4:3,5:3,6:3,7:3,8:4,9:4,10:4,11:4,12:4,13:4,14:4,15:4,16:5,17:5,18:5,19:5,20:5,21:5,22:5,23:5,24:5,25:5,26:5,27:5,28:5,29:5,30:5,31:5,32:6,...
static ALWAYS_INLINE int    __bsr32(               int x) {             asm("bsr  %1,%0" : "=r" (x) : "rm" (x) ); return x; }
static ALWAYS_INLINE int      bsr32(               int x) { int b = -1; asm("bsrl %1,%0" : "+r" (b) : "rm" (x) ); return b + 1; }
static ALWAYS_INLINE int      bsr64(uint64_t x          ) { return x?64 - __builtin_clzll(x):0; }
static ALWAYS_INLINE int    __bsr64(uint64_t x          ) { return   63 - __builtin_clzll(x);   }

static ALWAYS_INLINE unsigned rol32(unsigned x, int s) { asm ("roll %%cl,%0" :"=r" (x) :"0" (x),"c" (s)); return x; }
static ALWAYS_INLINE unsigned ror32(unsigned x, int s) { asm ("rorl %%cl,%0" :"=r" (x) :"0" (x),"c" (s)); return x; }
static ALWAYS_INLINE uint64_t rol64(uint64_t x, int s) { asm ("rolq %%cl,%0" :"=r" (x) :"0" (x),"c" (s)); return x; }
static ALWAYS_INLINE uint64_t ror64(uint64_t x, int s) { asm ("rorq %%cl,%0" :"=r" (x) :"0" (x),"c" (s)); return x; }
    #else
static ALWAYS_INLINE int    __bsr32(unsigned x          ) { return   31 - __builtin_clz(  x); }
static ALWAYS_INLINE int      bsr32(int x               ) { return x?32 - __builtin_clz(  x):0; }
static ALWAYS_INLINE int      bsr64(uint64_t x) { return x?64 - __builtin_clzll(x):0; }
static ALWAYS_INLINE int    __bsr64(uint64_t x          ) { return   63 - __builtin_clzll(x);   }

static ALWAYS_INLINE unsigned rol32(unsigned x, int s) { return x << s | x >> (32 - s); }
static ALWAYS_INLINE unsigned ror32(unsigned x, int s) { return x >> s | x << (32 - s); }
static ALWAYS_INLINE unsigned rol64(unsigned x, int s) { return x << s | x >> (64 - s); }
static ALWAYS_INLINE unsigned ror64(unsigned x, int s) { return x >> s | x << (64 - s); }
    #endif

#define ctz64(_x_) __builtin_ctzll(_x_)
#define ctz32(_x_) __builtin_ctz(_x_)    // 0:32  ctz32(1<<a) = a (a=1..31)
#define clz64(_x_) __builtin_clzll(_x_)
#define clz32(_x_) __builtin_clz(_x_)    // 00000000 00000000 00000000 01000000 = 25

  #elif _MSC_VER //----------------------------------------------------
#include <windows.h>
#include <intrin.h>
    #if _MSC_VER < 1600
#include "vs/stdint.h"
#define __builtin_prefetch(x,a)
#define inline          __inline
    #else
#include <stdint.h>
#define __builtin_prefetch(x,a) _mm_prefetch(x, _MM_HINT_NTA)
    #endif

#define ALIGNED(t,v,n)  __declspec(align(n)) t v
#define ALWAYS_INLINE   __forceinline
#define NOINLINE        __declspec(noinline)
#define THREADLOCAL     __declspec(thread)
#define likely(x)       (x)
#define unlikely(x)     (x)

static ALWAYS_INLINE int __bsr32(unsigned x) { unsigned long z=0; _BitScanReverse(&z, x); return z; }
static ALWAYS_INLINE int bsr32(  unsigned x) { unsigned long z;   _BitScanReverse(&z, x); return x?z+1:0; }
static ALWAYS_INLINE int ctz32(  unsigned x) { unsigned long z;   _BitScanForward(&z, x); return x?z:32; }
static ALWAYS_INLINE int clz32(  unsigned x) { unsigned long z;   _BitScanReverse(&z, x); return x?31-z:32; }
  #if !defined(_M_ARM64) && !defined(_M_X64)
static ALWAYS_INLINE unsigned char _BitScanForward64(unsigned long* ret, uint64_t x) {
  unsigned long x0 = (unsigned long)x, top, bottom;         _BitScanForward(&top, (unsigned long)(x >> 32)); _BitScanForward(&bottom, x0);
  *ret = x0 ? bottom : 32 + top;  return x != 0;
}
static unsigned char _BitScanReverse64(unsigned long* ret, uint64_t x) {
  unsigned long x1 = (unsigned long)(x >> 32), top, bottom; _BitScanReverse(&top, x1);                       _BitScanReverse(&bottom, (unsigned long)x);
  *ret = x1 ? top + 32 : bottom;  return x != 0;
}
  #endif
static ALWAYS_INLINE int __bsr64(uint64_t x) { unsigned long z = 0; _BitScanReverse64(&z, x); return z; }
static ALWAYS_INLINE int bsr64(uint64_t x) { unsigned long z=0; _BitScanReverse64(&z, x); return x?z+1:0; }
static ALWAYS_INLINE int ctz64(uint64_t x) { unsigned long z;   _BitScanForward64(&z, x); return x?z:64; }
static ALWAYS_INLINE int clz64(uint64_t x) { unsigned long z;   _BitScanReverse64(&z, x); return x?63-z:64; }

#define rol32(x,s) _lrotl(x, s)
#define ror32(x,s) _lrotr(x, s)

#define bswap16(x) _byteswap_ushort(x)
#define bswap32(x) _byteswap_ulong(x)
#define bswap64(x) _byteswap_uint64(x)

#define popcnt32(x) __popcnt(x)
  #ifdef _WIN64
#define popcnt64(x) __popcnt64(x)
  #else
#define popcnt64(x) (popcnt32(x) + popcnt32(x>>32))
  #endif

#define sleep(x)     Sleep(x/1000)
#define fseeko       _fseeki64
#define ftello       _ftelli64
#define strcasecmp   _stricmp
#define strncasecmp  _strnicmp
#define strtoull     _strtoui64
static ALWAYS_INLINE double round(double num) { return (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5); }
  #endif

#define __bsr8(_x_)  __bsr32(_x_)
#define __bsr16(_x_) __bsr32(_x_)
#define bsr8(_x_)    bsr32(_x_)
#define bsr16(_x_)   bsr32(_x_)
#define ctz8(_x_)    ctz32((_x_)+(1<< 8))
#define ctz16(_x_)   ctz32((_x_)+(1<<16))
#define clz8(_x_)    (clz32(_x_)-24)
#define clz16(_x_)   (clz32(_x_)-16)

#define popcnt8(x)   popcnt32(x)
#define popcnt16(x)  popcnt32(x)

//--------------- Unaligned memory access -------------------------------------
  #ifdef UA_MEMCPY
#include <string.h>
static ALWAYS_INLINE unsigned short     ctou16(const void *cp) { unsigned short     x; memcpy(&x, cp, sizeof(x)); return x; } // ua read
static ALWAYS_INLINE unsigned           ctou32(const void *cp) { unsigned           x; memcpy(&x, cp, sizeof(x)); return x; }
static ALWAYS_INLINE unsigned long long ctou64(const void *cp) { unsigned long long x; memcpy(&x, cp, sizeof(x)); return x; }
static ALWAYS_INLINE size_t             ctousz(const void *cp) { size_t             x; memcpy(&x, cp, sizeof(x)); return x; }
static ALWAYS_INLINE float              ctof32(const void *cp) { float              x; memcpy(&x, cp, sizeof(x)); return x; }
static ALWAYS_INLINE double             ctof64(const void *cp) { double             x; memcpy(&x, cp, sizeof(x)); return x; }

static ALWAYS_INLINE void               stou16(      void *cp, unsigned short     x) { memcpy(cp, &x, sizeof(x)); } // ua write
static ALWAYS_INLINE void               stou32(      void *cp, unsigned           x) { memcpy(cp, &x, sizeof(x)); }
static ALWAYS_INLINE void               stou64(      void *cp, unsigned long long x) { memcpy(cp, &x, sizeof(x)); }
static ALWAYS_INLINE void               stousz(      void *cp, size_t             x) { memcpy(cp, &x, sizeof(x)); }
static ALWAYS_INLINE void               stof32(      void *cp, float              x) { memcpy(cp, &x, sizeof(x)); }
static ALWAYS_INLINE void               stof64(      void *cp, double             x) { memcpy(cp, &x, sizeof(x)); }

static ALWAYS_INLINE void               ltou32(unsigned           *x, const void *cp) { memcpy(x, cp, sizeof(*x)); } // ua read into ptr 
static ALWAYS_INLINE void               ltou64(unsigned long long *x, const void *cp) { memcpy(x, cp, sizeof(*x)); }

  #elif defined(__i386__) || defined(__x86_64__) || \
    defined(_M_IX86) || defined(_M_AMD64) || _MSC_VER ||\
    defined(__powerpc__) || defined(__s390__) ||\
    defined(__ARM_FEATURE_UNALIGNED) || defined(__aarch64__) || defined(__arm__) ||\
    defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__) || \
    defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || \
    defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__)  || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6Z__)   || defined(__ARM_ARCH_6ZK__)
#define ctou16(_cp_) (*(unsigned short *)(_cp_))
#define ctou32(_cp_) (*(unsigned       *)(_cp_))
#define ctof32(_cp_) (*(float          *)(_cp_))

#define stou16(_cp_, _x_)  (*(unsigned short *)(_cp_) = _x_)
#define stou32(_cp_, _x_)  (*(unsigned       *)(_cp_) = _x_)
#define stof32(_cp_, _x_)  (*(float          *)(_cp_) = _x_)

#define ltou32(_px_, _cp_) *(_px_) = *(unsigned *)(_cp_)

    #if defined(__i386__) || defined(__x86_64__) || defined(__powerpc__) || defined(__s390__) || defined(_MSC_VER)
#define ctou64(_cp_)       (*(uint64_t *)(_cp_))
#define ctof64(_cp_)       (*(double   *)(_cp_))

#define stou64(_cp_, _x_)  (*(uint64_t *)(_cp_) = _x_)
#define stof64(_cp_, _x_)  (*(double   *)(_cp_) = _x_)

#define ltou64(_px_, _cp_) *(_px_) = *(uint64_t *)(_cp_)

    #elif defined(__ARM_FEATURE_UNALIGNED)
struct _PACKED longu     { uint64_t l; };
struct _PACKED doubleu   { double   d; };
#define ctou64(_cp_) ((struct longu     *)(_cp_))->l
#define ctof64(_cp_) ((struct doubleu   *)(_cp_))->d

#define stou64(_cp_) ((struct longu     *)(_cp_))->l = _x_
#define stof64(_cp_) ((struct doubleu   *)(_cp_))->d = _x_
#define ltou64(_px_, _cp_) *(_px_) = ((struct longu *)(_cp_))->l
    #endif

  #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7S__)
struct _PACKED shortu    { unsigned short     s; };
struct _PACKED unsignedu { unsigned           u; };
struct _PACKED longu     { uint64_t           l; };
struct _PACKED floatu    { float              f; };
struct _PACKED doubleu   { double             d; };

#define ctou16(_cp_) ((struct shortu    *)(_cp_))->s
#define ctou32(_cp_) ((struct unsignedu *)(_cp_))->u
#define ctou64(_cp_) ((struct longu     *)(_cp_))->l
#define ctof32(_cp_) ((struct floatu    *)(_cp_))->f
#define ctof64(_cp_) ((struct doubleu   *)(_cp_))->d

#define stou16(_cp_, _x_) ((struct shortu    *)(_cp_))->s = _x_
#define stou32(_cp_, _x_) ((struct unsignedu *)(_cp_))->u = _x_
#define stou64(_cp_, _x_) ((struct longu     *)(_cp_))->l = _x_
#define stof32(_cp_, _x_) ((struct floatu    *)(_cp_))->f = _x_
#define stof64(_cp_, _x_) ((struct doubleu   *)(_cp_))->d = _x_

#define ltou32(_cp_) *(_px_) = ((struct unsignedu *)(_cp_))->u
#define ltou64(_cp_) *(_px_) = ((struct longu *)(_cp_))->l
  #else
#error "unknown cpu"
  #endif

#define ctou24(_cp_) (ctou32(_cp_) & 0xffffff)
#define ctou48(_cp_) (ctou64(_cp_) & 0xffffffffffffull)
#define ctou8(_cp_) (*(_cp_))
//--------------------- wordsize ----------------------------------------------
  #if defined(__64BIT__) || defined(_LP64) || defined(__LP64__) || defined(_WIN64) ||\
    defined(__x86_64__) || defined(_M_X64) ||\
    defined(__ia64) || defined(_M_IA64) ||\
    defined(__aarch64__) ||\
    defined(__mips64) ||\
    defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) ||\
    defined(__s390x__)
#define __WORDSIZE 64
  #else
#define __WORDSIZE 32
  #endif
#endif

//---------------------misc ---------------------------------------------------
#define BZMASK64(_b_)                    (~(~0ull << (_b_)))
#define BZMASK32(_b_)                    (~(~0u   << (_b_)))
#define BZMASK16(_b_)                    BZMASK32(_b_)
#define BZMASK8( _b_)                    BZMASK32(_b_)

#define BZHI64(_u_, _b_)                 ((_u_) & BZMASK64(_b_))  // b Constant
#define BZHI32(_u_, _b_)                 ((_u_) & BZMASK32(_b_)) 
#define BZHI16(_u_, _b_)                 BZHI32(_u_, _b_)
#define BZHI8( _u_, _b_)                 BZHI32(_u_, _b_)
#define BEXTR32(x,start,len)             (((x) >> (start)) & ((1u << (len)) - 1)) //Bit field extract (with register)

    #ifdef __AVX2__
      #if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#include <intrin.h>
      #else
#include <x86intrin.h>
      #endif
#define bzhi32(_u_, _b_)                 _bzhi_u32(_u_, _b_)  // b variable
#define bextr32(x,start,len)             _bextr_u32(x,start,len)  

      #if !(defined(_M_X64) || defined(__amd64__)) && (defined(__i386__) || defined(_M_IX86))
#define bzhi64(_u_, _b_)                 BZHI64(_u_, _b_)
      #else
#define bzhi64(_u_, _b_)                 _bzhi_u64(_u_, _b_)
      #endif
    #else
#define bzhi64(_u_, _b_)                 BZHI64(_u_, _b_) 
#define bzhi32(_u_, _b_)                 BZHI32(_u_, _b_)
#define bextr32(x,start,len)             (((x) >> (start)) & ((1u << (len)) - 1)) //Bit field extract (with register)
    #endif

#define bzhi16(_u_, _b_)                 bzhi32(_u_, _b_)
#define bzhi8( _u_, _b_)                 bzhi32(_u_, _b_)

#define SIZE_ROUNDUP(_n_, _a_) (((size_t)(_n_) + (size_t)((_a_) - 1)) & ~(size_t)((_a_) - 1))
#define ALIGN_DOWN(__ptr, __a) ((void *)((uintptr_t)(__ptr) & ~(uintptr_t)((__a) - 1)))

#define T2_(_x_, _y_) _x_##_y_
#define T2(_x_, _y_) T2_(_x_,_y_)

#define T3_(_x_,_y_,_z_) _x_##_y_##_z_
#define T3(_x_,_y_,_z_) T3_(_x_, _y_, _z_)

#define CACHE_LINE_SIZE     64
#define PREFETCH_DISTANCE   (CACHE_LINE_SIZE*4)

#define CLAMP(_x_, _low_, _high_)  (((_x_) > (_high_)) ? (_high_) : (((_x_) < (_low_)) ? (_low_) : (_x_)))

//--- NDEBUG -------
#include <stdio.h>
  #ifdef _MSC_VER
    #ifdef NDEBUG
#define AS(expr, fmt, ...)
#define AC(expr, fmt, ...) do { if(!(expr)) { fprintf(stderr, fmt, ##__VA_ARGS__ ); fflush(stderr); exit(-1); } } while(0)
#define die(fmt, ...) do { fprintf(stderr, fmt, ##__VA_ARGS__ ); fflush(stderr); exit(-1); } while(0)
    #else
#define AS(expr, fmt, ...) do { if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ##__VA_ARGS__ ); fflush(stderr); exit(-1); } } while(0)
#define AC(expr, fmt, ...) do { if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ##__VA_ARGS__ ); fflush(stderr); exit(-1); } } while(0)
#define die(fmt, ...) do { fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ##__VA_ARGS__ ); fflush(stderr); exit(-1); } while(0)
    #endif
  #else
    #ifdef NDEBUG
#define AS(expr, fmt,args...)
#define AC(expr, fmt,args...) do { if(!(expr)) { fprintf(stderr, fmt, ## args ); fflush(stderr); exit(-1); } } while(0)
#define die(fmt,args...) do { fprintf(stderr, fmt, ## args ); fflush(stderr); exit(-1); } while(0)
    #else
#define AS(expr, fmt,args...) do { if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); exit(-1); } } while(0)
#define AC(expr, fmt,args...) do { if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); exit(-1); } } while(0)
#define die(fmt,args...) do { fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); exit(-1); } while(0)
    #endif
  #endif
//-- Turbo VLC : Novel Variable Length Coding for large integers with exponent + mantissa
#include "conf.h"

// Exponent base for the bit size vlcbits: 1=63 2=123, 3=239 4=463 5=895 6=1727 7=3327
#define VLC_VN6    ( 6-5)
#define VLC_VN7    ( 7-5)
#define VLC_VN8    ( 8-5)
#define VLC_VN9    ( 9-5)
#define VLC_VN10   (10-5)
#define VLC_VN11   (11-5)
#define VLC_VN12   (12-5)
#define VLC_VN15   (15-5)
#define VLC_VN16   (16-5)

#define VLC_VB6    0
#define VLC_VB7    4
#define VLC_VB8   16
#define VLC_VB9   48
#define VLC_VB10 128
#define VLC_VB11 296
#define VLC_VB12 768
#define VLC_VB16 768 //???

#define vlcbits(_vn_)               (5+_vn_)
#define vlcfirst(_vn_)              (1u<<(_vn_+1)) //1,0,4  2,4,8  3,16,16
#define vlcmbits(_expo_, _vn_)      (((_expo_) >> _vn_)-1)
#define _vlcexpo_(_x_, _vn_,_expo_) { unsigned _f = __bsr32(_x_)-_vn_; _expo_ = ((_f+1)<<_vn_) + bextr32((_x_),_f,_vn_); }
#ifndef VLCBIT_H_
#define VLCBIT_H_
static inline int vlcexpo(unsigned x, unsigned vn) { unsigned expo; _vlcexpo_(x, vn, expo); return expo; }
#endif

// return exponent, mantissa + bits length for x the value
#define vlcenc( _x_, _vn_,_expo_, _mb_, _ma_) { unsigned _x = _x_; _vlcexpo_(_x, _vn_, _expo_); _mb_ = vlcmbits(_expo_, _vn_); _ma_ = bzhi32(_x,_mb_); }

// build value from exponent, mantissa + length
#define vlcdec(_expo_, _mb_, _ma_, _vn_) ((((1u << _vn_) + bzhi32(_expo_,_vn_))<<(_mb_)) + (_ma_))

// encode the mantissa in bitio (R->L) and return the exponent in u
#define bitvrput(_bw_,_br_,_ep_, _vn_,_vb_, _u_) do { \
  if((_u_) >= vlcfirst(_vn_)+_vb_) {\
    unsigned _expo, _mb, _ma;\
    vlcenc((_u_)-_vb_, _vn_, _expo, _mb, _ma); \
    bitput(_bw_,_br_, _mb, _ma); bitenormr(_bw_,_br_,_ep_);\
	_u_ = _expo+_vb_;\
  }\
} while(0)

// get mantissa and bitio (R->L) decode value from x
/*#define bitvrget( _bw_,_br_,_ip_, _vn_,_vb_,_x_) \
  if(_x_ >= vlcfirst(_vn_)+_vb_) { \
    _x_ -= _vb_; \
	int _mb = vlcmbits(_x_, _vn_), _ma; \
    bitdnormr(_bw_,_br_,_ip_);\
	bitget(_bw_,_br_, _mb,_ma);\
	_x_ = vlcdec(_x_, _mb, _ma, _vn_)+_vb_;\
  }*/
#define bitvrget(_bw_,_br_,_ip_,_vn_,_vb_,_x_) \
  if(_x_ >= vlcfirst(_vn_)+_vb_) { \
    bitdnormr(_bw_,_br_,_ip_); \
    unsigned _mb = vlcmbits(_x_ -= _vb_, _vn_), _ma = bitpeek(_bw_,_br_, _mb); /*bitget(_bw_,_br_, _mb, _ma);*/\
    _x_ = vlcdec(_x_, _mb, _ma, _vn_)+_vb_; bitrmv(_bw_,_br_, _mb);\
  }

//--------- Expo & mantissa with bitio ------------------------------
#define bitvput(_bw_,_br_,_vn_,_vb_, _u_) do { \
  if((_u_) >= vlcfirst(_vn_)+_vb_) {\
    unsigned _expo, _mb, _ma;\
    vlcenc((_u_)-_vb_, _vn_, _expo, _mb, _ma); \
	bitput(_bw_,_br_, vlcbits(_vn_), _expo+_vb_); \
    bitput(_bw_,_br_, _mb, _ma); \
  } else bitput(_bw_,_br_, vlcbits(_vn_), _u_);\
} while(0)

#define bitvget( _bw_,_br_, _vn_,_vb_,_x_) do {\
  bitget(_bw_,_br_, vlcbits(_vn_),_x_);\
  if(_x_ >= vlcfirst(_vn_)+_vb_) { \
    _x_ -= _vb_; \
	int _mb = vlcmbits(_x_, _vn_), _ma; \
 	bitget(_bw_,_br_, _mb,_ma);\
	_x_ = vlcdec(_x_, _mb, _ma, _vn_)+_vb_;\
  }\
} while(0)

//--------- Branchless, expo with byte io, mantissa with bitio ---------------
#define bitvbput(_bw_,_br_,_cp_,_bp_,_vn_,_vb_, _u_) do { \
  unsigned _expo, _mb, _ma;\
  vlcenc((_u_)+vlcfirst(_vn_), _vn_, _expo, _mb, _ma); \
  *_cp_++ = _expo; \
  bitput(_bw_,_br_, _mb, _ma); \
} while(0)

#define bitvbget(_bw_,_br_,_cp_,_bp_,_vn_,_vb_,_x_) do { _x_ = *_cp_++; \
  unsigned _mb = vlcmbits(_x_, _vn_), _ma;\
  _ma = bitpeek(_bw_, _br_, _mb); bitrmv(_bw_, _br_, _mb);\
  _x_ = vlcdec(_x_, _mb, _ma, _vn_)-vlcfirst(_vn_);\
} while(0)

//--
#define bitvcput(_bw_,_br_,_cp_,_bp_,_vn_,_vb_, _u_) do { \
  if((_u_) >= vlcfirst(_vn_)+_vb_) {\
    unsigned _expo, _mb, _ma;\
    vlcenc((_u_)-_vb_, _vn_, _expo, _mb, _ma); \
	*_cp_++ = _expo+_vb_; \
    bitput(_bw_,_br_, _mb, _ma); /*bitenormr(_bw_,_br_,_bp_);*/\
  } else *_cp_++ = _u_; \
} while(0)

#define bitvcget(_bw_,_br_,_cp_,_bp_,_vn_,_vb_,_x_) do { /*bitdnormr(_bw_,_br_,_bp_);*/ _x_ = *_cp_++; \
  if(likely(_x_ >= vlcfirst(_vn_)+_vb_)) {\
    _x_ -= _vb_;\
	int _mb = vlcmbits(_x_, _vn_), _ma; /*_ma = bitpeek(_bw_, _br_, _mb); bitrmv(_bw_,_br_, _mb);*/\
	bitget(_bw_,_br_, _mb,_ma);\
	_x_ = vlcdec(_x_, _mb, _ma, _vn_)+_vb_;\
  }\
} while(0)


#define vhifirst(_k_) (1<<(_k_))

#define vhimbits(_expo_,_k_,_i_,_j_) (_k_ - (_i_ + _j_) + ((_expo_ - (1<<_k_)) >> (_i_ + _j_)))
// Hybrid integer https://www.lucaversari.it/phd/main.pdf
#define VHI_K 4
#define VHI_I 2
#define VHI_J 1

//#define _vlcexpo_(_x_, _k_,_i_,_j, _expo_) 
#define vhienc(_x_, _k_, _i_, _j_, _expo_, _mb_, _ma_) {\
  unsigned n = __bsr32(_x_), m = _x_ - (1 << n);\
  _expo_ = (1<<_k_) + ((n - _k_) << (_i_ + _j_)) + ((m >> (n - _i_)) << _j_) + BZHI32(m,_j_);\
  _mb_   = n - _i_ - _j_;\
  _ma_   = bzhi32(_x_ >> _j_,_mb_);\
}

#define vhidec(_x_, _mb_, _ma_, _k_, _i_, _j_) {\
  unsigned _mb_ = vhimbits(_x_,_k_,_i_,_j_), low_bits = _x_ & ((1 << _j_) - 1);\
  _x_ >>= _j_;\
  unsigned high_bits = (1 << _i_) | (_x_ & ((1 << _i_) - 1));\
  _x_ = (((high_bits << _mb_) | _ma_) << _j_) | low_bits;\
}

#define bithcput(_bw_,_br_,_cp_,_bp_,_k_,_i_,_j_, _u_) do { \
  if((_u_) >= vhifirst(_k_)) {\
    unsigned _expo, _mb, _ma;\
    vhienc(_u_, _k_, _i_, _j_, _expo, _mb, _ma); \
	*_cp_++ = _expo; \
    bitput(_bw_,_br_, _mb, _ma); /*bitenormr(_bw_,_br_,_bp_);*/\
  } else *_cp_++ = _u_; \
} while(0)

#define bithcget(_bw_,_br_,_cp_,_bp_,_k_,_i_,_j_,_x_) do { /*bitdnormr(_bw_,_br_,_bp_);*/ _x_ = *_cp_++; \
  if(likely(_x_ >= vhifirst(_k_))) {\
    unsigned _mb = _k_ - (_i_ + _j_) + ((_x_ - (1<<_k_)) >> (_i_ + _j_)), _ma; bitget(_bw_,_br_, _mb,_ma);\
    _x_ = (((((1 << _i_) | BZHI32(_x_ >> _j_,_i_)) << _mb) | _ma) << _j_) | BZHI32(_x_,_j_);\
  }\
} while(0)

//---- "Integer Compression" scalar variable byte -------------------------------
#include "conf.h"
//----------------------------------- Variable byte: single value macros (low level) -----------------------------------------------
//------------- 32 bits -------------
extern unsigned char _vtab32_[];
#define _vbxvlen32(_x_) _vtab32_[(unsigned char)(_x_)>>4] // (clz32((_x_) ^ 0xff) - 23) //
#define _vbxlen32(_x_) ((bsr32(_x_|1)+6)/7)

#define _vbxput32(_op_, _x_, _act_) {\
       if(likely((_x_) < (1<< 7))) {        *_op_++ = _x_;                                               _act_;}\
  else if(likely((_x_) < (1<<14))) { ctou16(_op_)   = bswap16((_x_) | 0x8000u);               _op_ += 2; _act_;}\
  else if(likely((_x_) < (1<<21))) {        *_op_++ = _x_ >> 16  | 0xc0u; ctou16(_op_) = _x_; _op_ += 2; _act_;}\
  else if(likely((_x_) < (1<<28))) { ctou32(_op_)   = bswap32((_x_) | 0xe0000000u);              _op_ += 4; _act_;}\
  else                             {        *_op_++ = (unsigned long long)(_x_) >> 32 | 0xf0u; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
}

#define _vbxget32(_ip_, _x_, _act_) do { _x_ = (unsigned)(*_ip_++);\
       if(!(_x_ & 0x80u)) {                                                                         _act_;}\
  else if(!(_x_ & 0x40u)) { _x_ = bswap16(ctou16(_ip_ - 1) & 0xff3fu); _ip_++;                      _act_;}\
  else if(!(_x_ & 0x20u)) { _x_ = (_x_ & 0x1f)<<16 | ctou16(_ip_);                       _ip_ += 2; _act_;}\
  else if(!(_x_ & 0x10u)) { _x_ = bswap32(ctou32(_ip_-1) & 0xffffff0fu);                 _ip_ += 3; _act_;}\
  else                    { _x_ = (unsigned long long)((_x_) & 0x07)<<32 | ctou32(_ip_); _ip_ += 4; _act_;}\
} while(0)

//------------- 64 bits -----------
#define _vbxlen64(_x_)  ((bsr64(_x_)+6)/7)
#define _vbxvlen64(_x_) ((_x_)==0xff?9:clz32((_x_) ^ 0xff) - 23)

#define _vbxput64(_op_, _x_, _act_) {\
       if(likely(_x_ < (1<< 7))) {        *_op_++ = _x_;                                                                                          _act_;}\
  else if(likely(_x_ < (1<<14))) { ctou16(_op_)   =        bswap16(_x_| 0x8000);                                                       _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<21))) {        *_op_++ =        _x_ >> 16  | 0xc0;      ctou16(_op_) = _x_;                                 _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<28))) { ctou32(_op_)   =        bswap32(_x_| 0xe0000000);                                                   _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<35) {        *_op_++ =         _x_ >> 32 | 0xf0;                                      ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<42) { ctou16(_op_)   = bswap16(_x_ >> 32 | 0xf800);                        _op_ += 2; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<49) {        *_op_++ =         _x_ >> 48 | 0xfc; ctou16(_op_) = _x_ >> 32; _op_ += 2; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<56) { ctou64(_op_)   = bswap64(_x_       | 0xfe00000000000000ull);                                        _op_ += 8; _act_;}\
  else {                                  *_op_++ =                     0xff;                                      ctou64(_op_) = _x_; _op_ += 8; _act_;}\
}

#define _vbxget64(_ip_, _x_, _act_) do { _x_ = *_ip_++;\
       if(!(_x_ & 0x80)) {                                                                                                _act_;}\
  else if(!(_x_ & 0x40)) { _x_ = bswap16(ctou16(_ip_++-1) & 0xff3f);                                                      _act_;}\
  else if(!(_x_ & 0x20)) { _x_ = (_x_ & 0x1f)<<16 | ctou16(_ip_);                                              _ip_ += 2; _act_;}\
  else if(!(_x_ & 0x10)) { _x_ = bswap32(ctou32(_ip_-1) & 0xffffff0f);                                         _ip_ += 3; _act_;}\
  else if(!(_x_ & 0x08)) { _x_ = (_x_ & 0x07)<<32 | ctou32(_ip_);                                              _ip_ += 4; _act_;}\
  else if(!(_x_ & 0x04)) { _x_ = (unsigned long long)(bswap16(ctou16(_ip_-1)) & 0x7ff) << 32 | ctou32(_ip_+1); _ip_ += 5; _act_;}\
  else if(!(_x_ & 0x02)) { _x_ = (_x_ & 0x03)<<48 |   (unsigned long long)ctou16(_ip_) << 32 | ctou32(_ip_+2); _ip_ += 6; _act_;}\
  else if(!(_x_ & 0x01)) { _x_ = bswap64(ctou64(_ip_-1)) & 0x01ffffffffffffffull;                              _ip_ += 7; _act_;}\
  else                   { _x_ = ctou64(_ip_);                                                                 _ip_ += 8; _act_;}\
} while(0)

#define vbxput64(_op_, _x_) { unsigned long long _x = _x_; _vbxput64(_op_, _x, ;); }
#define vbxput32(_op_, _x_) { register unsigned  _x = _x_; _vbxput32(_op_, _x, ;); }
#define vbxput16(_op_, _x_)   vbxput32(_op_, _x_)
#define vbxput8(  _op_, _x_)  (*_op_++ = _x_)

#define vbxget64(_ip_, _x_) _vbxget64(_ip_, _x_, ;)
#define vbxget32(_ip_, _x_) _vbxget32(_ip_, _x_, ;)
#define vbxget16(_ip_, _x_)  vbxget32(_ip_,_x_)
#define vbxget8(_ip_, _x_)       (_x_ = *_ip_++)

//-------------------------------------------------- small integers ----------------------------------------------------------------------------------------------------------------------------------------
#define _vsba2(_vsmax_,_vsb2_) (_vsmax_ - (1<<(_vsb2_)))  //max. integer vsmax=0xff:  vsb2=6: 4276351=41407f(22+ bits)  vsb2=5: 2171071=2120bf(21 bits)  vsb2=4:1118431=1110df(20bits)  vsb2=3:592111=908ef(19 bits)

#define _vso1(_vsmax_,_vsb2_) (_vsba2(_vsmax_,_vsb2_) - (1<<(_vsb2_)))
#define _vso2(_vsmax_,_vsb2_) (_vso1(_vsmax_,_vsb2_) + (1 << (8+(_vsb2_))))

#define _vsput(_op_, _x_, _vsmax_,_vsb2_, _act_) { \
  if(likely((_x_) < _vso1(_vsmax_,_vsb2_))){ *_op_++ = (_x_);																_act_	}\
  else if  ((_x_) < _vso2(_vsmax_,_vsb2_)) { ctou16(_op_) = bswap16((_vso1(_vsmax_,_vsb2_)<<8)+((_x_)-_vso1(_vsmax_,_vsb2_)));                _op_  += 2; _act_}\
  else                                       { *_op_++ = _vsba2(_vsmax_,_vsb2_) + (((_x_) -= _vso2(_vsmax_,_vsb2_)) >> 16); ctou16(_op_) = (_x_); _op_  += 2; _act_}\
}

#define _vsget(_ip_, _x_, _vsmax_,_vsb2_, _act_) do { _x_ = *_ip_++;\
       if(likely(_x_ < _vso1(_vsmax_,_vsb2_))) { _act_ }\
  else if(likely(_x_ < _vsba2(_vsmax_,_vsb2_)))  { _x_ = ((_x_<<8) + (*_ip_)) + (_vso1(_vsmax_,_vsb2_) - (_vso1(_vsmax_,_vsb2_) <<  8)); _ip_++; _act_} \
  else                                           { _x_ = ctou16(_ip_) + ((_x_ - _vsba2(_vsmax_,_vsb2_) ) << 16) + _vso2(_vsmax_,_vsb2_); _ip_ += 2; _act_}\
} while(0)

#define _vslen(_x_,_vsmax_,_vsb2_) ((_x_) < _vbo1(_vsmax_,_vsb2_)?1:((_x_) < _vbo2(_vsmax_,_vsb2_)?2):3)

 #ifndef NMACROS
#define VS22MAX            4276351
#define vslen22(_x_)       _vslen(_x_,0xff,6)
#define vsput22(_op_, _x_) _vsput(_op_, _x_, 0xff, 6, ;)
#define vsget22(_ip_, _x_) _vsget(_ip_, _x_, 0xff, 6, ;) 

#define VS21MAX            2171071
#define vslen21(_x_)       _vslen(_x_,       0xff, 5)
#define vsput21(_op_, _x_) _vsput(_op_, _x_, 0xff, 5, ;)
#define vsget21(_ip_, _x_) _vsget(_ip_, _x_, 0xff, 5, ;) 

#define VS20MAX            1118431
#define vslen20(_x_)       _vslen(_x_,0xff,4)
#define vsput20(_op_, _x_) _vsput(_op_, _x_, 0xff, 4, ;)
#define vsget20(_ip_, _x_) _vsget(_ip_, _x_, 0xff, 4, ;) 
 #else
 #endif

//----------------------------------------------------- 32/64 integer 1,2,3,4,5 bytes----------------------------------------------------------------------------------------
#define _vbba3(_vbsize_,_vbmax_)                    (_vbmax_ - (_vbsize_/8 - 3))
#define _vbba2(_vbsize_,_vbmax_,_vbb3_)             (_vbba3(_vbsize_,_vbmax_) - (1<<_vbb3_))  

#define _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)       (_vbba2( _vbsize_,_vbmax_,       _vbb3_) - (1<<_vbb2_))
#define _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_)       (_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_) + (1 << ( 8+_vbb2_)))
#define _vbo3(_vbsize_,_vbmax_,_vbb2_,_vbb3_)       (_vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_) + (1 << (16+_vbb3_)))

#define _vblen(_x_, _vbsize_,_vbmax_,_vbb2_,_vbb3_) ((_x_) < _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)?1:\
                                                    ((_x_) < _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_)?2:\
													((_x_) < _vbo3(_vbsize_,_vbmax_,_vbb2_,_vbb3_)?3:((T2(bsr,_vbsize_)(_x_)+7)/8+1))))
													
#define _vbvlen(_x_,_vbsize_,_vbmax_,_vbb2_,_vbb3_) ((_x_) < _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)?1:((_x_) < _vbba2(_vbsize_,_vbmax_,_vbb3_)?2:((_x_) < _vbba3(_vbsize_,_vbmax_))?3:((_x_)-_vbba3(_vbsize_,_vbmax_))))

#define _vbput(_op_, _x_, _vbsize_,_vbmax_,_vbb2_,_vbb3_, _act_) do {\
  if(likely((_x_) < _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_))){ *_op_++ = (_x_);																	                                                    _act_;}\
  else if  ((_x_) < _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_)) { ctou16(_op_) = bswap16((_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)<<8)+((_x_)-_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)));           _op_ += 2; /*(_x_) -= _vbo1; *_op_++ = _vbo1 + ((_x_) >> 8); *_op_++ = (_x_);*/ _act_; }\
  else if  ((_x_) < _vbo3(_vbsize_,_vbmax_,_vbb2_,_vbb3_)) { *_op_++ = _vbba2(_vbsize_,_vbmax_,_vbb3_) + (((_x_) -= _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_)) >> 16); ctou16(_op_) = (_x_); _op_ += 2;  _act_;}\
  else { unsigned _b = (T2(bsr,_vbsize_)(_x_)+7)/8; *_op_++ = _vbba3(_vbsize_,_vbmax_) + (_b - 3); T2(ctou,_vbsize_)(_op_) = (_x_); _op_  += _b; _act_; }\
} while(0)

#define _vbget(_ip_, _x_, _vbsize_,_vbmax_,_vbb2_,_vbb3_, _act_) do { _x_ = *_ip_++;\
       if(likely(_x_ < _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_))) { _act_ ;}\
  else if(likely(_x_ < _vbba2( _vbsize_,_vbmax_,_vbb3_)))      { _x_ = /*bswap16(ctou16(_ip_-1))*/ ((_x_<<8) + (*_ip_)) + (_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_) - (_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_) <<  8)); _ip_++; _act_;} \
  else if(likely(_x_ < _vbba3( _vbsize_,_vbmax_)))             { _x_ = ctou16(_ip_) + ((_x_ - _vbba2(_vbsize_,_vbmax_,_vbb3_) ) << 16) + _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_); _ip_ += 2; _act_;}\
  else { unsigned _b = _x_-_vbba3(_vbsize_,_vbmax_);  _x_ = T2(ctou,_vbsize_)(_ip_) & ((1ull << 8 * _b << 24) - 1); _ip_ += 3 + _b; _act_;}\
} while(0)

#ifndef VB_MAX
#define VB_MAX 0xff
#endif

  #ifndef NMACROS
//-- 64 bits -----  
#define vblen64(_x_)               _vblen( _x_,      64, VB_MAX, 6, 5) 
#define vbvlen64(_x_)              _vbvlen(_x_,      64, VB_MAX, 6, 5) 
#define _vbput64(_op_, _x_, _act_) _vbput(_op_, _x_, 64, VB_MAX, 6, 5, _act_)
#define _vbget64(_ip_, _x_, _act_) _vbget(_ip_, _x_, 64, VB_MAX, 6, 5, _act_) 
#define vbput64(_op_, _x_)         do { unsigned long long _x = _x_; _vbput64(_op_, _x_, ;); } while(0)
#define vbget64(_ip_, _x_)         _vbget64(_ip_, _x_, ;)
//-- 32 bits -----
#define vblen32(      _x_)         _vblen(      _x_, 32, VB_MAX, 6, 5) 
#define vbvlen32(     _x_)         _vbvlen(     _x_, 32, VB_MAX, 6, 5) 
#define _vbput32(_op_, _x_, _act_) _vbput(_op_, _x_, 32, VB_MAX, 6, 5, _act_)
#define _vbget32(_ip_, _x_, _act_) _vbget(_ip_, _x_, 32, VB_MAX, 6, 5, _act_) 
#define vbput32(_op_, _x_)         do { unsigned _x = _x_; _vbput32(_op_, _x, ;); } while(0)
#define vbget32(_ip_, _x_)         _vbget32(_ip_, _x_, ;) 
//-- 16 bits -----
#define vblen16( _x_)              vblen32(_x_)
#define vbvlen16(_x_)              vbvlen32(_x_)
#define _vbput16(_op_, _x_, _act_) _vbput32(_op_, _x_, _act_)
#define _vbget16(_ip_, _x_, _act_) _vbget32(_ip_, _x_, _act_)
#define vbput16(_op_, _x_)         vbput32(_op_, _x_)
#define vbget16(_ip_, _x_)         vbget32(_ip_, _x_)
//-- 8 bits ----
#define vblen8(_x_)  1
#define _vbput8(_op_, _x_, _act_)  { *_op_++ = _x_; _act_; }
#define _vbget8(_ip_, _x_, _act_)  { _x_ = *_ip_++; _act_; }
#define vbvlen8(_x_) 1

#define vllen32(_x_)                                _vblen(       _x_, 32, VB_MAX, 4, 3) 
#define vlput32(_op_, _x_)  do { unsigned _x = _x_; _vbput(_op_, _x,  32, VB_MAX, 4, 3, ;); } while(0)
#define vlget32(_ip_, _x_)                          _vbget(_ip_, _x_, 32, VB_MAX, 4, 3, ;) 
  #else
static ALWAYS_INLINE unsigned vblen32(unsigned       x) { return _vblen(      x, 32, VB_MAX, 6, 5); }
#define vbput32(_op_, _x_) _vbput(_op_, _x_, 32, VB_MAX, 6, 5, ;)
static ALWAYS_INLINE void vbget32(unsigned char **_ip, unsigned *_x)  { unsigned char *ip = *_ip; unsigned x; _vbget(ip, x, 32, VB_MAX, 6, 5, ;) ; *_ip = ip; *_x = x; }
static ALWAYS_INLINE unsigned vlget32(unsigned char **_ip)  { unsigned char *ip = *_ip; unsigned x; _vbget(ip, x, 32, VB_MAX, 4, 3, ;); *_ip = ip; return x; }
static ALWAYS_INLINE unsigned vllen32(unsigned       x) { return _vblen(      x, 32, VB_MAX, 4, 3); }
  #endif
