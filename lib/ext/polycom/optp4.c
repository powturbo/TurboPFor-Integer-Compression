#include "../OPT_PFD/opt_p4.h" 							// OptPFD

unsigned char *optpfdenc32(unsigned *__restrict in, int n, unsigned *__restrict out) {
  if(n < 128) 
    out = vbyteenc(in, n, (unsigned *)out);
  else { 
    unsigned tmp[OPTPFDMAX]; 
    for(i = 0; i < n; i++) tmp[i] = in[i]; 
    return out += OPT4(tmp, n, (unsigned *)out); 
  }  
  return out;
}

unsigned char *optpfddec32(unsigned *__restrict in, int n, unsigned *__restrict out) { 
  if(n < 128) 
    in = vbytedec(in, n, out); 
  else { 
    unsigned all_array[OPTPFDMAX]; 
    return (unsigned char *)detailed_p4_decode(out, (unsigned *)in, all_array); 
  }
}

