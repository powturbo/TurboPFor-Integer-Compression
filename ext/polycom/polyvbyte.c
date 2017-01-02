#include "vbyte_poly.h"
#include "polyvbyte.h"

unsigned char *vbpolyenc(unsigned *in, unsigned n, unsigned char *out) {
  unsigned i; 
  for(i = 0; i < n; i++) { unsigned x = in[i]; VBYTE_ENC(out, x); } 
  return out;
}
unsigned char *vbpolydec(unsigned char *in, unsigned n, unsigned *out) {
  unsigned i; 
  for(i = 0; i < n; i++) { unsigned x; VBYTE_DEC(in, x); out[i] = x; } 
  return in;
}

