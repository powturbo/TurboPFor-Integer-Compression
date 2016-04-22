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

#ifdef __cplusplus
extern "C" {
#endif
//----------------------------------- Variable byte single value macros (low level) -----------------------------------------------
//------------- 32 bits -------------
extern unsigned char vtab[];
#define _vbput32(_op_, _x_, _act_) {\
       if(likely(_x_ < (1<< 7))) {		  *_op_++ = _x_; 			 		                                             _act_;}\
  else if(likely(_x_ < (1<<14))) { ctou16(_op_)   = _x_ << 8 |  _x_ >>  8         | 0x80;                     _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<21))) {        *_op_++ = _x_ >> 16                     | 0xc0; ctou32(_op_) = _x_; _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<28))) { ctou32(_op_)   = rol32(_x_,8)                  | 0xe0;                     _op_ += 4; _act_;}\
  else { 		                          *_op_++ = (unsigned long long)_x_ >> 32 | 0xf0; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
}

#define _vbget32(_ip_, _x_, _act_) do { _x_ = *_ip_++;\
       if(!(_x_ & 0x80)) {   								        							 _act_;}\
  else if(!(_x_ & 0x40)) { _x_ = (_x_ & 0x3f)<< 8 | *_ip_++; 								     _act_;}\
  else if(!(_x_ & 0x20)) { _x_ = (_x_ & 0x1f)<<16 | ctou16(_ip_); 	    		      _ip_ += 2; _act_;}\
  else if(!(_x_ & 0x10)) { _x_ = ror32(ctou32(_ip_-1),8) & 0xfffffff;				  _ip_ += 3; _act_;}\
  else 			   	     { _x_ = (unsigned long long)(_x_ & 0x07)<<32 | ctou32(_ip_); _ip_ += 4; _act_;}\
} while(0)

//------------- 64 bits -----------
#define _vbput64(_op_, _x_, _act_) {\
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

#define _vbget64(_ip_, _x_, _act_) do {\
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

//------------- 16 bits -----------
#define _vbput16(_op_, _x_, _act_) _vbput32(_op_, _x_, _act_)
#define _vbget16(_ip_, _x_, _act_) _vbget32(_ip_, _x_, _act_)

//------------- 15 bits -----------
#define _vbput15(_op_, _x_, _act_) do { if(likely((_x_) < 0x80)) { *_op_++ = _x_; _act_; } else { *_op_++ = (_x_) >> 8 | 0x80; *_op_++ = _x_; } } while(0)
#define _vbget15(_ip_, _x_, _act_) do { if(!((_x_ = *_ip_++) & 0x80)) _x_ = (_x_ & 0x7f) << 8 | *_ip_++; _act_; } while(0)

//----------------------------- Variable byte functions -----------------------------------------------------------------------
// ---- Variable byte length after compressiing value _x_
#define vblen32(_x_) ({ unsigned _x = _x_; _x > 0x7f?(_x > 0x3fff?(_x > 0x1fffff?(_x > 0x0fffffff?5:4):3):2):1; })
#define vblen15(_x_) ((_x_) > 0x7f?2:1)

// ---- Length of compressed value. Input _x_ is the compressed buffer start
#define vbvlen32(_x_) vtab[((unsigned char)(_x_))>>4]

//----- encode/decode 16/32/64 single value and advance output/input pointer 
#define vbput64(_op_, _x_) { unsigned long long _x = _x_; _vbput64(_op_, _x, ;); }
#define vbput32(_op_, _x_) { register unsigned  _x = _x_; _vbput32(_op_, _x, ;); }
#define vbput16(_op_, _x_)   vbput32(_op_, _x_)
#define vbput15(_op_, _x_) { unsigned _x = _x_; _vbput15(_op_, _x, ;); }

#define vbget64(_ip_)     ({ unsigned long long _x;       _vbget64(_ip_, _x, ;); _x; })
#define vbget32(_ip_)     ({ register unsigned  _x;       _vbget32(_ip_, _x, ;); _x; })
#define vbget16(_ip_)       vbget32(_ip_)
#define vbget15(_ip_)     ({ unsigned _x;                 _vbget15(_ip_, _x, ;); _x; })

// ---- Encode array with n integer values to the buffer out. 	Return value = end of compressed buffer out
unsigned char *vbenc64(uint64_t        *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbenc32(unsigned        *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbenc16(unsigned short  *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbenc15(unsigned short  *__restrict in, unsigned n, unsigned char  *__restrict out);

//----- Decode       											Return value = end of decompressed buffer in 
unsigned char *vbdec64(unsigned char   *__restrict in, unsigned n, uint64_t       *__restrict out);
unsigned char *vbdec32(unsigned char   *__restrict in, unsigned n, unsigned       *__restrict out);
unsigned char *vbdec16(unsigned char   *__restrict in, unsigned n, unsigned short *__restrict out);
unsigned char *vbdec15(unsigned char   *__restrict in, unsigned n, unsigned short *__restrict out);

//----- Delta encoding for increasing integer lists.          	Return value = end of compressed buffer out
unsigned char *vbdenc64(uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t start);
unsigned char *vbdenc32(unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *vbdenc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbdenc15(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);

//----- Delta decode          								    Return value = end of decompressed buffer in
unsigned char *vbddec64(unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start);
unsigned char *vbddec32(unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);
unsigned char *vbddec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *vbddec15(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

//----- Delta encoding for strictly increasing (never remaining constant or decreasing) integer lists. Return value = end of compressed buffer out 
unsigned char *vbd1enc64(uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t start);
unsigned char *vbd1enc32(unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *vbd1enc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbd1enc15(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);

//----- Delta decode          								    Return value = end of decompressed buffer in
unsigned char *vbd1dec64(unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start);
unsigned char *vbd1dec32(unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);
unsigned char *vbd1dec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *vbd1dec15(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

//----- Zigzag encoding for unsorted integer lists.            Return value = end of compressed buffer out
unsigned char *vbzenc64(uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t start);
unsigned char *vbzenc32(unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *vbzenc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbzenc15(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);

//----- Zigzag decode          								  Return value = end of decompressed buffer in
unsigned char *vbzdec64(unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start);
unsigned char *vbzdec32(unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);
unsigned char *vbzdec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *vbzdec15(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

#ifdef __cplusplus
}
#endif

#endif
