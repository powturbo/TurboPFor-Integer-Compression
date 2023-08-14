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
#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>
#if defined(__clang__) && defined(__is_identifier)
  #if !__is_identifier(_Float16)
    #undef FLT16_BUILTIN
  #endif
#elif defined(FLT16_MAX)
#define FLT16_BUILTIN
#endif

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
#define _PACKED         //__attribute__ ((packed))
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
#ifdef FLT16_BUILTIN
static ALWAYS_INLINE _Float16           ctof16(const void *cp) { _Float16           x; memcpy(&x, cp, sizeof(x)); return x; }
#endif
static ALWAYS_INLINE float              ctof32(const void *cp) { float              x; memcpy(&x, cp, sizeof(x)); return x; }
static ALWAYS_INLINE double             ctof64(const void *cp) { double             x; memcpy(&x, cp, sizeof(x)); return x; }

static ALWAYS_INLINE void               stou16(      void *cp, unsigned short     x) { memcpy(cp, &x, sizeof(x)); } // ua write
static ALWAYS_INLINE void               stou32(      void *cp, unsigned           x) { memcpy(cp, &x, sizeof(x)); }
static ALWAYS_INLINE void               stou64(      void *cp, unsigned long long x) { memcpy(cp, &x, sizeof(x)); }
static ALWAYS_INLINE void               stousz(      void *cp, size_t             x) { memcpy(cp, &x, sizeof(x)); }
#ifdef FLT16_BUILTIN
static ALWAYS_INLINE void               stof16(      void *cp, _Float16           x) { memcpy(cp, &x, sizeof(x)); }
#endif
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
#define ctof16(_cp_) (*(_Float16       *)(_cp_))
#define ctof32(_cp_) (*(float          *)(_cp_))

#define stou16(_cp_, _x_)  (*(unsigned short *)(_cp_) = _x_)
#define stou32(_cp_, _x_)  (*(unsigned       *)(_cp_) = _x_)
#define stof16(_cp_, _x_)  (*(_Float16       *)(_cp_) = _x_)
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
#ifdef FLT16_BUILTIN
struct _PACKED float16u  { _Float16           g; };
#endif
struct _PACKED floatu    { float              f; };
struct _PACKED doubleu   { double             d; };

#define ctou16(_cp_) ((struct shortu    *)(_cp_))->s
#define ctou32(_cp_) ((struct unsignedu *)(_cp_))->u
#define ctou64(_cp_) ((struct longu     *)(_cp_))->l
#define ctof16(_cp_) ((struct float16u  *)(_cp_))->g
#define ctof32(_cp_) ((struct floatu    *)(_cp_))->f
#define ctof64(_cp_) ((struct doubleu   *)(_cp_))->d

#define stou16(_cp_, _x_) ((struct shortu    *)(_cp_))->s = _x_
#define stou32(_cp_, _x_) ((struct unsignedu *)(_cp_))->u = _x_
#define stou64(_cp_, _x_) ((struct longu     *)(_cp_))->l = _x_
#define stof16(_cp_, _x_) ((struct float16u  *)(_cp_))->g = _x_
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
