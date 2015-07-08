/**
    Copyright (C) powturbo 2013-2015
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
//  icbench.c - "Integer Compression" benchmark/test program
#define _LARGEFILE64_SOURCE 1 
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <string.h>
#include <malloc.h> 
#include <stdio.h>
#include <math.h> 
#include <getopt.h>
#include <sys/stat.h>
#include <x86intrin.h>

//#define _ZIGZAG        // switch to zigzag encoding mode. Default = differential encoding
//#define _TRANSFORM     // Transform functions : transpose,zigzag

//#define IC_STATS

//---------------------------------------- Platform ---------------------------
  #ifdef _WIN32
#include <windows.h>
#define sleep(t)    Sleep((t) * 1000)
#define srand48(x)  srand(x)
#define drand48()   ((double)(rand()) / RAND_MAX)
#define __off64_t   _off64_t  
  #endif 
//---------------------------------------- Time -------------------------------
typedef unsigned long long tm_t;
#define TM_TMAX (1ull<<63)

#include <sys/time.h>
#include <unistd.h> // sleep

#define TM_T 1000000.0
static   tm_t tmtime(void)    { struct timeval tm; gettimeofday(&tm, NULL); return (tm_t)tm.tv_sec*1000000ull + tm.tv_usec; }
static   tm_t tminit()        { tm_t t0=tmtime(),ts; while((ts = tmtime())==t0) {}; return ts; }
static double tmsec( tm_t tm) { return (double)tm/1000000.0; }
static double tmmsec(tm_t tm) { return (double)tm/1000.0; }

//--------------------------------------- TurboPFor ----------------------------
#include "vint.h"       
#include "vsimple.h"
#include "bitpack.h"
#include "bitunpack.h"
#include "vp4dc.h"
#include "vp4dd.h"
#include "eliasfano.h"
#include "bitutil.h"
#include "transpose.h"
#include "ext/ext.c"            // external functions for comparison. uncomment if not needed

  #ifdef _ZIGZAG
#define bitdelta32( in, n, pa, start, mode) bitzigzag32( in, n, pa, start)
#define bitundx32(out, n, start, mode) bitunzigzag32(out, n, start)
#define bitd32( in, n, x)  bitz32( in, n, x)
#define bitd132(in, n, x)  bitz32( in, n, x)

#define vbd1enc32(in, n, out, x) vbzenc32(in, n, out, x)
#define vbdenc32( in, n, out, x) vbzenc32(in, n, out, x)
#define vbd1encv32(in, n, out, x) vbzencv32(in, n, out, x)
#define vbdencv32( in, n, out, x) vbzencv32(in, n, out, x)

#define vbd1dec32(in, n, out, x) vbzdec32(in, n, out, x)
#define vbddec32( in, n, out, x) vbzdec32(in, n, out, x)
#define vbd1decv32(in, n, out, x) vbzdecv32(in, n, out, x)
#define vbddecv32( in, n, out, x) vbzdecv32(in, n, out, x)

#define bitd1pack32(in, n, out, x, b) bitzpack32(in, n, out, x, b)
#define bitdpack32( in, n, out, x, b) bitzpack32(in, n, out, x, b)
#define bitd1packv32(in, n, out, x, b) bitzpackv32(in, n, out, x, b)
#define bitdpackv32( in, n, out, x, b) bitzpackv32(in, n, out, x, b)

#define bitd1unpack32(in, n, out, x, b) bitzunpack32(in, n, out, x, b)
#define bitdunpack32( in, n, out, x, b) bitzunpack32(in, n, out, x, b)
#define bitd1unpackv32(in, n, out, x, b) bitzunpackv32(in, n, out, x, b)
#define bitdunpackv32( in, n, out, x, b) bitzunpackv32(in, n, out, x, b)
  #endif
 
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

#define PAD8(__x) (((__x)+7)/8)
unsigned char *_bitunpackx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out           , unsigned b) { unsigned i,k=0; for(i=0; i < n; i++,k+=b ) *out++ = _bitgetx32(in, b, k); return in + PAD8(n*b); }

// direct access functions included for demonstration only. Use the bulk functions instead, if you are decompressing all values
unsigned char *bitf1unpackx32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, int start, unsigned b) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, b, i)+start+i+1; return in + PAD8(n*b); }
unsigned char *bitfunpackx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, int start, unsigned b) { int i; for(i = 0; i < n; i++) out[i] = bitgetx32(in, b, i)+start; return in + PAD8(n*b); }

//-------------------------------------------------------------------------------------
#define BLK_SIZE (64*1024)
unsigned char sbuf[BLK_SIZE*5+64];
 
#define PACK_SIZE 128
//      TurboPFor                  External functions 
enum {  P_CPY,                                                            // copy
        P_VB,                      P_VBL, P_VG8, P_VBP, P_MVB,            // variable byte
        P_EFANO,                                                          // elias fano
        P_PCK, P_PCKR, P_PCKV,     P_SIMDV, P_FOR,                        // bit packing
        P_SV, P_SVANS,             P_S16, P_S64,                          // simple family: , simpleV, simple16, simple-8b
        P_P4D, P_P4DR,             P_OPTP4,                               // PFor, PForDelta
                                   P_LIBFOR,                              // For 
                                   P_LZT10, P_LZT20, P_LZT22,		      // LzTurbo
                                   P_LZ4,                          		  // lz4
                                   P_BSHUF, P_BLZ, P_BLZ4, P_BZLIB,       // https://github.com/Blosc/c-blosc
                                   P_ZLIB1, P_ZLIB2, P_ZLIB3, P_ZLIB4, P_ZLIB5, P_ZLIB6, P_ZLIB7, P_ZLIB8, P_ZLIB9,
        P_TRSP, P_TRSPV,           P_BTSHUF,                              // transform 
        P_ZZAG, P_DELTA, 
        P_MAX 
};

//------------------ random integer array (not sorted) ---------------------------------
unsigned char *beenc(unsigned *__restrict in, size_t n, unsigned char *__restrict out, int id, int b) { int i,xb;
  switch(id&0x3f) {
      //---------- copy ----------------------------------------------------
    case P_CPY:    return u32enc(   in, n, (unsigned *)out);
      // --------- variable byte -------------------------------------------
    case P_VB:     return vbenc32(    in, n, out);
 
    case P_VBL:    return vbyteenc( in, n, (unsigned *)out);
    case P_VBP:    return vbpolyenc(in, n, out);
      #ifdef _MASKEDVBYTE
    case P_MVB:    return out + vbyte_encode(in, n, out);
      #endif
      #ifdef _VARINTG8IU
    case P_VG8:    return vintg8enc(in, n, out);
      #endif 
      // --------- simple family: , simpleV, simple16, simple-8b -----------
    case P_SV:     return vsenc32(  in, n, out);
    case P_S16:    return vs16enc(  in, n, (unsigned *)out); 
    case P_S64:    return vs8benc(  in, n, out); 
      // --------- elias fano ----------------------------------------------
    case P_EFANO:  return out; 
      // --------- PFor ----------------------------------------------------
    case P_P4DR:   return p4denc32( in, n, out);
    case P_P4D:    return n == 128?p4dencv32(in, n, out):p4denc32(in, n, out);
    
    case P_OPTP4:  if(n < 128) return vbyteenc(in, n, (unsigned *)out);
                   else { unsigned tmp[2048]; for(i = 0; i < n; i++) tmp[i] = in[i]; return out + OPT4(tmp, n, (unsigned *)out); }
      // --------- bit packing ---------------------------------------------
    case P_FOR :
    case P_PCKR: 
    case P_PCK:    if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } return bitpack32(in, n, out, b);
    case P_PCKV:   if(b < 0) { BITSIZE32(in, n, b); *out++ = b; } return n != 128?bitpack32(in, n, out, b):bitpackv32(in, n, out, b);
      #ifdef _LIBFOR
    case P_LIBFOR: return out + for_compress_unsorted(in, out, n);
      #endif
                                                                                                                                       
    case P_SIMDV:  if(n < 128) return vbyteenc(in, n, (unsigned *)out); else { if(b < 0) b = maxbits(in), *out++ = b; return simdpackwn(in, n, b, (unsigned *)out); }

      // --------- transform ----------------------------------------
      #ifdef _TRANSFORM
    case P_ZZAG:   bitzigzag32(in, n, (unsigned *)out, 0);      return out + n*4;
    case P_TRSP:   transpose4( (unsigned char *)in, n*4, out);  return out + n*4;
    case P_TRSPV:  transposev4( (unsigned char *)in, n*4, out); return out + n*4;
        #ifdef _BLOSC      
    case P_BSHUF:  shuffle( 4, n*4, (unsigned char *)in, out); return out + n*4;
        #endif
        #ifdef _BTSHUF      
    case P_BTSHUF: bshuf_bitshuffle(in, out, n*4/32, 32, 0); return out + n*4;	//bitshuffle(4, const size_t blocksize, in, out, NULL);      
        #endif
      #endif

      // --------- transpose + lz77 ----------------------------------------
      #ifdef _LZT
    case P_LZT10: { n *= 4; transposev4( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 0; lz.hbits = 16; return out + lz8c01(&lz); }
    case P_LZT20: { n *= 4; transposev4( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 0; lz.hbits = 16; return out + lzbc01(&lz); }
    case P_LZT22: { n *= 4; transposev4( (unsigned char *)in, n, sbuf); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 2; lz.hbits = 26; return out + lzbc2(&lz); }     
      #endif
      #ifdef _LZ4
    case P_LZ4:    //bshuf_bitshuffle(in, sbuf, n*4/32, 32, 0);// 
      transposev4( (unsigned char *)in, n*4, sbuf);
      return out + LZ4_compress((char *)sbuf, (char *)out, n*4);
      #endif
      #ifdef _BLOSC
    case P_BLZ:
    case P_BLZ4:
    case P_BZLIB:  return out + blosc_compress(1/*clevel*/, 1/*doshuffle*/, 4/*typesize*/, n*4, in, out, n*4+BLOSC_MAX_OVERHEAD);
      #endif
      #ifdef _ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { n *= 4; transposev4( (unsigned char *)in, n, sbuf); uLongf outlen = n; int rc = compress2(out+4, &outlen, sbuf, n, id-P_ZLIB1+1); if(rc != Z_OK) die("zlib compress2 rc=%d\n", rc); *(unsigned *)out = outlen; return out + 4 + outlen; }
      #endif
    case P_MAX ... 63: die("Fatal- Not entry %d", id);
  }
  return out;
}

unsigned char *bedec(unsigned char *__restrict in, size_t n, unsigned *__restrict out, int id, int b) {
  switch(id&0x3f) {         
       //--------- copy ---------------------------------------------------
    case P_CPY:    return u32dec(   (unsigned *)in, n, out);
      // --------- variable byte -------------------------------------------
    case P_VB:     return vbdec32(    in, n, out); 

    case P_VBL:    return vbytedec( in, n, out); 
    case P_VBP:    return vbpolydec(in, n, out);
      #ifdef _MASKEDVBYTE
    case P_MVB:    return in + masked_vbyte_decode(in, out, n); 
      #endif
      #ifdef _VARINTG8IU
    case P_VG8:    return vintg8dec(in, n, out);
      #endif
      // --------- simple family: simple16, simpleV, simple-8b ---------------
    case P_SV:     return vsdec32(  in, n, out); 

    case P_S16:    return vs16dec(  (unsigned *)in, n, out);  
    case P_S64:    return vs8bdec(  in, n, out);  
      // --------- elias fano -----------------------------------------------
    case P_EFANO:  return in;
      // --------- PFor -----------------------------------------------------
    case P_P4D :   return n == 128?p4ddecv32(in, n, out):p4ddec32(in, n, out);
    case P_P4DR :  return p4ddecx32( in, n, out);
    case P_OPTP4 : if(n < 128) return vbytedec(in, n, out); else { unsigned all_array[2048]; return (unsigned char *)detailed_p4_decode(out, (unsigned *)in, all_array); }
      // --------- bit packing -------------------------------------------
    case P_FOR:    if(b < 0) b = *in++; return bitfunpack32(        in, n, out, 0, b);
    case P_PCKR:   if(b < 0) b = *in++; return _bitunpackx32(       in, n, out, b);
    case P_PCK:    if(b < 0) b = *in++; return bitunpack32(         in, n, out, b);
    case P_PCKV:   if(b < 0) b = *in++; return n != 128?bitunpack32(in, n, out, b):bitunpackv32(in, n, out, b);

    case P_SIMDV:  if(n < 128) return vbytedec(in, n, out); else { if(b < 0) b = *in++; return simdunpackn( (unsigned *)in, n, b, out); }
      #ifdef _LIBFOR
    case P_LIBFOR:    return in + for_uncompress(in, out, n);
      #endif

      //---------- transpose + lz77 ----------------------
      #ifdef _TRANSFORM
    case P_ZZAG:  memcpy(out, in, n*4); bitunzigzag32(out, n, 0); return in + n*4;
    case P_TRSP:  untranspose4(  (unsigned char *)in, n*4, (unsigned char *)out); return in + n*4;
    case P_TRSPV: untransposev4( (unsigned char *)in, n*4, (unsigned char *)out); return in + n*4;
        #ifdef _BLOSC
    case P_BSHUF:  unshuffle( 4, n*4, (unsigned char *)in, (unsigned char *)out); return in + n*4;
        #endif
        #ifdef _BTSHUF      
    case P_BTSHUF:  bshuf_bitunshuffle(in, out, n*4/32, 32, 0); return in + n*4;
        #endif
      #endif

      //---------- transpose + lz77 ----------------------
      #ifdef _LZT
    case P_LZT10:  { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lz8d(&lz); untransposev4(sbuf, n*4, (unsigned char *)out); } break;
    case P_LZT20: 
    case P_LZT22:  { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzbd(&lz); untransposev4(sbuf, n*4, (unsigned char *)out); } break;
      #endif
      #ifdef _LZ4
    case P_LZ4:     in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4); //bshuf_bitunshuffle(sbuf, out, n*4/32, 32, 0); 
       untransposev4(sbuf, n*4, (unsigned char *)out); 
       break;
      #endif
      #ifdef _BLOSC
    case P_BLZ:
    case P_BLZ4:
    case P_BZLIB: { blosc_decompress(in, out, n*4); size_t nbytes, cbytes,blocksize; blosc_cbuffer_sizes(in, &nbytes, &cbytes, &blocksize); return in+cbytes; }
      #endif
      #ifdef _ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { uLongf outsize = n*4; int l = *(unsigned *)in, rc = uncompress(sbuf, &outsize, in+4, l); in += 4 + l; untransposev4(sbuf, n*4, (unsigned char *)out); } break;
      #endif
    case P_MAX ... 63: die("Fatal- Not entry %d", id);
  }
  return in;        
} 

//-------------- Sorted integer array : Delta/Differential compression (mode=0 increasing, mode=1 strictly increasing sequence) ---------------
unsigned char *besenc(unsigned *__restrict in, size_t n, unsigned char *__restrict out, int id, int mode) {
  unsigned pa[BLK_SIZE+2048],x; unsigned b;
  switch(id&0x3f) {
      //----------- copy ---------------------------------------------------------------------------------------------------------
    case P_CPY:                                                                             return u32enc( in, n, (unsigned *)out);
      //----------- variable byte ------------------------------------------------------------------------------------------------     
    case P_VB: 										                                        return mode?vbd1enc32(in, n, out, -1):vbdenc32(in, n, out, 0);
      #ifndef _ZIGZAG
        #ifdef _MASKEDVBYTE
    case P_MVB:                                                                             return out+vbyte_encode_delta(in, n, out, 0);     
        #endif
      #endif
    case P_VBL:       bitdelta32( in, n, pa, -mode, mode);                                  return vbyteenc( pa, n, (unsigned *)out); 
    case P_VBP:       bitdelta32( in, n, pa, -mode, mode);                                  return vbpolyenc(pa, n, out);
      #ifdef _VARINTG8IU
    case P_VG8:       bitdelta32( in, n, pa, -mode, mode);                                  return vintg8enc(pa, n, out);
      #endif
      // --------- simple family: simpleV, simple16, simple-8b -------------------------------------------------------------------
    case P_SV:        bitdelta32( in+1, --n, pa, in[0], mode); vbput32(out, in[0]);         return vsenc32(  pa, n, out);

    case P_S16:   b = bitdelta32( in+1, --n, pa, in[0], mode); vbput32(out, in[0]);                                                     if(b>28) die("simple16 overflow.bits size>28\n");
                                                                                            return vs16enc(  pa, n, (unsigned *)out); 
    case P_S64:   b = bitdelta32( in+1, --n, pa, in[0], mode); vbput32(out, in[0]);                                                     if(b>28) die("simple-8b overflow.bits size>28\n");
                                                                                            return vs8benc(  pa, n, out);
      #ifndef _ZIGZAG
      // --------- elias fano -----------------------------------------------
    case P_EFANO: x = *in++;                                   vbput32(out, x); --n;        
      if(mode) {  return n == 128?efano1encv32(in, n, out, x+1):efano1enc32(in, n, out, x+1); }
      else {      return n == 128?efanoencv32( in, n, out, x  ):efanoenc32( in, n, out, x  ); }
      #endif
      // --------- PForDelta: TurboPFor, TurboPForDA, OptPFD ---------------------------------------------------------------------
    case P_P4D:       bitdelta32( in+1, --n, pa, in[0], mode); vbput32(out, in[0]);         return n==128?p4dencv32(pa, n, out):p4denc32(pa, n, out);
      #ifndef _ZIGZAG
    case P_P4DR:  DELTR( in, n, mode, pa);                     vbput32(out, in[0]);         return p4denc32(  pa+1, n-1, out);
    case P_OPTP4: b = bitdelta32( in+1, --n, pa, in[0], mode); vbput32(out, in[0]);                                                     if(b>28) die("optp4 overflow.bits size>28\n");
      if(n < 128)                                                                           return vbyteenc(  pa, n, (unsigned *)out);
      else {                                                                                return out + OPT4(pa, n, (unsigned *)out); }
      // --------- delta bit packing: TurboFor, TurboForDA, TurboPack, TurboPackV, SimdPackFPF --------------------------------------------
    case P_FOR :  
    case P_PCKR:  if(mode) { b = bitf132(in+1, --n, in[0]);    vbput32(out, in[0]); *out++=b; return bitf1pack32(in+1, n, out, in[0], b); }
                  else {     b = bitf32( in+1, --n, in[0]);    vbput32(out, in[0]); *out++=b; return bitfpack32( in+1, n, out, in[0], b); }      
      #endif
    case P_PCK:   if(mode) { b = bitd132(in+1, --n, in[0]);    vbput32(out, in[0]); *out++=b; return bitd1pack32(in+1, n, out, in[0], b); }
                  else {     b = bitd32( in+1, --n, in[0]);    vbput32(out, in[0]); *out++=b; return bitdpack32( in+1, n, out, in[0], b); } 
    case P_PCKV:  
      if(mode) {  b = bitd132(in+1, --n, in[0]);               vbput32(out, in[0]); *out++=b; return n < 128?bitd1pack32(in+1, n, out, in[0], b):bitd1packv32(in+1, n, out, in[0], b); }
      else {      b = bitd32( in+1, --n, in[0]);               vbput32(out, in[0]); *out++=b; return n < 128?bitdpack32( in+1, n, out, in[0], b):bitdpackv32( in+1, n, out, in[0], b); }
      
      #ifndef _ZIGZAG
    case P_SIMDV:                                                                                   
      if(n < 129) { bitdelta32( in, n, pa, -mode, mode);                                    return vbyteenc((unsigned *)pa, n, (unsigned *)out); }
      else { b = simdmaxbitsd1(in[0], in+1);                   vbput32(out, in[0]); *out++=b; return simdpackwn1((unsigned *)(in+1), n-1, b, in[0], (unsigned *)out); }   
        #ifdef _LIBFOR
    case P_LIBFOR:   return out + for_compress_sorted(in, out, n);
        #endif
      #endif

      // --------- transpose + lz77 ------------------------------------------------------------------------------------------------
      #ifdef _TRANSFORM
    case P_ZZAG:  b = bitzigzag32(in, n, (unsigned *)out,  0); return out + n*4;
    case P_TRSP:  bitdelta32(in, n, (unsigned *)sbuf, -mode, mode); transpose4( (unsigned char *)sbuf, n*4, out); return out + n*4;
    case P_TRSPV: bitdelta32(in, n, (unsigned *)sbuf, -mode, mode); transposev4( (unsigned char *)sbuf, n*4, out); return out + n*4;
    case P_DELTA: bitdelta32(in, n, (unsigned *)out,  -mode, mode); return out + n*4; 
      #endif

      // --------- delta + transpose + lz77 ----------------------------------------------------------------------------------------
      #ifdef _LZT
    case P_LZT10:{ bitdelta32(in, n, (unsigned *)out, -mode, mode); transposev4((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 0; lz.hbits = 16; return out + lz8c01(&lz); }
    case P_LZT20:{ bitdelta32(in, n, (unsigned *)out, -mode, mode); transposev4((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 0; lz.hbits = 16; return out + lzbc01(&lz); }
    case P_LZT22:{ bitdelta32(in, n, (unsigned *)out, -mode, mode); transposev4((unsigned char *)out, n*4, sbuf); struct lzobj lz; lz.srclen = n*4; lz.src = sbuf; lz.dst = out; lz.dstlen = n*4; lz.level = 2; lz.hbits = 26; return out + lzbc2(&lz);  }
      #endif

      #ifdef _LZ4
    case P_LZ4: { bitdelta32(in, n, (unsigned *)out, -mode, mode); transposev4((unsigned char *)out, n*4, sbuf); //  bshuf_bitshuffle(out, sbuf, n*4/32, 32, 0);
      return out + LZ4_compress((char *)sbuf, (char *)out, n*4); }
      #endif
      #ifdef _ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { bitdelta32(in, n, (unsigned *)out, -mode, mode); transposev4((unsigned char *)out, n*4, sbuf); uLongf outlen = n*4; int rc = compress2(out+4, &outlen, sbuf, n*4, id-P_ZLIB1+1); if(rc != Z_OK) die("zlib compress2 rc=%d\n", rc); *(unsigned *)out = outlen; return out + 4 + outlen; }
      #endif
    case P_MAX ... 63: break;
  }
  return out;
} 
 
unsigned char *besdec(unsigned char *__restrict in, size_t n, unsigned *__restrict out, int id, int mode) { unsigned b,x,v;
  switch(id&0x3f) { 
      //------------- copy -------------------------------------------------------
    case P_CPY:                              in = u32dec( (unsigned *)in, n,   out);                                 break;
      //------------- variable byte ---------------------------------------------- 
    case P_VB: 								  		  return mode?vbd1dec32(     in, n, out,   -1):vbddec32(in, n, out, 0);
      #ifndef _ZIGZAG
        #ifdef _MASKEDVBYTE
    case P_MVB:                              in += masked_vbyte_decode_delta(in, out,   n, 0);                       break;
        #endif
      #endif

    case P_VBL:                              in = vbytedec(           in, n,   out);          bitundx32(out, n, -mode, mode); break;
    case P_VBP:                              in = vbpolydec(          in, n,   out);          bitundx32(out, n, -mode, mode); break;
      #ifdef _VARINTG8IU
    case P_VG8:                              in = vintg8dec(          in, n,   out);          bitundx32(out, n, -mode, mode); break;    
      #endif 
      //------------- simple family ----------------------------------------------
      #ifdef _ZIGZAG
    case P_SV:    _vbget32(in, x, *out = x); in = vsdec32(            in, n-1, out+1);      bitundx32(out+1, n-1, x, mode); break;
      #else
    case P_SV:    _vbget32(in, x, *out = x); in = vsdec32(            in, n-1, out+1);      bitundx32(out, n, -mode, mode); break;
      #endif
    case P_S16:   _vbget32(in, x, *out = x); in = vs16dec((unsigned *)in, n-1, out+1);      bitundx32(out, n, -mode, mode); break;
    case P_S64:   _vbget32(in, x, *out = x); in = vs8bdec(            in, n-1, out+1);      bitundx32(out, n, -mode, mode); break;
      // ------------ elias fano -------------------------------------------------
      #ifndef _ZIGZAG
    case P_EFANO: _vbget32(in, x,;); *out++ = x; --n;
      if(mode) {                                      return n==128?efano1decv32(in, n, out, x+1):efano1dec32( in, n, out, x+1); }
      else {                                          return n==128?efanodecv32( in, n, out, x  ):efanodec32( in, n, out, x); }
      #endif
      // ------------ PFor -------------------------------------------------------

      #ifdef _ZIGZAG
    case P_P4D:   _vbget32(in, x, *out = x); in = n==129?p4ddecv32(in, n-1, out+1):p4ddec32(in, n-1, out+1); bitundx32(out+1, n-1, x, mode); break;
      #else
    case P_P4D:   _vbget32(in, x, ;); *out++ = x; --n; 
      if(mode) {                                      return n==128?p4dd1decv32(in, n, out, x):p4dd1dec32(in, n, out, x); }
      else {                                          return n==128?p4dddecv32( in, n, out, x):p4dddec32( in, n, out, x); }
    case P_P4DR:  _vbget32(in, x, *out = x);          return mode?p4dfdecx32(  in, n-1, out+1, x):p4df0decx32(in, n-1, out+1, x);
    case P_OPTP4: 
      if(n < 129)                          in = vbytedec(in, n, out);
      else {      _vbget32(in, x, *out = x); unsigned all_array[2048]; in = (unsigned char *)detailed_p4_decode(out+1, (unsigned *)in, all_array); }
                                                                                            bitundx32(out, n, -mode, mode); break;
      // ------------ bit packing ----------------------------------------
    case P_FOR:   _vbget32(in, x, *out = x); b = *in++; return mode?bitf1unpack32( in, n-1, out+1, x, b):bitfunpack32(  in, n-1, out+1, x, b);
    case P_PCKR:  _vbget32(in, x, *out = x); b = *in++; return mode?bitf1unpackx32(in, n-1, out+1, x, b):bitfunpackx32( in, n-1, out+1, x, b);
      #endif
      
    case P_PCK:   _vbget32(in, x, *out = x); b = *in++; return mode?bitd1unpack32( in, n-1, out+1, x, b):bitdunpack32(  in, n-1, out+1, x, b);
    case P_PCKV:  _vbget32(in, x, *out = x); b = *in++;
      if(n < 129) {                                     return mode?bitd1unpack32( in, n-1, out+1, x, b):bitdunpack32(  in, n-1, out+1, x, b); } 
      else {                                            return mode?bitd1unpackv32(in, n-1, out+1, x, b):bitdunpackv32( in, n-1, out+1, x, b); } 
 
      #ifndef _ZIGZAG
    case P_SIMDV:
      if(n < 129) { in = vbytedec(in, n, out);               bitundx32(out, n, -mode, mode); }
      else {    _vbget32(in, x, *out = x); b = *in++; in = simdunpackn1((uint32_t *)in, n-1, b, out[0], out+1); } break;
        #ifdef _LIBFOR
    case P_LIBFOR:    return in + for_uncompress(in, out, n);
        #endif
      #endif 

      //---------- transpose + lz77 ----------------------
      #ifdef _TRANSFORM
    case P_ZZAG:  memcpy(out, in, n*4); bitunzigzag32(out, n, 0); return in + n*4;
    case P_TRSP:  untranspose4( (unsigned char *)in, n*4, (unsigned char *)out); bitundx32(out, n, -mode, mode); return in + n*4;
    case P_TRSPV: untransposev4((unsigned char *)in, n*4, (unsigned char *)out); bitundx32(out, n, -mode, mode); return in + n*4;
    case P_DELTA: memcpy(out, in, n*4); bitundx32(out, n, -mode, mode); return in + n*4;
      #endif

      //---------- delta + transpose + lz77 ----------------------
      #ifdef _LZ4
    case P_LZ4: in += LZ4_decompress_fast((char *)in, (char *)sbuf, n*4); untransposev4(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -mode, mode); break;       
      #endif     

      #ifdef _ZLIB
    case P_ZLIB1: case P_ZLIB2: case P_ZLIB3: case P_ZLIB4: case P_ZLIB5: case P_ZLIB6: case P_ZLIB7: case P_ZLIB8: case P_ZLIB9: 
      { uLongf outsize = n*4; int l = *(unsigned *)in, rc = uncompress(sbuf, &outsize, in+4, l); in += 4 + l; untransposev4(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -mode, mode); } break;
      #endif

      #ifdef _LZT
    case P_LZT10: { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lz8d(&lz);  untransposev4(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -mode, mode); } break;
    case P_LZT20:
    case P_LZT22: { struct lzobj lz; lz.dstlen = n*4; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzbd(&lz);  untransposev4(sbuf, n*4, (unsigned char *)out); bitundx32(out, n, -mode, mode); } break;
      #endif

    case P_MAX ... 63: break;
  } 
  return in;
} 

//--------------------------------------- Zipfian generator --------------------------------------------------------
int dcmp(double *a, double *b) {
  if(*a < *b) return -1;
  if(*a > *b) return  1;
  return 0;
}

void zipfgen(unsigned *a, double alpha, unsigned x1, unsigned x2, int n) {
  int      i; 
  unsigned m = x2 - x1 + 1;
  double   prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error\n"); 

  // generate initial sample (slow)
  srand48(1); 
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++) 
      cum += 1.0 / pow(i+1, alpha); 
    cum = 1.0 / cum;    
    for(zmap[0] = prob = cum,i = 1; i < m; i++) zmap[i] = (prob += (cum / pow(i+1, alpha))); 
  } else for(i = 0; i < m; i++) zmap[i] = 1.0 / m;

  // use binary search to speed up zipfgen
  qsort(zmap, m, sizeof(zmap[0]), (int(*)(const void*,const void*))dcmp); 
  for(i = 0; i < n; i++) { 
    double r = drand48(); 
    int    l = 0, h = m-1; 
    while(l < h) { 
      int k = (l + h) >> 1; 
      if(r >= zmap[k]) l = k + 1; 
      else h = k; 
    }
    a[i] = x1 + l; 
  } 
  free(zmap); 
}
//---------------------------------------- bench --------------------------
#define TM_MAX (1ull<<63)
#define TMPRINT(__x) { printf("%7.2f MB/s\t%s", (double)(tm_tm>=0.000001?(((double)n*tm_rm/MBS)/(((double)tm_tm/1)/TM_T)):0.0), __x); fflush(stdout); }
#define TMDEF unsigned tm_r,tm_R; tm_t _t0,_tc
#define TMBEG for(tm_tm = TM_MAX,tm_R = 0; tm_R < tm_Reps; tm_R++) { if(tm_R && tm_slp) sleep(tm_slp); for(_t0 = tminit(), tm_r=0; tm_r < tm_reps;) {
#define TMEND                                            tm_r++; if((_tc = (tmtime() - _t0)) > tm_tx) break; } if(_tc < tm_tm) { tm_tm = _tc,tm_rm=tm_r; if(_tc>tm_Tx) break; } }
#define MBS 1000000.0 //MiBS 1048576.0
static unsigned tm_reps = 1<<30, tm_Reps = 3, tm_rm, tm_slp;
static tm_t tm_tm, tm_tx=2*TM_T,tm_Tx=60*TM_T;

void memrcpy(unsigned char *__restrict out, unsigned char *__restrict in, size_t n) { int i; for(i = 0; i < n; i++) out[i] = ~in[i]; }
unsigned argtoi(char *s) {
  char *p; unsigned n = strtol(s, &p, 10),f=1;
  switch(*p) {
     case 'k': f = 1000;       break;
     case 'm': f = 1000000;    break;
     case 'g': f = 1000000000; break;
     case 'K': f = 1<<10;      break;
     case 'M': f = 1<<20;      break;
     case 'G': f = 1<<30;      break;
  }
  return n*f;
}
#define ALGN

  #ifdef ALGN
static void *amalloc(size_t size, size_t align) {
    #if defined(__MINGW32__)
  return __mingw_aligned_malloc(size, align);
    #elif defined(_WIN32)
  return _aligned_malloc(__size, __align);
    #elif _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
  void *p; 
  return posix_memalign(&p, align, size)?NULL:p;
    #elif __STDC_VERSION__ >= 201112L
  return aligned_alloc(16, size);
    #else
  return malloc(size);
    #endif
}

void afree(void *p) {
    #if defined(__MINGW32__)
  __mingw_aligned_free(p);
    #elif defined(_WIN32)
  _aligned_free(p);
    #elif _POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600
  free(p);
    #elif __STDC_VERSION__ >= 201112L
  aligned_free(p);
    #else
  free(size);
    #endif
}
  #endif
//----------------------------------------------- Benchmark -------------------
struct libss { int id; char *s;int size; };

struct libss libss[] = {            
  //------- PFor + PForDelta ----------
  { P_P4D,    "TurboPFor",   128    }, 
    #if !defined(_WIN32)
  { P_P4DR,   "TurboPForDA", 128    }, // actually not working w. mingw 
    #endif
    
    #ifdef _OPTPFD
  { P_OPTP4,  "OptPFD",      128    }, //max. 28 bits
    #endif

  //-------------- Bit Packing --------
  { P_PCKV,   "TurboPackV",  128    },
  { P_PCK,    "TurboPack",   PACK_SIZE  },
  { P_FOR,    "TurboFor",    PACK_SIZE  },
  { P_PCKR,   "TurboForDA",  PACK_SIZE  },
  
  { P_SIMDV,  "SIMDPackFPF", 128    }, 
    #ifdef _LIBFOR
  { P_LIBFOR, "LibFor",     PACK_SIZE },
    #endif

  //------ Variable byte -------------
  { P_VB,     "TurboVbyte"          },
  
  { P_VBL,    "VbyteFPF"            },
  { P_VG8,    "VarintG8IU"          }, 
    #ifdef _MASKEDVBYTE
  { P_MVB,    "MaskedVByte"         }, 
    #endif
    #ifdef _VBYTEPOLY
  { P_VBP,    "VBytePoly"           },
    #endif

  // ----- Simple family -----
  { P_SV,     "VSimple"             },
//  { P_SVANS,      "VSimpleANS", BLK_SIZE },
    #ifdef _SIMPLE_8B
  { P_S64,    "Simple-8b",  SIMPLE8BMAX },  //crash on integers w. size 32 bits !
    #endif
    #ifdef _SIMPLE16
  { P_S16,    "Simple16"            },  //max. 28 bits
    #endif

  //------- Elias Fano ---------
  { P_EFANO,  "EliasFano"           },
  { P_CPY,    "Copy"                },

  // ------ transpose/delta + lz77 -------
    #ifdef _LZT
  { P_LZT10,  "LzTurbo 10", BLK_SIZE },
//  { P_LZT20,  "LzTurbo 20", BLK_SIZE },
//  { P_LZT22,  "LzTurbo 22", BLK_SIZE },
    #endif     

    #ifdef _LZ4
  { P_LZ4,    "lz4",        BLK_SIZE },
    #endif 

    #ifdef _BLOSC
  { P_BLZ,    "blosc_lz",   BLK_SIZE },
  { P_BLZ4,   "blosc_lz4",  BLK_SIZE },
  { P_BZLIB,  "blosc_zlib", BLK_SIZE },
    #endif 

    #ifdef _ZLIB
  { P_ZLIB1,  "zlib 1",     BLK_SIZE },
//{ P_ZLIB2,  "zlib 2",     BLK_SIZE },
//{ P_ZLIB3,  "zlib 3",     BLK_SIZE },
//{ P_ZLIB4,  "zlib 4",     BLK_SIZE },
  { P_ZLIB5,  "zlib 5",     BLK_SIZE },
//{ P_ZLIB6,  "zlib 6",     BLK_SIZE },
//{ P_ZLIB7,  "zlib 7",     BLK_SIZE },
//{ P_ZLIB8,  "zlib 8",     BLK_SIZE },
  { P_ZLIB9,  "zlib 9",     BLK_SIZE },
    #endif 

  //------- Transform -------------------
    #ifdef _TRANSFORM
  { P_TRSP,   "transpose",  BLK_SIZE },
  { P_TRSPV,  "transposev", BLK_SIZE },
  { P_ZZAG,   "zigzag",     BLK_SIZE },
  { P_DELTA,  "delta",      BLK_SIZE },
      #ifdef _BLOSC
  { P_BSHUF,  "shuffle",    BLK_SIZE },
      #endif 
      #ifdef _BTSHUF
  { P_BTSHUF, "bshuffle",   BLK_SIZE },
      #endif 
    #endif
  { -1,       ""                     },
};
  
#define MB 1000000
int verb = 0, xcheck=2;                                                                                 unsigned xbits[33];
          
enum { T_TST, T_TXT, T_CHAR, T_BYTE, T_BYTE2, T_BYTE4, T_DBL };

struct libs { int id,err,size; char *s,*v; unsigned long long l, c[33]; double tc,td; };
struct libs libs[64],slibs[64];
void libini() { int m; for(m = 0; libs[m].id >= 0; m++) libs[m].l = libs[m].tc = libs[m].td = 0; }

int l_cmp(struct libs *a, struct libs *b) {
  if(a->l  < b->l || (a->l == b->l && a->td < b->td) || (a->l == b->l && a->td == b->td && a->tc < b->tc)) return -1;
  if(a->l  > b->l || (a->l == b->l && a->td > b->td) || (a->l == b->l && a->td == b->td && a->tc > b->tc)) return  1;
  return 0;
}

void check(unsigned *in, unsigned n, unsigned *out, char *s) {
  unsigned k,j; 
  for(k = 0; k < n; k++) 
    if(in[k] != out[k]) { 
      printf("\nFATAL in check %x,%x at %u[%u] in %s\n", in[k], out[k], k, n, s); 
      for(j=k & 0xffffff80u; j < k+128;j++) { unsigned e = in[j] != out[j];
        if(e) printf("*"); printf("%d:%x,%x ", j, in[j], out[j] );
      }
      printf("\n");
      exit(0); 
    } 
} 

void stprint(char *s) {
  int m;
  unsigned long long t=0; 
  for(m = 0; m < 33; m++) 
    t += xbits[m];
  printf("\n%s bits histogram:",s); 
  for(m = 0; m < 33; m++) 
    if(xbits[m]) printf("%d:%.2f%% ", m, (double)xbits[m]*100/t); printf("\n");
}

#define BI 1   // BI=4 -> MB/S=Megabyte/Sec  BI=1 -> Millions integer/Sec
void print(struct libs *libs, unsigned long long n, char *s, unsigned long long *u) { 
  int m, k; 
  for(k = 0; libs[k].id >= 0; k++) {};
    qsort(libs, k, sizeof(libs[0]), (int(*)(const void*,const void*))l_cmp);    
  char *prtname = s?s:""; { char *p; if((p = strrchr(prtname, '/')) || (p = strrchr(prtname, '\\'))) prtname = p+1;} 
  for(m = 0; m < k; m++) 
    if(/*libs[m].tc ||*/ libs[m].l) { 
      struct libs *lb = &libs[m]; if(!lb->l) lb->tc=lb->td=0.0;
      printf("%-16s%12llu\t%5.2f\t%5.2f\t%8.2f\t%8.2f\t%s\n", prtname, lb->l, (double)lb->l*100.0/((double)n*4.0), (double)lb->l*8.0/(double)n, 
        lb->tc>=0.000001?((double)n*BI/1000000.0) / (lb->tc/TM_T):0.0,
        lb->td>=0.000001?((double)n*BI/1000000.0) / (lb->td/TM_T):0.0,
        lb->s ); 
    }
}

//-----------------------------------------------------------------------------------------------
unsigned bench(unsigned *__restrict _in, unsigned _inlen, int blksize, unsigned char *__restrict _out, unsigned long long outsize, char *inname, tm_t tx, unsigned *__restrict cpy, int bb, int mode ) { int m,id,b=bb,i; 
  if(!_inlen) return 0;                                                 if(verb>1) { printf(":%d,", _inlen);  fflush(stdout); }
  unsigned cn;                                                          
  memcpy(_out, _in, (unsigned long long)_inlen*4);                                                                                                                                          
  for(m = 0; (id=libs[m].id) >= 0; m++) {
    unsigned    bsize = libs[m].size?libs[m].size:blksize, cl;
    int         insize=(mode>=0)?bsize+1:bsize; 
    struct libs *lb = &libs[m];                                                                         if(verb) printf("%s,%d", libs[m].s, insize);
      #ifdef _BLOSC
    if(lb->id == P_BLZ4)
      blosc_set_compressor(BLOSC_LZ4_COMPNAME);
    else if(lb->id == P_BZLIB)
      blosc_set_compressor(BLOSC_ZLIB_COMPNAME);
    else if(lb->id == P_BLZ)
      blosc_set_compressor(BLOSC_BLOSCLZ_COMPNAME);
      #endif
                  memrcpy((unsigned char *)_out, (unsigned char *)_in, _inlen*4);
    if(cpy !=_in) memrcpy((unsigned char *)cpy,  (unsigned char *)_in, _inlen*4);
    
    TMDEF; TMBEG
    cn = cl = 0;                                                
    unsigned      *in;
    unsigned char *out;                                          
    for(out = _out, in = _in; in < _in+_inlen; ) {
      unsigned n, inlen = *in++, *ip = in; in += inlen; cn += inlen;
      *(unsigned *)out = inlen; out += 4; unsigned char *sout = out;
      if(mode >= 0)
        for(;ip < in; ip += n) { n = in - ip; n = min(n,insize);      
          out = besenc(ip, n, out, id, mode);                           
        }                                                           
      else
        for(;ip < in; ip += n) { n = in - ip; n = min(n,insize);      
          out = beenc(ip, n, out, id, bb);  
        }                                                             
      if(out > _out+outsize) die("Overflow error %llu, %u in %s\n", outsize, (int)(ptrdiff_t)(out - _out), lb->s);      
      cl += out - sout;                                                                                 
    }
    TMEND   																						 if(verb) { printf("/");fflush(stdout); }   	
    lb->l += cl; lb->tc += (double)tm_tm/tm_rm;                	//printf("(%d %.2f,%2u) ", cl, (double)tm_tm, tm_rm);                                           
    
    if(xcheck) {
      TMDEF; TMBEG   
      unsigned *out; 
      unsigned char *in;
      for(out = cpy, in = _out; out < cpy+_inlen;) {
        unsigned n,*op, outlen = *(unsigned *)in; in += 4;
        *out++ = outlen;                                          
        if(mode >= 0)
          for(op=out,out += outlen; op < out; op += n) { n = out - op; n = min(n,insize); 
            in = besdec(in, n, op, id, mode);
          }
        else
          for(op=out,out += outlen; op < out; op += n) { n = out - op; n = min(n,insize);   
            in = bedec(in, n, op, id, bb);
          }
      }
      TMEND  
      lb->td += (double)tm_tm/tm_rm;       						//printf("(%d %.2f,%2u) ", cl, (double)tm_tm, tm_rm); 										printf("(%d,%d %.2f,%.2f) ", cl, lb->l, lb->tc, lb->td);                                                                      
      if(xcheck > 1 && lb->l) check(_in, _inlen, cpy, lb->s);
    }
  }
  return cn;
}

void usage() {
  fprintf(stderr, "\nTurboPFor Copyright (c) 2013-2015 Powturbo  %s\n", __DATE__);
  fprintf(stderr, "Usage: icbench [options] [file]\n");
  fprintf(stderr, "Use zipfian generator when no file specified\n");
  fprintf(stderr, "<options>\n");
  fprintf(stderr, " -bNs    N = blocksize (default 128) ex. -b64k -b64K\n");
  fprintf(stderr, " -cN     N = 0,1 delta for ordered integer lists)/9,10,11=1 integer per line/19,20,21:integers separated by non digit char\n");
  fprintf(stderr, " -eS     S = encoder schemes sparated by / (default all)\n");
  fprintf(stderr, " -vN     N = verbosity 1..3\n");
  fprintf(stderr, " -tN     N = time in seconds per interation\n");
  fprintf(stderr, " -RN     N = Iterations (default 3)\n");
  fprintf(stderr, "----- file specified --------------\n");
  fprintf(stderr, " -fN     N = file format: 1=text:one integer per line 2=text:integers separated by non digit char, 3=bytes\n");
  fprintf(stderr, " -FNs    N = max. file size to read\n");
  fprintf(stderr, "Ex. ./icbench -c1 gov2.sorted -eturbopack/turbopfor\n");
  fprintf(stderr, "----- file not specified --------------\n");
  fprintf(stderr, " -fN     N = 0:bits range test. Ex. ./icbench -f0 -n0 -M32 -ebitpack\n");
  fprintf(stderr, " -aF     F = zipfian distribution alpha ex. -a1.0 uniform -a1.5 skewed\n");
  fprintf(stderr, " -mNs    N = minimum integer generated. s=(k,K, m,M, g,G\n");
  fprintf(stderr, " -MNs    N = maximum integer generated\n");
  fprintf(stderr, " -nNs    N = number of integers to generate\n");
  fprintf(stderr, "Ex. ./icbench -a1.5 -m0 -M255 -n100m\n");
  fprintf(stderr, "         s = modifier s:k,m,g=(1000,1 million,1 billion) s:K,M,G=(1024,1MB,1GB) ex. 64k or 64K\n");
  exit(0);
}

#define TEST64
  #ifdef TEST64
#define R64 ((unsigned long long)rand()) 
#define RND64 ( (R64<<60) ^ (R64<<45) ^ (R64<<30) ^ (R64<<15) ^ (R64<<0) )
#include <stdint.h>
  #define NN (4*1024*1024)
  uint64_t in[NN],cpy[NN]; 
  unsigned char out[NN*9];
void vstest64(int id, int rm,int rx, unsigned n) { fprintf(stderr,"bitpack.n=%d ", n);   
  unsigned b,i;                                                            
  for(b = rm; b <= rx; b++) {                                           fprintf(stderr,"\nb=%d:", b);        
    uint64_t start = 0;
    for(i = 0; i < n; i++) 
      in[i] = (/*start +=*/ RND64 & (b==64?0xffffffffffffffffull:((1ull << b)-1)));//fprintf(stderr, ".%llx ", in[0]); 
    
    unsigned char *op;
    switch(id) { 
      case 0: op = vbenc64(   in, n, out);    break;
      case 1: op = vsenc64(   in, n, out);    break;
      case 2: op = bitpack64( in, n, out, b); break;
      case 3: op = p4denc64(  in, n, out);    break;
      case 4: op = efanoenc64(in, n, out, 0); break;
    }
    fprintf(stderr,"%d ", (int)(op-out) );     
    memset(cpy, 0, sizeof(cpy));
    switch(id) {
      case 0: vbdec64(    out, n, cpy);      break;
      case 1: vsdec64(    out, n, cpy);      break;
      case 2: bitunpack64(out, n, cpy, b);   break;
      case 3: p4ddec64(   out, n, cpy);      break;
      case 4: efanodec64( out, n, cpy, 0);   break;
    }
    for(i = 0; i < n; i++) 
      if(in[i] != cpy[i]) {
        fprintf(stderr, "Error b=%d at '%d'", b, i); break;
      } 
  }
  exit(0);
}
  #else
#define vstest64(id,rm,rx,n)
  #endif

#define OVD (10*MB)
int main(int argc, char *argv[]) { int r; 
  char      fname[0x100], *cmd=NULL;
  unsigned  rm=0,rx=1<<29,n=0;
  int       mode = -1,fmt = -1;
  long long rdmax = 1ull<<32; 
  double    a = 1.5;
  tm_t      tx=1*1000000; 
  unsigned  blksize = PACK_SIZE;
  tminit();
    #ifdef _VARINTG8IU
  VarIntG8IU(); 
    #endif
    #ifdef _MASKEDVBYTE
  simdvbyteinit();  
    #endif
    #ifdef _BLOSC
  blosc_init();
  blosc_set_nthreads(1);
    #endif
    #ifdef _LZT
  anscset(0); ansdset(0);
    #endif
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"repeat",    0, 0, 'r'}, {0,0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "Bsha:b:c:e:f:F:H:m:n:r:R:S:t:T:X:v:M:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;                                
      case 'a': a = strtod(optarg, NULL);   break;
      case 'b': blksize = argtoi(optarg); if(blksize>BLK_SIZE) blksize = BLK_SIZE; break;
      case 'c': mode    = atoi(optarg);     break;
      case 'f': fmt     = atoi(optarg);     break;
      case 'F': rdmax   = argtoi(optarg);   break;
      case 'h': usage();                    break;
      case 'H': xcheck  = atoi(optarg);     break;
      case 'e': cmd = optarg;               break;
      case 'v': verb    = atoi(optarg);     break;
      
      case 'S': tm_slp  = atoi(optarg);     break;
      case 't': tm_tx   = atoi(optarg)*TM_T;break;
      case 'T': tm_Tx   = atoi(optarg)*TM_T;break;
      case 'r': tm_reps = atoi(optarg);     break;
      case 'R': tm_Reps = atoi(optarg);     break;
      
      case 'n': n       = argtoi(optarg);   break;
      case 'm': rm      = argtoi(optarg);   break;
      case 'M': rx      = argtoi(optarg);   break;
      case 'X': vstest64(atoi(optarg),rm,rx,n); break;
      default:  usage();
    }
  }                                                                                                                     
  int fno,i=0;
  if(!tm_reps) tm_reps=tm_Reps=1;                                                   //printf("range=(min=%u, max=%u)\n", rm, rx);fflush(stdout);
  // build the test functions set
  struct libss *ls; 
  if(cmd) {                     
    char *q = NULL;
    for(i=0,libs[0].id = -1;;) { 
      if(cmd) { 
        if(!*cmd) break;                            
        q = strchr(cmd,','); 
        if(q) *q=' '; 
        if((q = strchr(cmd,'/')) != NULL)
          *q = '\0';
        for(ls = libss; ls->id >= 0; ls++) 
          if(!strcasecmp(ls->s, cmd)) { 
            memset(&libs[i], 0, sizeof(struct libs)); 
            libs[i].id     = ls->id; 
            libs[i].err    = 0; 
            libs[i].s      = ls->s; 
            libs[i++].size = ls->size;                                      if(verb) printf("%s/", ls->s);fflush(stdout);
            break; 
          }
        if(ls->id < 0) die("library: '%s' not found\n", cmd); 
        cmd = q?(q+1):"";
      }
    }
  } else for(ls = libss; ls->id >= 0; ls++) { 
    libs[i].id     = ls->id; 
    libs[i].err    = 0; 
    libs[i].s      = ls->s;                                                 if(verb) printf("%s/", ls->s);fflush(stdout);
    libs[i++].size = ls->size;    
  } 
  libs[i].id = -1;                                                          if(verb) printf("\n");
  unsigned long long totlen = 0; 
  memcpy(slibs,libs,sizeof(slibs));
  if(argc <= optind) {                                                      // No file specified at commandline
    unsigned      *in, *cpy,*ip; 
    unsigned char *out; 
    
    if(!n) n = 100000000;
      #ifdef ALGN
    unsigned *_cpy,*_in; unsigned char *_out;  
    _in  = amalloc(n*4+OVD,64);     if(!_in)  die("malloc err=%u", n); in =_in +0;
    _out = amalloc(n*5+OVD,64);     if(!_out) die("malloc err=%u", n); out=_out+12;
    _cpy = amalloc(n*4+OVD,64);     if(!_cpy) die("malloc err=%u", n); cpy=_cpy+3; 
      #else
    in  = malloc(n*4+OVD); if(!in)  die("malloc err=%u", n);
    out = malloc(n*5+OVD); if(!out) die("malloc err=%u", n);
    cpy = malloc(n*4+OVD); if(!cpy) die("malloc err=%u", n);
      #endif
    char s[33]; 
    s[0] = 0; 
    if(fmt == T_TST) {                                                      // Unit test for fixed bit sizes
      unsigned b;                                                           printf("bittest: %u-%u\n", rm, rx); fflush(stdout);
      for(b = rm; b <= max(rx,32); b++) {                                           
        libini();                                                           sprintf(s,"b=%d", b);        
        for(*in = n,i = 1; i <= n; i++) 
          in[i] = (1ull << b)-1;
        totlen = bench(in, n+1, blksize, out, n*5+OVD, s, tx, cpy, b, 
            #ifdef _ZIGZAG
          mode
            #else
          -1
            #endif
					  );  													print(libs,totlen, s, NULL);
      }
    } else {                                                                // Benchmark w. generated data
                                                                            printf("zipf alpha=%.2f range[%u..%u].n=%u\n ", a, rm, rx, n);
      *in = n; 
      zipfgen(in+1, a, rm, rx, n);                                          for(i = 1; i <= n; i++) xbits[bsr32(in[i])]++; 
      if(mode >= 0) {                                                       stprint("delta"); 
        unsigned *ip = in+1; int v;
        for(ip[0]=0,v = 1; v < n; v++) { 
          ip[v] += ip[v-1] + mode;                                          if(ip[v]>=(1u<<31)) die("overflow generating sorted array\n" );  
        }
      } else stprint("");     
      totlen = bench(in, n+1, blksize, out, n*5+OVD, s, tx, cpy, -1, mode);
      print(libs,totlen, s, NULL); 
    }
      #ifdef ALGN
    afree(_in); afree(_cpy); afree(_out);
      #else
    free(in); free(cpy); free(out);
      #endif
  } else for(fno = optind; fno < argc; fno++) {                             // Benchmark w. specified data files
    libini();
    char *inname = argv[fno];
    if(fmt >= T_TXT) { //------------ convert text file to integer array format
      FILE *fi = fopen(inname, "r");   if(!fi) { fprintf(stderr, "open error '%s'", inname); perror(inname); exit(-1); }
      unsigned      *in = NULL, *cpy,*ip, nmax = 0;
      unsigned char *out; 

      n = 1;
      #define LSIZE 1024
      char     s[LSIZE+1]; 
      switch(fmt) {
        case T_TXT: {
          while(fgets(s, LSIZE, fi)) {
          s[strlen(s) - 1] = 0;
          unsigned u = strtoul(s, NULL, 10);
          if(n >= nmax) {  
            nmax = nmax?(nmax << 1):(1<<20);
            in = realloc(in, nmax*4+OVD); if(!in) die("malloc err=%u", nmax);
          }
          in[n++] = u;
          }
        } break;
        case T_CHAR:
          for(;;) {
            char *p = s;
            int c;
            while((c = getc(fi)) >= '0' && c <= '9') if(p - s < LSIZE) *p++ = c;
            *p = 0;
            unsigned u = strtoul(s, NULL, 10);  //printf("%d,",u);
            if(n >= nmax) {
              nmax = nmax?(nmax << 1):(1<<20);
              in = realloc(in, nmax*4+OVD); if(!in) die("malloc err=%u", nmax);
            }
            in[n++] = u;
            if(c == EOF) break;
          }
          break;
        case T_BYTE: {
          unsigned char c;
          while(fread(&c, 1, 1, fi)>0) {
            if(n >= nmax) {
              nmax = nmax?(nmax << 1):(1<<20);
              in = realloc(in, nmax*4+OVD); if(!in)  die("malloc err=%u", nmax);
            }
            in[n++] = c;
          }
        } break;
        case T_BYTE2: {
          unsigned short c;
          while(fread(&c, sizeof(c), 1, fi)>0) {
            if(n >= nmax) {
              nmax = nmax?(nmax << 1):(1<<20);
              in = realloc(in, nmax*4+OVD); if(!in)  die("malloc err=%u", nmax);
            }
            in[n++] = c;
          }          
        } break;
        case T_BYTE4: {
          unsigned c;
          while(fread(&c, sizeof(c), 1, fi)>0) { //printf("%d,", c);
            if(n >= nmax) {
              nmax = nmax?(nmax << 1):(1<<20);
              in = realloc(in, nmax*4+OVD); if(!in)  die("malloc err=%u", nmax);
            }
            in[n++] = c; 
          }
        } break;
        case T_DBL: { //Test floating pint decomposition
          double c,*din = NULL; n=0;
          
          while(fread(&c, 8, 1, fi)>0) { printf("%e\n", c);
            if(!din || n >= nmax) {
              nmax = nmax?(nmax << 1):(1<<20);
              din = realloc(din, nmax*sizeof(c)+OVD); if(!din)  die("malloc err=%u", nmax);
            }
            din[n++] = c; 
          }
          double *dcpy = malloc(n*sizeof(dcpy[0]));
          uint64_t *mantissa = malloc(n*sizeof(mantissa[0]));
          unsigned *sign     = malloc(n*sizeof(sign[0]));
          unsigned *exp      = malloc(n*sizeof(exp[0]));   if(!mantissa || !exp || !sign || !dcpy) die("alloc error\n");
          bitdouble(   din, n, sign, exp, mantissa);
          bitundouble(         sign, exp, mantissa, n, dcpy);
          int i; for(i=0;i < n; i++) { printf("%d,%d,%llu,%e,%e\n", sign[i], exp[i],(long long unsigned int)mantissa[i], din[i], dcpy[i]); if(din[i]!=dcpy[i]) die("check error at %d %e %e\n", i, din[i], dcpy[i]); }
          free(din); free(mantissa); free(exp); free(sign); free(dcpy);
          exit(0);
        }
        default: die("unkown data format\n");
      }
      fclose(fi);                                                 
      out = malloc(n*5+OVD); if(!out) die("malloc err=%u", n);
      cpy = malloc(n*4+OVD); if(!cpy) die("malloc err=%u", n);
      in[0] = n-1; s[0] = 0; 
      unsigned long long l = bench(in, n, blksize, out, n*5+OVD, s, tx, cpy, -1, mode);
      print(libs, l, inname, NULL);                                             //printf("n=%d.%d\n", n-1,argc);
      int i; for(i=0;libs[i].id>=0;i++) slibs[i].tc += libs[i].tc,slibs[i].td += libs[i].td,slibs[i].l += libs[i].l;
      totlen += l;
      continue; 
    } 
    //------- process integer array file ------------------
    FILE *fi = fopen64(inname, "rb"); 
    if(!fi) { fprintf(stderr, "open error '%s'", inname); perror(inname); exit(-1); } 
    fseeko(fi, 0, SEEK_END); unsigned long long fisize = ftello(fi); fseeko(fi, 0, SEEK_SET);                       //printf("fisize=%llu\n", fisize);
    if(fisize > rdmax) fisize = rdmax; 
    fisize /= 4;            
                
    unsigned *in, *cpy,*ip,num;  
    unsigned char *out; 
    unsigned long long outsize=fisize*5+OVD,totlen=0,bitslen[33]={0}; 
    out = malloc(outsize);        if(!out) die("malloc err=%llu", fisize); 
    cpy = malloc(fisize*4+OVD);   if(!cpy) die("malloc err=%llu", fisize);                                              
    in  = malloc(fisize*4+1024);  if(!in)  die("malloc err=%llu", fisize);  
    
    ip = in;
    while(fread(&num, 1, 4, fi) == 4 && num) {                                                                  
      if(num < rm || num > rx) { fseeko(fi, num*4, SEEK_CUR); continue; }   
      if(ip+num > in+fisize) { 
        totlen += bench(in, ip-in, blksize, out, outsize, inname, tx, cpy, -1, mode); 
        if(n && totlen > n) 
          break;
        ip = in;
      }                                             
      *ip++ = num; if(fread(ip, 4, num, fi) != num) break;  
      bitslen[bsr32(num)] += num*4;
        #ifdef IC_STATS
      unsigned *ep = ip+num,insize=(mode>=0)?blksize+1:blksize;
      while(ip < ep) {
        unsigned m = min(ep-ip, insize),i;
        if(mode >= 0) {
          for(i = 1; i < m; i++) {                              
            if(verb>3) printf(":%u ", ip[i]);fflush(stdout); 
            xbits[bsr32((ip[i] - ip[i-1]) - mode)]++; 
            if(ip[i] < ip[i-1]+mode) die("IDs in '%s' not sorted.[did=%u,%u] at line=%d\n", inname, ip[i], ip[i-1], (int)(ip-in));
          }
        } else for(i = 0; i < m; i++) xbits[bsr32(ip[i])]++;
        ip += m;                                    
      }
        #else
      ip += num;
        #endif
    }
    fclose(fi);
    totlen += bench(in, ip-in, blksize, out, outsize, inname, tx, cpy, -1, mode);   //    printf("N#%u,%u ", (int)(ip-in), (unsigned)(totlen/1000000));
    free(in); free(cpy); free(out);                                                 
      #ifdef IC_STATS
    stprint("delta"); 
      #endif
    print(libs,totlen,inname, bitslen);
  }     
  if(fmt >= T_TXT && (fno-optind)>1) { printf("\n"); print(slibs,totlen, "TOT", NULL);                                                                  }
}
