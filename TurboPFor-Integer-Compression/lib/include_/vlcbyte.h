//---- "Integer Compression" scalar variable byte -------------------------------
#include "conf.h"
//----------------------------------- Variable byte: single value macros (low level) -----------------------------------------------
//------------- 32 bits -------------
extern unsigned char _vtab32_[];
#define _vbxvlen32(_x_) _vtab32_[(unsigned char)(_x_)>>4] // (clz32((_x_) ^ 0xff) - 23) //
#define _vbxlen32(_x_) ((bsr32(_x_|1)+6)/7)

#define _vbxput32(_op_, _x_, _act_) {\
       if(likely((_x_) < (1<< 7))) {        *_op_++ = _x_;                                               _act_;}\
  else if(likely((_x_) < (1<<14))) { ctou16(_op_)   = bswap16((_x_) | 0x8000u);               _op_ += 2; _act_;}\
  else if(likely((_x_) < (1<<21))) {        *_op_++ = _x_ >> 16  | 0xc0u; ctou16(_op_) = _x_; _op_ += 2; _act_;}\
  else if(likely((_x_) < (1<<28))) { ctou32(_op_)   = bswap32((_x_) | 0xe0000000u);              _op_ += 4; _act_;}\
  else                             {        *_op_++ = (unsigned long long)(_x_) >> 32 | 0xf0u; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
}

#define _vbxget32(_ip_, _x_, _act_) do { _x_ = (unsigned)(*_ip_++);\
       if(!(_x_ & 0x80u)) {                                                                         _act_;}\
  else if(!(_x_ & 0x40u)) { _x_ = bswap16(ctou16(_ip_ - 1) & 0xff3fu); _ip_++;                      _act_;}\
  else if(!(_x_ & 0x20u)) { _x_ = (_x_ & 0x1f)<<16 | ctou16(_ip_);                       _ip_ += 2; _act_;}\
  else if(!(_x_ & 0x10u)) { _x_ = bswap32(ctou32(_ip_-1) & 0xffffff0fu);                 _ip_ += 3; _act_;}\
  else                    { _x_ = (unsigned long long)((_x_) & 0x07)<<32 | ctou32(_ip_); _ip_ += 4; _act_;}\
} while(0)

//------------- 64 bits -----------
#define _vbxlen64(_x_)  ((bsr64(_x_)+6)/7)
#define _vbxvlen64(_x_) ((_x_)==0xff?9:clz32((_x_) ^ 0xff) - 23)

#define _vbxput64(_op_, _x_, _act_) {\
       if(likely(_x_ < (1<< 7))) {        *_op_++ = _x_;                                                                                          _act_;}\
  else if(likely(_x_ < (1<<14))) { ctou16(_op_)   =        bswap16(_x_| 0x8000);                                                       _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<21))) {        *_op_++ =        _x_ >> 16  | 0xc0;      ctou16(_op_) = _x_;                                 _op_ += 2; _act_;}\
  else if(likely(_x_ < (1<<28))) { ctou32(_op_)   =        bswap32(_x_| 0xe0000000);                                                   _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<35) {        *_op_++ =         _x_ >> 32 | 0xf0;                                      ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<42) { ctou16(_op_)   = bswap16(_x_ >> 32 | 0xf800);                        _op_ += 2; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<49) {        *_op_++ =         _x_ >> 48 | 0xfc; ctou16(_op_) = _x_ >> 32; _op_ += 2; ctou32(_op_) = _x_; _op_ += 4; _act_;}\
  else if(       _x_ < 1ull<<56) { ctou64(_op_)   = bswap64(_x_       | 0xfe00000000000000ull);                                        _op_ += 8; _act_;}\
  else {                                  *_op_++ =                     0xff;                                      ctou64(_op_) = _x_; _op_ += 8; _act_;}\
}

#define _vbxget64(_ip_, _x_, _act_) do { _x_ = *_ip_++;\
       if(!(_x_ & 0x80)) {                                                                                                _act_;}\
  else if(!(_x_ & 0x40)) { _x_ = bswap16(ctou16(_ip_++-1) & 0xff3f);                                                      _act_;}\
  else if(!(_x_ & 0x20)) { _x_ = (_x_ & 0x1f)<<16 | ctou16(_ip_);                                              _ip_ += 2; _act_;}\
  else if(!(_x_ & 0x10)) { _x_ = bswap32(ctou32(_ip_-1) & 0xffffff0f);                                         _ip_ += 3; _act_;}\
  else if(!(_x_ & 0x08)) { _x_ = (_x_ & 0x07)<<32 | ctou32(_ip_);                                              _ip_ += 4; _act_;}\
  else if(!(_x_ & 0x04)) { _x_ = (unsigned long long)(bswap16(ctou16(_ip_-1)) & 0x7ff) << 32 | ctou32(_ip_+1); _ip_ += 5; _act_;}\
  else if(!(_x_ & 0x02)) { _x_ = (_x_ & 0x03)<<48 |   (unsigned long long)ctou16(_ip_) << 32 | ctou32(_ip_+2); _ip_ += 6; _act_;}\
  else if(!(_x_ & 0x01)) { _x_ = bswap64(ctou64(_ip_-1)) & 0x01ffffffffffffffull;                              _ip_ += 7; _act_;}\
  else                   { _x_ = ctou64(_ip_);                                                                 _ip_ += 8; _act_;}\
} while(0)

#define vbxput64(_op_, _x_) { unsigned long long _x = _x_; _vbxput64(_op_, _x, ;); }
#define vbxput32(_op_, _x_) { register unsigned  _x = _x_; _vbxput32(_op_, _x, ;); }
#define vbxput16(_op_, _x_)   vbxput32(_op_, _x_)
#define vbxput8(  _op_, _x_)  (*_op_++ = _x_)

#define vbxget64(_ip_, _x_) _vbxget64(_ip_, _x_, ;)
#define vbxget32(_ip_, _x_) _vbxget32(_ip_, _x_, ;)
#define vbxget16(_ip_, _x_)  vbxget32(_ip_,_x_)
#define vbxget8(_ip_, _x_)       (_x_ = *_ip_++)

//-------------------------------------------------- small integers ----------------------------------------------------------------------------------------------------------------------------------------
#define _vsba2(_vsmax_,_vsb2_) (_vsmax_ - (1<<(_vsb2_)))  //max. integer vsmax=0xff:  vsb2=6: 4276351=41407f(22+ bits)  vsb2=5: 2171071=2120bf(21 bits)  vsb2=4:1118431=1110df(20bits)  vsb2=3:592111=908ef(19 bits)

#define _vso1(_vsmax_,_vsb2_) (_vsba2(_vsmax_,_vsb2_) - (1<<(_vsb2_)))
#define _vso2(_vsmax_,_vsb2_) (_vso1(_vsmax_,_vsb2_) + (1 << (8+(_vsb2_))))

#define _vsput(_op_, _x_, _vsmax_,_vsb2_, _act_) { \
  if(likely((_x_) < _vso1(_vsmax_,_vsb2_))){ *_op_++ = (_x_);																_act_	}\
  else if  ((_x_) < _vso2(_vsmax_,_vsb2_)) { ctou16(_op_) = bswap16((_vso1(_vsmax_,_vsb2_)<<8)+((_x_)-_vso1(_vsmax_,_vsb2_)));                _op_  += 2; _act_}\
  else                                       { *_op_++ = _vsba2(_vsmax_,_vsb2_) + (((_x_) -= _vso2(_vsmax_,_vsb2_)) >> 16); ctou16(_op_) = (_x_); _op_  += 2; _act_}\
}

#define _vsget(_ip_, _x_, _vsmax_,_vsb2_, _act_) do { _x_ = *_ip_++;\
       if(likely(_x_ < _vso1(_vsmax_,_vsb2_))) { _act_ }\
  else if(likely(_x_ < _vsba2(_vsmax_,_vsb2_)))  { _x_ = ((_x_<<8) + (*_ip_)) + (_vso1(_vsmax_,_vsb2_) - (_vso1(_vsmax_,_vsb2_) <<  8)); _ip_++; _act_} \
  else                                           { _x_ = ctou16(_ip_) + ((_x_ - _vsba2(_vsmax_,_vsb2_) ) << 16) + _vso2(_vsmax_,_vsb2_); _ip_ += 2; _act_}\
} while(0)

#define _vslen(_x_,_vsmax_,_vsb2_) ((_x_) < _vbo1(_vsmax_,_vsb2_)?1:((_x_) < _vbo2(_vsmax_,_vsb2_)?2):3)

 #ifndef NMACROS
#define VS22MAX            4276351
#define vslen22(_x_)       _vslen(_x_,0xff,6)
#define vsput22(_op_, _x_) _vsput(_op_, _x_, 0xff, 6, ;)
#define vsget22(_ip_, _x_) _vsget(_ip_, _x_, 0xff, 6, ;) 

#define VS21MAX            2171071
#define vslen21(_x_)       _vslen(_x_,       0xff, 5)
#define vsput21(_op_, _x_) _vsput(_op_, _x_, 0xff, 5, ;)
#define vsget21(_ip_, _x_) _vsget(_ip_, _x_, 0xff, 5, ;) 

#define VS20MAX            1118431
#define vslen20(_x_)       _vslen(_x_,0xff,4)
#define vsput20(_op_, _x_) _vsput(_op_, _x_, 0xff, 4, ;)
#define vsget20(_ip_, _x_) _vsget(_ip_, _x_, 0xff, 4, ;) 
 #else
 #endif

//----------------------------------------------------- 32/64 integer 1,2,3,4,5 bytes----------------------------------------------------------------------------------------
#define _vbba3(_vbsize_,_vbmax_)                    (_vbmax_ - (_vbsize_/8 - 3))
#define _vbba2(_vbsize_,_vbmax_,_vbb3_)             (_vbba3(_vbsize_,_vbmax_) - (1<<_vbb3_))  

#define _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)       (_vbba2( _vbsize_,_vbmax_,       _vbb3_) - (1<<_vbb2_))
#define _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_)       (_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_) + (1 << ( 8+_vbb2_)))
#define _vbo3(_vbsize_,_vbmax_,_vbb2_,_vbb3_)       (_vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_) + (1 << (16+_vbb3_)))

#define _vblen(_x_, _vbsize_,_vbmax_,_vbb2_,_vbb3_) ((_x_) < _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)?1:\
                                                    ((_x_) < _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_)?2:\
													((_x_) < _vbo3(_vbsize_,_vbmax_,_vbb2_,_vbb3_)?3:((T2(bsr,_vbsize_)(_x_)+7)/8+1))))
													
#define _vbvlen(_x_,_vbsize_,_vbmax_,_vbb2_,_vbb3_) ((_x_) < _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)?1:((_x_) < _vbba2(_vbsize_,_vbmax_,_vbb3_)?2:((_x_) < _vbba3(_vbsize_,_vbmax_))?3:((_x_)-_vbba3(_vbsize_,_vbmax_))))

#define _vbput(_op_, _x_, _vbsize_,_vbmax_,_vbb2_,_vbb3_, _act_) do {\
  if(likely((_x_) < _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_))){ *_op_++ = (_x_);																	                                                    _act_;}\
  else if  ((_x_) < _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_)) { ctou16(_op_) = bswap16((_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)<<8)+((_x_)-_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_)));           _op_ += 2; /*(_x_) -= _vbo1; *_op_++ = _vbo1 + ((_x_) >> 8); *_op_++ = (_x_);*/ _act_; }\
  else if  ((_x_) < _vbo3(_vbsize_,_vbmax_,_vbb2_,_vbb3_)) { *_op_++ = _vbba2(_vbsize_,_vbmax_,_vbb3_) + (((_x_) -= _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_)) >> 16); ctou16(_op_) = (_x_); _op_ += 2;  _act_;}\
  else { unsigned _b = (T2(bsr,_vbsize_)(_x_)+7)/8; *_op_++ = _vbba3(_vbsize_,_vbmax_) + (_b - 3); T2(ctou,_vbsize_)(_op_) = (_x_); _op_  += _b; _act_; }\
} while(0)

#define _vbget(_ip_, _x_, _vbsize_,_vbmax_,_vbb2_,_vbb3_, _act_) do { _x_ = *_ip_++;\
       if(likely(_x_ < _vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_))) { _act_ ;}\
  else if(likely(_x_ < _vbba2( _vbsize_,_vbmax_,_vbb3_)))      { _x_ = /*bswap16(ctou16(_ip_-1))*/ ((_x_<<8) + (*_ip_)) + (_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_) - (_vbo1(_vbsize_,_vbmax_,_vbb2_,_vbb3_) <<  8)); _ip_++; _act_;} \
  else if(likely(_x_ < _vbba3( _vbsize_,_vbmax_)))             { _x_ = ctou16(_ip_) + ((_x_ - _vbba2(_vbsize_,_vbmax_,_vbb3_) ) << 16) + _vbo2(_vbsize_,_vbmax_,_vbb2_,_vbb3_); _ip_ += 2; _act_;}\
  else { unsigned _b = _x_-_vbba3(_vbsize_,_vbmax_);  _x_ = T2(ctou,_vbsize_)(_ip_) & ((1ull << 8 * _b << 24) - 1); _ip_ += 3 + _b; _act_;}\
} while(0)

#ifndef VB_MAX
#define VB_MAX 0xff
#endif

  #ifndef NMACROS
//-- 64 bits -----  
#define vblen64(_x_)               _vblen( _x_,      64, VB_MAX, 6, 5) 
#define vbvlen64(_x_)              _vbvlen(_x_,      64, VB_MAX, 6, 5) 
#define _vbput64(_op_, _x_, _act_) _vbput(_op_, _x_, 64, VB_MAX, 6, 5, _act_)
#define _vbget64(_ip_, _x_, _act_) _vbget(_ip_, _x_, 64, VB_MAX, 6, 5, _act_) 
#define vbput64(_op_, _x_)         do { unsigned long long _x = _x_; _vbput64(_op_, _x_, ;); } while(0)
#define vbget64(_ip_, _x_)         _vbget64(_ip_, _x_, ;)
//-- 32 bits -----
#define vblen32(      _x_)         _vblen(      _x_, 32, VB_MAX, 6, 5) 
#define vbvlen32(     _x_)         _vbvlen(     _x_, 32, VB_MAX, 6, 5) 
#define _vbput32(_op_, _x_, _act_) _vbput(_op_, _x_, 32, VB_MAX, 6, 5, _act_)
#define _vbget32(_ip_, _x_, _act_) _vbget(_ip_, _x_, 32, VB_MAX, 6, 5, _act_) 
#define vbput32(_op_, _x_)         do { unsigned _x = _x_; _vbput32(_op_, _x, ;); } while(0)
#define vbget32(_ip_, _x_)         _vbget32(_ip_, _x_, ;) 
//-- 16 bits -----
#define vblen16( _x_)              vblen32(_x_)
#define vbvlen16(_x_)              vbvlen32(_x_)
#define _vbput16(_op_, _x_, _act_) _vbput32(_op_, _x_, _act_)
#define _vbget16(_ip_, _x_, _act_) _vbget32(_ip_, _x_, _act_)
#define vbput16(_op_, _x_)         vbput32(_op_, _x_)
#define vbget16(_ip_, _x_)         vbget32(_ip_, _x_)
//-- 8 bits ----
#define vblen8(_x_)  1
#define _vbput8(_op_, _x_, _act_)  { *_op_++ = _x_; _act_; }
#define _vbget8(_ip_, _x_, _act_)  { _x_ = *_ip_++; _act_; }
#define vbvlen8(_x_) 1

#define vllen32(_x_)                                _vblen(       _x_, 32, VB_MAX, 4, 3) 
#define vlput32(_op_, _x_)  do { unsigned _x = _x_; _vbput(_op_, _x,  32, VB_MAX, 4, 3, ;); } while(0)
#define vlget32(_ip_, _x_)                          _vbget(_ip_, _x_, 32, VB_MAX, 4, 3, ;) 
  #else
static ALWAYS_INLINE unsigned vblen32(unsigned       x) { return _vblen(      x, 32, VB_MAX, 6, 5); }
#define vbput32(_op_, _x_) _vbput(_op_, _x_, 32, VB_MAX, 6, 5, ;)
static ALWAYS_INLINE void vbget32(unsigned char **_ip, unsigned *_x)  { unsigned char *ip = *_ip; unsigned x; _vbget(ip, x, 32, VB_MAX, 6, 5, ;) ; *_ip = ip; *_x = x; }
static ALWAYS_INLINE unsigned vlget32(unsigned char **_ip)  { unsigned char *ip = *_ip; unsigned x; _vbget(ip, x, 32, VB_MAX, 4, 3, ;); *_ip = ip; return x; }
static ALWAYS_INLINE unsigned vllen32(unsigned       x) { return _vblen(      x, 32, VB_MAX, 4, 3); }
  #endif
