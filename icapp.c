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
#include "fp.h"
int mcpy;

int memcheck(unsigned char *in, unsigned n, unsigned char *cpy) { 
  int i;
  for(i = 0; i < n; i++)
    if(in[i] != cpy[i]) {
      printf("ERROR at %d:in=%x,%x dec=%x,%x\n", i, in[i], in[i+1], cpy[i],cpy[i+1]);
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

void pr(unsigned l, unsigned n) { printf("%10u %5.1f%%", l, (double)l*100.0/n); }

#define ID_MEMCPY 32
unsigned benchf64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname) { 
  unsigned l,m=n/8;
  memrcpy(cpy,in,n); 
  switch(id) {
    case 1: 	    TMBENCH("\nfppenc64        ",l=fppenc64(    in, m, out,0)-out  ,n); 		pr(l,n); TMBENCH("",fppdec64(    out, m, cpy,0)   ,n); break;
    case 2: 		TMBENCH("\nfpddenc64       ",l=fpddenc64(   in, m, out,0)-out  ,n); 		pr(l,n); TMBENCH("",fpdddec64(   out, m, cpy,0)   ,n); break;
    case 3: 		TMBENCH("\nfpfcmenc64      ",l=fpfcmenc64(  in, m, out,0)-out  ,n); 		pr(l,n); TMBENCH("",fpfcmdec64(  out, m, cpy,0)   ,n); break;
    case 4: 		TMBENCH("\nfpdfcmenc64     ",l=fpdfcmenc64( in, m, out,0)-out  ,n); 		pr(l,n); TMBENCH("",fpdfcmdec64( out, m, cpy,0)   ,n); break;
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  			    printf("%10u %5.1f%%", n, (double)100.0); return n;
	default: return l;
  }
  printf(" %s ", inname);
  memcheck(in,m*8,cpy);
  return l;
}  

unsigned bench64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname) { 
  unsigned l,m=n/8;
  memrcpy(cpy,in,n); 
  switch(id) {
    case 1: 	    TMBENCH("\np4nenc64        ",l=p4nenc64(        in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4ndec64(         out, m, cpy)   ,n); break;
    case 2: 	    TMBENCH("\np4nenc128v64    ",l=p4nenc128v64(    in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4ndec128v64(     out, m, cpy)   ,n); break;

    case 3: 		TMBENCH("\np4nzenc64       ",l=p4nzenc64(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nzdec64(        out, m, cpy)   ,n); break;
 
    case 5: 		TMBENCH("\np4ndenc64       ",l=p4ndenc64(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nddec64(        out, m, cpy)   ,n); break;

    case 7: 		TMBENCH("\np4nd1enc64      ",l=p4nd1enc64(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nd1dec64(        out, m, cpy)   ,n); break;

    case 9: 	    TMBENCH("\nvbzenc64        ",l=vbzenc64(        in, m, out,0)-out,n); 	pr(l,n); TMBENCH("",vbzdec64(         out, m, cpy,0) ,n); break;
	
    case 10: 	    TMBENCH("\nbitnpack64      ",l=bitnpack64(      in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnunpack64(     out, m, cpy)   ,n); break;
    case 11: 	    TMBENCH("\nbitnpack128v64  ",l=bitnpack128v64(  in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnunpack128v64( out, m, cpy)   ,n); break;

    case 12: 	    TMBENCH("\nbitnzpack64     ",l=bitnzpack64(     in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnzunpack64(    out, m, cpy)   ,n); break;

    case 14: 	    TMBENCH("\nbitndpack64     ",l=bitndpack64(     in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitndunpack64(    out, m, cpy)   ,n); break;

    case 16: 	    TMBENCH("\nbitnd1pack64    ",l=bitnd1pack64(    in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnd1unpack64(    out, m, cpy)   ,n); break;
    case 18: 	    TMBENCH("\np4nsenc64       ",l=p4nsenc64(       in, m, out)  ,n); 	    pr(l,n); TMBENCH("",p4nsdec64(         out, m, cpy)   ,n); break;
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  			    printf("%10u %5.1f%%", n, (double)100.0); return n;
	default: return l;
  }
  printf(" %s ", inname);
  memcheck(in,m*8,cpy);
  return l;
}

unsigned bench32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname) { 
  unsigned l,m=n/4;
  memrcpy(cpy,in,n); 
  switch(id) {
    case 1: 	    TMBENCH("\np4nenc32        ",l=p4nenc32(        in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4ndec32(         out, m, cpy)   ,n); break;
    case 2: 	    TMBENCH("\np4nenc128v32    ",l=p4nenc128v32(    in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4ndec128v32(     out, m, cpy)   ,n); break;

    case 3: 		TMBENCH("\np4nzenc32       ",l=p4nzenc32(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nzdec32(        out, m, cpy)   ,n); break;
    case 4: 	    TMBENCH("\np4nzenc128v32   ",l=p4nzenc128v32(   in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nzdec128v32(    out, m, cpy)   ,n); break;
 
    case 5: 		TMBENCH("\np4ndenc32       ",l=p4ndenc32(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nddec32(        out, m, cpy)   ,n); break;
    case 6: 		TMBENCH("\np4ndenc128v32   ",l=p4ndenc128v32(   in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nddec128v32(    out, m, cpy)   ,n); break;

    case 7: 		TMBENCH("\np4nd1enc32      ",l=p4nd1enc32(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nd1dec32(        out, m, cpy)   ,n); break;
    case 8: 		TMBENCH("\np4nd1enc128v32  ",l=p4nd1enc128v32(   in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nd1dec128v32(    out, m, cpy)   ,n); break;

    case 9: 	    TMBENCH("\nvbzenc32        ",l=vbzenc32(        in, m, out,0)-out,n); 	pr(l,n); TMBENCH("",vbzdec32(         out, m, cpy,0) ,n); break;
	
    case 10: 	    TMBENCH("\nbitnpack32      ",l=bitnpack32(      in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnunpack32(     out, m, cpy)   ,n); break;
    case 11: 	    TMBENCH("\nbitnpack128v32  ",l=bitnpack128v32(  in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnunpack128v32( out, m, cpy)   ,n); break;

    case 12: 	    TMBENCH("\nbitnzpack32     ",l=bitnzpack32(     in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnzunpack32(    out, m, cpy)   ,n); break;
    case 13: 	    TMBENCH("\nbitnzpack128v32 ",l=bitnzpack128v32( in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnzunpack128v32(out, m, cpy)   ,n); break;

    case 14: 	    TMBENCH("\nbitndpack32     ",l=bitndpack32(     in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitndunpack32(    out, m, cpy)   ,n); break;
    case 15: 	    TMBENCH("\nbitndpack128v32 ",l=bitndpack128v32( in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitndunpack128v32(out, m, cpy)   ,n); break;

    case 16: 	    TMBENCH("\nbitnd1pack32    ",l=bitnd1pack32(     in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnd1unpack32(    out, m, cpy)   ,n); break;
    case 17: 	    TMBENCH("\nbitnd1pack128v32",l=bitnd1pack128v32( in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnd1unpack128v32(out, m, cpy)   ,n); break;
    case 18: 	    TMBENCH("\np4nsenc32       ",l=p4nsenc32(       in, m, out)  ,n); 	    pr(l,n); TMBENCH("",p4nsdec32(         out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 19: 	    TMBENCH("\np4nenc256v32    ", l=p4nenc256v32(    in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4ndec256v32(     out, m, cpy)   ,n); break;
    case 20: 	    TMBENCH("\np4nzenc256v32   ", l=p4nzenc256v32(   in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nzdec256v32(    out, m, cpy)   ,n); break;
    case 21: 		TMBENCH("\np4ndenc256v32   ", l=p4ndenc256v32(   in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nddec256v32(    out, m, cpy)   ,n); break;
    case 22: 	    TMBENCH("\nbitnpack256v32  ", l=bitnpack256v32(  in, m, out)  ,n);   	pr(l,n); TMBENCH("",bitnunpack256v32( out, m, cpy)   ,n); break;
    case 23: 	    TMBENCH("\nbitnzpack256v32 ", l=bitnzpack256v32( in, m, out)  ,n);   	pr(l,n); TMBENCH("",bitnzunpack256v32(out, m, cpy)   ,n); break;
    case 24: 	    TMBENCH("\nbitndpack256v32 ", l=bitndpack256v32( in, m, out)  ,n);   	pr(l,n); TMBENCH("",bitndunpack256v32(out, m, cpy)   ,n); break;
    case 25: 	    TMBENCH("\nbitnd1pack256v32",l=bitnd1pack256v32( in, m, out) ,n);   	pr(l,n); TMBENCH("",bitnd1unpack256v32(out, m, cpy)   ,n); break;
      #endif
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  			    	printf("%10u %5.1f%%", n, (double)100.0); return n;
	default: return l;
  }
  printf(" %s ", inname);
  memcheck(in,m*4,cpy);
  return l;
}

unsigned bench16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname) { 
  unsigned l,m=n/2;
  memrcpy(cpy,in,n); 
  switch(id) {
    case 1: 	    TMBENCH("\np4nenc16        ",l=p4nenc16(        in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4ndec16(         out, m, cpy)   ,n); break;
    case 2: 	    TMBENCH("\np4nenc128v16    ",l=p4nenc128v16(    in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4ndec128v16(     out, m, cpy)   ,n); break;

    case 3: 		TMBENCH("\np4nzenc16       ",l=p4nzenc16(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nzdec16(        out, m, cpy)   ,n); break;
    case 4: 	    TMBENCH("\np4nzenc128v16   ",l=p4nzenc128v16(   in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nzdec128v16(    out, m, cpy)   ,n); break;

    case 5: 		TMBENCH("\np4ndenc16       ",l=p4ndenc16(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nddec16(        out, m, cpy)   ,n); break;
    case 6: 		TMBENCH("\np4ndenc128v16   ",l=p4ndenc128v16(   in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nddec128v16(    out, m, cpy)   ,n); break;

    case 7: 		TMBENCH("\np4nd1enc16      ",l=p4nd1enc16(       in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nd1dec16(       out, m, cpy)   ,n); break;
    case 8: 		TMBENCH("\np4nd1enc128v16  ",l=p4nd1enc128v16(   in, m, out)  ,n); 		pr(l,n); TMBENCH("",p4nd1dec128v16(   out, m, cpy)   ,n); break;
    case 9: 	    TMBENCH("\nvbzenc16        ",l=vbzenc16(        in, m, out,0)-out,n); 	pr(l,n); TMBENCH("",vbzdec16(         out, m, cpy,0) ,n); break;
	
    case 10: 	    TMBENCH("\nbitnpack16      ",l=bitnpack16(      in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnunpack16(     out, m, cpy)   ,n); break;
    case 11: 	    TMBENCH("\nbitnpack128v16  ",l=bitnpack128v16(  in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnunpack128v16( out, m, cpy)   ,n); break;

    case 12: 	    TMBENCH("\nbitnzpack16     ",l=bitnzpack16(     in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnzunpack16(    out, m, cpy)   ,n); break;
    case 13: 	    TMBENCH("\nbitnzpack128v16 ",l=bitnzpack128v16( in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnzunpack128v16(out, m, cpy)   ,n); break;

    case 14: 	    TMBENCH("\nbitndpack16     ",l=bitndpack16(     in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitndunpack16(    out, m, cpy)   ,n); break;
    case 15: 	    TMBENCH("\nbitndpack128v16 ",l=bitndpack128v16( in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitndunpack128v16(out, m, cpy)   ,n); break;

    case 16: 	    TMBENCH("\nbitnd1pack16    ",l=bitnd1pack16(     in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnd1unpack16(    out, m, cpy)   ,n); break;
    case 17: 	    TMBENCH("\nbitnd1pack128v16",l=bitnd1pack128v16( in, m, out)  ,n);	    pr(l,n); TMBENCH("",bitnd1unpack128v16(out, m, cpy)   ,n); break;
    case 18: 	    TMBENCH("\np4nsenc16       ",l=p4nsenc16(        in, m, out)  ,n); 	pr(l,n); TMBENCH("",p4nsdec16(         out, m, cpy)   ,n); break;
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  				printf("%10u %5.1f%%", n, (double)100.0); return n;
	default: return l;
  }
  printf(" %s ", inname);
  memcheck(in,m*2,cpy);
  return l;
}

int main(int argc, char* argv[]) {
  unsigned trips = 4,cmp=1, b = 1 << 31, esize=4, lz=0, fno,idmin=1,idmax=-1, usize=4, fsize=0;
  unsigned char *in,*out,*cpy;
  
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"blocsize", 	0, 0, 'b'}, {0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "B:ce:E:f:i:I:ms:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;								
      case 'e': idmin  = atoi(optarg); if(idmax == -1) idmax = idmin; break;
      case 'E': idmax  = atoi(optarg);  break;
      case 'i': tm_rep = atoi(optarg); if(!tm_rep) tm_rep=1,trips=1;  break;
      case 'I': trips  = atoi(optarg); if(!trips) trips=1;  break;
      case 's': usize = argtoi(optarg,1); 	break;
      case 'B': b = argtoi(optarg,1); 	break;
      case 'c': cmp++; 				  	break;
      case 'm': mcpy++; 			  	break;
      case 'f': fsize = argtoi(optarg,1); break;
    }
  }
  if(idmax == -1) idmax = ID_MEMCPY;
  if(idmax < idmin) idmax = idmin;
  if(argc - optind < 1) { fprintf(stderr, "raw file not specified\n"); 
    fprintf(stderr, "usage: icpp file [-s2|-s4|-s8|-f8] -e# -E#\n");
    fprintf(stderr, "Integer:-s2=16 -s4=32 -s8=64 bits. floating-point:-f8=64 bits. #:function number  -e:start -E:end\n"); 	
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
    n = fread(in, 1, n, fi);								 			
    fclose(fi);
    if(n <= 0) exit(0); 
    if(fno == optind)
      tm_init(trips, 1); 
    printf("function           E MB/s   size      ratio   D MB/s");      
    for(i=idmin; i <= idmax; i++) 
	  if(fsize) switch(fsize) {
        case 8: benchf64(in,n,out,cpy,i,inname); break;    
        default: die("floating point size must be 8\n"); 
	  }
      else switch(usize) {
        case 2: bench16(in,n,out,cpy,i,inname); break;  
        case 4: bench32(in,n,out,cpy,i,inname); break;    
        case 8: bench64(in,n,out,cpy,i,inname); break;    
        default: die("integer size must be 2, 4 or 8\n"); 
      }
    printf("\n");      
  }
}

