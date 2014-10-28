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

    bitunpack_.h - "Integer Compression" binary packing 
**/

#include "bitunpack.h"

#define PAD8(__x) (((__x)+7)/8)
unsigned char * bitunpackx32(unsigned char *__restrict__ in, unsigned n, unsigned b, unsigned *__restrict__ out) { unsigned i; 	   for(i=0; i < n; i++      ) out[i] =  bitgetx32(in, b, i); return in + PAD8(n*b); }
unsigned char *_bitunpackx32(unsigned char *__restrict__ in, unsigned n, unsigned b, unsigned *__restrict__ out) { unsigned i,k=0; for(i=0; i < n; i++,k+=b ) *out++ = _bitgetx32(in, b, k); return in + PAD8(n*b); }

#define BPI(__w,__parm) __w
#include "bitunpack_.h"
unsigned char *bitunpack32( unsigned char *__restrict__ in, unsigned n, unsigned b, unsigned       *__restrict__ out) { unsigned char *pin = in+PAD8(n*b); BITUNPACK32(in, n, b, out, 0); return pin; }
unsigned char *bitunpack16( unsigned char *__restrict__ in, unsigned n, unsigned b, unsigned short *__restrict__ out) { unsigned char *pin = in+PAD8(n*b); BITUNPACK32(in, n, b, out, 0); return pin; }
#undef BPI

//------------------------------------------------------------------------------------------
#define BPI(__w,__parm) (__parm += (__w) + 1)
#include "bitunpack_.h"

unsigned char *bitdunpack32( unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned       *__restrict__ out) { unsigned char *pin=in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return pin; }
unsigned char *bitdunpackx32(unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned       *__restrict__ out) { int i; for(i = 0; i < n; i++) out[i] = (start += bitgetx32(in, b, i)+1); return in + PAD8(n*b); }
unsigned char *bitdunpack16( unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned short *__restrict__ out) { unsigned char *pin=in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return pin; }
#undef BPI

//------------------------------------------------------------------------------------------
#define BPI(__w,__parm) (__parm + (__w) + 1)
#include "bitunpack_.h"

unsigned char *bitfunpack32( unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned       *__restrict__ out) { unsigned char *pin=in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return pin; }
unsigned char *bitfunpackx32(unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned       *__restrict__ out) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, b, i)+start+1; return in + PAD8(n*b); }
unsigned char *bitfunpack16( unsigned char *__restrict__ in, unsigned n, unsigned b, int start, unsigned short *__restrict__ out) { unsigned char *pin=in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return pin; }
#undef BPI

