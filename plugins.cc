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
#define _LARGEFILE64_SOURCE 1 
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <string.h>
#include <malloc.h> 
#include <stdio.h>
#include <math.h> 
#include <time.h>
#include <getopt.h>
#include <sys/stat.h>
#include <stdint.h>

#include "plugins.h"

  #ifdef NCODEC0
#define CODEC0  0
#define CODEC0S 0
  #else
#define CODEC0  1
    #ifdef NSIMD
#define CODEC0V 0
    #else
#define CODEC0V 1
    #endif
  #endif

  #ifdef N
#define CODEC0 0
  #else
#define CODEC0 1
  #endif


  #ifdef NCODEC1
#define CODEC1 0
  #else
#define CODEC1 1
  #endif

  #ifdef NCODEC2
#define CODEC2 0
  #else
#define CODEC2 0
  #endif

  #ifdef NTRANSFORM
#define TRANSFORM 0
  #else
#define TRANSFORM 1
  #endif

enum { 
#define C_MEMCPY		1
  P_LMCPY,   	// must be 0
  P_MCPY,		// must be 1
  P_COPY,  
  
#define C_TURBOPFOR		CODEC0
  TB_PFORDA,   						// actually not working w. mingw 
  TB_FOR,    	
  TB_FORDA,  	
  TB_PACK,		
  TB_VBYTE,    
  TB_VSIMPLE,
  TB_ELIASFANO, 	
  TB_VS_S1,
  TB_VS_S4,   
  TB_VS_S8,

#define C_TURBOPFORV	CODEC0V
  TB_PFOR128,    	
  TB_PFOR256,
  TB_PACK128H,		
  TB_PACK128V,		
  TB_PACK256H,		
  TB_PACK256V,		
  TB_ELIASFANOV, 	

#define C_BITSHUFFLE	CODEC1
  P_BITSHUFFLE,
#define C_C_BLOSC		CODEC2
  BS_LZ,
  BS_LZ4, 
  BS_ZLIB,
  BS_LZ_1,
  BS_LZ4_S1, 
  BS_LZ4_1, 
  BS_ZLIB_1,
  BS_SHUFFLE, 	
#define C_FASTPFOR		CODEC1
  FP_VBYTE,
  FP_FASTPFOR,
  FP_SIMDFASTPFOR,
  FP_SIMDOPTPFOR,
  FP_OPTPFOR,
  FP_SIMPLE8BRLE,
  FP_SIMDPACK,
#define C_LIBFOR		CODEC1
  LF_FOR,
  LF_FORX,
#define C_LITTLEPACK	CODEC1
  LI_PACK,
  LI_BMIPACK,
  LI_TURBOPACK,
  LI_HORPACK,
  LI_SCPACK,
#define C_LZ4			CODEC1
  LZ4_,
  LZ4_S1,
  LZ4_S4,   
  LZ4_S8,
#define C_MASKEDVBYTE   CODEC1		
  P_MASKEDVBYTE,
#define C_POLYCOM  		CODEC1		
  PC_OPTPFD,             			// compression too slow and limited to 28 bits. crashs on some lists
  PC_VBYTE,
  PC_RICE,
  PC_SIMPLE16,						// limited to 28 bits.
#define C_QMX			CODEC1
  P_QMX,P_QMX2,P_QMX3,P_QMX4,
#define C_SIMDCOMP		CODEC1
  SC_PACK,
  SC_SIMDPACK128,
  SC_SIMDPACK256,
  SC_FOR,    	
  SC_FORDA,   	
#define C_SIMPLE8B 		CODEC1		// //crash on integers w. size 32 bits !	
  AM_SIMPLE8B,
#define C_STREAMVBYTE   CODEC1
  P_STREAMVBYTE,
#define C_VARINTG8IU    CODEC1
  P_VARINTG8IU,
  #ifdef ZLIB
#define C_ZLIB			CODEC2
  #else
#define C_ZLIB			0
  #endif
  P_ZLIB,
#define C_TRANSFORM     CODEC0
  TB_TP8_32, 	
  TB_TP8V_32, 	
  TB_TP4V_32, 	
  TB_ZIGZAG_32, 	
  TB_DELTA_32, 	
#define C_LZTURBO       CODEC2
  P_LZT,
  P_VSHUF,
  
  P_MAX
};

  #ifndef __SSSE3__
#define C_VINTG8IU 0
#define C_MASKEDVBYTE 0			
  #endif
  #if C_SIMPLE8B
#include "ext/simple8b.h"       							// optimized simple-8b by powturbo
  #endif  


//-------------------------------------------------------------------------------
  #if C_C_BLOSC
#include "ext/c-blosc2/blosc/shuffle.h"
#include "ext/c-blosc2/blosc/blosc.h"
  #endif

  #if C_FASTPFOR
#include "ext/FastPFor/headers/variablebyte.h"
#include "ext/FastPFor/headers/simple16.h"
#include "ext/FastPFor/headers/simple8b_rle.h"

#include "ext/FastPFor/headers/fastpfor.h"
#include "ext/FastPFor/headers/simdfastpfor.h"
#include "ext/FastPFor/headers/optpfor.h"
#include "ext/FastPFor/headers/simdoptpfor.h"
//#include "ext/FastPFor/headers/compositecodec.h"
  #endif
  
  #if C_LIBFOR
#include "ext/libfor/for.h"
unsigned char *for_selectx( unsigned char *__restrict in, unsigned n, unsigned *__restrict out) { unsigned b = in[4], i; for(i = 0; i < n; i++) out[i] = for_select(in, i); return in + 5 + for_compressed_size_bits(n, b); }
  #endif
  
  #if C_LZ4
#include "ext/lz4/lib/lz4.h"
#include "ext/lz4/lib/lz4hc.h"
  #endif

//  #if C_SIMDCOMPLIB
//#include "ext/SIMDCompressionAndIntersection/include/bitpacking.h"
//  #endif

  #if  C_POLYCOM  
#include "ext/vas16c.h"										// Simple 16	
#include "ext/vas16d.h"   
#include "ext/rc.h"
#include "ext/polycom/optpfd.h" 							
#include "ext/polycom/polyvbyte.h" 							
  #endif

  #if C_QMX
//#include "ext/qmx/compress_qmx.h"
//#include "ext/qmx.h"
#include "ext/bench_/bench/compress_qmx.h" 
#include "ext/bench_/bench/compress_qmx_v2.h"
#include "ext/bench_/bench/compress_qmx_v3.h"
#include "ext/bench_/bench/compress_qmx_v4.h"
  #endif
  
  #if C_TURBOPFOR  
#include "vint.h"       
#include "vsimple.h"
#include "bitpack.h"
#include "bitunpack.h"
#include "eliasfano.h"
#include "bitutil.h"
#include "transpose.h"
#include "vp4c.h"
#include "vp4d.h"

  #endif
  
  #if C_ZLIB
#include <zlib.h>
  #endif

#ifdef __cplusplus
extern "C" {
#endif

  #if C_STREAMVBYTE
#include "ext/streamvbyte/include/streamvbyte.h"
#include "ext/streamvbyte/include/streamvbytedelta.h"
#undef VARINTDECODE_H_

  #endif

  #if C_MASKEDVBYTE									
#include "ext/MaskedVByte/include/varintencode.h"
    #undef VARINTDECODE_H_
#include "ext/MaskedVByte/include/varintdecode.h"
  #endif 

  #if C_VARINTG8IU
#include "ext/varintg8iu.h"							        // SIMD Varint G8IU
  #endif
  #if C_LITTLEPACK
#include "ext/LittleIntPacker/include/bitpacking.h"
#include "ext/LittleIntPacker/include/util.h"
  #endif

#include "ext/simdcomp/include/simdbitpacking.h"  
  
#ifdef __cplusplus
}
#endif
  
  #if C_SIMDCOMP
#undef SIMDBITPACKING_H_
#include "ext/vabyte.h"                                     // Standard Variable Byte
#include "ext/simdcomp/include/simdcomp.h"  
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

/*unsigned char *simdfor_selectx( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
    unsigned i; 
    for(i=0; i < n;i++) out[i] = simdselectFOR(start, (const __m128i *)in, b, i); return in + simdpackFOR_compressedbytes(n, b); 
  }*/

    #ifdef __AVX2__
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
  #endif

  #if C_LZTURBO 
#include "../../lz/lz8.h"
int lz8c0( struct lzobj *lz);
int lz8c01(struct lzobj *lz);
int lz8d(  struct lzobj *lz);

#include "../../lz/lzb.h"
int lzbc0( struct lzobj *lz);
int lzbc01(struct lzobj *lz);
int lzbc2( struct lzobj *lz);
int lzbd(  struct lzobj *lz);  

//#include "../../lz/lzh.h"
int lzhc2( struct lzobj *lz);
int lzhd(  struct lzobj *lz);
int prunelen = 0x200, ans_seg = 12*1024;
#include "../../lz/anst.h"
#include "../../lz/ans.h"
#include "../../lz/mh.h"
#define MH_BLK (16*1024)
  #endif

  #if C_BITSHUFFLE
#define __STDC_VERSION__ 199901L
#include "ext/bitshuffle/src/bitshuffle.h"
  #endif

#define BLK_SIZE  (64*1024*4)
#define BLK_V128 (128*4)
#define BLK_V256 (256*4)

unsigned char sbuf[BLK_SIZE*2+64];

//------------------------------------------------- registry -------------------------------------------------------------------------------------------------
struct plugs plugs[] = {
  { TB_PFOR128, 	"TurboPFor", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V128 }, 
  { TB_PFOR256, 	"TurboPFor256", 	C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V256 }, 
  { TB_PFORDA,		"TurboPForDA", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V128 }, 
  { TB_FOR,			"TurboFor", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V128 }, 
  { TB_FORDA, 		"TurboForDA", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V128 }, 
  { TB_PACK128V, 	"TurboPackV", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V128 }, 
//  { TB_PACK128H, 	"TurboPackH", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V128 }, 
  { TB_PACK256V, 	"TurboPack256V",	C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V256 }, 
  { TB_PACK256H, 	"TurboPack256H",	C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V256 }, 
  { TB_PACK,		"TurboPack", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_V128 }, 
  { TB_VBYTE, 		"TurboVByte", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"" }, 
  { TB_VSIMPLE, 	"VSimple", 			C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"" }, 
  { TB_ELIASFANO, 	"EliasFano",		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"" }, 
  { TB_ELIASFANOV, 	"EliasFanoV",		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"" }, 

  { TB_VS_S1,  		"vs_bitshuffle",	C_LZ4, 			"",	"TurboPFor",			"",		"https://github.com/Cyan4973/lz4", 						    "", 0,BLK_SIZE }, 
  { TB_VS_S4,  		"vs_s4",			C_LZ4, 			"",	"TurboPFor",			"",		"https://github.com/Cyan4973/lz4", 							"", 0,BLK_SIZE }, 
  { TB_VS_S8,  		"vs_s8",			C_LZ4, 			"",	"TurboPFor",			"",		"https://github.com/Cyan4973/lz4", 							"", 0,BLK_SIZE }, 
  
  { TB_TP8_32, 		"TP8s_32", 			C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_SIZE }, 
  { TB_TP8V_32, 	"TP8_32",			C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_SIZE }, 
  { TB_TP4V_32, 	"TP4_32",			C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_SIZE }, 
  { TB_ZIGZAG_32, 	"ZigZag_32", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_SIZE }, 
  { TB_DELTA_32, 	"Delta_32", 		C_TURBOPFOR,	"",	"TurboPFor",			"",	"https://github.com/powturbo/TurboPFor", 						"", 0,BLK_SIZE }, 
 
  { AM_SIMPLE8B, 	"Simple8b", 		C_SIMPLE8B,		"",	"Simple-8b optimized",	"",	"", 						"", 0,1024 },

  { PC_SIMPLE16, 	"PC_Simple16", 		C_POLYCOM,		"",	"Polycom",				"",	"https://github.com/encode84/bcm", 								"", 0,BLK_V128 }, 
  { PC_OPTPFD,     	"PC_OptPFD",		C_POLYCOM,    	"",	"Polycom",				"",	"", 															"", 0,BLK_V128},
  { PC_VBYTE,  		"PC_Vbyte",			C_POLYCOM, 		"",	"Polycom",				"",	"https://github.com/jibsen/brieflz", 							"" }, 
  { PC_RICE,  		"PC_Rice",			C_POLYCOM, 		"",	"Polycom optimized",	"",	"https://github.com/jibsen/brieflz", 							"", 0,BLK_V128}, 

  { P_VARINTG8IU,	"VarintG8IU", 		C_VARINTG8IU, 	"",	"VarintG8IU",			"Apache license",	"", 						""},
  { P_MASKEDVBYTE,	"MaskedVbyte", 		C_MASKEDVBYTE,	"",	"MaskedVbyte",			"Apache license",	"http://maskedvbyte.org", 									""},
  { P_STREAMVBYTE,	"StreamVbyte", 		C_STREAMVBYTE,	"",	"StreamVbyte",			"Apache license",	"", 						""},

  { FP_FASTPFOR,	"FP_FastPFor", 		C_FASTPFOR, 	"",	"FastPFor",				"",	"", 										""/*, 0,BLK_SIZE*/},
  { FP_SIMDFASTPFOR,"FP_SimdFastPFor", 	C_FASTPFOR, 	"",	"FastPFor",				"",	"", 										""/*, 0,BLK_SIZE*/},
  { FP_OPTPFOR,		"FP_OptPFor",  		C_FASTPFOR, 	"",	"FastPFor",				"",	"", 										"", 0},
  { FP_SIMDOPTPFOR,	"FP_SIMDOptPFor",  	C_FASTPFOR, 	"",	"FastPFor",				"",	"", 										"", 0},
  { FP_VBYTE,		"FP_VByte", 		C_FASTPFOR, 	"",	"FastPFor",				"",	"", 										""/*, 0,BLK_SIZE*/},
  { FP_SIMPLE8BRLE,	"FP_Simple8bRLE", 	C_FASTPFOR, 	"",	"FastPFor",				"",	"", 										""/*, 0,BLK_SIZE*/},

  { SC_SIMDPACK128,	"SC_SIMDPack128", 	C_SIMDCOMP, 	"",	"Simdcomp",				"",	"", 										"", 0,BLK_V128},
  { SC_SIMDPACK256,	"SC_SIMDPack256", 	C_SIMDCOMP, 	"",	"Simdcomp",				"",	"", 										"", 0,BLK_V256},
  { SC_FOR,		   	"SC_For", 			C_SIMDCOMP, 	"",	"Simdcomp",				"",	"", 										"", 0,BLK_V128},
  { SC_FORDA,	 	"SC_ForDA", 		C_SIMDCOMP, 	"",	"Simdcomp",				"",	"", 										"", 0,BLK_V128},

// { CL_FASTPFORD1,	"CL.SIMDPFORD1", 	C_SIMDCOMP, 	"",	"Simdcomp",				"",	"", 										"", 0,BLK_V128},

  { LF_FOR,			"LibFor_For", 		C_SIMDCOMP, 	"",	"LibFor",				"",	"", 										"", 0,BLK_V128},
  { LF_FORX,		"LibFor_ForDA",		C_SIMDCOMP, 	"",	"LibFor",				"",	"", 										"", 0,BLK_V128},

  { LI_PACK,		"LI_Pack",			C_LITTLEPACK, 	"",	"LittlePack",			"",	"", 										""/*, 0,BLK_V128*/},
  { LI_TURBOPACK,	"LI_TurboPack",		C_LITTLEPACK, 	"",	"LittlePack",			"",	"", 										""/*, 0,BLK_V128*/},
  { LI_SCPACK,		"LI_SuperPack",		C_LITTLEPACK, 	"",	"LittlePack",			"",	"", 										""/*, 0,BLK_V128*/},
  { LI_HORPACK,		"LI_HorPack",		C_LITTLEPACK, 	"",	"LittlePack",			"",	"", 										""/*, 0,BLK_V128*/},
  { LI_BMIPACK,		"LI_BMIPack256",	C_LITTLEPACK, 	"",	"LittlePack",			"",	"", 										""/*, 0,BLK_V128*/},

  { P_QMX,			"qmx",		    	C_QMX, 			"",	"QMX",			"https://bitbucket.org/andrewtrotman/bench.git",	"", 										""/*, 0,BLK_V128*/},
  { P_QMX2,			"qmx2",		    	C_QMX, 			"",	"QMX",			"https://bitbucket.org/andrewtrotman/bench.git",	"", 										""/*, 0,BLK_V128*/},
  { P_QMX3,			"qmx3",		    	C_QMX, 			"",	"QMX",			"https://bitbucket.org/andrewtrotman/bench.git",	"", 										""/*, 0,BLK_V128*/},
  { P_QMX4,			"qmx4",		    	C_QMX, 			"",	"QMX",			"https://bitbucket.org/andrewtrotman/bench.git",	"", 										""/*, 0,BLK_V128*/},
  
  { P_LZT,			"LzTurbo",	    	C_LZTURBO, 		"",	"LzTurbo",				"",	"https://sites.google.com/site/powturbo", 						"20,21,22,32", 0,BLK_SIZE },
  { P_VSHUF,		"VSimpleANS",		C_LZTURBO, 		"",	"LzTurbo",				"",	"https://sites.google.com/site/powturbo", 	"20,21,22,32", 0,BLK_SIZE },
  { LZ4_,  			"lz4",				C_LZ4, 			"",		"Lz4",					"BSD license",		"https://github.com/Cyan4973/lz4", 							"" }, 
  { LZ4_S1,  		"lz4_bitshufle",			C_LZ4, 			"",		"Lz4",					"BSD license",		"https://github.com/Cyan4973/lz4", 						    "", 0,BLK_SIZE }, 
  { LZ4_S4,  		"lz4_tp4",			C_LZ4, 			"",		"Lz4",					"BSD license",		"https://github.com/Cyan4973/lz4", 							"", 0,BLK_SIZE }, 
  { LZ4_S8,  		"lz4_tp8",			C_LZ4, 			"",		"Lz4",					"BSD license",		"https://github.com/Cyan4973/lz4", 							"", 0,BLK_SIZE }, 
  { P_ZLIB, 		"zlib", 			C_ZLIB, 		"1.2.8","zlib",					"zlib license",		"http://zlib.net\thttps://github.com/madler/zlib", 			"", 0,BLK_SIZE },
// { P_ZSTD, 		"zstd", 		C_ZSTD,		"1.0.0","ZSTD",					"BSD license+Patents","https://github.com/facebook/zstd", 													"1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22" },
  //-----------------------------------------------------------------------------------	  
  { P_MCPY, 		"memcpy", 			C_MEMCPY, 		".",		"memcpy",		"------------",		"--------------------------------------",   "" },
  { P_COPY, 		"copy",		    	C_MEMCPY,  		".",		"copy",		"",					"",											"" },
  //----- Transform --------------

  { P_BITSHUFFLE, 	"BitShuffle", 		C_TURBOPFOR,	"",	"BitShuffle",			"",	"", 															"", 0,BLK_SIZE }, 
  { BS_SHUFFLE, 		"Blosc_Shuffle",	C_C_BLOSC,		"",	"Blosc",				"",	"https://github.com/Blosc/c-blosc2", 							"", 0,BLK_SIZE }, 

//{ P_MYCODEC, 	"mycodec",			C_MYCODEC, 	"0",		"My codec",				"           ",		"",																						"" },
    #ifdef LZTURBO
//  #include "../beplugr.h"
    #endif  
  { -1 }
};
//-----------------------------------
#define VBPUT32(a,b) vbxput32(a,b)
#define VBGET32(a,b) vbxget32(a,b)

#define _TRANSFORM     // Transform functions : transpose,zigzag
#define _TP_BITS 1     // transpose bits (4,8,16,32,64)

//--------------------------------------- TurboPFor ----------------------------
  #if C_BITSHUFFLE      
#define   BITSHUFFLE(in,n,out)   bshuf_bitshuffle(in, out, (n)/4, 4, 0);  memcpy((char *)out+((n)&(~31)),(char *)in+((n)&(~31)),(n)&31)
#define BITUNSHUFFLE(in,n,out)   bshuf_bitunshuffle(in, out, (n)/4, 4, 0);memcpy((char *)out+((n)&(~31)),(char *)in+((n)&(~31)),(n)&31)
  #endif 

  #if _TP_BITS == 16
#define   TRANSPOSE(in,n,out)   transpose2(in,n,out)
#define UNTRANSPOSE(in,n,out) untranspose2(in,n,out)
  #elif _TP_BITS == 64
#define   TRANSPOSE(in,n,out)   transpose8(in,n,out)
#define UNTRANSPOSE(in,n,out) untranspose8(in,n,out)
  #elif _TP_BITS == 128
#define   TRANSPOSE(in,n,out)   transpose16(in,n,out)
#define UNTRANSPOSE(in,n,out) untranspose16(in,n,out)
  #else
#define   TRANSPOSE(in,n,out)   transpose4(in,n,out)
#define UNTRANSPOSE(in,n,out) untranspose4(in,n,out)
  #endif 

#define PAD8(__x) (((__x)+7)/8)

unsigned char *u32enc(unsigned *__restrict in, int n, unsigned *__restrict out) { unsigned *ip; 
    #if 0
  memcpy(out,in,n*4); return (unsigned char *)(out+n);
    #else
  for(ip = in; ip != in+(n&~3); ) {
    *out++ = *ip++;
    *out++ = *ip++;
    *out++ = *ip++;
    *out++ = *ip++;
  }
  while(ip <  in+n) *out++ = *ip++;
  return (unsigned char *)out;
   #endif
}

unsigned char *u32dec(unsigned *__restrict in, int n, unsigned *__restrict out) { unsigned *op; 
    #if 0
  memcpy(out,in,n*4); return (unsigned char *)(in+n);
    #else
  for(op = out; op != out+(n&~3); ) { 
    *op++ = *in++;
    *op++ = *in++;
    *op++ = *in++;
    *op++ = *in++;
  }
  while(op < out+n) *op++ = *in++;
  return (unsigned char *)in;
  #endif
}

unsigned char *_bitunpackx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out           , unsigned b) { unsigned i,k=0; for(i=0; i < n; i++,k+=b ) *out++ = _bitgetx32(in, k, b); return in + PAD8(n*b); }

// direct access functions included for demonstration only. Use the bulk functions instead, if you are decompressing most of the values
unsigned char *bitf1unpackx32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, int start, unsigned b) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, i, b)+start+i+1; return in + PAD8(n*b); }
unsigned char *bitfunpackx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, int start, unsigned b) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, i, b)+start; return in + PAD8(n*b); }
//---------------------------------------------- plugins --------------------------------------------------------
#include "conf.h"
  #ifndef max
#define max(x,y) (((x)>(y)) ? (x) : (y))
  #endif

void libmemcpy(unsigned char *dst, unsigned char *src, int len) {
  void *(*memcpy_ptr)(void *, const void *, size_t) = memcpy;
  if (time(NULL) == 1) 
    memcpy_ptr = NULL;
  memcpy_ptr(dst, src, len);
}

int codini(size_t insize, int codec) {
  switch(codec) {
      #if C_C_BLOSC2
    case BS_LZ4:  blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_LZ4_COMPNAME);break;
    case BS_ZLIB: blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_ZLIB_COMPNAME); break;
    case BS_LZ:   blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_BLOSCLZ_COMPNAME); break;
      #endif     
      #if C_VARINTG8IU
    case P_VARINTG8IU: VarIntG8IU();
      #endif
       #ifdef LZTURBO  
 //   #include "../beplug0.h"
      #endif
  }
}  

void codexit(int codec) {} 

//-------------- Sorted integer array : Delta/Differential compression (mode=0 increasing, mode=1 strictly increasing sequence) ---------------
#define VBPUT32(a,b) { unsigned _xx= b; vbxput32(a,_xx);}
#define VBGET32(a,b) vbxget32(a,b)

unsigned char *codcomps(unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int inc) { 
  unsigned *in = (unsigned *)_in, n = (_n+3) / 4,i; 	  
 
  unsigned pa[BLK_SIZE+2048],x,b;
  
  switch(codec&0x3f) {
      //----------- copy ---------------------------------------------------------------------------------------------------------
    case P_COPY:                                                             						return u32enc( in, n, (unsigned *)out);
    case P_MCPY: 	   memcpy(out, _in, _n); 														return out+_n;
	  
    case TB_VBYTE:                                                  VBPUT32(out, in[0]);		    return inc?vbd1enc32(in+1, n-1, out, in[0]):vbdenc32(in+1, n-1, out, in[0]);
    case TB_VSIMPLE:   bitdelta32( in+1, --n, pa, in[0], inc); 		VBPUT32(out, in[0]);        	return vsenc32(  pa, n, out);
    case TB_ELIASFANO: x = *in++;                                   VBPUT32(out, x); --n;        
      if(inc) {   																					return efano1enc32(in, n, out, x+1); }
      else {      																					return efanoenc32( in, n, out, x  ); }
	  #if C_TURBOPFORV
    case TB_ELIASFANOV: x = *in++;                                   VBPUT32(out, x); --n;        
      if(inc) {   																					return n == 128?efano1enc128v32(in, n, out, x+1):efano1enc32(in, n, out, x+1); }
      else {      																					return n == 128?efanoenc128v32( in, n, out, x  ):efanoenc32( in, n, out, x  ); }
    case TB_PFOR128:  bitdelta32( in+1, --n, pa, in[0], inc); 		VBPUT32(out, in[0]);       		return n == 128?p4enc128v32(pa, n, out):p4enc32(pa, n, out);
    case TB_PACK128V: if(inc) { b = bitd132(in+1, --n, in[0]);      VBPUT32(out, in[0]); *out++=b; 	return n < 128?bitd1pack32(in+1, n, out, in[0], b):bitd1pack128v32(in+1, n, out, in[0], b); }
                      else {    b = bitd32( in+1, --n, in[0]); 		VBPUT32(out, in[0]); *out++=b; 	return n < 128?bitdpack32( in+1, n, out, in[0], b):bitdpack128v32( in+1, n, out, in[0], b); }
        #ifdef __AVX2__
    case TB_PFOR256:  bitdelta32( in+1, --n, pa, in[0], inc); 		VBPUT32(out, in[0]);       		return n == 256?p4enc256v32(pa, n, out):p4enc32(pa, n, out);
    case TB_PACK256V:  b = bitdelta32( in+1, --n, pa, in[0], inc); 	VBPUT32(out, in[0]); *out++=b;	return n == 256?bitpack256v32(pa, n, out,b):bitpack32(pa, n, out,b);
    //case TB_PACK256V: if(inc) { b = bitd132(in+1, --n, in[0]);      VBPUT32(out, in[0]); *out++=b; 	return n < 256?bitd1pack32(in+1, n, out, in[0], b):bitd1pack256v32(in+1, n, out, in[0], b); }
    //                  else {    b = bitd32( in+1, --n, in[0]); 		VBPUT32(out, in[0]); *out++=b; 	return n < 256?bitdpack32( in+1, n, out, in[0], b):bitdpack256v32( in+1, n, out, in[0], b); }
        #endif 
      #endif
    case TB_PFORDA:   DELTR( in, n, inc, pa);                     	VBPUT32(out, in[0]);       		return p4encx32(  pa+1, n-1, out);
    case TB_FOR :  
    case TB_FORDA:    if(inc) { b = bitf132(in+1, --n, in[0]);    	VBPUT32(out, in[0]); *out++=b; 	return bitf1pack32(in+1, n, out, in[0], b); }
                      else {    b = bitf32( in+1, --n, in[0]);    	VBPUT32(out, in[0]); *out++=b; 	return bitfpack32( in+1, n, out, in[0], b); }

    case TB_PACK128H:
    case TB_PACK:     if(inc) { b = bitd132(in+1, --n, in[0]);    	VBPUT32(out, in[0]); *out++=b; 	return bitd1pack32(in+1, n, out, in[0], b); }
                      else {    b = bitd32( in+1, --n, in[0]); 		VBPUT32(out, in[0]); *out++=b; 	return bitdpack32( in+1, n, out, in[0], b); } 
      #if C_SIMPLE8B  
    case AM_SIMPLE8B:   b = bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]);                   if(b>28) die("simple-8b overflow.bits size>28\n");
																									return vs8benc(  pa, n, out);
      #endif 

	  #if C_FASTPFOR
    case FP_VBYTE:       bitdelta32( in, n, pa, -inc, inc);                                  		return vbyteenc( pa, n, (unsigned *)out); 
      #endif

	  #if C_MASKEDVBYTE
    case P_MASKEDVBYTE:                                                                        		return out+vbyte_encode_delta(in, n, out, 0);     
      #endif  
	
      #if C_LIBFOR
    case LF_FORX: //b = bitf32( in+1, --n, in[0]); *(unsigned *)out = in[0]; out+= 4; *out++=b; return out + for_compress_bits( in+1, out, n, in[0], b); //if(b < 0) b = maxbits(in), *out++ = b; return out + for_compress_bits(in, out, n, 0, b);
    case LF_FOR:  return out + for_compress_sorted(in, out, n);
      #endif

	  #if C_POLYCOM
    case PC_VBYTE:      bitdelta32( in, n, pa, -inc, inc);                                  		return vbpolyenc(pa, n, out);
    case PC_SIMPLE16:   b = bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]);                                                     if(b>28) die("simple16 overflow.bits size>28\n");
																									return vs16enc(  pa, n, (unsigned *)out); 
    case PC_OPTPFD: b = bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]);												if(b>28) die("optp4 overflow.bits size>28\n");
																									return optpfdenc32(pa, n, out);
      /*b = bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]);                                                     if(b>28) die("optp4 overflow.bits size>28\n");
      if(n < 128)                                                                           return vbyteenc(  pa, n, (unsigned *)out);
      else {                                                                                return out + OPT4(pa, n, (unsigned *)out); }*/
      #endif

	  #if C_QMX
    //case P_QMX:  { bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]); unsigned char *q = qmx_enc(pa, n, out+4); *(unsigned *)out = q - (out+4); return q; }
	  #endif
	  
	  #if C_QMX
    case P_QMX:  { bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]); ANT_compress_qmx    qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)pa, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX2: { bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]); ANT_compress_qmx_v2 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)pa, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX3: { bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]); ANT_compress_qmx_v3 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)pa, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX4: { bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]); ANT_compress_qmx_v4 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)pa, (uint64_t)n); ctou32(out)=r; return out+4+r; }	 
	  #endif
	  

      #if C_SIMDCOMP				  
    case SC_FOR:
    case SC_FORDA:            b = bitf32( in+1, --n, in[0]);    VBPUT32(out, in[0]); *out++=b; if(n<128) return (unsigned char *)simdpackFOR_length(in[0], (in+1), n, (__m128i *)out, b);
    case SC_SIMDPACK128:                                                                                   
      if(n < 129) { bitdelta32( in, n, pa, -inc, inc);                                        return vbyteenc((unsigned *)pa, n, (unsigned *)out); }
      else { b = simdmaxbitsd1(in[0], in+1);                   VBPUT32(out, in[0]); *out++=b; return simdpackwithoutmaskd1n(in+1, n-1, (unsigned *)out, in[0], b); }   
	  #endif																					  else {                                   simdpackFOR(in[0], (in+1),    (__m128i *)out, b); return out + simdpackFOR_compressedbytes(n, b); } 

	  #if C_STREAMVBYTE
    case P_STREAMVBYTE: 																		return out +  streamvbyte_delta_encode(in, n, out, inc);
      #endif  
	  
      #if C_VARINTG8IU
    case P_VARINTG8IU:     bitdelta32( in, n, pa, -inc, inc);                                  return vintg8enc(pa, n, out);
      #endif
      // --------- transpose + lz77 ------------------------------------------------------------------------------------------------
      #if C_TRANSFORM
    case TB_ZIGZAG_32:  b = bitzigzag32(in, n, (unsigned *)out,  0); return out + n*4;
    case TB_TP8_32:  bitdelta32(in, n, (unsigned *)sbuf, -inc, inc); _transpose4( (unsigned char *)sbuf, n*4, out); return out + n*4;
    case TB_TP8V_32: bitdelta32(in, n, (unsigned *)sbuf, -inc, inc); TRANSPOSE( (unsigned char *)sbuf, n*4, out); return out + n*4;
    case TB_TP4V_32: bitdelta32(in, n, (unsigned *)sbuf, -inc, inc); transposen4( (unsigned char *)sbuf, n*4, out); return out + n*4;
    case TB_DELTA_32: bitdelta32(in, n, (unsigned *)out,  -inc, inc); return out + n*4; 
      #endif

      // --------- delta + transpose + lz77 ----------------------------------------------------------------------------------------
      #if C_LZTURBO
    case P_LZT10:{ bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 0; lz.hbits = 16; return out + lz8c01(&lz); }
    case P_LZT20:{ bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 0; lz.hbits = 16; return out + lzbc01(&lz); }
    case P_LZT22:{ bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 2; lz.hbits = 26; return out + lzbc2(&lz);  }
    case P_VSHUF:{ bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); unsigned char *p = mheenc(sbuf, n*4,    MH_BLK, out+4, out+n*4+MH_BLK); ctou32(out) = p-(out+4); return p; }
    //case P_VSHUF:{ bitdelta32( in+1, --n, pa, in[0], inc); VBPUT32(out, in[0]); unsigned char *p=vsenc32(pa, n, sbuf); p = mheenc(sbuf, p-sbuf, MH_BLK, out+4, out+n*4+1024); ctou32(out) = p-(out+4); return p; }
      #endif
 
      #if C_C_BLOSC
    case BS_LZ:
    case BS_LZ4: 
    case BS_ZLIB:    return out + blosc_compress(1/*clevel*/, BLOSC_SHUFFLE, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
    case BS_LZ_1:
    case BS_LZ4_1: 
    case BS_ZLIB_1:  return out + blosc_compress(1/*clevel*/, BLOSC_DELTA, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
      #endif

      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_S1: { bitdelta32(in, n, (unsigned *)out, -inc, inc);  BITSHUFFLE((unsigned char *)out, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4); } //  bshuf_bitshuffle(out, sbuf, n*4/32, 32, 0);
        #endif
    case LZ4_S4: { bitdelta32(in, n, (unsigned *)out, -inc, inc); transposen4((unsigned char *)out, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4); } //  bshuf_bitshuffle(out, sbuf, n*4/32, 32, 0);
    case LZ4_S8: { bitdelta32(in, n, (unsigned *)out, -inc, inc);   TRANSPOSE((unsigned char *)out, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4); } //  bshuf_bitshuffle(out, sbuf, n*4/32, 32, 0);
      #endif
	    
      #if C_ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); uLongf outlen = n*4; int rc = compress2(out+4, &outlen, sbuf, n*4, codec-P_ZLIB1+1); if(rc != Z_OK) die("zlib compress2 rc=%d\n", rc); *(unsigned *)out = outlen; return out + 4 + outlen; }
      #endif
    case P_MAX ... 63: break;
  }
  return out;
} 

unsigned char *coddecomps(unsigned char *in, unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int inc) { 
  unsigned *out = (unsigned *)_out, n = (outlen+3) / 4,x,b; 	
  switch(codec&0x3f) { 
      //------------- copy -------------------------------------------------------
    case P_COPY:                                          return u32dec( (unsigned *)in, n, out);
    case P_MCPY: 	   memcpy(_out, in, _n); 		  	  return _out+_n;

    case TB_PFORDA:   VBGET32(in, x);*out = x;            return inc?p4fdecx32(      in, n-1, out+1, x   ):p4f0decx32(in, n-1, out+1, x);
    case TB_VBYTE: 	  VBGET32(in, x);*out = x; 	  	  	  return inc?vbd1dec32(      in, n-1, out+1, x   ):vbddec32(in, n-1, out+1, x);
    case TB_VSIMPLE:  VBGET32(in, x);*out = x;            in = vsdec32(              in, n-1, out+1);      bitundx32(out, n, -inc, inc); break;
    case TB_FOR:      VBGET32(in, x);*out = x; b = *in++; return inc?bitf1unpack32(  in, n-1, out+1, x, b):bitfunpack32(  in, n-1, out+1, x, b);
    case TB_FORDA:    VBGET32(in, x);*out = x; b = *in++; return inc?bitf1unpackx32( in, n-1, out+1, x, b):bitfunpackx32( in, n-1, out+1, x, b);      
    case TB_PACK:     VBGET32(in, x);*out = x; b = *in++; return inc?bitd1unpack32(  in, n-1, out+1, x, b):bitdunpack32(  in, n-1, out+1, x, b);
    case TB_ELIASFANO:VBGET32(in, x); *out++ = x; --n;
      if(inc) {                                           return efano1dec32( in, n, out, x+1); }
      else {                                              return efanodec32( in, n, out, x); }
	  #if C_TURBOPFORV
    case TB_ELIASFANOV:VBGET32(in, x); *out++ = x; --n;
      if(inc) {                                           return n==128?efano1dec128v32(in,  n, out,  x+1 ):efano1dec32(in, n, out, x+1); }
      else {                                              return n==128?efanodec128v32( in,  n, out,  x   ):efanodec32( in, n, out, x); }
    case TB_PFOR128:  VBGET32(in, x); *out++ = x; --n;  //__builtin_prefetch(in+256);
      if(inc) {                                           return n==128?p4d1dec128v32(  in,  n, out,  x   ):p4d1dec32(in, n, out, x); }
      else {                                              return n==128?p4ddec128v32(   in,  n, out,  x   ):p4ddec32( in, n, out, x); }
    case TB_PACK128V:    VBGET32(in, x);*out = x; b = *in++;
      if(n <= 128) {                                      return inc?bitd1unpack32(  in, n-1, out+1, x, b):bitdunpack32(    in,n-1, out+1, x, b); } 
      else {                                              return inc?bitd1unpack128v32(in,n,  out+1, x, b):bitdunpack128v32(in,n,   out+1, x, b); }
        #ifdef __AVX2__
    case TB_PFOR256:  VBGET32(in, x); *out++ = x; --n;  //__builtin_prefetch(in+256);
      if(inc) {                                           return n==256?p4d1dec256v32(  in, n,   out,   x   ):p4d1dec32(in, n, out, x); }
      else {                                              return n==256?p4ddec256v32(   in, n,   out,   x   ):p4ddec32( in, n, out, x); }
    /*case TB_PACK256V: VBGET32(in, x);*out = x; b = *in++;
      if(n <= 256)                                       return inc?bitd1unpack32(   in, n-1, out+1, x, b):bitdunpack32(  in, n-1, out+1, x, b);
      else                                               { in = bitunpack256v32(     in,      out+1,    b);bitundx32(out, n, -inc, inc); } break;*/
    case TB_PACK256V: VBGET32(in, x);*out = x; b = *in++;  
      if(n <= 256) {                                      return inc?bitd1unpack32(    in, n-1, out+1, x, b):bitdunpack32(     in, n-1, out+1, x, b); } 
      else {                                              return inc?bitd1unpack256v32(in,      out+1, x, b):bitdunpack256v32( in,      out+1, x, b); }
	    #endif
	  #endif

    /*case TB_PACK128H:    VBGET32(in, x);*out = x; b = *in++;
      if(n <= 128) {                                      return inc?bitd1unpack32(  in, n-1, out+1, x, b):bitdunpack32(  in, n-1, out+1, x, b); } 
      else {                                              return inc?bitd1unpack128h32 (in, out+1, x, b):bitdunpack128h32( in, out+1, x, b); }*/
	
      #if C_SIMPLE8B  
    case AM_SIMPLE8B: VBGET32(in, x); *out = x; in = vs8bdec(            in, n-1, out+1);      bitundx32(out, n, -inc, inc); break;
	  #endif

	  #if C_FASTPFOR
    case FP_VBYTE:                              in = vbytedec(           in, n,   out);          bitundx32(out, n, -inc, inc); break;
	  #endif
	  
      #if C_LIBFOR
    case LF_FORX:   return for_selectx(in, n, out); //{ out[0] = *(unsigned *)in, b = in[4]; return for_selectx(in+5, n-1, out+1, out[0], b); }
    case LF_FOR:    return in + for_uncompress(in, out, n);
      #endif

      #if C_MASKEDVBYTE
    case P_MASKEDVBYTE:                              in += masked_vbyte_decode_delta(in, out,   n, 0);                       break;
      #endif

	  #if C_POLYCOM
    case PC_VBYTE:                              in = vbpolydec(          in, n,   out);          bitundx32(out, n, -inc, inc); break;
    case PC_SIMPLE16: VBGET32(in, x); *out = x; in = vs16dec((unsigned *)in, n-1, out+1);      bitundx32(out, n, -inc, inc); break;
    case PC_OPTPFD:   VBGET32(in, x); *out = x; in = optpfddec32(in,n-1,out+1); bitundx32(out+1, n-1, x, inc); break;
      /*if(n < 129)                          in = vbytedec(in, n, out);
      else {      _VBGET32(in, x, *out = x); unsigned all_array[2048]; in = (unsigned char *)detailed_p4_decode(out+1, (unsigned *)in, all_array); }
                                                                                            bitundx32(out, n, -inc, inc); break;*/
	  #endif
    //case P_QMX:  { VBGET32(in, x); *out = x; unsigned l = *(unsigned *)in; in = qmx_dec(in+4, l, out+1, n-1); bitundx32(out+1, n-1, x, inc); break; }   
      #if C_QMX    										//case P_QMX: return qmx_dec(in+4, ctou32(in), out, n); 
	case P_QMX:  { VBGET32(in, x); *out = x; unsigned l = *(unsigned *)in; ANT_compress_qmx  qmx;   qmx.decompress(out+1, n-1,  in+4, ctou32(in)); bitundx32(out+1, n-1, x, inc); return in+4+ctou32(in);} 
    case P_QMX2: { VBGET32(in, x); *out = x; unsigned l = *(unsigned *)in; ANT_compress_qmx_v2 qmx; qmx.decompress(out+1, n-1,  in+4, ctou32(in)); bitundx32(out+1, n-1, x, inc); return in+4+ctou32(in);}
    case P_QMX3: { VBGET32(in, x); *out = x; unsigned l = *(unsigned *)in; ANT_compress_qmx_v3 qmx; qmx.decompress(out+1, n-1,  in+4, ctou32(in)); bitundx32(out+1, n-1, x, inc); return in+4+ctou32(in);}
    case P_QMX4: { VBGET32(in, x); *out = x; unsigned l = *(unsigned *)in; ANT_compress_qmx_v4 qmx; qmx.decompress(out+1, n-1,  in+4, ctou32(in)); bitundx32(out+1, n-1, x, inc); return in+4+ctou32(in);}	 
      #endif
 	  
	  #if C_SIMDCOMP
    case SC_SIMDPACK128:
      if(n < 129) { in = vbytedec(in, n, out);               bitundx32(out, n, -inc, inc); }
      else {        VBGET32(in, x);*out = x; b = *in++; in = simdunpackd1n((uint32_t *)in, n-1, out+1, out[0], b); } break;
	  
    case SC_FOR:    VBGET32(in, x);*out = x; b = *in++; if(n < 129) return (unsigned char *)simdunpackFOR_length(x, (const __m128i *)in, n-1, (unsigned *)(out+1), b); else { simdunpackFOR(x, (const __m128i *)in, (unsigned *)(out+1), b); return in + simdpackFOR_compressedbytes(n-1, b); }
    case SC_FORDA: { VBGET32(in, x);*out++ = x; b = *in++; unsigned i; for(i=0; i < n-1;i++) out[i] = simdselectFOR(x, (const __m128i *)in, b, i); return in + simdpackFOR_compressedbytes(n-1, b); }
	  #endif

	  #if C_STREAMVBYTE
    case P_STREAMVBYTE:   return in + streamvbyte_delta_decode(in, out, n, inc);
	  #endif  
	  
      #if C_VARINTG8IU
    case P_VARINTG8IU:                              in = vintg8dec(          in, n,   out);          bitundx32(out, n, -inc, inc); break;    
      #endif   
					//---------- transpose + lz77 ----------------------
      #if C_TRANSFORM
    case TB_ZIGZAG_32:memcpy(out, in, n*4); bitunzigzag32(out, n, 0); return in + n*4;
    case TB_TP8_32:  _untranspose4( (unsigned char *)in, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); return in + n*4;
    case TB_TP8V_32: UNTRANSPOSE((unsigned char *)in, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); return in + n*4;
    case TB_TP4V_32: untransposen4((unsigned char *)in, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); return in + n*4;
    case TB_DELTA_32: memcpy(out, in, n*4); bitundx32(out, n, -inc, inc); return in + n*4;
      #endif
                     //---------- delta + transpose + lz77 ----------------------
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_S1: in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4);  BITUNSHUFFLE(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); break;       
        #endif     
    case LZ4_S4: in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4); untransposen4(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); break;       
    case LZ4_S8: in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4);   UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); break;       
      #endif     

      #if C_ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { uLongf outsize = n*4; int l = *(unsigned *)in, rc = uncompress(sbuf, &outsize, in+4, l); in += 4 + l; UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); } break;
      #endif

      #if C_C_BLOSC
    case BS_LZ:
    case BS_LZ4:
    case BS_ZLIB:
    case BS_LZ_1:
    case BS_LZ4_1:
    case BS_ZLIB_1: { blosc_decompress(in, out, n*4); size_t nbytes, cbytes,blocksize; blosc_cbuffer_sizes(in, &nbytes, &cbytes, &blocksize); return in+cbytes; }
      #endif

      #if C_LZTURBO
    case P_LZT10: { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lz8d(&lz);  UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); } break;
    case P_LZT20:
    case P_LZT22: { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzbd(&lz);  UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); } break;
    //case P_VSHUF: VBGET32(in, x);*out = x; in += ransdecompress( in, (n-1)*4, sbuf); vsdec32( sbuf, n-1, out+1); bitundx32(out, n, -inc, inc); break;

    case P_VSHUF:  { unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, n*4); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -inc, inc); } break;
    //case P_VSHUF:  { VBGET32(in, x);*out = x; unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, n*4); vsdec32(sbuf, n-1, out+1); bitundx32(out+1, n-1, x, inc); } break;
      #endif
    case P_MAX ... 63: break;
  } 
  return in;
}

unsigned char *codcomp(unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int b) {
  unsigned *in = (unsigned *)_in, n = (_n+3) / 4; 	
  int i,xb;
  switch(codec&0x3f) {
      //---------- copy ----------------------------------------------------
    case P_COPY:      return u32enc(   in, n, (unsigned *)out);
    case P_MCPY:      return u32enc(   in, n, (unsigned *)out); //memcpy(out, _in, _n); return out+_n;
      // --------- variable byte -------------------------------------------
    case TB_VBYTE: 	  return vbenc32(  in, n, out);
    case TB_VSIMPLE:  return vsenc32(  in, n, out);
      #if C_SIMPLE8B  
    case AM_SIMPLE8B: return vs8benc(  in, n, out);
      #endif
    case TB_ELIASFANO:return out; 
    case TB_FOR :
    case TB_FORDA: 
    case TB_PACK128H:
    case TB_PACK:        if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } return bitpack32(in, n, out, b);
    case TB_PFORDA:   return p4encx32( in, n, out);

	  #if C_TURBOPFORV
    case TB_ELIASFANOV:return out; 
    case TB_PFOR128:  return n == 128?p4enc128v32(in, n, out):p4enc32(in, n, out);
    case TB_PACK128V:    if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } return n != 128?bitpack32(in, n, out, b):bitpack128v32(in, n, out, b);
        #ifdef __AVX2__
    case TB_PFOR256:  return n == 256?p4enc256v32(in, n, out):p4enc32(in, n, out);
    case TB_PACK256V:    if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } return n != 256?bitpack32(in, n, out, b):bitpack256v32(in, n, out, b);
        #endif
      #endif

	  #if C_FASTPFOR
	case FP_FASTPFOR: {   
      size_t nvalue = outsize/4;
      FastPForLib::FastPFor<4> ic; ic.encodeArray((const int32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
      if(n & 127) {
        size_t nvalue2 = outsize/4 - nvalue;
        FastPForLib::VariableByte vc; vc.encodeArray((const int32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
        nvalue += nvalue2;
      }
	  ctou32(out) = nvalue;
	  return out+4+nvalue*4;
    }
	case FP_SIMDFASTPFOR: {   
      size_t nvalue = outsize/4;
      FastPForLib::SIMDFastPFor<4> ic; ic.encodeArray((const int32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
      if(n & 127) {
        size_t nvalue2 = outsize/4 - nvalue;
        FastPForLib::VariableByte vc; vc.encodeArray((const int32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
        nvalue += nvalue2;
      }
	  ctou32(out) = nvalue;
	  return out+4+nvalue*4;
    }
	case FP_SIMDOPTPFOR: {   
      size_t nvalue = outsize/4;
      FastPForLib::SIMDOPTPFor<4> ic; ic.encodeArray((const int32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
      if(n & 127) {
        size_t nvalue2 = outsize/4 - nvalue;
        FastPForLib::VariableByte vc; vc.encodeArray((const int32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
        nvalue += nvalue2;
      }
	  ctou32(out) = nvalue;
	  return out+4+nvalue*4;
    }
    case FP_VBYTE:    //return vbyteenc( in, i, (unsigned *)out);
	  { size_t nvalue=outsize/4; FastPForLib::VariableByte ic; ic.encodeArray((const int32_t *)in, (const size_t)n, (uint32_t *)(out+4), nvalue); ctou32(out)=nvalue; return out+4+nvalue*4; }
    case FP_SIMPLE8BRLE: { size_t nvalue=outsize/4; FastPForLib::Simple8b_RLE<true> ic; ic.encodeArray((const int32_t *)in, (const size_t)n, (uint32_t *)(out+4), nvalue); ctou32(out)=nvalue; return out+4+nvalue*4; }
	  #endif 
	  
      #if C_LIBFOR
    case LF_FORX: //if(b < 0) b = maxbits(in), *out++ = b; return out + for_compress_bits(in, out, n, 0, b);
    case LF_FOR:  return out + for_compress_unsorted(in, out, n);
      #endif
	  
	  #if C_LITTLEPACK
    case LI_PACK:     if(b < 0) { b = maxbits_length(in,n); *out++ = b; } pack32(     in, n, b, out); return out + byte_count(n,b);
    case LI_HORPACK:   	
    case LI_TURBOPACK:if(b < 0) { b = maxbits_length(in,n); *out++ = b; } turbopack32(in, n, b, out); return out + byte_count(n,b);                                                                                                                                
    case LI_SCPACK:   if(b < 0) { b = maxbits_length(in,n); *out++ = b; } scpack32(   in, n, b, out); return out + byte_count(n,b);                                                                                                                                
        #ifdef __AVX2__
    case LI_BMIPACK:  if(b < 0) { b = maxbits_length(in,n); *out++ = b; } bmipack32(  in, n, b, out); return out + byte_count(n,b);                                                                                                                                
        #endif
      #endif

      #if C_MASKEDVBYTE
    case P_MASKEDVBYTE: return out + vbyte_encode(in, n, out);
      #endif
	  
	  #if C_POLYCOM
    case PC_VBYTE:      return vbpolyenc(in, n, out);
    case PC_SIMPLE16:   return vs16enc(  in, n, (unsigned *)out); 
    case PC_RICE:       return rcenc32(  in, n, (unsigned *)out); 
    case PC_OPTPFD:     return optpfdenc32(in, n, out); //if(n < 128) return vbyteenc(in, n, (unsigned *)out); else { unsigned tmp[2048]; for(i = 0; i < n; i++) tmp[i] = in[i]; return out + OPT4(tmp, n, (unsigned *)out); }
      #endif 
	  
	  #if C_QMX
    case P_QMX:  { ANT_compress_qmx    qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)in, (uint64_t)n); ctou32(out)=r; return out+4+r; } // { unsigned char *q = qmx_enc(in, n, out+4); ctou32(out) = q - (out+4); return q;
    case P_QMX2: { ANT_compress_qmx_v2 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)in, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX3: { ANT_compress_qmx_v3 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)in, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX4: { ANT_compress_qmx_v4 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)in, (uint64_t)n); ctou32(out)=r; return out+4+r; }	 
	  #endif
	  
      #if C_SIMDCOMP
    //case SC_PACK:    if(b < 0) { b = maxbits_length(in,n); *out++ = b; } return fastpackwithoutmask32(in, n, (uint32_t *)out, b);                                                                                                                              
    case SC_FOR:
    case SC_FORDA:
    case SC_SIMDPACK128:   if(b < 0) b =    maxbits(in), *out++ = b; return (unsigned char *)simdpack_length(in, n, (__m128i *)out, b);
        #ifdef __AVX2__
    case SC_SIMDPACK256:   if(b < 0) b = avxmaxbits(in), *out++ = b; return (unsigned char *)avxpackwithoutmaskn(in, n, (unsigned *)out, b);
        #endif
      #endif

	  #if C_STREAMVBYTE
    case P_STREAMVBYTE:   return out + streamvbyte_encode(in, n, out); 
      #endif   
      #if C_VARINTG8IU
    case P_VARINTG8IU:    return vintg8enc(in, n, out);
      #endif 
	  
      // --------- transform ----------------------------------------
      #if C_TRANSFORM
    case TB_ZIGZAG_32:   bitzigzag32(in, n, (unsigned *)out, 0);      return out + n*4;
    case TB_TP8_32:      _transpose4( (unsigned char *)in, n*4, out); return out + n*4;
    case TB_TP8V_32:     TRANSPOSE( (unsigned char *)in, n*4, out);   return out + n*4;
    case TB_TP4V_32:     transposen4( (unsigned char *)in, n*4, out);   return out + n*4;
      #endif
      #if C_C_BLOSC      
    case BS_SHUFFLE:     shuffle( 4, n*4, (unsigned char *)in, out); return out + n*4;
      #endif

      #if C_BITSHUFFLE      
    case P_BITSHUFFLE:   bshuf_bitshuffle(in, out, n, 4, 0); return out + n*4;    
      #endif
 
      // --------- transpose + lz77 ----------------------------------------
      #if C_LZTURBO
    case P_LZT10: { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 0; lz.hbits = 16; return out + lz8c01(&lz); }
    case P_LZT20: { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 0; lz.hbits = 16; return out + lzbc01(&lz); }
    case P_LZT22: { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 2; lz.hbits = 26; return out + lzbc2(&lz); }     
    case P_LZT32: { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 2; lz.hbits = 26; return out + lzhc2(&lz); }
    //case P_VSHUF: { unsigned char *p = vsenc32(in, n, sbuf); p = mheenc(sbuf, p-sbuf, MH_BLK, out+2, out+n*4+256); ctou16(out) = p-(out+2); printf("L=%d ", p-(out+2));  return p; } //out + ranscompress( sbuf, xb, out, out+n*4);
    //case P_VSHUF: { unsigned char                           *p = mheenc(in, n*4, MH_BLK, out+4, out+n*4+256); ctou32(out) = p-(out+4); return p; } //out + ranscompress( sbuf, xb, out, out+n*4);
    case P_VSHUF: { unsigned char *p;/* = vsenc32(in, n, sbuf)*/ n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); p = mheenc(sbuf, n/*p-sbuf*/, MH_BLK, out+4, out+n+256); ctou32(out) = p-(out+4); return p; } //out + ranscompress( sbuf, xb, out, out+n*4);
      #endif

	  #if C_BITSHUFFLE      
    case TB_VS_S1:  BITSHUFFLE( (unsigned char *)in, n*4, sbuf); return vsenc32((unsigned *)sbuf, n, out);
	  #endif
    case TB_VS_S4: transposen4( (unsigned char *)in, n*4, sbuf); return vsenc32((unsigned *)sbuf, n, out);
    case TB_VS_S8:   TRANSPOSE( (unsigned char *)in, n*4, sbuf); return vsenc32((unsigned *)sbuf, n, out);
	  
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_S1:  BITSHUFFLE( (unsigned char *)in, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4);
	    #endif
    case LZ4_S4: transposen4( (unsigned char *)in, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4);
    case LZ4_S8:   TRANSPOSE( (unsigned char *)in, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4);
      #endif
	  
      #if C_C_BLOSC
    case BS_LZ:
    case BS_LZ4: 
    case BS_ZLIB:  return out + blosc_compress(1/*clevel*/, BLOSC_SHUFFLE, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
    case BS_LZ_1:
    case BS_LZ4_S1: 
    case BS_ZLIB_1:  return out + blosc_compress(1/*clevel*/, BLOSC_BITSHUFFLE, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
      #endif
	  
      #if C_ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); uLongf outlen = n; int rc = compress2(out+4, &outlen, sbuf, n, codec-P_ZLIB1+1); if(rc != Z_OK) die("zlib compress2 rc=%d\n", rc); *(unsigned *)out = outlen; return out + 4 + outlen; }
      #endif
	  
    case P_MAX ... 63: die("library '%d' not included\n", codec);
  }
  return out;
} 

unsigned char *coddecomp(unsigned char *in, unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int b) { 
  unsigned *out = (unsigned *)_out, n = (outlen+3) / 4; 	
  switch(codec&0x3f) {         
    case P_COPY:    return u32dec(   (unsigned *)in, n, out);
    case P_MCPY:    return u32dec(   (unsigned *)in, n, out); //memcpy(_out, in, _n); return in+_n;

	//    case P_VU:    return vudec32(   in, n, out);
    case TB_VBYTE:      					 return vbdec32(    in, n, out); 

    case TB_PFORDA :    					 return p4decx32( in, n, out);
      #if C_SIMPLE8B  
    case AM_SIMPLE8B:						 return vs8bdec(  in, n, out); 
      #endif
    case TB_VSIMPLE:    					 return vsdec32(  in, n, out); 
    case TB_ELIASFANO:    					 return in;
    case TB_FOR:   		if(b < 0) b = *in++; return bitfunpack32(        in, n, out, 0, b);
    case TB_FORDA: 		if(b < 0) b = *in++; return _bitunpackx32(       in, n, out, b);
    case TB_PACK:    	if(b < 0) b = *in++; return bitunpack32(         in, n, out, b);
 	  #if C_TURBOPFORV
    case TB_PFOR128 :      __builtin_prefetch(in+256);return n == 128?p4dec128v32(in, n, out):p4dec32(in, n, out);
        #ifdef __AVX2__
    case TB_PFOR256 :      __builtin_prefetch(in+256);return n == 256?p4dec256v32(in, n, out):p4dec32(in, n, out);
    case TB_PACK256V:   	if(b < 0) b = *in++; return n != 256?bitunpack32(in, n, out, b):bitunpack256v32(in, n, out, b);
        #endif
    case TB_ELIASFANOV:    					 return in;
   //case TB_PACK128H:   	if(b < 0) b = *in++; return n != 128?bitunpack32(in, n, out, b):bitunpack128h32(in, out, b);
    case TB_PACK128V:   	if(b < 0) b = *in++; return n != 128?bitunpack32(in, n, out, b):bitunpack128v32(in, n, out, b);
      #endif

      #if C_FASTPFOR
	case FP_FASTPFOR: { 	
	  size_t nvalue = n;
      FastPForLib::FastPFor<4> ic; const uint32_t *ip = ic.decodeArray((const int32_t *)(in+4), ctou32(in), out, nvalue);
      if(n & 127) { 
        nvalue = n - nvalue;
	    FastPForLib::VariableByte vc;
		return (unsigned char *)vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
      }
      return (unsigned char *)ip; 
    }

	case FP_SIMDFASTPFOR:  { 	
	  size_t nvalue = n;
      FastPForLib::SIMDFastPFor<4> ic; const uint32_t *ip = ic.decodeArray((const int32_t *)(in+4), ctou32(in), out, nvalue);
      if(n & 127) { 
        nvalue = n - nvalue;
	    FastPForLib::VariableByte vc;
		return (unsigned char *)vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
      }
      return (unsigned char *)ip; 
    }
	case FP_SIMDOPTPFOR:  { 	
	  size_t nvalue = n;
      FastPForLib::SIMDOPTPFor<4> ic; const uint32_t *ip = ic.decodeArray((const int32_t *)(in+4), ctou32(in), out, nvalue);
      if(n & 127) { 
        nvalue = n - nvalue;
	    FastPForLib::VariableByte vc;
		return (unsigned char *)vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
      }
      return (unsigned char *)ip; 
    } 

    case FP_VBYTE:  //return vbytedec( in, n, out); 
	  { size_t nvalue=n; FastPForLib::VariableByte ic;       return (unsigned char *)ic.decodeArray((const int32_t *)(in+4), ctou32(in), (uint32_t *)out, nvalue); }
    case FP_SIMPLE8BRLE:  
	  { size_t nvalue=n; FastPForLib::Simple8b_RLE<true> ic; ic.decodeArray((const int32_t *)(in+4), ctou32(in), (uint32_t *)out, nvalue); return in+4+ctou32(in)*4; }
	#endif
	
      #if C_LIBFOR
    case LF_FORX: 		 return for_selectx(in, n, out); //if(b < 0) b = *in++; return for_selectx(in, n, out, 0, b);//
    case LF_FOR: 		 return in + for_uncompress(in, out, n); //if(b < 0) b = *in++; return in + for_uncompress_bits(in, out, n, 0, b); //return in + for_uncompress(in, out, n);
      #endif

	  #if C_LITTLEPACK
	case LI_PACK:    	 if(b < 0) b = *in++; unpack32(          in, n, b, out); return in + byte_count(n,b);
	case LI_TURBOPACK:   if(b < 0) b = *in++; turbounpack32(     in, n, b, out); return in + byte_count(n,b);
	case LI_SCPACK:   	 if(b < 0) b = *in++; scunpack32(        in, n, b, out); return in + byte_count(n,b);
	case LI_HORPACK:   	 if(b < 0) b = *in++; horizontalunpack32(in, n, b, out); return in + byte_count(n,b);
        #ifdef __AVX2__
	case LI_BMIPACK:   	 if(b < 0) b = *in++; bmiunpack32(       in, n, b, out); return in + byte_count(n,b);
        #endif
      #endif

      #if C_MASKEDVBYTE
    case P_MASKEDVBYTE:  return in + masked_vbyte_decode(in, out, n);
      #endif
	  
      #if C_POLYCOM
    case PC_VBYTE:    	return vbpolydec(in, n, out);
    case PC_SIMPLE16: 	return vs16dec(  (unsigned *)in, n, out);  
    case PC_RICE:     	return rcdec32(  (unsigned *)in, n, out);
    case PC_OPTPFD : 	return optpfddec32(   in, n, out); //if(n < 128) return vbytedec(in, n, out); else { unsigned all_array[2048]; return (unsigned char *)detailed_p4_decode(out, (unsigned *)in, all_array); }
     #endif

      #if C_SIMDCOMP 
	//case SC_PACK:          if(b < 0) b = *in++; return fastunpack32((uint32_t *)in, n, out, b); 
    case SC_FOR:
    case SC_SIMDPACK128:   if(b < 0) b = *in++; return (unsigned char *)simdunpack_length( (__m128i *)in, n, out, b); 
    //case SC_FORDA:  if(b < 0) b = *in++; return simdfor_selectx(in, n, out, 0, b);            
        #ifdef __AVX2__
    case SC_SIMDPACK256:   if(b < 0) b = *in++; return (unsigned char *)avxunpackn( (unsigned *)in, n, out, b); 
        #endif
      #endif

	  #if C_STREAMVBYTE 
    case P_STREAMVBYTE:   return in +  streamvbyte_decode(in, out, n); 
      #endif 
	 
      #if C_QMX    										//case P_QMX: return qmx_dec(in+4, ctou32(in), out, n); 
	case P_QMX:  { ANT_compress_qmx  qmx;   qmx.decompress(out, n,  in+4, ctou32(in)); return in+4+ctou32(in);} 	// { unsigned char *q = qmx_enc(in, n, out+4); ctou32(out) = q - (out+4); return q;
    case P_QMX2: { ANT_compress_qmx_v2 qmx; qmx.decompress(out, n,  in+4, ctou32(in)); return in+4+ctou32(in);}
    case P_QMX3: { ANT_compress_qmx_v3 qmx; qmx.decompress(out, n,  in+4, ctou32(in)); return in+4+ctou32(in);}
    case P_QMX4: { ANT_compress_qmx_v4 qmx; qmx.decompress(out, n,  in+4, ctou32(in)); return in+4+ctou32(in);}	 
      #endif

      #if C_VARINTG8IU
    case P_VARINTG8IU:    return vintg8dec(in, n, out);
      #endif
      //---------- transpose + lz77 ----------------------
      #if C_TRANSFORM
    case TB_ZIGZAG_32:  memcpy(out, in, n*4); bitunzigzag32(out, n, 0); return in + n*4;
    case TB_TP8_32:  _untranspose4(  (unsigned char *)in, n*4, (unsigned char *)out); return in + n*4;
    case TB_TP8V_32:    UNTRANSPOSE( (unsigned char *)in, n*4, (unsigned char *)out); return in + n*4;
    case TB_TP4V_32: untransposen4( (unsigned char *)in, n*4, (unsigned char *)out); return in + n*4;
	  #endif
      #if C_C_BLOSC
    case BS_SHUFFLE:  unshuffle( 4, n*4, (unsigned char *)in, (unsigned char *)out); return in + n*4;
      #endif
		
      #if C_BITSHUFFLE      
    case P_BITSHUFFLE:  bshuf_bitunshuffle(in, out, n, 4, 0); return in + n*4;
      #endif
 
      //---------- transpose + lz77 ----------------------
      #if C_LZTURBO
    case P_LZT10:  { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lz8d(&lz); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); } break;
    case P_LZT20: 
    case P_LZT22:  { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzbd(&lz); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); } break;
    case P_LZT32:  { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzhd(&lz); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); } break;
    //case P_VSHUF:  { unsigned inlen = ctou16(in); printf("l=%d ", inlen); in+=2; in = mhdec(in, inlen, MH_BLK, sbuf, n*4); vsdec32(sbuf, n, out); } break;//      in = ransdecompress( in, n*4, sbuf); 
    //case P_VSHUF:  { unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, out, n*4); } break;//      in = ransdecompress( in, n*4, sbuf); 
    case P_VSHUF:  { unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, n*4); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); /*vsdec32(sbuf, n, out);*/ } break;//      in = ransdecompress( in, n*4, sbuf); 
      #endif
	  
      #if C_BITSHUFFLE      
    case TB_VS_S1:     in = vsdec32(in, n, (unsigned *)sbuf);  BITUNSHUFFLE(sbuf, n*4, (unsigned char *)out); break;/*bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); */ 
      #endif
    case TB_VS_S4:     in = vsdec32(in, n, (unsigned *)sbuf); untransposen4(sbuf, n*4, (unsigned char *)out); break;
    case TB_VS_S8:     in = vsdec32(in, n, (unsigned *)sbuf);   UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); break;/*bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); */ 
      
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_S1:     in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4);  BITUNSHUFFLE(sbuf, n*4, (unsigned char *)out); break;/*bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); */ 
        #endif
    case LZ4_S4:     in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4); untransposen4(sbuf, n*4, (unsigned char *)out); break;
    case LZ4_S8:     in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4);   UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); break;/*bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); */ 
      #endif
	  
      #if C_BLOSC
    case BS_LZ:
    case BS_LZ4:
    case BS_ZLIB:
    case BS_LZ_1:
    case BS_LZ4_1:
    case BS_ZLIB_1: { blosc_decompress(in, out, n*4); size_t nbytes, cbytes,blocksize; blosc_cbuffer_sizes(in, &nbytes, &cbytes, &blocksize); return in+cbytes; }
      #endif
	  
      #if C_ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { uLongf outsize = n*4; int l = *(unsigned *)in, rc = uncompress(sbuf, &outsize, in+4, l); in += 4 + l; UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); } break;
      #endif
    case P_MAX ... 63: die("library '%d' not included\n", codec);
  }
  return in;        
}

char *codver(int codec, char *v, char *s) {
  switch(codec) {  
      #if C_C_BLOSC2
    return BLOSC_VERSION_STRING;
      #endif 
	  #if C_LZ4
    case LZ4_:     sprintf(s,"%d.%d.%d", LZ4_VERSION_MAJOR, LZ4_VERSION_MINOR, LZ4_VERSION_RELEASE); return s;
	  #endif

	  #if C_ZSTD
    case P_ZSTD:    sprintf(s,"%d.%d.%d", ZSTD_VERSION_MAJOR, ZSTD_VERSION_MINOR, ZSTD_VERSION_RELEASE); return s;
      #endif

	default:        strcpy(s,v);
  }
  return s;
}
