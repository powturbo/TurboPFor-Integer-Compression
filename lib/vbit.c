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
//   vbit.c - "Integer Compression" variable length coding with bitio
#pragma warning( disable : 4005)
#pragma warning( disable : 4090)
#pragma warning( disable : 4068)

#include <string.h>
#include "include_/conf.h"
#include "include_/vbit.h"

#include "include_/bitutil_.h"
#include "include_/bitiobe.h"
#include "include_/vlcbit.h"

#define OVERFLOW( _in_,_inlen_,_out_, _op_, _goto_) if( _op_                        >= _out_+((uint64_t)_inlen_*255)/256-8) { memcpy(_out_,_in_,_inlen_); _op_ = _out_+_inlen_; _goto_; }
#define OVERFLOWR(_in_,_inlen_,_out_, _op_, _op__, _goto_) if((_out_+_inlen_-_op__) >=       ((uint64_t)_inlen_*255)/256-8) { memcpy(_out_,_in_,_inlen_); _op_ = _out_+_inlen_; _goto_; }
// store the last bytes without encoding, when inlen is not multiple of array element size
#define INDEC  unsigned inlen  = _inlen /sizeof( in[0]); { unsigned char *p_=_in+_inlen,  *_p = _in +(_inlen & ~(sizeof(in[0] )-1)); while(_p < p_) { *op++  = *_p++; } }
#define OUTDEC unsigned outlen = _outlen/sizeof(out[0]); { unsigned char *p_=_out+_outlen,*_p = _out+(_outlen& ~(sizeof(out[0])-1)); while(_p < p_) *_p++  = *ip++; }

//-------- Gamma Coding: Full 32-bit range length limited gamma coding ---------------------------------------------------------------------
#define _bitgenc32(_bw_, _br_, _op_, _x_, _qmax_) do {\
  uint64_t _x = (uint64_t)(_x_)+1,\
           _q = __bsr64(_x), _log2m_ = _q; 												/*_x >> _log2m_ */\
  if(_q > _qmax_) {											           					/* quotient greater than limit _qmax_ */\
    unsigned _qx = _q - _qmax_, _qb = __bsr32(_qx)+1; 		           					/* (_q - _qmax_) size in bits */\
    bitput(_bw_,_br_, _qmax_+_qb+1, 1);                                             	/* encode (_qmax_+_qb) in unary coding */\
	bitput(_bw_,_br_, _qb-1+_log2m_, ((uint64_t)bzhi32(_qx,_qb-1))<<_log2m_ | bzhi32(_x,_log2m_) );\
	bitenormr(_bw_,_br_, _op_);                                                         /* encode _qb-1 lsb bits (without the msb bit) */\
  } else bitput(_bw_,_br_, _q+1+_log2m_, _x);      	                                    /* _q in unary coding */\
                                                                                        /* _log2m_ lsb bits of the remainder */\
} while(0)

#define _bitgdec32(_bw_, _br_, _ip_, _x_, _qmax_) do { \
  uint64_t _x; \
  unsigned _q = clz64(bitbw(_bw_,_br_));                                                /* read in unary */\
  if(_q > _qmax_) { \
    bitrmv(_bw_,_br_,_q+1);\
    unsigned _qb = _q - _qmax_; _q = 1;                              	                /* lsb bits length of the quotient */\
	if(_qb > 1) { bitget(_bw_,_br_,_qb-1, _q); _q = 1ull<<(_qb-1) | _q; }               /* decode lsb bits (msb always 1) */\
    _q += _qmax_;                                                                       /* build the quotient */\
    bitget(_bw_,_br_, _q, _x); _x = 1ull<<_q | _x;                                      /* decode mantissa _q bits */\
  } else bitget(_bw_,_br_, _q+_q+1, _x);\
  _x_ = _x - 1;\
} while(0)

#define QMAX_G 12
size_t bitgenc32(unsigned char *_in, size_t _inlen, unsigned char *out) {
  unsigned char *op = out+4, *out_ = out+_inlen, *bp = out_;
  uint32_t      *in = (uint32_t *)_in, *ip;
  INDEC;
  bitedef(bw,br); biteinir(bw,br,bp);

  for(ip = in; ip < in+inlen; ip++) {
	uint32_t x = ip[0];
	_bitgenc32(bw,br, bp, x, QMAX_G);                                        //bitgxput32(&bw, &br, QMAX_G, x, &bp); //  bitvcput(bw,br,op,bp,VLC_VN8,VLC_VB8, x); 	
	bitenormr(bw,br,bp);	                                                 OVERFLOWR(_in, _inlen, out, op,bp, goto e);
  }

  bitflushr(bw,br,bp);
  unsigned l = out_ - bp;
  memmove(op, bp, l); op += l;
  ctou32(out) = op - out;                                                    OVERFLOW(_in,_inlen,out, op, goto e);
  e:return op - out;
}

size_t bitgdec32(unsigned char *in, size_t _outlen, unsigned char *_out) {
  unsigned inlen = ctou32(in);
  unsigned char *ip = in+4,*ip_ = in+inlen;
  uint32_t      *out = (uint32_t *)_out, *op, x;
  OUTDEC;
  bitddef(bw, br); bitdinir(bw,br,ip_);

  for(op = out; op < out+outlen; op++) {
    bitdnormr(bw,br,ip_);
	_bitgdec32(bw, br, ip_, x, QMAX_G);                                      //bitgxget32(&bw, &br, QMAX_G, &x, &ip_);    bitvcget(bw,br,ip,ip_,VLC_VN8,VLC_VB8, x);
	*op = x;
  }

  bitalignr(bw, br, ip_);
  return ip_ - in;
}

//------------------- Golomb Rice Coding : _qmax_ length limited rice coding with rice parameter _log2m_ -----------------------------------------------
#define EMA(  _n_,_x_,_a_,_y_) (((_x_)*_a_ + ((1<<(_n_)) -_a_)*(_y_)) >>(_n_)) // Exponential moving average EMA2=1->2 EMA4=2->4 EMA8=3->8,...
#define RICEK(_x_)             __bsr32((_x_)+1)                                // Rice parameter

#define _bitrenc32(_bw_, _br_, _op_, _x_, _qmax_, _log2m_) {\
  unsigned      _x = _x_, _q = _x >> _log2m_;\
  if(_q > _qmax_) {											           		            /* quotient greater than limit _qmax_ */\
    unsigned _qb = __bsr32(_q -= _qmax_)+1; 	   				                        /*AS(_qmax_+_qb+1 < 46,"Fatal %d\n", _qmax_+_qb+1);*/	/* (_q - _qmax_) size in bits */\
    bitput(_bw_,_br_, _qmax_+_qb+1, 1);    bitenormr(_bw_,_br_,_op_);                   /* encode (_qmax_+_qb) in unary coding */\
	bitput(_bw_,_br_,_qb-1+_log2m_, bzhi32(_q,_qb-1)<<_log2m_ | bzhi32(_x,_log2m_));\
  } else bitput(_bw_,_br_,_q+1 +_log2m_ , 1u<<_log2m_ | bzhi32(_x,_log2m_));            /* _q in unary coding*/\
}

#define _bitrdec32(_bw_, _br_, _ip_, _x_, _qmax_, _log2m_) {\
  unsigned      _x, _q = clz64(bitbw(_bw_,_br_)); bitrmv(_bw_,_br_,_q+1); bitdnormr(_bw_,_br_,_ip_); \
  if(_q > _qmax_) {\
    int _qb = _q - _qmax_;                                                              /* lsb bits length of the quotient */\
	_x = 1; if(_qb > 1) { bitget(_bw_,_br_, _qb-1, _x); _x = 1u<<(_qb-1) | _x; }        /* decode lsb bits (msb always 1) */\
    _x += _qmax_;                                                                       /* build the quotient*/\
    _q  = _x;\
  } else _x = _q;\
  if(_log2m_) { bitget(_bw_,_br_,_log2m_, _q); _x = _x << _log2m_ | _q; }               /* decode mantissa _log2m_ bits */\
  _x_ = _x; \
}

#define QMAX_R 7
size_t bitrenc32(unsigned char *_in, size_t _inlen, unsigned char *out) {
  unsigned char *op = out+4, *out_ = out+_inlen, *op_ = out_;
  uint32_t      *in = (uint32_t *)_in, *ip, log2m=1,ema=0;
  INDEC;
  bitedef(bw,br); biteinir(bw,br,op_);

  for(ip = in; ip < in+inlen; ip++) {
	uint32_t x = ip[0];
	log2m = RICEK(ema);
	_bitrenc32(bw, br, op_, x, QMAX_R, log2m); bitenormr(bw,br,op_);      OVERFLOWR(_in, _inlen, out, op,op_, goto e);
	ema = EMA(5,ema, 23, x);                                              //  bitvcput(bw,br,op,op_,VLC_VN8,VLC_VB8, x); 		//bitrxput16(&bw, &br, QMAX_R, 4, x) ;
  }

  bitflushr(bw,br,op_);
  unsigned l = out_-op_;
  memmove(op, op_, l); op += l;
  ctou32(out) = op - out;                                                   OVERFLOW(_in,_inlen,out, op, goto e);
  e: return op - out;
}

size_t bitrdec32(unsigned char *in, size_t _outlen, unsigned char *_out) {
  unsigned      inlen = ctou32(in);
  unsigned char *ip   = in+4,*ip_ = in+inlen;
  uint32_t      *out  = (uint32_t *)_out, *op, x, log2m = 1,ema = 0;
  OUTDEC;
  bitddef(bw, br);
  bitdinir(bw,br,ip_);

  for(op = out; op < out+outlen; op++) {
    bitdnormr(bw,br,ip_);
	log2m = RICEK(ema);
	_bitrdec32(bw, br, ip_, x, QMAX_R, log2m);
	ema = EMA(5, ema, 23, x);	                                            //bitrxget32(&bw, &br, QMAX_R, 4, &x, &ip_); //    bitvcget(bw,br,ip,ip_,VLC_VN8,VLC_VB8, x);
	*op = x;
  }
  bitalignr(bw, br, ip_);
  return ip_ - in;
}

//-------- Turbo VLC: Variable Length Code for large integers: 16+32 bits similar to µ-Law/Extra Bit Codes encoding ------------------------
size_t vlcenc32(unsigned char *_in, size_t _inlen, unsigned char *out) {
  unsigned char *op   = out+4, *out_ = out+_inlen, *bp = out_;
  uint32_t      *in   = (uint32_t *)_in, *ip = in;
  INDEC;
  bitedef(bw,br);
  biteinir(bw,br,bp);

  #define VE(_i_) { bitvcput(bw,br,op,bp,VLC_VN8,0/*VLC_VB8*/, ip[_i_]); }
  for(; ip < in + (inlen&(~(4-1))); ip += 4) { VE(0); VE(1); bitenormr(bw,br,bp); VE(2); VE(3); bitenormr(bw,br,bp); if(bp <= op+20) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; } }
  for(; ip < in+inlen; ip++) { VE(0); bitenormr(bw,br,bp); if(bp <= op+5) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; }}

  bitflushr(bw,br,bp);
  unsigned l  = out_ - bp;
  if(op+l >= out+_inlen) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; }
  memmove(op, bp, l); op += l;
  ctou32(out) = op-out;
  e: return op - out;
}

size_t vlcdec32(unsigned char *in, size_t _outlen, unsigned char *_out) {
  unsigned      inlen = ctou32(in);
  unsigned char *ip = in+4, *bp = in+inlen;
  uint32_t      *out = (uint32_t *)_out, *op = out, x;
  OUTDEC;
  bitddef(bw, br); bitdinir(bw,br,bp);
  #define VD(_i_) { bitvcget(bw,br,ip,bp,VLC_VN8,0/*VLC_VB8*/, op[_i_]); }
  for(; op != out+(outlen&(~(4-1))); op+=4) { bitdnormr(bw,br,bp); VD(0); VD(1); bitdnormr(bw,br,bp); VD(2); VD(3); }
  for(; op != out+outlen; op++) { bitdnormr(bw,br,bp); VD(0); }
  return inlen+4;
}

size_t vlcenc16(unsigned char *_in, size_t _inlen, unsigned char *out) {
  unsigned char *op   = out+4, *out_ = out+_inlen, *bp = out_;
  uint16_t      *in   = (uint16_t *)_in, *ip = in;
  INDEC;
  bitedef(bw,br);
  biteinir(bw,br,bp);

  #define VE(_i_) { bitvput(bw,br,VLC_VN6,VLC_VB6, ip[_i_]); }
  for(; ip < in + (inlen&(~(4-1))); ip += 4) { VE(0); VE(1); bitenormr(bw,br,bp); VE(2); VE(3); bitenormr(bw,br,bp); if(bp <= op+20) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; } }
  for(; ip < in+inlen; ip++) { VE(0); bitenormr(bw,br,bp); if(bp <= op+5) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; }}

  bitflushr(bw,br,bp);
  unsigned l  = out_ - bp;
  if(op+l >= out+_inlen) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; }
  memmove(op, bp, l); op += l;
  ctou32(out) = op-out;
  e: return op - out;
}

size_t vlcdec16(unsigned char *in, size_t _outlen, unsigned char *_out) {
  unsigned      inlen = ctou32(in);
  unsigned char *ip = in+4, *bp = in+inlen;
  uint16_t      *out = (uint16_t *)_out, *op = out, x;
  OUTDEC;
  bitddef(bw, br); bitdinir(bw,br,bp);
  #define VD(_i_) { bitvget(bw,br,VLC_VN6,VLC_VB6, op[_i_]); }
  for(; op != out+(outlen&(~(4-1))); op+=4) { bitdnormr(bw,br,bp); VD(0); VD(1); bitdnormr(bw,br,bp); VD(2); VD(3); }
  for(; op != out+outlen; op++) { bitdnormr(bw,br,bp); VD(0); }
  return inlen+4;
}

size_t vlczenc16(unsigned char *_in, size_t _inlen, unsigned char *out) {
  unsigned char *op = out+4, *out_ = out+_inlen, *bp = out_;
  uint16_t      *in = (uint16_t *)_in, *ip, start = 0;
  INDEC;
  bitedef(bw,br); biteinir(bw,br,bp);

  for(ip = in; ip < in+inlen; ip++) {
	uint16_t x = ip[0];
	x = zigzagenc16(x - start);
	start = ip[0];
	bitvput(bw,br, VLC_VN6,VLC_VB6, x); bitenormr(bw,br,bp);                   OVERFLOWR(_in, _inlen, out,op,bp, goto e);
  }

  bitflushr(bw,br,bp);
  unsigned l = out_-bp;
  memmove(op, bp, l); op += l;
  ctou32(out) = op-out;
  OVERFLOW(_in,_inlen,out, op, goto e);
  e:return op - out;
}

size_t vlczdec16(unsigned char *in, size_t _outlen, unsigned char *_out) {
  unsigned      inlen = ctou32(in);
  unsigned char *ip = in+4,*bp = in+inlen;
  uint16_t      *out = (uint16_t *)_out, *op, x, start = 0;
  OUTDEC;
  bitddef(bw, br); bitdinir(bw,br,bp);

  for(op = out; op < out+outlen; op++) {
	bitdnormr(bw,br,bp);
	bitvget(bw,br,VLC_VN6,VLC_VB6, x);
	*op = (start+=zigzagdec16(x));
  }
  return inlen + 4;
}

size_t vlczenc32(unsigned char *_in, size_t _inlen, unsigned char *out) {
  unsigned char *op = out+4, *out_ = out+_inlen, *bp = out_;
  uint32_t      *in = (uint32_t *)_in, *ip, start = 0;
  INDEC;
  bitedef(bw,br); biteinir(bw,br,bp);

  for(ip = in; ip < in+inlen; ip++) {
	uint32_t x = ip[0];
	x = zigzagenc32(x - start);
	start = ip[0];
	bitvput(bw,br, VLC_VN8,0/*VLC_VB8*/, x); bitenormr(bw,br,bp);                   OVERFLOWR(_in, _inlen, out,op,bp, goto e);
  }

  bitflushr(bw,br,bp);
  unsigned l = out_-bp;
  memmove(op, bp, l); op += l;
  ctou32(out) = op-out;
  OVERFLOW(_in,_inlen,out, op, goto e);
  e:return op - out;
}

size_t vlczdec32(unsigned char *in, size_t _outlen, unsigned char *_out) {
  unsigned      inlen = ctou32(in);
  unsigned char *ip = in+4,*bp = in+inlen;
  uint32_t      *out = (uint32_t *)_out, *op, x, start = 0;
  OUTDEC;
  bitddef(bw, br); bitdinir(bw,br,bp);

  for(op = out; op < out+outlen; op++) {
	bitdnormr(bw,br,bp);
	bitvget(bw,br,VLC_VN8,0/*VLC_VB8*/, x);
	*op = (start+=zigzagdec32(x));
  }
  return inlen + 4;
}
