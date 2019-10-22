#if defined(_MSC_VER) && _MSC_VER < 1600
#include "../vs/stdint.h"
#else
#include <stdint.h>
#endif

#include "fastpfor.h"
#include "FastPFor/headers/variablebyte.h"
#include "FastPFor/headers/simple16.h"
//#include "FastPFor/headers/simple8b_rle.h"
#include "FastPFor/headers/fastpfor.h"

#include "FastPFor/headers/simdfastpfor.h"
#include "FastPFor/headers/optpfor.h"
#include "FastPFor/headers/simdoptpfor.h"
  #ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
  #endif
#define _mm_store_si128 _mm_storeu_si128
#define _mm_load_si128 _mm_loadu_si128
#include "FastPFor/headers/simdgroupsimple.h"
#undef min

#include "FastPFor/headers/compositecodec.h"

#define ctou32(_cp_) (*(unsigned *)(_cp_))

unsigned FastPFore32(const uint32_t *in, unsigned n, unsigned char *out, unsigned outsize) {
  size_t nvalue = outsize/4;
  FastPForLib::FastPFor<4> ic; 
  ic.encodeArray((const uint32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
  if(n & 127) {
    size_t nvalue2 = outsize/4 - nvalue;
    FastPForLib::VariableByte vc; 
	vc.encodeArray((const uint32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
    nvalue += nvalue2;
  }
  ctou32(out) = nvalue;
  return 4+nvalue*4;
}

unsigned FastPFord32(const unsigned char *in, unsigned n, uint32_t *out) {
  size_t nvalue = n;
  FastPForLib::FastPFor<4> ic; 
  const uint32_t *ip = ic.decodeArray((const uint32_t *)(in+4), ctou32(in), out, nvalue);
  if(n & 127) { 
    nvalue = n - nvalue;
	FastPForLib::VariableByte vc;
	ip = vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);
  }
  return ctou32(ip);
}

/*unsigned FastPFore64(const uint64_t *in, unsigned n, unsigned char *out, unsigned outsize) {
  size_t nvalue = outsize/8;
  FastPForLib::FastPFor<4> ic; 
  ic.encodeArray(in, (size_t)(n & (~127)), (uint32_t *)(out+4), nvalue);
  if(n & 127) {
    size_t nvalue2 = outsize/8 - nvalue;
    FastPForLib::VariableByte vc; 
	
	vc.encodeArray((const uint64_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
    nvalue += nvalue2;
  }
  ctou32(out) = nvalue;
  return 4+nvalue*4;
}

unsigned FastPFord64(const unsigned char *in, unsigned n, uint64_t *out) {
  size_t nvalue = n;
  FastPForLib::FastPFor<4> ic; 
  const uint32_t *ip = ic.decodeArray((const uint32_t *)(in+4), ctou32(in), (uint64_t *)out, nvalue);
  if(n & 127) {
    nvalue = n - nvalue;
	FastPForLib::VariableByte vc;
	ip = vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  
  }
  return ctou32(ip);
}*/

unsigned FastPFore128v32(const uint32_t *in, unsigned n, unsigned char *out, unsigned outsize) {
  size_t nvalue = outsize/4;
  FastPForLib::SIMDFastPFor<4> ic; 
  ic.encodeArray(in, n & (~127), (uint32_t *)(out+4), nvalue);
  if(n & 127) {
    size_t nvalue2 = outsize/4 - nvalue;
    FastPForLib::VariableByte vc; vc.encodeArray((const uint32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
    nvalue += nvalue2;
  }
  ctou32(out) = nvalue;
  return 4+nvalue*4;
}

unsigned FastPFord128v32(const unsigned char *in, unsigned n, uint32_t *out) {
  size_t nvalue = n;
  FastPForLib::SIMDFastPFor<4> ic; 
  const uint32_t *ip = ic.decodeArray((const uint32_t *)(in+4), *(uint32_t *)in, out, nvalue);
  if(n & 127) { 
    nvalue = n - nvalue;
	FastPForLib::VariableByte vc;
	ip = vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
  }
  return (unsigned char *)ip - (unsigned char *)in; 
}

unsigned OptPFore128v32(const uint32_t *in, unsigned n, unsigned char *out, unsigned outsize) {
  size_t nvalue = outsize/4;
  FastPForLib::SIMDOPTPFor<4> ic; ic.encodeArray((const uint32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
  if(n & 127) {
    size_t nvalue2 = outsize/4 - nvalue;
    FastPForLib::VariableByte vc; vc.encodeArray((const uint32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
    nvalue += nvalue2;
  }
  ctou32(out) = nvalue;
  return 4+nvalue*4;
}

unsigned OptPFord128v32(const unsigned char *in, unsigned n, uint32_t *out) {
  size_t nvalue = n;
  FastPForLib::SIMDOPTPFor<4> ic; 
  const uint32_t *ip = ic.decodeArray((const uint32_t *)(in+4), ctou32(in), out, nvalue);
  if(n & 127) { 
    nvalue = n - nvalue;
	FastPForLib::VariableByte vc;
	ip = vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
  }
  return (unsigned char *)ip-in; 
}
