#include <string.h>
#include "include_/conf.h"
#include "include_/transpose.h"

static unsigned _cpuisa;
//--------------------- CPU detection -------------------------------------------
    #if defined(__i386__) || defined(__x86_64__)
      #if _MSC_VER >=1300
#include <intrin.h>
      #elif defined (__INTEL_COMPILER)
#include <x86intrin.h>
      #endif

static inline void cpuid(int reg[4], int id) {
      #if defined (_MSC_VER) //|| defined (__INTEL_COMPILER)
  __cpuidex(reg, id, 0);
      #elif defined(__i386__) || defined(__x86_64__)
  __asm("cpuid" : "=a"(reg[0]),"=b"(reg[1]),"=c"(reg[2]),"=d"(reg[3]) : "a"(id),"c"(0) : );
      #endif
}

static inline uint64_t xgetbv (int ctr) {
      #if(defined _MSC_VER && (_MSC_FULL_VER >= 160040219) || defined __INTEL_COMPILER)
  return _xgetbv(ctr);
      #elif defined(__i386__) || defined(__x86_64__)
  unsigned a, d;
  __asm("xgetbv" : "=a"(a),"=d"(d) : "c"(ctr) : );
  return (uint64_t)d << 32 | a;
      #else
  unsigned a=0, d=0;
  return (uint64_t)d << 32 | a;
      #endif
}
    #endif

#define AVX512F     0x001
#define AVX512DQ    0x002
#define AVX512IFMA  0x004
#define AVX512PF    0x008
#define AVX512ER    0x010
#define AVX512CD    0x020
#define AVX512BW    0x040
#define AVX512VL    0x080
#define AVX512VNNI  0x100
#define AVX512VBMI  0x200
#define AVX512VBMI2 0x400

#define IS_SSE       0x10
#define IS_SSE2      0x20
#define IS_SSE3      0x30
#define IS_SSSE3     0x32
#define IS_POWER9    0x34 // powerpc
#define IS_NEON      0x38 // arm neon
#define IS_SSE41     0x40
#define IS_SSE41x    0x41 //+popcount
#define IS_SSE42     0x42
#define IS_AVX       0x50
#define IS_AVX2      0x60
#define IS_AVX512    0x800

unsigned cpuisa(void) {
  int c[4] = {0};
  if(_cpuisa) return _cpuisa;
  _cpuisa++;
    #if defined(__i386__) || defined(__x86_64__)
  cpuid(c, 0);
  if(c[0]) {
    cpuid(c, 1);
    //family = ((c >> 8) & 0xf) + ((c >> 20) & 0xff)
    //model  = ((c >> 4) & 0xf) + ((c >> 12) & 0xf0)
    if( c[3] & (1 << 25)) {         _cpuisa  = IS_SSE;
    if( c[3] & (1 << 26)) {         _cpuisa  = IS_SSE2;
    if( c[2] & (1 <<  0)) {         _cpuisa  = IS_SSE3;
      //                            _cpuisa  = IS_SSE3SLOW; // Atom SSSE3 slow
    if( c[2] & (1 <<  9)) {         _cpuisa  = IS_SSSE3;
    if( c[2] & (1 << 19)) {         _cpuisa  = IS_SSE41;
    if( c[2] & (1 << 23)) {         _cpuisa  = IS_SSE41x; // +popcount
    if( c[2] & (1 << 20)) {         _cpuisa  = IS_SSE42;  // SSE4.2
    if((c[2] & (1 << 28)) &&
       (c[2] & (1 << 27)) &&                           // OSXSAVE
       (c[2] & (1 << 26)) &&                           // XSAVE
       (xgetbv(0) & 6)==6) {        _cpuisa  = IS_AVX; // AVX
      if(c[2]& (1 <<  3))           _cpuisa |= 1;      // +FMA3
      if(c[2]& (1 << 16))           _cpuisa |= 2;      // +FMA4
      if(c[2]& (1 << 25))           _cpuisa |= 4;      // +AES
      cpuid(c, 7);
      if(c[1] & (1 << 5)) {         _cpuisa = IS_AVX2;
        if(c[1] & (1 << 16)) {
          cpuid(c, 0xd);
          if((c[0] & 0x60)==0x60) { _cpuisa = IS_AVX512;
            cpuid(c, 7);
            if(c[1] & (1<<16))      _cpuisa |= AVX512F;
            if(c[1] & (1<<17))      _cpuisa |= AVX512DQ;
            if(c[1] & (1<<21))      _cpuisa |= AVX512IFMA;
            if(c[1] & (1<<26))      _cpuisa |= AVX512PF;
            if(c[1] & (1<<27))      _cpuisa |= AVX512ER;
            if(c[1] & (1<<28))      _cpuisa |= AVX512CD;
            if(c[1] & (1<<30))      _cpuisa |= AVX512BW;
            if(c[1] & (1u<<31))     _cpuisa |= AVX512VL;
            if(c[2] & (1<< 1))      _cpuisa |= AVX512VBMI;
            if(c[2] & (1<<11))      _cpuisa |= AVX512VNNI;
            if(c[2] & (1<< 6))      _cpuisa |= AVX512VBMI2;
      }}}
    }}}}}}}}}
    #elif defined(__powerpc64__)
  _cpuisa = IS_POWER9; // power9
    #elif defined(__ARM_NEON)
  _cpuisa = IS_NEON; // ARM_NEON
    #endif
  return _cpuisa;
}

unsigned cpuini(unsigned cpuisa) { if(cpuisa) _cpuisa = cpuisa; return _cpuisa; }

char *cpustr(unsigned cpuisa) {
  if(!cpuisa) cpuisa = _cpuisa;
    #if defined(__i386__) || defined(__x86_64__)
  if(cpuisa >= IS_AVX512) {
    if(cpuisa & AVX512VBMI2) return "avx512vbmi2";
    if(cpuisa & AVX512VBMI)  return "avx512vbmi";
    if(cpuisa & AVX512VNNI)  return "avx512vnni";
    if(cpuisa & AVX512VL)    return "avx512vl";
    if(cpuisa & AVX512BW)    return "avx512bw";
    if(cpuisa & AVX512CD)    return "avx512cd";
    if(cpuisa & AVX512ER)    return "avx512er";
    if(cpuisa & AVX512PF)    return "avx512pf";
    if(cpuisa & AVX512IFMA)  return "avx512ifma";
    if(cpuisa & AVX512DQ)    return "avx512dq";
    if(cpuisa & AVX512F)     return "avx512f";
    return "avx512";
  }
  else if(cpuisa >= IS_AVX2)    return "avx2";
  else if(cpuisa >= IS_AVX)
    switch(cpuisa&0xf) {
      case 1: return "avx+fma3";
      case 2: return "avx+fma4";
      case 4: return "avx+aes";
      case 5: return "avx+fma3+aes";
      default:return "avx";
    }
  else if(cpuisa >= IS_SSE42)   return "sse4.2";
  else if(cpuisa >= IS_SSE41x)  return "sse4.1+popcnt";
  else if(cpuisa >= IS_SSE41)   return "sse4.1";
  else if(cpuisa >= IS_SSSE3)   return "ssse3";
  else if(cpuisa >= IS_SSE3)    return "sse3";
  else if(cpuisa >= IS_SSE2)    return "sse2";
  else if(cpuisa >= IS_SSE)     return "sse";
     #elif defined(__powerpc64__)
  if(cpuisa >= IS_POWER9)       return "power9";
    #elif defined(__ARM_NEON)
  if(cpuisa >= IS_NEON)         return "arm_neon";
    #endif
  return "none";
}

//----------------------------------------------------------------------------------------------------------------------
typedef void (*TPFUNC)( unsigned char *in, unsigned n, unsigned char *out);

                        // 0  1       2       3       4      5  6  7       8   9                    16
static TPFUNC _tpe[]   = { 0, 0, tpenc2,  tpenc3,  tpenc4,   0, 0, 0, tpenc8,  0, 0, 0, 0, 0, 0, 0, tpenc16 }; //  byte
static TPFUNC _tpd[]   = { 0, 0, tpdec2,  tpdec3,  tpdec4,   0, 0, 0, tpdec8,  0, 0, 0, 0, 0, 0, 0, tpdec16 };

static TPFUNC _tp4e[]  = { 0, 0, tpenc2,  tpenc3,  tpenc4,   0, 0, 0, tpenc8,  0, 0, 0, 0, 0, 0, 0, tpenc16 }; // Nibble
static TPFUNC _tp4d[]  = { 0, 0, tpdec2,  tpdec3,  tpdec4,   0, 0, 0, tpdec8,  0, 0, 0, 0, 0, 0, 0, tpdec16 };

//-- zigzag delta
  #ifndef NTP_ZZAG
static TPFUNC _tpze[]  = { 0, 0, tpzenc2, tpzenc3, tpzenc4,  0, 0, 0, tpzenc8, 0, 0, 0, 0, 0, 0, 0, tpzenc16 }; // byte
static TPFUNC _tpzd[]  = { 0, 0, tpzdec2, tpzdec3, tpzdec4,  0, 0, 0, tpzdec8, 0, 0, 0, 0, 0, 0, 0, tpzdec16 };

static TPFUNC _tp4ze[] = { 0, 0, tpzenc2, tpzenc3, tpzenc4,  0, 0, 0, tpzenc8, 0, 0, 0, 0, 0, 0, 0, tpzenc16 }; // Nibble
static TPFUNC _tp4zd[] = { 0, 0, tpzdec2, tpzdec3, tpzdec4,  0, 0, 0, tpzdec8, 0, 0, 0, 0, 0, 0, 0, tpzdec16 };
  #endif

//-- xor 
  #ifndef NTP_XOR
static TPFUNC _tpxe[]  = { 0, 0, tpxenc2, tpxenc3, tpxenc4,  0, 0, 0, tpxenc8, 0, 0, 0, 0, 0, 0, 0, tpxenc16 }; // byte
static TPFUNC _tpxd[]  = { 0, 0, tpxdec2, tpxdec3, tpxdec4,  0, 0, 0, tpxdec8, 0, 0, 0, 0, 0, 0, 0, tpxdec16 };

static TPFUNC _tp4xe[] = { 0, 0, tpxenc2, tpxenc3, tpxenc4,  0, 0, 0, tpxenc8, 0, 0, 0, 0, 0, 0, 0, tpxenc16 }; // Nibble
static TPFUNC _tp4xd[] = { 0, 0, tpxdec2, tpxdec3, tpxdec4,  0, 0, 0, tpxdec8, 0, 0, 0, 0, 0, 0, 0, tpxdec16 };
  #endif

static int tpset;

void tpini(int id) {
  int i;
  if(tpset) return;
  tpset++;
  i = id?id:cpuisa();
    #if defined(__i386__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_IX86)
  if(i >= IS_AVX2) {
    _tpe[ 2] = tpenc256v2;  _tpd[ 2] = tpdec256v2;  _tp4e[ 2] = tp4enc256v2;  _tp4d[ 2] = tp4dec256v2;
    _tpe[ 4] = tpenc256v4;  _tpd[ 4] = tpdec256v4;  _tp4e[ 4] = tp4enc256v4;  _tp4d[ 4] = tp4dec256v4; 
    _tpe[ 8] = tpenc256v8;  _tpd[ 8] = tpdec256v8;  _tp4e[ 8] = tp4enc256v8;  _tp4d[ 8] = tp4dec256v8;

      #ifndef NTP_ZZAG
    _tpze[2] = tpzenc128v2; _tpzd[2] = tpzdec128v2; _tp4ze[2] = tp4zenc128v2; _tp4zd[2] = tp4zdec128v2; // 16 bits: only sse 
    _tpze[4] = tpzenc256v4; _tpzd[4] = tpzdec256v4; _tp4ze[4] = tp4zenc256v4; _tp4zd[4] = tp4zdec256v4; 
    _tpze[8] = tpzenc256v8; _tpzd[8] = tpzdec256v8; _tp4ze[8] = tp4zenc256v8; _tp4zd[8] = tp4zdec256v8;
      #endif

      #ifndef NTP_XOR
    _tpxe[2] = tpxenc128v2; _tpxd[2] = tpxdec128v2; _tp4xe[2] = tp4xenc128v2; _tp4xd[2] = tp4xdec128v2; // 16 bits: only sse 
    _tpxe[4] = tpxenc256v4; _tpxd[4] = tpxdec256v4; _tp4xe[4] = tp4xenc256v4; _tp4xd[4] = tp4xdec256v4; 
    _tpxe[8] = tpxenc256v8; _tpxd[8] = tpxdec256v8; _tp4xe[8] = tp4xenc256v8; _tp4xd[8] = tp4xdec256v8;
      #endif
  } else
    #endif
      #if defined(__i386__) || defined(__x86_64__) || defined(__ARM_NEON) || defined(__powerpc64__) || defined(_M_X64) || defined(_M_IX86)
    if(i >= IS_SSE2) {
      _tpe[ 2] = tpenc128v2;  _tpd[ 2] = tpdec128v2;  _tp4e[ 2] = tp4enc128v2;  _tp4d[ 2] = tp4dec128v2;
      _tpe[ 4] = tpenc128v4;  _tpd[ 4] = tpdec128v4;  _tp4e[ 4] = tp4enc128v4;  _tp4d[ 4] = tp4dec128v4;
      _tpe[ 8] = tpenc128v8;  _tpd[ 8] = tpdec128v8;  _tp4e[ 8] = tp4enc128v8;  _tp4d[ 8] = tp4dec128v8;
      if(i == 35) _tpd[8] = tpdec8; // ARM NEON scalar is faster!, TODO:retest on Apple M1
	  
       #ifndef NTP_ZZAG
      _tpze[2] = tpzenc128v2; _tpzd[2] = tpzdec128v2; _tp4ze[2] = tp4zenc128v2; _tp4zd[2] = tp4zdec128v2;
      _tpze[4] = tpzenc128v4; _tpzd[4] = tpzdec128v4; _tp4ze[4] = tp4zenc128v4; _tp4zd[4] = tp4zdec128v4;
      _tpze[8] = tpzenc128v8; _tpzd[8] = tpzdec128v8; _tp4ze[8] = tp4zenc128v8; _tp4zd[8] = tp4zdec128v8;
      if(i == 35) _tpzd[8] = tpzdec8; 
        #endif

        #ifndef NTP_XOR
      _tpxe[2] = tpxenc128v2; _tpxd[2] = tpxdec128v2; _tp4xe[2] = tp4xenc128v2; _tp4xd[2] = tp4xdec128v2;
      _tpxe[4] = tpxenc128v4; _tpxd[4] = tpxdec128v4; _tp4xe[4] = tp4xenc128v4; _tp4xd[4] = tp4xdec128v4;
      _tpxe[8] = tpxenc128v8; _tpxd[8] = tpxdec128v8; _tp4xe[8] = tp4xenc128v8; _tp4xd[8] = tp4xdec128v8;
      if(i == 35) _tpxd[8] = tpxdec8; 
        #endif
}
      #endif
  ;
}

void tpenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tpe[esize])) f(in,n,out);
  else {
    unsigned i, stride=n/esize;
    unsigned char *op,*ip;
    for(ip = in,op = out; ip < in+stride*esize; op++)
      for(i = 0; i < esize; i++)
        op[i*stride] = *ip++;
    for(op = out + esize*stride; ip < in+n;)
      *op++ = *ip++;
  }
}

void tpdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tpd[esize])) f(in,n,out);
  else {
    unsigned i, stride = n/esize;
    unsigned char *op,*ip;
    for(op = out,ip = in; op < out+stride*esize; ip++)
      for(i = 0; i < esize; i++)
        *op++ = ip[i*stride];
    for(ip = in+esize*stride; op < out+n;)
      *op++ = *ip++;
  }
}

void tp4enc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tp4e[esize])) f(in,n,out);
  else tpenc(in,n,out,esize);
}

void tp4dec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tp4d[esize])) f(in,n,out);
  else tpdec(in,n,out,esize);
}

//-- zigzag 
void tpzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tpze[esize])) f(in,n,out);
  else {
    unsigned i, stride = n/esize;
    unsigned char *op, *ip;
    for(ip = in,op = out; ip < in+stride*esize; op++)
      for(i = 0; i < esize; i++)
        op[i*stride] = *ip++; // TODO:zigzag
    for(op = out + esize*stride; ip < in+n;)
      *op++ = *ip++;          // TODO:zigzag
  }
} 

void tpzdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tpzd[esize])) f(in,n,out);
  else {
    unsigned i, stride = n/esize;
    unsigned char *op,*ip;
    for(op = out,ip = in; op < out+stride*esize; ip++)
      for(i = 0; i < esize; i++)
        *op++ = ip[i*stride];  
    for(ip = in+esize*stride; op < out+n;)
      *op++ = *ip++;           
  }
}

void tp4zenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tp4ze[esize])) f(in,n,out);
  else tpzenc(in,n,out,esize);
}

void tp4zdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tp4zd[esize])) f(in,n,out);
  else tpzdec(in,n,out,esize);
}

//-- xor
void tpxenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tpxe[esize])) f(in,n,out);
  else {
    unsigned i, stride = n/esize;
    unsigned char *op, *ip;
    for(ip = in,op = out; ip < in+stride*esize; op++)
      for(i = 0; i < esize; i++)
        op[i*stride] = *ip++; // TODO:xor
    for(op = out + esize*stride; ip < in+n;)
      *op++ = *ip++;          // TODO:xor
  }
} 

void tpxdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tpxd[esize])) f(in,n,out);
  else {
    unsigned i, stride = n/esize;
    unsigned char *op,*ip;
    for(op = out,ip = in; op < out+stride*esize; ip++)
      for(i = 0; i < esize; i++)
        *op++ = ip[i*stride];
    for(ip = in+esize*stride; op < out+n;)
      *op++ = *ip++;
  }
}

void tp4xenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tp4xe[esize])) f(in,n,out);
  else tpxenc(in,n,out,esize);
}

void tp4xdec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  TPFUNC f;
  if(!tpset) tpini(0);
  if(esize <= 16 && (f = _tp4xd[esize])) f(in,n,out);
  else tpxdec(in,n,out,esize);
}

#define E for(e = esize-1; e >= 0; e--)
#define ODX2 (x + y * nx) * esize + e
void tp2denc(unsigned char *in, unsigned nx, unsigned ny, unsigned char *out, unsigned esize) {
  switch(esize) {
	case 2: tp2denc2(in,nx,ny,out); break;
	case 4: tp2denc4(in,nx,ny,out); break;
	case 8: tp2denc8(in,nx,ny,out); break;
	default: {  
	  unsigned x,y; 
      uint8_t  *op = out, *ip = in; 
      int     e;
      for(  x = 0; x < nx; x++)
        for(y = 0; y < ny; y++) E
          op[ODX2] = *ip++;
    }
  }
}

void tp2ddec(unsigned char *in, unsigned nx, unsigned ny, unsigned char *out, unsigned esize) {
  switch(esize) {
	case 2: tp2ddec2(in,nx,ny,out); break;
	case 4: tp2ddec4(in,nx,ny,out); break;
	case 8: tp2ddec8(in,nx,ny,out); break;
    default: { unsigned x,y; 
      uint8_t *op = out, *ip = in; 
      int     e;
      for(  x = 0; x < nx; x++)
        for(y = 0; y < ny; y++) E
          *op++ = ip[ODX2];
    }
  }
}
#undef ODX2

#define ODX3 (x + y * nx + z * ny * nx) * esize + e
void tp3denc(unsigned char *in, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize) {
  switch(esize) {
	case 2: tp3denc2(in,nx,ny,nz,out); break;
	case 4: tp3denc4(in,nx,ny,nz,out); break;
	case 8: tp3denc8(in,nx,ny,nz,out); break;
	default: {  unsigned x,y,z; 
      uint8_t *op = out, *ip = in; 
      int e;
      for(    x = 0; x < nx; x++)
        for(  y = 0; y < ny; y++)
          for(z = 0; z < nz; z++) E
            op[ODX3] = *ip++;
	}
  }
}

void tp3ddec(unsigned char *in, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize) {
  switch(esize) {
	case 2: tp3ddec2(in,nx,ny,nz,out); break;
	case 4: tp3ddec4(in,nx,ny,nz,out); break;
	case 8: tp3ddec8(in,nx,ny,nz,out); break;
    default: {  unsigned x,y,z;  
      uint8_t *op = out, *ip = in; 
      int e;
      for(x = 0; x < nx; ++x)
        for(y = 0; y < ny; ++y)
          for(z = 0; z < nz; ++z) E
            *op++ = ip[ODX3];
    }
  }
}
#undef ODX3

#define ODX4 (w + x * nw + y * nx * nw + z * nx * ny * nw) * esize + e
void tp4denc(unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize) {
  switch(esize) {
	case 2: tp4denc2(in,nw,nx,ny,nz,out); break;
	case 4: tp4denc4(in,nw,nx,ny,nz,out); break;
	case 8: tp4denc8(in,nw,nx,ny,nz,out); break;
	default: { 
	  unsigned w, x, y, z; 
      uint8_t  *op = out, *ip = in; 
      int      e;
      for(      w = 0; w < nw; w++)
        for(    x = 0; x < nx; x++)
          for(  y = 0; y < ny; y++)
            for(z = 0; z < nz; z++) E
              op[ODX4] = *ip++;
	}
  }
}

void tp4ddec(unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize) {
  switch(esize) {
	case 2: tp4ddec2(in,nw,nx,ny,nz,out); break;
	case 4: tp4ddec4(in,nw,nx,ny,nz,out); break;
	case 8: tp4ddec8(in,nw,nx,ny,nz,out); break;
    default: {    
	  unsigned w,x,y,z; 
      uint8_t *op = out,*ip = in; 
      int e;
      for(      w = 0; w < nw; w++)
        for(    x = 0; x < nx; ++x)
          for(  y = 0; y < ny; ++y)
            for(z = 0; z < nz; ++z) E
              *op++ = ip[ODX4];
    }
  }
}
