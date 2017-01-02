/**
    Copyright (C) powturbo 2013-2017
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
//  Nibble/Byte transpose

#if !defined(TRANSPOSE) && !defined(TRANSPOSEV)
#include <string.h>
  #ifdef __SSSE3__
#include <tmmintrin.h>
  #elif defined(__SSE2__)
#include <pmmintrin.h>
  #endif

#include "conf.h"
#include "transpose.h"

#define TRANSPOSE     transpose
#define UNTRANSPOSE untranspose

#define ESIZE 2
#include "transpose.c"
#undef ESIZE

#define ESIZE 3
#include "transpose.c"
#undef ESIZE

#define ESIZE 8
#include "transpose.c"
#undef ESIZE

#define ESIZE 16
#include "transpose.c"
#undef ESIZE

#undef TRANSPOSE   
#undef UNTRANSPOSE 

#define ESIZE 4
#define TRANSPOSE   _transpose
#define UNTRANSPOSE _untranspose

#define STRIDE 4
#define TRANSPOSEV   transpose
#define UNTRANSPOSEV untranspose
#include "transpose.c"
#undef ESIZE
#undef TRANSPOSE   
#undef UNTRANSPOSE 

#undef STRIDE 
#undef TRANSPOSEV   
#undef UNTRANSPOSEV 

#define STRIDE 8
#define TRANSPOSEV     transposen
#define UNTRANSPOSEV untransposen
#include "transpose.c"

void transpose(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case  2:  transpose2 (in,n,out); break;
    case  3:  transpose3 (in,n,out); break;
    case  4:  transpose4( in,n,out); break;
    case  8:  transpose8 (in,n,out); break;
    case 16:  transpose16(in,n,out); break;
	default: { 
      unsigned bsize = n/esize,i;
      unsigned char *op,*ip;
      for(ip = in,op = out; ip < in+bsize*esize; op++)
        for(i = 0; i < esize; i++)
	      op[i*bsize] = *ip++; 
      for(op = out + esize*bsize; ip < in+n;) 
		*op++ = *ip++;
    }
  }	
}  

void untranspose(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case  2: untranspose2 (in,n,out); break;
    case  3: untranspose3 (in,n,out); break;
    case  4: untranspose4( in,n,out); break;
    case  8: untranspose8 (in,n,out); break;
    case 16: untranspose16(in,n,out); break;
	default: {
      unsigned bsize = n/esize,i;
      unsigned char *op,*ip; 
      for(op = out,ip = in; op < out+bsize*esize; ip++)
	    for(i = 0; i < esize; i++) 
	      *op++ = ip[i*bsize];
      for(ip = in+esize*bsize; op < out+n;) 
		*op++ = *ip++;
    }
  }
}  
#else

    #ifdef TRANSPOSE
#define powof2(n) !((n)&((n)-1))

void TEMPLATE2(TRANSPOSE, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
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

void TEMPLATE2(UNTRANSPOSE, ESIZE)(unsigned char *in, unsigned n, unsigned char *out) {
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

  #ifdef TRANSPOSEV
void TEMPLATE2(TRANSPOSEV, 4)(unsigned char *in, unsigned n, unsigned char *out) {
    #ifdef __SSE2__
  unsigned v = n&~(64-1), bsize = v/STRIDE,i;
  unsigned char *op,*ip;
      #ifdef __SSE3__
  static unsigned char s[16] = { 0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15 };
  __m128i sv = _mm_loadu_si128((__m128i *)s);
      #endif
      #if STRIDE == 8
  __m128i cl = _mm_set1_epi8(0x0f),ch=_mm_set1_epi8(0xf0), cb = _mm_set1_epi16(0xff);
      #endif
  for(ip = in,op = out; ip != in+v; op += 64/STRIDE) {
    __m128i iv[4],ov[4];
      
      #ifdef __SSSE3__
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
      #elif defined(__SSE2__)
    iv[0] = _mm_loadu_si128((__m128i *)ip); ip += 16;
    iv[1] = _mm_loadu_si128((__m128i *)ip); ip += 16;
    iv[2] = _mm_loadu_si128((__m128i *)ip); ip += 16;
    iv[3] = _mm_loadu_si128((__m128i *)ip); ip += 16;

    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);

    iv[0] = _mm_unpacklo_epi8(ov[0], ov[1]);
    iv[1] = _mm_unpackhi_epi8(ov[0], ov[1]);
    iv[2] = _mm_unpacklo_epi8(ov[2], ov[3]);
    iv[3] = _mm_unpackhi_epi8(ov[2], ov[3]);

    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);

    iv[0] = _mm_unpacklo_epi64(ov[0], ov[2]);
    iv[1] = _mm_unpackhi_epi64(ov[0], ov[2]);
    iv[2] = _mm_unpacklo_epi64(ov[1], ov[3]);
    iv[3] = _mm_unpackhi_epi64(ov[1], ov[3]);
      #endif

      #if STRIDE == 8
    ov[0] = _mm_and_si128(iv[0], cl);                             					
    ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(  ov[0],2));    				      					

    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[0], ch),4);            					
    ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128(  ov[1],2));    				      					

    ov[2] = _mm_and_si128(iv[1], cl);                             					
    ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(  ov[2],2));    				      					

    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[1], ch),4);            					
    ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(  ov[3],2));    				      					

    _mm_storel_epi64((__m128i *)op,              ov[0]);
    _mm_storel_epi64((__m128i *)(op+(i =bsize)), ov[1]);
    _mm_storel_epi64((__m128i *)(op+(i+=bsize)), ov[2]);
    _mm_storel_epi64((__m128i *)(op+(i+=bsize)), ov[3]); 

    ov[0] = _mm_and_si128(iv[2], cl);                             					
    ov[0] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[0],4), ov[0]),cb);         
    ov[0] = _mm_packus_epi16(ov[0], _mm_srli_si128(  ov[0],2));    				      					

    ov[1] = _mm_srli_epi16(_mm_and_si128(iv[2], ch),4);            					
    ov[1] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[1],4), ov[1]),cb);         
    ov[1] = _mm_packus_epi16(ov[1], _mm_srli_si128(  ov[1],2));    				      					

    ov[2] = _mm_and_si128(iv[3], cl);                             					
    ov[2] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[2],4), ov[2]),cb);         
    ov[2] = _mm_packus_epi16(ov[2], _mm_srli_si128(  ov[2],2));    				      					

    ov[3] = _mm_srli_epi16(_mm_and_si128(iv[3], ch),4);            					
    ov[3] = _mm_and_si128(_mm_or_si128(_mm_srli_epi16(ov[3],4), ov[3]),cb);         
    ov[3] = _mm_packus_epi16(ov[3], _mm_srli_si128(  ov[3],2));    				      					

    _mm_storel_epi64((__m128i *)(op+(i+=bsize)), ov[0]);
    _mm_storel_epi64((__m128i *)(op+(i+=bsize)), ov[1]);
    _mm_storel_epi64((__m128i *)(op+(i+=bsize)), ov[2]);
    _mm_storel_epi64((__m128i *)(op+(i+=bsize)), ov[3]); 

      #else 
    _mm_storeu_si128((__m128i *)op,              iv[0]);
    _mm_storeu_si128((__m128i *)(op+(i =bsize)), iv[1]);
    _mm_storeu_si128((__m128i *)(op+(i+=bsize)), iv[2]);
    _mm_storeu_si128((__m128i *)(op+(i+=bsize)), iv[3]); 
      #endif
  } 
  _transpose4(in+v, n-v, out+v);
    #else
  _transpose4(in, n, out);
    #endif
}
  #endif

  #ifdef UNTRANSPOSEV 
void TEMPLATE2(UNTRANSPOSEV, 4)(unsigned char *in, unsigned n, unsigned char *out) {
    #ifdef __SSE2__
      #if STRIDE == 8
  __m128i cl = _mm_set1_epi8(0x0f),ch=_mm_set1_epi8(0xf0), cb = _mm_set1_epi16(0xff);
      #endif
  unsigned v = n&~(64-1), bsize = v/STRIDE,i;
  unsigned char *op,*ip; 
  for(op = out,ip = in; op != out+v; ip += 64/STRIDE) {
    __m128i iv[4], ov[4];

      #if STRIDE == 8
    ov[0] = _mm_loadl_epi64((__m128i *) ip)            ;   
    ov[1] = _mm_loadl_epi64((__m128i *)(ip+(i =bsize)));   

    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); 						
    ov[0] = _mm_and_si128(ov[0], cl);												
    
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); 						
    ov[1] = _mm_and_si128(ov[1], cl);												
    iv[0] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);					


    ov[2] = _mm_loadl_epi64((__m128i *)(ip+(i+=bsize)));   
    ov[3] = _mm_loadl_epi64((__m128i *)(ip+(i+=bsize)));  
 
    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); 						
    ov[2] = _mm_and_si128(ov[2], cl);												
    
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); 						
    ov[3] = _mm_and_si128(ov[3], cl);												
    iv[1] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);	


    ov[0] = _mm_loadl_epi64((__m128i *)(ip+(i+=bsize)));   
    ov[1] = _mm_loadl_epi64((__m128i *)(ip+(i+=bsize)));  
 
    ov[0] = _mm_unpacklo_epi8(ov[0], _mm_srli_epi16(ov[0],4)); 						
    ov[0] = _mm_and_si128(ov[0], cl);												
    
    ov[1] = _mm_unpacklo_epi8(ov[1], _mm_srli_epi16(ov[1],4)); 						
    ov[1] = _mm_and_si128(ov[1], cl);												
    iv[2] = _mm_or_si128(_mm_slli_epi16(ov[1],4), ov[0]);					


    ov[2] = _mm_loadl_epi64((__m128i *)(ip+(i+=bsize)));   
    ov[3] = _mm_loadl_epi64((__m128i *)(ip+(i+=bsize)));  
 
    ov[2] = _mm_unpacklo_epi8(ov[2], _mm_srli_epi16(ov[2],4)); 						
    ov[2] = _mm_and_si128(ov[2], cl);												
    
    ov[3] = _mm_unpacklo_epi8(ov[3], _mm_srli_epi16(ov[3],4)); 						
    ov[3] = _mm_and_si128(ov[3], cl);												
    iv[3] = _mm_or_si128(_mm_slli_epi16(ov[3],4), ov[2]);	
      #else
    iv[0] = _mm_loadu_si128((__m128i *) ip)            ;   
    iv[1] = _mm_loadu_si128((__m128i *)(ip+(i =bsize)));   
    iv[2] = _mm_loadu_si128((__m128i *)(ip+(i+=bsize)));   
    iv[3] = _mm_loadu_si128((__m128i *)(ip+(i+=bsize)));	
      #endif

    ov[0] = _mm_unpacklo_epi8(iv[0], iv[1]);
    ov[1] = _mm_unpackhi_epi8(iv[0], iv[1]);
    ov[2] = _mm_unpacklo_epi8(iv[2], iv[3]);
    ov[3] = _mm_unpackhi_epi8(iv[2], iv[3]);
		  
    _mm_storeu_si128((__m128i *)op, _mm_unpacklo_epi16(ov[0], ov[2])); op += 16;
    _mm_storeu_si128((__m128i *)op, _mm_unpackhi_epi16(ov[0], ov[2])); op += 16;
    _mm_storeu_si128((__m128i *)op, _mm_unpacklo_epi16(ov[1], ov[3])); op += 16;
    _mm_storeu_si128((__m128i *)op, _mm_unpackhi_epi16(ov[1], ov[3])); op += 16;
  } 
  _untranspose4(in+v, n-v, out+v);
    #else
  _untranspose4(in, n, out);
    #endif
}
  #endif

#endif
