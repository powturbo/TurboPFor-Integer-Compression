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
//    vint.h - "Integer Compression" variable byte include header 
#ifndef VINT_H
#define VINT_H
#include <stdint.h>
#include "conf.h"

#ifdef __cplusplus
extern "C" {
#endif
//----------------------------------- Variable byte single value macros (low level) -----------------------------------------------
//------------- 32 bits -------------
#define _vbput32(_op_, _x_, _act_) {\
       if(likely(_x_ < (1<< 7))) {		  *_op_++ = _x_; 			 		                                                  _act_;}\
  else if(likely(_x_ < (1<<14))) { ctou16(_op_)   =                    bswap16(_x_| 0x8000);                       _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<21))) {        *_op_++ =                    _x_ >> 16  | 0xc0;      ctou16(_op_) = _x_; _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<28))) { ctou32(_op_)   =                    bswap32(_x_| 0xe0000000);                   _op_ += 4; _act_;}\
  else { 		                          *_op_++ = (unsigned long long)_x_ >> 32 | 0xf0;      ctou32(_op_) = _x_; _op_ += 4; _act_;}\
}

#define _vbget32(_ip_, _x_, _act_) do { _x_ = *_ip_++;\
       if(!(_x_ & 0x80)) {   								        							 _act_;}\
  else if(!(_x_ & 0x40)) { _x_ = bswap16(ctou16(_ip_++-1) & 0xff3f);						     _act_;}\
  else if(!(_x_ & 0x20)) { _x_ = (_x_ & 0x1f)<<16 | ctou16(_ip_); 	    		      _ip_ += 2; _act_;}\
  else if(!(_x_ & 0x10)) { _x_ = bswap32(ctou32(_ip_-1) & 0xffffff0f) ;				  _ip_ += 3; _act_;}\
  else 			   	     { _x_ = (unsigned long long)(_x_ & 0x07)<<32 | ctou32(_ip_); _ip_ += 4; _act_;}\
} while(0)

#define _vblen32(_x_) ((_x_) >= (1<<7)?((_x_) >= (1<<14)?((_x_) >= (1<<21)?((_x_) >= (1<<28)?5:4):3):2):1)
#define _vbvlen32(_x_) _vtab32_[((unsigned char)(_x_))>>4]
//------------- 64 bits -----------
#define _vbput64(_op_, _x_, _act_) {\
       if(likely(_x_ < (1<< 7))) {		  *_op_++ = _x_; 			 		                                                                    _act_;}\
  else if(likely(_x_ < (1<<14))) { ctou16(_op_)   =                    bswap16(_x_| 0x8000);                                           _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<21))) {        *_op_++ =                    _x_ >> 16  | 0xc0;      ctou16(_op_) = _x_;                     _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<28))) { ctou32(_op_)   =                    bswap32(_x_| 0xe0000000);                                       _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<35) {        *_op_++ =         _x_ >> 32 | 0xf0;                                      ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<42) { ctou16(_op_)   = bswap16(_x_ >> 32 | 0xf800);                        _op_ += 2; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<49) {        *_op_++ =         _x_ >> 48 | 0xfc; ctou16(_op_) = _x_ >> 32; _op_ += 2; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<56) { ctou64(_op_)   = bswap64(_x_       | 0xfe00000000000000ull); 								       _op_ += 8; _act_;}\
  else {                                  *_op_++ =                     0xff;                                      ctou64(_op_) = _x_; _op_ += 8; _act_;}\
}

#define _vbget64(_ip_, _x_, _act_) do { _x_ = *_ip_++;\
       if(!(_x_ & 0x80)) {   								        							                          _act_;}\
  else if(!(_x_ & 0x40)) { _x_ = bswap16(ctou16(_ip_++-1) & 0xff3f);						                              _act_;}\
  else if(!(_x_ & 0x20)) { _x_ = (_x_ & 0x1f)<<16 | ctou16(_ip_); 	    		                               _ip_ += 2; _act_;}\
  else if(!(_x_ & 0x10)) { _x_ = bswap32(ctou32(_ip_-1) & 0xffffff0f);				                           _ip_ += 3; _act_;}\
  else if(!(_x_ & 0x08)) { _x_ = (_x_ & 0x07)<<32 | ctou32(_ip_);                                              _ip_ += 4; _act_;}\
  else if(!(_x_ & 0x04)) { _x_ = (unsigned long long)(bswap16(ctou16(_ip_-1)) & 0x7ff) << 32 | ctou32(_ip_+1); _ip_ += 5; _act_;}\
  else if(!(_x_ & 0x02)) { _x_ = (_x_ & 0x03)<<48 |   (unsigned long long)ctou16(_ip_) << 32 | ctou32(_ip_+2); _ip_ += 6; _act_;}\
  else if(!(_x_ & 0x01)) { _x_ = bswap64(ctou64(_ip_-1)) & 0x01ffffffffffffffull;                              _ip_ += 7; _act_;}\
  else                   { _x_ = ctou64(_ip_); _ip_ += 8; _act_;}\
} while(0)


#define _vbput640(_op_, _x_, _act_) {\
       if(_x_ < 1   << 7) {		   		        *_op_++ = _x_ << 1; 			 		                                         _act_;}\
  else if(_x_ < 1   <<14) { ctou16(_op_) = _x_ << 2 | 0x01; _op_ += 2; 		             	                                     _act_;}\
  else if(_x_ < 1   <<21) { ctou16(_op_) = _x_ << 3 | 0x03; _op_ += 2;      *_op_++ = _x_ >> 13;                                 _act_;}\
  else if(_x_ < 1   <<28) { ctou32(_op_) = _x_ << 4 | 0x07; _op_ += 4; 		                                                     _act_;}\
  else if(_x_ < 1ull<<35) { ctou32(_op_) = _x_ << 5 | 0x0f; _op_ += 4;      *_op_++ = _x_ >> 27;                                 _act_;}\
  else if(_x_ < 1ull<<42) { ctou32(_op_) = _x_ << 6 | 0x1f; _op_ += 4; ctou16(_op_) = _x_ >> 26;                        _op_+=2; _act_;}\
  else if(_x_ < 1ull<<49) { ctou32(_op_) = _x_ << 7 | 0x3f; _op_ += 4; ctou16(_op_) = _x_ >> 25; *(_op_+2) = _x_ >> 41; _op_+=3; _act_;}\
  else if(_x_ < 1ull<<56) { ctou64(_op_) = _x_ << 8 | 0x7f; _op_ += 8;                                                           _act_;}\
  else {                         *_op_++ =            0xff;            ctou64(_op_) = _x_;                              _op_+=8; _act_;}\
}

#define _vbget640(_ip_, _x_, _act_) do {\
  if(!((_x_ = *_ip_) & 1<<0)) {  _ip_++; _x_	   >>= 1; 		                                                _act_;}\
  else if(!(_x_      & 1<<1)) { _x_ = ctou16(_ip_) >>  2;		                                     _ip_ += 2; _act_;}\
  else if(!(_x_      & 1<<2)) { _x_ = ctou16(_ip_) >>  3 | *(_ip_+2) << 13;                          _ip_ += 3; _act_;}\
  else if(!(_x_      & 1<<3)) { _x_ = ctou32(_ip_) >>  4; 		      	                             _ip_ += 4; _act_;}\
  else if(!(_x_      & 1<<4)) { _x_ = ctou32(_ip_) >>  5 | (unsigned long long)_ip_[4]        << 27; _ip_ += 5; _act_;}\
  else if(!(_x_      & 1<<5)) { _x_ = ctou32(_ip_) >>  6 | (unsigned long long)ctou16(_ip_+4) << 26; _ip_ += 6; _act_;}\
  else if(!(_x_      & 1<<6)) { _x_ = ctou32(_ip_) >>  7 | (unsigned long long)ctou16(_ip_+4) << 25 | (unsigned long long)(_ip_[6]) << 41; _ip_ += 7; _act_;}\
  else if(!(_x_      & 1<<7)) { _x_ = ctou64(_ip_) >>  8; 		                                     _ip_ += 8; _act_;}\
  else 			   	          { _x_ = ctou64(_ip_+1);                                                _ip_ += 9; _act_;}\
} while(0)


#define _vblen64(_x_) ((_x_) >= (1<<7)?((_x_) >= (1<<14)?((_x_) >= (1<<21)?((_x_) >= (1<<28)?((_x_) >= (1ull<<35)?((_x_) >= (1ull<<42)?((_x_) >= (1ull<<49)?((_x_) >= (1ull<<56)?9:8):7):6):5):4):3):2):1)
#define _vbvlen64(_x_) _vtab64_[_x_]

//------------- 16 bits -----------
#define _vbput16(_op_, _x_, _act_) _vbput32(_op_, _x_, _act_)
#define _vbget16(_ip_, _x_, _act_) _vbget32(_ip_, _x_, _act_)

//------------- 15 bits -----------
#define _vbput15(_op_, _x_, _act_) do { if(likely((_x_) < 0x80)) { *_op_++ = _x_; _act_; } else { *_op_++ = (_x_) >> 8 | 0x80; *_op_++ = _x_; } } while(0)
#define _vbget15(_ip_, _x_, _act_) do { if(!((_x_ = *_ip_++) & 0x80)) _x_ = (_x_ & 0x7f) << 8 | *_ip_++; _act_; } while(0)
#define _vblen15(_x_) ((_x_) >= 0x80?2:1)

//----------------------------- Variable byte functions -----------------------------------------------------------------------
// ---- Variable byte length after compressing value _x_
static inline unsigned vblen64(uint64_t       x) { return _vblen64(x); }
static inline unsigned vblen32(unsigned       x) { return _vblen32(x); }
static inline unsigned vblen16(unsigned short x) { return _vblen32(x); }
static inline unsigned vblen15(unsigned short x) { return _vblen15(x); }

// ---- Length of compressed value. Input in is the compressed buffer start
static inline unsigned vbvlen64(unsigned char *in) { return in[0]==0xff?9:clz32(in[0] ^ 0xff) - 23; }
static inline unsigned vbvlen32(unsigned char *in) { return clz32((in[0] ^ 0xff) | 0x08) - 23; }
#define vbvlen16(p) vbvlen32(p)
static inline unsigned vbvlen15(unsigned char *in) { return (in[0] >> 7)+1; }

//----- encode/decode 16/32/64 single value and advance output/input pointer 
#define vbput64(_op_, _x_) { unsigned long long _x = _x_; _vbput64(_op_, _x, ;); }
#define vbput32(_op_, _x_) { register unsigned  _x = _x_; _vbput32(_op_, _x, ;); }
#define vbput16(_op_, _x_)   vbput32(_op_, _x_)
#define vbput15(_op_, _x_) { unsigned _x = _x_; _vbput15(_op_, _x, ;); }

#define vbget64(_ip_)     ({ unsigned long long _x;       _vbget64(_ip_, _x, ;); _x; })
#define vbget32(_ip_)     ({ register unsigned  _x;       _vbget32(_ip_, _x, ;); _x; })
#define vbget16(_ip_)       vbget32(_ip_)
#define vbget15(_ip_)     ({ unsigned _x;                 _vbget15(_ip_, _x, ;); _x; })

// ---- Encode array with n integer values to the buffer out. 	    Return value = end of compressed output buffer out
unsigned char *vbenc64(  uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbenc32(  unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbenc16(  unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbenc15(  unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out); // 15 bits range (0-0x7fff)

//----- Decode       											    Return value = end of compressed input buffer in 
unsigned char *vbdec64(  unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out);
unsigned char *vbdec32(  unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out);
unsigned char *vbdec16(  unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out);
unsigned char *vbdec15(  unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out);

//----- Delta encoding for increasing integer lists.          	    Return value = end of compressed output buffer out
unsigned char *vbdenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbdenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbdenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbdenc15( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);

//----- Delta decode          								        Return value = end of compressed input buffer in
unsigned char *vbddec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);
unsigned char *vbddec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);
unsigned char *vbddec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *vbddec15( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

//----- Delta encoding for strictly increasing (never remaining constant or decreasing) integer lists. Return value = end of compressed output buffer out
unsigned char *vbd1enc64(uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbd1enc32(unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbd1enc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbd1enc15(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);

//----- Delta decode          								        Return value = end of compressed input buffer in
unsigned char *vbd1dec64(unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);
unsigned char *vbd1dec32(unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);
unsigned char *vbd1dec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *vbd1dec15(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

//----- Zigzag encoding for unsorted integer lists.                 Return value = end of compressed output buffer out
unsigned char *vbzenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbzenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbzenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbzenc15( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);

//----- Zigzag decode          								        Return value = end of compressed input buffer in
unsigned char *vbzdec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);
unsigned char *vbzdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);
unsigned char *vbzdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *vbzdec15( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

#ifdef __cplusplus
}
#endif

#endif
