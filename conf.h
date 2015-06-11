/**
    Copyright (C) powturbo 2013-2015
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

//	   conf.h - "Integer Compression" config & common 
#ifndef CONF_H
#define CONF_H

  #if defined(__GNUC__)
#define ALIGNED(t,v,n)  t v __attribute__ ((aligned (n))) 
#define ALWAYS_INLINE   inline __attribute__((always_inline))
#define _PACKED 		__attribute__ ((packed))
#define likely(x)     	__builtin_expect((x),1)
#define unlikely(x)   	__builtin_expect((x),0)

#define popcnt32(__x) 	__builtin_popcount(__x)
#define popcnt64(__x) 	__builtin_popcountll(__x)

#define TEMPLATE2_(__x, __y) __x##__y
#define TEMPLATE2(__x, __y) TEMPLATE2_(__x,__y)

#define TEMPLATE3_(__x,__y,__z) __x##__y##__z
#define TEMPLATE3(__x,__y,__z) TEMPLATE3_(__x, __y, __z)

    #if defined(__x86_64__) || defined(__x86_32__)
static inline int bsr32(int x) {
  int b = -1;
  asm("bsrl %1,%0" : "+r" (b): "rm" (x) );
  return b + 1;
}

static inline int __bsr32(int x) {
  asm("bsr %1,%0" : "=r" (x) : "rm" (x) );
  return x;
}

static inline int bsr64(unsigned long long x) {
  return x?64 - __builtin_clzll(x):0;
}

#define bsr16(__x) bsr32(__x)
    #else
static inline int bsr32(int x) {
  return x?32 - __builtin_clz(x):0;
}

static inline int bsr64(unsigned long long x) {
  return x?64 - __builtin_clzll(x):0;
}
    #endif
#define ctzll(__x) __builtin_ctzll(__x)
  #else
#error "only gcc support in this version"
  #endif
  
#define SIZE_ROUNDUP(__n, __a) (((size_t)(__n) + (size_t)((__a) - 1)) & ~(size_t)((__a) - 1))
//---------------------------------------------------------------------------------------------------
#define ctou8(__cp)  (*(unsigned char *)(__cp))
#define ctou16(__cp) (*(unsigned short *)(__cp))
#define ctou24(__cp) ((*(unsigned *)(__cp)) & 0xffffff)
#define ctou32(__cp) (*(unsigned *)(__cp))
#define ctou64(__cp) (*(unsigned long long *)(__cp))
#define ctou48(__cp) ((*(unsigned long long *)(__cp)) & 0xffffffffffffull)
#define ctou(__cp_t, __cp)  (*(__cp_t *)(__cp))
  #ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#define max(x,y) (((x)>(y)) ? (x) : (y))
  #endif

  #ifdef NDEBUG
#define AS(expr, fmt,args...)
  #else
#include <stdio.h>
#define AS(expr, fmt,args...) if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); abort(); }
  #endif
#define AC(expr, fmt,args...) if(!(expr)) { fflush(stdout);fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); abort(); }
#define die(fmt,args...) do { fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); fprintf(stderr, fmt, ## args ); fflush(stderr); exit(-1); } while(0)

#endif
