#include <stdlib.h>
#include "../OPT_PFD/opt_p4.h" 							// OptPFD

#include "optpfd.h"
#include "polyvbyte.h"
unsigned char *optpfdenc32(unsigned *in, int n, unsigned char *out) {
  if(n < 128) 
    out = vbpolyenc(in, n, out);
  else { 
    unsigned tmp[OPTPFDMAX],i; 
    for(i = 0; i < n; i++) tmp[i] = in[i]; 
    return out += OPT4(tmp, n, (unsigned *)out); 
  }  
  return out;
}

unsigned char *optpfddec32(unsigned char *in, int n, unsigned *out) { 
  if(n < 128) 
    in = vbpolydec(in, n, out); 
  else { 
    unsigned all_array[OPTPFDMAX]; 
    in = (unsigned char *)detailed_p4_decode(out, (unsigned *)in, all_array); 
  }
  return in;
}

