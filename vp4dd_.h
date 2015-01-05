/**
    Copyright (C) powturbo 2013-2014
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

    vp4dd_.h - "Integer Compression" Turbo PforDelta 
**/

  #ifdef USE__AVX2__  // disabled per default.
#include <immintrin.h>

static ALIGNED(unsigned char, shuffles[256][8], 32) = {
  { 0,0,0,0,0,0,0,0 },
  { 0,1,1,1,1,1,1,1 },
  { 1,0,1,1,1,1,1,1 },
  { 0,1,2,2,2,2,2,2 },
  { 1,1,0,1,1,1,1,1 },
  { 0,2,1,2,2,2,2,2 },
  { 2,0,1,2,2,2,2,2 },
  { 0,1,2,3,3,3,3,3 },
  { 1,1,1,0,1,1,1,1 },
  { 0,2,2,1,2,2,2,2 },
  { 2,0,2,1,2,2,2,2 },
  { 0,1,3,2,3,3,3,3 },
  { 2,2,0,1,2,2,2,2 },
  { 0,3,1,2,3,3,3,3 },
  { 3,0,1,2,3,3,3,3 },
  { 0,1,2,3,4,4,4,4 },
  { 1,1,1,1,0,1,1,1 },
  { 0,2,2,2,1,2,2,2 },
  { 2,0,2,2,1,2,2,2 },
  { 0,1,3,3,2,3,3,3 },
  { 2,2,0,2,1,2,2,2 },
  { 0,3,1,3,2,3,3,3 },
  { 3,0,1,3,2,3,3,3 },
  { 0,1,2,4,3,4,4,4 },
  { 2,2,2,0,1,2,2,2 },
  { 0,3,3,1,2,3,3,3 },
  { 3,0,3,1,2,3,3,3 },
  { 0,1,4,2,3,4,4,4 },
  { 3,3,0,1,2,3,3,3 },
  { 0,4,1,2,3,4,4,4 },
  { 4,0,1,2,3,4,4,4 },
  { 0,1,2,3,4,5,5,5 },
  { 1,1,1,1,1,0,1,1 },
  { 0,2,2,2,2,1,2,2 },
  { 2,0,2,2,2,1,2,2 },
  { 0,1,3,3,3,2,3,3 },
  { 2,2,0,2,2,1,2,2 },
  { 0,3,1,3,3,2,3,3 },
  { 3,0,1,3,3,2,3,3 },
  { 0,1,2,4,4,3,4,4 },
  { 2,2,2,0,2,1,2,2 },
  { 0,3,3,1,3,2,3,3 },
  { 3,0,3,1,3,2,3,3 },
  { 0,1,4,2,4,3,4,4 },
  { 3,3,0,1,3,2,3,3 },
  { 0,4,1,2,4,3,4,4 },
  { 4,0,1,2,4,3,4,4 },
  { 0,1,2,3,5,4,5,5 },
  { 2,2,2,2,0,1,2,2 },
  { 0,3,3,3,1,2,3,3 },
  { 3,0,3,3,1,2,3,3 },
  { 0,1,4,4,2,3,4,4 },
  { 3,3,0,3,1,2,3,3 },
  { 0,4,1,4,2,3,4,4 },
  { 4,0,1,4,2,3,4,4 },
  { 0,1,2,5,3,4,5,5 },
  { 3,3,3,0,1,2,3,3 },
  { 0,4,4,1,2,3,4,4 },
  { 4,0,4,1,2,3,4,4 },
  { 0,1,5,2,3,4,5,5 },
  { 4,4,0,1,2,3,4,4 },
  { 0,5,1,2,3,4,5,5 },
  { 5,0,1,2,3,4,5,5 },
  { 0,1,2,3,4,5,6,6 },
  { 1,1,1,1,1,1,0,1 },
  { 0,2,2,2,2,2,1,2 },
  { 2,0,2,2,2,2,1,2 },
  { 0,1,3,3,3,3,2,3 },
  { 2,2,0,2,2,2,1,2 },
  { 0,3,1,3,3,3,2,3 },
  { 3,0,1,3,3,3,2,3 },
  { 0,1,2,4,4,4,3,4 },
  { 2,2,2,0,2,2,1,2 },
  { 0,3,3,1,3,3,2,3 },
  { 3,0,3,1,3,3,2,3 },
  { 0,1,4,2,4,4,3,4 },
  { 3,3,0,1,3,3,2,3 },
  { 0,4,1,2,4,4,3,4 },
  { 4,0,1,2,4,4,3,4 },
  { 0,1,2,3,5,5,4,5 },
  { 2,2,2,2,0,2,1,2 },
  { 0,3,3,3,1,3,2,3 },
  { 3,0,3,3,1,3,2,3 },
  { 0,1,4,4,2,4,3,4 },
  { 3,3,0,3,1,3,2,3 },
  { 0,4,1,4,2,4,3,4 },
  { 4,0,1,4,2,4,3,4 },
  { 0,1,2,5,3,5,4,5 },
  { 3,3,3,0,1,3,2,3 },
  { 0,4,4,1,2,4,3,4 },
  { 4,0,4,1,2,4,3,4 },
  { 0,1,5,2,3,5,4,5 },
  { 4,4,0,1,2,4,3,4 },
  { 0,5,1,2,3,5,4,5 },
  { 5,0,1,2,3,5,4,5 },
  { 0,1,2,3,4,6,5,6 },
  { 2,2,2,2,2,0,1,2 },
  { 0,3,3,3,3,1,2,3 },
  { 3,0,3,3,3,1,2,3 },
  { 0,1,4,4,4,2,3,4 },
  { 3,3,0,3,3,1,2,3 },
  { 0,4,1,4,4,2,3,4 },
  { 4,0,1,4,4,2,3,4 },
  { 0,1,2,5,5,3,4,5 },
  { 3,3,3,0,3,1,2,3 },
  { 0,4,4,1,4,2,3,4 },
  { 4,0,4,1,4,2,3,4 },
  { 0,1,5,2,5,3,4,5 },
  { 4,4,0,1,4,2,3,4 },
  { 0,5,1,2,5,3,4,5 },
  { 5,0,1,2,5,3,4,5 },
  { 0,1,2,3,6,4,5,6 },
  { 3,3,3,3,0,1,2,3 },
  { 0,4,4,4,1,2,3,4 },
  { 4,0,4,4,1,2,3,4 },
  { 0,1,5,5,2,3,4,5 },
  { 4,4,0,4,1,2,3,4 },
  { 0,5,1,5,2,3,4,5 },
  { 5,0,1,5,2,3,4,5 },
  { 0,1,2,6,3,4,5,6 },
  { 4,4,4,0,1,2,3,4 },
  { 0,5,5,1,2,3,4,5 },
  { 5,0,5,1,2,3,4,5 },
  { 0,1,6,2,3,4,5,6 },
  { 5,5,0,1,2,3,4,5 },
  { 0,6,1,2,3,4,5,6 },
  { 6,0,1,2,3,4,5,6 },
  { 0,1,2,3,4,5,6,7 },
  { 1,1,1,1,1,1,1,0 },
  { 0,2,2,2,2,2,2,1 },
  { 2,0,2,2,2,2,2,1 },
  { 0,1,3,3,3,3,3,2 },
  { 2,2,0,2,2,2,2,1 },
  { 0,3,1,3,3,3,3,2 },
  { 3,0,1,3,3,3,3,2 },
  { 0,1,2,4,4,4,4,3 },
  { 2,2,2,0,2,2,2,1 },
  { 0,3,3,1,3,3,3,2 },
  { 3,0,3,1,3,3,3,2 },
  { 0,1,4,2,4,4,4,3 },
  { 3,3,0,1,3,3,3,2 },
  { 0,4,1,2,4,4,4,3 },
  { 4,0,1,2,4,4,4,3 },
  { 0,1,2,3,5,5,5,4 },
  { 2,2,2,2,0,2,2,1 },
  { 0,3,3,3,1,3,3,2 },
  { 3,0,3,3,1,3,3,2 },
  { 0,1,4,4,2,4,4,3 },
  { 3,3,0,3,1,3,3,2 },
  { 0,4,1,4,2,4,4,3 },
  { 4,0,1,4,2,4,4,3 },
  { 0,1,2,5,3,5,5,4 },
  { 3,3,3,0,1,3,3,2 },
  { 0,4,4,1,2,4,4,3 },
  { 4,0,4,1,2,4,4,3 },
  { 0,1,5,2,3,5,5,4 },
  { 4,4,0,1,2,4,4,3 },
  { 0,5,1,2,3,5,5,4 },
  { 5,0,1,2,3,5,5,4 },
  { 0,1,2,3,4,6,6,5 },
  { 2,2,2,2,2,0,2,1 },
  { 0,3,3,3,3,1,3,2 },
  { 3,0,3,3,3,1,3,2 },
  { 0,1,4,4,4,2,4,3 },
  { 3,3,0,3,3,1,3,2 },
  { 0,4,1,4,4,2,4,3 },
  { 4,0,1,4,4,2,4,3 },
  { 0,1,2,5,5,3,5,4 },
  { 3,3,3,0,3,1,3,2 },
  { 0,4,4,1,4,2,4,3 },
  { 4,0,4,1,4,2,4,3 },
  { 0,1,5,2,5,3,5,4 },
  { 4,4,0,1,4,2,4,3 },
  { 0,5,1,2,5,3,5,4 },
  { 5,0,1,2,5,3,5,4 },
  { 0,1,2,3,6,4,6,5 },
  { 3,3,3,3,0,1,3,2 },
  { 0,4,4,4,1,2,4,3 },
  { 4,0,4,4,1,2,4,3 },
  { 0,1,5,5,2,3,5,4 },
  { 4,4,0,4,1,2,4,3 },
  { 0,5,1,5,2,3,5,4 },
  { 5,0,1,5,2,3,5,4 },
  { 0,1,2,6,3,4,6,5 },
  { 4,4,4,0,1,2,4,3 },
  { 0,5,5,1,2,3,5,4 },
  { 5,0,5,1,2,3,5,4 },
  { 0,1,6,2,3,4,6,5 },
  { 5,5,0,1,2,3,5,4 },
  { 0,6,1,2,3,4,6,5 },
  { 6,0,1,2,3,4,6,5 },
  { 0,1,2,3,4,5,7,6 },
  { 2,2,2,2,2,2,0,1 },
  { 0,3,3,3,3,3,1,2 },
  { 3,0,3,3,3,3,1,2 },
  { 0,1,4,4,4,4,2,3 },
  { 3,3,0,3,3,3,1,2 },
  { 0,4,1,4,4,4,2,3 },
  { 4,0,1,4,4,4,2,3 },
  { 0,1,2,5,5,5,3,4 },
  { 3,3,3,0,3,3,1,2 },
  { 0,4,4,1,4,4,2,3 },
  { 4,0,4,1,4,4,2,3 },
  { 0,1,5,2,5,5,3,4 },
  { 4,4,0,1,4,4,2,3 },
  { 0,5,1,2,5,5,3,4 },
  { 5,0,1,2,5,5,3,4 },
  { 0,1,2,3,6,6,4,5 },
  { 3,3,3,3,0,3,1,2 },
  { 0,4,4,4,1,4,2,3 },
  { 4,0,4,4,1,4,2,3 },
  { 0,1,5,5,2,5,3,4 },
  { 4,4,0,4,1,4,2,3 },
  { 0,5,1,5,2,5,3,4 },
  { 5,0,1,5,2,5,3,4 },
  { 0,1,2,6,3,6,4,5 },
  { 4,4,4,0,1,4,2,3 },
  { 0,5,5,1,2,5,3,4 },
  { 5,0,5,1,2,5,3,4 },
  { 0,1,6,2,3,6,4,5 },
  { 5,5,0,1,2,5,3,4 },
  { 0,6,1,2,3,6,4,5 },
  { 6,0,1,2,3,6,4,5 },
  { 0,1,2,3,4,7,5,6 },
  { 3,3,3,3,3,0,1,2 },
  { 0,4,4,4,4,1,2,3 },
  { 4,0,4,4,4,1,2,3 },
  { 0,1,5,5,5,2,3,4 },
  { 4,4,0,4,4,1,2,3 },
  { 0,5,1,5,5,2,3,4 },
  { 5,0,1,5,5,2,3,4 },
  { 0,1,2,6,6,3,4,5 },
  { 4,4,4,0,4,1,2,3 },
  { 0,5,5,1,5,2,3,4 },
  { 5,0,5,1,5,2,3,4 },
  { 0,1,6,2,6,3,4,5 },
  { 5,5,0,1,5,2,3,4 },
  { 0,6,1,2,6,3,4,5 },
  { 6,0,1,2,6,3,4,5 },
  { 0,1,2,3,7,4,5,6 },
  { 4,4,4,4,0,1,2,3 },
  { 0,5,5,5,1,2,3,4 },
  { 5,0,5,5,1,2,3,4 },
  { 0,1,6,6,2,3,4,5 },
  { 5,5,0,5,1,2,3,4 },
  { 0,6,1,6,2,3,4,5 },
  { 6,0,1,6,2,3,4,5 },
  { 0,1,2,7,3,4,5,6 },
  { 5,5,5,0,1,2,3,4 },
  { 0,6,6,1,2,3,4,5 },
  { 6,0,6,1,2,3,4,5 },
  { 0,1,7,2,3,4,5,6 },
  { 6,6,0,1,2,3,4,5 },
  { 0,7,1,2,3,4,5,6 },
  { 7,0,1,2,3,4,5,6 },
  { 0,1,2,3,4,5,6,7, }
  };

  #elif defined(__SSE4_1__)
#include <tmmintrin.h>
  #endif

#define uint_t TEMPLATE3(uint, USIZE, _t)

unsigned char *TEMPLATE2(p4ddec, USIZE)(unsigned char *__restrict in, int n, uint_t *__restrict out) { 
  uint_t ex[0x100+8]; 
  unsigned i = *(unsigned short *)in; 
  uint_t b = P4D_B(i); 
  unsigned xb = P4D_XB(i); 
  P4D_ININC(in,i); 
  in = TEMPLATE2(bitunpack, USIZE)(in, n, b, out);

  if(i & 1) { 
      #if P4DN == 2
	unsigned long long bb[P4DN]; unsigned num=0; 
    bb[0] = *(unsigned long long *)in; in += 8; 
    bb[1] = *(unsigned long long *)in; in += 8;  
    in = TEMPLATE2(bitunpack, USIZE)(in, popcnt64(bb[0]) + popcnt64(bb[1]), xb, ex); 								
	  #else
	unsigned long long bb[P4DN]; unsigned num=0; 
	for(i = 0; i < P4DN; i++) { bb[i] = *(unsigned long long *)in; in += 8; num += popcnt64(bb[i]); }  
    in = TEMPLATE2(bitunpack, USIZE)(in, num, xb, ex); 		
	  #endif
	  
      #if 0 //def __AVX2__
    uint_t *op,*pex = ex;
      #if 0 //P4DN == 2
    for(op = out;    b0; b0 >>= 8,op += 8) { unsigned m = (unsigned char)b0, mc=popcnt32(m), s = pex[mc]; pex[mc]=0;		
      _mm256_storeu_si256((__m256i *)op, _mm256_add_epi32(_mm256_loadu_si256((const __m256i*)op), _mm256_permutevar8x32_epi32(_mm256_slli_epi32(_mm256_load_si256((const __m256i*)pex), b), _mm256_cvtepu8_epi32(_mm_loadl_epi64((const __m128i *)shuffles[m])) )) ); pex += mc; *pex=s;
    } 
    for(op = out+64; b1; b1 >>= 8,op += 8) { unsigned m = (unsigned char)b1, mc=popcnt32(m), s = pex[mc]; pex[mc]=0;		
      _mm256_storeu_si256((__m256i *)op, _mm256_add_epi32(_mm256_loadu_si256((const __m256i*)op), _mm256_permutevar8x32_epi32(_mm256_slli_epi32(_mm256_load_si256((const __m256i*)pex), b), _mm256_cvtepu8_epi32(_mm_loadl_epi64((const __m128i *)shuffles[m])) )) ); pex += mc; *pex=s;
    } 
	  #else
	for(i = 0; i < P4DN; i++) {
    for(op = out;    bb[i]; bb[i] >>= 8,op += 8) { unsigned m = (unsigned char)bb[i], mc=popcnt32(m), s = pex[mc]; pex[mc]=0;		
      _mm256_storeu_si256((__m256i *)op, _mm256_add_epi32(_mm256_loadu_si256((const __m256i*)op), _mm256_permutevar8x32_epi32(_mm256_slli_epi32(_mm256_load_si256((const __m256i*)pex), b), _mm256_cvtepu8_epi32(_mm_loadl_epi64((const __m128i *)shuffles[m])) )) ); pex += mc; *pex=s;
    } out += 64; 
	}
	  #endif
      #elif defined(__SSE4_1__)
  static ALIGNED(char, shuffles[16][16], 16) = {
  #define _ 0x80
        { _,_,_,_, _,_,_,_, _,_, _, _,  _, _, _,_  },
        { 0,1,2,3, _,_,_,_, _,_, _, _,  _, _, _,_  },
        { _,_,_,_, 0,1,2,3, _,_, _, _,  _, _, _,_  },
        { 0,1,2,3, 4,5,6,7, _,_, _, _,  _, _, _,_  },
        { _,_,_,_, _,_,_,_, 0,1, 2, 3,  _, _, _,_  },
        { 0,1,2,3, _,_,_,_, 4,5, 6, 7,  _, _, _,_  },
        { _,_,_,_, 0,1,2,3, 4,5, 6, 7,  _, _, _,_  },
        { 0,1,2,3, 4,5,6,7, 8,9,10,11,  _, _, _,_  },
        { _,_,_,_, _,_,_,_, _,_,_,_,    0, 1, 2, 3 },
        { 0,1,2,3, _,_,_,_, _,_,_,  _,  4, 5, 6, 7 },
        { _,_,_,_, 0,1,2,3, _,_,_,  _,  4, 5, 6, 7 },
        { 0,1,2,3, 4,5,6,7, _,_, _, _,  8, 9,10,11 },
        { _,_,_,_, _,_,_,_, 0,1, 2, 3,  4, 5, 6, 7 },
        { 0,1,2,3, _,_,_,_, 4,5, 6, 7,  8, 9,10,11 },
        { _,_,_,_, 0,1,2,3, 4,5, 6, 7,  8, 9,10,11 },
        { 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15 }, 
  #undef _
    };                                        
    uint_t *op,*pex = ex; 
	
        #if P4DN == 2
    for(op = out;  bb[0]; bb[0] >>= 4,op+=4) { const unsigned m = bb[0]&0xf; 
      _mm_storeu_si128((__m128i *)op, _mm_add_epi32(_mm_loadu_si128((__m128i*)op), _mm_shuffle_epi8(_mm_slli_epi32(_mm_load_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) )); pex += popcnt32(m);
    } 
    for(op=out+64; bb[1]; bb[1] >>= 4,op+=4) { const unsigned m = bb[1]&0xf; 
      _mm_storeu_si128((__m128i *)op, _mm_add_epi32(_mm_loadu_si128((__m128i*)op), _mm_shuffle_epi8(_mm_slli_epi32(_mm_load_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) )); pex += popcnt32(m);
    }
	    #else
    for(i = 0; i < P4DN; i++) { // Loop unrolling
      for(op = out;  bb[i]; bb[i] >>= 4,op+=4) { const unsigned m = bb[i]&0xf; 
        _mm_storeu_si128((__m128i *)op, _mm_add_epi32(_mm_loadu_si128((__m128i*)op), _mm_shuffle_epi8(_mm_slli_epi32(_mm_load_si128((__m128i*)pex), b), _mm_load_si128((__m128i*)shuffles[m]) ) )); pex += popcnt32(m);
      } out+=64; 
	}
	    #endif
      #else
    unsigned k = 0;
        #if P4DN == 2
    while(bb[0]) { unsigned x = ctzll(bb[0]); out[x] += ex[k++]<<b; bb[0] ^= (1ull<<x); } out += 64;
    while(bb[1]) { unsigned x = ctzll(bb[1]); out[x] += ex[k++]<<b; bb[1] ^= (1ull<<x); }    
        #else
    for(i = 0; i < P4DN; i++) {
	  while(bb[i]) { unsigned x = ctzll(b0); out[x] += ex[k++]<<b; bb[i] ^= (1ull<<x); } out += 64;
	}
        #endif	  
      #endif
  }
  return in;
}

unsigned char *TEMPLATE2(p4ddecx, USIZE)(unsigned char *__restrict in, int n, uint_t *__restrict out) { 
  unsigned b,i; 
  struct p4d p4d; 
  p4dini(&p4d, &in, n, &b);

  if(unlikely(p4d.i & 1)) {
    for(i = 0; i < n; i++) 
      out[i] = TEMPLATE2(vp4dget, USIZE)(p4d, in, b, i);  
    return p4d.ex + PAD8((p4d.cum[P4DN-1] + popcnt64(p4d.xmap[P4DN-1]))*p4d.xb);
  } else {
    for(i = 0; i < n; i++) out[i] = TEMPLATE2(_bitgetx, USIZE)(in, b, i*b); 
    return p4d.ex; 
  }
}

unsigned char *TEMPLATE2(p4dfdecx, USIZE)(unsigned char *__restrict in, int n, unsigned start, uint_t *__restrict out) {
  unsigned b,i; 
  struct p4d p4d; 
  p4dini(&p4d, &in, n, &b);

  if(unlikely(p4d.i & 1)) {
    for(i = 0; i < n; i++) 
      out[i] = TEMPLATE2(vp4dget, USIZE)(p4d, in, b, i)+start+i+1;  
    return p4d.ex + PAD8((p4d.cum[P4DN-1] + popcnt64(p4d.xmap[P4DN-1]))*p4d.xb);
  } else {
    for(i = 0; i < n; i++) out[i] = TEMPLATE2(_bitgetx, USIZE)(in, b, i*b)+start+i+1;
    return p4d.ex; 
  }
}

unsigned char *TEMPLATE2(p4df0decx, USIZE)(unsigned char *__restrict in, int n, unsigned start, uint_t *__restrict out) {
  unsigned b,i; 
  struct p4d p4d; 
  p4dini(&p4d, &in, n, &b);

  if(unlikely(p4d.i & 1)) {
    for(i = 0; i < n; i++) 
      out[i] = TEMPLATE2(vp4dget, USIZE)(p4d, in, b, i)+start;  
    return p4d.ex + PAD8((p4d.cum[P4DN-1] + popcnt64(p4d.xmap[P4DN-1]))*p4d.xb);
  } else {
    for(i = 0; i < n; i++) out[i] = TEMPLATE2(_bitgetx, USIZE)(in, b, i*b)+start;
    return p4d.ex; 
  }
}


