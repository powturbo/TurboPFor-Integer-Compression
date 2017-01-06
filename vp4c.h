/**
    Copyright (C) powturbo 2013-2017
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
//  "Integer Compression" TurboPfor  (see vp4d.h for decompression) 

#ifdef __cplusplus
extern "C" {
#endif

// Low level API: Single block n limited 
//compress integer array with n values to the buffer out. Return value = end of compressed buffer out
unsigned char *p4enc16(      unsigned short *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4enc32(      unsigned       *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4enc128v32(  unsigned       *__restrict in, unsigned n, unsigned char *__restrict out); // SIMD (Vertical bitpacking)
unsigned char *p4enc256v32(  unsigned       *__restrict in, unsigned n, unsigned char *__restrict out); // SIMD (Vertical bitpacking)
unsigned char *p4enc64(      uint64_t       *__restrict in, unsigned n, unsigned char *__restrict out);

unsigned char *p4encx16(     unsigned short *__restrict in, unsigned n, unsigned char *__restrict out);// Direct access 
unsigned char *p4encx32(     unsigned       *__restrict in, unsigned n, unsigned char *__restrict out);

unsigned char *p4denc16(     unsigned short *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start);
unsigned char *p4denc32(     unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start);
unsigned char *p4denc128v32( unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start); // SIMD (Vertical bitpacking)
unsigned char *p4denc256v32( unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start); 
unsigned char *p4denc64(     uint64_t       *__restrict in, unsigned n, unsigned char *__restrict out, uint64_t       start);

unsigned char *p4dencx16(    unsigned short *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start);// Direct access 
unsigned char *p4dencx32(    unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start);

unsigned char *p4d1enc16(    unsigned short *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start);
unsigned char *p4d1enc32(    unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start);
unsigned char *p4d1enc128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start); // SIMD (Vertical bitpacking)
unsigned char *p4d1enc256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start); 
unsigned char *p4d1enc64(    uint64_t       *__restrict in, unsigned n, unsigned char *__restrict out, uint64_t       start);

unsigned char *p4d1encx16(   unsigned short *__restrict in, unsigned n, unsigned char *__restrict out, unsigned short start);// Direct access 
unsigned char *p4d1encx32(   unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned       start);

// same as p4enc, but with b and bx as parameters. Call after _p4d16,_p4d32,_p4d64
unsigned char *_p4enc16(     unsigned short *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
unsigned char *_p4enc32(     unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx); 
unsigned char *_p4enc128v32( unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx); // SIMD (Vertical bitpacking)
unsigned char *_p4enc256v32( unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx); 
unsigned char *_p4enc64(     uint64_t       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned b, unsigned bx);
// calculate the best bit sizes b and bx, return b. 
unsigned _p4bits16(          unsigned short *__restrict in, unsigned n, unsigned *pbx);
unsigned _p4bits32(          unsigned       *__restrict in, unsigned n, unsigned *pbx);
unsigned _p4bits64(          uint64_t       *__restrict in, unsigned n, unsigned *pbx);

//************************** n unlimited ************************************************************************************
// compress integer array with n values to the buffer out. Return value = end of compressed buffer out
unsigned char *p4nenc16(      unsigned short *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4nenc32(      unsigned       *__restrict in, unsigned n, unsigned char *__restrict out);
unsigned char *p4nenc128v32(  unsigned       *__restrict in, unsigned n, unsigned char *__restrict out); // SIMD (Vertical bitpacking)
unsigned char *p4nenc256v32(  unsigned       *__restrict in, unsigned n, unsigned char *__restrict out); 
unsigned char *p4nenc64(      uint64_t       *__restrict in, unsigned n, unsigned char *__restrict out);

unsigned char *p4nencx16(     unsigned short *__restrict in, unsigned n, unsigned char *__restrict out);// Direct access 
unsigned char *p4nencx32(     unsigned       *__restrict in, unsigned n, unsigned char *__restrict out);

unsigned char *p4ndenc128v32( unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start); // SIMD (Vertical bitpacking)
unsigned char *p4ndenc256v32( unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start); 

unsigned char *p4nd1enc128v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start);
unsigned char *p4nd1enc256v32(unsigned       *__restrict in, unsigned n, unsigned char *__restrict out, unsigned start); 

#ifdef __cplusplus
}
#endif

#define P4D_MAX 256

#define P4EB(_b_) (_b_ << 1)
#define P4EBX(_b_, _bx_) (_bx_ << 8 | _b_ << 1 | 1) 
#define P4SAVE(_out_, _b_, _bx_) do { if(!_bx_) *_out_++ = P4EB(_b_);else *(unsigned short *)_out_ = P4EBX(_b_, _bx_), _out_ += 2; } while(0)

