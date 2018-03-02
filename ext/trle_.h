/**
    Copyright (C) powturbo 2015-2018
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

    TurboRLE - "Most efficient and fastest Run Length Encoding"
**/
//------------------------- Variable Byte from https://github.com/powturbo/TurboPFor -----------------------------------------------------
#include "../conf.h"
#define VB_SIZE 64
#define VB_MAX 254
#define VB_B2 6
#define VB_B3 3
#define VB_BA3 (VB_MAX - (VB_SIZE/8 - 3))
#define VB_BA2 (VB_BA3 - (1<<VB_B3))  

#define VB_OFS1 (VB_BA2 - (1<<VB_B2))
#define VB_OFS2 (VB_OFS1 + (1 << (8+VB_B2)))
#define VB_OFS3 (VB_OFS2 + (1 << (16+VB_B3)))

#define _vblen32(_x_)  ((_x_) < VB_OFS1?1:((_x_) < VB_OFS2?2:((_x_) < VB_OFS3)?3:(bsr32(_x_)+7)/8+1))
#define _vbvlen32(_x_) ((_x_) < VB_OFS1?1:((_x_) < VB_BA2?2:((_x_) < VB_BA3)?3:(_x_-VB_BA3)))

#define _vbput32(_op_, _x_, _act_) {\
  if(likely((_x_) < VB_OFS1)){ *_op_++ = (_x_);																	_act_;}\
  else if  ((_x_) < VB_OFS2) { ctou16(_op_) = bswap16((VB_OFS1<<8)+((_x_)-VB_OFS1));             _op_  += 2; /*(_x_) -= VB_OFS1; *_op_++ = VB_OFS1 + ((_x_) >> 8); *_op_++ = (_x_);*/ _act_; }\
  else if  ((_x_) < VB_OFS3) { *_op_++ = VB_BA2 + (((_x_) -= VB_OFS2) >> 16); ctou16(_op_) = (_x_); _op_  += 2;  _act_;}\
  else { unsigned _b = (bsr32((_x_))+7)/8; *_op_++ = VB_BA3 + (_b - 3);    ctou32(_op_) = (_x_); _op_  += _b; _act_;}\
}

#define _vbget32(_ip_, _x_, _act_) do { _x_ = *_ip_++;\
       if(likely(_x_ < VB_OFS1)) { _act_ ;}\
  else if(likely(_x_ < VB_BA2))  { _x_ = /*bswap16(ctou16(_ip_-1))*/ ((_x_<<8) + (*_ip_)) + (VB_OFS1 - (VB_OFS1 <<  8)); _ip_++; _act_;} \
  else if(likely(_x_ < VB_BA3))  { _x_ = ctou16(_ip_) + ((_x_ - VB_BA2 ) << 16) + VB_OFS2; _ip_ += 2; _act_;}\
  else { unsigned _b = _x_-VB_BA3; _x_ = ctou32(_ip_) & ((1u << 8 * _b << 24) - 1); _ip_ += 3 + _b; _act_;}\
} while(0)

#define vbput32(_op_, _x_) { register unsigned  _x = _x_; _vbput32(_op_, _x, ;); }
#define vbget32(_ip_, _x_) _vbget32(_ip_, _x_, ;)

#define vbzput(_op_, _x_, _m_, _emap_) do { if(unlikely((_x_) < _m_)) *_op_++ = _emap_[_x_]; else { unsigned _xi = (_x_) - _m_; *_op_++ = _emap_[_m_]; vbput32(_op_, _xi); } } while(0)
#define vbzget(_ip_, _x_, _m_, _e_) { _x_ = _e_; if(unlikely(_x_ == _m_)) { vbget32(_ip_,_x_); _x_+=_m_; } }

#define TMIN 3
