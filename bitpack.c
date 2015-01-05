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

    bitpack.c - "Integer Compression" binary packing 
**/
#include "bitpack_.h"
#include "bitpack.h"
#define IPPB( __ip,__x, __parm)

#define PAD8(__x) ( (((__x)+8-1)/8) )

unsigned char *bitpack32(unsigned       *__restrict in, int n, int nb, unsigned char *__restrict out) { unsigned char *pout = out+PAD8(n*nb); BITPACK32(in, n, nb, out, 0); return pout; } 
unsigned char *bitpack16(unsigned short *__restrict in, int n, int nb, unsigned char *__restrict out) { unsigned char *pout = out+PAD8(n*nb); BITPACK32(in, n, nb, out, 0); return pout; }

