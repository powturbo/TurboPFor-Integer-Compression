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

#define ID_MEMCPY 14
unsigned bench(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id) { 
  unsigned l;
  memrcpy(cpy,in,n); 
  switch(id) {
    case 1: 	    TMBENCH("\np4nenc16    ",l=p4nenc16(        in, n/2, out)  ,n); 	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4ndec16(         out, n/2, cpy)   ,n); break;
    case 2: 	    TMBENCH("\np4nenc16v   ",l=p4nenc128v16(    in, n/2, out)  ,n); 	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4ndec128v16(     out, n/2, cpy)   ,n); break;

    case 3: 		TMBENCH("\np4nzenc16   ",l=p4nzenc16(       in, n/2, out)  ,n); 	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nzdec16(        out, n/2, cpy)   ,n); break;
    case 4: 	    TMBENCH("\np4nzenc16v  ",l=p4nzenc128v16(   in, n/2, out)  ,n); 	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nzdec128v16(    out, n/2, cpy)   ,n); break;

    case 5: 		TMBENCH("\np4ndenc16   ",l=p4ndenc16(       in, n/2, out)  ,n); 	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nddec16(        out, n/2, cpy)   ,n); break;
    case 6: 		TMBENCH("\np4ndenc16v  ",l=p4ndenc128v16(   in, n/2, out)  ,n); 	printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",p4nddec128v16(    out, n/2, cpy)   ,n); break;

    case 7: 	    TMBENCH("\nvbzenc16    ",l=vbzenc16(        in, n/2, out,0)-out,n); printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",vbzdec16(         out, n/2, cpy,0) ,n); break;
	
    case 8: 	    TMBENCH("\nbitnpack16  ",l=bitnpack16(      in, n/2, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnunpack16(     out, n/2, cpy)   ,n); break;
    case 9: 	    TMBENCH("\nbitnpack16v ",l=bitnpack128v16(  in, n/2, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnunpack128v16( out, n/2, cpy)   ,n); break;

    case 10: 	    TMBENCH("\nbitnzpack16 ",l=bitnzpack16(     in, n/2, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnzunpack16(    out, n/2, cpy)   ,n); break;
    case 11: 	    TMBENCH("\nbitnzpack16v",l=bitnzpack128v16( in, n/2, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitnzunpack128v16(out, n/2, cpy)   ,n); break;

    case 12: 	    TMBENCH("\nbitndpack16 ",l=bitndpack16(     in, n/2, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitndunpack16(    out, n/2, cpy)   ,n); break;
    case 13: 	    TMBENCH("\nbitndpack16v",l=bitndpack128v16( in, n/2, out)  ,n);	    printf("%10u %5.1f%%", l, (double)l*100.0/n); TMBENCH("",bitndunpack128v16(out, n/2, cpy)   ,n); break;
    case ID_MEMCPY:TMBENCH("\nmemcpy     ",libmemcpy(  in,out,n) ,n);  				    printf("%10u %5.1f%%", n, (double)100.0); return n;
	default: return l;
  }
  memcheck(in,(n/2)*2,cpy);
  return l;
}

int main(int argc, char* argv[]) {
  unsigned trips = 4,cmp=1, b = 1 << 30, esize=4, lz=0, fno,id=0;
  unsigned char *in,*out,*cpy;
  
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"blocsize", 	0, 0, 'b'}, {0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "B:ce:i:I:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;								
      case 'e': id     = atoi(optarg);  break;
      case 'i': tm_rep = atoi(optarg); if(!tm_rep) tm_rep=1,trips=1;  break;
      case 'I': trips  = atoi(optarg); if(!trips) trips=1;  break;
      case 'B': b = argtoi(optarg,1); 	break;
      case 'c': cmp++; 				  	break;
    }
  }
  if(argc - optind < 1) { fprintf(stderr, "16 Bits file not specified\n"); exit(-1); }

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
    n = fread(in, 1, n, fi);								 printf("File='%s' Length=%u\n", inname, n);			
    fclose(fi);
    if(n <= 0) exit(0); 
    if(fno == optind)
      tm_init(trips, 1);  
    if(!id) {
      printf("function      E MB/s   size      ratio   D MB/s");      
      for(i=1; i <= ID_MEMCPY; i++) bench(in,n,out,cpy,i);
    } else 
      bench(in,n,out,cpy,id);    
    printf("\n");      
  }
}

/* fc_300MHz.dat skylake: i7-6700 3.4GHz
./icapp fc_300MHz.dat -I23
File='fc_300MHz.dat' Length=711595784
function      E MB/s   size      ratio   D MB/s
p4nenc16       627.18  441887368  62.1%  3284.07      
p4nenc16v      684.00  441887368  62.1%  6193.39      
p4nzenc16      507.39  332356287  46.7%  1405.11      
p4nzenc16v     548.99  332356287  46.7%  2651.81      
p4ndenc16      450.75  525441761  73.8%  1375.51      
p4ndenc16v     459.79  525441761  73.8%  2339.04      
vbzenc16      2228.28  390678354  54.9%  2027.44      
bitnpack16    4139.66  587990808  82.6%  5244.86      
bitnpack16v   6659.51  587990808  82.6%  9207.07      
bitnzpack16   1441.67  594311978  83.5%  2442.65      
bitnzpack16v  2218.05  594311978  83.5%  4417.44      
bitndpack16   2676.71  713562010 100.3%  4937.93      
bitndpack16v  2994.07  713562010 100.3%  6393.84      
memcpy      13334.75  711595784 100.0%
*/
