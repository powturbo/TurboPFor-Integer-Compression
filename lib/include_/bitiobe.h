#include <stdint.h>
#include "../include_/conf.h"

#define bitput_t  					     T3(uint, __WORDSIZE, _t)
#define bitebr_t                         unsigned char
#define bitedef(  _bw_,_br_)             bitput_t _bw_; bitebr_t _br_
#define biteini(  _bw_,_br_)             _bw_ = 0, _br_ = sizeof(bitput_t)*8
#define bitrest(  _bw_, _br_) 			 (sizeof(_bw_)*8+7-_br_)
#define bitput(   _bw_, _br_, _nb_, _x_) _bw_ |= (bitput_t)(_x_) << (_br_ -= (_nb_))
//------------------------------------- bitget --------------------------------------------
#define bitget_t  					     T3(uint, __WORDSIZE, _t)
#define bitdbr_t                         unsigned char
#define bitddef(  _bw_,_br_)             bitget_t _bw_; bitdbr_t _br_
#define bitdini(  _bw_,_br_)             _bw_ = _br_ = 0
#define bitbw(    _bw_,_br_)             (_bw_ << _br_) //  _bw_//

#define bitrmv(   _bw_,_br_,_nb_)        (_br_ += (_nb_)) // _bw_ <<= (_nb_), _br_ += (_nb_)//
#define bitpeek(   _bw_,_br_,_nb_)       (bitbw(_bw_,_br_)>>(sizeof(_bw_)*8-  (_nb_)))
#define bitpeek32( _bw_,_br_,_nb_)       bitpeek(_bw_,_br_,_nb_)

#define BITPEEK32(   _bw_,_br_,_nb_)     bitpeek32( _bw_,_br_,_nb_)

#define bitpeek0( _bw_,_br_,_nb_)        (bitbw(_bw_,_br_)>>1)>>(sizeof(_bw_)*8-1-(_nb_))
#define bitget(   _bw_,_br_,_nb_,_x_)    _x_ = bitpeek(_bw_, _br_, _nb_), bitrmv(_bw_, _br_, _nb_)
#define bitget32( _bw_,_br_,_nb_,_x_)    bitget(   _bw_,_br_,_nb_,_x_)
#define BITGET32( _bw_,_br_,_nb_,_x_)    bitget(   _bw_,_br_,_nb_,_x_)
//------------------------
#define biteinir(    _bw_,_br_,  _op_)   biteini(_bw_,_br_),(_op_) -= sizeof(_bw_)
#define bitenormr(   _bw_, _br_, _op_)   do { *(bitput_t *)_op_ = _bw_; unsigned _b = (sizeof(_bw_)*8  -_br_)&~7; (_op_) -= _b>>3; _bw_ <<= _b; _br_ += _b;  } while(0)
#define bitflushr(   _bw_, _br_, _op_)   do { *(bitput_t *)_op_ = _bw_; unsigned _b = (sizeof(_bw_)*8+7-_br_)&~7; (_op_) -= _b>>3; (_op_) += sizeof(_bw_); } while(0)

#define _bitenormr64(_bw_,_br_,_op_)
#define _bitenormr32(_bw_,_br_,_op_)     bitenormr( _bw_, _br_, _op_)
#define bitenormr2(  _bw_,_br_,_op_)     T2(_bitenormr,__WORDSIZE)(_bw_,_br_, _op_)
//----------------
#define bitdinir(    _bw_,_br_,_ip_)     bitdini(_bw_,_br_),_ip_ -= sizeof(_bw_)
#define bitalignr(   _bw_,_br_,_ip_)     _ip_ += sizeof(_bw_)-((_br_+7)>>3), _br_ = sizeof(_bw_)*8, _bw_=0
#define bitdnormr(   _bw_,_br_,_ip_)     _bw_  = *(bitget_t *)(_ip_ -= _br_>>3), _br_ &= 7 //, _bw_  <<= _br_

#define _bitdnormr64(_bw_,_br_,_ip_)
#define _bitdnormr32(_bw_,_br_,_ip_)     bitdnormr( _bw_, _br_, _ip_)
#define bitdnormr2(  _bw_,_br_,_ip_)     T2(_bitdnormr,__WORDSIZE)( _bw_, _br_, _ip_)
