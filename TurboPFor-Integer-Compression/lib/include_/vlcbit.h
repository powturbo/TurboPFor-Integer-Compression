//-- Turbo VLC : Novel Variable Length Coding for large integers with exponent + mantissa
#include "conf.h"

// Exponent base for the bit size vlcbits: 1=63 2=123, 3=239 4=463 5=895 6=1727 7=3327
#define VLC_VN6    ( 6-5)
#define VLC_VN7    ( 7-5)
#define VLC_VN8    ( 8-5)
#define VLC_VN9    ( 9-5)
#define VLC_VN10   (10-5)
#define VLC_VN11   (11-5)
#define VLC_VN12   (12-5)
#define VLC_VN15   (15-5)
#define VLC_VN16   (16-5)

#define VLC_VB6    0
#define VLC_VB7    4
#define VLC_VB8   16
#define VLC_VB9   48
#define VLC_VB10 128
#define VLC_VB11 296
#define VLC_VB12 768
#define VLC_VB16 768 //???

#define vlcbits(_vn_)               (5+_vn_)
#define vlcfirst(_vn_)              (1u<<(_vn_+1)) //1,0,4  2,4,8  3,16,16
#define vlcmbits(_expo_, _vn_)      (((_expo_) >> _vn_)-1)
#define _vlcexpo_(_x_, _vn_,_expo_) { unsigned _f = __bsr32(_x_)-_vn_; _expo_ = ((_f+1)<<_vn_) + bextr32((_x_),_f,_vn_); }
#ifndef VLCBIT_H_
#define VLCBIT_H_
static inline int vlcexpo(unsigned x, unsigned vn) { unsigned expo; _vlcexpo_(x, vn, expo); return expo; }
#endif

// return exponent, mantissa + bits length for x the value
#define vlcenc( _x_, _vn_,_expo_, _mb_, _ma_) { unsigned _x = _x_; _vlcexpo_(_x, _vn_, _expo_); _mb_ = vlcmbits(_expo_, _vn_); _ma_ = bzhi32(_x,_mb_); }

// build value from exponent, mantissa + length
#define vlcdec(_expo_, _mb_, _ma_, _vn_) ((((1u << _vn_) + bzhi32(_expo_,_vn_))<<(_mb_)) + (_ma_))

// encode the mantissa in bitio (R->L) and return the exponent in u
#define bitvrput(_bw_,_br_,_ep_, _vn_,_vb_, _u_) do { \
  if((_u_) >= vlcfirst(_vn_)+_vb_) {\
    unsigned _expo, _mb, _ma;\
    vlcenc((_u_)-_vb_, _vn_, _expo, _mb, _ma); \
    bitput(_bw_,_br_, _mb, _ma); bitenormr(_bw_,_br_,_ep_);\
	_u_ = _expo+_vb_;\
  }\
} while(0)

// get mantissa and bitio (R->L) decode value from x
/*#define bitvrget( _bw_,_br_,_ip_, _vn_,_vb_,_x_) \
  if(_x_ >= vlcfirst(_vn_)+_vb_) { \
    _x_ -= _vb_; \
	int _mb = vlcmbits(_x_, _vn_), _ma; \
    bitdnormr(_bw_,_br_,_ip_);\
	bitget(_bw_,_br_, _mb,_ma);\
	_x_ = vlcdec(_x_, _mb, _ma, _vn_)+_vb_;\
  }*/
#define bitvrget(_bw_,_br_,_ip_,_vn_,_vb_,_x_) \
  if(_x_ >= vlcfirst(_vn_)+_vb_) { \
    bitdnormr(_bw_,_br_,_ip_); \
    unsigned _mb = vlcmbits(_x_ -= _vb_, _vn_), _ma = bitpeek(_bw_,_br_, _mb); /*bitget(_bw_,_br_, _mb, _ma);*/\
    _x_ = vlcdec(_x_, _mb, _ma, _vn_)+_vb_; bitrmv(_bw_,_br_, _mb);\
  }

//--------- Expo & mantissa with bitio ------------------------------
#define bitvput(_bw_,_br_,_vn_,_vb_, _u_) do { \
  if((_u_) >= vlcfirst(_vn_)+_vb_) {\
    unsigned _expo, _mb, _ma;\
    vlcenc((_u_)-_vb_, _vn_, _expo, _mb, _ma); \
	bitput(_bw_,_br_, vlcbits(_vn_), _expo+_vb_); \
    bitput(_bw_,_br_, _mb, _ma); \
  } else bitput(_bw_,_br_, vlcbits(_vn_), _u_);\
} while(0)

#define bitvget( _bw_,_br_, _vn_,_vb_,_x_) do {\
  bitget(_bw_,_br_, vlcbits(_vn_),_x_);\
  if(_x_ >= vlcfirst(_vn_)+_vb_) { \
    _x_ -= _vb_; \
	int _mb = vlcmbits(_x_, _vn_), _ma; \
 	bitget(_bw_,_br_, _mb,_ma);\
	_x_ = vlcdec(_x_, _mb, _ma, _vn_)+_vb_;\
  }\
} while(0)

//--------- Branchless, expo with byte io, mantissa with bitio ---------------
#define bitvbput(_bw_,_br_,_cp_,_bp_,_vn_,_vb_, _u_) do { \
  unsigned _expo, _mb, _ma;\
  vlcenc((_u_)+vlcfirst(_vn_), _vn_, _expo, _mb, _ma); \
  *_cp_++ = _expo; \
  bitput(_bw_,_br_, _mb, _ma); \
} while(0)

#define bitvbget(_bw_,_br_,_cp_,_bp_,_vn_,_vb_,_x_) do { _x_ = *_cp_++; \
  unsigned _mb = vlcmbits(_x_, _vn_), _ma;\
  _ma = bitpeek(_bw_, _br_, _mb); bitrmv(_bw_, _br_, _mb);\
  _x_ = vlcdec(_x_, _mb, _ma, _vn_)-vlcfirst(_vn_);\
} while(0)

//--
#define bitvcput(_bw_,_br_,_cp_,_bp_,_vn_,_vb_, _u_) do { \
  if((_u_) >= vlcfirst(_vn_)+_vb_) {\
    unsigned _expo, _mb, _ma;\
    vlcenc((_u_)-_vb_, _vn_, _expo, _mb, _ma); \
	*_cp_++ = _expo+_vb_; \
    bitput(_bw_,_br_, _mb, _ma); /*bitenormr(_bw_,_br_,_bp_);*/\
  } else *_cp_++ = _u_; \
} while(0)

#define bitvcget(_bw_,_br_,_cp_,_bp_,_vn_,_vb_,_x_) do { /*bitdnormr(_bw_,_br_,_bp_);*/ _x_ = *_cp_++; \
  if(likely(_x_ >= vlcfirst(_vn_)+_vb_)) {\
    _x_ -= _vb_;\
	int _mb = vlcmbits(_x_, _vn_), _ma; /*_ma = bitpeek(_bw_, _br_, _mb); bitrmv(_bw_,_br_, _mb);*/\
	bitget(_bw_,_br_, _mb,_ma);\
	_x_ = vlcdec(_x_, _mb, _ma, _vn_)+_vb_;\
  }\
} while(0)


#define vhifirst(_k_) (1<<(_k_))

#define vhimbits(_expo_,_k_,_i_,_j_) (_k_ - (_i_ + _j_) + ((_expo_ - (1<<_k_)) >> (_i_ + _j_)))
// Hybrid integer https://www.lucaversari.it/phd/main.pdf
#define VHI_K 4
#define VHI_I 2
#define VHI_J 1

//#define _vlcexpo_(_x_, _k_,_i_,_j, _expo_) 
#define vhienc(_x_, _k_, _i_, _j_, _expo_, _mb_, _ma_) {\
  unsigned n = __bsr32(_x_), m = _x_ - (1 << n);\
  _expo_ = (1<<_k_) + ((n - _k_) << (_i_ + _j_)) + ((m >> (n - _i_)) << _j_) + BZHI32(m,_j_);\
  _mb_   = n - _i_ - _j_;\
  _ma_   = bzhi32(_x_ >> _j_,_mb_);\
}

#define vhidec(_x_, _mb_, _ma_, _k_, _i_, _j_) {\
  unsigned _mb_ = vhimbits(_x_,_k_,_i_,_j_), low_bits = _x_ & ((1 << _j_) - 1);\
  _x_ >>= _j_;\
  unsigned high_bits = (1 << _i_) | (_x_ & ((1 << _i_) - 1));\
  _x_ = (((high_bits << _mb_) | _ma_) << _j_) | low_bits;\
}

#define bithcput(_bw_,_br_,_cp_,_bp_,_k_,_i_,_j_, _u_) do { \
  if((_u_) >= vhifirst(_k_)) {\
    unsigned _expo, _mb, _ma;\
    vhienc(_u_, _k_, _i_, _j_, _expo, _mb, _ma); \
	*_cp_++ = _expo; \
    bitput(_bw_,_br_, _mb, _ma); /*bitenormr(_bw_,_br_,_bp_);*/\
  } else *_cp_++ = _u_; \
} while(0)

#define bithcget(_bw_,_br_,_cp_,_bp_,_k_,_i_,_j_,_x_) do { /*bitdnormr(_bw_,_br_,_bp_);*/ _x_ = *_cp_++; \
  if(likely(_x_ >= vhifirst(_k_))) {\
    unsigned _mb = _k_ - (_i_ + _j_) + ((_x_ - (1<<_k_)) >> (_i_ + _j_)), _ma; bitget(_bw_,_br_, _mb,_ma);\
    _x_ = (((((1 << _i_) | BZHI32(_x_ >> _j_,_i_)) << _mb) | _ma) << _j_) | BZHI32(_x_,_j_);\
  }\
} while(0)

