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
  #if !defined(_WIN32)
#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/param.h>
  #else
#include <io.h>
#include <fcntl.h>
#define srand48(x)  srand(x)
#define drand48()   ((double)(rand()) / RAND_MAX)
#define __off64_t   _off64_t  
  #endif 

#include "bitutil.h"
#include "bitpack.h"
#include "vp4.h"
#include "vint.h"
#include "fp.h"
#include "vsimple.h"

//--------------------------------------- Zipfian generator --------------------------------------------------------
#include <math.h>
static double    a = 1.5;
static unsigned xbits[65],verbose;
void stprint(char *s) {
  int m;
  uint64_t t=0; 
  for(m = 0; m <= 64; m++) 
    t += xbits[m];
  printf("\n%s bits histogram:",s); 
  for(m = 0; m <= 64; m++) 
    if(xbits[m]) printf("%d:%.2f%% ", m, (double)xbits[m]*100/t); printf("\n");
}

int dcmp(double *a, double *b) {
  if(*a < *b) return -1;
  if(*a > *b) return  1;
  return 0;
}

void zipu32(unsigned *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i; 
  unsigned m = x2 - x1 + 1;
  double   prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m); 

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

void zipu64(uint64_t *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i; 
  unsigned m = x2 - x1 + 1;
  double   prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m); 

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

void zipd64(double *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i; 
  unsigned m = x2 - x1 + 1;
  double   prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m); 

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

void tms64(double *a, unsigned n, unsigned x1, unsigned x2) {
  double freq = drand48()*0.5 + 0.1, amp = drand48()+ 0.5;
  int i; for(i=0; i < n; i++) a[i] = x1 + sin(i * freq) * amp;
}

#define OVD (10*MB)
static unsigned rm=0,rx=255;
								
unsigned datagen(unsigned char *in, unsigned n, int isize, double be_mindelta) {	
  unsigned char *ip; 													 printf("zipf alpha=%.2f range[%u..%u].n=%u\n ", a, rm, rx, n);
  int mindelta = be_mindelta,i;
  //in = malloc(n*isize+OVD); 												if(!in) die("malloc err=%u", n*isize);
  switch(isize) {
	case 4: zipu32((unsigned *)in, n, a, rm, rx);    								//{ int i; for(i = 0; i < n; i++) in[i] = i; } 	//
	                                                                            for(i = 1; i <= n; i++) xbits[bsr32(ctou32(in+i*4))]++; 
      if(mindelta == 0 || mindelta == 1) {                                                       	
        unsigned *ip = (unsigned *)in, v;										stprint("delta"); 
        for(ip[0]=0,v = 1; v < n; v++) { 
          ip[v] += ip[v-1] + mindelta;                                     if(ip[v]>=(1u<<31)) die("overflow generating sorted array %d\n", ip[v]);  
        }
      } else stprint("");
      break;
	case 8: zipu64((uint64_t *)in, n, a, rm, rx);    								//{ int i; for(i = 0; i < n; i++) in[i] = i; } 	//
	                                                                           for(i = 1; i <= n; i++) xbits[bsr64(ctou64(in+i*8))]++; 
      if(mindelta == 0 || mindelta == 1) {                                                       	
        uint64_t *ip = (uint64_t *)in, v;										stprint("delta"); 
        for(ip[0]=0,v = 1; v < n; v++) { 
          ip[v] += ip[v-1] + mindelta;                                     if(ip[v]>=(1u<<31)) die("overflow generating sorted array %d\n", ip[v]);  
        }
      } else stprint("");
      break;
    case -8: zipd64((double *)in, n, a, rm, rx);    								//{ int i; for(i = 0; i < n; i++) in[i] = i; } 	//
	                                                                        for(i = 1; i <= n; i++) xbits[bsr64(ctou64(in+i*8))]++; 
      /*if(be_mindelta >= 0 && be_mindelta <= 1) {                                                       	
        double *ip = (double *)in, v;										stprint("delta"); 
        for(ip[0]=0,v = 1; v < n; v++) { 
          ip[v] = ip[v]+ip[v-1] + be_mindelta;                                     //if(ip[v]>=(double)(1u<<31)) die("overflow generating sorted array %.2f\n", ip[v]);  
        }
      } else*/ 
	  stprint("");
      break;
  }	  
  return n*abs(isize);
}

int mdelta;
//       0       1        2         3         4         5         6         7,       8        9        10
enum { T_0, T_UINT8, T_UINT16, T_UINT24, T_UINT32, T_UINT40, T_UINT48, T_UINT56, T_UINT64, T_FLOAT, T_DOUBLE, T_CHAR, T_TXT, T_TST };
#define IPUSH(in,n,isize, nmax,u) { if(n >= nmax) { nmax = nmax?(nmax << 1):(1<<20); in = realloc(in, nmax*isize+OVD); if(!in) die("malloc err=%u", nmax); }\
                                  ctou32(in+n*isize) = u; n++;\
								}

unsigned befgen(unsigned char **_in, unsigned n, int fmt, int isize, FILE *fi, int kid, int skiph, int decs, int divs, int mdelta) {	
  unsigned char *in = *_in,*ip; 
  unsigned nmax = 0;           
  #define LSIZE 1024
  char s[LSIZE+1];
  double pre;

  n = 0;																	 
  while(skiph-- > 0) {
	fgets(s, LSIZE, fi);													if(verbose>=5 && n < 100 ||verbose>5) printf("skip first line\n");
  }
  pre = decs?pow(10.0f,(float)decs):1;
  pre /= divs;
  switch(fmt) {	  
    case T_TXT:																if(verbose) printf("reading text lines. pre=%.2f\n", pre);	
      while(fgets(s, LSIZE, fi)) {
		unsigned char *p = s,*q;
		int k;
        s[strlen(s) - 1] = 0; 					                      	
        for(k = 0; *p; ) {
          if(isize > 0) {
            while(*p && (*p < '0' || *p > '9') ) *p++;
		    uint64_t u = strtoull(p, &q, 10) - mdelta;
		    if(*q == '.')
			  u = round(strtod(p, &q)*pre) - mdelta;		    
		    p = q;
		    if(++k == kid) {														
			  IPUSH(in,n,isize,nmax,u);										if(verbose>=5 && n < 100 || verbose>=9) printf("%llu ", (uint64_t)u);
              break;			
            }  			
            while(*p && (*p >= '0' && *p <= '9' || *p == '.') ) *p++;
          } else {
            while(*p && (*p < '0' || *p > '9' || *p == '.') ) *p++;
		    double d = strtod(p, &q)*pre - mdelta;  					 	
		    p = q;
		    if(++k == kid) {														
			  IPUSH(in,n,-isize,nmax,d);									if(verbose>=5 && n < 100 || verbose>=9) printf("%.4f ", d);
              break;			
            }  			
          }
        } 
      }  
      break;
    case T_CHAR:                                                         	if(verbose) printf("reading char file. pre=%.2f\n", pre);		
      for(;;) {
        char *p = s,*q;
        int c;
        if(isize > 0) {
		  uint64_t u;
          while((c = getc(fi)) >= '0' && c <= '9') 
		    if(p - s < LSIZE) *p++ = c;
          *p = 0;
          u = strtoull(s, &p, 10) - mdelta;
	      if(*p == '.') 
		    u = round(strtod(s, &q)*pre) - mdelta;
          IPUSH(in,n,isize,nmax,u);		
        } else {
          while((c = getc(fi)) >= '0' && c <= '9' || c == '.')
		    if(p - s < LSIZE) *p++ = c;
          *p = 0;
		  double d = strtod(s, &p) - mdelta;
          IPUSH(in,n,-isize,nmax,d);		
        }
        if(c == EOF) break;
      }
      break;
    /*case T_UINT8: {
      unsigned char u;
      while(fread(&u, 1, 1, fi)>0)
        IPUSH(in,n,isize,nmax, u-mdelta);		
    } break;
    case T_UINT16: {
      unsigned short u;
      while(fread(&u, sizeof(u), 1, fi)>0)
        IPUSH(in,n,isize,nmax, u-mdelta);		
    } break;*/
	default: die("unknown data format %d\n", fmt);
  }
  *_in = in;
  return n*abs(isize);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
int mcpy=1;

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

void pr(unsigned l, unsigned n) { double r = (double)l*100.0/n; if(r>0.1) printf("%10u %6.2f%% ", l, r);else printf("%10u %7.3f%%", l, r); fflush(stdout); }

#define ID_MEMCPY 40
unsigned bench64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname) { 
  unsigned l,m=n/8;

  memrcpy(cpy,in,n); 
  switch(id) {
    case  1: 	    TMBENCH("\np4nenc64        ",l=p4nenc64(       in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4ndec64(         out, m, cpy)   ,n); break;
    case  2: 	    TMBENCH("\np4nenc128v64    ",l=p4nenc128v64(   in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4ndec128v64(     out, m, cpy)   ,n); break;

    case  3: 		TMBENCH("\np4nzenc64       ",l=p4nzenc64(      in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nzdec64(        out, m, cpy)   ,n); break;
 
    case  5: 		TMBENCH("\np4ndenc64       ",l=p4ndenc64(      in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nddec64(        out, m, cpy)   ,n); break;

    case  7: 		TMBENCH("\np4nd1enc64      ",l=p4nd1enc64(     in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nd1dec64(       out, m, cpy)   ,n); break;
    case  9: 	    TMBENCH("\np4nsenc64       ",l=p4nsenc64(      in, m, out)  ,n); 	    pr(l,n); TMBENCH2("",p4nsdec64(        out, m, cpy)   ,n); break;

    case 10: 	    TMBENCH("\nvbzenc64        ",l=vbzenc64(       in, m, out,0)-out,n); 	pr(l,n); TMBENCH2("",vbzdec64(         out, m, cpy,0) ,n); break;
    case 11: 	    TMBENCH("\nvbddenc64       ",l=vbddenc64(      in, m, out,0)-out,n); 	pr(l,n); TMBENCH2("",vbdddec64(        out, m, cpy,0) ,n); break;
	
    case 12: 	    TMBENCH("\nbitnpack64      ",l=bitnpack64(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnunpack64(     out, m, cpy)   ,n); break;
    case 13: 	    TMBENCH("\nbitnpack128v64  ",l=bitnpack128v64( in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnunpack128v64( out, m, cpy)   ,n); break;

    case 14: 	    TMBENCH("\nbitnzpack64     ",l=bitnzpack64(    in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnzunpack64(    out, m, cpy)   ,n); break;

    case 16: 	    TMBENCH("\nbitndpack64     ",l=bitndpack64(    in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitndunpack64(    out, m, cpy)   ,n); break;

    case 18: 	    TMBENCH("\nbitnd1pack64    ",l=bitnd1pack64(   in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnd1unpack64(   out, m, cpy)   ,n); break;
    //case 20: 	    TMBENCH("\nbitnfpack64     ",l=bitnfpack64(  in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnfunpack64(     out, m, cpy)   ,n); break;
    
    //case 30: 	    TMBENCH("\nbitg0enc64      ",l=bitg0enc64(     in, m, out,0),n); 	pr(l,n); TMBENCH2("",bitg0dec64(       out, m, cpy,0) ,n); break;
    case 30: 	    TMBENCH("\nvsenc64         ",l=vsenc64(        in, m, out)-out,n); 	    pr(l,n); TMBENCH2("",vsdec64(          out, m, cpy) ,n); break;
    case 31: 	    TMBENCH("\nbitgenc64       ",l=bitgenc64(      in, m, out,0),n); 	    pr(l,n); TMBENCH2("",bitgdec64(        out, m, cpy,0) ,n); break;
	// Function for floating point compression 
    case 32: 	    TMBENCH("\nfppenc64        ",l=fppenc64(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("",fppdec64(         out, m, cpy,0) ,n); break;
    case 33: 	    TMBENCH("\nfpgenc64        ",l=fpgenc64(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("",fpgdec64(         out, m, cpy,0) ,n); break;
    case 34: 		TMBENCH("\nfpddenc64       ",l=fpddenc64(      in, m, out,0),n); 	    pr(l,n); TMBENCH2("",fpdddec64(        out, m, cpy,0) ,n); break;
    case 35: 		TMBENCH("\nfpfcmenc64      ",l=fpfcmenc64(     in, m, out,0),n); 	    pr(l,n); TMBENCH2("",fpfcmdec64(       out, m, cpy,0) ,n); break;
    case 36: 		TMBENCH("\nfpdfcmenc64     ",l=fpdfcmenc64(    in, m, out,0),n); 	    pr(l,n); TMBENCH2("",fpdfcmdec64(      out, m, cpy,0) ,n); break;
//   case 29: 	    TMBENCH("\nbitnfpack128v64 ",l=bitnfpack128v64( in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnfunpack128v64( out, m, cpy)   ,n); break;
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  			    printf("%10u %6.2f%%", n, (double)100.0); return n;
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
    case 1: 	    TMBENCH("\np4nenc32        ",l=p4nenc32(         in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4ndec32(          out, m, cpy)   ,n); break;
    case 2: 	    TMBENCH("\np4nenc128v32    ",l=p4nenc128v32(     in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4ndec128v32(      out, m, cpy)   ,n); break;

    case 3: 		TMBENCH("\np4nzenc32       ",l=p4nzenc32(        in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nzdec32(         out, m, cpy)   ,n); break;
    case 4: 	    TMBENCH("\np4nzenc128v32   ",l=p4nzenc128v32(    in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nzdec128v32(     out, m, cpy)   ,n); break;
 
    case 5: 		TMBENCH("\np4ndenc32       ",l=p4ndenc32(        in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nddec32(         out, m, cpy)   ,n); break;
    case 6: 		TMBENCH("\np4ndenc128v32   ",l=p4ndenc128v32(    in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nddec128v32(     out, m, cpy)   ,n); break;

    case 7: 		TMBENCH("\np4nd1enc32      ",l=p4nd1enc32(       in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nd1dec32(        out, m, cpy)   ,n); break;
    case 8: 		TMBENCH("\np4nd1enc128v32  ",l=p4nd1enc128v32(   in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nd1dec128v32(    out, m, cpy)   ,n); break;
    case 9: 	    TMBENCH("\np4nsenc32       ",l=p4nsenc32(        in, m, out)  ,n); 	    pr(l,n); TMBENCH2("",p4nsdec32(         out, m, cpy)   ,n); break;

    case 10: 	    TMBENCH("\nvbzenc32        ",l=vbzenc32(         in, m, out,0)-out,n); 	pr(l,n); TMBENCH2("",vbzdec32(          out, m, cpy,0) ,n); break;
    case 11: 	    TMBENCH("\nvbddenc32       ",l=vbddenc32(        in, m, out,0)-out,n); 	pr(l,n); TMBENCH2("",vbdddec32(         out, m, cpy,0) ,n); break;
	
    case 12: 	    TMBENCH("\nbitnpack32      ",l=bitnpack32(       in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnunpack32(      out, m, cpy)   ,n); break;
    case 13: 	    TMBENCH("\nbitnpack128v32  ",l=bitnpack128v32(   in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnunpack128v32(  out, m, cpy)   ,n); break;

    case 14: 	    TMBENCH("\nbitnzpack32     ",l=bitnzpack32(      in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnzunpack32(     out, m, cpy)   ,n); break;
    case 15: 	    TMBENCH("\nbitnzpack128v32 ",l=bitnzpack128v32(  in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnzunpack128v32( out, m, cpy)   ,n); break;

    case 16: 	    TMBENCH("\nbitndpack32     ",l=bitndpack32(      in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitndunpack32(     out, m, cpy)   ,n); break;
    case 17: 	    TMBENCH("\nbitndpack128v32 ",l=bitndpack128v32(  in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitndunpack128v32( out, m, cpy)   ,n); break;

    case 18: 	    TMBENCH("\nbitnd1pack32    ",l=bitnd1pack32(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnd1unpack32(    out, m, cpy)   ,n); break;
    case 19: 	    TMBENCH("\nbitnd1pack128v32",l=bitnd1pack128v32( in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnd1unpack128v32(out, m, cpy)   ,n); break;
    //case 20: 	    TMBENCH("\nbitnfpack32     ",l=bitnfpack32(      in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnfunpack32(     out, m, cpy)   ,n); break;
    //case 21: 	    TMBENCH("\nbitnfpack128v32 ",l=bitnfpack128v32(  in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnfunpack128v32( out, m, cpy)   ,n); break;
	  #if defined(__AVX2__) && defined(USE_AVX2)
    case 22: 	    TMBENCH("\np4nenc256v32    ", l=p4nenc256v32(    in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4ndec256v32(      out, m, cpy)   ,n); break;
    case 23: 	    TMBENCH("\np4nzenc256v32   ", l=p4nzenc256v32(   in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nzdec256v32(     out, m, cpy)   ,n); break;
    case 24: 		TMBENCH("\np4ndenc256v32   ", l=p4ndenc256v32(   in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nddec256v32(     out, m, cpy)   ,n); break;
    case 25: 	    TMBENCH("\nbitnpack256v32  ", l=bitnpack256v32(  in, m, out)  ,n);   	pr(l,n); TMBENCH2("",bitnunpack256v32(  out, m, cpy)   ,n); break;
    case 26: 	    TMBENCH("\nbitnzpack256v32 ", l=bitnzpack256v32( in, m, out)  ,n);   	pr(l,n); TMBENCH2("",bitnzunpack256v32( out, m, cpy)   ,n); break;
    case 27: 	    TMBENCH("\nbitndpack256v32 ", l=bitndpack256v32( in, m, out)  ,n);   	pr(l,n); TMBENCH2("",bitndunpack256v32( out, m, cpy)   ,n); break;
    case 28: 	    TMBENCH("\nbitnd1pack256v32", l=bitnd1pack256v32(in, m, out)  ,n);   	pr(l,n); TMBENCH2("",bitnd1unpack256v32(out, m, cpy)   ,n); break;
    //case 29: 	    TMBENCH("\nbitnfpack256v32 ",l=bitnfpack256v32( in, m, out)  ,n);   	pr(l,n); TMBENCH2("",bitnfunpack256v32( out, m, cpy)   ,n); break;
      #endif
    //case 30: 	    TMBENCH("\nbitg0enc32      ", l=bitg0enc32(      in, m, out,0),n); 	    pr(l,n); TMBENCH2("",bitg0dec32(        out, m, cpy,0) ,n); break;
    case 30: 	    TMBENCH("\nvsenc32         ", l=vsenc32(         in, m, out)-out,n); 	    pr(l,n); TMBENCH2("",vsdec32(           out, m, cpy) ,n); break;
    case 31: 	    TMBENCH("\nbitgenc32       ", l=bitgenc32(       in, m, out,0),n); 	    pr(l,n); TMBENCH2("",bitgdec32(         out, m, cpy,0) ,n); break;

    //case 27: 	    TMBENCH("\nbitdienc32      ",l=bitdienc32(      in, m, out,0,0),n); 	pr(l,n); memcpy(cpy, out, n);bitddec32( cpy, m,0); TMBENCH2("",bitddec32( out, m,0)   ,n); break;
    //case 28: 	    TMBENCH("\nbitddenc32      ",l=bitddenc32(      in, m, out,0,0),n); 	pr(l,n); memcpy(cpy, out, n);bitdddec32(cpy, m,0); TMBENCH2("",bitdddec32(out, m,0)   ,n); break;
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  			    	printf("%10u %6.2f%%", n, (double)100.0); return n;
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
    case 1: 	    TMBENCH("\np4nenc16        ",l=p4nenc16(        in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4ndec16(         out, m, cpy)   ,n); break;
    case 2: 	    TMBENCH("\np4nenc128v16    ",l=p4nenc128v16(    in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4ndec128v16(     out, m, cpy)   ,n); break;

    case 3: 		TMBENCH("\np4nzenc16       ",l=p4nzenc16(       in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nzdec16(        out, m, cpy)   ,n); break;
    case 4: 	    TMBENCH("\np4nzenc128v16   ",l=p4nzenc128v16(   in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nzdec128v16(    out, m, cpy)   ,n); break;

    case 5: 		TMBENCH("\np4ndenc16       ",l=p4ndenc16(       in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nddec16(        out, m, cpy)   ,n); break;
    case 6: 		TMBENCH("\np4ndenc128v16   ",l=p4ndenc128v16(   in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nddec128v16(    out, m, cpy)   ,n); break;

    case 7: 		TMBENCH("\np4nd1enc16      ",l=p4nd1enc16(       in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nd1dec16(       out, m, cpy)   ,n); break;
    case 8: 		TMBENCH("\np4nd1enc128v16  ",l=p4nd1enc128v16(   in, m, out)  ,n); 		pr(l,n); TMBENCH2("",p4nd1dec128v16(   out, m, cpy)   ,n); break;
    case 9: 	    TMBENCH("\nvbzenc16        ",l=vbzenc16(        in, m, out,0)-out,n); 	pr(l,n); TMBENCH2("",vbzdec16(         out, m, cpy,0) ,n); break;
	
    case 10: 	    TMBENCH("\nbitnpack16      ",l=bitnpack16(      in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnunpack16(     out, m, cpy)   ,n); break;
    case 11: 	    TMBENCH("\nbitnpack128v16  ",l=bitnpack128v16(  in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnunpack128v16( out, m, cpy)   ,n); break;

    case 12: 	    TMBENCH("\nbitnzpack16     ",l=bitnzpack16(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnzunpack16(    out, m, cpy)   ,n); break;
    case 13: 	    TMBENCH("\nbitnzpack128v16 ",l=bitnzpack128v16( in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnzunpack128v16(out, m, cpy)   ,n); break;

    case 14: 	    TMBENCH("\nbitndpack16     ",l=bitndpack16(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitndunpack16(    out, m, cpy)   ,n); break;
    case 15: 	    TMBENCH("\nbitndpack128v16 ",l=bitndpack128v16( in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitndunpack128v16(out, m, cpy)   ,n); break;

    case 16: 	    TMBENCH("\nbitnd1pack16    ",l=bitnd1pack16(     in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnd1unpack16(    out, m, cpy)   ,n); break;
    case 17: 	    TMBENCH("\nbitnd1pack128v16",l=bitnd1pack128v16( in, m, out)  ,n);	    pr(l,n); TMBENCH2("",bitnd1unpack128v16(out, m, cpy)   ,n); break;
    case 18: 	    TMBENCH("\np4nsenc16       ",l=p4nsenc16(        in, m, out)  ,n); 	    pr(l,n); TMBENCH2("",p4nsdec16(         out, m, cpy)   ,n); break;
    case ID_MEMCPY: if(!mcpy) return l; TMBENCH("\nmemcpy          ",libmemcpy(  in,out,n) ,n);  				printf("%10u %6.2f%%", n, (double)100.0); return n;
	default: return l;
  }
  printf(" %s ", inname);
  memcheck(in,m*2,cpy);
  return l;
}

void usage(char *pgm) {
  fprintf(stderr, "\nIcApp Copyright (c) 2013-2018 Powturbo %s\n", __DATE__);
  fprintf(stderr, "Usage: %s [options] [file]\n", pgm);
  fprintf(stderr, " -b#s     # = blocksize (default filesize,). max=1GB\n");
  fprintf(stderr, " -B#s     # = max. benchmark filesize (default 1GB) ex. -B4G\n");
  fprintf(stderr, "          s = modifier s:K,M,G=(1000, 1.000.000, 1.000.000.000) s:k,m,h=(1024,1Mb,1Gb). (default m) ex. 64k or 64K\n");
  fprintf(stderr, "Benchmark:\n");
  fprintf(stderr, " -i#/-j#  # = Minimum  de/compression iterations per run (default=auto)\n");
  fprintf(stderr, " -I#/-J#  # = Number of de/compression runs (default=3)\n");
  fprintf(stderr, "File format:\n");
  fprintf(stderr, " -f[Xx[k][H]][.d][s]\n");
  fprintf(stderr, "    X = file format:\n");
  fprintf(stderr, "        t = text:one integer per line, k=column number in multiple columns line\n");
  fprintf(stderr, "        c = text:integers separated by non digit char\n");
  fprintf(stderr, "    x = entry format\n");
  fprintf(stderr, "        [c=int8], [s=int16], [u=int32(default)], [s=int64], [f:float] [d:double]\n");
  fprintf(stderr, "    .# = decimal digits (default 2). Convert dec. numbers to integers\n");
  fprintf(stderr, "    H = skip first line\n");  
  fprintf(stderr, "Output:\n");
  fprintf(stderr, " -v#      # = verbosity 0..3 (default 1)\n");
  fprintf(stderr, "----- arg. ZIPF specified --------------\n");
  fprintf(stderr, " -aF     F = zipfian distribution alpha ex. -a1.0 uniform -a1.5 skewed\n");
  fprintf(stderr, " -mNs    N = minimum number. s=(k,K, m,M, g,G\n");
  fprintf(stderr, " -MNs    N = maximum number\n");
  fprintf(stderr, " -nNs    N = number of integers to generate\n");
  fprintf(stderr, "Ex. ./icapp -a1.5 -m0 -M255 -n100M ZIPF\n");
  fprintf(stderr, "ex. ./icapp file -e2\n");
  fprintf(stderr, "ex. ./icbench -ftl.2 im.data\n");
  fprintf(stderr, "ex. ./icbench -ftd usages.dat\n");
  exit(0);
} 

int main(int argc, char* argv[]) {
  unsigned cmp=1, b = 1 << 31, lz=0, fno,idmin=1,idmax=-1,m=1000000; int isize=4,dfmt = 0,kid=1,skiph=0,decs=2,divs=1,be_mindelta=0;
  unsigned char *in=NULL,*out,*cpy;
  double mdelta=-10;
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"blocsize", 	0, 0, 'b'}, {0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "a:B:ce:E:f:F:i:j:I:J:m:M:n:s:v:y", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;								
      case 'e': idmin  = atoi(optarg); if(idmax == -1) idmax = idmin; break;
      case 'E': idmax  = atoi(optarg);  break;

      case 'F': { char *s = optarg; 
	         if(*s=='c') { dfmt = T_CHAR; s++;} 
		else if(*s=='t') { dfmt = T_TXT; if(*++s > '0' && *s <= '9') { kid = s[0] - '0'; s++; } } 
		else if(*s=='e') { dfmt = T_TST; s++; } 
		else if(*s=='b') { dfmt = 0;     s++; } 

		     if(*s=='f') isize = -4, s++; 
		else if(*s=='d') isize = -8, s++;
	    else if(*s=='c') isize =  1, s++;
		else if(*s=='s') isize =  2, s++;
		else if(*s=='u') isize =  4, s++;
		else if(*s=='l') isize =  8, s++;
		if(*s == '.') { if(*++s >= '0' && *s <= '9') { decs = s[0] - '0'; s++; } }
		if(*s == 'v') { divs = strtod(++s, &s); }
		if(*s == 'H') { skiph++; s++; }
	    switch(*s) { case 's': be_mindelta = 0; break; case 'S': be_mindelta = 1; break; case 'z': be_mindelta = 2; break; }
	  } break;

      case 'i': if((tm_rep  = atoi(optarg))<=0) 
		          tm_rep=tm_Rep=1;         	 break;
      case 'I': tm_Rep  = atoi(optarg);      break;
      case 'j': if((tm_rep2  = atoi(optarg))<=0) 
		          tm_rep2=tm_Rep2=1;         break;
      case 'J': tm_Rep2 = atoi(optarg);      break;
      case 's': isize = argtoi(optarg,1); 	 break;
      case 'B': b = argtoi(optarg,1); 	     break;
      case 'c': cmp++; 				  	     break;
      case 'y': mcpy=0; 			  	     break;
	  
      case 'a': a = strtod(optarg, NULL);    break;
      case 'd': mdelta  = strtod(optarg, NULL);break;
      case 'n': m       = argtoi(optarg,1);  break;
      case 'm': rm      = argtoi(optarg,1);  break;
      case 'M': rx      = argtoi(optarg,1);  break;
      case 'f': isize   = -argtoi(optarg,1); break;
      case 'v': verbose = atoi(optarg);      break;
      default: 
        usage(argv[0]);
        exit(0); 
    }
  }
  if(idmax == -1) idmax = ID_MEMCPY;
  if(idmax < idmin) idmax = idmin;
  if(argc - optind < 1) {
    usage(argv[0]);
    exit(0); 
  }																			if(verbose) printf("dfmt=%d,size=%d\n", dfmt, isize);
  for(fno = optind; fno < argc; fno++) {
    char *inname = argv[fno];  							
    int i,n; 
	long long flen;
	FILE *fi=NULL;
	if(!strcmp(inname,"ZIPF") || !strcmp(inname,"TMS")) flen = n = m*abs(isize);
    else {
      fi = fopen(inname, "rb"); 							if(!fi ) { perror(inname); continue; } 							
      if(dfmt) { 
	    n = flen = befgen(&in, 0, dfmt, isize, fi, kid, skiph, decs, divs, be_mindelta);
      } else { 
        fseek(fi, 0, SEEK_END); 
        flen = ftell(fi); 
	    fseek(fi, 0, SEEK_SET);
        if(flen > b) flen = b;
        n = flen; 
      } 
	}
    if(!in && !(in  = (unsigned char*)malloc(n+1024)))     {    fprintf(stderr, "malloc error 'in =%d'\n", n); exit(-1); } cpy = in;
    if(!(out =        (unsigned char*)malloc(flen*4/3+1024))) { fprintf(stderr, "malloc error 'out=%d'\n", n); exit(-1); } 
    if(cmp && !(cpy = (unsigned char*)malloc(n+1024)))     {    fprintf(stderr, "malloc error 'cpy=%d'\n", n); exit(-1); }
	if(fi) {
      if(!dfmt) n = fread(in, 1, n, fi);								 			
      fclose(fi);
	} else if(!strcmp(inname,"TMS") && abs(isize) == 8)
	  tms64(in, m, rm, rx);
	else 
	  datagen(in, m, isize, mdelta);
    if(n <= 0) exit(0); 
    if(fno == optind)
      tm_init(tm_Rep, 1); 
    printf("function           E MB/s   size      ratio   D MB/s");      
    for(i=idmin; i <= idmax; i++) 
	  if(isize<0) switch(isize) {
        case -8: bench64(in,n,out,cpy,i,inname); break;    
        default: die("floating point size must be 8\n"); 
	  }
      else switch(isize) {
        case 2: bench16(in,n,out,cpy,i,inname); break;  
        case 4: bench32(in,n,out,cpy,i,inname); break;    
        case 8: bench64(in,n,out,cpy,i,inname); break;    
        default: die("integer size must be 2, 4 or 8\n"); 
      }
    printf("\n");      
    free(in); free(out); free(cpy); in = out = cpy = NULL;
  }
}
