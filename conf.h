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

//	    conf.h - config & common 
#ifndef CONF_H
#define CONF_H
//------------------------- Compiler ------------------------------------------
  #if defined(__GNUC__)
#define ALIGNED(t,v,n)  t v __attribute__ ((aligned (n))) 
#define ALWAYS_INLINE   inline __attribute__((always_inline))
#define NOINLINE        __attribute__((noinline))
#define _PACKED 		__attribute__ ((packed))
#define likely(x)     	__builtin_expect((x),1)
#define unlikely(x)   	__builtin_expect((x),0)

#define popcnt32(_x_) 	__builtin_popcount(_x_)
#define popcnt64(_x_) 	__builtin_popcountll(_x_)

    #if defined(__i386__) || defined(__x86_64__)
static inline int __bsr32(int x) {             asm("bsr  %1,%0" : "=r" (x) : "rm" (x) ); return x; }
static inline int bsr32(  int x) { int b = -1; asm("bsrl %1,%0" : "+r" (b) : "rm" (x) ); return b + 1; }
static inline int bsr64(unsigned long long x) { return x?64 - __builtin_clzll(x):0; }
#define bsr16(_x_) bsr32(_x_)
    #else
static inline int bsr32(int x               ) { return x?32 - __builtin_clz(  x):0; }
static inline int bsr64(unsigned long long x) { return x?64 - __builtin_clzll(x):0; }
    #endif

#define ctz64(_x_) __builtin_ctzll(_x_)
#define ctz32(_x_) __builtin_ctz(_x_)

  #elif _MSC_VER
#define ALIGNED(x)		__declspec(align(x))
#define ALWAYS_INLINE	__forceinline
#define NOINLINE		__declspec(noinline)
#define inline          __inline
#define THREADLOCAL		__declspec(thread)
#define likely(x)     	(x)
#define unlikely(x)   	(x)
#define __builtin_prefetch(x) //_mm_prefetch(x, _MM_HINT_NTA)
static inline int bsr32(int x) { return x ? 32 - __builtin_clz(x) : 0; }
    #ifdef _WIN64
static inline int bsr64(unsigned long long x) { unsigned long z = 0; _BitScanForward64(&z, x); return 64 - z; }
static inline int ctz64(unsigned long long x) { unsigned long z = 0; _BitScanForward64(&z, x); return z; }
    #endif
static inline int ctz32(unsigned           x) { unsigned      z = 0; _BitScanForward(&z, x); return z; }
#define fseeko _fseeki64
#define ftello _ftelli64
#define sleep(x) Sleep(x/1000)
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
  #endif 

#define ctz16(_x_) ctz32(_x_)
//--------------- Unaligned memory access -------------------------------------
/*# || defined(i386) || defined(_X86_) || defined(__THW_INTEL)*/
  #if defined(__i386__) || defined(__x86_64__) || \
    defined(_M_IX86) || defined(_M_AMD64) || /*MSC_VER*/\
    defined(__powerpc__) ||\
    defined(__ARM_FEATURE_UNALIGNED) || defined(__aarch64__) || defined(__arm__) ||\
    defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__) || \
    defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__) || \
    defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__)  || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6Z__)   || defined(__ARM_ARCH_6ZK__)
#define ctou16(_cp_) *(unsigned short *)(_cp_)
#define ctou32(_cp_) *(unsigned       *)(_cp_)

    #if defined(__i386__) || defined(__x86_64__) || defined(__powerpc__)
#define ctou64(_cp_)       (*(unsigned long long *)(_cp_))
#define ctou(_cp_t, _cp_) (*(_cp_t *)(_cp_))
    #endif

  #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7S__)
struct _PACKED shortu    { unsigned short     s; };
struct _PACKED unsignedu { unsigned           u; };
struct _PACKED longu     { unsigned long long l; };

#define ctou16(_cp_) ((struct shortu    *)(_cp_))->s
#define ctou32(_cp_) ((struct unsignedu *)(_cp_))->u
#define ctou64(_cp_) ((struct longu     *)(_cp_))->l
  #else
#error "unknown cpu"	  
  #endif

  #ifdef ctou16
#define utoc16(_x_,_cp_) ctou16(_cp_) = _x_
  #else
static inline unsigned short     ctou16(const void *cp) { unsigned short     x; memcpy(&x, cp, sizeof(x)); return x; }
static inline               void utoc16(unsigned short     x, void *cp ) { memcpy(cp, &x, sizeof(x)); }
  #endif

  #ifdef ctou32
#define utoc32(_x_,_cp_) ctou32(_cp_) = _x_
  #else
static inline unsigned           ctou32(const void *cp) { unsigned           x; memcpy(&x, cp, sizeof(x)); return x; }
static inline               void utoc32(unsigned           x, void *cp ) { memcpy(cp, &x, sizeof(x)); }
  #endif

  #ifdef ctou64
#define utoc64(_x_,_cp_) ctou64(_cp_) = _x_
  #else
static inline unsigned long long ctou64(const void *cp) { unsigned long long x; memcpy(&x, cp, sizeof(x)); return x; }
static inline               void utoc64(unsigned long long x, void *cp ) { memcpy(cp, &x, sizeof(x)); }
  #endif

#define ctou24(_cp_) (ctou32(_cp_) & 0xffffff)
#define ctou48(_cp_) (ctou64(_cp_) & 0xffffffffffffull)
#define ctou8(_cp_) (*_cp_)
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
#define SIZE_ROUNDUP(_n_, _a_) (((size_t)(_n_) + (size_t)((_a_) - 1)) & ~(size_t)((_a_) - 1))
  #ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#define max(x,y) (((x)>(y)) ? (x) : (y))
  #endif
  
#define TEMPLATE2_(_x_, _y_) _x_##_y_
#define TEMPLATE2(_x_, _y_) TEMPLATE2_(_x_,_y_)

#define TEMPLATE3_(_x_,_y_,_z_) _x_##_y_##_z_
#define TEMPLATE3(_x_,_y_,_z_) TEMPLATE3_(_x_, _y_, _z_)

//--- NDEBUG -------
#include <stdio.h>
  #ifdef _MSC_VER
    #ifdef NDEBUG
#define AS(expr, fmt, ...)
#define AC(expr, fmt, ...) if(!(expr)) { fprintf(stderr, fmt, __VA_ARGS__ ); fflush(stderr); abort(); }
#define die(fmt, ...) do { fprintf(stderr, fmt, __VA_ARGS__ ); fflush(stderr); exit(-1); } while(0)
    #else
#define AS(expr, fmt, ...) if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ,__VA_ARGS__ ); fflush(stderr); abort(); }
#define AC(expr, fmt, ...) if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ,__VA_ARGS__ ); fflush(stderr); abort(); }
#define die(fmt, ...) do { fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ,__VA_ARGS__ ); fflush(stderr); exit(-1); } while(0)
    #endif
  #else 
    #ifdef NDEBUG
#define AS(expr, fmt,args...)
#define AC(expr, fmt,args...) if(!(expr)) { fprintf(stderr, fmt, ## args ); fflush(stderr); abort(); }
#define die(fmt,args...) do { fprintf(stderr, fmt, ## args ); fflush(stderr); exit(-1); } while(0)
    #else
#define AS(expr, fmt,args...) if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); abort(); }
#define AC(expr, fmt,args...) if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); abort(); }
#define die(fmt,args...) do { fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); exit(-1); } while(0)
    #endif
  #endif

