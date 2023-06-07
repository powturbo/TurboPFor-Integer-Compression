/**
    Copyright (C) powturbo 2013-2023
    SPDX-License-Identifier: GPL v2 License

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
// v8pack.c - "Integer Compression" TurboBitByte = Hybrid TurboByte + TurboPack (use BitPack for small values, otherwise TurboByte)
#pragma warning( disable : 4005)
#pragma warning( disable : 4090)
#pragma warning( disable : 4068)

#include <string.h>
#include "include_/conf.h"
#include "include_/bitpack.h"
#include "include_/bitutil.h"
#include "include_/vint.h"
#define VB_MAX 0xfe
#include "include_/vlcbyte.h"

#include "include_/bitutil_.h"
#define V8PAYLOAD(_n_, _usize_) (((_n_)*(_usize_/16)+7)/8)  //from v8.c
#define V8BOUND_(_n_, _usize_) (V8PAYLOAD(_n_, _usize_)+ (_n_)*(_usize_/8))

#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#define max(x,y) (((x)>(y)) ? (x) : (y))
#endif

#define PAD8(_x_) ( (((_x_)+8-1)/8) )
// 0-0x1f: bitpacking, 0xff = memcpy, 0xfe:varint

#define _V8E(in, _n_, out, _csize_, _usize_, _bit_, _bitpackv_, _bitpack_) {   if(!_n_) return 0;\
  unsigned char *op = out, *op_ = out+_n_*(_usize_/8);\
  for(ip = in; ip < in + _n_; ) {                                               PREFETCH(ip+512,0);\
    unsigned _b, _bs, _o, iplen = (in+_n_) - ip; iplen = min(iplen,_csize_);\
	_o = T2(_bit_, _usize_)(ip, iplen, &x); _b = T2(bsr,_usize_)(_o); \
	_bs = (_b*iplen+7)/8+1;\
	if(_b > 8+(_usize_/16)) { /*TurboByte < bitpacking?*/\
	  if(op+1+V8BOUND_(iplen, _usize_) < op_) { /*overflow?*/\
	    unsigned char *_sp = op;\
	    *op++ = 0xfe; \
		op = T2(v8enc, _usize_)(ip, iplen, op);                                 /*AS(op < op_, "#_V8DE overflow %u:%u,%u,%u\n", op - op_, 1+V8BOUND_(iplen, _usize_), op-_sp, v8len16(ip, iplen));*/\
	    if(op - _sp < _bs) goto a;\
	    op = _sp; /*restore*/\
	  }\
	}\
    if(op+_bs >= op_) {  op = out; *op++ = 0xff; memcpy(op, in, _n_*(_usize_/8)); op += _n_*(_usize_/8);  /*AS(op == op_+1, "#_V8DE overflow %u", op - op_);*/ goto e; }\
	*op++ = _b; if(_b) op = iplen == _csize_?T2(_bitpackv_, _usize_)(ip, _csize_, op, _b):\
                                         T2(_bitpack_,  _usize_)(ip, iplen,   op, _b);\
    a:ip += iplen; \
  } 															                /*AS(op <= op_, "#_V8DE overflow %u", op - op_);*/\
  e: return op - out;\
} 

#define _V8DE(in, _n_, out, _csize_, _usize_, _v8enc_, _bitd_, _bitpackv_, _bitpack_,_delta_) {\
  if(!_n_) return 0;\
  unsigned char *op = out, *op_ = out+_n_*(_usize_/8);\
  start = *in++;\
  T2(vbput, _usize_)(op, start);\
  for(_n_--,ip = in; ip < in + _n_; ) {                                         /*PREFETCH(ip+512,0);*/\
    unsigned _b, _bs, _o, iplen = (in+_n_) - ip; iplen = min(iplen,_csize_);\
	_o = T2(_bitd_, _usize_)(ip, iplen, &x, start); _b = T2(bsr,_usize_)(_o); \
	_bs = (_b*iplen+7)/8+1;\
	if(_b > 8+(_usize_/16)) { /*TurboByte < bitpacking?*/\
	  if(op+1+V8BOUND_(iplen, _usize_) < op_) { /*overflow?*/\
	    unsigned char *_sp = op;\
	    *op++ = 0xfe; \
		op = T2(_v8enc_, _usize_)(ip, iplen, op, start);                        /*AS(op < op_, "#_V8DE overflow %u:%u,%u,%u\n", op - op_, 1+V8BOUND_(iplen, _usize_), op-_sp, v8len16(ip, iplen));*/\
	    if(op - _sp < _bs) goto a;\
	    op = _sp; /*restore*/\
	  }\
	}\
    if(op+_bs >= op_) { op = out; *op++ = 0xff; memcpy(op, in-1, (_n_+1)*(_usize_/8)); op += (_n_+1)*(_usize_/8);/*AS(op == op_+1, "#_V8DE overflow %u", op - op_);*/ goto e; }\
	  *op++ = _b; if(_b) { op = iplen == _csize_?T2(_bitpackv_, _usize_)(ip, _csize_, op, start, _b):\
	                                         T2(_bitpack_,  _usize_)(ip, iplen,   op, start, _b);\
	}\
    a: ip += iplen;\
	start = ip[-1];\
  }                                                                             /*AS(op <= op_, "#_V8DE overflow %u", op - op_);*/\
  e:return op - out;\
}

size_t v8nenc16(      uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8E( in, n, out, 128, 16, bit,            bitpack,      bitpack); }
size_t v8nenc32(      uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8E( in, n, out, 128, 32, bit,            bitpack,      bitpack); }

size_t v8ndenc16(     uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8DE(in, n, out, 128, 16, v8denc,  bitd,  bitdpack,     bitdpack,0); }
size_t v8ndenc32(     uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 128, 32, v8denc,  bitd,  bitdpack,     bitdpack,0); }

size_t v8nd1enc16(    uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8DE(in, n, out, 128, 16, v8d1enc, bitd1, bitd1pack,    bitd1pack,1); }
size_t v8nd1enc32(    uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 128, 32, v8d1enc, bitd1, bitd1pack,    bitd1pack,1); }

size_t v8nzenc16(     uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8DE(in, n, out, 128, 16, v8zenc,  bitz,  bitzpack,     bitzpack,2); }
size_t v8nzenc32(     uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 128, 32, v8zenc,  bitz,  bitzpack,     bitzpack,2); }
//----
size_t v8nenc128v16(  uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8E( in, n, out, 128, 16,          bit,   bitpack128v,  bitpack); }
size_t v8nenc128v32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8E( in, n, out, 128, 32,          bit,   bitpack128v,  bitpack); }

size_t v8ndenc128v16( uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8DE(in, n, out, 128, 16, v8denc,  bitd,  bitdpack128v, bitdpack,0); }
size_t v8ndenc128v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 128, 32, v8denc,  bitd,  bitdpack128v, bitdpack,0); }

size_t v8nd1enc128v16(uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8DE(in, n, out, 128, 16, v8d1enc, bitd1, bitd1pack128v,bitd1pack,1); }
size_t v8nd1enc128v32(uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 128, 32, v8d1enc, bitd1, bitd1pack128v,bitd1pack,1); }

size_t v8nzenc128v16( uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8DE(in, n, out, 128, 16, v8zenc,  bitz,  bitzpack128v, bitzpack,2); }
size_t v8nzenc128v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 128, 32, v8zenc,  bitz,  bitzpack128v, bitzpack,2); }

size_t v8nxenc128v16( uint16_t *__restrict in, size_t n, unsigned char *__restrict out) { uint16_t *ip,start,x; _V8DE(in, n, out, 128, 16, v8xenc,  bitx,      bitxpack128v, bitxpack,3); }
size_t v8nxenc128v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 128, 32, v8xenc,  bitx,      bitxpack128v, bitxpack,3); }
//-------
  #if defined(__i386__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_IX86)
size_t v8nenc256v32(  uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8E( in, n, out, 256, 32,          bit256v,   bitpack256v,  bitpack); }
size_t v8ndenc256v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 256, 32, v8denc,  bitd256v,  bitdpack256v, bitdpack,0); }
size_t v8nd1enc256v32(uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 256, 32, v8d1enc, bitd1256v, bitd1pack256v,bitd1pack,1); }
size_t v8nzenc256v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 256, 32, v8zenc,  bitz256v,  bitzpack256v, bitzpack,2); }
size_t v8nxenc256v32( uint32_t *__restrict in, size_t n, unsigned char *__restrict out) { uint32_t *ip,start,x; _V8DE(in, n, out, 256, 32, v8xenc,  bitx256v,  bitxpack256v, bitxpack,3); }
  #endif

#define _V8D(in, n, out, _csize_, _usize_, _bitunpackv_, _bitunpack_) {      if(!n) return 0;\
  unsigned char *ip = in;\
  if(*in == 0xff) { \
    memcpy(out, ip+1, n*(_usize_/8)); \
	ip += 1+n*(_usize_/8); \
  } else { \
    for(op = out, out += n; op < out; ) {                                 /*PREFETCH(ip+512,0);*/\
      unsigned oplen = min(out-op,_csize_),_b = *ip++;\
      if(_b == 0xfe) ip = T2(v8dec, _usize_)(ip, oplen, op);\
      else           ip = oplen == _csize_?T2(_bitunpackv_, _usize_)(ip, _csize_, op, _b):\
                                           T2(_bitunpack_,  _usize_)(ip, oplen,   op, _b);\
 	  op += oplen;\
    }\
  }\
  return ip - in;\
} 

#define _V8DD(in, n, out, _csize_, _usize_, _v8dec_, _bitunpackv_, _bitunpack_, _delta_) {      if(!n) return 0;\
  unsigned char *ip = in;\
  if(*ip == 0xff) { \
    memcpy(out, ip+1, n*(_usize_/8)); \
	ip += 1+n*(_usize_/8);\
  } else {\
    T2(vbget, _usize_)(ip, start);\
    for(*out++ = start,--n, op = out, out+=n; op < out; ) {                                 /*PREFETCH(ip+512,0);*/\
      unsigned oplen = min(out-op,_csize_), _b = *ip++;\
      if(_b==0xfe) ip = T2(_v8dec_, _usize_)(ip, oplen, op, start);\
      else         ip = oplen == _csize_?T2(_bitunpackv_, _usize_)(ip, _csize_, op, start, _b):\
                                         T2(_bitunpack_,  _usize_)(ip, oplen,   op, start, _b);\
      op += oplen; start = op[-1];\
    }\
  }\
  return ip - in;\
}

size_t v8ndec16(      unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op;       _V8D(in,  n, out, 128, 16,         bitunpack,      bitunpack); }
size_t v8ndec32(      unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op;       _V8D(in,  n, out, 128, 32,         bitunpack,      bitunpack); }

size_t v8nddec16(     unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; _V8DD(in, n, out, 128, 16, v8ddec, bitdunpack,     bitdunpack, 0); }
size_t v8nddec32(     unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 128, 32, v8ddec, bitdunpack,     bitdunpack, 0); }

size_t v8nd1dec16(    unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; _V8DD(in, n, out, 128, 16, v8d1dec,bitd1unpack,    bitd1unpack,1); }
size_t v8nd1dec32(    unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 128, 32, v8d1dec,bitd1unpack,    bitd1unpack,1); }

size_t v8nzdec16(     unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; _V8DD(in, n, out, 128, 16, v8zdec, bitzunpack,     bitzunpack, 2); }
size_t v8nzdec32(     unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 128, 32, v8zdec, bitzunpack,     bitzunpack, 2); }
//---------
size_t v8ndec128v16(  unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op;       _V8D(in,  n, out, 128, 16,         bitunpack128v,  bitunpack); }
size_t v8ndec128v32(  unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op;       _V8D(in,  n, out, 128, 32,         bitunpack128v,  bitunpack); }

size_t v8nddec128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; _V8DD(in, n, out, 128, 16, v8ddec, bitdunpack128v, bitdunpack, 0); }
size_t v8nddec128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 128, 32, v8ddec, bitdunpack128v, bitdunpack, 0); }

size_t v8nd1dec128v16(unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; _V8DD(in, n, out, 128, 16, v8d1dec,bitd1unpack128v,bitd1unpack,1); }
size_t v8nd1dec128v32(unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 128, 32, v8d1dec,bitd1unpack128v,bitd1unpack,1); }

size_t v8nzdec128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; _V8DD(in, n, out, 128, 16, v8zdec, bitzunpack128v, bitzunpack, 2); }
size_t v8nzdec128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 128, 32, v8zdec, bitzunpack128v, bitzunpack, 2); }

size_t v8nxdec128v16( unsigned char *__restrict in, size_t n, uint16_t *__restrict out) { uint16_t *op,start; _V8DD(in, n, out, 128, 16, v8xdec, bitxunpack128v, bitxunpack, 2); }
size_t v8nxdec128v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 128, 32, v8xdec, bitxunpack128v, bitxunpack, 2); }
//---------
    #if defined(__i386__) || defined(__x86_64__) || defined(_M_X64)  || defined(_M_IX86)
size_t v8ndec256v32(  unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op;       _V8D( in, n, out, 256, 32,         bitunpack256v,  bitunpack); }
size_t v8nddec256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 256, 32, v8ddec, bitdunpack256v, bitdunpack, 0); }
size_t v8nd1dec256v32(unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 256, 32, v8d1dec,bitd1unpack256v,bitd1unpack,1); }
size_t v8nzdec256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 256, 32, v8zdec, bitzunpack256v, bitzunpack, 2); }
size_t v8nxdec256v32( unsigned char *__restrict in, size_t n, uint32_t *__restrict out) { uint32_t *op,start; _V8DD(in, n, out, 256, 32, v8xdec, bitxunpack256v, bitxunpack, 2); }
    #endif
