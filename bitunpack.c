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

    - email    : powturbo [AT] gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo

    bitunpack_.h - "Integer Compression" Binary Packing 
**/
#include "conf.h"
#include "bitunpack.h"
#define PAD8(__x) (((__x)+7)/8)

//-----------------------------------------------------------------------------------------------------------------
#define BPI(__w, __op, __parm) __w
#include "bitunpack_.h"
unsigned char *bitunpack32( unsigned char *__restrict in, unsigned n, unsigned b,             unsigned       *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out,    0); return ip; }
unsigned char *bitunpack16( unsigned char *__restrict in, unsigned n, unsigned b,             unsigned short *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out,    0); return ip; }
#undef BPI

//-----------------------------------------------------------------------------------------------------------------
#define BPI(__w, __op, __parm) (__parm += (__w) + 1)
#include "bitunpack_.h"
unsigned char *bitdunpack32( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned       *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
unsigned char *bitdunpack16( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned short *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI

//------------------------------------------------------------------------------------------
#define BPI(__w, __op, __parm) (__parm += (__w))
#include "bitunpack_.h"
unsigned char *bitd0unpack32( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned       *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
unsigned char *bitd0unpack16( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned short *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI

//------------------------------------------------------------------------------------------
#define BPI(__w, __op, __parm) (__parm + (__op+1-_op))//#define BPI(__w, __op, __parm) (__parm + (__w) + 1)
#include "bitunpack_.h"
unsigned char *bitfunpack32( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned       *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
unsigned char *bitfunpack16( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned short *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI

//------------------------------------------------------------------------------------------
#define BPI(__w, __op, __parm) (__parm + (__op-_op))
#include "bitunpack_.h"

unsigned char *bitf0unpack32( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned       *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
unsigned char *bitf0unpack16( unsigned char *__restrict in, unsigned n, unsigned b, int start, unsigned short *__restrict out) { unsigned char *ip = in+PAD8(n*b); BITUNPACK32(in, n, b, out, start); return ip; }
#undef BPI

