/**
    Copyright (C) powturbo 2013-2015
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
//   transpose.c - byte transpose

  #ifndef ESIZE
#include "transpose.h"
  
#define ESIZE 2
#include __FILE__
#undef ESIZE

#define ESIZE 3
#include __FILE__
#undef ESIZE

#define ESIZE 4
#include __FILE__
#undef ESIZE

#define ESIZE 8
#include __FILE__
#undef ESIZE

#define ESIZE 16
#include __FILE__
#undef ESIZE

#ifdef __SSSE3__
#include <tmmintrin.h>

#include <stdio.h>
void transposev4(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned bsize = n/4,i;
  static unsigned char s[16] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 };
  unsigned char *op,*ip,*e;
  __m128i sv = _mm_loadu_si128((__m128i *)s);
  for(ip = in,op = out; ip != in+(n&~(64-1)); op += 16) {
	__m128i iv[4],ov[4];
	iv[0] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)ip), sv); ip += 16;
	iv[1] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)ip), sv); ip += 16;
	iv[2] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)ip), sv); ip += 16;
	iv[3] = _mm_shuffle_epi8(_mm_loadu_si128((__m128i *)ip), sv); ip += 16;

	ov[0] = _mm_unpacklo_epi32(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi32(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi32(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi32(iv[2], iv[3]);

    iv[0] = _mm_unpacklo_epi64(ov[0], ov[2]);
    iv[1] = _mm_unpackhi_epi64(ov[0], ov[2]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[3]);
    iv[3] = _mm_unpackhi_epi64(ov[1], ov[3]);
	
    _mm_storeu_si128((__m128i *)op,              iv[0]);
    _mm_storeu_si128((__m128i *)(op+(i =bsize)), iv[1]);
    _mm_storeu_si128((__m128i *)(op+(i+=bsize)), iv[2]);
    _mm_storeu_si128((__m128i *)(op+(i+=bsize)), iv[3]);
  }
  transpose4(ip, (in+n)-ip, op);
}

void untransposev4(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned bsize = n/4,i;
  unsigned char *op,*ip; 
  for(op = out,ip = in; op != out+(n&~(64-1)); ip += 16) {
	__m128i iv[4], ov[4];
	iv[0] = _mm_loadu_si128((__m128i *) ip)            ;   
	iv[1] = _mm_loadu_si128((__m128i *)(ip+(i =bsize)));   
	iv[2] = _mm_loadu_si128((__m128i *)(ip+(i+=bsize)));   
	iv[3] = _mm_loadu_si128((__m128i *)(ip+(i+=bsize)));	
	
	ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);
		  
    _mm_storeu_si128((__m128i *)op, _mm_unpacklo_epi16(ov[0], ov[2])); op += 16;
    _mm_storeu_si128((__m128i *)op, _mm_unpackhi_epi16(ov[0], ov[2])); op += 16;
    _mm_storeu_si128((__m128i *)op, _mm_unpacklo_epi16(ov[1], ov[3])); op += 16;
    _mm_storeu_si128((__m128i *)op, _mm_unpackhi_epi16(ov[1], ov[3])); op += 16;
  }
  untranspose4(ip, (out+n)-op, op);
}
#endif

void transpose(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case  2: transpose2 (in,n,out); break;
    case  3: transpose3 (in,n,out); break;
    case  4: transpose4 (in,n,out); break;
    case  8: transpose8 (in,n,out); break;
    case 16: transpose16(in,n,out); break;
	default: {
      unsigned bsize = n/esize,i;
      unsigned char *op,*ip;
      for(ip = in,op = out; ip < in+bsize*esize; op++)
        for(i = 0; i < esize; i++)
	      op[i*bsize] = *ip++; 
      for(op += esize*bsize; ip < in+n;) 
		*op++ = *ip++;
    }
  }	
}  

void untranspose(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case  2: untranspose2 (in,n,out); break;
    case  3: untranspose3 (in,n,out); break;
    case  4: untranspose4 (in,n,out); break;
    case  8: untranspose8 (in,n,out); break;
    case 16: untranspose16(in,n,out); break;
	default: {
      unsigned bsize = n/esize,i;
      unsigned char *op,*ip; 
      for(op = out,ip = in; op < out+bsize*esize; ip++)
	    for(i = 0; i < esize; i++) 
	      *op++ = ip[i*bsize];
      for(ip += esize*bsize; op < out+n;) 
		*op++ = *ip++;
    }
  }
}  

  #else

#include "conf.h"
#define powof2(n) !((n)&((n)-1))

void TEMPLATE2(transpose, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned bsize = n/ESIZE,i;
  unsigned char *op,*ip,*e;
    #if powof2(ESIZE)
  e = in+(n&~(ESIZE-1));
	#else      
  e = in+bsize*ESIZE;		
    #endif
	
  for(ip = in,op = out; ip < e; op++) {
	op[0]        = *ip++; 
	op[i =bsize] = *ip++;
	  #if ESIZE > 2
	op[i+=bsize] = *ip++; 
	    #if ESIZE > 3
	op[i+=bsize] = *ip++; 
	      #if ESIZE > 4
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	        #if ESIZE > 8
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	op[i+=bsize] = *ip++;
	        #endif	
	      #endif
	    #endif
	  #endif
  }	
  op = out+bsize*ESIZE; while(ip < in+n) *op++ = *ip++;
}  

void TEMPLATE2(untranspose, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
  unsigned bsize = n/ESIZE,i;
  unsigned char *op,*ip,*e; 
  
    #if powof2(ESIZE)
  e = out+(n&~(ESIZE-1));
	#else      
  e = out+bsize*ESIZE;		
    #endif
  for(op = out,ip = in; op < e; ip++) {
	*op++ = ip[0];
	*op++ = ip[i =bsize];
	  #if ESIZE > 2
	*op++ = ip[i+=bsize];
	    #if ESIZE > 3	
	*op++ = ip[i+=bsize];
	      #if ESIZE > 4
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	        #if ESIZE > 8
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	*op++ = ip[i+=bsize];
	        #endif
	      #endif
	    #endif
	  #endif
  } 	
  ip = in+bsize*ESIZE; 
  while(op < out+n) 
	*op++ = *ip++;
}  
  #endif
