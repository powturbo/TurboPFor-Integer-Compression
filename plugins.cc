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
// TurboPFor: Integer Compression - Benchmark App 
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
#define CODEC2 1
  #endif

  #ifdef LZTURBO
#define C_LZTURBO    CODEC2
  #else	
#define C_LZTURBO    0
  #endif

  #ifdef NTRANSFORM
#define TRANSFORM 0
  #else
#define TRANSFORM 1
  #endif
  #ifdef BLOSC
#define C_C_BLOSC    CODEC2
  #else	
#define C_C_BLOSC    0
  #endif

enum { 
#define C_MEMCPY		1
  P_LMCPY,   	// must be 0
  P_MCPY,		// must be 1
  P_COPY,  
  
#define C_TURBOPFOR		CODEC0
  TB_PFDA,   						// actually not working w. mingw 
  TB_FOR,    	
  TB_FORDA,  	
  TB_BP,		
  TB_BPN,		
  TB_VBYTE,    
  TB_VSIMPLE,
  TB_EF, 	
  TB_VSIMPLE_S1,
  TB_VSIMPLE_S4,   
  TB_VSIMPLE_S8,
  #ifdef TURBOPFORH
#define C_TURBOPFORH    CODEC0V
  #else
#define C_TURBOPFORH    0
  #endif
  TB_BP128H,		
#define C_TURBOPFORV	CODEC0V
  TB_FOR128V,    	
  TB_PF128V,    	
  TB_PF256V,
  TB_PFN128V,    	
  TB_PFN256V,
  TB_BP128V,		

  TB_FOR256V,    	
  TB_BP256H,		
  TB_BP256V,		
  TB_EF128V, 	
  TB_EF256V, 	
#define C_BITSHUFFLE	CODEC1
  P_BITSHUFFLE,
  P_BS_LZ,
  P_BS_LZ4, 
  P_BS_ZLIB,
//P_BS_ZSTD,
  P_BS_SHUFFLE, 	
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
  PC_RICE,							// incl. only as demo, crash on some lists
  PC_SIMPLE16,						// limited to 28 bits.
#define C_QMX			CODEC1
  P_QMX,P_QMX2,P_QMX3,P_QMX4,       // crash on gov2
#define C_SIMDCOMP		CODEC1
  SC_PACK,
  SC_SIMDPACK128,
  SC_SIMDPACK256,
  SC_FOR,    	
  SC_FORDA,   	
#define C_SIMPLE8B 		CODEC1		//crash on integers w. size 32 bits !	
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
#define C_LZTURBO       0
  P_LZT,
  P_VSHUF,
  
  P_MAX
};

  #ifndef __SSSE3__
#define C_VINTG8IU 		0
#define C_MASKEDVBYTE 	0			
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
  
  #if C_SIMPLE8B
#include "ext/simple8b.h"       		// optimized simple-8b by powturbo
  #endif  

  #if C_C_BLOSC
#include "ext/c-blosc/blosc/shuffle.h"
#include "ext/c-blosc/blosc/blosc.h"
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
#include "eliasfano.h"
#include "bitutil.h"
#include "transpose.h"
#include "vp4.h"
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

unsigned char *simdfor_selectx( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b) { 
    unsigned i; 
    for(i=0; i < n;i++) out[i] = simdselectFOR(start, (const __m128i *)in, b, i); return in + simdpackFOR_compressedbytes(n, b); 
}
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
 
//------------------------------------------------- plugins: codes+function -------------------------------------------------
struct codecs codecs[] = {
  { C_C_BLOSC, 		"blosc",				"https://github.com/Blosc/c-blosc" }, 
  { C_FASTPFOR, 	"FastPFor",				"https://github.com/lemire/FastPFor" },
  { C_LZ4, 			"lz4",					"https://github.com/Cyan4973/lz4" }, 
  { C_LITTLEPACK, 	"LittleIntPacker",		"https://github.com/lemire/LittleIntPacker" },
  { C_LZTURBO, 		"LzTurbo",				"https://sites.google.com/site/powturbo" },
  { C_MASKEDVBYTE,	"MaskedVbyte",			"http://maskedvbyte.org" },
  { C_POLYCOM,		"Polycom",				"https://github.com/encode84/bcm" }, 
  { C_QMX, 			"QMX",					"https://bitbucket.org/andrewtrotman/bench" },
  { C_SIMDCOMP, 	"simdcomp",				"https://github.com/lemire/simdcomp" },
  { C_SIMPLE8B,		"Simple-8b optimized",	"https://github.com/powturbo/TurboPFor" },
  { C_STREAMVBYTE,	"Streamvbyte",			"https://github.com/lemire/streamvbyte" },
  { C_TURBOPFOR,	"TurboPFor",			"https://github.com/powturbo/TurboPFor"	}, 
  { C_VARINTG8IU, 	"VarintG8IU",			"https://github.com/lemire/FastPFor" },
  { C_ZLIB, 		"zlib",					"http://zlib.net\thttps://github.com/madler/zlib" },
  { C_MEMCPY, 		"memcpy/copy",	    	"--------------------------------------" },
  { -1 }
};

struct plugs plugs[] = {
  { TB_PF128V, 		"TurboPFor", 		C_TURBOPFOR,	BLK_V128,0,"","PFor (SSE2)" }, 
  { TB_PFN128V, 	"TurboPForN", 		C_TURBOPFOR,    0,    	 0,"","PFor (SSE2) large blocks" }, 
  { TB_PF256V, 		"TurboPFor256", 	C_TURBOPFOR,	BLK_V256,0,"","PFor (AVX2)" }, 
  { TB_PFN256V, 	"TurboPFor256N", 	C_TURBOPFOR,    0,    	 0,"","PFor (AVX2) large blocks" }, 
  { TB_PFDA,		"TurboPForDA", 		C_TURBOPFOR,	BLK_V128,0,"","PFor direct access" }, 
  
  { TB_FOR,			"TurboFor", 		C_TURBOPFOR,	BLK_V128,0,"","FOR" }, 
  { TB_FORDA, 		"TurboForDA", 		C_TURBOPFOR,	BLK_V128,0,"","FOR direct access" }, 
  { TB_FOR128V,		"TurboForV", 		C_TURBOPFOR,	BLK_V128,0,"","FOR (SIMD)" }, 
  { TB_FOR256V,		"TurboFor256V", 	C_TURBOPFOR,	BLK_V256,0,"","FOR (AVX2)" }, 

  { TB_BP,			"TurboPack", 		C_TURBOPFOR,	0,    	 0,"","Bit packing (scalar)" }, 
  { TB_BPN,			"TurboPackN", 		C_TURBOPFOR,	0,    	 0,"","Bit packing (scalar) large blocks" }, 
  { TB_BP128V, 		"TurboPackV", 		C_TURBOPFOR,	BLK_V128,0,"","Bit packing (SSE2 Vertical)" }, 
  { TB_BP128H, 		"TurboPackH", 		C_TURBOPFOR,	BLK_V128,0,"","Bit packing (SSE2 Horizontal)" }, 
  { TB_BP256V, 		"TurboPack256V",	C_TURBOPFOR,	BLK_V256,0,"","Bit packing (AVX2 Vertical)" }, 
  
  { TB_VBYTE, 		"TurboVByte", 		C_TURBOPFOR,	0,    	 0,"","Variable byte (scalar)" }, 
  { TB_VSIMPLE, 	"VSimple", 			C_TURBOPFOR,	0,    	 0,"","Variable simple (scalar)" }, 
  { TB_EF, 			"EliasFano",		C_TURBOPFOR,	0,       PG_DLT,"","Elias fano (scalar)" }, 
  { TB_EF128V, 		"EliasFanoV",		C_TURBOPFOR,	BLK_V128,0,"","Eliasfano  (SSE2)" }, 
  { TB_EF256V,		"EliasFano256V",	C_TURBOPFOR,	BLK_V256,0,"","Elias fano (AVX2" }, 

  { TB_VSIMPLE_S1,	"vs_bitshuffle",	C_LZ4,  		BLK_SIZE }, 
  { TB_VSIMPLE_S4,	"vs_s4",			C_LZ4,			BLK_SIZE }, 
  { TB_VSIMPLE_S8,	"vs_s8",			C_LZ4, 			BLK_SIZE }, 
  
  { AM_SIMPLE8B, 	"Simple8b", 		C_SIMPLE8B,		1024,    0,"","64 bits Simple family (instable)" },

  { PC_SIMPLE16, 	"PC_Simple16", 		C_POLYCOM,		BLK_V128,0,"","Simple 16. limited to 28 bits" }, 
  { PC_OPTPFD,     	"PC_OptPFD",		C_POLYCOM, 		BLK_V128,0,"","OptPFD. limited to 28 bits" },
  { PC_VBYTE,  		"PC_Vbyte",			C_POLYCOM,		0,    	 0,"","Variable byte" }, 
  { PC_RICE,  		"PC_Rice",			C_POLYCOM, 		BLK_V128,0,"","Rice coding (instable)" }, 

  { P_VARINTG8IU,	"VarintG8IU", 		C_VARINTG8IU,	0,    	 0,"","Variable byte SIMD" },
  { P_MASKEDVBYTE,	"MaskedVbyte", 		C_MASKEDVBYTE,	0,    	 0,"","Variable byte SIMD" },
  { P_STREAMVBYTE,	"StreamVbyte", 		C_STREAMVBYTE,	0,    	 0,"","Variable byte SIMD" },

  { FP_FASTPFOR,	"FP_FastPFor", 		C_FASTPFOR,	    0,    	 0,"","PFor scalar (inefficient for small blocks)" },
  { FP_SIMDFASTPFOR,"FP_SimdFastPFor", 	C_FASTPFOR,	    0,    	 0,"","PFor SIMD (inefficient for small blocks)" },
  { FP_OPTPFOR,		"FP_OptPFor",  		C_FASTPFOR,	    0,    	 0,"","OptPFor scalar" },
  { FP_SIMDOPTPFOR,	"FP_SIMDOptPFor",  	C_FASTPFOR,	    0,    	 0,"","OptPFor SIMD" },
  { FP_VBYTE,		"FP_VByte", 		C_FASTPFOR,	    0,    	 0,"","Variable byte" },
  { FP_SIMPLE8BRLE,	"FP_Simple8bRLE", 	C_FASTPFOR,	    0,    	 0,"","Simple-8b + rle" },

  { SC_SIMDPACK128,	"SC_SIMDPack128", 	C_SIMDCOMP,		BLK_V128,0,"","Bit packing (SSE4.1)"},
  { SC_SIMDPACK256,	"SC_SIMDPack256", 	C_SIMDCOMP,		BLK_V256,0,"","Bit packing (SSE4.1)"},
  { SC_FOR,		   	"SC_For", 			C_SIMDCOMP,		BLK_V128,0,"","For (SSE4.1)"},
  { SC_FORDA,	 	"SC_ForDA", 		C_SIMDCOMP,		BLK_V128,0,"","For direct access (SSE4.1)"},

//{ CL_FASTPFORD1,	"CL.SIMDPFORD1", 	C_SIMDCOMP, 	"", 0,BLK_V128},
  { P_BS_LZ,  		"blosc_lz",			C_C_BLOSC, 		BLK_SIZE,0, "","TurboPFor's [delta]+transpose+lz4" }, 
  { P_BS_LZ4,  		"blosc_lz4",		C_C_BLOSC, 		BLK_SIZE,0, "","TurboPFor's [delta]+transpose+lz4" }, 
  { P_BS_ZLIB,  	"blosc_zlib",		C_C_BLOSC, 		BLK_SIZE,0, "","TurboPFor's [delta]+transpose+lz4" }, 

  { LF_FOR,			"LibFor_For", 		C_SIMDCOMP, 	BLK_V128,0,"","For"},
  { LF_FORX,		"LibFor_ForDA",		C_SIMDCOMP, 	BLK_V128,0,"","For direct access"},

  { LI_PACK,		"LI_Pack",			C_LITTLEPACK,	0,    	 0,"","Bit packing (scalar)" },
  { LI_TURBOPACK,	"LI_TurboPack",		C_LITTLEPACK,	0,    	 0,"","Bit packing (scalar)" },
  { LI_SCPACK,		"LI_SuperPack",		C_LITTLEPACK,	0,    	 0,"","Bit packing (scalar)" },
  { LI_HORPACK,		"LI_HorPack",		C_LITTLEPACK,	0,    	 0,"","Bit packing (sse4.1 horizontal)" },
  { LI_BMIPACK,		"LI_BMIPack256",	C_LITTLEPACK,	0,    	 0,"","Bit packing (avx2)"	},

  { P_QMX,			"qmx",		    	C_QMX,	    	0,    	 0,"","QMX SIMD (inefficient for small blocks)"	},
  { P_QMX2,			"qmx2",		    	C_QMX,	    	0,    	 0,"","QMX SIMD (inefficient for small blocks)"	},
  { P_QMX3,			"qmx3",		    	C_QMX,	    	0,    	 0,"","QMX SIMD (inefficient for small blocks)"	},
  { P_QMX4,			"qmx4",		    	C_QMX,	    	0,    	 0,"","QMX SIMD (inefficient for small blocks)"	},
  //-------- lz77 + [delta] + transpose/shuffle ---------------
  { P_LZT,			"LzTurbo",	    	C_LZTURBO, 		BLK_SIZE, 0, "20,21,22,32" },
  { P_VSHUF,		"VSimpleANS",		C_LZTURBO, 		BLK_SIZE, 0, "20,21,22,32" },
  { LZ4_,  			"lz4",				C_LZ4,			BLK_SIZE, 0, "","lz4" }, 
  { LZ4_S1,  		"lz4_bitshufle",	C_LZ4,			BLK_SIZE, 0, "","Bitshuffle + lz4" }, 
  { LZ4_S4,  		"lz4_tp4",			C_LZ4,			BLK_SIZE, 0, "","TurboPFor's [delta]+byte transpose + lz4" }, 
  { LZ4_S8,  		"lz4_tp8",			C_LZ4,			BLK_SIZE, 0, "","TurboPFor's [delta]+transpose + lz4" }, 
  { P_ZLIB, 		"zlib", 			C_ZLIB,			BLK_SIZE, 0, "","TurboPFor's [delta]+transpose + zlib" },
  //----- Transform --------------
  { P_MCPY, 		"memcpy", 			C_MEMCPY 				 },
  { P_COPY, 		"copy",		    	C_MEMCPY 				 },

  { TB_TP8_32, 		"TP8s_32", 			C_TURBOPFOR,	BLK_SIZE,0,"","Byte Transpose (scalar)" }, 
  { TB_TP8V_32, 	"TP8_32",			C_TURBOPFOR,	BLK_SIZE,0,"","Byte transpose (sse)" }, 
  { TB_TP4V_32, 	"TP4_32",			C_TURBOPFOR,	BLK_SIZE,0,"","Nibble transpose (sse)" }, 
  { TB_ZIGZAG_32, 	"ZigZag_32", 		C_TURBOPFOR,	BLK_SIZE,0,"","ZigZag encoding (sse2)" }, 
  { TB_DELTA_32, 	"Delta_32", 		C_TURBOPFOR,	BLK_SIZE,0,"","Delta encoding (sse2)"}, 
 
  { P_BITSHUFFLE, 	"BitShuffle", 		C_TURBOPFOR,	BLK_SIZE,0,"","Bit shuffle (SIMD)" }, 
  { P_BS_SHUFFLE, 	"Blosc_Shuffle",	C_C_BLOSC,		BLK_SIZE,0,"","Blosc's shuffle (SIMD)" }, 
    #ifdef LZTURBO
//  #include "../beplugr.h"
    #endif  
  { -1 }
};

struct plugg plugg[] = {
  { "TURBOPFOR","TURBOPFOR",  					"TurboPFor/TurboPFor256/TurboPackV256/TurboPackV/TurboVByte/TurboPack/TurboForDA/EliasFano/VSimple" },
  { "DEFAULT",	"Default",  					"TurboPFor/TurboPFor256/TurboPackV256/TurboPackV/TurboVByte/TurboPack" },
  { "BENCH",	"Benchmark",  					"TurboPFor/TurboPackV/TurboVByte/TurboPack/QMX/FP.SimdFastPfor/FP.SimdOptPFor/MaskedVbyte/StreamVbyte" },
  { "EFFICIENT","Efficient",					"TurboPFor/vsimple/turbovbyte" },

  { "BITPACK",	"Bit Packing",  				"TurboPackV/TurboPack256V/TurboPackH/TurboPack/SC.SimdPack128/SC.SimdPack256" },
  { "VBYTE",	"Variable byte",				"TurboVByte/FP.VByte/PC.Vbyte/VarintG8IU/MaskedVbyte/StreamVbyte" },
  { "SIMPLE",	"Simple Family",				"simple8b/simple16/vsimple/qmx" },
  { "LZ4",		"lz4+bitshufle/transpose 4/8",	"lz4_bitshuffle/lz4_tp4/lz4_tp8" },
  
  { "LI",		"Little Integer",				"LI.Pack/LI.TurboPack/LI.SuperPack/LI.HorPack/LI.BMIPack256" },
  { "" }
};

#define _TRANSFORM     // Transform functions : transpose,zigzag
#define _TP_BITS 1     // transpose bits (4,8,16,32,64)

  #if C_C_BLOSC  
#define   BITSHUFFLE(in,n,out)   bitshuffle(   1, n, (unsigned char *)in, out, NULL) 
#define BITUNSHUFFLE(in,n,out)   bitunshuffle( 1, n, (unsigned char *)in, (unsigned char *)out,NULL)
  #else    
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
  for(ip = in; ip != in+(n&~3); ) {
    *out++ = *ip++;
    *out++ = *ip++;
    *out++ = *ip++;
    *out++ = *ip++;
  }
  while(ip <  in+n) *out++ = *ip++;
  return (unsigned char *)out;
}

unsigned char *u32dec(unsigned *__restrict in, int n, unsigned *__restrict out) { unsigned *op; 
  for(op = out; op != out+(n&~3); ) { 
    *op++ = *in++;
    *op++ = *in++;
    *op++ = *in++;
    *op++ = *in++;
  }
  while(op < out+n) *op++ = *in++;
  return (unsigned char *)in;
}

unsigned char *_bitunpackx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out           , unsigned b) { unsigned i,k=0; for(i=0; i < n; i++,k+=b ) *out++ = _bitgetx32(in, k, b); return in + PAD8(n*b); }

// direct access functions : included for demonstration only. Use the bulk functions instead, if you are decompressing most of the values
unsigned char *bitf1unpackx32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, int start, unsigned b) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, i, b)+start+i+1; return in + PAD8(n*b); }
unsigned char *bitfunpackx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, int start, unsigned b) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, i, b)+start;     return in + PAD8(n*b); }

//---------------------------------------------- plugins --------------------------------------------------------
#include "conf.h"  

int codini(size_t insize, int codec) {
  switch(codec) {
      #if C_C_BLOSC
    case P_BS_LZ4:  blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_LZ4_COMPNAME);break;
    case P_BS_ZLIB: blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_ZLIB_COMPNAME); break;
    case P_BS_LZ:   blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_BLOSCLZ_COMPNAME); break;
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

//--- Sorted integer array : Delta/Differential compression (inc=0 increasing, inc=1 strictly increasing sequence) ---------------
unsigned char *codcomps(unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int inc) { 
  unsigned *in = (unsigned *)_in, n = (_n+3) / 4, i, pa[BLK_SIZE+2048], x, b;
  
  switch(codec) {	  
    case TB_VSIMPLE: x = *in++; bitdelta32( in, --n, pa, x, inc);
	                                 vbxput32(out, x);			return vsenc32(         pa, n, out);
    case TB_VBYTE:   x = *in++; --n; vbxput32(out, x);			return inc?vbd1enc32(   in, n, out, x   ):vbdenc32(     in, n, out, x);
    case TB_EF:      x = *in++; --n; vbxput32(out, x);			return inc?efano1enc32( in, n, out, x+1 ):efanoenc32(   in, n, out, x);
	
    case TB_PFDA:    x = *in++; --n; vbxput32(out, x); DELTR(in,n,x,inc,pa); 
																return p4encx32(pa,n,out);
    case TB_FOR :  
    case TB_FORDA:   x = *in++; --n; vbxput32(out, x); 
      if(inc) { b = bitf132(in, n, x);  *out++=b;				return bitf1pack32(in, n, out, x, b); }
      else {    b = bitf32( in, n, x);  *out++=b;				return bitfpack32( in, n, out, x, b); }

    case TB_BP128H:
    case TB_BP:      x = *in++; --n; vbxput32(out, x); 
      if(inc) { b = bitd132(in, n, x);  *out++=b;				return bitd1pack32(in, n, out, x, b); }
      else {    b = bitd32( in, n, x);  *out++=b;				return bitdpack32( in, n, out, x, b); } 
    case TB_BPN:    											return out+(inc?bitnd1pack32(in, n, out):bitndpack32( in, n, out));
	
	  #if C_TURBOPFORV
    case TB_EF128V:   x = *in++; --n; vbxput32(out, x);         
      if(inc)  													return n == 128?efano1enc128v32(in, n, out, x+1 ):efano1enc32(  in, n, out, x+1);
      else     													return n == 128?efanoenc128v32( in, n, out, x   ):efanoenc32(   in, n, out, x  );
    case TB_PF128V:   x = *in++; --n; vbxput32(out, x);
      if(inc)                     								return n == 128?p4d1enc128v32(  in, n, out, x   ):p4d1enc32(    in, n, out, x); 
      else                                               		return n == 128?p4denc128v32(   in, n, out, x   ):p4denc32(     in, n, out, x); 
    case TB_PFN128V:    										return out+(inc?p4nd1enc128v32( in, n, out      ):p4ndenc128v32(in, n, out));
	
    case TB_FOR128V: x = *in++; --n; vbxput32(out, x); 
      if(inc) { b = bitf132(in, n, x);  *out++=b; 				return n == 128?bitf1pack128v32(in, n, out, x, b):bitf1pack32(  in, n, out, x, b); }
      else {    b = bitf32( in, n, x);  *out++=b;				return n == 128?bitfpack128v32( in, n, out, x, b):bitfpack32(   in, n, out, x, b); }
    case TB_BP128V:  x = *in++; --n; vbxput32(out, x); 
      if(inc) { b = bitd132(in, n, x);  *out++=b;				return n == 128?bitd1pack128v32(in, n, out, x, b):bitd1pack32(  in, n, out, x, b); }
      else {    b = bitd32( in, n, x); 	*out++=b;				return n == 128?bitdpack128v32( in, n, out, x, b):bitdpack32(   in, n, out, x, b); }
        #ifdef __AVX2__
    case TB_FOR256V: x = *in++; --n; vbxput32(out, x); 
      if(inc) { b = bitf132(in, n, x);  *out++=b; 				return n == 256?bitf1pack256v32(in, n, out, x, b):bitf1pack32(  in, n, out, x, b); }
      else {    b = bitf32( in, n, x);  *out++=b;				return n == 256?bitfpack256v32( in, n, out, x, b):bitfpack32(   in, n, out, x, b); }
    case TB_EF256V: x = *in++; --n;  vbxput32(out, x);         
      if(inc)  													return n == 256?efano1enc256v32(in, n, out, x+1 ):efano1enc32(  in, n, out, x+1);
      else     													return n == 256?efanoenc256v32( in, n, out, x   ):efanoenc32(   in, n, out, x  );
    case TB_PF256V: x = *in++; bitdelta32( in, --n, pa, x, inc);
	                                 vbxput32(out, x);       	return n == 256?p4enc256v32(    pa, n, out      ):p4enc32(      pa, n, out);
    case TB_PFN256V:         									return out+(inc?p4nd1enc256v32( in, n, out      ):p4ndenc256v32(in, n, out));
    case TB_BP256V: x = *in++;bitdelta32( in, --n, pa, x, inc);
	                                 vbxput32(out, x);*out++=b; return n == 256?bitpack256v32(  pa, n, out, b   ):bitpack32(    pa, n, out,b);
  /*case TB_BP256V:   x = *in++;                 --n;
      if(inc) { b = bitd132(in, n, x); vbxput32(out, x); *out++=b; return n == 256?bitd1pack256v32(in, n, out, x, b):bitd1pack32(  in, n, out, x, b); }
      else {    b = bitd32( in, n, x); vbxput32(out, x); *out++=b; return n == 256?bitdpack256v32( in, n, out, x, b):bitdpack32(   in, n, out, x, b); }*/
        #endif 
      #endif
	//-------- External libs ----------   
      #if C_SIMPLE8B  
    case AM_SIMPLE8B: b = bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]); if(b>28) die("simple-8b overflow.bits size>28\n");
																return vs8benc(  pa, n, out);
      #endif  
    case P_COPY:                                                return u32enc( in, n, (unsigned *)out);
    case P_MCPY: 	   	memcpy(out, _in, _n); 					return out+_n;
	  #if C_FASTPFOR
    case FP_VBYTE:      bitdelta32( in, n, pa, -inc, inc);      return vbyteenc( pa, n, (unsigned *)out); 
      #endif

	  #if C_MASKEDVBYTE
    case P_MASKEDVBYTE:                                         return out+vbyte_encode_delta(in, n, out, 0);     
      #endif  
	
      #if C_LIBFOR
    case LF_FORX: //b = bitf32( in+1, --n, in[0]); *(unsigned *)out = in[0]; out+= 4; *out++=b; return out + for_compress_bits( in+1, out, n, in[0], b); //if(b < 0) b = maxbits(in), *out++ = b; return out + for_compress_bits(in, out, n, 0, b);
    case LF_FOR:  return out + for_compress_sorted(in, out, n);
      #endif

	  #if C_POLYCOM
    case PC_VBYTE:          bitdelta32( in,     n, pa, -inc, inc);return vbpolyenc(pa, n, out);
    case PC_SIMPLE16:   b = bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]);                    if(b>28) die("simple16 overflow.bits size>28\n");
															    return vs16enc(  pa, n, (unsigned *)out); 
    case PC_OPTPFD: b = bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]);						if(b>28) die("optp4 overflow.bits size>28\n");
																return optpfdenc32(pa, n, out);
      /*b = bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]);                                                     if(b>28) die("optp4 overflow.bits size>28\n");
      if(n < 128)                                                                           return vbyteenc(  pa, n, (unsigned *)out);
      else {                                                                                return out + OPT4(pa, n, (unsigned *)out); }*/
      #endif

	  #if C_QMX
  //case P_QMX:  { bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]); unsigned char *q = qmx_enc(pa, n, out+4); *(unsigned *)out = q - (out+4); return q; }
    case P_QMX:  { bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]); ANT_compress_qmx    qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)pa, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX2: { bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]); ANT_compress_qmx_v2 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)pa, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX3: { bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]); ANT_compress_qmx_v3 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)pa, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX4: { bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]); ANT_compress_qmx_v4 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)pa, (uint64_t)n); ctou32(out)=r; return out+4+r; }	 
	  #endif  

      #if C_SIMDCOMP				  
    case SC_FOR:
    case SC_FORDA:  x = *in++; b = bitf32( in, --n, x); vbxput32(out, x); *out++=b; 	   return (unsigned char *)simdpackFOR_length(x, in, n, (__m128i *)out, b);
    case SC_SIMDPACK128:                                                                                   
      if(n < 129) { bitdelta32( in, n, pa, -inc, inc);                                     return vbyteenc((unsigned *)pa, n, (unsigned *)out); }
      else { b = simdmaxbitsd1(in[0], in+1);               vbxput32(out, in[0]); *out++=b; return simdpackwithoutmaskd1n(in+1, n-1, (unsigned *)out, in[0], b); }   
	  #endif																					  else {                                   simdpackFOR(in[0], (in+1),    (__m128i *)out, b); return out + simdpackFOR_compressedbytes(n, b); } 

	  #if C_STREAMVBYTE
    case P_STREAMVBYTE: 																		return out +  streamvbyte_delta_encode(in, n, out, inc);
      #endif  
	  
      #if C_VARINTG8IU
    case P_VARINTG8IU:     bitdelta32( in, n, pa, -inc, inc);                                  return vintg8enc(pa, n, out);
      #endif
      // --------- transpose + lz77 ------------------------------------------------------------------------------------------------
      #if C_TRANSFORM
    case TB_ZIGZAG_32:b = bitzigzag32(in, n, (unsigned *)out,  0); return out + n*4;
    case TB_TP8_32:   bitdelta32(in, n, (unsigned *)sbuf, -inc, inc); _transpose4( (unsigned char *)sbuf, n*4, out); return out + n*4;
    case TB_TP8V_32:  bitdelta32(in, n, (unsigned *)sbuf, -inc, inc); TRANSPOSE(   (unsigned char *)sbuf, n*4, out); return out + n*4;
    case TB_TP4V_32:  bitdelta32(in, n, (unsigned *)sbuf, -inc, inc); transposen4( (unsigned char *)sbuf, n*4, out); return out + n*4;
    case TB_DELTA_32: bitdelta32(in, n, (unsigned *)out,  -inc, inc); return out + n*4; 
      #endif

      // --------- delta + transpose + lz77 ----------------------------------------------------------------------------------------
      #if C_LZTURBO
    case P_LZT10:{ bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 0; lz.hbits = 16; return out + lz8c01(&lz); }
    case P_LZT20:{ bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 0; lz.hbits = 16; return out + lzbc01(&lz); }
    case P_LZT22:{ bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 2; lz.hbits = 26; return out + lzbc2(&lz);  }
    case P_VSHUF:{ bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); unsigned char *p = mheenc(sbuf, n*4,    MH_BLK, out+4, out+n*4+MH_BLK); ctou32(out) = p-(out+4); return p; }
    //case P_VSHUF:{ bitdelta32( in+1, --n, pa, in[0], inc); vbxput32(out, in[0]); unsigned char *p=vsenc32(pa, n, sbuf); p = mheenc(sbuf, p-sbuf, MH_BLK, out+4, out+n*4+1024); ctou32(out) = p-(out+4); return p; }
      #endif
	  
      #if C_C_BLOSC
    case P_BS_LZ:    
    case P_BS_LZ4:    
    case P_BS_ZLIB: return out + blosc_compress(lev, BLOSC_SHUFFLE, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
    //case P_BS_ZLIB_1:  return out + blosc_compress(lev, BLOSC_SHUFFLE/*BLOSC_DELTA*/, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
      #endif

      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_S1: { bitdelta32(in, n, (unsigned *)out, -inc, inc);  BITSHUFFLE((unsigned char *)out, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4); } //  bshuf_bitshuffle(out, sbuf, n*4/32, 32, 0);
        #endif
    case LZ4_  : return out + LZ4_compress((char *)in, (char *)out, n*4);
    case LZ4_S4: { bitdelta32(in, n, (unsigned *)out, -inc, inc); transposen4((unsigned char *)out, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4); } //  bshuf_bitshuffle(out, sbuf, n*4/32, 32, 0);
    case LZ4_S8: { bitdelta32(in, n, (unsigned *)out, -inc, inc);   TRANSPOSE((unsigned char *)out, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4); } //  bshuf_bitshuffle(out, sbuf, n*4/32, 32, 0);
      #endif
	    
      #if C_ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { bitdelta32(in, n, (unsigned *)out, -inc, inc); TRANSPOSE((unsigned char *)out, n*4, sbuf); uLongf outlen = n*4; int rc = compress2(out+4, &outlen, sbuf, n*4, codec-P_ZLIB1+1); if(rc != Z_OK) die("zlib compress2 rc=%d\n", rc); *(unsigned *)out = outlen; return out + 4 + outlen; }
      #endif
  }
  return out;
} 

unsigned char *coddecomps(unsigned char *in, unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int inc) { 
  unsigned *out = (unsigned *)_out, n = (outlen+3) / 4,x,b; 	
  
  switch(codec) { 
    case TB_VSIMPLE:vbxget32(in, x);*out++ = x; --n; 			in = vsdec32(                 in, n, out); bitundn32(out, n, x+inc, inc); return in;
    case TB_EF:	    vbxget32(in, x);*out++ = x; --n;     		return inc?efano1dec32(       in, n, out, x+1 ):efanodec32(      in, n, out, x);
    case TB_VBYTE: 	vbxget32(in, x);*out++ = x; --n; 	   		return inc?vbd1dec32(         in, n, out, x   ):vbddec32(        in, n, out, x);

    case TB_PFDA:   vbxget32(in, x);*out++ = x; --n; 			return inc?p4f1decx32(        in, n, out, x   ):p4fdecx32(       in, n, out, x);

    case TB_FOR:    vbxget32(in, x);*out++ = x; --n; b = *in++; return inc?bitf1unpack32(     in, n, out, x, b):bitfunpack32(    in, n, out, x, b);
    case TB_FORDA:  vbxget32(in, x);*out++ = x; --n; b = *in++; return inc?bitf1unpackx32(    in, n, out, x, b):bitfunpackx32(   in, n, out, x, b);      
    case TB_BP:     vbxget32(in, x);*out++ = x; --n; b = *in++; return inc?bitd1unpack32(     in, n, out, x, b):bitdunpack32(    in, n, out, x, b);
    case TB_BPN:               						  		    return in+(inc?bitnd1unpack32(in, n, out      ):bitndunpack32(   in, n, out));
	
	  #if C_TURBOPFORV
    case TB_FOR128V:vbxget32(in, x);*out++ = x; --n; b = *in++;
	  if(inc) {                                      	        return n==128?bitf1unpack128v32( in, n, out, x, b):bitf1unpack32(   in, n, out, x, b); } 
      else {                                              		return n==128?bitfunpack128v32(  in, n, out, x, b):bitfunpack32(in, n, out, x, b); }
    case TB_EF128V:	vbxget32(in, x);*out++ = x; --n;
	  if(inc) {                                           		return n==128?efano1dec128v32(in, n, out, x+1 ):efano1dec32(     in, n, out, x+1); }
      else {                                              		return n==128?efanodec128v32( in, n, out, x   ):efanodec32(      in, n, out, x); }
    case TB_PF128V:  vbxget32(in, x);*out++ = x; --n;  			//__builtin_prefetch(in+256);
      if(inc) {                                           		return n==128?p4d1dec128v32(  in, n, out, x   ):p4d1dec32(       in, n, out, x); }
      else {                                              		return n==128?p4ddec128v32(   in, n, out, x   ):p4ddec32(        in, n, out, x); }
    case TB_PFN128V:       							  		    return in+(inc?p4nd1dec128v32(in, n, out      ):p4nddec128v32(   in, n, out));
    case TB_BP128V: vbxget32(in, x);*out++ = x; --n; b = *in++;
	  if(inc) {                                      	    	return n==128?bitd1unpack128v32(in, n, out, x, b):bitd1unpack32( in, n, out, x, b); } 
      else {                                              		return n==128?bitdunpack128v32( in, n, out, x, b):bitdunpack32(  in, n, out, x, b); }
        #ifdef __AVX2__
    case TB_EF256V: vbxget32(in, x);*out++ = x; --n;
      if(inc) {                                           		return n==256?efano1dec256v32(in, n, out, x+1 ):efano1dec32(     in, n, out, x+1); }
      else {                                              		return n==256?efanodec256v32( in, n, out, x   ):efanodec32(      in, n, out, x); }
    case TB_PF256V:  vbxget32(in, x);*out++ = x; --n;  			//__builtin_prefetch(in+256);
      if(inc) {                                           		return n==256?p4d1dec256v32(  in, n, out, x   ):p4d1dec32(       in, n, out, x); }
      else {                                              		return n==256?p4ddec256v32(   in, n, out, x   ):p4ddec32(        in, n, out, x); }
    case TB_PFN256V:                                     		return in+(inc?p4nd1dec256v32(in, n, out      ):p4nddec256v32(   in, n, out));


    case TB_FOR256V:vbxget32(in, x);*out++ = x; --n; b = *in++;
	  if(inc) {                                      	        return n==256?bitf1unpack256v32( in, n, out, x, b):bitf1unpack32(   in, n, out, x, b); } 
      else {                                              		return n==256?bitfunpack256v32(  in, n, out, x, b):bitfunpack32(in, n, out, x, b); }
    case TB_BP256V: vbxget32(in, x);*out++ = x; --n; b = *in++;  
      if(inc) {                                      		    return n==256?bitd1unpack256v32(in, n, out, x, b):bitd1unpack32( in, n, out, x, b); } 
      else {                                              		return n==256?bitdunpack256v32( in, n, out, x, b):bitdunpack32(  in, n, out, x, b); }
    /*case TB_BP256V:vbxget32(in, x);*out = x; --n; b = *in++;
      if(n <= 256)                                       		return inc?bitd1unpack32(   in, n-1, out+1, x, b):bitdunpack32(  in, n-1, out+1, x, b);
      else { 													in = bitunpack256v32(     in,      out+1,    b);bitundn32(out, n, -inc, inc); } break;*/
	    #endif
	  #endif

	  #if C_TURBOPFORH
    case TB_BP128H: vbxget32(in, x);*out++ = x; --n; b = *in++; 
      if(inc) {                                      		    return n==128?bitd1unpack128h32(in, n, out, x, b):bitd1unpack32( in, n, out, x, b); } 
      else {                                              		return n==128?bitdunpack128h32( in, n, out, x, b):bitdunpack32(  in, n, out, x, b); }
	  #endif
	
	//-------- Externa libs
    case P_COPY:                                          		return u32dec( (unsigned *)in, n, out);
    case P_MCPY: 	  memcpy(_out, in, _n); 		  	  		return _out+_n;

      #if C_SIMPLE8B  
    case AM_SIMPLE8B: vbxget32(in, x);*out++ = x; --n; in = vs8bdec(in, n, out); bitundn32(out, n, x+inc, inc); break;
	  #endif

	  #if C_FASTPFOR
    case FP_VBYTE:    in = vbytedec(  in, n,   out);            bitundn32(out, n, -inc, inc); break;
	  #endif
	  
      #if C_LIBFOR
    case LF_FORX:     return for_selectx(in, n, out); //{ out[0] = *(unsigned *)in, b = in[4]; return for_selectx(in+5, n-1, out+1, out[0], b); }
    case LF_FOR:      return in + for_uncompress(in, out, n);
      #endif

      #if C_MASKEDVBYTE
    case P_MASKEDVBYTE: in += masked_vbyte_decode_delta(in, out,   n, 0); break;
      #endif

	  #if C_POLYCOM
    case PC_VBYTE:                              in = vbpolydec(          in, n,   out);          bitundn32(out, n, -inc, inc); break;
    case PC_SIMPLE16: vbxget32(in, x); *out = x; in = vs16dec((unsigned *)in, n-1, out+1);      bitundn32(out, n, -inc, inc); break;
    case PC_OPTPFD:   vbxget32(in, x); *out = x; in = optpfddec32(in,n-1,out+1); bitundn32(out+1, n-1, x, inc); break;
      /*if(n < 129)                          in = vbytedec(in, n, out);
      else {      _VBGET32(in, x, *out = x); unsigned all_array[2048]; in = (unsigned char *)detailed_p4_decode(out+1, (unsigned *)in, all_array); }
                                                                                            bitundn32(out, n, -inc, inc); break;*/
	  #endif
    //case P_QMX:  { vbxget32(in, x); *out = x; unsigned l = *(unsigned *)in; in = qmx_dec(in+4, l, out+1, n-1); bitundn32(out+1, n-1, x, inc); break; }   
      #if C_QMX    										//case P_QMX: return qmx_dec(in+4, ctou32(in), out, n); 
	case P_QMX:  { vbxget32(in, x); *out = x; unsigned l = *(unsigned *)in; ANT_compress_qmx  qmx;   qmx.decompress(out+1, n-1,  in+4, ctou32(in)); bitundn32(out+1, n-1, x, inc); return in+4+ctou32(in);} 
    case P_QMX2: { vbxget32(in, x); *out = x; unsigned l = *(unsigned *)in; ANT_compress_qmx_v2 qmx; qmx.decompress(out+1, n-1,  in+4, ctou32(in)); bitundn32(out+1, n-1, x, inc); return in+4+ctou32(in);}
    case P_QMX3: { vbxget32(in, x); *out = x; unsigned l = *(unsigned *)in; ANT_compress_qmx_v3 qmx; qmx.decompress(out+1, n-1,  in+4, ctou32(in)); bitundn32(out+1, n-1, x, inc); return in+4+ctou32(in);}
    case P_QMX4: { vbxget32(in, x); *out = x; unsigned l = *(unsigned *)in; ANT_compress_qmx_v4 qmx; qmx.decompress(out+1, n-1,  in+4, ctou32(in)); bitundn32(out+1, n-1, x, inc); return in+4+ctou32(in);}	 
      #endif
 	  
	  #if C_SIMDCOMP
    case SC_SIMDPACK128:
      if(n < 129) { in = vbytedec(in, n, out);               bitundn32(out, n, -inc, inc); }
      else {        vbxget32(in, x);*out = x; b = *in++; in = simdunpackd1n((uint32_t *)in, n-1, out+1, out[0], b); } break;
	  
    case SC_FOR:    vbxget32(in, x);*out = x; b = *in++; if(n < 129) return (unsigned char *)simdunpackFOR_length(x, (const __m128i *)in, n-1, (unsigned *)(out+1), b); else { simdunpackFOR(x, (const __m128i *)in, (unsigned *)(out+1), b); return in + simdpackFOR_compressedbytes(n-1, b); }
    case SC_FORDA: { vbxget32(in, x);*out++ = x; --n; b = *in++; unsigned i; for(i=0; i < n;i++) out[i] = simdselectFOR(x, (const __m128i *)in, b, i); return in + simdpackFOR_compressedbytes(n, b); }
	  #endif

	  #if C_STREAMVBYTE
    case P_STREAMVBYTE:   return in + streamvbyte_delta_decode(in, out, n, inc);
	  #endif  
	  
      #if C_VARINTG8IU
    case P_VARINTG8IU:                              in = vintg8dec(          in, n,   out);          bitundn32(out, n, -inc, inc); break;    
      #endif   
					//---------- transpose + lz77 ----------------------
      #if C_TRANSFORM
    case TB_ZIGZAG_32:memcpy(out, in, n*4); bitunzigzag32(out, n, 0); return in + n*4;
    case TB_TP8_32:   _untranspose4((unsigned char *)in, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); return in + n*4;
    case TB_TP8V_32:  UNTRANSPOSE(  (unsigned char *)in, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); return in + n*4;
    case TB_TP4V_32:  untransposen4((unsigned char *)in, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); return in + n*4;
    case TB_DELTA_32: memcpy(out, in, n*4); bitundn32(out, n, -inc, inc); return in + n*4;
      #endif
                     //---------- delta + transpose + lz77 ----------------------
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_S1: in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4);  BITUNSHUFFLE(sbuf, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); break;       
        #endif     
    case LZ4_  : in += LZ4_decompress_fast((char *)in, (char *)out, n*4); break;       
    case LZ4_S4: in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4); untransposen4(sbuf, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); break;       
    case LZ4_S8: in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4);   UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); break;       
      #endif     

      #if C_ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { uLongf outsize = n*4; int l = *(unsigned *)in, rc = uncompress(sbuf, &outsize, in+4, l); in += 4 + l; UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); } break;
      #endif

      #if C_C_BLOSC
    case P_BS_LZ:
    case P_BS_LZ4:
    case P_BS_ZLIB: { blosc_decompress(in, out, n*4); size_t nbytes, cbytes,blocksize; blosc_cbuffer_sizes(in, &nbytes, &cbytes, &blocksize); return in+cbytes; }
      #endif

      #if C_LZTURBO
    case P_LZT10: { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lz8d(&lz);  UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); } break;
    case P_LZT20:
    case P_LZT22: { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzbd(&lz);  UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); } break;
    //case P_VSHUF: vbxget32(in, x);*out = x; in += ansdecompress( in, (n-1)*4, sbuf); vsdec32( sbuf, n-1, out+1); bitundn32(out, n, -inc, inc); break;

    case P_VSHUF:  { unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, n*4); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); bitundn32(out, n, -inc, inc); } break;
    //case P_VSHUF:  { vbxget32(in, x);*out = x; unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, n*4); vsdec32(sbuf, n-1, out+1); bitundn32(out+1, n-1, x, inc); } break;
      #endif
  } 
  return in;
}

unsigned char *codcompz(unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int inc) { 
  unsigned *in = (unsigned *)_in, n = (_n+3) / 4, i, pa[BLK_SIZE+2048], x, b;

  switch(codec) {
    case TB_VBYTE:  x = *in++; --n; vbxput32(out, x);								return vbzenc32(  in, n, out, x);
    case TB_BP:     x = *in++; --n; vbxput32(out, x);b = bitz32(in, n, x); *out++=b;return bitzpack32(in, n, out, x, b);
    case TB_BPN:    x = *in++; --n; vbxput32(out, x);           					return out+bitnzpack32(in, n, out);
	  #if C_TURBOPFORV
    case TB_BP128V: x = *in++; --n; vbxput32(out, x);b = bitz32(in, n, x); *out++=b;return n == 128?bitzpack128v32(in, n, out, x, b):bitzpack32(in, n, out, x, b);
        #ifdef __AVX2__
    //case TB_BP256V: b = bitz32(in, n, x); *out++=b; return n == 256?bitzpack256v32(in, n, out, x, b ):bitzpack32(in, n, out,x, b);
        #endif 
      #endif
  }
  return out;
} 

unsigned char *coddecompz(unsigned char *in, unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int inc) { 
  unsigned *out = (unsigned *)_out, n = (outlen+3) / 4,x,b; 	
  
  
  switch(codec) { 
    case TB_VBYTE: vbxget32(in, x); *out++ = x; --n; 			return vbzdec32(in, n, out, x);
    case TB_BP:    vbxget32(in, x); *out++ = x; --n; b = *in++; return bitzunpack32(in, n, out, x, b);
    case TB_BPN:   vbxget32(in, x); *out++ = x; --n;            return in+bitnzunpack32(in, n, out);

	  #if C_TURBOPFORV
    case TB_BP128V:vbxget32(in, x); *out++ = x; --n; b = *in++; return n == 128?bitzunpack128v32(in, n, out, x, b):bitzunpack32(in, n, out, x, b);
        #ifdef __AVX2__
    //case TB_BP256V: b = *in++; return n == 256?bitzunpack256v32(in, n, out, x, b):bitzunpack32(in, n, out, x, b);
	    #endif
	  #endif
  } 
  return in;
}

unsigned char *codcomp(unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int b) {
  unsigned *in = (unsigned *)_in, n = (_n+3) / 4, x;
  
  switch(codec) {
    case TB_EF:    	 return out; 
    case TB_VBYTE: 	 return vbenc32(  in, n, out);
    case TB_VSIMPLE: return vsenc32(  in, n, out);
	
    case TB_PFDA:    return p4encx32( in, n, out);

    case TB_FOR :  
    case TB_FORDA:   b = bitfm32( in, n, &x); vbxput32(out, x); *out++=b; return bitfpack32( in, n, out, x, b); 

    case TB_BP128H:
    case TB_BP:      if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } 
	                 return bitpack32(in, n, out, b);
    case TB_BPN:     return out+bitnpack32(in, n, out);

	  #if C_TURBOPFORV
    case TB_EF128V:  return out; 

    case TB_PF128V:  return n == 128?p4enc128v32(in, n, out):p4enc32(in, n, out);
    case TB_PFN128V: return out+p4nenc128v32(in, n, out);

    case TB_FOR128V: b = bitfm32( in, n, &x); vbxput32(out, x); *out++=b; return bitfpack128v32( in, n, out, x, b); 

    case TB_BP128V:  if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } 
					 return n == 128?bitpack128v32(in, n, out, b):bitpack32(in, n, out, b);
        #ifdef __AVX2__
    case TB_PF256V:  return n == 256?p4enc256v32(in, n, out):p4enc32(in, n, out);
    case TB_PFN256V: return out+p4nenc256v32(in, n, out);

    case TB_FOR256V: b = bitfm32( in, n, &x); vbxput32(out, x); *out++=b; return bitfpack256v32( in, n, out, x, b);

    case TB_BP256V:  if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } 
	                 return n == 256?bitpack256v32(in, n, out, b):bitpack32(in, n, out, b);
        #endif
      #endif
	  
    //----- External libs ----------
    case P_COPY:     return u32enc(   in, n, (unsigned *)out);
    case P_MCPY: 	 memcpy(out, _in, _n); return out+_n;

	  #if C_SIMPLE8B  
    case AM_SIMPLE8B:return vs8benc(  in, n, out);
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
    case FP_VBYTE:    	{ size_t nvalue=outsize/4; FastPForLib::VariableByte ic; ic.encodeArray((const int32_t *)in, (const size_t)n, (uint32_t *)(out+4), nvalue); ctou32(out)=nvalue; return out+4+nvalue*4; }
    case FP_SIMPLE8BRLE:{ size_t nvalue=outsize/4; FastPForLib::Simple8b_RLE<true> ic; ic.encodeArray((const int32_t *)in, (const size_t)n, (uint32_t *)(out+4), nvalue); ctou32(out)=nvalue; return out+4+nvalue*4; }
	  #endif 
	  
      #if C_LIBFOR
    case LF_FORX: 		//if(b < 0) b = maxbits(in), *out++ = b; return out + for_compress_bits(in, out, n, 0, b);
    case LF_FOR:  		return out + for_compress_unsorted(in, out, n);
      #endif
	  
	  #if C_LITTLEPACK
    case LI_PACK:     	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } pack32(     in, n, b, out); return out + byte_count(n,b);
    case LI_HORPACK:   	
    case LI_TURBOPACK:	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } turbopack32(in, n, b, out); return out + byte_count(n,b);                                                                                                                                
    case LI_SCPACK:   	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } scpack32(   in, n, b, out); return out + byte_count(n,b);                                                                                                                                
        #ifdef __AVX2__
    case LI_BMIPACK:  	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } bmipack32(  in, n, b, out); return out + byte_count(n,b);                                                                                                                                
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
    case P_QMX:  		{ ANT_compress_qmx    qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)in, (uint64_t)n); ctou32(out)=r; return out+4+r; } // { unsigned char *q = qmx_enc(in, n, out+4); ctou32(out) = q - (out+4); return q;
    case P_QMX2: 		{ ANT_compress_qmx_v2 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)in, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX3: 		{ ANT_compress_qmx_v3 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)in, (uint64_t)n); ctou32(out)=r; return out+4+r; }
    case P_QMX4: 		{ ANT_compress_qmx_v4 qmx; unsigned r=qmx.compress(out+4, outsize, (uint32_t *)in, (uint64_t)n); ctou32(out)=r; return out+4+r; }	 
	  #endif
	  
      #if C_SIMDCOMP
    //case SC_PACK:    	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } return fastpackwithoutmask32(in, n, (uint32_t *)out, b);                                                                                                                              
    case SC_FOR:
    case SC_FORDA:
    case SC_SIMDPACK128:if(b < 0) b =    maxbits(in), *out++ = b; return (unsigned char *)simdpack_length(in, n, (__m128i *)out, b);
        #ifdef __AVX2__
    case SC_SIMDPACK256:if(b < 0) b = avxmaxbits(in), *out++ = b; return (unsigned char *)avxpackwithoutmaskn(in, n, (unsigned *)out, b);
        #endif
      #endif

	  #if C_STREAMVBYTE
    case P_STREAMVBYTE: return out + streamvbyte_encode(in, n, out); 
      #endif   
      #if C_VARINTG8IU
    case P_VARINTG8IU:  return vintg8enc(in, n, out);
      #endif 
	  
      // --------- transform ----------------------------------------
      #if C_TRANSFORM
    case TB_ZIGZAG_32:  bitzigzag32(in, n, (unsigned *)out, 0);      return out + n*4;
    case TB_TP8_32:     _transpose4( (unsigned char *)in, n*4, out); return out + n*4;
    case TB_TP8V_32:    TRANSPOSE( (unsigned char *)in, n*4, out);   return out + n*4;
    case TB_TP4V_32:    transposen4( (unsigned char *)in, n*4, out); return out + n*4;
      #endif
      #if C_C_BLOSC      
    case P_BS_SHUFFLE:  shuffle( 4, n*4, (unsigned char *)in, out); return out + n*4;
      #endif

      #if C_BITSHUFFLE      
    case P_BITSHUFFLE:  BITSHUFFLE( (unsigned char *)in, n*4, out);/* bshuf_bitshuffle(in, out, n, 4, 0);*/ return out + n*4;    
      #endif
 
      // --------- transpose + lz77 ----------------------------------------
      #if C_LZTURBO
    case P_LZT10: { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 0; lz.hbits = 16; return out + lz8c01(&lz); }
    case P_LZT20: { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 0; lz.hbits = 16; return out + lzbc01(&lz); }
    case P_LZT22: { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 2; lz.hbits = 26; return out + lzbc2(&lz); }     
    case P_LZT32: { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 2; lz.hbits = 26; return out + lzhc2(&lz); }
    //case P_VSHUF: { unsigned char *p = vsenc32(in, n, sbuf); p = mheenc(sbuf, p-sbuf, MH_BLK, out+2, out+n*4+256); ctou16(out) = p-(out+2); printf("L=%d ", p-(out+2));  return p; } //out + anscompress( sbuf, xb, out, out+n*4);
    //case P_VSHUF: { unsigned char                           *p = mheenc(in, n*4, MH_BLK, out+4, out+n*4+256); ctou32(out) = p-(out+4); return p; } //out + anscompress( sbuf, xb, out, out+n*4);
    case P_VSHUF: { unsigned char *p;/* = vsenc32(in, n, sbuf)*/ n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); p = mheenc(sbuf, n/*p-sbuf*/, MH_BLK, out+4, out+n+256); ctou32(out) = p-(out+4); return p; } //out + anscompress( sbuf, xb, out, out+n*4);
      #endif

	  #if C_BITSHUFFLE      
    case TB_VSIMPLE_S1:  BITSHUFFLE( (unsigned char *)in, n*4, sbuf); return vsenc32((unsigned *)sbuf, n, out);
	  #endif
    case TB_VSIMPLE_S4: transposen4( (unsigned char *)in, n*4, sbuf); return vsenc32((unsigned *)sbuf, n, out);
    case TB_VSIMPLE_S8:   TRANSPOSE( (unsigned char *)in, n*4, sbuf); return vsenc32((unsigned *)sbuf, n, out);
	  
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_S1:  BITSHUFFLE( (unsigned char *)in, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4);
	    #endif
    case LZ4_  : return out + LZ4_compress((char *)in, (char *)out, n*4);
    case LZ4_S4: transposen4( (unsigned char *)in, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4);
    case LZ4_S8: TRANSPOSE( (unsigned char *)in, n*4, sbuf); return out + LZ4_compress((char *)sbuf, (char *)out, n*4);
      #endif
	  
      #if C_C_BLOSC
    case P_BS_LZ:
    case P_BS_LZ4: 
    case P_BS_ZLIB:  return out + blosc_compress(lev, BLOSC_SHUFFLE, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
    //return out + blosc_compress(1/*clevel*/, BLOSC_BITSHUFFLE, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
      #endif
	  
      #if C_ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { n *= 4; TRANSPOSE( (unsigned char *)in, n, sbuf); uLongf outlen = n; int rc = compress2(out+4, &outlen, sbuf, n, codec-P_ZLIB1+1); if(rc != Z_OK) die("zlib compress2 rc=%d\n", rc); *(unsigned *)out = outlen; return out + 4 + outlen; }
      #endif  
    default : die("library '%d' not included\n", codec);
  }
  return out;
} 

unsigned char *coddecomp(unsigned char *in, unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int b) { 
  unsigned *out = (unsigned *)_out, n = (outlen+3) / 4,x; 	
  switch(codec) {         
    case TB_VBYTE:      				 		return vbdec32(  in, n, out); //    case P_VU: return vudec32(   in, n, out);
    case TB_VSIMPLE:    				 		return vsdec32(  in, n, out); 
    case TB_EF:    					     		return in;
    case TB_PFDA :  							return p4decx32( in, n, out);

    case TB_FOR:    vbxget32(in, x); b = *in++; return bitfunpack32( in, n, out, x, b);
    case TB_FORDA:  vbxget32(in, x); b = *in++; return bitfunpackx32(in, n, out, x, b);      

    case TB_BP:    	if(b < 0) b = *in++; 		return bitunpack32(         in, n, out, b);
    case TB_BPN: 	                     		return in+bitnunpack32(     in, n, out);
 	  #if C_TURBOPFORV
    case TB_PF128V : __builtin_prefetch(in+256);return n == 128?p4dec128v32(in, n, out):p4dec32(in, n, out);
    case TB_PFN128V :  					 		return in+p4ndec128v32(in, n, out);

    case TB_EF128V:   					 	 		return in;

    case TB_FOR128V: vbxget32(in, x); b = *in++;return bitfunpack128v32( in, n, out, x, b);

    case TB_BP128V:   if(b < 0) b = *in++; 		return n != 128?bitunpack32(in, n, out, b):bitunpack128v32(in, n, out, b);
        #ifdef __AVX2__
    case TB_FOR256V: vbxget32(in, x); b = *in++;return bitfunpack256v32( in, n, out, x, b);

    case TB_PF256V : __builtin_prefetch(in+256); return n == 256?p4dec256v32(in, n, out):p4dec32(in, n, out);
    case TB_PFN256V :      				 		return in+p4ndec256v32(in, n, out);
    case TB_BP256V: if(b < 0) b = *in++; 		return n != 256?bitunpack32(in, n, out, b):bitunpack256v32(in, n, out, b);
        #endif
      #endif
	  #if C_TURBOPFORH
    case TB_BP128H: if(b < 0) b = *in++; 		return n == 128?bitunpack128h32(in, n, out, b):bitunpack32(in, n, out, b);
      #endif
	  
    //---- external libs -------------
    case P_COPY:       					 		return u32dec(   (unsigned *)in, n, out);
    case P_MCPY:       					 		memcpy(_out, in, _n); return in+_n;	
	
      #if C_SIMPLE8B  
    case AM_SIMPLE8B:					 		return vs8bdec(  in, n, out); 
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
    case PC_VBYTE:    	 return vbpolydec(in, n, out);
    case PC_SIMPLE16: 	 return vs16dec(  (unsigned *)in, n, out);  
    case PC_RICE:     	 return rcdec32(  (unsigned *)in, n, out);
    case PC_OPTPFD : 	 return optpfddec32(   in, n, out); //if(n < 128) return vbytedec(in, n, out); else { unsigned all_array[2048]; return (unsigned char *)detailed_p4_decode(out, (unsigned *)in, all_array); }
     #endif

      #if C_SIMDCOMP 
	//case SC_PACK:          if(b < 0) b = *in++; return fastunpack32((uint32_t *)in, n, out, b); 
    case SC_FOR:
    case SC_SIMDPACK128: if(b < 0) b = *in++; return (unsigned char *)simdunpack_length( (__m128i *)in, n, out, b); 
    case SC_FORDA:  	 if(b < 0) b = *in++; return simdfor_selectx(in, n, out, 0, b);            
        #ifdef __AVX2__
    case SC_SIMDPACK256: if(b < 0) b = *in++; return (unsigned char *)avxunpackn( (unsigned *)in, n, out, b); 
        #endif
      #endif

	  #if C_STREAMVBYTE 
    case P_STREAMVBYTE:  return in +  streamvbyte_decode(in, out, n); 
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
      //---------- transform ----------------------
      #if C_TRANSFORM
    case TB_ZIGZAG_32:  memcpy(out, in, n*4); bitunzigzag32(out, n, 0); 				return in + n*4;
    case TB_TP8_32:    _untranspose4(  (unsigned char *)in, n*4, (unsigned char *)out); return in + n*4;
    case TB_TP8V_32:    UNTRANSPOSE( (unsigned char *)in, n*4, (unsigned char *)out); 	return in + n*4;
    case TB_TP4V_32:    untransposen4( (unsigned char *)in, n*4, (unsigned char *)out); return in + n*4;
	  #endif
      #if C_C_BLOSC
    case P_BS_SHUFFLE:  unshuffle( 4, n*4, (unsigned char *)in, (unsigned char *)out); 	return in + n*4;
      #endif		
      #if C_BITSHUFFLE      
    case P_BITSHUFFLE:  BITUNSHUFFLE(in, n*4, (unsigned char *)out); /*bshuf_bitunshuffle(in, out, n, 4, 0);*/ return in + n*4;
      #endif
 
      //---------- transpose + lz77 ----------------------
      #if C_LZTURBO
    case P_LZT10:  	{ struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lz8d(&lz); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); } break;
    case P_LZT20: 
    case P_LZT22:  	{ struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzbd(&lz); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); } break;
    case P_LZT32:  	{ struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzhd(&lz); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); } break;
    //case P_VSHUF: { unsigned inlen = ctou16(in); printf("l=%d ", inlen); in+=2; in = mhdec(in, inlen, MH_BLK, sbuf, n*4); vsdec32(sbuf, n, out); } break;//      in = ansdecompress( in, n*4, sbuf); 
    //case P_VSHUF: { unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, out, n*4); } break;//      in = ansdecompress( in, n*4, sbuf); 
    case P_VSHUF:  	{ unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, n*4); UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); /*vsdec32(sbuf, n, out);*/ } break;//      in = ansdecompress( in, n*4, sbuf); 
      #endif
	  
      #if C_BITSHUFFLE      
    case TB_VSIMPLE_S1:     	in = vsdec32(in, n, (unsigned *)sbuf);  BITUNSHUFFLE(sbuf, n*4, (unsigned char *)out); break;/*bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); */ 
      #endif
    case TB_VSIMPLE_S4:  in = vsdec32(in, n, (unsigned *)sbuf); untransposen4(sbuf, n*4, (unsigned char *)out); break;
    case TB_VSIMPLE_S8:  in = vsdec32(in, n, (unsigned *)sbuf);   UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); break;/*bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); */ 
      
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_S1:    in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4);  BITUNSHUFFLE(sbuf, n*4, (unsigned char *)out); break;/*bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); */ 
        #endif
    case LZ4_  : 	in += LZ4_decompress_fast((char *)in, (char *)out, n*4); break;       
    case LZ4_S4:    in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4); untransposen4(sbuf, n*4, (unsigned char *)out); break;
    case LZ4_S8:    in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4);   UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); break;/*bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); */ 
      #endif
	  
      #if C_C_BLOSC
    case P_BS_LZ:    
    case P_BS_LZ4:    
    case P_BS_ZLIB: { blosc_decompress(in, out, n*4); size_t nbytes, cbytes,blocksize; blosc_cbuffer_sizes(in, &nbytes, &cbytes, &blocksize); return in+cbytes; }
      #endif
	  
      #if C_ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { uLongf outsize = n*4; int l = *(unsigned *)in, rc = uncompress(sbuf, &outsize, in+4, l); in += 4 + l; UNTRANSPOSE(sbuf, n*4, (unsigned char *)out); } break;
      #endif
    default : 	  die("library '%d' not included\n", codec);
  }
  return in;        
}

char *codver(int codec, char *v, char *s) {
  switch(codec) {  
      #if C_C_BLOSC
    return BLOSC_VERSION_STRING;
      #endif 
	  #if C_LZ4
    case LZ4_:     sprintf(s,"%d.%d.%d", LZ4_VERSION_MAJOR, LZ4_VERSION_MINOR, LZ4_VERSION_RELEASE); return s;
	  #endif

	  #if C_ZSTD
    case P_ZSTD:    sprintf(s,"%d.%d.%d", ZSTD_VERSION_MAJOR, ZSTD_VERSION_MINOR, ZSTD_VERSION_RELEASE); return s;
      #endif

	default:        if(v) strcpy(s,v);
  }
  return s;
}
