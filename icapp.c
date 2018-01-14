/**
    Copyright (C) powturbo 2015-2018
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

    TurboPFor - "Benchmark demo application"
**/      
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
  #ifdef _MSC_VER
#include "vs/getopt.h"
  #else
#include <getopt.h> 
#endif
#include "time_.h"

  #if defined(_WIN32)
#include <windows.h>
  #endif
#define NTURBOPFOR_DAC
#include "conf.h"
#include "bitutil.h"
#include "bitpack.h"
#include "vp4.h"
#include "vint.h"

int memcheck(unsigned char *in, unsigned n, unsigned char *cpy) { 
  int i;
  for(i = 0; i < n; i++)
    if(in[i] != cpy[i]) {
      printf("ERROR in[%d]=%x, dec[%d]=%x\n", i, in[i], i, cpy[i]);
	  return i+1; 
	} 
  return 0;
}

void libmemcpy(unsigned char *dst, unsigned char *src, int len) {
  void *(*memcpy_ptr)(void *, const void *, size_t) = memcpy;
  if (time(NULL) == 1) 
    memcpy_ptr = NULL;
  memcpy_ptr(dst, src, len);
}

#define ID_MEMCPY 32

unsigned bench32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id) { 
  unsigned l,m=(n+3)/4;
  memrcpy(cpy,in,n); 
  switch(id) {
    case 1: 	    TMBENCH("\np4nenc32        ",l=p4nenc32(        in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4ndec32(         out, m, cpy)   ,n); break;
    case 2: 	    TMBENCH("\np4nenc128v32    ",l=p4nenc128v32(    in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4ndec128v32(     out, m, cpy)   ,n); break;

    case 3: 		TMBENCH("\np4nzenc32       ",l=p4nzenc32(       in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nzdec32(        out, m, cpy)   ,n); break;
    case 4: 	    TMBENCH("\np4nzenc128v32   ",l=p4nzenc128v32(   in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nzdec128v32(    out, m, cpy)   ,n); break;
 
    case 5: 		TMBENCH("\np4ndenc32       ",l=p4ndenc32(       in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nddec32(        out, m, cpy)   ,n); break;
    case 6: 		TMBENCH("\np4ndenc128v32   ",l=p4ndenc128v32(   in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nddec128v32(    out, m, cpy)   ,n); break;

    case 7: 		TMBENCH("\np4nd1enc32      ",l=p4nd1enc32(       in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nd1dec32(        out, m, cpy)   ,n); break;
    case 8: 		TMBENCH("\np4nd1enc128v32  ",l=p4nd1enc128v32(   in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nd1dec128v32(    out, m, cpy)   ,n); break;

    case 9: 	    TMBENCH("\nvbzenc32        ",l=vbzenc32(        in, m, out,0)-out,n); 	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",vbzdec32(         out, m, cpy,0) ,n); break;
	
    case 10: 	    TMBENCH("\nbitnpack32      ",l=bitnpack32(      in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnunpack32(     out, m, cpy)   ,n); break;
    case 11: 	    TMBENCH("\nbitnpack128v32  ",l=bitnpack128v32(  in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnunpack128v32( out, m, cpy)   ,n); break;

    case 12: 	    TMBENCH("\nbitnzpack32     ",l=bitnzpack32(     in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnzunpack32(    out, m, cpy)   ,n); break;
    case 13: 	    TMBENCH("\nbitnzpack128v32 ",l=bitnzpack128v32( in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnzunpack128v32(out, m, cpy)   ,n); break;

    case 14: 	    TMBENCH("\nbitndpack32     ",l=bitndpack32(     in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitndunpack32(    out, m, cpy)   ,n); break;
    case 15: 	    TMBENCH("\nbitndpack128v32 ",l=bitndpack128v32( in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitndunpack128v32(out, m, cpy)   ,n); break;

    case 16: 	    TMBENCH("\nbitnd1pack32    ",l=bitnd1pack32(     in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnd1unpack32(    out, m, cpy)   ,n); break;
    case 17: 	    TMBENCH("\nbitnd1pack128v32",l=bitnd1pack128v32( in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnd1unpack128v32(out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 18: 	    TMBENCH("\np4nenc256v32    ", l=p4nenc256v32(    in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4ndec256v32(     out, m, cpy)   ,n); break;
    case 19: 	    TMBENCH("\np4nzenc256v32   ", l=p4nzenc256v32(   in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nzdec256v32(    out, m, cpy)   ,n); break;
    case 20: 		TMBENCH("\np4ndenc256v32   ", l=p4ndenc256v32(   in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nddec256v32(    out, m, cpy)   ,n); break;
    case 21: 	    TMBENCH("\nbitnpack256v32  ", l=bitnpack256v32(  in, m, out)  ,n);   	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnunpack256v32( out, m, cpy)   ,n); break;
    case 22: 	    TMBENCH("\nbitnzpack256v32 ", l=bitnzpack256v32( in, m, out)  ,n);   	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnzunpack256v32(out, m, cpy)   ,n); break;
    case 23: 	    TMBENCH("\nbitndpack256v32 ", l=bitndpack256v32( in, m, out)  ,n);   	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitndunpack256v32(out, m, cpy)   ,n); break;
    case 24: 	    TMBENCH("\nbitnd1pack256v32",l=bitnd1pack256v32( in, m, out) ,n);   	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnd1unpack256v32(out, m, cpy)   ,n); break;
      #endif
    case ID_MEMCPY: TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  			    	printf("%10u %5.1f%%", n, (double)100.0); return n;
	default: return l;
  }
  memcheck(in,n,cpy);
  return l;
}

unsigned bench16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id) { 
  unsigned l,m=(n+1)/2;
  memrcpy(cpy,in,n); 
  switch(id) {
    case 1: 	    TMBENCH("\np4nenc16        ",l=p4nenc16(        in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4ndec16(         out, m, cpy)   ,n); break;
    case 2: 	    TMBENCH("\np4nenc128v16    ",l=p4nenc128v16(    in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4ndec128v16(     out, m, cpy)   ,n); break;

    case 3: 		TMBENCH("\np4nzenc16       ",l=p4nzenc16(       in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nzdec16(        out, m, cpy)   ,n); break;
    case 4: 	    TMBENCH("\np4nzenc128v16   ",l=p4nzenc128v16(   in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nzdec128v16(    out, m, cpy)   ,n); break;

    case 5: 		TMBENCH("\np4ndenc16       ",l=p4ndenc16(       in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nddec16(        out, m, cpy)   ,n); break;
    case 6: 		TMBENCH("\np4ndenc128v16   ",l=p4ndenc128v16(   in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nddec128v16(    out, m, cpy)   ,n); break;

    case 7: 		TMBENCH("\np4nd1enc16      ",l=p4nd1enc16(       in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nd1dec16(       out, m, cpy)   ,n); break;
    case 8: 		TMBENCH("\np4nd1enc128v16  ",l=p4nd1enc128v16(   in, m, out)  ,n); 		printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nd1dec128v16(   out, m, cpy)   ,n); break;
    case 9: 	    TMBENCH("\nvbzenc16        ",l=vbzenc16(        in, m, out,0)-out,n); 	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",vbzdec16(         out, m, cpy,0) ,n); break;
	
    case 10: 	    TMBENCH("\nbitnpack16      ",l=bitnpack16(      in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnunpack16(     out, m, cpy)   ,n); break;
    case 11: 	    TMBENCH("\nbitnpack128v16  ",l=bitnpack128v16(  in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnunpack128v16( out, m, cpy)   ,n); break;

    case 12: 	    TMBENCH("\nbitnzpack16     ",l=bitnzpack16(     in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnzunpack16(    out, m, cpy)   ,n); break;
    case 13: 	    TMBENCH("\nbitnzpack128v16 ",l=bitnzpack128v16( in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnzunpack128v16(out, m, cpy)   ,n); break;

    case 14: 	    TMBENCH("\nbitndpack16     ",l=bitndpack16(     in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitndunpack16(    out, m, cpy)   ,n); break;
    case 15: 	    TMBENCH("\nbitndpack128v16 ",l=bitndpack128v16( in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitndunpack128v16(out, m, cpy)   ,n); break;

    case 16: 	    TMBENCH("\nbitnd1pack16    ",l=bitnd1pack16(     in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnd1unpack16(    out, m, cpy)   ,n); break;
    case 17: 	    TMBENCH("\nbitnd1pack128v16",l=bitnd1pack128v16( in, m, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnd1unpack128v16(out, m, cpy)   ,n); break;
    case ID_MEMCPY: TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  				printf("%10u %5.1f%%", n, (double)100.0); return n;
	default: return l;
  }
  memcheck(in,n,cpy);
  return l;
}

int main(int argc, char* argv[]) {
  unsigned trips = 4,cmp=1, b = 1 << 31, esize=4, lz=0, fno,idmin=1,idmax=-1, usize=4;
  unsigned char *in,*out,*cpy;
  
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"blocsize", 	0, 0, 'b'}, {0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "B:ce:E:i:I:s:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;								
      case 'e': idmin  = atoi(optarg); if(idmax == -1) idmax = idmin; break;
      case 'E': idmax  = atoi(optarg);  break;
      case 'i': tm_rep = atoi(optarg); if(!tm_rep) tm_rep=1,trips=1;  break;
      case 'I': trips  = atoi(optarg); if(!trips) trips=1;  break;
      case 's': usize = argtoi(optarg,1); 	break;
      case 'B': b = argtoi(optarg,1); 	break;
      case 'c': cmp++; 				  	break;
    }
  }
  if(idmax == -1) idmax = ID_MEMCPY;
  if(idmax < idmin) idmax = idmin;
  if(argc - optind < 1) { fprintf(stderr, "raw file not specified\n"); 
    fprintf(stderr, "usage: icpp file [-s2|-s4] -e# -E#\n");
    fprintf(stderr, "-s2:16 bits, -s4:32 bits,  #:function number  -e:start -E:end\n"); 	
    exit(-1); 
  }

  for(fno = optind; fno < argc; fno++) {
    char *inname = argv[fno];  									
    int n,i; 
	long long flen;
    FILE *fi = fopen(inname, "rb"); 							if(!fi ) { perror(inname); continue; } 							
    fseek(fi, 0, SEEK_END); 
    flen = ftell(fi); 
	fseek(fi, 0, SEEK_SET);
	
    if(flen > b) flen = b;
    n = flen; 
    if(!(in  =        (unsigned char*)malloc(n+1024)))     { fprintf(stderr, "malloc error\n"); exit(-1); } cpy = in;
    if(!(out =        (unsigned char*)malloc(flen*4/3+1024))) { fprintf(stderr, "malloc error\n"); exit(-1); } 
    if(cmp && !(cpy = (unsigned char*)malloc(n+1024)))     { fprintf(stderr, "malloc error\n"); exit(-1); }
    n = fread(in, 1, n, fi);								 printf("File='%s' Length=%u Integer size=%d\n", inname, n, usize);			
    fclose(fi);
    if(n <= 0) exit(0); 
    if(fno == optind)
      tm_init(trips, 1);  
    printf("function      E MB/s   size      ratio   D MB/s");      
    for(i=idmin; i <= idmax; i++) 
      switch(usize) {
        case 2: bench16(in,n,out,cpy,i); break;  
        case 4: bench32(in,n,out,cpy,i); break;    
        default: die("integer size must be 2 or 4\n"); 
      }
    printf("\n");      
  }
}

/* fc_300MHz.dat skylake: i7-6700 3.4GHz
./icapp fc_300MHz.dat -I23
File='fc_300MHz.dat' Length=711595784
function      E MB/s   size      ratio   D MB/s
p4nenc16       639.32  441887368  62.1%  3440.60      
p4nenc16v      684.74  441887368  62.1%  6235.07      
p4nzenc16      507.39  332356287  46.7%  1796.33      
p4nzenc16v     548.99  332356287  46.7%  2651.81      
p4ndenc16      450.75  525441761  73.8%  1375.51      
p4ndenc16v     459.79  525441761  73.8%  2340.81      
vbzenc16      2228.28  390678354  54.9%  2028.28      
bitnpack16    4139.86  587990808  82.6%  5244.86      
bitnpack16v   6725.41  587990808  82.6%  9207.07      
bitnzpack16   1439.90  594311978  83.5%  2442.53      
bitnzpack16v  2219.25  594311978  83.5%  4417.38      
bitndpack16   4637.59  713562010 100.3%  4942.12      
bitndpack16v  6753.63  713562010 100.3%  6393.84      
bitnd1pack16  2742.09  714375456 100.4%  4952.64      
bitnd1pack16v 3600.12  714375456 100.4%  6301.49      
memcpy      13429.38  711595784 100.0%


*/
