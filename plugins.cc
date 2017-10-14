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
#include <sys/stat.h>
#include "conf.h"

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

  #ifdef NCODEC
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
  TB_PFDA,   						
  TB_FOR,    	
  TB_FORDA,  	
  TB_BP,		
  TB_BPN,		
  TB_BPDA,		
  TB_PDI,		
  TB_VBYTE,    
  TB_VSIMPLE,
  TB_EF, 	
  TB_FPPFOR64,
  TB_FPFFOR64,
  TB_FPDFOR64,
  TB_PF64,
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
  P_BS_SHUFFLE, 	
#define C_FASTPFOR		CODEC1
  FP_VBYTE,
  FP_FASTPFOR,
  FP_SIMDFASTPFOR,
  FP_SIMDOPTPFOR,
  FP_OPTPFOR,
  FP_SIMPLE8BRLE,
  FP_SIMDPACK,
  FP_GROUPSIMPLE,
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
  LZ4_BIT,
  LZ4_BITX,
  LZ4_NIBBLE,   
  LZ4_NIBBLEX,   
  LZ4_BYTE,
//  LZ4_FP8,
    #ifdef __SSSE3__
#define C_MASKEDVBYTE   CODEC1		
  P_MASKEDVBYTE,
    #else
#define C_MASKEDVBYTE  0
    #endif
#define C_POLYCOM  		CODEC1		
  PC_OPTPFD,             			// compression too slow and limited to 28 bits. crashs on some lists
  PC_VBYTE,
  PC_RICE,							// incl. only as demo, crash on some lists
  PC_SIMPLE16,						// limited to 28 bits.
#if defined(QMX)
#define C_QMX			CODEC1	
#else
#define C_QMX			0
#endif
  P_QMX,       			// crash on gov2
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
  
    #ifdef __SSSE3__
#define C_VARINTG8IU    CODEC1
  P_VARINTG8IU,
    #else
#define C_VARINTG8IU  0
    #endif  
  #ifdef ZLIB
#define C_ZLIB			CODEC2
  #else
#define C_ZLIB			0
  #endif
  P_ZLIB,
#define C_TRANSFORM     CODEC0
  TP_BYTES, 	
  TP_BYTE, 	
  TP_NIBBLE, 	
  TB_ZIGZAG32, 	
  TB_DELTA32, 	
  TB_XOR32, 	
#define C_LZTURBO       0
  P_LZT,
  P_VSHUF,
  
  P_MAX
};

#define PAD8(_x_) 		( (((_x_)+8-1)/8) )  
#if !defined(NCODEC1) && !defined(NCODEC2)
#include "ext/beplug_.h" // external libs
#endif

  #if C_TURBOPFOR  
#include "vint.h"       
#include "vsimple.h"
#include "bitpack.h"
#include "eliasfano.h"
#include "bitutil.h"
#include "transpose.h"
#include "vp4.h"
#include "fp.h"
  #endif

#define BLK_SIZE  (64*1024*4)
#define BLK_V128 (128*4)
#define BLK_V256 (256*4)

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
 
unsigned char *_bitunpackx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out           , unsigned b) { unsigned i,k=0; for(i=0; i < n; i++,k+=b ) *out++ = _bitgetx16(in, k, b); return in + PAD8(n*b); }
// direct access functions : included for demonstration only. Use the bulk functions instead, if you are decompressing most of the values
unsigned char *bitf1unpackx32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, int start, unsigned b) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, i, b)+start+i+1; return in + PAD8(n*b); }
unsigned char *bitfunpackx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, int start, unsigned b) { int i; for(i = 0; i < n; i++) out[i] = bitgetx16(in, i, b)+start;     return in + PAD8(n*b); }

//------------------------------------------------- plugins: codes+function -------------------------------------------------
struct codecs codecs[] = {
  { C_TURBOPFOR,	"TurboPFor",			"https://github.com/powturbo/TurboPFor"	}, 
  // External libs
  { C_C_BLOSC, 		"blosc",				"https://github.com/Blosc/c-blosc" }, 
  { C_FASTPFOR, 	"FastPFor",				"https://github.com/lemire/FastPFor" },
  { C_LZ4, 			"lz4",					"https://github.com/Cyan4973/lz4" }, 
  { C_LITTLEPACK, 	"LittleIntPacker",		"https://github.com/lemire/LittleIntPacker" },
  { C_LZTURBO, 		"LzTurbo",				"https://sites.google.com/site/powturbo" },
  { C_MASKEDVBYTE,	"MaskedVbyte",			"http://maskedvbyte.org" },
  { C_POLYCOM,		"Polycom",				"https://github.com/encode84/bcm" }, 
  { C_QMX, 		"QMX",					"https://bitbucket.org/andrewtrotman/bench" },
  { C_SIMDCOMP, 	"simdcomp",				"https://github.com/lemire/simdcomp" },
  { C_SIMPLE8B,		"Simple-8b optimized",	"https://github.com/powturbo/TurboPFor" },
  { C_STREAMVBYTE,	"Streamvbyte",			"https://github.com/lemire/streamvbyte" },
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
  { TB_PDI,			"TurboPForDI", 		C_TURBOPFOR,	BLK_V128,0,"","PFord min" }, 
  
  { TB_FOR,			"TurboFor", 		C_TURBOPFOR,	BLK_V128,0,"","FOR" }, 
  { TB_FOR128V,		"TurboForV", 		C_TURBOPFOR,	BLK_V128,0,"","FOR (SIMD)" }, 
  { TB_FOR256V,		"TurboFor256V", 	C_TURBOPFOR,	BLK_V256,0,"","FOR (AVX2)" }, 
  { TB_FORDA, 		"TurboForDA", 		C_TURBOPFOR,	BLK_V128,0,"","FOR direct access" }, 
  { TB_BPDA, 		"TurboPackDA", 		C_TURBOPFOR,	BLK_V128,0,"","Bit packing direct access" }, 

  { TB_BP,			"TurboPack", 		C_TURBOPFOR,	0,    	 0,"","Bit packing (scalar)" }, 
  { TB_BPN,			"TurboPackN", 		C_TURBOPFOR,	0,    	 0,"","Bit packing (scalar) large blocks" }, 
  { TB_BP128V, 		"TurboPackV", 		C_TURBOPFOR,	BLK_V128,0,"","Bit packing (SSE2 Vertical)" }, 
  { TB_BP128H, 		"TurboPackH", 		C_TURBOPFOR,	BLK_V128,0,"","Bit packing (SSE2 Horizontal)" }, 
  { TB_BP256V, 		"TurboPack256V",	C_TURBOPFOR,	BLK_V256,0,"","Bit packing (AVX2 Vertical)" }, 
  
  { TB_VBYTE, 		"TurboVByte", 		C_TURBOPFOR,	0,    	 0,"","Variable byte (scalar)" }, 
  { TB_VSIMPLE, 	"VSimple", 			C_TURBOPFOR,	0,    	 0,"","Variable simple (scalar)" }, 
  { TB_EF, 			"EliasFano",		C_TURBOPFOR,	0,       0,"","Elias fano (scalar)" }, 
  { TB_EF128V, 		"EliasFanoV",		C_TURBOPFOR,	BLK_V128,0,"","Eliasfano  (SSE2)" }, 
  { TB_EF256V,		"EliasFano256V",	C_TURBOPFOR,	BLK_V256,0,"","Elias fano (AVX2" }, 

  { P_MCPY, 		"memcpy", 			C_MEMCPY 				 },
  { P_COPY, 		"copy",		    	C_MEMCPY 				 },
  //----- Transform --------------
  { TP_BYTES, 		"tpbyte4s", 		C_TURBOPFOR,	BLK_SIZE,0,"","Byte Transpose (scalar)" }, 
  { TP_BYTE, 		"tpbyte",			C_TURBOPFOR,	BLK_SIZE,0,"2,4,8","Byte transpose (simd)" }, 
  { TP_NIBBLE, 		"tpnibble",			C_TURBOPFOR,	BLK_SIZE,0,"2,4,8","Nibble transpose (simd)" }, 
  { TB_ZIGZAG32, 	"ZigZag32", 		C_TURBOPFOR,	BLK_SIZE,0,"","ZigZag encoding (sse2)" }, 
  { TB_DELTA32, 	"Delta32", 		    C_TURBOPFOR,	BLK_SIZE,0,"","Delta encoding (sse2)"},  
  { TB_XOR32, 		"Xor32", 		    C_TURBOPFOR,	BLK_SIZE,0,"","Xor encoding (sse2)" },  
  // ---- Floating point --------
  { TB_FPPFOR64,	"FP_PREV64", 		C_TURBOPFOR,	BLK_SIZE,0,"","Floating point PFOR" }, 
  { TB_FPFFOR64,	"FP_FCM64", 		C_TURBOPFOR,	BLK_SIZE,0,"","Floating point PFOR (FCM)" }, 
  { TB_FPDFOR64,	"FP_DFCM64", 		C_TURBOPFOR,	BLK_SIZE,0,"","Floating point PFOR (DFCM)" }, 
  { TB_PF64,		"TurboPFor64", 		C_TURBOPFOR,	BLK_V128,0,"","PFOR 64" }, 
    #if !defined(NCODEC1) && !defined(NCODEC2)
  #include "ext/beplugr_.h" // external libs
    #endif
  { -1 }
};

struct plugg plugg[] = {
  { "TURBOPFOR","TurboPFor library",  			"TurboPFor256V/TurboPack256V/TurboPFor256N/TurboPFor/TurboPackV/TurboVByte/TurboPack/TurboForDA/EliasFano/VSimple/TurboPForN/TurboPackN/TurboPForDI" },
  { "DEFAULT",	"Default",  					"TurboPFor/TurboPackV/TurboVByte/TurboPack/TurboFor/TurboPForN/TurboPackN/TurboPForDI/TurboPFor256V/TurboPack256V/TurboPFor256N" },
  { "BENCH",	"Benchmark",  					"TurboPFor/TurboPackV/TurboVByte/TurboPack/QMX/FP.SimdFastPfor/FP.SimdOptPFor/MaskedVbyte/StreamVbyte" },
  { "EFFICIENT","Efficient",					"TurboPFor/vsimple/turbovbyte" },
  { "TRANSFORM","transpose/shufle,delta,zigzag","tpbyte4s/tpbyte,4/tpnibble,4/ZigZag_32/Delta_32/BitShuffle,4" },

  { "BITPACK",	"Bit Packing",  				"TurboPack256V/TurboPackV/TurboPackH/TurboPack/SC.SimdPack128/SC.SimdPack256" },
  { "VBYTE",	"Variable byte",				"TurboVByte/FP.VByte/PC.Vbyte/VarintG8IU/MaskedVbyte/StreamVbyte" },
  { "SIMPLE",	"Simple Family",				"simple8b/simple16/vsimple/qmx" },
  
  { "LZ4",		"lz4+bitshufle/transpose 4,8",	"lz4_bitshufle/lz4_tp4/lz4_tp8" }, 
  { "LI",		"Little Integer",				"LI_Pack/LI_TurboPack/LI_SuperPack/LI_HorPack" },
  { "" }
};

#define PAD8(__x) (((__x)+7)/8)
//---------------------------------------------- plugins --------------------------------------------------------
#include "conf.h"  
unsigned char sbuf[BLK_SIZE*2+1024];

int codini(size_t insize, int codec) {
  switch(codec) {
      #if !defined(NCODEC1) && !defined(NCODEC2)
    #include "ext/beplugi_.c"
      #endif
  }
  return 0;
}   
 
void codexit(int codec) {} 

//--- Sorted integer array : Delta/Differential compression (mdelta=0 increasing, mdelta=1 strictly increasing sequence) ---------------
unsigned char *codcomps(unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int mdelta) { 
  unsigned *in = (unsigned *)_in, n = (_n+3) / 4, i, *pa=(unsigned *)sbuf, x, b;
  
  switch(codec) {	  
    case TB_VSIMPLE: x = *in++; bitdienc32( in, --n, pa, x, mdelta);
	                                 vbxput32(out, x);			return vsenc32(         pa, n, out);
    case TB_VBYTE:   x = *in++; --n; vbxput32(out, x);			return mdelta?vbd1enc32(   in, n, out, x   ):vbdenc32(     in, n, out, x);
    case TB_EF:      x = *in++; --n; vbxput32(out, x); 			return mdelta?efano1enc32( in, n, out, x+1 ):efanoenc32(   in, n, out, x);
	
    case TB_PFDA:    x = *in++; --n; vbxput32(out, x); DELTR(in,n,x,mdelta,pa); 
																return p4encx32(pa,n,out);
    case TB_FOR :  
    case TB_FORDA:   x = *in++; --n; vbxput32(out, x); 
      if(mdelta) { b = bitf132(in, n, x);  *out++=b;			return bitf1pack32(in, n, out, x, b); }
      else {    b = bitf32( in, n, x);  *out++=b;				return bitfpack32( in, n, out, x, b); }

    case TB_BP128H:
    case TB_BP:     x = *in++; --n; vbxput32(out, x); 
      if(mdelta) { b = bitd132(in, n, x);  *out++=b;			return bitd1pack32(in, n, out, x, b); }
      else {    b = bitd32( in, n, x);  *out++=b;				return bitdpack32( in, n, out, x, b); } 
    case TB_BPN:    											return out+(mdelta?bitnd1pack32(in, n, out):bitndpack32( in, n, out));
	
    case TB_PDI: { unsigned mdelta; x = *in++; mdelta = bitdi32(in, --n, x); if(mdelta>(1<<27)) mdelta=1<<27; bitdienc32(in, n, pa, x, mdelta); 
	    mdelta=mdelta<<5|(x&31); x>>=5; vbxput32(out, x); vbput32(out, mdelta); return p4enc32(pa, n, out);
	  }

	  #if C_TURBOPFORV
    case TB_EF128V:   x = *in++; --n; vbxput32(out, x);         
      if(mdelta)  													return n == 128?efano1enc128v32(in, n, out, x+1 ):efano1enc32(  in, n, out, x+1);
      else     													return n == 128?efanoenc128v32( in, n, out, x   ):efanoenc32(   in, n, out, x  );
    case TB_PF128V:   x = *in++; --n; vbxput32(out, x);
      if(mdelta)                     								return n == 128?p4d1enc128v32(  in, n, out, x   ):p4d1enc32(    in, n, out, x); 
      else                                               		return n == 128?p4denc128v32(   in, n, out, x   ):p4denc32(     in, n, out, x); 
    case TB_PFN128V:    										return out+(mdelta?p4nd1enc128v32( in, n, out      ):p4ndenc128v32(in, n, out));
	
    case TB_FOR128V: x = *in++; --n; vbxput32(out, x); 
      if(mdelta) { b = bitf132(in, n, x);  *out++=b; 				return n == 128?bitf1pack128v32(in, n, out, x, b):bitf1pack32(  in, n, out, x, b); }
      else {    b = bitf32( in, n, x);  *out++=b;				return n == 128?bitfpack128v32( in, n, out, x, b):bitfpack32(   in, n, out, x, b); }
    case TB_BP128V:  x = *in++; --n; vbxput32(out, x); 
      if(mdelta) { b = bitd132(in, n, x);  *out++=b;				return n == 128?bitd1pack128v32(in, n, out, x, b):bitd1pack32(  in, n, out, x, b); }
      else {    b = bitd32( in, n, x); 	*out++=b;				return n == 128?bitdpack128v32( in, n, out, x, b):bitdpack32(   in, n, out, x, b); }
      #if defined(__AVX2__) && defined(AVX2_ON)
    case TB_FOR256V: x = *in++; --n; vbxput32(out, x); 
      if(mdelta) { b = bitf132(in, n, x);  *out++=b; 				return n == 256?bitf1pack256v32(in, n, out, x, b):bitf1pack32(  in, n, out, x, b); }
      else {    b = bitf32( in, n, x);  *out++=b;				return n == 256?bitfpack256v32( in, n, out, x, b):bitfpack32(   in, n, out, x, b); }
    case TB_EF256V: x = *in++; --n;  vbxput32(out, x);         
      if(mdelta)  													return n == 256?efano1enc256v32(in, n, out, x+1 ):efano1enc32(  in, n, out, x+1);
      else     													return n == 256?efanoenc256v32( in, n, out, x   ):efanoenc32(   in, n, out, x  );
    case TB_PF256V: x = *in++; bitdienc32( in, --n, pa, x, mdelta);
	                                 vbxput32(out, x);       	return n == 256?p4enc256v32(    pa, n, out      ):p4enc32(      pa, n, out);
    case TB_PFN256V:         									return out+(mdelta?p4nd1enc256v32( in, n, out      ):p4ndenc256v32(in, n, out));
    case TB_BP256V: x = *in++; --n; vbxput32(out, x); 
      if(mdelta) { b = bitd132(in, n, x); *out++=b;                return n == 256?bitd1pack256v32(in, n, out, x, b):bitd1pack32(  in, n, out, x, b); }
      else {    b = bitd32( in, n, x); *out++=b;                return n == 256?bitdpack256v32( in, n, out, x, b):bitdpack32(   in, n, out, x, b); }
        #endif 
      #endif
      // --------- transform ------------------------------------------------------------------------------------------------
      #if C_TRANSFORM
    case TB_ZIGZAG32: bitzenc32( in, n, (unsigned *)out, 0, 0); return out + _n;
    case TP_BYTES:    bitdienc32(in, n, (unsigned *)sbuf, -mdelta, mdelta); tpenc4((unsigned char *)sbuf, _n, out);      return out + _n; 
    case TP_BYTE:     bitdienc32(in, n, (unsigned *)sbuf, -mdelta, mdelta); tpenc( (unsigned char *)sbuf, _n, out, lev); return out + _n;
    case TP_NIBBLE:   bitdienc32(in, n, (unsigned *)sbuf, -mdelta, mdelta); tp4enc((unsigned char *)sbuf, _n, out, lev); return out + _n;
    case TB_DELTA32:  bitdienc32(in, n, (unsigned *)out,  -mdelta, mdelta); return out + _n; 
    case TB_XOR32:    bitxenc32( in, n, (unsigned *)out,  -mdelta);         return out + _n; 
      #endif
      #if !defined(NCODEC1) && !defined(NCODEC2)
    #include "ext/beplugcs_.c"
      #endif
  }
  return out;
} 

unsigned char *coddecomps(unsigned char *in, unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int mdelta) { 
  unsigned *out = (unsigned *)_out, n = (outlen+3) / 4,x,b; 	
  
  switch(codec) { 
    case TB_VSIMPLE:vbxget32(in, x);*out++ = x; --n; 			in = vsdec32(                 in, n, out); bitdidec32(out, n, x+mdelta, mdelta); return in;
    case TB_EF:	    vbxget32(in, x);*out++ = x; --n;     		return mdelta?efano1dec32(       in, n, out, x+1 ):efanodec32(      in, n, out, x);
    case TB_VBYTE: 	vbxget32(in, x);*out++ = x; --n; 	   		return mdelta?vbd1dec32(         in, n, out, x   ):vbddec32(        in, n, out, x);

    case TB_PFDA:   vbxget32(in, x);*out++ = x; --n; 			return mdelta?p4f1decx32(        in, n, out, x   ):p4fdecx32(       in, n, out, x);

    case TB_FOR:    vbxget32(in, x);*out++ = x; --n; b = *in++; return mdelta?bitf1unpack32(     in, n, out, x, b):bitfunpack32(    in, n, out, x, b);
    case TB_FORDA:  vbxget32(in, x);*out++ = x; --n; b = *in++; return mdelta?bitf1unpackx32(    in, n, out, x, b):bitfunpackx32(   in, n, out, x, b);      
    case TB_BP:     vbxget32(in, x);*out++ = x; --n; b = *in++; return mdelta?bitd1unpack32(     in, n, out, x, b):bitdunpack32(    in, n, out, x, b);
    case TB_BPN:               						  		    return in+(mdelta?bitnd1unpack32(in, n, out      ):bitndunpack32(   in, n, out));
	
    case TB_PDI: { uint32_t mdelta; vbxget32(in, x);  vbget32(in, mdelta); x = x << 5 | (mdelta & 31); *out++ = x; --n; in = p4dec32(in, n, out); bitdidec32(out, n, x, mdelta>>5); break; }
	  #if C_TURBOPFORV
    case TB_FOR128V:vbxget32(in, x);*out++ = x; --n; b = *in++;
	  if(mdelta) {                                      	        return n==128?bitf1unpack128v32( in, n, out, x, b):bitf1unpack32(   in, n, out, x, b); } 
      else {                                              		return n==128?bitfunpack128v32(  in, n, out, x, b):bitfunpack32(in, n, out, x, b); }
    case TB_EF128V:	vbxget32(in, x);*out++ = x; --n;
	  if(mdelta) {                                           		return n==128?efano1dec128v32(in, n, out, x+1 ):efano1dec32(     in, n, out, x+1); }
      else {                                              		return n==128?efanodec128v32( in, n, out, x   ):efanodec32(      in, n, out, x); }
    case TB_PF128V:  vbxget32(in, x);*out++ = x; --n;  			//__builtin_prefetch(in+256);
      if(mdelta) {                                           		return n==128?p4d1dec128v32(  in, n, out, x   ):p4d1dec32(       in, n, out, x); }
      else {                                              		return n==128?p4ddec128v32(   in, n, out, x   ):p4ddec32(        in, n, out, x); }
    case TB_PFN128V:       							  		    return in+(mdelta?p4nd1dec128v32(in, n, out      ):p4nddec128v32(   in, n, out));
    case TB_BP128V: vbxget32(in, x);*out++ = x; --n; b = *in++;
	  if(mdelta) {                                      	    	return n==128?bitd1unpack128v32(in, n, out, x, b):bitd1unpack32( in, n, out, x, b); } 
      else {                                              		return n==128?bitdunpack128v32( in, n, out, x, b):bitdunpack32(  in, n, out, x, b); }
      #if defined(__AVX2__) && defined(AVX2_ON)
    case TB_EF256V: vbxget32(in, x);*out++ = x; --n;
      if(mdelta) {                                           		return n==256?efano1dec256v32(in, n, out, x+1 ):efano1dec32(     in, n, out, x+1); }
      else {                                              		return n==256?efanodec256v32( in, n, out, x   ):efanodec32(      in, n, out, x); }
    case TB_PF256V:  vbxget32(in, x);*out++ = x; --n;  			//__builtin_prefetch(in+256);
      if(mdelta) {                                           		return n==256?p4d1dec256v32(  in, n, out, x   ):p4d1dec32(       in, n, out, x); }
      else {                                              		return n==256?p4ddec256v32(   in, n, out, x   ):p4ddec32(        in, n, out, x); }
    case TB_PFN256V:                                     		return in+(mdelta?p4nd1dec256v32(in, n, out      ):p4nddec256v32(   in, n, out));


    case TB_FOR256V:vbxget32(in, x);*out++ = x; --n; b = *in++;
	  if(mdelta) {                                      	        return n==256?bitf1unpack256v32( in, n, out, x, b):bitf1unpack32(   in, n, out, x, b); } 
      else {                                              		return n==256?bitfunpack256v32(  in, n, out, x, b):bitfunpack32(in, n, out, x, b); }
    case TB_BP256V: vbxget32(in, x);*out++ = x; --n; b = *in++;  
      if(mdelta) {                                      		    return n==256?bitd1unpack256v32(in, n, out, x, b):bitd1unpack32( in, n, out, x, b); } 
      else {                                              		return n==256?bitdunpack256v32( in, n, out, x, b):bitdunpack32(  in, n, out, x, b); }
	    #endif
	  #endif

	  #if C_TURBOPFORH
    case TB_BP128H: vbxget32(in, x);*out++ = x; --n; b = *in++; 
      if(mdelta) {                                      		    return n==128?bitd1unpack128h32(in, n, out, x, b):bitd1unpack32( in, n, out, x, b); } 
      else {                                              		return n==128?bitdunpack128h32( in, n, out, x, b):bitdunpack32(  in, n, out, x, b); }
	  #endif	
					//---------- transpose + lz77 ----------------------
      #if C_TRANSFORM
    case TB_ZIGZAG32: memcpy(out, in, outlen); bitzdec32(out, n, 0); return in + outlen;
    case TP_BYTES:    tpdec4((unsigned char *)in, outlen, (unsigned char *)out);     bitdidec32(out, n, -mdelta, mdelta); return in + outlen;
    case TP_BYTE:     tpdec( (unsigned char *)in, outlen, (unsigned char *)out,lev); bitdidec32(out, n, -mdelta, mdelta); return in + outlen;
    case TP_NIBBLE:   tp4dec((unsigned char *)in, outlen, (unsigned char *)out,lev); bitdidec32(out, n, -mdelta, mdelta); return in + outlen;
    case TB_DELTA32:  memcpy(out, in, outlen); mdelta?bitd1dec32(out, n, -mdelta):bitddec32(out, n, 0); return in + outlen;
    case TB_XOR32:    memcpy(out, in, outlen); bitxdec32(out, n, -mdelta); return in + outlen;
      #endif
      #if !defined(NCODEC1) && !defined(NCODEC2)
    #include "ext/beplugds_.c" 
      #endif
  } 
  return in;
}

//---- ZigZag----
unsigned char *codcompz(unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int mdelta) { 
  unsigned *in = (unsigned *)_in, n = (_n+3) / 4, i, *pa = (unsigned *)sbuf, x, b;

  switch(codec) {
    case TB_VBYTE:  x = *in++; --n; vbxput32(out, x);								return vbzenc32(  in, n, out, x);
    case TB_BP:     x = *in++; --n; vbxput32(out, x);b = bitz32(in, n, x); *out++=b;return bitzpack32(in, n, out, x, b);
    case TB_BPN:    x = *in++; --n; vbxput32(out, x);           					return out+bitnzpack32(in, n, out);
	  #if C_TURBOPFORV
    case TB_PF128V: x = *in++; --n; vbxput32(out, x);                               return n == 128?p4zenc128v32(in, n, out, x):p4zenc32(in, n, out, x);
    case TB_PFN128V:    										                    return out+p4nzenc128v32( in, n, out );
    case TB_BP128V: x = *in++; --n; vbxput32(out, x);b = bitz32(in, n, x); *out++=b;return n == 128?bitzpack128v32(in, n, out, x, b):bitzpack32(in, n, out, x, b);
      #if defined(__AVX2__) && defined(AVX2_ON)
    case TB_BP256V: x = *in++; --n; vbxput32(out, x);b = bitz32(in, n, x); *out++=b;return n == 256?bitzpack256v32(in, n, out, x, b):bitzpack32(in, n, out,x, b);
        #endif 
      #endif
  }
  return out;
} 

unsigned char *coddecompz(unsigned char *in, unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int mdelta) { 
  unsigned *out = (unsigned *)_out, n = (outlen+3) / 4,x,b; 	
  
  switch(codec) { 
    case TB_VBYTE: vbxget32(in, x); *out++ = x; --n; 			return vbzdec32(in, n, out, x);
    case TB_BP:    vbxget32(in, x); *out++ = x; --n; b = *in++; return bitzunpack32(in, n, out, x, b);
    case TB_BPN:   vbxget32(in, x); *out++ = x; --n;            return in+bitnzunpack32(in, n, out);

	  #if C_TURBOPFORV
    case TB_PFN128V:       							  		    return in+p4nzdec128v32(in, n, out);
    case TB_PF128V:vbxget32(in, x); *out++ = x; --n;            return n == 128?p4zdec128v32(in, n, out, x):p4zdec32(in, n, out, x);
    case TB_BP128V:vbxget32(in, x); *out++ = x; --n; b = *in++; return n == 128?bitzunpack128v32(in, n, out, x, b):bitzunpack32(in, n, out, x, b);
      #if defined(__AVX2__) && defined(AVX2_ON)
    case TB_BP256V:vbxget32(in, x); *out++ = x; --n; b = *in++; return n == 256?bitzunpack256v32(in, n, out, x, b):bitzunpack32(in, n, out, x, b);
	    #endif
	  #endif
  } 
  return in;
}

unsigned char *codcomp(unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int b) {
  unsigned *in = (unsigned *)_in, n = (_n+3) / 4, x;
  
  switch(codec) {
    case TB_EF:    	 return out; // Not applicable
    case TB_VBYTE: 	 return vbenc32(  in, n, out);
    case TB_VSIMPLE: return vsenc32(  in, n, out);
	
    case TB_PFDA:    return p4encx32( in, n, out);    //case TB_PFM:     bitfm32( in, n, &x); vbxput32(out, x); return p4denc32(in, n, out, x); 
    case TB_PDI:     { unsigned *pa = (unsigned *)sbuf,mdelta; 
	    bitfm32(in, n, &x); 										// determine minimum value 
		mdelta = bitdi32(in, n, x); if(mdelta>(1<<27)) mdelta=1<<27;// determine minimum delta 
		bitdienc32(in, n, pa, x, mdelta); 							// delta transform 
	    mdelta=mdelta<<5|(x&31); x>>=5; vbxput32(out, x); vbput32(out, mdelta); 
		             return p4enc32(pa, n, out);
	  }
    case TB_FOR :  
    case TB_FORDA:   b = bitfm32( in, n, &x); vbxput32(out, x); *out++=b; return bitfpack32( in, n, out, x, b); 

    case TB_BPDA:
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
      #if defined(__AVX2__) && defined(AVX2_ON)
    case TB_PF256V:  return n == 256?p4enc256v32(in, n, out):p4enc32(in, n, out);
    case TB_PFN256V: return out+p4nenc256v32(in, n, out);

    case TB_FOR256V: b = bitfm32( in, n, &x); vbxput32(out, x); *out++=b; return bitfpack256v32( in, n, out, x, b);

    case TB_BP256V:  if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } 
	                 return n == 256?bitpack256v32(in, n, out, b):bitpack32(in, n, out, b);
        #endif
      #endif

    case P_COPY:     return u32enc(   in, n, (unsigned *)out);
    case P_MCPY: 	 memcpy(out, _in, _n); return out+_n;
      // --------- transform ----------------------------------------
      #if C_TRANSFORM
    case TP_BYTES:    tpenc4((unsigned char *)in, _n,           out);     return out + _n;
    case TP_BYTE:     tpenc( (unsigned char *)in, _n,           out,lev); return out + _n;
    case TP_NIBBLE:   tp4enc((unsigned char *)in, _n,           out,lev); return out + _n;
    case TB_DELTA32:  bitdienc32(             in,n, (unsigned *)out,0,0); return out + _n; 
    case TB_ZIGZAG32: bitzenc32(              in, n,(unsigned *)out,0,0); return out + _n;
      #endif
      //---- Floating point ----------------------
	case TB_FPPFOR64: ctou64(out) = ctou64(_in); return fppenc64(   (uint64_t *)(_in+8), PAD8(_n)-1, out+8, ctou64(_in)); 
	case TB_FPFFOR64: ctou64(out) = ctou64(_in); return fpfcmenc64( (uint64_t *)(_in+8), PAD8(_n)-1, out+8, ctou64(_in));
	case TB_FPDFOR64: ctou64(out) = ctou64(_in); return fpdfcmenc64((uint64_t *)(_in+8), PAD8(_n)-1, out+8, ctou64(_in));
    case TB_PF64:     return p4enc64((uint64_t *)in,   PAD8(_n), out);
      #if !defined(NCODEC1) && !defined(NCODEC2)
    #include "ext/beplugc_.c"	  
      #endif
  }
  return out;
} 

unsigned char *coddecomp(unsigned char *in, unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int b) {   
  unsigned *out = (unsigned *)_out, n = (outlen+3) / 4,x; 	
  switch(codec) {         
    case TB_VBYTE:      				 		return vbdec32(  in, n, out); //    case P_VU: return vudec32(   in, n, out);
    case TB_VSIMPLE:    				 		return vsdec32(  in, n, out); 
    case TB_EF:    					     		return in; // Not applicable
    case TB_PFDA :  							return p4decx32( in, n, out);  //case TB_PFM:    vbxget32(in, x); 			return p4ddec32( in, n, out, x); 
    case TB_PDI: { uint32_t mdelta;  vbxget32(in, x); vbget32(in, mdelta); x = x << 5 | (mdelta & 31); in = p4dec32(in, n, out); bitdidec32(out, n, x, mdelta>>5); break; }

    case TB_FOR:    vbxget32(in, x); b = *in++; return bitfunpack32( in, n, out, x, b);
    case TB_FORDA:  vbxget32(in, x); b = *in++; return bitfunpackx32(in, n, out, x, b);      
    
    case TB_BPDA:  	if(b < 0) b = *in++; 		return _bitunpackx32(       in, n, out, b);
    case TB_BP:    	if(b < 0) b = *in++; 		return bitunpack32(         in, n, out, b);
    case TB_BPN: 	                     		return in+bitnunpack32(     in, n, out);
 	  #if C_TURBOPFORV
    case TB_PF128V : 							return n == 128?p4dec128v32(in, n, out):p4dec32(in, n, out);
    case TB_PFN128V :  					 		return in+p4ndec128v32(in, n, out);

    case TB_EF128V:   					 	 	return in;

    case TB_FOR128V: vbxget32(in, x); b = *in++;return bitfunpack128v32( in, n, out, x, b);

    case TB_BP128V:   if(b < 0) b = *in++; 		return n != 128?bitunpack32(in, n, out, b):bitunpack128v32(in, n, out, b);
      #if defined(__AVX2__) && defined(AVX2_ON)
    case TB_FOR256V: vbxget32(in, x); b = *in++;return bitfunpack256v32( in, n, out, x, b);

    case TB_PF256V : __builtin_prefetch(in+256);return n == 256?p4dec256v32(in, n, out):p4dec32(in, n, out);
    case TB_PFN256V :      				 		return in+p4ndec256v32(in, n, out);
    case TB_BP256V: if(b < 0) b = *in++; 		return n != 256?bitunpack32(in, n, out, b):bitunpack256v32(in, n, out, b);
        #endif
      #endif
	  #if C_TURBOPFORH
    case TB_BP128H: if(b < 0) b = *in++; 		return n == 128?bitunpack128h32(in, n, out, b):bitunpack32(in, n, out, b);
      #endif
	  
      //---------- transform ----------------------
      #if C_TRANSFORM
    case TP_BYTES:    tpdec4(  (unsigned char *)in, outlen, (unsigned char *)out);     return in + outlen;
    case TP_BYTE:     tpdec(   (unsigned char *)in, outlen, (unsigned char *)out,lev); return in + outlen;
    case TP_NIBBLE:   tp4dec(  (unsigned char *)in, outlen, (unsigned char *)out,lev); return in + outlen;
    case TB_DELTA32:  memcpy(out, in, outlen); bitddec32(out, n, 0); 			   	   return in + outlen;
    case TB_ZIGZAG32: memcpy(out, in, outlen); bitzdec32(out, n, 0); 			  	   return in + outlen;
	  #endif
      //---- Floating point (64 bits)----------------------
	case TB_FPPFOR64: ctou64(_out) = ctou64(in); return fppdec64(   in+8, PAD8(outlen)-1, (uint64_t *)(_out+8), ctou64(in)); 
	case TB_FPFFOR64: ctou64(_out) = ctou64(in); return fpfcmdec64( in+8, PAD8(outlen)-1, (uint64_t *)(_out+8), ctou64(in));; 
	case TB_FPDFOR64: ctou64(_out) = ctou64(in); return fpdfcmdec64(in+8, PAD8(outlen)-1, (uint64_t *)(_out+8), ctou64(in));
    case TB_PF64:     return p4dec64(    in, PAD8(outlen), (uint64_t *)out);
      #if !defined(NCODEC1) && !defined(NCODEC2)
    #include "ext/beplugd_.c"	  
      #endif
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
