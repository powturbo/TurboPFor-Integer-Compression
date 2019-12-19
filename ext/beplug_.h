  #ifndef __SSSE3__
#define C_VINTG8IU 		0
#define C_MASKEDVBYTE 	0			
  #endif

  #if C_FASTPFOR128
#include "FastPFor/headers/variablebyte.h"
#include "FastPFor/headers/simple16.h"
#include "FastPFor/headers/simple8b_rle.h"

#include "FastPFor/headers/fastpfor.h"
#include "FastPFor/headers/optpfor.h"

#include "FastPFor/headers/simdfastpfor.h"
#include "FastPFor/headers/simdoptpfor.h"
#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#endif
#define _mm_store_si128 _mm_storeu_si128
#define _mm_load_si128 _mm_loadu_si128
#include "FastPFor/headers/simdgroupsimple.h"
#undef min
  #endif
  
  #if C_SIMPLE8B
#include "simple8b.h"       		// optimized simple-8b by powturbo
  #endif  

  #if C_C_BLOSC
#include "c-blosc/blosc/shuffle.h"
#include "c-blosc/blosc/blosc.h"
  #endif

  #if C_LIBFOR
#include "libfor/for.h"
unsigned char *for_selectx( unsigned char *__restrict in, unsigned n, unsigned *__restrict out) { unsigned b = in[4], i; for(i = 0; i < n; i++) out[i] = for_select(in, i); return in + 5 + for_compressed_size_bits(n, b); }
  #endif
  
  #if C_LZ4
#include "lz4/lib/lz4.h"
#include "lz4/lib/lz4hc.h"
  #endif

//  #if C_SIMDCOMPLIB
//#include "ext/SIMDCompressionAndIntersection/include/bitpacking.h"
//  #endif

  #if  C_POLYCOM  
#include "vas16c.h"										// Simple 16	
#include "vas16d.h"   
#include "rc.h"
#include "polycom/optpfd.h" 							
#include "polycom/polyvbyte.h" 							
  #endif

  #if C_QMX
#include "JASSv2/source/compress_integer_qmx_improved.h" 
  #endif

  #if  C_VTENC
#include "VTEnc/vtenc.h"
  #endif
  
  #if defined(ZLIB)
#include "zlib/zlib.h" //#include <zlib.h>

#define USE_LZ
  #endif

#ifdef __cplusplus
extern "C" {
#endif

  #if C_STREAMVBYTE
#include "streamvbyte/include/streamvbyte.h"
#include "streamvbyte/include/streamvbytedelta.h"
#undef VARINTDECODE_H_

  #endif

  #if C_MASKEDVBYTE									
#include "MaskedVByte/include/varintencode.h"
    #undef VARINTDECODE_H_
#include "MaskedVByte/include/varintdecode.h"
  #endif 

  #if C_VARINTG8IU
#include "varintg8iu.h"							        // SIMD Varint G8IU
  #endif
  #if C_LITTLEPACK
#include "LittleIntPacker/include/bitpacking.h"
#include "LittleIntPacker/include/util.h"
  #endif

#if C_SIMDCOMP128
#include "simdcomp/include/simdbitpacking.h"  
#endif

#ifdef __cplusplus
}
#endif
  
  #if C_SIMDCOMP128
#undef SIMDBITPACKING_H_
#include "vabyte.h"                                     // Standard Variable Byte
#include "simdcomp/include/simdcomp.h"  
unsigned char *simdpackwithoutmaskd1n(uint32_t *in, uint32_t n, uint32_t *out, uint32_t start, uint32_t b) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *ip;
  for(ip = in; ip != in+(n&~(128-1)); ip += 128,out += 4 * b) 
	simdpackwithoutmaskd1(start, ip, (__m128i *)out, b);
  return (unsigned char *)out;
}
   
unsigned char *simdunpackd1n(uint32_t *in, uint32_t n, uint32_t *out, uint32_t start, uint32_t b) {
  uint32_t k, *op; 
  for(op = out; op != out+(n&~(128-1)); op += 128,in += 4 * b)
	simdunpackd1(start, (__m128i *)in, out, b); 
  return (unsigned char *)in;
}  

unsigned char *simdfor_selectx( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
    unsigned i; 
    for(i=0; i < n;i++) out[i] = simdselectFOR(start, (const __m128i *)in, b, i); return in + simdpackFOR_compressedbytes(n, b); 
}
  #endif
  #if C_SIMDCOMP256
#undef SIMDBITPACKING_H_
unsigned char *avxpackwithoutmaskn(uint32_t *in, uint32_t n, uint32_t *out, uint32_t b) {
  uint32_t *ip;
  for(ip = in; ip != in+(n&~(256-1)); ip += 256,out += 8 * b)
	avxpackwithoutmask(ip, (__m256i *)out, b);
  return (unsigned char *)simdpack_shortlength(ip, n & (256-1), (__m128i *)out, b);
}
unsigned char *avxunpackn(uint32_t *in, uint32_t n, uint32_t *out, uint32_t b) {
  uint32_t k, *op; 
  for(op = out; op != out+(n&~(256-1)); op += 256,in += 8 * b)
	avxunpack((__m256i *)in, op, b); 
  return (unsigned char *)simdunpack_shortlength((__m128i *)in,  n & (256-1), op, b);
}  
  #endif

  #if C_BITSHUFFLE
#define __STDC_VERSION__ 199901L
#include "bitshuffle/src/bitshuffle.h"
  #endif

  #if C_C_BLOSC  
#define   BITSHUFFLE(in,n,out,esize) bitshuffle(   esize, (n)/esize, (unsigned char *)in, out, NULL)                    //crash
#define BITUNSHUFFLE(in,n,out,esize) bitunshuffle( esize, (n)/esize, (unsigned char *)in, (unsigned char *)out, NULL)
  #else
#define   BITSHUFFLE(in,n,out,esize) bshuf_bitshuffle(  in, out, (n)/esize, esize, 0); memcpy((char *)out+((n)&(~(8*esize-1))),(char *)in+((n)&(~(8*esize-1))),(n)&(8*esize-1))
#define BITUNSHUFFLE(in,n,out,esize) bshuf_bitunshuffle(in, out, (n)/esize, esize, 0); memcpy((char *)out+((n)&(~(8*esize-1))),(char *)in+((n)&(~(8*esize-1))),(n)&(8*esize-1))
  #endif 

