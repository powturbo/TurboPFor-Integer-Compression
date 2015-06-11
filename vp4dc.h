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
//     vp4dc.h - "Integer Compression" TurboPfor  (see vp4dd.h for decompression) 

#define P4DEB(__b) (__b << 1)
#define P4DEBX(__b, __bx) (__bx << 8 | __b << 1 | 1) 
#define P4DSAVE(__out, __b, __bx) do { if(!__bx) *__out++ = P4DEB(__b);else *(unsigned short *)__out = P4DEBX(__b, __bx), __out += 2; } while(0)

#ifdef __cplusplus
extern "C" {
#endif

// compress integer array with n values to the buffer out. Return value = end of compressed buffer out
unsigned char *p4denc32(unsigned          *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4denc16(unsigned short    *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4denc64(uint64_t          *__restrict in, unsigned n, unsigned char *__restrict out);

// SIMD compress integer array with n values to the buffer out. Return value = end of compressed buffer out
unsigned char *p4dencv32(unsigned        *__restrict in, unsigned n, unsigned char *__restrict out);
//unsigned char *p4denc16(unsigned short *__restrict in, unsigned n, unsigned char *__restrict out);

// calculate the best bit size b and bx. 
unsigned p4d32(unsigned       *__restrict in, unsigned n, unsigned *pbx);
unsigned p4d16(unsigned short *__restrict in, unsigned n, unsigned *pbx);
unsigned p4d64(uint64_t       *__restrict in, unsigned n, unsigned *pbx);

// same as p4denc, but with b and bx as parameters. Call after p4d32/p4d16
unsigned char *p4de32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
unsigned char *p4de16(unsigned short *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
unsigned char *p4de64(uint64_t       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);

unsigned char *p4dev32(unsigned      *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);

#ifdef __cplusplus
}
#endif
