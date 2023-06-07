/**
    Copyright (C) powturbo 2015-2023
    SPDX-License-Identifier: GPL v2 License

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
#include <stdlib.h>
#include <string.h>
#include "include_/conf.h"
#include "include_/transpose.h"
#include "include_/bitutil.h"
#include "include_/vint.h"
#include "include_/iccodec.h"
#include "include_/trle.h"

  #ifdef _BITSHUFFLE
#include "ext/bitshuffle/src/bitshuffle.h"
    #ifndef LZ4
#include "ext/bitshuffle/lz4/lz4.h"
    #endif
  #endif
extern int bwtx, forcelzp;
 
//------------------------------------------------------------------------------------------------------------------------------
#define powof2(n) !((n)&((n)-1))

char *_codstr[] = { "none", "lzturbo", "lz4", "zlib", "zstd", "fse", "fsehuf", "turboanx", "turborc", "memcpy", NULL };
char *codstr(unsigned cid) { return (cid < ICC_LAST)?_codstr[cid]:""; }

int lzidget(char *scmd) {
  int i;
  for(i = 0; _codstr[i]; i++)
    if(!strcasecmp(scmd, _codstr[i])) break;
  if(!_codstr[i]) die("compressor '%s' not implemented\n", scmd);
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
#include "ext/Turbo-Range-Coder/include/turborc.h"
#include "ext/Turbo-Range-Coder/include/anscdf.h"
  #endif

  #ifdef _ZSTD
#define ZSTD_STATIC_LINKING_ONLY
#include "ext/zstd/lib/zstd.h"
#define USE_LZ
  #endif

  #ifdef _FSEHUF
#define HUF_PUBLIC_API 
HUF_PUBLIC_API size_t HUF_compress(void* dst, size_t dstCapacity, const void* src, size_t srcSize);
HUF_PUBLIC_API size_t HUF_decompress(void* dst,  size_t originalSize, const void* cSrc, size_t cSrcSize);
  #endif

  #ifdef _FSE
#include "ext/zstd/lib/common/fse.h"  
#include "ext/fse/fse.h"  
  #endif
 
  #ifdef _ZLIB
#include "ext/zlib/zlib.h" //#include <zlib.h>
  #endif

size_t codecenc(unsigned char *in, size_t inlen, unsigned char *out, unsigned outsize, int codid, int codlev, unsigned char *codprm) { 
  if(!inlen) return 0;
  switch(codid) {
      #ifdef _LZTURBO
        #include "lztc.c"
      #endif

      #ifdef _LZ4
    case ICC_LZ4: if(codlev > 16) codlev = 16;
      if(strchr(codprm,'M')) { 
	    return !codlev?LZ4_compress_fast(   (char *)in, (char *)out, inlen, outsize, 4):
		     (codlev<9?LZ4_compress_default((char *)in, (char *)out, inlen, outsize):
			           LZ4_compress_HC(     (char *)in, (char *)out, inlen, outsize, codlev)); 
	  } else { 
	    char *q;
		  #if 1
        LZ4F_preferences_t opts = LZ4F_INIT_PREFERENCES;
        opts.compressionLevel      = codlev;
        if(strchr(codprm,'s'))   opts.favorDecSpeed         = 1;
        if(strchr(codprm,'f'))   opts.autoFlush             = 1;
        opts.frameInfo.blockSizeID = LZ4F_max4MB;
        if(q=strchr(codprm,'B')) 
		  opts.frameInfo.blockSizeID = (LZ4F_blockSizeID_t)atoi(q+(q[1]=='='?2:1));
        if(opts.frameInfo.blockSizeID > LZ4F_max4MB) 
		  opts.frameInfo.blockSizeID = LZ4F_max4MB;
        else if(opts.frameInfo.blockSizeID && opts.frameInfo.blockSizeID<LZ4F_default) 
		  opts.frameInfo.blockSizeID = LZ4F_default;
        int rc = LZ4F_compressFrame(out, outsize, in, inlen, &opts);
		if(LZ4F_isError(rc)) return -1;
		  #else
		int rc = LZ4_compress_fast(in, out, (int)inlen, (int)outsize, 0);
          #endif
		return rc;
      }
      #endif

      #ifdef _TURBORC
    case ICC_TURBORC: { int ec = 0, bwtlev=8; char *q; 
	  if(q=strstr(codprm,"ss")) ec = 2;
	  if(q = strchr(codprm,'l')) bwtlev = atoi(q+(q[1]=='='?2:1)); if(bwtlev > 9) bwtlev=9;
      switch(codlev) {
        case 1 : return ec==2?rccssenc(in, inlen, out, 5,6):rccsenc(in, inlen, out);
        case 2 : return ec==2?rcssenc( in, inlen, out, 5,6):rcsenc( in, inlen, out);
		//#define bwtflag(z) (z==2?BWT_BWT16:0) | (xprep8?BWT_PREP8:0) | forcelzp | (verbose?BWT_VERBOSE:0) | xsort <<14 | itmax <<10 | lenmin
		case 20: return rcbwtenc(in,inlen,out,bwtlev,0, 1);
		  #ifdef _ANS
		case 56: return anscdfenc(in,inlen,out);
		case 64: return anscdf1enc(in,inlen,out);
		default: die("TurboRC codec level '%d' not included", codlev);
		  #endif
      }
    }
      #endif

      #ifdef _ZSTD
    case ICC_ZSTD:{ 
	  ZSTD_CCtx    *ctx = ZSTD_createCCtx();
      ZSTD_parameters p = ZSTD_getParams(codlev, inlen, 0);
      if(inlen) { 
	    int windowLog = bsr32(inlen)-powof2(inlen); 
		ZSTD_CCtx_setParameter(ctx, ZSTD_c_windowLog, windowLog); 
	  }
      unsigned rc = ZSTD_compress_advanced(ctx, out, outsize, in, inlen, NULL, 0, p);
      ZSTD_freeCCtx(ctx);
      return rc;
    }
	  #endif
	  
      #ifdef _FSE
    case ICC_FSE: { 
	  size_t o = FSE_compress(out, outsize, in, inlen);
      if(o == 1) { out[0] = in[0]; return 1; }
      if(!o || o >= inlen) { memcpy(out, in, inlen); return inlen; }
      return o;
    }
	  #endif
	  
      #ifdef _FSEHUF
    case ICC_FSEH: { 
	  size_t o = HUF_compress(out, outsize, in, inlen);
      if(o == 1) { out[0] = in[0]; return 1; }
      if(!o || o >= inlen) { memcpy(out, in, inlen); return inlen; }
      return o;
    }
      #endif

      #ifdef _ZLIB
    case ICC_ZLIB: { 
	  uLongf outlen = outsize;                 
	  if(codlev < 1) codlev = 1; 
	  if(codlev > 9) codlev = 9;
      int rc = compress2(out, &outlen, in, inlen, codlev);
      if(rc != Z_OK) printf("zlib compress2 rc=%d\n", rc);
      return outlen;
    }
      #endif
	case ICC_MEMCPY: memcpy(out,in,inlen); return inlen;
	default: die("codec '%d' '%s' not implemented\n", codid, codstr(codid) );
  }
  return 0;
}

size_t codecdec(unsigned char *in, size_t inlen, unsigned char *out, unsigned outlen, int codid, int codlev, unsigned char *codprm) { 
  if(!outlen) return 0;
  switch(codid) {
      #ifdef _LZTURBO
    #include "lztd.c"
      #endif

      #ifdef _LZ4
    case ICC_LZ4:
      if(strchr(codprm,'M')) { LZ4_decompress_safe((const char *)in, (char *)out, inlen, outlen); break; }
      else {
		  #if 1
        LZ4F_dctx *ctx; 
		LZ4F_createDecompressionContext(&ctx, LZ4F_VERSION);
        size_t ilen = inlen, olen = outlen, rc = LZ4F_decompress(ctx, out, &olen, in, &ilen, NULL);
        LZ4F_freeDecompressionContext(ctx);
        return rc;
		  #else
		LZ4_decompress_safe(in, out, (int)inlen, (int)outlen);
		  #endif
      }
      #endif

      #ifdef _TURBORC
    case ICC_TURBORC: { 
	  int ec = 0, bwtlev = 8; 
	  char *q; 
      if(q=strstr(codprm,"ss")) ec = 2;
      if(q = strchr(codprm,'l')) bwtlev = atoi(q+(q[1]=='='?2:1)); if(bwtlev > 9) bwtlev=9;
      switch(codlev) {
        case 1 : return ec==2?rccssdec(in, outlen, out, 5,6):rccsdec(in, outlen, out);
        case 2 : return ec==2?rcssdec( in, outlen, out, 5,6):rcsdec( in, outlen, out);
		case 20: inlen==outlen?memcpy(out,in,outlen):rcbwtdec(in,outlen,out,bwtlev, 0); return 0;
		  #ifdef _ANS
		case 56: inlen==outlen?memcpy(out,in,outlen):anscdfdec( in, outlen, out); return outlen;
		case 64: inlen==outlen?memcpy(out,in,outlen):anscdf1dec(in, outlen, out); return outlen;
		  #endif
//        case 2 : return turborcndec( in, outlen, out);
      }
    }
      #endif
      #ifdef _ZSTD
    case ICC_ZSTD: ZSTD_decompress( out, outlen, in, inlen); break;
	  #endif
	  
      #ifdef _FSE
    case ICC_FSE:  if(inlen == outlen) memcpy(out, in, outlen); else if(inlen == 1) memset(out,in[0],outlen); else FSE_decompress(out, outlen, in, inlen); break;
      #endif
      #ifdef _FSEHUF
    case ICC_FSEH: if(inlen == outlen) memcpy(out, in, outlen); else if(inlen == 1) memset(out,in[0],outlen); else HUF_decompress(out, outlen, in, inlen); break;
      #endif

      #ifdef _ZLIB
    case ICC_ZLIB: { uLongf outsize = outlen; int rc = uncompress(out, &outsize, in, inlen); } break;
      #endif
	case ICC_MEMCPY: memcpy(out,in, outlen); return inlen;
	default: die("codec '%d' '%s' not implemented\n", codid, codstr(codid) );	
  }
  return 0;
}

//------------ block processing -----------------------------------------------
#define ICCBEG unsigned char *ip = in, *op = out;\
  unsigned iplen = 0;\
  bsize = bsize < inlen?bsize:inlen;\
  for(; ip < in + inlen; ip += iplen) {\
    iplen = (in+inlen) - ip;\
	iplen = iplen>bsize?bsize:iplen;\
	if(op+iplen > out+outsize) { ctou32(out) = (unsigned)-1; memcpy(out+4, in, inlen); return inlen+4; }

#define ICCEND if(op >= out+inlen) { ctou32(out) = (unsigned)-1; memcpy(out+4, in, inlen); return inlen+4; } }

#define ICDBEG unsigned char *ip = in, *op = out;\
  unsigned oplen = 0;\
  if(ctou32(in) == (unsigned)-1) { memcpy(out, in+4, outlen); return inlen-4; }\
  bsize = bsize < outlen?bsize:outlen;\
  for(; op < out + outlen; op += oplen) {\
	oplen = (out+outlen)-op;\
	oplen = oplen>bsize?bsize:oplen;
	
#define ICDEND }

//------------------------------------------------ TurboByte (SIMD Varint) -> codec (lz, entropy coding, bwt...) ----------------------------
//-- TurboByte -> codec
unsigned lzv8enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned clen = 0;
  switch(esize) {
    case 1: clen = inlen; memcpy(tmp, in, inlen); break;
    case 2: clen = v8enc16(in, inlen/2, tmp) - tmp; break;
	case 4: clen = v8enc32(in, inlen/4, tmp) - tmp; break;
	case 8: clen = vbenc64(in, inlen/8, tmp) - tmp; break;
  }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lzv8dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned clen = ctou32(in); 
  codecdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  switch(esize) {
    case 1: memcpy( out, tmp, outlen);   break;
    case 2: v8dec16(tmp, outlen/2, out); break;
	case 4: v8dec32(tmp, outlen/4, out); break;
  	case 8: vbdec64(tmp, outlen/8, out); break;
}
  return inlen;
}

// TurboByte xor -> codec
unsigned lzv8xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) { // TurboVByte
  unsigned clen=0; 
    switch(esize) {
    case 1: clen = inlen; memcpy(tmp, in, inlen); break;
    case 2: clen = v8xenc16(in, inlen/2, tmp, 0) - tmp; break;
	case 4: clen = v8xenc32(in, inlen/4, tmp, 0) - tmp; break;
	case 8: clen = vbxenc64(in, inlen/8, tmp, 0) - tmp; break;
  }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lzv8xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned clen = ctou32(in); codecdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  switch(esize) {
    case 1: memcpy(out, tmp, outlen);   break;
    case 2: v8xdec16(tmp, outlen/2, out, 0); break;
	case 4: v8xdec32(tmp, outlen/4, out, 0); break;
	case 8: vbxdec64(tmp, outlen/8, out, 0); break;
  }
  return inlen;
}

// TurboByte zigzag -> codec
unsigned lzv8zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned clen = 0;
  switch(esize) {
    case 1: clen = inlen; memcpy(tmp, in, inlen); break;
    case 2: clen = v8zenc16(in, inlen/2, tmp, 0) - tmp; break;
	case 4: clen = v8zenc32(in, inlen/4, tmp, 0) - tmp; break;
	case 8: clen = vbzenc64(in, inlen/8, tmp, 0) - tmp; break;
  }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lzv8zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned clen = ctou32(in); codecdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  switch(esize) {
    case 1: memcpy( out, tmp, outlen);   break;
    case 2: v8zdec16(tmp, outlen/2, out, 0); break;
	case 4: v8zdec32(tmp, outlen/4, out, 0); break;
	case 8: vbzdec64(tmp, outlen/8, out, 0); break;
  }
  return inlen;
}
//-------------------------------------------- Byte transpose -> codec (lz, entropy coding, bwt,...) ------------------------------------------------------------------------

//-- transpose -> codec
unsigned lztpenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICCBEG;
    tpenc(ip, iplen, tmp, esize);
    unsigned clen = ctou32(op) = codecenc(tmp, iplen, op+4, outsize, codid, codlev, codprm);        //AC(clen > 0 && clen <= iplen, "#lztpenc %u ", clen);
    op += 4 + clen;
  ICCEND;
  return op - out;
}
unsigned lztpdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICDBEG;
    unsigned iplen = ctou32(ip); 
    codecdec(ip+4, iplen, tmp, oplen, codid, codlev, codprm);
    ip += 4 + iplen;
    tpdec(tmp, oplen, op, esize);
  ICDEND;
  return inlen;
}

//-- xor -> transpose -> codec
unsigned lztpxenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) { // XOR
  ICCBEG;
    tpxenc(ip, iplen, tmp, esize);
    unsigned clen = ctou32(op) = codecenc(tmp, iplen, op+4, outsize, codid, codlev, codprm);
    op += 4 + clen;
  ICCEND;
  return op - out;
}

unsigned lztpxdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICDBEG;
    unsigned iplen = ctou32(ip); 
    codecdec(ip+4, iplen, tmp, oplen, codid, codlev, codprm);
    ip += 4+iplen;
    tpxdec(tmp, oplen, op, esize);
  ICDEND;
  return inlen;
}

//-- zigzag -> transpose -> codec
unsigned lztpzenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) { // Zigzag
  ICCBEG;
    tpzenc(ip, iplen, tmp, esize);
    unsigned clen = ctou32(op) = codecenc(tmp, iplen, op+4, outsize, codid, codlev, codprm);
    op += 4 + clen;
  ICCEND;
  return op - out;
}

unsigned lztpzdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICDBEG;
    unsigned iplen = ctou32(ip); 
    codecdec(ip+4, iplen, tmp, oplen, codid, codlev, codprm);
    ip += 4+iplen;
    tpzdec(tmp, oplen, op, esize);
  ICDEND;
  return inlen;
}
//----------------------------------------------- tp4 : Nibble transpose -> codec --------------------------------

//-- transpose Nibble -> codec
unsigned lztp4enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICCBEG;
    tp4enc(ip, iplen, tmp, esize);
    unsigned clen = ctou32(op) = codecenc(tmp, iplen, op+4, outsize, codid, codlev, codprm);
    op += 4 + clen;
  ICCEND;
  return op - out;
}

unsigned lztpd4ec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICDBEG;
    unsigned iplen = ctou32(ip); 
    codecdec(ip+4, iplen, tmp, oplen, codid, codlev, codprm);
    ip += 4 + iplen;
    tp4dec(tmp, oplen, op, esize);
  ICDEND;
  return inlen;
}

//-- xor -> transpose Nibble -> codec
unsigned lztp4xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) { //XOR
  ICCBEG;
    tp4xenc(ip, iplen, tmp, esize);
    unsigned clen = ctou32(op) = codecenc(tmp, iplen, op+4, outsize, codid, codlev, codprm);
    op += 4 + clen;
  ICCEND;
  return op - out;
}

unsigned lztp4xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICDBEG;
    unsigned iplen = ctou32(ip); 
    codecdec(ip+4, iplen, tmp, oplen, codid, codlev, codprm);
    ip += 4 + iplen;
    tp4xdec(tmp, oplen, op, esize);
  ICDEND;
  return inlen;
}

//-- zigzag delta -> transpose Nibble -> codec
unsigned lztp4zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICCBEG;
    tp4zenc(ip, iplen, tmp, esize);
    unsigned clen = ctou32(op) = codecenc(tmp, iplen, op+4, outsize, codid, codlev, codprm);
    op += 4 + clen;
  ICCEND;
  return op - out;
}

unsigned lztp4zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm, unsigned bsize) {
  ICDBEG;
    unsigned iplen = ctou32(ip); 
    codecdec(ip+4, iplen, tmp, oplen, codid, codlev, codprm);
    ip += 4 + iplen;
    tp4zdec(tmp, oplen, op, esize);
  ICDEND;
  return inlen;
}

//-----------------------------------------------------------------------------------------------------------------------------------
static int tpmode;
void tpmodeset(unsigned _tpmode) {
  tpmode = _tpmode;   //printf("tpmode=%d ", tpmode);
}

#define TPENC(in, n, out, esize) tpmode==4?tp4enc(in, n, out, esize):tpenc(in, n, out, esize)   // use nibble transpose
#define TPDEC(in, n, out, esize) tpmode==4?tp4dec(in, n, out, esize):tpdec(in, n, out, esize) 

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
unsigned lztprleenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) { // Zigzag rle
  tpenc(in, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); 
  if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lztprledec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned clen = ctou32(in); 
  codecdec(in+4, inlen-4, out, clen, codid, codlev, codprm); 
  if(clen >= outlen) memcpy(tmp, out, outlen); else trled(out, clen, tmp, outlen);
  tpdec(tmp, outlen, out, esize);
  return inlen;
}

//-- xor -> transpose -> rle -> codec
unsigned lztprlexenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) { // Zigzag rle
  tpxenc(in, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); 
  if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lztprlexdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned clen = ctou32(in); codecdec(in+4, inlen-4, out, clen, codid, codlev, codprm); 
  if(clen >= outlen) memcpy(tmp, out, outlen); else trled(out, clen, tmp, outlen);
  tpxdec(tmp, outlen, out, esize);
  return inlen;
}

//-- zigzag delta -> transpose -> rle -> codec
unsigned lztprlezenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) { // Zigzag rle
  tpzenc(in, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); 
  if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}

unsigned lztprlezdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned clen = ctou32(in); 
  codecdec(in+4, inlen-4, out, clen, codid, codlev, codprm); 
  if(clen >= outlen) memcpy(tmp, out, outlen); else trled(out, clen, tmp, outlen);
  tpzdec(tmp, outlen, out, esize);
  return inlen;
}

//------------------------------------------------------- 2D -------------------------------------------------
// transpose 2D -> transpose (byte/Nibble) -> codec
unsigned lztpd2enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, unsigned char *codprm) { 
  tp2denc(     in,  x, y,  out, esize);                            
  TPENC(       out, inlen, tmp, esize);                           
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpd2dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, unsigned char *codprm) {
  codecdec(  in,  inlen,  out, outlen, codid, codlev, codprm); 
  TPDEC(  out, outlen, tmp, esize); 
  tp2ddec(tmp, x, y,   out, esize); 
  return x*y*esize;
}

// transpose 2D -> xor -> transpose (byte/Nibble) -> codec
unsigned lztpd2xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, unsigned char *codprm) { 
  tp2denc( in,  x, y,  out, esize);
  tpxenc(  out, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpd2xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, unsigned char *codprm) {
  codecdec(  in,  inlen,  out, outlen, codid, codlev, codprm);
  tpxdec(  out, outlen, tmp, esize);
  tp2ddec(tmp, x, y,   out, esize);
  return inlen;
}

// transpose 2D -> zigzag -> transpose (byte/Nibble) -> codec
unsigned lztpd2zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, unsigned char *codprm) { 
 
  tp2denc(      in,  x, y, out, esize);
  tpzenc(       out, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpd2zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, unsigned char *codprm) {
  codecdec( in,  inlen, out, outlen, codid, codlev, codprm);
  tpzdec(  out, outlen, tmp, esize); //  bitzdec(tmp, outlen, esize);
  tp2ddec(tmp, x, y,   out, esize);
  return inlen;
}

//--------------------------------------------------------- 3D ------------------------------------------------------------
// transpose 3D -> transpose (byte/Nibble) -> codec
unsigned lztpd3enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  tp3denc(     in, x, y, z, out, esize);
  TPENC(      out, inlen,  tmp, esize);
  return codecenc(tmp, inlen,  out, outsize, codid, codlev, codprm);
}

unsigned lztpd3dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize,  unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  codecdec(  in,  inlen,   out, outlen, codid, codlev, codprm);
  TPDEC(  out, outlen,  tmp, esize);
  tp3ddec(tmp, x, y, z, out, esize);
  return inlen;
}

// transpose 3D -> xor -> transpose (byte/Nibble) -> codec 
unsigned lztpd3xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  tp3denc(     in, x, y, z, out, esize);//  bitxenc(     tmp, inlen,  out, esize);
  tpxenc(     out, inlen,  tmp, esize);
  return codecenc(tmp, inlen,  out, outsize, codid, codlev, codprm);
}

unsigned lztpd3xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  codecdec(  in,  inlen,   out, outlen, codid, codlev, codprm);
  tpxdec( out, outlen,  tmp, esize);//  bitxdec(tmp, outlen,       esize);
  tp3ddec(tmp, x, y, z, out, esize);
  return inlen;
}

// transpose 3D -> zigzag -> transpose (byte/Nibble) -> codec
unsigned lztpd3zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  tp3denc(     in,  x, y, z, out, esize);//  bitzenc(     tmp, inlen,   out, esize);
  tpzenc(     out, inlen,   tmp, esize);
  return codecenc(tmp, inlen,   out, outsize, codid, codlev, codprm);
}

unsigned lztpd3zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  codecdec(  in, inlen,  out, outlen, codid, codlev, codprm);
  tpzdec(   out, outlen, tmp, esize);//  bitzdec(tmp, outlen, esize);
  tp3ddec(tmp, x, y, z, out, esize);
  return inlen;
}

//--------------------------------------------------------- 4D ------------------------------------------------------------
// transpose 4D -> transpose (byte/Nibble) -> codec
unsigned lztpd4enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  tp4denc(     in, w, x, y, z, out, esize);
  TPENC(       out, inlen,     tmp, esize);
  return codecenc(tmp, inlen,     out, outsize, codid, codlev, codprm);
}

unsigned lztpd4dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize,  unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  codecdec(  in,  inlen,      out, outlen, codid, codlev, codprm);
  TPDEC(  out, outlen,     tmp, esize);
  tp4ddec(tmp, w, x, y, z, out, esize);
  return inlen;
}

// transpose 4D -> xor -> transpose (byte/Nibble) -> codec
unsigned lztpd4xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  tp4denc( in, w, x, y, z, out, esize);
  tpxenc( out, inlen,      tmp, esize);
  return codecenc(tmp, inlen,  out, outsize, codid, codlev, codprm);
}

unsigned lztpd4xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  codecdec(  in,  inlen,      out, outlen, codid, codlev, codprm);
  tpxdec(   out, outlen,     tmp, esize);
  tp4ddec(tmp, w, x, y, z, out, esize);
  return inlen;
}

// transpose 4D -> zigzag -> transpose (byte/Nibble) -> codec
unsigned lztpd4zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
  tp4denc(     in, w, x, y, z, out, esize);
  tpzenc(     out, inlen,      tmp, esize);
  return codecenc(tmp, inlen,  out, outsize, codid, codlev, codprm);
}

unsigned lztpd4zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, unsigned char *codprm) {
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
unsigned lztp1enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  bitshuffle(in, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp1dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  bitunshuffle(tmp, outlen, out, esize);
  return inlen;
}

//-- xor -> bitshuffle -> codec
unsigned lztp1xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  bitxenc(in, inlen, out, esize);
  bitshuffle(out, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp1xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  codecdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  bitunshuffle(tmp, outlen, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

//-- zigzag delta -> bitshuffle -> codec 
unsigned lztp1zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  bitzenc(in, inlen, out, esize);                
  bitshuffle(out, inlen, tmp, esize);
  return codecenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp1zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
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

size_t vlccomp32(unsigned char *_in, size_t _inlen, unsigned char *out, size_t outsize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) { //bitgput32(bw,br, x); bitenormr(bw,br,op_);//bitdnormr(bw,br,bp); bitgget32(bw,br, x);
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

size_t vlcdecomp32(unsigned char *in, size_t inlen, unsigned char *_out, size_t _outlen, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned char *ip = in+4, *bp = in+inlen, *tp = tmp;
  uint32_t      *out = (uint32_t *)_out, *op = out, x=0;
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

size_t vhicomp32(unsigned char *_in, size_t _inlen, unsigned char *out, size_t outsize, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) { //bitgput32(bw,br, x); bitenormr(bw,br,op_);//bitdnormr(bw,br,bp); bitgget32(bw,br, x);
  unsigned char *op = out+4, *tp = tmp, *tmp_ = tmp+_inlen, *bp = tmp_;
  uint32_t      *in = (uint32_t *)_in, *ip;
  INDEC;
  bitedef(bw,br); biteinir(bw,br,bp);
  for(ip = in; ip < in+inlen; ip++) {
	uint32_t x = ip[0];
	bithcput(bw,br,tp,bp,VHI_K,VHI_I,VHI_J,x); bitenormr(bw,br,bp);              if(bp <= tp+8) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; }
  }
  bitflushr(bw,br,bp);
  unsigned l = tmp_ - bp;  
  
  int clen = codecenc(tmp, tp-tmp, op, _inlen-4, codid, codlev, codprm);        if(clen < 0 || op+clen+l >= out+_inlen) { memcpy(out,_in,_inlen); op = out+_inlen; goto e; }
  ctou32(out) = clen; op += clen;                               
  memmove(op, bp, l); op += l;
  e:return op - out;
}

size_t vhidecomp32(unsigned char *in, size_t inlen, unsigned char *_out, size_t _outlen, unsigned char *tmp, int codid, int codlev, unsigned char *codprm) {
  unsigned char *ip = in+4, *bp = in+inlen, *tp = tmp;
  uint32_t      *out = (uint32_t *)_out, *op = out, x=0;
  if(inlen >= _outlen){ memcpy(_out, in, _outlen); return inlen; }
  OUTDEC;
  bitddef(bw, br); bitdinir(bw,br,bp);
  unsigned clen = ctou32(ip-4); 
  codecdec(ip, clen, tmp, outlen, codid, codlev, codprm);
  for(; op != out+(outlen&~(2-1)); op+=2) { 
    bitdnormr(bw,br,bp);
    bithcget(bw,br,tp,bp,VHI_K,VHI_I,VHI_J, op[0]);
    bithcget(bw,br,tp,bp,VHI_K,VHI_I,VHI_J, op[1]);
  }
  for(; op != out+outlen; op++) { 
    bitdnormr(bw,br,bp);
    bithcget(bw,br,tp,bp,VHI_K,VHI_I,VHI_J, op[0]);
  }
  return inlen;
}
