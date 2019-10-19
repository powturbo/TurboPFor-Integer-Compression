/**
    Copyright (C) powturbo 2015-2019
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
  #ifndef USIZE
#include <string.h> 
#include "conf.h"
#include "trle.h"
#include "trle_.h"

  #ifdef __ARM_NEON
#define PREFETCH(_ip_,_rw_)
  #else
#define PREFETCH(_ip_,_rw_) __builtin_prefetch(_ip_,_rw_)
  #endif

//------------------------------------- Fastet Histogram : https://github.com/powturbo/TurboHist -------------------------------------------
#define cnt_t unsigned
#define CSIZE (256 + 8)

#define CU32(u,_i_) {\
  c[_i_+0][(unsigned char) u     ]++;\
  c[_i_+1][(unsigned char)(u>> 8)]++;\
  c[_i_+2][(unsigned char)(u>>16)]++;\
  c[_i_+3][ 	           u>>24 ]++;\
}

#define OV 8
#define INC4_32(_i_) { { unsigned u = ux, v = vx; ux = ctou32(ip+_i_+OV+0); vx = ctou32(ip+_i_+OV+ 4); CU32(u,0); CU32(v,0); }\
                       { unsigned u = ux, v = vx; ux = ctou32(ip+_i_+OV+8); vx = ctou32(ip+_i_+OV+12); CU32(u,0); CU32(v,0); }\
}

static unsigned cntcalc32(const unsigned char *__restrict in, unsigned inlen, cnt_t *__restrict cnt) {
  cnt_t c[4][CSIZE] = {0},i; 

  unsigned char *ip = in; 
  if(inlen >= 64) {
    unsigned ux = ctou32(ip), vx = ctou32(ip+4);
    for(; ip != in+(inlen&~(64-1))-64; ip += 64) { INC4_32(0); INC4_32(16); INC4_32(32); INC4_32(48); __builtin_prefetch(ip+512, 0); }
  }
  while(ip != in+inlen) 
    c[0][*ip++]++; 

  for(i = 0; i < 256; i++)
    cnt[i] = c[0][i]+c[1][i]+c[2][i]+c[3][i];
  unsigned a = 256; while(a > 1 && !cnt[a-1]) a--; 
  return a;
}

//------------------------------ speed optimized RLE 8 with escape char. SSE/AVX2 slower than scalar in encoding ----------------------------------------
#define SRLE8   (__WORDSIZE/2)

#define USIZE 8
#include "trlec.c"

  #if SRLE8
#define PUTC(_op_, _x_) *_op_++ = _x_
#define PUTE(_op_, _e_) do { PUTC(_op_, _e_); vlput32(_op_, 0); } while(0)

#define SZ64 if((z = (ctou64(ip) ^ ctou64(ip+1)))) goto a; ip += 8;		
#define SZ32 if((z = (ctou32(ip) ^ ctou32(ip+1)))) break; ip += 4;

#define SRLEPUT8(_pp_, _ip_, _e_, _op_) do {\
  unsigned _r = (_ip_ - _pp_)+1;\
  if(_r >= 4)  { PUTC(_op_, _e_); _r = (_r-4)+3; vlput32(_op_, _r); PUTC(_op_, pp[0]); }\
  else if(pp[0] == _e_) {\
	             PUTC(_op_, _e_); _r -= 1;       vlput32(_op_, _r);  /*1-3:Escape char -> 2-6 bytes */\
  } else while(_r--) PUTC(_op_, pp[0]);\
} while(0) 

unsigned _srlec8(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint8_t e) {
  uint8_t *ip = in, *pp = in, *ie = in+inlen, *op = out; 
    
  if(inlen > SRLE8+1)
    while(ip < ie-1-SRLE8) {		
        #if __WORDSIZE == 64
      uint64_t z; SZ64; SZ64; SZ64; SZ64; 						__builtin_prefetch(ip +256, 0);					
      continue;                                                                                               
      a: ip += ctz64(z)>>3;									    																						  
        #else
      uint32_t z; SZ32; SZ32; SZ32; SZ32; 						__builtin_prefetch(ip +256, 0);      
      continue;                                                                                             
      a: ip += ctz32(z)>>3;									    
        #endif
      SRLEPUT8(pp, ip, e, op);
	  pp = ++ip;												
    }
	   
  while(ip < ie-1) {                              		 
    while(ip < ie-1 && ip[1] == *pp) ip++;
	SRLEPUT8(pp, ip, e, op);
	pp = ++ip; 
  }
  if(ip < ie) { 
    unsigned c = *ip++;
    if(c == e) PUTE(op,e);
	else PUTC(op, c);
  }							  									//AS(ip == ie,"FatalI ip!=ie=%d ", ip-ie)  
  return op - out;
}

#define SRLEPUT8X(_pp_, _ip_, _e_, _op_) do {\
  unsigned _r = (_ip_ - _pp_)+1, _cr = pp[0];\
  if(_r >= 4 /*|| _r == 3 && _cr == ix*/) { PUTC(_op_, _e_); _r = ((_r-4)+3)<<1; if(_cr == ix) { vlput32(_op_, _r); } else { vlput32(_op_, _r|1); PUTC(_op_, pp[0]); } }\
  else if(_cr == _e_) { PUTC(_op_, _e_); _r = (_r-1)<<1|1; vlput32(_op_, _r);  /*1-3:Escape char -> 2-6 bytes */ } \
  else while(_r--) PUTC(_op_, _cr);\
} while(0) 

static inline unsigned _srlec8x(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint8_t e, uint8_t ix) {
  uint8_t *ip = in, *pp = in, *ie = in+inlen, *op = out; 
    
  if(inlen > SRLE8+1)
    while(ip < ie-1-SRLE8) {		
        #if __WORDSIZE == 64
      uint64_t z; SZ64; SZ64; SZ64; SZ64; 						__builtin_prefetch(ip +256, 0);					
      continue;                                                                                               
      a: ip += ctz64(z)>>3;									    																						  
        #else
      uint32_t z; SZ32; SZ32; SZ32; SZ32; 						__builtin_prefetch(ip +256, 0);      
      continue;                                                                                             
      a: ip += ctz32(z)>>3;									    
        #endif
      SRLEPUT8X(pp, ip, e, op);
	  pp = ++ip;												
    }
	   
  while(ip < ie-1) {                              		 
    while(ip < ie-1 && ip[1] == *pp) ip++;
	SRLEPUT8X(pp, ip, e, op);
	pp = ++ip; 
  }
  if(ip < ie) { 
    unsigned c = *ip++;
    if(c == e) PUTE(op,e);
	else PUTC(op, c);
  }							  									//AS(ip == ie,"FatalI ip!=ie=%d ", ip-ie)  
  return op - out;
}
  #endif

unsigned srlec(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out) { // Automatic escape char determination
  unsigned cnt[256] = {0}, a, m = -1, x = 0, im = 0, i, ix, l; 
  if(!inlen) return 0; 

  a = cntcalc32(in, inlen, cnt);  		
  if(cnt[a-1] == inlen) {            
    *out = *in;
    return 1;                        							// RETURN 1 = memset 
  }

  if(a != 256) {                    						    // determine escape char
    for(im = a, i = m = 0; i < a; i++) 
      if(cnt[i] > x) x = cnt[i],ix = i;
  } else for(i = 0; i < a; i++) {
    if(cnt[i] < m) m = cnt[i],im = i;  						    // minimum for ESC char
    if(cnt[i] > x) x = cnt[i],ix = i;                           // maximum for embeding in the run length
  } 
  out[0] = im; 
  out[1] = ix; 
  if((l = _srlec8x(in, inlen, out+2, im, ix)+2) < inlen) 
    return l;
  memcpy(out, in, inlen);
  return inlen;
}

//------------------------------------------------- TurboRLE ------------------------------------------
#define TRLEPUT(pp, ip, m, rmap, op) do {\
  int _r_ = (ip - pp)+1;\
  if(_r_ >= TMIN) { \
    unsigned char *q = op; /*checkpoint*/\
    if(pp[0] == ix) { unsigned _r = (_r_ - TMIN)<<1|1; vlzput(op, _r, m, rmap); } else { unsigned _r = (_r_ - TMIN)<<1; vlzput(op, _r, m, rmap); *op++ = pp[0]; }\
    if(op-q >= _r_) { op = q; while(_r_--) *op++ = pp[0]; }  /*rollback rle*/\
  } else while(_r_--) *op++ = pp[0];\
} while(0)

unsigned trlec(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out) {
  unsigned      cnt[256] = {0}, m=-1, x=0, im, i, a, c; 
  unsigned char rmap[256], *op=out, *ie = in+inlen, *ip = in,*pp = in, ix;
  if(!inlen) return 0; 											// RETURN 0 = zero length

  a = cntcalc32(in, inlen, cnt);  		
  if(cnt[a-1] == inlen) {
    *out = *in;
    return 1;          											// RETURN 1 = memset
  }															
  
  if(a != 256) {                    						    // determine escape char
    for(im = a, i = m = 0; i < a; i++) 
      if(cnt[i] > x) x = cnt[i],ix = i;
  } else for(i = 0; i < a; i++) {
    if(cnt[i] < m) m = cnt[i],im = i;  						    // minimum for ESC char
    if(cnt[i] > x) x = cnt[i],ix = i;                           // maximum for embeding in the run length
  } 
  if(m) {  														// no unused bytes found 
    PUTC(op, 0);      						    				// 0: srle mode
	PUTC(op, im);					    						// _srlec8 escape char
    op += _srlec8(in, inlen, op, im);
    if(op - out < inlen) return op - out;       				// RETURN rle/escape
    memcpy(out, in, inlen);  				    				// no compression, use memcpy 
    return inlen; 						    					// RETURN outlen = inlen (memcpy)
  }																	
 
  c = (a+7)/8; 								    				 
  PUTC(op, c);  								    			// c = bitmap length in bytes 
  memset(op, 0, 32);
  for(m = i = 0; i != c*8; i++) 								// set bitmap for unused chars
    if(!cnt[i]) op[i>>3] |= 1<<(i&7), rmap[m++] = i;
  op += c; 
  for(; i != 256; i++) rmap[m++] = i;

  m--;                                 							 
  PUTC(op, ix);   

  if(inlen > SRLE8+1)                                           // encode    
    while(ip < ie-1-SRLE8) {	
        #if __WORDSIZE == 64	
      uint64_t z; SZ64; SZ64; SZ64; SZ64; 						__builtin_prefetch(ip +256, 0);					
      continue;                                                                                             
      a: ip += ctz64(z)>>3;																																								  
        #else
      uint32_t z;
      uint32_t z; SZ32; SZ32; SZ32; SZ32; 						__builtin_prefetch(ip +256, 0);      
      continue;                                                                                             
      a: ip += ctz32(z)>>3;										
        #endif
      TRLEPUT(pp, ip, m, rmap, op);
	  pp = ++ip;												
    }
	   
  while(ip < ie-1) {                              		 
    while(ip < ie-1 && ip[1] == *pp) ip++;
	TRLEPUT(pp, ip, m, rmap, op);
	pp = ++ip; 
  }
  if(ip < ie) PUTC(op, *ip++);              						AS(ip == ie, "Fatal ip>ie=%d ", ip-ie);    

  if(op - out < inlen) 
    return op - out;       										// RETURN length = rle
  memcpy(out, in, inlen);  				    					// no compression, use memcpy 
  return inlen; 						    					// RETURN outlen = inlen (memcpy)
}

#undef USIZE
#undef SRLE8
//------------------------------------- RLE 16, 32, 64 -------------------------------------------------- 
#define USIZE 16
#include "trlec.c"
#undef USIZE

#define USIZE 32
#include "trlec.c"
#undef USIZE

#define USIZE 64
#include "trlec.c"
#undef USIZE

#else // ------------------- include RLE 16, 32, 64
#define uint_t TEMPLATE3(uint, USIZE, _t)
#define ctout(_x_) *(uint_t *)(_x_)

#define PUTC(_op_, _x_) ctout(_op_) = _x_, _op_ += sizeof(uint_t)
#define PUTE(_op_, _e_) do { PUTC(_op_, _e_); vlput32(_op_, 0); } while(0)

#define SRLEPUT(_pp_, _ip_, _e_, _op_) do {\
  unsigned _r = (_ip_ - _pp_)+1;\
  if(_r >= 4)  { PUTC(_op_, _e_); _r = (_r-4)+3; vlput32(_op_, _r); PUTC(_op_, pp[0]); }\
  else if(pp[0] == _e_) {\
	             PUTC(_op_, _e_); _r -= 1;       vlput32(_op_, _r);\
  } else while(_r--) PUTC(_op_, pp[0]);\
} while(0) 

  #if !SRLE8
unsigned TEMPLATE2(_srlec, USIZE)(const unsigned char *__restrict cin, unsigned inlen, unsigned char *__restrict out, uint_t e) { 
  unsigned  char *op = out;
  unsigned  n = inlen/sizeof(uint_t);
  uint_t   *in = (uint_t *)cin, *pp = in, *ip = in, *ie = in+n;

  if(!inlen) return 0;
  #define SZ1 if(ip[0] != ip[1]) goto a; ++ip;
  if(n > 6+1)                                                    
    while(ip < ie-1-6) {		                                // fast encode
      SZ1; SZ1; SZ1; SZ1; SZ1; SZ1; 			                __builtin_prefetch(ip +128*USIZE/8, 0);					
      continue;                                                 																					  
      a: 													    		
      SRLEPUT(pp, ip, e, op);
	  pp = ++ip; 																								
    }

  while(ip < ie - 1) {                              		    // encode rest
    while(ip < ie-1 && ip[1] == *pp) ip++;
	SRLEPUT(pp, ip, e, op);
	pp = ++ip;
  }
  if(ip < ie) {                                                 // last item
    uint_t c = *ip++;
    if(c == e) PUTE(op, e);
	else PUTC(op, c);
  }							  									//AS(ip == ie,"FatalI ip!=ie=%d ", ip-ie)  	  											
    #if USIZE > 8
  { unsigned char *p = (unsigned char *)ip;                     // remaining bytes inlen % USIZE/8
    while(p < cin+inlen) 
	  *op++ = *p++;
  }  
    #endif
																//AS(ip == ie,"FatalI ip!=ie=%d ", ip-ie)  
  return op - out;
}
 #endif
#undef SRLEPUT
#undef PUTC
#undef PUTE

unsigned TEMPLATE2(srlec, USIZE)(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint_t e) {
  unsigned l = TEMPLATE2(_srlec, USIZE)(in, inlen, out, e);

  if(l < inlen) 
    return l;
  memcpy(out, in, inlen);
  return inlen;
}
#endif

