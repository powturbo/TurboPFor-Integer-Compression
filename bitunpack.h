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

    bitunpack.h - "Integer Compression" binary packing 
**/

// BP
static inline unsigned  bitgetx32(unsigned *__restrict__ in, unsigned b, unsigned  idx) { unsigned bidx = b*idx; return ((*(unsigned long long *)(in+(bidx>>5))) >> (bidx&0x1f)) & ((1ull<<b)-1); }
static inline unsigned _bitgetx32(unsigned *__restrict__ in, unsigned b, unsigned bidx) {                        return ((*(unsigned long long *)(in+(bidx>>5))) >> (bidx&0x1f)) & ((1ull<<b)-1); }

static inline unsigned  bitgetx16(unsigned *__restrict__ in, unsigned b, unsigned  idx) { unsigned bidx = b*idx; return ((*(unsigned           *)(in+(bidx>>4))) >> (bidx& 0xf)) & ((1   <<b)-1); }
static inline unsigned _bitgetx16(unsigned *__restrict__ in, unsigned b, unsigned bidx) {                        return ((*(unsigned           *)(in+(bidx>>4))) >> (bidx& 0xf)) & ((1   <<b)-1); }

unsigned char *bitunpack32(unsigned char *__restrict__ in, unsigned n, unsigned b, unsigned       *__restrict__ out);
unsigned char *bitunpack16(unsigned char *__restrict__ in, unsigned n, unsigned b, unsigned short *__restrict__ out);

unsigned char * bitunpackx32(unsigned char *__restrict__ in, unsigned n, unsigned b, unsigned *__restrict__ out);
unsigned char *_bitunpackx32(unsigned char *__restrict__ in, unsigned n, unsigned b, unsigned *__restrict__ out);

// DFOR
unsigned char *bitdunpack16( unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned short *__restrict__ out);
unsigned char *bitdunpack32( unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned       *__restrict__ out);
unsigned char *bitdunpackb32(unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned       *__restrict__ out);

// FOR+
unsigned char *bitfunpack16( unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned short *__restrict__ out);
unsigned char *bitfunpack32( unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned       *__restrict__ out);
unsigned char *bitfunpackb32(unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned       *__restrict__ out);

//static inline int bitgeq(unsigned char *__restrict__ in, unsigned n, unsigned b, int *oidx, int oval, int val) { unsigned idx=*oidx; while(++idx < n) { oval += bitgetb(in, b, idx)+1; if(oval >= val) { *oidx=idx; return oval; } }  return INT_MAX; }

