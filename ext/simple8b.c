// modified and optimized (speed + compression) by powturbo
// 64 bits version from: Vo Ngoc Anh, Alistair Moffat: Index compression using 64-bit words.
// Softw., Pract. Exper. 40(2): 131-147 (2010)
// http://ww2.cs.mu.oz.au/~alistair/coders-64bit/
#include "simple8b.h"
  #if defined(__x86_64__) || defined(__x86_32__)
static inline int bsr32(int x) {
  int b = -1;
  asm("bsrl %1,%0" : "+r" (b): "rm" (x) );
  return b + 1;
}
  #else
static inline int bsr32(int x) {
  return x?32 - __builtin_clz(x):0;
}
  #endif

#define WPUT(__x,__bit) { __bw |= (unsigned long long)(__x)<<__br; __br += __bit; } 
#define WPUTZERO(__sel) { __bw = __br = 0; WPUT(__sel,4); } 
#define WPUTFLUSH(__out) { *(typeof(__bw) *)__out = __bw; __out += sizeof(__bw)/sizeof(__out[0]); }

#if 0 //WORD_SIZE==32
  #define CODE_TABLE                                            \
	unsigned char sel2bit[]=  {  0,  0, 0, 0, 0, 0, 0, 1 ,2,3,4,5,7,9,14,28};       \
	unsigned sel2elems[]=   {256,120,90,60,50,40,32,28,14,9,7,5,4,3, 2, 1};       \

  #define BIT_2_SEL                                             \
	char bit2sel[]=   { 0,7,8,9,10,11,12,12,13,13,14,14,14,14,14,           \
	                    15,15,15,15,15,15,15,15,15,15,15,15,15,15,          \
											-1,-1,-1,-1};
  #define MAX_BIT 28
#else
#define CODE_TABLE                                            							 \
	/*                       0   1  2  3  4  5  6  7  8 9 10 11 12 13 14 15 */ 			 \
unsigned char sel2bit[]=  {  0,  0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 15, 20, 30, 60,61};   \
unsigned sel2elems[]=     {256,120,60,30,20,15,12,10, 8, 7,  6,  5,  4,  3,  2,  1};  \
unsigned sellim[]=        {256,120,60,60,60,60,60,60,56,56, 60, 60, 60, 60, 60, 60};  

#define BIT_2_SEL char bit2sel[]=                                    \
        {0,2,3,4,5,6,7,8, 9,10,10,11,11,12,12,12,                    \
		13,13,13,13,13,14,14,14, 14,14,14,14,14,14,14,15,            \
		15,15,15,15,15,15,15,15, 15,15,15,15,15,15,15,15,            \
		15,15,15,15,15,15,15,15, 15,15,15,15,15,-1, -1, -1, -1};
				 
  #define MAX_BIT 60 
#endif

CODE_TABLE
BIT_2_SEL   
                       
unsigned char *vs8benc(unsigned *__restrict in, int n, unsigned char *__restrict out) { 
  unsigned long long __bw; unsigned __br = 0; 
  unsigned char bits[SIMPLE8BMAX];                          
  int elems;       
  int i,j;
  for (i = 0; i < n; i++) 
    bits[i] = bsr32(in[i])+1;  //CalcMinBits(in, bits, n);
  int sel, bit,tmp;                                              /*BLK_ENC_ADJUST*/                                                
  for (i=0; i<n; ) {                                            
    j     = i;                                                       
    sel   = bit2sel[bits[j]];                                      
    bit   = sel2bit[sel];                                          
    elems = 1;                                                   
    while (++j<n) {                                             
      if (elems == sel2elems[sel]) break;                       
	if (bits[j] > bit) {                                      
	  tmp = bit2sel[bits[j]] ;                                 
	  if(elems < sel2elems[ tmp ]) {                         
	    sel = tmp;                                             
	    bit= sel2bit[sel];                                    
	  } else {                                                
	    while ( elems < sel2elems[sel] ) sel++;               
	    elems = sel2elems[sel];                                
	    bit   = sel2bit[sel];                                    
	    break;                                                
	  }                                                       
	}                                                         
	elems++;                                                  
    }                                                          
    if (bit == 0) { /* can be downgrade to bit=1 */         
      if (i+elems<n) {                                          
	    for (sel = 0; sel2elems[sel] > elems; sel++);             
	    elems = sel2elems[sel];                                   
	    bit   = sel2bit[sel];                                   
      } else sel = 0; /* what a waste! */                        
    } else {                                                    
      sel = bit2sel[bit];                                    
      bit = sel2bit[sel];                                     
    }                                                           
    WPUTZERO(sel);                              
    if (bit) {                                                
      for ( ; elems ; elems--, i++) WPUT(in[i],bit);                               
    } else 
      i += elems;                                               
    WPUTFLUSH(out);
  }                                                             
  return out;
}

#define MSK(__x) ((1ul<<__x)-1)
unsigned char *vs8bdec(unsigned char *__restrict in, int n, unsigned *__restrict out) {
  unsigned char *ip = in; 
  unsigned i,*out_ = out+n;
  while(out < out_) {
    unsigned long long w = *(unsigned long long *)ip;
    switch(w & 15) {  
        #if 1
      case 0: ip+=8; for(i=0; i<256; i++) out[i]= 1; out += 256; break; 
        #else                     
      case 0: { int r = (w>>4)&0xf; ip++; if(r == 0xf) { r = (w>>8)&0xff; ip++; } while(r-->=0) *out++=0;  } break; 
        #endif
                                              
      case 1: ip+=8;                                     
        for(i=0; i<120; i++) out[i]= 1; out += 120;              
        break;                                        
      case 2: ip+=8;                                     
        out[ 0]= (w >>  4) & MSK(1);      
		out[ 1]= (w >>  5) & MSK(1);      
        out[ 2]= (w >>  6) & MSK(1);      
        out[ 3]= (w >>  7) & MSK(1);      
        out[ 4]= (w >>  8) & MSK(1);      
        out[ 5]= (w >>  9) & MSK(1);      
        out[ 6]= (w >> 10) & MSK(1);      
        out[ 7]= (w >> 11) & MSK(1);      
        out[ 8]= (w >> 12) & MSK(1);      
        out[ 9]= (w >> 13) & MSK(1);      
        out[10]= (w >> 14) & MSK(1);      
        out[11]= (w >> 15) & MSK(1);      
        out[12]= (w >> 16) & MSK(1);      
        out[13]= (w >> 17) & MSK(1);      
        out[14]= (w >> 18) & MSK(1);      
        out[15]= (w >> 19) & MSK(1);      
        out[16]= (w >> 20) & MSK(1);      
        out[17]= (w >> 21) & MSK(1);      
        out[18]= (w >> 22) & MSK(1);      
        out[19]= (w >> 23) & MSK(1);      
        out[20]= (w >> 24) & MSK(1);      
        out[21]= (w >> 25) & MSK(1);      
        out[22]= (w >> 26) & MSK(1);      
        out[23]= (w >> 27) & MSK(1);      
        out[24]= (w >> 28) & MSK(1);      
        out[25]= (w >> 29) & MSK(1);      
        out[26]= (w >> 30) & MSK(1);      
        out[27]= (w >> 31) & MSK(1);      
        out[28]= (w >> 32) & MSK(1);      
        out[29]= (w >> 33) & MSK(1);      
        out[30]= (w >> 34) & MSK(1);      
        out[31]= (w >> 35) & MSK(1);      
        out[32]= (w >> 36) & MSK(1);      
        out[33]= (w >> 37) & MSK(1);      
        out[34]= (w >> 38) & MSK(1);      
        out[35]= (w >> 39) & MSK(1);      
        out[36]= (w >> 40) & MSK(1);      
        out[37]= (w >> 41) & MSK(1);      
        out[38]= (w >> 42) & MSK(1);      
        out[39]= (w >> 43) & MSK(1);      
        out[40]= (w >> 44) & MSK(1);      
        out[41]= (w >> 45) & MSK(1);      
        out[42]= (w >> 46) & MSK(1);      
        out[43]= (w >> 47) & MSK(1);      
        out[44]= (w >> 48) & MSK(1);      
        out[45]= (w >> 49) & MSK(1);      
        out[46]= (w >> 50) & MSK(1);      
        out[47]= (w >> 51) & MSK(1);      
        out[48]= (w >> 52) & MSK(1);      
        out[49]= (w >> 53) & MSK(1);      
        out[50]= (w >> 54) & MSK(1);      
        out[51]= (w >> 55) & MSK(1);      
        out[52]= (w >> 56) & MSK(1);      
        out[53]= (w >> 57) & MSK(1);      
        out[54]= (w >> 58) & MSK(1);      
        out[55]= (w >> 59) & MSK(1);      
        out[56]= (w >> 60) & MSK(1);      
        out[57]= (w >> 61) & MSK(1);      
        out[58]= (w >> 62) & MSK(1);      
        out[59]= (w >> 63) & MSK(1); out += 60;              
        break;                                        
      case 3: ip+=8;                                     
        out[ 0]= (w >>  4) & MSK(2);      
        out[ 1]= (w >>  6) & MSK(2);      
        out[ 2]= (w >>  8) & MSK(2);      
        out[ 3]= (w >> 10) & MSK(2);      
        out[ 4]= (w >> 12) & MSK(2);      
        out[ 5]= (w >> 14) & MSK(2);      
        out[ 6]= (w >> 16) & MSK(2);      
        out[ 7]= (w >> 18) & MSK(2);      
        out[ 8]= (w >> 20) & MSK(2);      
        out[ 9]= (w >> 22) & MSK(2);      
        out[10]= (w >> 24) & MSK(2);      
        out[11]= (w >> 26) & MSK(2);      
        out[12]= (w >> 28) & MSK(2);      
        out[13]= (w >> 30) & MSK(2);      
        out[14]= (w >> 32) & MSK(2);      
        out[15]= (w >> 34) & MSK(2);      
        out[16]= (w >> 36) & MSK(2);      
        out[17]= (w >> 38) & MSK(2);      
        out[18]= (w >> 40) & MSK(2);      
        out[19]= (w >> 42) & MSK(2);      
        out[20]= (w >> 44) & MSK(2);      
        out[21]= (w >> 46) & MSK(2);      
        out[22]= (w >> 48) & MSK(2);      
        out[23]= (w >> 50) & MSK(2);      
        out[24]= (w >> 52) & MSK(2);      
        out[25]= (w >> 54) & MSK(2);      
        out[26]= (w >> 56) & MSK(2);      
        out[27]= (w >> 58) & MSK(2);      
        out[28]= (w >> 60) & MSK(2);      
        out[29]= (w >> 62) & MSK(2); out += 30;              
        break;                                        
      case 4: ip+=8;                                    
        out[ 0]= (w >>  4) & MSK(3);      
        out[ 1]= (w >>  7) & MSK(3);      
        out[ 2]= (w >> 10) & MSK(3);      
        out[ 3]= (w >> 13) & MSK(3);      
        out[ 4]= (w >> 16) & MSK(3);      
        out[ 5]= (w >> 19) & MSK(3);      
        out[ 6]= (w >> 22) & MSK(3);      
        out[ 7]= (w >> 25) & MSK(3);      
        out[ 8]= (w >> 28) & MSK(3);      
        out[ 9]= (w >> 31) & MSK(3);      
        out[10]= (w >> 34) & MSK(3);      
        out[11]= (w >> 37) & MSK(3);      
        out[12]= (w >> 40) & MSK(3);      
        out[13]= (w >> 43) & MSK(3);      
        out[14]= (w >> 46) & MSK(3);      
        out[15]= (w >> 49) & MSK(3);      
        out[16]= (w >> 52) & MSK(3);      
        out[17]= (w >> 55) & MSK(3);      
        out[18]= (w >> 58) & MSK(3);      
        out[19]= (w >> 61) & MSK(3); out += 20;              
        break;                                        
      case 5: ip+=8;                                    
        out[ 0]= (w >>  4) & MSK(4);      
        out[ 1]= (w >>  8) & MSK(4);      
        out[ 2]= (w >> 12) & MSK(4);      
        out[ 3]= (w >> 16) & MSK(4);      
        out[ 4]= (w >> 20) & MSK(4);      
        out[ 5]= (w >> 24) & MSK(4);      
        out[ 6]= (w >> 28) & MSK(4);      
        out[ 7]= (w >> 32) & MSK(4);      
        out[ 8]= (w >> 36) & MSK(4);      
        out[ 9]= (w >> 40) & MSK(4);      
        out[10]= (w >> 44) & MSK(4);      
        out[11]= (w >> 48) & MSK(4);      
        out[12]= (w >> 52) & MSK(4);      
        out[13]= (w >> 56) & MSK(4);      
        out[14]= (w >> 60) & MSK(4); out += 15;              
        break;                                        
      case 6: ip+=8;                                     
        out[ 0]= (w >>  4) & MSK(5);      
        out[ 1]= (w >>  9) & MSK(5);      
        out[ 2]= (w >> 14) & MSK(5);      
        out[ 3]= (w >> 19) & MSK(5);      
        out[ 4]= (w >> 24) & MSK(5);      
        out[ 5]= (w >> 29) & MSK(5);      
        out[ 6]= (w >> 34) & MSK(5);      
        out[ 7]= (w >> 39) & MSK(5);      
        out[ 8]= (w >> 44) & MSK(5);      
        out[ 9]= (w >> 49) & MSK(5);      
        out[10]= (w >> 54) & MSK(5);      
        out[11]= (w >> 59) & MSK(5); out += 12;              
        break;                                        
      case 7: ip+=8;                                     
        out[0]= (w >>  4) & MSK(6);      
        out[1]= (w >> 10) & MSK(6);      
        out[2]= (w >> 16) & MSK(6);      
        out[3]= (w >> 22) & MSK(6);      
        out[4]= (w >> 28) & MSK(6);      
        out[5]= (w >> 34) & MSK(6);      
        out[6]= (w >> 40) & MSK(6);      
        out[7]= (w >> 46) & MSK(6);      
        out[8]= (w >> 52) & MSK(6);      
        out[9]= (w >> 58) & MSK(6);  out += 10;              
        break;                                        
      case 8: ip+=8;                                     
        out[0]= (w >> 4 ) & MSK(7);      
        out[1]= (w >> 11) & MSK(7);      
        out[2]= (w >> 18) & MSK(7);      
        out[3]= (w >> 25) & MSK(7);      
        out[4]= (w >> 32) & MSK(7);      
        out[5]= (w >> 39) & MSK(7);      
        out[6]= (w >> 46) & MSK(7);      
        out[7]= (w >> 53) & MSK(7);  out += 8;              
        break;                                        
      case 9: ip+=8;                                     
        out[0]= (w >> 4 ) & MSK(8);      
        out[1]= (w >> 12) & MSK(8);      
        out[2]= (w >> 20) & MSK(8);      
        out[3]= (w >> 28) & MSK(8);      
        out[4]= (w >> 36) & MSK(8);      
        out[5]= (w >> 44) & MSK(8);      
        out[6]= (w >> 52) & MSK(8);  out += 7;              
        break;                                        
      case 10: ip+=8;                                     
        out[0]= (w >>  4) & MSK(10);      
        out[1]= (w >> 14) & MSK(10);      
        out[2]= (w >> 24) & MSK(10);      
        out[3]= (w >> 34) & MSK(10);      
        out[4]= (w >> 44) & MSK(10);      
        out[5]= (w >> 54) & MSK(10); out += 6;              
        break;                                        
      case 11: ip+=8;                                     
        out[0]= (w >>  4) & MSK(12);      
        out[1]= (w >> 16) & MSK(12);      
        out[2]= (w >> 28) & MSK(12);      
        out[3]= (w >> 40) & MSK(12);      
        out[4]= (w >> 52) & MSK(12); out += 5;              
        break;                                        
      case 12: ip+=8;                                     
        out[0]= (w >>  4) & MSK(15);      
        out[1]= (w >> 19) & MSK(15);      
        out[2]= (w >> 34) & MSK(15);      
        out[3]= (w >> 49) & MSK(15); out += 4;              
        break;                                        
      case 13: ip+=8;                                     
        out[0]= (w >>  4) & MSK(20);      
        out[1]= (w >> 24) & MSK(20);      
        out[2]= (w >> 44) & MSK(20); out += 3;              
        break;                                        
      case 14: ip+=8;                                     
        out[0]= (w >>  4) & MSK(30);      
        out[1]= (w >> 34) & MSK(30); out += 2;              
         break;                                        
      case 15: ip+=8;                                     
        out[0]= (w >> 4) & ((1ull<<60)-1);  out += 1;              
        break;                                        
    }
  }
 return ip;
}
