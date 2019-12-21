//------------------- LZ compression --------------------------------------------------
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
  P_LAST,
};

char *_codstr[] = { "none", "lzturbo", "lz4", "zlib", "zstd", "fse", "fsehuf", "turboanx", "turborc", NULL };
char *codstr(unsigned id) { return (id < P_LAST)?_codstr[id]:""; }

  #ifdef LZTURBO
#include "lzt.c"
#define LZTANS
#include "../lz/ans.h"
#include "../lz/anst.h"
#include "ext/TurboRC/turborc.h"
  #endif

  #if defined(LZ4)
#include "ext/lz4/lib/lz4.h"
#include "ext/lz4/lib/lz4hc.h"
#include "ext/lz4/lib/lz4frame.h"
#define USE_LZ
  #endif

  #if defined(TURBORC)
#include "ext/TurboRC/turborc.h"
  #endif

  #if defined(ZSTD)
#define ZSTD_STATIC_LINKING_ONLY
#include "ext/zstd/lib/zstd.h"
#define USE_LZ
  #endif

  #if defined(ZLIB)
#include "ext/zlib/zlib.h" //#include <zlib.h>

#define USE_LZ
  #endif

int lzenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, int codid, int codlev, char *codprm) { if(!inlen) return 0;
  switch(codid) {
      #if defined(LZTURBO)
        #include "lztc.c"
     #endif

      #if defined(LZ4)
    case P_LZ4: if(codlev > 16) codlev = 16;
      if(strchr(codprm,'M')) { return !codlev?LZ4_compress_fast((char *)in, (char *)out, inlen, outsize, 4):(codlev<9?LZ4_compress_default((char *)in, (char *)out, inlen, outsize):LZ4_compress_HC((char *)in, (char *)out, inlen, outsize, codlev)); }
      else { char *q;
         LZ4F_preferences_t opts = LZ4F_INIT_PREFERENCES;
                              opts.compressionLevel      = codlev;
        if(strchr(codprm,'s'))   opts.favorDecSpeed         = 1;
        if(strchr(codprm,'f'))   opts.autoFlush             = 1;
                              opts.frameInfo.blockSizeID = LZ4F_max4MB;
        if(q=strchr(codprm,'B')) opts.frameInfo.blockSizeID = (LZ4F_blockSizeID_t)atoi(q+(q[1]=='='?2:1));
        if(opts.frameInfo.blockSizeID>LZ4F_max4MB) opts.frameInfo.blockSizeID=LZ4F_max4MB;
        else if(opts.frameInfo.blockSizeID && opts.frameInfo.blockSizeID<LZ4F_default) opts.frameInfo.blockSizeID=LZ4F_default;
        return LZ4F_compressFrame(out, outsize, in, inlen, &opts);
      }
      #endif

      #if defined(TURBORC)
    case P_TURBORC: { int ec = 0; char *q; if(q=strchr(codprm,'e')) ec = atoi(q+(q[1]=='='?2:1));
      switch(codlev) {
        case 0 : return ec==2?turborcsxenc( in, inlen, out):turborcsenc( in, inlen, out);
        case 1 : return ec==2?turborcssxenc(in, inlen, out):turborcssenc(in, inlen, out);
        case 2 : return turborcnenc( in, inlen, out);
      }
    }
      #endif

      #if defined(ZSTD)
    case P_ZSTD:{ ZSTD_CCtx    *ctx = ZSTD_createCCtx();
                   ZSTD_parameters p = ZSTD_getParams(codlev, inlen, 0);
      if(dsize) { int windowLog = bsr32(dsize)-powof2(dsize); ZSTD_CCtx_setParameter(ctx, ZSTD_c_windowLog, windowLog); }
      unsigned rc = ZSTD_compress_advanced(ctx, out, outsize, in, inlen, NULL, 0, p);
      ZSTD_freeCCtx(ctx);
      return rc;
    }
    case P_FSE: { size_t o = FSE_compress(out, outsize, in, inlen);
                   if(o == 1) { out[0] = in[0]; return 1; }
                   if(!o || o >= inlen) { memcpy(out, in, inlen); return inlen; }
                   return o;
                }
    case P_FSEH: { size_t o = HUF_compress(out, outsize, in, inlen);
      if(o == 1) { out[0] = in[0]; return 1; }
      if(!o || o >= inlen) { memcpy(out, in, inlen); return inlen; }
      return o;
    }
      #endif

      #if defined(ZLIB)
    case P_ZLIB: { uLongf outlen = outsize;                 if(codlev < 1) codlev=1; if(codlev>9) codlev=9;
      int rc = compress2(out, &outlen, in, inlen, codlev);
      if(rc != Z_OK) printf("zlib compress2 rc=%d\n", rc);
      return outlen;
    }
      #endif
  }
  return 0;
}

int lzdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, int codec, int codlev, char *codprm) { if(!inlen) return 0;
  switch(codid) {
      #if defined(LZTURBO)
    #include "lztd.c"
      #endif

      #if defined(LZ4)
    case P_LZ4:
      if(strchr(codprm,'M')) { LZ4_decompress_safe((const char *)in, (char *)out, inlen, outlen); break; }
      else {
        LZ4F_dctx *ctx; LZ4F_createDecompressionContext(&ctx, LZ4F_VERSION);
        size_t ilen = inlen, olen = outlen, rc = LZ4F_decompress(ctx, out, &olen, in, &ilen, NULL);
        LZ4F_freeDecompressionContext(ctx);
        return rc;
      }
      #endif

      #if defined(TURBORC)
    case P_TURBORC: { int ec = 0; char *q; if(q=strchr(codprm,'e')) ec = atoi(q+(q[1]=='='?2:1));
      switch(codlev) {
        case 0 : return ec==2?turborcsxdec( in, outlen, out):turborcsdec(in, outlen, out);
        case 1 : return ec==2?turborcssxdec(in, outlen, out):turborcssdec( in, outlen, out);
        case 2 : return turborcndec( in, outlen, out);
      }
    }
      #endif

      #if defined(ZSTD)
    case P_ZSTD: ZSTD_decompress( out, outlen, in, inlen); break;
    case P_FSE:  if(inlen == outlen) memcpy(out, in, outlen); else if(inlen == 1) memset(out,in[0],outlen); else FSE_decompress(out, outlen, in, inlen); break;
    case P_FSEH: if(inlen == outlen) memcpy(out, in, outlen); else if(inlen == 1) memset(out,in[0],outlen); else HUF_decompress(out, outlen, in, inlen); break;
      #endif

      #if defined(ZLIB  )
    case P_ZLIB: { uLongf outsize = outlen; int rc = uncompress(out, &outsize, in, inlen); } break;
      #endif
  }
  return 0;
}

//------------------- TurboByte + lz ----------------------------
unsigned lzv8enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  unsigned clen = (esize==2?v8enc16(in, inlen/2, tmp):v8enc32(in, inlen/4, tmp)) - tmp; //streamvbyte_encode(in, inlen/4, tmp);//
  ctou32(out) = clen;
  return lzenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}
unsigned lzv8dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); lzdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  esize==2?v8dec16(tmp, outlen/2, out):v8dec32(tmp, outlen/4, out);//streamvbyte_decode(tmp, outlen/4, out);
  return inlen;
}

unsigned lzv8zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  unsigned clen = (esize==2?v8zenc16(in, inlen/2, tmp,0):v8zenc32(in, inlen/4, tmp,0)) - tmp; ctou32(out) = clen;
  return lzenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}
unsigned lzv8zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); lzdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  esize==2?v8zdec16(tmp, outlen/2, out,0):v8zdec32(tmp, outlen/4, out,0);
  return inlen;
}

unsigned lzv8xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { // TurboVByte
  unsigned clen = (esize==2?v8xenc16(in, inlen/2, tmp,0):v8xenc32(in, inlen/4, tmp,0)) - tmp; ctou32(out) = clen;
  return lzenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}
unsigned v8lzxdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); lzdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  esize==2?v8xdec16(tmp, outlen/2, out,0):v8xdec32(tmp, outlen/4, out,0);
  return inlen;
}

//----------------- Byte transpose  + Lz -------------------------------------------------------------------------------------
unsigned lztpenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  tpenc(in, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tpdec(tmp, outlen, out, esize);
  return inlen;
}

unsigned lztpxenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { // XOR
  bitxenc(in, inlen, out, esize);
  tpenc(out, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpxdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tpdec(tmp, outlen, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

unsigned lztpzenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { // Zigzag
  bitzenc(in, inlen, out, esize);
  tpenc(out, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpzdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tpdec(  tmp, outlen, out, esize);
  bitzdec(out, outlen, esize);
  return inlen;
}

unsigned lztprleenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { // Zigzag rle
  tpenc(in, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return lzenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}
unsigned lztprledec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); lzdec(in+4, inlen-4, out, clen, codid, codlev, codprm); if(clen >= outlen) { memcpy(tmp, out, outlen); } else
  trled(out, clen, tmp, outlen);
  tpdec(tmp, outlen, out, esize);
  return inlen;
}

unsigned lztprlexenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { // Zigzag rle
  bitxenc(in, inlen, tmp, esize);
  tpenc(tmp, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return lzenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}
unsigned lztprlexdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); lzdec(in+4, inlen-4, out, clen, codid, codlev, codprm); if(clen >= outlen) { memcpy(tmp, out, outlen); } else
  trled(out, clen, tmp, outlen);
  tpdec(tmp, outlen, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

unsigned lztprlezenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { // Zigzag rle
  bitzenc(in, inlen, tmp, esize);
  tpenc(tmp, inlen, out, esize);
  unsigned clen = trlec(out, inlen, tmp); if(clen >= inlen) { clen = inlen; memcpy(tmp,out,inlen); }
  ctou32(out) = clen;
  return lzenc(tmp, clen, out+4, outsize, codid, codlev, codprm)+4;
}
unsigned lztprlezdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  unsigned clen = ctou32(in); lzdec(in+4, inlen-4, out, clen, codid, codlev, codprm); if(clen >= outlen) { memcpy(tmp, out, outlen); } else
  trled(out, clen, tmp, outlen);
  tpdec(tmp, outlen, out, esize);
  bitzdec(out, outlen, esize);
  return inlen;
}

//------------------- tp4 : Nibble transpose + lz --------------------------------
unsigned lztp4enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  tp4enc(in, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd4ec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4dec(tmp, outlen, out, esize);
  return inlen;
}

unsigned lztp4xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) { //XOR
  bitxenc(in, inlen, out, esize);
  tp4enc(out, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztp4xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4dec( tmp, outlen, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

unsigned lztp4zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  bitzenc(in, inlen, out, esize);
  tp4enc(out, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztp4zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4dec(tmp, outlen, out, esize);
  bitzdec(out, outlen, esize);
  return inlen;
}

//------------------ 2D -----------------
unsigned lztpd2enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y) {
  tp2denc(in, x, y, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd2dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp2ddec(tmp, x, y, out, esize);
  return x*y*esize;
}

unsigned lztpd2xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y) {
  bitxenc(in, inlen, out, esize);
  tp2denc(out, x, y, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd2xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp2ddec(tmp, x, y, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

unsigned lztpd2zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y) {
  bitzenc(in, inlen, out, esize);
  tp2denc(out, x, y, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}

unsigned lztpd2zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp2ddec(tmp, x, y, out, esize);
  bitzdec(out, outlen, esize);
  return inlen;
}

//------------------ 3D -----------------
unsigned lztpd3enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z) {
  tp3denc(in, x, y, z, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd3dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize,  unsigned char *tmp, unsigned x, unsigned y, unsigned z) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp3ddec(tmp, x, y, z, out, esize);
  return inlen;
}

unsigned lztpd3xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z) {
  bitxenc(in, inlen, out, esize);
  tp3denc(out, x, y, z, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd3xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp3ddec(tmp, x, y, z, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

unsigned lztpd3zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z) {
  bitzenc(in, inlen, out, esize);
  tp3denc(out, x, y, z, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd3zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp3ddec(tmp, x, y, z, out, esize);
  bitzdec(out, outlen, esize);
  return inlen;
}

//------------------ 4D -----------------
unsigned lztpd4enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z) {
  tp4denc(in, w, x, y, z, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd4dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize,  unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4ddec(tmp, w, x, y, z, out, esize);
  return inlen;
}

unsigned lztpd4xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z) {
  bitxenc(in, inlen, out, esize);
  tp4denc(out, w, x, y, z, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd4xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4ddec(tmp, w, x, y, z, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

unsigned lztpd4zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z) {
  bitzenc(in, inlen, out, esize);
  tp4denc(out, w, x, y, z, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztpd4zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  tp4ddec(tmp, w, x, y, z, out, esize);
  bitzdec(out, outlen, esize);
  return inlen;
}

  #ifdef BITSHUFFLE //--------------------------bit transpose ----------------------------------------------------------------------
unsigned lztp1enc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  bitshuffle(in, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztp1dec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  bitunshuffle(tmp, outlen, out, esize);
  return inlen;
}

unsigned lztp1xenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  bitxenc(in, inlen, out, esize);
  bitshuffle(out, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztp1xdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  bitunshuffle(tmp, outlen, out, esize);
  bitxdec(out, outlen, esize);
  return inlen;
}

unsigned lztp1zenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp) {
  bitzenc(in, inlen, out, esize);                CPYR(in,inlen,esize,out);
  bitshuffle(out, inlen, tmp, esize);
  return lzenc(tmp, inlen, out, outsize, codid, codlev, codprm);
}
unsigned lztp1zdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize, unsigned char *tmp) {
  lzdec(in, inlen, tmp, outlen, codid, codlev, codprm);
  bitunshuffle(tmp, outlen, out, esize);
  bitzdec(out, outlen, esize);
  return inlen;
}
  #endif
