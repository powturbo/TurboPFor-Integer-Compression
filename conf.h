/**
    Copyright (C) powturbo 2013-2014
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

    - email    : powturbo@gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo

    conf.h - "Integer Compression" config & common 
**/


  #if defined(__GNUC__)
#define ALIGNED(t,v,n)  __attribute__ ((aligned (n))) t v
#define ALWAYS_INLINE   __attribute__((always_inline))
#define _PACKED 	__attribute__ ((packed))
#define likely(x)     	__builtin_expect((x),1)
#define unlikely(x)   	__builtin_expect((x),0)

#define popcnt32(__x) 	__builtin_popcount(__x)
#define popcnt64(__x) 	__builtin_popcountll(__x)

#define TEMPLATE2_(__x, __y) __x##__y
#define TEMPLATE2(__x, __y) TEMPLATE2_(__x,__y)

#define TEMPLATE3_(x,y,z) x ## ## y ## z
#define TEMPLATE3(x,y,z) TEMPLATE3_(x, y, z)

    #if defined(__x86_64__) || defined(__x86_32__)
static inline int bsr32(int x) {
  int b = -1;
  asm("bsrl %1,%0" : "+r" (b): "rm" (x) );
  return b + 1;
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


