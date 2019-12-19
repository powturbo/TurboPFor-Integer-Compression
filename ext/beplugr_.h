 // { TB_VSIMPLE_S1,	"vs_bitshuffle",	C_LZ4,  		BLK_SIZE }, 
 // { TB_VSIMPLE_S4,	"vs_s4",			C_LZ4,			BLK_SIZE }, 
 // { TB_VSIMPLE_S8,	"vs_s8",			C_LZ4, 			BLK_SIZE }, 
  
  { AM_SIMPLE8B, 	"Simple8b", 		C_SIMPLE8B,		1024,    0,"","64 bits Simple family (instable)" },

  { PC_SIMPLE16, 	"PC_Simple16", 		C_POLYCOM,		BLK_V128,0,"","Simple 16. limited to 28 bits" }, 
  { PC_OPTPFD,     	"PC_OptPFD",		C_POLYCOM, 		BLK_V128,0,"","OptPFD. limited to 28 bits" },
  { PC_VBYTE,  		"PC_Vbyte",			C_POLYCOM,		0,    	 0,"","Variable byte" }, 
  { PC_RICE,  		"PC_Rice",			C_POLYCOM, 		BLK_V128,0,"","Rice coding (instable)" }, 

  { P_VARINTG8IU,	"VarintG8IU", 		C_VARINTG8IU,	0,    	 0,"","Variable byte SIMD" },
  { P_MASKEDVBYTE,	"MaskedVbyte", 		C_MASKEDVBYTE,	0,    	 0,"","Variable byte SIMD" },
  { P_STREAMVBYTE,	"StreamVbyte", 		C_STREAMVBYTE,	0,    	 0,"","Variable byte SIMD" },

  { FP_FASTPFOR,	"FP_FastPFor", 		C_FASTPFOR128,	    0,    	 0,"","PFor scalar (inefficient for small blocks)" },
  { FP_SIMDFASTPFOR,"FP_SimdFastPFor", 	C_FASTPFOR128,	    0,    	 0,"","PFor SIMD (inefficient for small blocks)" },
  { FP_OPTPFOR,		"FP_OptPFor",  		C_FASTPFOR128,	    0,    	 0,"","OptPFor scalar" },
  { FP_SIMDOPTPFOR,	"FP_SIMDOptPFor",  	C_FASTPFOR128,	    0,    	 0,"","OptPFor SIMD" },
  { FP_VBYTE,		"FP_VByte", 		C_FASTPFOR128,	    0,    	 0,"","Variable byte" },
  { FP_SIMPLE8BRLE,	"FP_Simple8bRLE", 	C_FASTPFOR128,	    0,    	 0,"","Simple-8b + rle" },
  { FP_GROUPSIMPLE,	"FP_GROUPSIMPLE", 	C_FASTPFOR128,	    0,    	 0,"","Group Simple" },

  { SC_SIMDPACK128,	"SC_SIMDPack128", 	C_SIMDCOMP128,	BLK_V128,0,"","Bit packing (SSE4.1)"},
  { SC_SIMDPACK256,	"SC_SIMDPack256", 	C_SIMDCOMP256,	BLK_V256,0,"","Bit packing (SSE4.1)"},
  { SC_FOR,		   	"SC_For", 	        C_SIMDCOMP128,	BLK_V128,0,"","For (SSE4.1)"},
  { SC_FORDA,	 	"SC_ForDA", 		C_SIMDCOMP128,	BLK_V128,0,"","For direct access (SSE4.1)"},

//{ CL_FASTPFORD1,	"CL.SIMDPFORD1", 	C_SIMDCOMP128, 	"", 0,BLK_V128},
  { P_BS_LZ,  		"blosc_lz",			C_C_BLOSC, 		BLK_SIZE,0, "","TurboPFor's [delta]+transpose+lz4" }, 
  { P_BS_LZ4,  		"blosc_lz4",		C_C_BLOSC, 		BLK_SIZE,0, "","TurboPFor's [delta]+transpose+lz4" }, 
  { P_BS_ZLIB,  	"blosc_zlib",		C_C_BLOSC, 		BLK_SIZE,0, "","TurboPFor's [delta]+transpose+lz4" }, 

  { LF_FOR,			"LibFor_For", 		C_SIMDCOMP128, 	BLK_V128,0,"","For"},
  { LF_FORX,		"LibFor_ForDA",		C_SIMDCOMP128, 	BLK_V128,0,"","For direct access"},

  { LI_PACK,		"LI_Pack",			C_LITTLEPACK,	0,    	 0,"","Bit packing (scalar)" },
  { LI_TURBOPACK,	"LI_TurboPack",		C_LITTLEPACK,	0,    	 0,"","Bit packing (scalar)" },
  { LI_SCPACK,		"LI_SuperPack",		C_LITTLEPACK,	0,    	 0,"","Bit packing (scalar)" },
  { LI_HORPACK,		"LI_HorPack",		C_LITTLEPACK,	0,    	 0,"","Bit packing (sse4.1 horizontal)" },
  { LI_BMIPACK,		"LI_BMIPack256",	C_LITTLEPACK,	0,    	 0,"","Bit packing (avx2)"	},

  { P_QMX,			"qmx",		C_QMX,	    	0,    	 0,"","QMX SIMD (inefficient for small blocks)"	},
  //-------- lz77 + [delta] + transpose/shuffle ---------------
  { P_LZT,			"LzTurbo",	C_LZTURBO, 		BLK_SIZE, 0, "20,21,22,32" },
  { P_VSHUF,		"VSimpleANS",		C_LZTURBO, 		BLK_SIZE, 0, "20,21,22,32" },

  { LZ4_,  			"lz4",				C_LZ4,			BLK_SIZE, 0, "",   "lz4" }, 
  { LZ4_BIT,  		"lz4_bit",			C_LZ4,			BLK_SIZE, 0, "2,4,8","Bitshuffle + [delta]+lz4" }, 
  { LZ4_NIBBLE,  	"lz4_nibble",		C_LZ4,			BLK_SIZE, 0, "2,4,8","TurboPFor's [delta]+nibble transpose + lz4" }, 
  { LZ4_BITX,  		"lz4_bitxor",		C_LZ4,			BLK_SIZE, 0, "2,4,8","Bitshuffle + [xor]+lz4" }, 
  { LZ4_NIBBLEX,  	"lz4_nibblexor",	C_LZ4,			BLK_SIZE, 0, "2,4,8","TurboPFor's [xor]+nibble transpose + lz4" }, 
  { LZ4_BYTE,  		"lz4_byte",			C_LZ4,			BLK_SIZE, 0, "2,4,8","TurboPFor's [delta]+byte transpose + lz4" }, 
// { LZ4_FP8,  		"lz4_fp8",			C_LZ4,			BLK_SIZE, 0, "4,8","TurboPFor's [delta]+fp transpose + lz4" }, 
  { P_ZLIB, 		"zlib", 			C_ZLIB,			BLK_SIZE, 0, "4,8","TurboPFor's [delta]+byte transpose + zlib" },
  { P_BITSHUFFLE, 	"BitShuffle", 		C_TURBOPFOR,	BLK_SIZE,0,"2,4,8","Bit shuffle (simd)" }, 
  { P_BS_SHUFFLE, 	"Blosc_Shuffle",	C_C_BLOSC,		BLK_SIZE,0,"2,4,8","Blosc's shuffle (simd)" }, 
  { P_VTENC, 		"vtenc",			C_VTENC,		BLK_SIZE,0,"2,4,8","VTEnc" }, 

    #ifdef LZTURBO
//  #include "../beplugr.h"
    #endif  
