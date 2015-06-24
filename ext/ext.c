// simple-8b simple16 and optpfd don't work with all interger lists. Enable if you to want to test
//#define _SIMPLE_8B  // crashs on some lists 
//#define _SIMPLE16   // limited to 28 bits
//#define _OPTPFD     // compression very slow and limited to 28 bits. crashs on some lists
//#define _VBYTEPOLY  // limited to 28 bits. 
  #ifdef __SSSE3__
#define _VARINTG8IU
#define _MASKEDVBYTE
  #endif

// Optional external librrary. Activate also in makefile
//#define _FORLIB // libfor 
//#define _LZT    // LzTurbo not inluded
//#define _BLOSC  // 
//#define _LZ4
//#define _ZLIB

//-------------------------------------- External functions for comparison ------------------------------------------------------------------------
  #ifdef _ZLIB
#include <zlib.h>
  #endif

#include "vabyte.h"                                     // Standard Variable Byte

#include "simdcomp/include/simdbitpacking.h"   			// SIMD FastPFor
#include "simdcomp/include/simdcomputil.h"
#include "simdcomp/include/simdintegratedbitpacking.h"

  #ifdef _VARINTG8IU
#include "varintg8iu.h"							        // SIMD Varint G8IU
#include "varintg8iu.h"
  #endif

#include "vas16c.h"										// Simple 16	
#include "vas16d.h"   
  
#include "OPT_PFD/opt_p4.h" 							// OptPFD
#include "simple8b.h"       							// optimized simple-8b
    
  #ifdef _MASKEDVBYTE										// http://maskedvbyte.org
#include "MaskedVByte/include/varintencode.h"
#include "MaskedVByte/include/varintdecode.h"
  #endif

  #ifdef _FORLIB
#include "for/for.h"
  #endif

//---------------- FastPFor functions ---------------------  
unsigned char *simdpackwn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpackwithoutmask(in, (__m128i *)out, b);
  return (unsigned char *)out;
}

unsigned char *simdpackn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpack(in, (__m128i *)out, b);
  return (unsigned char *)out;
} 

unsigned char *simdunpackn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {  
  uint32_t k, *out_; 
  for(out_ = out + n; out + 128 <= out_; out += 128, in += 4 * b) simdunpack((const __m128i *)in, out, b); 
  return (unsigned char *)in;
}

unsigned char *simdpackwn1(uint32_t *in, uint32_t n, uint32_t b, uint32_t start, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpackwithoutmaskd1(start, in, (__m128i *)out, b); //simdpackwithoutmaskd1(x, ip+1, (__m128i *)out, b); 
  return (unsigned char *)out;
}
     
unsigned char *simdunpackn1(uint32_t *in, uint32_t n, uint32_t b, uint32_t start, uint32_t *out) {
  uint32_t k, *out_; 
  for(out_ = out + n; out + 128 <= out_; out += 128, in += 4 * b) simdunpackd1(start, (__m128i *)in, out, b); 
  return (unsigned char *)in;
}  
//--------------- Polytec variable byte ----------- 
#include "vbyte_poly.h"
unsigned char *vbpolyenc(unsigned *in, unsigned n, unsigned char *out) {
  unsigned i; for(i = 0; i < n; i++) { unsigned x = in[i]; VBYTE_ENC(out, x); } return out;
}
unsigned char *vbpolydec(unsigned char *in, unsigned n, unsigned *out) {
  unsigned i; for(i = 0; i < n; i++) { unsigned x; VBYTE_DEC(in, x); out[i] = x; } return in;
}


  #ifdef _LZT  
#include "../../lz/lz8.h"
int lz8c0( struct lzobj *lz);
int lz8c01(struct lzobj *lz);
int lz8d(  struct lzobj *lz);

int lzbc0( struct lzobj *lz);
int lzbc01(struct lzobj *lz);
int lzbd(  struct lzobj *lz);
  #endif

  #ifdef _LZ4
#include "lz4.h"
  #endif

  #ifdef _BLOSC
#include "c-blosc/blosc/shuffle.h"
#include "c-blosc/blosc/blosc.h"
  #endif
