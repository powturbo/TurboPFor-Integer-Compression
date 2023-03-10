/**
    Copyright (C) powturbo 2015-2023
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
**/
//  TurboPFor - "general purpose compression codec: lz, entropy coding, bwt,... "
//------------------- LZ compression --------------------------------------------------
#include <stdio.h>
#include "include_/conf.h"
#include "include_/transpose.h"
#include "include_/bitutil.h"
#include "include_/iccodec.h"


  #ifdef _BITSHUFFLE
#include "bitshuffle/src/bitshuffle.h"
    #ifndef LZ4
#include "bitshuffle/lz4/lz4.h"
    #endif
  #endif

//------------------------------------------------------------------------------------------------------------------------------
#define powof2(n) !((n)&((n)-1))

enum {
  P_NONE,
  P_LZTURBO,
  P_LZ4,
  P_ZLIB,

  P_ZSTD,
  P_FSE,
  P_FSEH,
  P_LZTANS,
  P_TURBORC,
  P_MEMCPY,
  P_LAST, 
};

char *_codstr[] = { "none", "lzturbo", "lz4", "zlib", "zstd", "fse", "fsehuf", "turboanx", "turborc", "memcpy", NULL };
char *codstr(unsigned cid) { return (cid < P_LAST)?_codstr[cid]:""; }

int lzidget(char *scmd) {
  int i;
  for(i = 0; _codstr[i]; i++)
    if(!strcasecmp(scmd, _codstr[i])) break;
  if(!_codstr[i]) { printf("compressor '%s' not implemented\n", scmd); exit(-1); }
  return i;
}
  #ifdef _LZTURBO
#define _TURBOANX
#include "../lz/ans.h"
#include "../lz/anst.h"
#include "lzt.c"
  #endif

  #ifdef _LZ4
#include "ext/lz4/lib/lz4.h"
#include "ext/lz4/lib/lz4hc.h"
#include "ext/lz4/lib/lz4frame.h"
  #endif

  #ifdef _TURBORC
#include "ext/Turbo-Range-Coder/turborc.h"
  #endif

  #ifdef _ZSTD
#define ZSTD_STATIC_LINKING_ONLY
#include "ext/zstd/lib/zstd.h"
#define USE_LZ
  #endif

  #ifdef _ZLIB
#include "ext/zlib/zlib.h" //#include <zlib.h>
  #endif

size_t codecenc(unsigned char *in, size_t inlen, unsigned char *out, unsigned outsize, int codid, int codlev, char *codprm) { 
  if(!inlen) return 0;
  switch(codid) {
      #ifdef _LZTURBO
        #include "lztc.c"
      #endif

      #ifdef _LZ4
    case P_LZ4: if(codlev > 16) codlev = 16;
      if(strchr(codprm,'M')) { 
	    return !codlev?LZ4_compress_fast(   (char *)in, (char *)out, inlen, outsize, 4):
		     (codlev<9?LZ4_compress_default((char *)in, (char *)out, inlen, outsize):
			           LZ4_compress_HC(     (char *)in, (char *)out, inlen, outsize, codlev)); 
	  } else { 
	    char *q;
        LZ4F_preferences_t opts = LZ4F_INIT_PREFERENCES;
        opts.compressionLevel      = codlev;
        if(strchr(codprm,'s'))   opts.favorDecSpeed         = 1;
        if(strchr(codprm,'f'))   opts.autoFlush             = 1;
        opts.frameInfo.blockSizeID = LZ4F_max4MB;
        if(q=strchr(codprm,'B')) 
		  opts.frameInfo.blockSizeID = (LZ4F_blockSizeID_t)atoi(q+(q[1]=='='?2:1));
        if(opts.frameInfo.blockSizeID > LZ4F_max4MB) 
		  opts.frameInfo.blockSizeID=LZ4F_max4MB;
        else if(opts.frameInfo.blockSizeID && opts.frameInfo.blockSizeID<LZ4F_default) 
		  opts.frameInfo.blockSizeID=LZ4F_default;
        int rc = LZ4F_compressFrame(out, outsize, in, inlen, &opts);
		if(LZ4F_isError(rc)) return -1;
		return rc;
      }
      #endif

      #ifdef _TURBORC
    case P_TURBORC: { int ec = 0; char *q; if(q=strstr(codprm,"ss")) ec = 2;
      switch(codlev) {
        case 1 : return ec==2?rccssenc(in, inlen, out, 5,6):rccsenc(in, inlen, out);
        case 2 : return ec==2?rcssenc( in, inlen, out, 5,6):rcsenc( in, inlen, out);
		//#define bwtflag(z) (z==2?BWT_BWT16:0) | (xprep8?BWT_PREP8:0) | forcelzp | (verbose?BWT_VERBOSE:0) | xsort <<14 | itmax <<10 | lenmin
		case 20: return rcbwtenc(in,inlen,out,9,0, 1);
      }
    }
      #endif

      #ifdef _ZSTD
    case P_ZSTD:{ 
	  ZSTD_CCtx    *ctx = ZSTD_createCCtx();
      ZSTD_parameters p = ZSTD_getParams(codlev, inlen, 0);
      if(inlen) { 
	    int windowLog = bsr32(inlen)-powof2(inlen); 
		ZSTD_CCtx_setParameter(ctx, 
		ZSTD_c_windowLog, windowLog); 
	  }
      unsigned rc = ZSTD_compress_advanced(ctx, out, outsize, in, inlen, NULL, 0, p);
      ZSTD_freeCCtx(ctx);
      return rc;
    }
	  #endif
	  
      #ifdef _FSE
    case P_FSE: { 
	  size_t o = FSE_compress(out, outsize, in, inlen);
      if(o == 1) { out[0] = in[0]; return 1; }
      if(!o || o >= inlen) { memcpy(out, in, inlen); return inlen; }
      return o;
    }
	  #endif
	  
      #ifdef _FSEHUF
    case P_FSEH: { 
	  size_t o = HUF_compress(out, outsize, in, inlen);
      if(o == 1) { out[0] = in[0]; return 1; }
      if(!o || o >= inlen) { memcpy(out, in, inlen); return inlen; }
      return o;
    }
      #endif

      #ifdef _ZLIB
    case P_ZLIB: { 
	  uLongf outlen = outsize;                 
	  if(codlev < 1) codlev = 1; 
	  if(codlev > 9) codlev = 9;
      int rc = compress2(out, &outlen, in, inlen, codlev);
      if(rc != Z_OK) printf("zlib compress2 rc=%d\n", rc);
      return outlen;
    }
      #endif
	default: memcpy(out,in,inlen); return inlen;
  }
  return 0;
}

size_t codecdec(unsigned char *in, size_t inlen, unsigned char *out, unsigned outlen, int codid, int codlev, char *codprm) { 
  if(!outlen) return 0;
  switch(codid) {
      #ifdef _LZTURBO
    #include "lztd.c"
      #endif

      #ifdef _LZ4
    case P_LZ4:
      if(strchr(codprm,'M')) { LZ4_decompress_safe((const char *)in, (char *)out, inlen, outlen); break; }
      else {
        LZ4F_dctx *ctx; LZ4F_createDecompressionContext(&ctx, LZ4F_VERSION);
        size_t ilen = inlen, olen = outlen, rc = LZ4F_decompress(ctx, out, &olen, in, &ilen, NULL);
        LZ4F_freeDecompressionContext(ctx);
        return rc;
      }
      #endif

      #ifdef _TURBORC
    case P_TURBORC: { 
	  int ec = 0; 
	  char *q; 
      if(q=strstr(codprm,"ss")) ec = 2;
      switch(codlev) {
        case 1 : return ec==2?rccssdec(in, outlen, out, 5,6):rccsdec(in, outlen, out);
        case 2 : return ec==2?rcssdec( in, outlen, out, 5,6):rcsdec( in, outlen, out);
		case 20: inlen==outlen?memcpy(out,in,outlen):rcbwtdec(in,outlen,out,9, 0); return 0;
//        case 2 : return turborcndec( in, outlen, out);
      }
    }
      #endif

      #ifdef _ZSTD
    case P_ZSTD: ZSTD_decompress( out, outlen, in, inlen); break;
	  #endif
	  
      #ifdef _FSE
    case P_FSE:  if(inlen == outlen) memcpy(out, in, outlen); else if(inlen == 1) memset(out,in[0],outlen); else FSE_decompress(out, outlen, in, inlen); break;
      #endif
      #ifdef _FSEHUF
    case P_FSEH: if(inlen == outlen) memcpy(out, in, outlen); else if(inlen == 1) memset(out,in[0],outlen); else HUF_decompress(out, outlen, in, inlen); break;
      #endif

      #ifdef _ZLIB
    case P_ZLIB: { uLongf outsize = outlen; int rc = uncompress(out, &outsize, in, inlen); } break;
      #endif
	default: memcpy(out,in, outlen); return inlen;
  }
  return 0;
}

//------------------------------------ https://github.com/zeux/meshoptimizer ------------------------------------------------------------------------------------------
  #ifdef _MESHOPT 
#include "ext/meshoptimizer/src/meshoptimizer.h"
unsigned meshenc(const float *in, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned outsize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned vs   = nz <= 64?nz*sizeof(in[0]):sizeof(in[0]), vn = nz <= 64?nx*ny:nx*ny*nz, 
           clen = meshopt_encodeVertexBuffer(tmp, outsize, in, vn, vs);
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize-4, codid, codlev, codprm)+4;
}

void meshdec(const uint8_t *in, unsigned inlen, float *out, unsigned nx, unsigned ny, unsigned nz, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned vs   = nz <= 64?nz*sizeof(out[0]):sizeof(out[0]), vn = nz <= 64?nx*ny:nx*ny*nz,
           clen = ctou32(in); 
  codecdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  meshopt_decodeVertexBuffer(out, vn, vs, tmp, clen);
}
  #endif
  
//------------------------------------------------ TurboByte (SIMD Varint) -> codec (lz, entropy coding, bwt...) ----------------------------
//-- TurboByte -> codec
unsigned lzv8enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned clen = (unsigned char *)(esize==2?v8enc16(in, inlen/2, tmp):v8enc32(in, inlen/4, tmp)) - tmp;
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lzv8dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned clen = ctou32(in); codecdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  esize==2?v8dec16(tmp, outlen/2, out):v8dec32(tmp, outlen/4, out);
  return inlen;
}

// TurboByte xor -> codec
unsigned lzv8xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) { // TurboVByte
  unsigned clen = (unsigned char *)(esize==2?v8xenc16(in, inlen/2, tmp,0):v8xenc32(in, inlen/4, tmp,0)) - tmp; ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned v8lzxdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned clen = ctou32(in); codecdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  esize==2?v8xdec16(tmp, outlen/2, out,0):v8xdec32(tmp, outlen/4, out,0);
  return inlen;
}

// TurboByte zigzag -> codec
unsigned lzv8zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned clen = (unsigned char *)(esize==2?v8zenc16(in, inlen/2, tmp,0):v8zenc32(in, inlen/4, tmp,0)) - tmp; ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lzv8zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned clen = ctou32(in); codecdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  esize==2?v8zdec16(tmp, outlen/2, out,0):v8zdec32(tmp, outlen/4, out,0);
  return inlen;
}

//-------------------------------------------- Byte transpose -> codec (lz, entropy coding, bwt,...) ------------------------------------------------------------------------
//-- transpose in blocks ---------
static unsigned tpbsize;

void tpsizeset(unsigned _tpbsize) {
  tpbsize = _tpbsize;   
}

unsigned tpbenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned esize) {
  unsigned char *ip = in, *op = out;
  unsigned bsize = tpbsize?tpbsize:inlen, iplen; 
  
  for(; ip < in + inlen; ip += bsize, op += iplen) {
    iplen = (in+inlen)-ip; iplen = iplen>bsize?bsize:iplen;
	tpenc(ip, iplen, op, esize);
  }
}

unsigned tpbdec(unsigned char *in, unsigned outlen, unsigned char *out, unsigned esize) {
  unsigned char *ip = in, *op = out;
  unsigned bsize = tpbsize?tpbsize:outlen, oplen;
  
  for(; op < out + outlen; ip += bsize, op += oplen) {
	oplen = (out+outlen)-op; oplen = oplen>bsize?bsize:oplen;
	tpdec(ip, oplen, op, esize);
  }
}

//-- transpose -> codec
unsigned lztpenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  tpbenc(in, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tpbdec(tmp, outlen, out, esize);
  return inlen;
}

//-- xor -> transpose -> codec
unsigned lztpxenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) { // XOR
  tpxenc(in, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpxdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tpxdec(tmp, outlen, out, esize);
  return inlen;
}

//-- zigzag -> transpose -> codec
unsigned lztpzenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) { // Zigzag
  tpzenc(in, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpzdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tpzdec(tmp, outlen, out, esize);
  return inlen;
}
//----------------------------------------------------------------------------------------------------------------
//-- transpose in blocks/segments
unsigned tp4benc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned esize) {
  unsigned char *ip = in, *op = out;
  unsigned bsize = tpbsize?tpbsize:inlen, iplen; 
  
  for(; ip < in + inlen; ip += bsize, op += iplen) {
    iplen = (in+inlen)-ip; iplen = iplen>bsize?bsize:iplen;
	tp4enc(ip, iplen, op, esize);
  }
}

unsigned tp4bdec(unsigned char *in, unsigned outlen, unsigned char *out, unsigned esize) {
  unsigned char *ip = in, *op = out;
  unsigned bsize = tpbsize?tpbsize:outlen, oplen;
  
  for(; op < out + outlen; ip += bsize, op += oplen) {
	oplen = (out+outlen)-op; oplen = oplen>bsize?bsize:oplen;
	tp4dec(ip, oplen, op, esize);
  }
}

//----------------------------------------------- tp4 : Nibble transpose -> codec --------------------------------

//-- transpose Nibble -> codec
unsigned lztp4enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  tp4enc(in, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpd4ec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4dec(tmp, outlen, out, esize);
  return inlen;
}

//-- xor -> transpose Nibble -> codec
unsigned lztp4xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) { //XOR
  tp4xenc(in, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp4xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4xdec( tmp, outlen, out, esize);
  return inlen;
}

//-- zigzag delta -> transpose Nibble -> codec
unsigned lztp4zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  tp4zenc(in, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp4zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4zdec(tmp, outlen, out, esize);
  return inlen;
}


//-----------------------------------------------------------------------------------------------------------------------------------
static int tpmode;
void tpmodeset(unsigned _tpmode) {
  tpmode = _tpmode;   printf("tpmode=%d ", tpmode);
}

#define TPENC(in, n, out, esize) tpmode==4?tp4benc(in, n, out, esize):tpbenc(in, n, out, esize)   // use nibble transpose
#define TPDEC(in, n, out, esize) tpmode==4?tp4bdec(in, n, out, esize):tpbdec(in, n, out, esize) 

//-- transpose -> rle
unsigned tprleenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  tpenc(in, inlen, tmp, esize);
  unsigned clen = trlec(tmp, inlen, out+4); 
  if(clen >= inlen) { 
    clen = inlen; 
	memcpy(out+4,in,inlen); 
  }
  ctou32(out) = clen;
  return clen + 4;
}

unsigned tprledec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); 
  if(clen >= outlen) 
	memcpy(out, in+4, outlen);
  else {
	trled(in+4, clen, tmp, outlen);
    tpdec(tmp, outlen, out, esize);
  }
  return inlen;
}

//-- xor -> transpose -> rle
unsigned tprlexenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { // xor rle
  tpxenc(in, inlen, tmp, esize);
  unsigned clen = trlec(tmp, inlen, out+4); 
  if(clen >= inlen) { 
    clen = inlen; 
	memcpy(out+4,in,inlen); 
  }
  ctou32(out) = clen;
  return clen + 4;
}

unsigned tprlexdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); 
  if(clen >= outlen) memcpy(out, in+4, outlen);
  else {
	trled(in+4, clen, tmp, outlen);
    tpxdec(tmp, outlen, out, esize);
  }
  return inlen;
}

//-- zigzag delta -> transpose -> rle
unsigned tprlezenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { // Zigzag rle
  tpzenc(in, inlen, tmp, esize);
  unsigned clen = trlec(tmp, inlen, out+4); 
  if(clen >= inlen) { 
    clen = inlen; 
	memcpy(out+4,in,inlen); 
  }
  ctou32(out) = clen;
  return clen + 4;
}

unsigned tprlezdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); 
  if(clen >= outlen) 
	memcpy(out, in+4, outlen);
  else {
	trled(in+4, clen, tmp, outlen);
    tpzdec(tmp, outlen, out, esize);
  }
  return inlen;
}

//-- transpose -> rle -> codec  (usually entropy coder )
unsigned lztprleenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) { // Zigzag rle
  tpenc(in, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); 
  if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lztprledec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned clen = ctou32(in); 
  codecdec(in+4, inlen-4, out, clen, codid, codlev, codprm); 
  clen >= outlen?memcpy(tmp, out, outlen):trled(out, clen, tmp, outlen);
  tpdec(tmp, outlen, out, esize);
  return inlen;
}

//-- xor -> transpose -> rle -> codec
unsigned lztprlexenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) { // Zigzag rle
  tpxenc(in, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); 
  if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lztprlexdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned clen = ctou32(in); codecdec(in+4, inlen-4, out, clen, codid, codlev, codprm); 
  clen >= outlen?memcpy(tmp, out, outlen):trled(out, clen, tmp, outlen);
  tpxdec(tmp, outlen, out, esize);
  return inlen;
}

//-- zigzag delta -> transpose -> rle -> codec
unsigned lztprlezenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) { // Zigzag rle
  tpzenc(in, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); 
  if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lztprlezdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned clen = ctou32(in); 
  codecdec(in+4, inlen-4, out, clen, codid, codlev, codprm); 
  clen >= outlen?memcpy(tmp, out, outlen):trled(out, clen, tmp, outlen);
  tpzdec(tmp, outlen, out, esize);
  return inlen;
}

//------------------------------------------------------- 2D -------------------------------------------------
// transpose 2D -> transpose (byte/Nibble) -> codec
unsigned lztpd2enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm) { 
  tp2denc(     in,  x, y,  out, esize);                            
  TPENC(       out, inlen, tmp, esize);                            
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpd2dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm) { //printf("#2D[%u,%u]", x, y);
  codecdec(  in,  inlen,  out, outlen, codid, codlev, codprm); 
  TPDEC(  out, outlen, tmp, esize); 
  tp2ddec(tmp, x, y,   out, esize); 
  return x*y*esize;
}

// transpose 2D -> xor -> transpose (byte/Nibble) -> codec
unsigned lztpd2xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm) { 
  tp2denc( in,  x, y,  out, esize);
  tpxenc(  out, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpd2xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm) {
  codecdec(  in,  inlen,  out, outlen, codid, codlev, codprm);
  tpxdec(  out, outlen, tmp, esize);
  tp2ddec(tmp, x, y,   out, esize);
  return inlen;
}

// transpose 2D -> zigzag -> transpose (byte/Nibble) -> codec
unsigned lztpd2zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm) { 
  tp2denc(      in,  x, y, out, esize);
  tpzenc(       out, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpd2zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm) {
  codecdec( in,  inlen, out, outlen, codid, codlev, codprm);
  tpzdec(  out, outlen, tmp, esize); //  bitzdec(tmp, outlen, esize);
  tp2ddec(tmp, x, y,   out, esize);
  return inlen;
}

//--------------------------------------------------------- 3D ------------------------------------------------------------
// transpose 3D -> transpose (byte/Nibble) -> codec
unsigned lztpd3enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  tp3denc(     in, x, y, z, out, esize);
  TPENC(      out, inlen,  tmp, esize);
  return codecenc(tmp, inlen,  out, outsize, codid, codlev, codprm);
}

unsigned lztpd3dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize,  unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  codecdec(  in,  inlen,   out, outlen, codid, codlev, codprm);
  TPDEC(  out, outlen,  tmp, esize);
  tp3ddec(tmp, x, y, z, out, esize);
  return inlen;
}

// transpose 3D -> xor -> transpose (byte/Nibble) -> codec 
unsigned lztpd3xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  tp3denc(     in, x, y, z, out, esize);//  bitxenc(     tmp, inlen,  out, esize);
  tpxenc(     out, inlen,  tmp, esize);
  return codecenc(tmp, inlen,  out, outsize, codid, codlev, codprm);
}

unsigned lztpd3xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  codecdec(  in,  inlen,   out, outlen, codid, codlev, codprm);
  tpxdec( out, outlen,  tmp, esize);//  bitxdec(tmp, outlen,       esize);
  tp3ddec(tmp, x, y, z, out, esize);
  return inlen;
}

// transpose 3D -> zigzag -> transpose (byte/Nibble) -> codec
unsigned lztpd3zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  tp3denc(     in,  x, y, z, out, esize);//  bitzenc(     tmp, inlen,   out, esize);
  tpzenc(     out, inlen,   tmp, esize);
  return codecenc(tmp, inlen,   out, outsize, codid, codlev, codprm);
}

unsigned lztpd3zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  codecdec(  in, inlen,  out, outlen, codid, codlev, codprm);
  tpzdec(   out, outlen, tmp, esize);//  bitzdec(tmp, outlen, esize);
  tp3ddec(tmp, x, y, z, out, esize);
  return inlen;
}

//--------------------------------------------------------- 4D ------------------------------------------------------------
// transpose 4D -> transpose (byte/Nibble) -> codec
unsigned lztpd4enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  tp4denc(     in, w, x, y, z, out, esize);
  TPENC(       out, inlen,     tmp, esize);
  return codecenc(tmp, inlen,     out, outsize, codid, codlev, codprm);
}

unsigned lztpd4dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize,  unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  codecdec(  in,  inlen,      out, outlen, codid, codlev, codprm);
  TPDEC(  out, outlen,     tmp, esize);
  tp4ddec(tmp, w, x, y, z, out, esize);
  return inlen;
}

// transpose 4D -> xor -> transpose (byte/Nibble) -> codec
unsigned lztpd4xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  tp4denc(     in, w, x, y, z, out, esize);
  tpxenc(     out, inlen,      tmp, esize);
  return codecenc(tmp, inlen,  out, outsize, codid, codlev, codprm);
}

unsigned lztpd4xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  codecdec(  in,  inlen,      out, outlen, codid, codlev, codprm);
  tpxdec(   out, outlen,     tmp, esize);
  tp4ddec(tmp, w, x, y, z, out, esize);
  return inlen;
}

// transpose 4D -> zigzag -> transpose (byte/Nibble) -> codec
unsigned lztpd4zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  tp4denc(     in, w, x, y, z, out, esize);
  tpzenc(     out, inlen,      tmp, esize);
  return codecenc(tmp, inlen,  out, outsize, codid, codlev, codprm);
}

unsigned lztpd4zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm) {
  codecdec(  in,  inlen,     out, outlen, codid, codlev, codprm);
  tpzdec(   out, outlen,     tmp, esize);
  tp4ddec(tmp, w, x, y, z, out, esize);
  return inlen;
}
 
  #ifdef _BITSHUFFLE //-------------------------- bit transpose ----------------------------------------------------------------------  
//-- bitshuffle
static void bitshuffle(uint8_t *in, unsigned n, uint8_t *out, unsigned esize) {
  bshuf_bitshuffle(  in, out, n/esize, esize, 0); 
  memcpy((char *)out+(n&(~(8*esize-1))), (char *)in+(n&(~(8*esize-1))), n&(8*esize-1));
}

static void bitunshuffle(uint8_t *in, unsigned n, uint8_t *out, unsigned esize) {
  bshuf_bitunshuffle(in, out, n/esize, esize, 0); memcpy((char *)out+(n&(~(8*esize-1))), (char *)in+(n&(~(8*esize-1))), n&(8*esize-1));
}
  
//-- bitshuffle -> codec
unsigned lztp1enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  bitshuffle(in, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp1dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  bitunshuffle(tmp, outlen, out, esize);
  return inlen;
}

//-- xor -> bitshuffle -> codec
unsigned lztp1xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  bitxenc(in, inlen, out, esize);
  bitshuffle(out, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp1xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  bitunshuffle(tmp, outlen, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

//-- zigzag delta -> bitshuffle -> codec 
unsigned lztp1zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  bitzenc(in, inlen, out, esize);                
  bitshuffle(out, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp1zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  bitunshuffle(tmp, outlen, out, esize);
  bitzdec(out, outlen, esize);
  return inlen;
}
  #endif
  
//---------------------------------- TurboVLC + Codec ( Exponent -> Codec,  Mantissa -> direct bitio (right <- left) ---------------------------------

#include "include_/bitiobe.h"
#include "include_/vlcbit.h"
// store the last bytes without encoding, when inlen is not multiple of array element size
#define INDEC  size_t inlen  = _inlen /sizeof( in[0]); { unsigned char *p_=_in+_inlen,  *_p = _in +(_inlen & ~(sizeof(in[0] )-1)); while(_p < p_) { *op++  = *_p++; } }
#define OUTDEC size_t outlen = _outlen/sizeof(out[0]); { unsigned char *p_=_out+_outlen,*_p = _out+(_outlen& ~(sizeof(out[0])-1)); while(_p < p_) *_p++  = *ip++; }
	
size_t vlccomp32(unsigned char *_in, size_t _inlen, unsigned char *out, size_t outsize, unsigned char *tmp, int codid, int codlev, char *codprm) { //bitgput32(bw,br, x); bitenormr(bw,br,op_);//bitdnormr(bw,br,bp); bitgget32(bw,br, x);
  unsigned char *op = out+4, *tp = tmp, *tmp_ = tmp+_inlen, *bp = tmp_;
  uint32_t      *in = (uint32_t *)_in, *ip;
  INDEC;
  bitedef(bw,br); biteinir(bw,br,bp);
  for(ip = in; ip < in+inlen; ip++) {
	uint32_t x = ip[0];
	bitvcput(bw,br,tp,bp,VLC_VN8,VLC_VB8, x); bitenormr(bw,br,bp);              if(bp <= tp+8) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; }
  }
  bitflushr(bw,br,bp);
  unsigned l = tmp_ - bp;  
  
  int clen = codecenc(tmp, tp-tmp, op, _inlen-4, codid, codlev, codprm);        if(clen < 0 || op+clen+l >= out+_inlen) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; }
  ctou32(out) = clen; op += clen;                               
  memmove(op, bp, l); op += l;
  e:return op - out;
}

size_t vlcdecomp32(unsigned char *in, size_t inlen, unsigned char *_out, size_t _outlen, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned char *ip = in+4, *bp = in+inlen, *tp = tmp;
  uint32_t      *out = (uint32_t *)_out, *op = out, x;
  if(inlen >= _outlen){ memcpy(_out, in, _outlen); return inlen; }
  OUTDEC;
  bitddef(bw, br); bitdinir(bw,br,bp);
  unsigned clen = ctou32(ip-4); 
  codecdec(ip, clen, tmp, outlen, codid, codlev, codprm);
  for(; op != out+(outlen&~(2-1)); op+=2) { 
    bitdnormr(bw,br,bp);
    bitvcget(bw,br,tp,bp,VLC_VN8,VLC_VB8, op[0]);
    bitvcget(bw,br,tp,bp,VLC_VN8,VLC_VB8, op[1]);
  }
  for(; op != out+outlen; op++) { 
    bitdnormr(bw,br,bp);
    bitvcget(bw,br,tp,bp,VLC_VN8,VLC_VB8, op[0]);
  }
  return inlen;
}
