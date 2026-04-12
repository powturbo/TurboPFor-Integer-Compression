/**
    Copyright (C) powturbo 2015-2023
    SPDX-License-Identifier: GPL v2 License

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
**/
//  TurboPFor - "IcApp: Benchmark application"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>       /* time_t, struct tm, time, mktime */
  #ifdef __APPLE__
    #ifdef HAVE_MALLOC_MALLOC
#include <sys/malloc.h>
    #else
#include <malloc/malloc.h>
    #endif
  #else
#include <malloc.h>
  #endif
  #ifdef _MSC_VER
#include "../vs/getopt.h"
  #else
#include <getopt.h>
#endif
  #if defined(_WIN32)
#include <windows.h>
#define srand48(x)  srand(x)
#define drand48()   ((double)(rand()) / RAND_MAX)
#define __off64_t   _off64_t
  #else
#include <dlfcn.h> // dlopen
  #endif
#include <math.h> // pow,fabs

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>
#include "../include/ic.h"
#include "include_/iccodec.h"

#include "include_/conf.h"
#include "include_/time_.h"
#include "include_/bitutil_.h" //zigzag

#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#define max(x,y) (((x)>(y)) ? (x) : (y))
#endif

int verbose = 1,isa;
extern int tpbsize;
extern int bwtx, forcelzp;

//------------------------------ bits statistics --------------------------------------------------
static unsigned xbits[65],tbits[65],zbits[65];
unsigned histl8( uint8_t  *in, unsigned n) { unsigned i,l; uint64_t s=0; for(i = 0; i < 65; i++) xbits[i]=0; for(i = 0; i < n; i++) xbits[l=bsr8( in[i])]++,s+=l; return (s+7)/8; }
unsigned histl16(uint16_t *in, unsigned n) { unsigned i,l; uint64_t s=0; for(i = 0; i < 65; i++) xbits[i]=0; for(i = 0; i < n; i++) xbits[l=bsr16(in[i])]++,s+=l; return (s+7)/8; }
unsigned histl32(uint32_t *in, unsigned n) { unsigned i,l; uint64_t s=0; for(i = 0; i < 65; i++) xbits[i]=0; for(i = 0; i < n; i++) xbits[l=bsr32(in[i])]++,s+=l; return (s+7)/8; }
unsigned histl64(uint64_t *in, unsigned n) { unsigned i,l; uint64_t s=0; for(i = 0; i < 65; i++) xbits[i]=0; for(i = 0; i < n; i++) xbits[l=bsr64(in[i])]++,s+=l; return (s+7)/8; }

unsigned histz8( uint8_t  *in, unsigned n) { unsigned i,l; uint64_t s=0; uint8_t  x = 0; for(i = 0; i < 65; i++) zbits[i]=0; for(i = 0; i < n; i++) zbits[l=bsr8( zigzagenc8( in[i]-x))]++,s+=l,x=in[i]; return (s+7)/8; }
unsigned histz16(uint16_t *in, unsigned n) { unsigned i,l; uint64_t s=0; uint16_t x = 0; for(i = 0; i < 65; i++) zbits[i]=0; for(i = 0; i < n; i++) zbits[l=bsr16(zigzagenc16(in[i]-x))]++,s+=l,x=in[i]; return (s+7)/8; }
unsigned histz32(uint32_t *in, unsigned n) { unsigned i,l; uint64_t s=0; uint32_t x = 0; for(i = 0; i < 65; i++) zbits[i]=0; for(i = 0; i < n; i++) zbits[l=bsr32(zigzagenc32(in[i]-x))]++,s+=l,x=in[i]; return (s+7)/8; }
unsigned histz64(uint64_t *in, unsigned n) { unsigned i,l; uint64_t s=0; uint64_t x = 0; for(i = 0; i < 65; i++) zbits[i]=0; for(i = 0; i < n; i++) zbits[l=bsr64(zigzagenc64(in[i]-x))]++,s+=l,x=in[i]; return (s+7)/8; }

unsigned histt8( uint8_t  *in, unsigned n) { unsigned i,l; uint64_t s=0; for(i = 0; i < 65; i++) tbits[i]=0; for(i = 0; i < n; i++) l=tbits[in[i]?ctz8( in[i]): 8]++,s+=l; return (s+7)/8; }
unsigned histt16(uint16_t *in, unsigned n) { unsigned i,l; uint64_t s=0; for(i = 0; i < 65; i++) tbits[i]=0; for(i = 0; i < n; i++) l=tbits[in[i]?ctz16(in[i]):16]++,s+=l; return (s+7)/8; }
unsigned histt32(uint32_t *in, unsigned n) { unsigned i,l; uint64_t s=0; for(i = 0; i < 65; i++) tbits[i]=0; for(i = 0; i < n; i++) l=tbits[in[i]?ctz32(in[i]):32]++,s+=l; return (s+7)/8; }
unsigned histt64(uint64_t *in, unsigned n) { unsigned i,l; uint64_t s=0; for(i = 0; i < 65; i++) tbits[i]=0; for(i = 0; i < n; i++) l=tbits[in[i]?ctz64(in[i]):64]++,s+=l; return (s+7)/8; }

unsigned histx32(uint32_t *in, unsigned n) { unsigned i,l; uint64_t s=0; uint32_t x = 0; for(i = 0; i < 65; i++) zbits[i]=0; for(i = 0; i < n; i++) zbits[l=ctz32(in[i]^x)]++,s+=l,x=in[i]; return (s+7)/8; }

//----------------------------- Convert iso-8601 and similar formats to timestamp -------------------------
// Date separator : '.'. '/' or '-'
// Hour separator : ':'
// Fraction sep.: '.' or ','
// examples: "2020" "20211203" "20211022 11:09:45.1234",

uint64_t strtots(char *p, char **pq, int type) {  // string to timestamp
  struct   tm tm;
  uint64_t u;
  char     *s = p;
  int      frac = 0, c;

  memset(&tm, 0, sizeof(tm)); tm.tm_mday = 1;
  while(!isdigit(*p)) p++;
  u = strtoull(p, &p, 10);                  // first number

  if(     u <= 99) u += 2000;               // year  "yy": 00-99 -> 2000-2099
  else if(u >= 19710101 && u < 20381212) {  // date: "yyyymmdd"
    tm.tm_year =  u/10000;
    tm.tm_mon  = (u%10000)/100;   if(!tm.tm_mon  || tm.tm_mon  > 12) goto a; tm.tm_mon--;
    tm.tm_mday = u%10;            if(!tm.tm_mday || tm.tm_mday > 31) goto a;
    goto h;
  } else if(u < 1971 || u > 2099) goto a;   // invalid
  tm.tm_year = u;                           // year       "yyyy"

  c = *p;                                   // month,day: "mm.dd", "mm-dd", "mm/dd"
  if(c != '.' && c != '-' && c != '/') goto b;    tm.tm_mon    = strtoul(p+1, &p, 10);   if(!tm.tm_mon  || tm.tm_mon  > 12) goto a; tm.tm_mon--;
  if(c != '.' && c != '-' && c != '/') goto b;    tm.tm_mday   = strtoul(p+1, &p, 10);   if(!tm.tm_mday || tm.tm_mday > 31) goto a;
  if(c != '.' && c != '-' && c != '/') goto b;    h:tm.tm_hour = strtoul(p+1, &p, 10);

  if(tm.tm_hour <= 24 && *p == ':') {       // time ":hh:mm:ss.frac", ":hh:mm:ss,frac"
    tm.tm_min = strtoul(p+1, &p, 10);   if(tm.tm_min > 60) tm.tm_hour = tm.tm_min = 0;
    tm.tm_sec = strtoul(p+1, &p, 10);   if(tm.tm_sec > 60) tm.tm_hour = tm.tm_min = tm.tm_sec = 0;
    if(type > 0 && (*p == '.' || *p == ',' || *p == ':')) {
	  frac = strtoul(p+1, &p, 10);
	  if((c = p-(p+1)) > 6) frac /= 1000000;
	  else if(c > 3) frac /= 1000;
	}
  } else tm.tm_hour = 0;
  
  b:tm.tm_year -= 1900;
  u = mktime(&tm);
  u = u * 1000 + frac;                      // milliseconds
  a:*pq = p;                                if(verbose >= 9) printf("[%d-%d-%d %.2d:%.2d:%.2d.%d->%llx]\n", tm.tm_year, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, frac, u);
  return u;
}

//----------------------------- Zipfian + Timestamps generator --------------------------------------------------------
static double   a = 1.5;

void stprint(char *s, unsigned *xbits) {
  int      i;
  uint64_t t = 0;

  if(verbose <= 0) return;
  for(i = 0; i <= 64; i++)
    t += xbits[i];
  printf("------ %s bits histogram:",s);
  if(!t) {
    printf("ALL zero\n"); return;
  }
  printf("\n");
  for(i = 0; i <= 64; i++)
    if(xbits[i]) {
      double   f = (double)xbits[i]*100/(double)t;
      unsigned u = round(f);
      printf("%.2d:", i);
	  for(int j=0; j < u; j++) printf("#");
      if     (f >  10)     printf(" %.0f%% ", f);
      else if(f > 0.1)     printf(" %.1f%% ", f);
      else if(f > 0.01)    printf(" %.2f%% ", f);
      else if(f > 0.001)   printf(" %.3f%% ", f);
      else if(f > 0.0001)  printf(" %.4f%% ", f);
      else if(f > 0.00001) printf(" %.5f%% ", f);
      else                 printf(" %.6f%% ", f);
      printf("\n");
    }
}

int dcmp(double *a, double *b) {
  if(*a < *b) return -1;
  if(*a > *b) return  1;
  return 0;
}

void zipu8(uint8_t *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i;
  double   prob, cum, *zmap;

  x2 = x2>0xffu?0xffu:x2;
  if(x1 > x2) x1 = x2;
  unsigned m = x2 - x1 + 1;
  if(!(zmap = malloc(m*sizeof(zmap[0]))))
    die("mallo error %d\n", m);

  // generate initial sample (slow)
  srand48(1);
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++)
      cum += 1.0 / pow(i+1, alpha);
    cum = 1.0 / cum;
    for(zmap[0] = prob = cum,i = 1; i < m; i++)
	  zmap[i] = (prob += (cum / pow(i+1, alpha)));
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
	if(a[i] > x2) a[i] = x2;
  }
  free(zmap);
}

void zipu16(uint16_t *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i;
  double   prob, cum, *zmap;

  x2 = x2>0xffffu?0xffffu:x2;
  if(x1 > x2) x1 = x2;
  unsigned m = x2 - x1 + 1;
  if(!(zmap = (double *)malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m);

  // generate initial sample (slow)
  srand48(1);
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++)
      cum += 1.0 / pow(i+1, alpha);
    cum = 1.0 / cum;
    for(zmap[0] = prob = cum,i = 1; i < m; i++)
	  zmap[i] = (prob += (cum / pow(i+1, alpha)));
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

void zipu32(unsigned *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i;
  double   prob, cum, *zmap;
  if(x1 > x2) x1 = x2;
  unsigned m = x2 - x1 + 1;

  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m);

  // generate initial sample (slow)
  srand48(1);
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++)
      cum += 1.0 / pow(i+1, alpha);
    cum = 1.0 / cum;
    for(zmap[0] = prob = cum,i = 1; i < m; i++)
	  zmap[i] = (prob += (cum / pow(i+1, alpha)));
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

void zipf32(float *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i;
  double   prob, cum, *zmap;
  if(x1 > x2) x1 = x2;
  unsigned m = x2 - x1 + 1;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m);

  // generate initial sample (slow)
  srand48(1);
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++)
      cum += 1.0 / pow(i+1, alpha);
    cum = 1.0 / cum;
    for(zmap[0] = prob = cum,i = 1; i < m; i++)
	  zmap[i] = (prob += (cum / pow(i+1, alpha)));
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

void zipu64(uint64_t *a, unsigned n, double alpha, uint64_t x1, uint64_t x2) {
  int      i;
  double   prob, cum, *zmap;
  if(x1 > x2) x1 = x2;
  unsigned m = x2 - x1 + 1;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) die("mallo error %d\n", m);

  // generate initial sample (slow)
  srand48(1);
  if(alpha > 0) {
    for(cum = 0.0,i = 0; i < m; i++)
      cum += 1.0 / pow(i+1, alpha);
    cum = 1.0 / cum;
    for(zmap[0] = prob = cum,i = 1; i < m; i++)
	  zmap[i] = (prob += (cum / pow(i+1, alpha)));
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

void zipf64(double *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
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
    for(zmap[0] = prob = cum,i = 1; i < m; i++)
	  zmap[i] = (prob += (cum / pow(i+1, alpha)));
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

void tms64(uint64_t *a, unsigned n, unsigned x1, unsigned x2, double z) {  // generator for timestamps
  srand48(1);
  double freq = drand48()*0.5 + 0.1, amp = (drand48()*z + 0.5);
  int i;
  for(i = 0; i < n; i++) {
    double x = x1 + sin(i * freq) * amp;
    a[i] = x>x2?x2:x;
  }
}

#define OVD (10*MB)
static unsigned rm = 0, rx = 255;

unsigned datagen(unsigned char *in, unsigned n, int isize, double be_mindelta) {
  printf("zipf alpha=%.2f range[%u..%u].n=%u\n ", a, rm, rx, n);
  int mindelta = be_mindelta,i;

  switch(isize) {
    case 1: zipu8((uint8_t *)in, n, a, rm, rx);
                                                                                for(i = 1; i <= n; i++) xbits[bsr32(ctou8(in+i))]++;
      if(mindelta == 0 || mindelta == 1) {
        uint8_t *ip = (uint8_t *)in, v;                                         stprint("delta", xbits);
        for(ip[0]=0,v = 1; v < n; v++) {                                        if((unsigned)ip[v] + (unsigned)ip[v-1] + mindelta>=(1u<<8)) die("overflow generating sorted array %d\n", ip[v]);
          ip[v] += ip[v-1] + mindelta;                                          
        }
      } else stprint("", xbits);
      break;
    case 2: zipu16((unsigned short *)in, n, a, rm, rx);
                                                                                for(i = 1; i <= n; i++) xbits[bsr32(ctou16(in+i*2))]++;
      if(mindelta == 0 || mindelta == 1) {
        unsigned short *ip = (unsigned short *)in, v;                           stprint("delta", xbits);
        for(ip[0]=0,v = 1; v < n; v++) {                                        if((unsigned)ip[v] + (unsigned)ip[v-1] + mindelta >= (1u<<16)) die("overflow generating sorted array %d\n", ip[v]);
          ip[v] += ip[v-1] + mindelta;                                          
        }
      } else stprint("", xbits);
      break;
    case 4: zipu32((unsigned *)in, n, a, rm, rx);
                                                                                for(i = 1; i <= n; i++) xbits[bsr32(ctou32(in+i*4))]++;
      if(mindelta == 0 || mindelta == 1) {
        unsigned *ip = (unsigned *)in, v;                                       stprint(mindelta?"delta=1":"delta=0", xbits);
        for(ip[0]=0,v = 1; v < n; v++) {                                        if((uint64_t)ip[v] + (uint64_t)ip[v-1] + mindelta >= (1ull<<32)) die("overflow generating sorted array %d\n", ip[v]);  
          ip[v] += ip[v-1] + mindelta;                                          
        }
      } else stprint("", xbits);
      break;
    case 8: zipu64((uint64_t *)in, n, a, rm, rx);
                                                                                for(i = 1; i <= n; i++) xbits[bsr64(ctou64(in+i*8))]++;
      if(mindelta == 0 || mindelta == 1) {
        uint64_t *ip = (uint64_t *)in, v;                                       stprint("delta", xbits);
        for(ip[0]=0,v = 1; v < n; v++) {                                        if((uint64_t)ip[v] + (uint64_t)ip[v-1] + mindelta >= (1ull<<63)) die("overflow generating sorted array %lld\n", (int64_t)ip[v]); 
          ip[v] += ip[v-1] + mindelta;                                          
        }
      } else stprint("", xbits);
      break;
    case -4: zipf32((float *)in, n, a, rm, rx);
      stprint("", xbits);
      break;
    case -8: zipf64((double *)in, n, a, rm, rx);
      stprint("", xbits);
      break;
  }
  return n*abs(isize);
}

//       0       1        2         3         4         5         6         7,       8        9        10      11      12      13      14
enum { T_0, T_UINT8, T_UINT16, T_UINT24, T_UINT32, T_UINT40, T_UINT48, T_UINT56, T_UINT64, T_FLOAT, T_DOUBLE, T_CHAR, T_TXT, T_TIM32, T_TIM64, T_TST };

#define IPUSH(in,n,isize, nmax,u) { \
  if(n >= nmax) { nmax = nmax?(nmax << 1):(1<<20);\
  in = realloc(in, nmax*isize+OVD); if(!in) die("malloc err=%u", nmax); }\
  ctou64(in+n*isize) = u; n++;\
}

int    mdelta, elog2[16],nsd = 3, autoraz;
char   *keysep;

//float  errlimf[] = { 0,   1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9, 1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15, 1e-16 };
float  errlimf[] = { 0,   5e-1, 5e-2, 5e-3, 5e-4, 5e-5, 5e-6, 5e-7, 5e-8, 5e-9, 5e-10, 5e-11, 5e-12, 5e-13, 5e-14, 5e-15, 5e-16 };
double errlima[] = { 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001, 0.00000001 };
double zerrlim;
unsigned befgen(unsigned char **_in, unsigned n, int fmt, int isize, FILE *fi, int kid, int skiph, int decs, int divs, int mdelta, double errlim) {
  unsigned char *in = *_in,*ip=in;
  unsigned      nmax = 0, ovf=0;
  #define LSIZE (1024*16)
  char          s[LSIZE+1];
  double        pre;

  for(int i = 0; i < 16; i++)
	elog2[i] = -log(errlima[i])/log(2.0);

  n = 0;
  while(skiph-- > 0) {
    fgets(s, LSIZE, fi);                                                    if(verbose>=5 && n < 100 ||verbose>5) printf("skip first line\n");
  }
  if(decs) {
    pre = decs?pow(10.0f,(float)decs):1;
    pre /= divs;
  } else pre = 1;
  switch(fmt) {
    case T_TXT:
    case T_TIM32:
    case T_TIM64:                                                           if(verbose>1) printf("reading text lines. pre=%.2f, col=%d, sep=%s\n", pre, kid, keysep?keysep:"");
      while(fgets(s, LSIZE, fi)) {
        char *p = s,*q;
        int k = 0, keyid = 1, c;
        s[strlen(s) - 1] = 0;
        q = p;
        if(kid > 1)
          do {
            p = q;
            if(keysep && strchr(keysep,*q)) keyid++;
            q++;
          } while(*q && keyid != kid);
        if(fmt == T_TIM32 || fmt == T_TIM64) {
          while(!isdigit(*p)) p++;
          uint64_t u = strtots(p, &q, fmt == T_TIM64?1:0);
          if(fmt == T_TIM32) u /= 1000;
          a: IPUSH(in,n,isize,nmax,u);                          c=*q; *q=0; if(verbose>=5 && n < 100 || verbose>=9) printf("\'%s\'->%llu  ", p, u); *q = c;
        } else if(isize > 0) {
          while(!isdigit(*p) && *p != '-' && *p != '+') p++;
          uint64_t u = strtoll(p, &q, 10)*pre - mdelta;
          if(*q == '.')
            u = pre>1.0?round(strtod(p, &q)*pre):strtod(p, &q) - mdelta;
		  unsigned sovf=ovf;
          switch(isize) {
            case 1: { signed char  u8  = u; if(u8  != u) ovf++; IPUSH(in,n,isize,nmax, u8); } break;
            case 2: { short        u16 = u; if(u16 != u) ovf++; IPUSH(in,n,isize,nmax,u16); } break; //c=*q; *q=0; printf("\'%s\'->%lld:%u ", p, u, u16); *q = c; }
            case 4: { int          u32 = u; if(u32 != u) ovf++; IPUSH(in,n,isize,nmax,u32); } break;
            case 8: { long long    u64 = u; if(u64 != u) ovf++; IPUSH(in,n,isize,nmax,u64); } break;
          }
                                       if(verbose>=5 && n < 100 || verbose>=9) { c=*q; *q=0; printf("\'%s\'->%lld ", p, u); *q = c; }
        } else {
          while(*p && !isdigit(*p) && *p != '-' && *p != '.' && *p != '+') { if(keysep && strchr(keysep,*p)) keyid++; p++; }
		  char sbuf[65]; 
		  if(isize == -4) {
            float d = strtof(p, &q) - mdelta;                             *q = 0; strcpy(sbuf, p);
		    if(autoraz) {
		      *q = 0; while(q[-1] == '0') *q-- = 0;                       
			  char *t = q; for(;q > p; q--) if(*q == '.') break;
		      int e = t - q - 1;   	                                      //if(e < 0) e = 0; //{ printf("[%s,%d] ", p, e); die(" FATAL"); 			 }
			  if(e >= 0) {
		        float ed = errlimf[e], dd = d;                             
		        d = _fprazor32(d, ed, elog2[e]);    	                  //printf("[%s,%d,%g->%g] ", p, e, ed, d);//if(verbose > 3 && fabs(d-dd) > DBL_EPSILON) { printf("[%s,%d:%f ", p, e, d); printf("%f] ", d-dd);}
			  }
		    }
            uint32_t u;
            memcpy(&u,&d,sizeof(u));
            IPUSH(in,n,4,nmax,u);                                          if(verbose>=5 && n < 100 || verbose>=9) { c=*q; *q=0; float d; memcpy(&d,&u,sizeof(d)); printf("\'%s\'->%f  ", sbuf, d+mdelta); *q = c; }
		  } else {
            double d = strtod(p, &q) - mdelta;
		    if(autoraz) {
		      *q = 0; while(q[-1] == '0') *q-- = 0;
			  char *t = q; for(;q > p; q--) if(*q == '.') break;
		      int e = t - q - 1;   				
			  if(e >= 0) {
		        double ed = errlima[e],dd=d;             //printf("[%f ", d);
		        d = _fprazor64(d, ed, elog2[e]);    	 //if(verbose > 3 && fabs(d-dd) > DBL_EPSILON) { printf("[%s,%d:%f ", p, e, d); printf("%f] ", d-dd);}
			  }
		    }
            uint64_t u;
            memcpy(&u,&d,sizeof(u));
            IPUSH(in,n,-isize,nmax,u);                                          if(verbose>=5 && n < 100 || verbose>=9) { c=*q; *q=0; double d; memcpy(&d,&u,sizeof(d)); printf("\'%s\'->%f  ", sbuf, d+mdelta); *q = c; }
		  }
		}
      }
      break;
    case T_CHAR:                                                              if(verbose>1) printf("reading char file. pre=%.2f\n", pre);
      for(;;) {
        char *p = s,*q;
        int c;
        if(isize > 0) {
          int64_t u;
          while((c = getc(fi)) >= '0' && c <= '9' || c == '-' || c == '+')
            if(p - s < LSIZE) *p++ = c;
          if(c == '.')  {
            *p++ = c;
            while((c = getc(fi)) >= '0' && c <= '9' || c == '-' || c == '+' || c == 'e' || c == 'E')
              if(p - s < LSIZE) *p++ = c;
            *p = 0;
            u = pre>1.0?round(strtod(s, &q)*pre):strtod(s, &q) - mdelta;
          } else {
            *p = 0;
            u = strtoll(s, &p, 10) - mdelta;
          }
          IPUSH(in,n,isize,nmax,u);                                           if(verbose>=5 && n < 100 || verbose>=9) printf("%s->%lld ", s, (int64_t)u);
        } else {
          while((c = getc(fi)) >= '0' && c <= '9' || c == '-')
            if(p - s < LSIZE) *p++ = c;
          if((*p++ = c) == '.')
            while((c = getc(fi)) >= '0' && c <= '9')
              if(p - s < LSIZE) *p++ = c;
          *p = 0;
          double d = strtod(s, &p) - mdelta;
          uint64_t u;
          memcpy(&u,&d,sizeof(u));                                              if(verbose>=5 && n < 100 || verbose>=9) { double d; memcpy(&d,&u,sizeof(u)); printf("\'%s\'->%e  ", s, d); }
          IPUSH(in,n,-isize,nmax,u);
        }
        if(c == EOF) break;
      }
      break;
    default: die("unknown data format %d\n", fmt);
  }
  if(verbose>=5) printf("\n");
  *_in = in;
  if(ovf) printf("number of items truncated=%d\n", ovf);
  return n*abs(isize);
}

//-------------------------------------------- memcheck : buffer compare for equality ---------------------------------------------------------------
#define CBUF(_n_) (((size_t)(_n_))*5/3+1024*1024)
static int mcpy = 1, cmp = 2;

int memcheck(unsigned char *in, unsigned n, unsigned char *cpy) {
  int i;
  if(cmp <= 1)
	return 0;
  for(i = 0; i < n; i++)
    if(in[i] != cpy[i]) {
      if(cmp > 3) abort(); // crash (AFL) fuzzing
      printf("ERROR at %d:in=%x,%x,%x,%x dec=%x,%x,%x,%x cmp=%d\n", i, in[i], in[i+1], in[i+2], in[i+3], cpy[i],cpy[i+1],cpy[i+2],cpy[i+3], cmp);
      if(cmp > 2) exit(EXIT_FAILURE);
      return i+1;
    }
  return 0;
}

int memcheck32(unsigned *in, unsigned n, unsigned *cpy) {
  int i;
  if(cmp <= 1) return 0;
  for(i = 0; i < n; i++)
    if(in[i] != cpy[i]) {
      if(cmp > 3) abort(); // crash (AFL) fuzzing
      printf("ERROR at %d:in=%x,%x,%x,%x dec=%x,%x,%x,%x cmp=%d\n", i, in[i], in[i+1], in[i+2], in[i+3], cpy[i],cpy[i+1],cpy[i+2],cpy[i+3], cmp);
      if(cmp > 2) exit(EXIT_FAILURE);
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

//---------------------------------------- IcApp: Benchmark --------------------------------------------------------------------------
#include  "ext/gb.c"
//----------- external libraries --------------
  #ifdef _STREAMVBYTE
#include "ext/streamvbyte/include/streamvbyte.h"
#include "ext/streamvbyte/include/streamvbytedelta.h"
static size_t streamvbyte_zzag_encode(const uint32_t *in, uint32_t length, uint8_t *out, uint32_t prev, uint8_t *tmp) {
  zigzag_delta_encode(in, tmp, length, prev);
  return streamvbyte_encode(tmp, length, out);
}
static size_t streamvbyte_zzag_decode(const uint8_t *in, uint32_t *out, uint32_t length, uint32_t prev, uint8_t *tmp) {
  streamvbyte_decode(in, tmp, length);
  zigzag_delta_decode(tmp, out, length, prev);
  return length;
}
  #endif

  #ifdef _MASKEDVBYTE
#undef VARINTDECODE_H_
#include "ext/fastpfor.h"
#include "ext/MaskedVByte/include/varintencode.h"
    #undef VARINTDECODE_H_
#include "ext/MaskedVByte/include/varintdecode.h"
  #endif

  #ifdef _BITSHUFFLE
#include "ext/bitshuffle/src/bitshuffle.h"
    #ifndef LZ4
#include "ext/bitshuffle/lz4/lz4.h"
    #endif
  #endif

  #ifdef _BLOSC
#include "ext/c-blosc2/include/blosc2.h"
#include "ext/c-blosc2/include/blosc2/filters-registry.h"
  #endif

  #ifdef _VBZ
#define bool int
#include "ext/vbz_compression/vbz/vbz.h"
  #endif

  #ifdef _VTENC
#include "ext/VTEnc/vtenc.h"
  #endif

  #ifdef _ZFP
#include "ext/zfp/include/zfp.h"

unsigned zfpcompress(const void *in, int nx, int ny, int nz, int nw, uint8_t *out, unsigned outsize, int dtype, double errlim) { 
  zfp_field field = {0};                                                        if(verbose>2) printf("x=%d,y=%d,z=%d", nx, ny, nz);
  field.type = dtype;
  field.nx   = nx;
  field.ny   = ny;
  field.nz   = nz;
  field.nw   = nw;
  field.data = in;
  zfp_stream *zfp = zfp_stream_open(NULL);
  errlim <= DBL_EPSILON?zfp_stream_set_reversible(zfp):zfp_stream_set_accuracy(zfp, errlim); 
  bitstream *stream = stream_open(out, outsize);
  zfp_stream_set_bit_stream(zfp, stream);  

  unsigned outlen = zfp_compress(zfp, &field);	
  zfp_stream_close(zfp);
  stream_close(stream);
  return outlen;
}

void zfpdecompress(const uint8_t *in, unsigned inlen, void *out, int nx, int ny, int nz, int nw, int dtype, double errlim) {
  zfp_field field = {0};
  field.type = dtype;
  field.nx   = nx;
  field.ny   = ny;
  field.nz   = nz;
  field.nw   = nw;
  field.data = out;
  zfp_stream *zfp = zfp_stream_open(NULL);									
  errlim <= DBL_EPSILON?zfp_stream_set_reversible(zfp):zfp_stream_set_accuracy(zfp, errlim);
  bitstream *stream = stream_open(in, inlen);
  zfp_stream_set_bit_stream(zfp, stream);

  zfp_decompress(zfp, &field);	
  zfp_stream_close(zfp);
  stream_close(stream);
  return inlen;
}
  #endif

//------------------------------------ https://github.com/zeux/meshoptimizer ------------------------------------------------------------------------------------------
  #ifdef _MESHOPT 
#include "ext/meshoptimizer/src/meshoptimizer.h"
unsigned meshenc(const float *in, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned outsize, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned vs   = nz <= 64?nz*sizeof(in[0]):sizeof(in[0]), vn = nz <= 64?nx*ny:nx*ny*nz, 
           clen = meshopt_encodeVertexBuffer(tmp, outsize, in, vn, vs);
  ctou32(out) = clen;
  return codecenc(tmp, clen, out+4, outsize-4, codid, codlev, codprm)+4;
}

void meshdec(const uint8_t *in, unsigned inlen, float *out, unsigned nx, unsigned ny, unsigned nz, unsigned char *tmp, int codid, int codlev, char *codprm) {
  unsigned vs   = nz <= 64?nz*sizeof(out[0]):sizeof(out[0]), vn = nz <= 64?nx*ny:nx*ny*nz,
           clen = ctou32(in); 
  codecdec(in+4, inlen-4, tmp, clen, codid, codlev, codprm);
  meshopt_decodeVertexBuffer(out, vn, vs, tmp, clen);
}
  #endif

  #ifdef _BLOSC
unsigned blosccomp(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned codid, int codlev, unsigned esize, int filter0, int filter1, int filter2) {
  unsigned clevel   = codid==ICC_ZSTD?((codlev+1)/2):codlev; 
  unsigned compcode = codid==ICC_LZ4?(clevel>9?BLOSC_LZ4HC:BLOSC_LZ4):BLOSC_ZSTD;
  blosc2_schunk schunk; 
  schunk.typesize   = esize;
  blosc2_cparams cp = BLOSC2_CPARAMS_DEFAULTS; 		   
		cp.typesize = esize;
	    cp.compcode = compcode;                                                        //BLOSC_LZ4HC, BLOSC_LZ4, BLOSC_ZSTD, BLOSC_LZ4, BLOSC_BLOSCLZ
		cp.clevel   = clevel<1?1:(clevel<9?clevel:9);                                  //blocksize=[1,32768[2,65536][3,131072][4,262144][5,262144][6,524288][7,524288][8,524288][9,1048576]
		cp.nthreads = 1;
		cp.schunk   = &schunk;
		cp.filters[BLOSC2_MAX_FILTERS - 1] = filter0; //BLOSC_NOFILTER, BLOSC_SHUFFLE, BLOSC_BITSHUFFLE
		cp.filters[BLOSC2_MAX_FILTERS - 2] = filter1; //BLOSC_DELTA, BLOSC_FILTER_BYTEDELTA
		cp.filters[BLOSC2_MAX_FILTERS - 3] = filter2; //BLOSC_TRUNC_PREC
        //cp.filters_meta[BLOSC2_MAX_FILTERS - 1] = 0;  // 0 means typesize when using schunks
		
  blosc2_context *ctx = blosc2_create_cctx(cp);
  int rc = blosc2_compress_ctx(ctx, in, (int)inlen, out, (int)outsize);
  blosc2_free_ctx(ctx);
  if(rc>inlen) { memcpy(out,in,inlen); rc = inlen; }
  return rc;
}

unsigned bloscdecomp(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen, unsigned esize) {
  blosc2_schunk schunk;
  schunk.typesize     = esize;
  blosc2_dparams   dp = BLOSC2_DPARAMS_DEFAULTS;
                   dp.schunk = &schunk;
  blosc2_context *ctx = blosc2_create_dctx(dp);
  int rc = blosc2_decompress_ctx(ctx, in, inlen, out, outlen);
  blosc2_free_ctx(ctx);
  return rc;
}
  #endif

  #ifdef _LIBROUNDFAST
#include "ext/libdroundfast.c"

void fround32(float *in, unsigned n, float *out, int nsd) {
  float *ip;
  for(ip = in; ip < in + n; ip++)
	*out++ = droundFast(*ip, nsd);
}	

void fround64(double *in, unsigned n, double *out, int nsd) {
  double *ip;
  for(ip = in; ip < in + n; ip++)
	*out++ = droundFast(*ip, nsd);
}	
  #endif
  
  #ifdef _BITGROOMING
#include "ext/bg/bg.c"
  #endif
  
  #ifndef _ICCODEC
char *codstr(unsigned codecid) { return ""; }  
void tpsizeset(unsigned _tpbsize) {}
void tpmodeset(unsigned _tpmode) {}
int lzidget(char *scmd) { return 0; }
  #endif
  
  #ifdef _QCOMPRESS  
#include "ext/q_compress/q_compress.h"
typedef FfiVec (*fauto_compress_i32)(int *nums, unsigned len, unsigned level);
typedef FfiVec (*fauto_compress_i64)(long long *nums, unsigned len, unsigned level);
typedef void   (*ffree_compressed)(FfiVec ffi_vec);
typedef FfiVec (*fauto_decompress_i32)( unsigned char *compressed, unsigned len );
typedef void   (*ffree_i32)(FfiVec c_vec);
typedef FfiVec (*fauto_decompress_i64)( unsigned char *compressed, unsigned len );
typedef void   (*ffree_i64)(FfiVec c_vec);
static fauto_compress_i32   auto_compress_i32_;
static fauto_compress_i64   auto_compress_i64_;
static ffree_compressed     free_compressed_;
static fauto_decompress_i32 auto_decompress_i32_;
static fauto_decompress_i64 auto_decompress_i64_;
static ffree_i32 free_i32_;
static ffree_i64 free_i64_;
static int qcomp;
void qcompini() {
  if(qcomp)	return; qcomp++;
        #if _WIN32
  { HINSTANCE hdll; int i;  
	char *qcomp = "q_compress_ffi.dll";
    if(hdll = LoadLibrary(qcomp)) {
      if(!(auto_compress_i32_   =   (fauto_compress_i32)GetProcAddress(hdll, "auto_compress_i32")))   die("auto_compress_i32 not found\n");
      if(!(auto_compress_i64_   =   (fauto_compress_i64)GetProcAddress(hdll, "auto_compress_i64")))   die("auto_compress_i64 not found\n");
	  if(!(free_compressed_     =     (ffree_compressed)GetProcAddress(hdll, "free_compressed")))     die("free_compressed not found\n");
      if(!(auto_decompress_i32_ = (fauto_decompress_i32)GetProcAddress(hdll, "auto_decompress_i32"))) die("auto_decompress_i32 not found\n");
      if(!(auto_decompress_i64_ = (fauto_decompress_i64)GetProcAddress(hdll, "auto_decompress_i64"))) die("auto_decompress_i64 not found\n");
	  if(!(free_i32_            =            (ffree_i32)GetProcAddress(hdll, "free_i32")))            die("free_i32 not found\n");
	  if(!(free_i64_            =            (ffree_i64)GetProcAddress(hdll, "free_i64")))            die("free_i64 not found\n");
    } else fprintf(stderr,"q_compress_ffi.dll not found\n");
  } 
    #elif !defined(_STATIC)
  { char *qcomp = "./libq_compress_ffi.so";
    void *hdll = dlopen(qcomp, RTLD_LAZY);
    if(hdll) { 
      if(!(auto_compress_i32_   =   (fauto_compress_i32)dlsym(hdll, "auto_compress_i32")))   die("fauto_compress_i32 not found\n");
      if(!(auto_compress_i64_   =   (fauto_compress_i64)dlsym(hdll, "auto_compress_i64")))   die("fauto_compress_i64 not found\n");
      if(!(free_compressed_     =     (ffree_compressed)dlsym(hdll, "free_compressed")))     die("ffree_compressed not found\n");
      if(!(auto_decompress_i32_ = (fauto_decompress_i32)dlsym(hdll, "auto_decompress_i32"))) die("auto_decompress_i32 not found\n");
      if(!(auto_decompress_i64_ = (fauto_decompress_i64)dlsym(hdll, "auto_decompress_i64"))) die("auto_decompress_i64 not found\n");
      if(!(free_i32_            =            (ffree_i32)dlsym(hdll, "free_i32")))            die("free_i32 not found\n");
      if(!(free_i64_            =            (ffree_i64)dlsym(hdll, "free_i64")))            die("free_i64 not found\n");
    } else fprintf(stderr,"qcompress shared library '%s' not found.'%s'\n", qcomp, dlerror());   
  }
    #endif 
}

unsigned qcomp32(unsigned char *in, unsigned inlen, unsigned char *out, int lev) { 
  qcompini(); FfiVec v = auto_compress_i32_((int *)in, inlen/4, lev); memcpy(out, v.ptr, v.len); inlen = v.len; free_compressed_(v); 
  return inlen;
}	

unsigned qdecomp32(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen) { 
  qcompini(); FfiVec v = auto_decompress_i32_(in, inlen); memcpy(out, v.ptr, outlen); free_i32_(v);   
  return outlen;
}	

unsigned qcomp64(unsigned char *in, unsigned inlen, unsigned char *out, int lev) { 
  qcompini(); FfiVec v = auto_compress_i64_((int *)in, inlen/8, lev); memcpy(out, v.ptr, v.len); inlen = v.len; free_compressed_(v); 
  return inlen;
}	

unsigned qdecomp64(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen) { 
  qcompini(); FfiVec v = auto_decompress_i64_(in, inlen); memcpy(out, v.ptr, outlen); free_i64_(v);   
  return outlen;
}	

unsigned qzcomp32(unsigned char *in, unsigned inlen, unsigned char *out, int lev, unsigned char *tmp) { 
  bitzenc(in, inlen, tmp, 4);                
  qcompini(); FfiVec v = auto_compress_i32_((int *)tmp, inlen/4, lev); memcpy(out, v.ptr, v.len); inlen = v.len; free_compressed_(v); 
  return inlen;
}	

unsigned qzdecomp32(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen) { 
  qcompini(); FfiVec v = auto_decompress_i32_(in, inlen); memcpy(out, v.ptr, outlen); free_i32_(v);
  bitzdec(out, outlen, 4);  
  return outlen;
}	

unsigned qzcomp64(unsigned char *in, unsigned inlen, unsigned char *out, int lev, unsigned char *tmp) { 
  bitzenc(in, inlen, tmp, 8);                
  qcompini(); FfiVec v = auto_compress_i64_((int *)tmp, inlen/8, lev); memcpy(out, v.ptr, v.len); inlen = v.len; free_compressed_(v); 
  return inlen;
}	

unsigned qzdecomp64(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen) { 
  qcompini(); FfiVec v = auto_decompress_i64_(in, inlen); memcpy(out, v.ptr, outlen); free_i64_(v);   
  bitzdec(out, outlen, 8);  
  return outlen;
}	

  #endif
//----------------------------------------------------------------------------------
#define CPYR(in,n,esize,out) memcpy(out+((n)&(~(esize-1))),in+((n)&(~(esize-1))),(n)&(esize-1))  //, out+((n)&(8*esize-1))

//------------------ TurboVSsimple zigzag------------------------------------------
uint16_t bitzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta);

unsigned char *vszenc8( uint8_t  *in, unsigned n, unsigned char *out, uint8_t  *tmp) { bitzenc8( in, n, tmp, 0, 0); return vsenc8( tmp, n, out); }
unsigned char *vszenc16(uint16_t *in, unsigned n, unsigned char *out, uint16_t *tmp) { bitzenc16(in, n, tmp, 0, 0); return vsenc16(tmp, n, out); }
unsigned char *vszenc32(uint32_t *in, unsigned n, unsigned char *out, uint32_t *tmp) { bitzenc32(in, n, tmp, 0, 0); return vsenc32(tmp, n, out); }
unsigned char *vszenc64(uint64_t *in, unsigned n, unsigned char *out, uint64_t *tmp) { bitzenc64(in, n, tmp, 0, 0); return vsenc64(tmp, n, out); }

unsigned char *vszdec8( unsigned char *in, unsigned n, uint8_t  *out) { unsigned char *p = vsdec8( in,n,out); bitzdec8( out, n, 0); return p; }
unsigned char *vszdec16(unsigned char *in, unsigned n, uint16_t *out) { unsigned char *p = vsdec16(in,n,out); bitzdec16(out, n, 0); return p; }
unsigned char *vszdec32(unsigned char *in, unsigned n, uint32_t *out) { unsigned char *p = vsdec32(in,n,out); bitzdec32(out, n, 0); return p; }
unsigned char *vszdec64(unsigned char *in, unsigned n, uint64_t *out) { unsigned char *p = vsdec64(in,n,out); bitzdec64(out, n, 0); return p; }

// Only for integrated speed test
void tpz0enc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp) { bitzenc(in, n, tmp, esize); tpenc( tmp, n, out, esize); }
void tpz0dec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize                    ) { tpdec(  in, n, out, esize); bitzdec(out, n, esize); }
void tpx0enc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp) { bitxenc(in, n, tmp, esize); tpenc( tmp, n, out, esize); }
void tpx0dec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize                    ) { tpdec(  in, n, out, esize); bitxdec(out, n, esize); }
void tp4z0enc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp) { bitzenc(in, n, tmp, esize); tp4enc( tmp, n, out, esize); }
void tp4z0dec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize                    ) { tp4dec(  in, n, out, esize); bitzdec(out, n, esize); }
void tp4x0enc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize, unsigned char *tmp) { bitxenc(in, n, tmp, esize); tp4enc( tmp, n, out, esize); }
void tp4x0dec(unsigned char *in, unsigned n, unsigned char *out, unsigned esize                    ) { tp4dec(  in, n, out, esize); bitxdec(out, n, esize); }

//------------------ TurboRLE (Run Length Encoding) + zigzag/xor -------------------------
#define RLE8  0xdau
#define RLE16 0xdadau
#define RLE32 0xdadadadau
#define RLE64 0xdadadadadadadadaull
unsigned trlezc( uint8_t      *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc8(in, n, tmp, 0, 0); unsigned rc = trlec(tmp, n, out); if(rc >=n) { rc = n; memcpy(out,in,n); } return rc; }
unsigned trlezd(unsigned char *in, unsigned inlen, uint8_t *out, unsigned n) { if(inlen >= n) { memcpy(out,in,n); return inlen; } trled(in, inlen, out, n); bitzdec8(out, n, 0); return n; }

unsigned trlexc( uint8_t      *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitxenc8(in, n, tmp, 0); unsigned rc = trlec(tmp, n, out); if(rc >=n) { rc = n; memcpy(out,in,n); } return rc; }
unsigned trlexd(unsigned char *in, unsigned inlen, uint8_t *out, unsigned n) { if(inlen >= n) { memcpy(out,in,n); return inlen; } trled(in, inlen, out, n); bitxdec8(out, n, 0); return n; }

unsigned srlezc8( uint8_t  *in, unsigned n, unsigned char *out, uint8_t *tmp, uint8_t  e) { bitzenc8( in, n/( 8/8), tmp, 0, 0); return srlec8( tmp, n, out, e); }
unsigned srlezc16(uint16_t *in, unsigned n, unsigned char *out, uint16_t *tmp, uint16_t e) { bitzenc16(in, n/(16/8), tmp, 0, 0); return srlec16(tmp, n, out, e); }
unsigned srlezc32(uint32_t *in, unsigned n, unsigned char *out, uint32_t *tmp, uint32_t e) { bitzenc32(in, n/(32/8), tmp, 0, 0); return srlec32(tmp, n, out, e); }
unsigned srlezc64(uint64_t *in, unsigned n, unsigned char *out, uint64_t *tmp, uint64_t e) { bitzenc64(in, n/(64/8), tmp, 0, 0); return srlec64(tmp, n, out, e); }

unsigned srlezd8( unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint8_t  e) { srled8(in,inlen,out,  n, e); bitzdec8( out, n/(8/8),  0); return n; }
unsigned srlezd16(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint16_t e) { srled16(in,inlen,out, n, e); bitzdec16(out, n/(16/8), 0); return n; }
unsigned srlezd32(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint32_t e) { srled32(in,inlen,out, n, e); bitzdec32(out, n/(32/8), 0); return n; }
unsigned srlezd64(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint64_t e) { srled64(in,inlen,out, n, e); bitzdec64(out, n/(64/8), 0); return n; }

unsigned srlexc8( uint8_t  *in, unsigned n, unsigned char *out, unsigned char *tmp, uint8_t  e) { bitxenc8( in, n/( 8/8), tmp, 0); return srlec8( tmp, n, out, e); }
unsigned srlexc16(uint16_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint16_t e) { bitxenc16(in, n/(16/8), tmp, 0); return srlec16(tmp, n, out, e); }
unsigned srlexc32(uint32_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint32_t e) { bitxenc32(in, n/(32/8), tmp, 0); return srlec32(tmp, n, out, e); }
unsigned srlexc64(uint64_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint64_t e) { bitxenc64(in, n/(64/8), tmp, 0); return srlec64(tmp, n, out, e); }

unsigned srlexd8( unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint8_t  e) { srled8(in,inlen,out,  n, e); bitxdec8( out, n/(8/8),  0); return n; }
unsigned srlexd16(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint16_t e) { srled16(in,inlen,out, n, e); bitxdec16(out, n/(16/8), 0); return n; }
unsigned srlexd32(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint32_t e) { srled32(in,inlen,out, n, e); bitxdec32(out, n/(32/8), 0); return n; }
unsigned srlexd64(unsigned char *in, unsigned inlen, unsigned char *out, unsigned n, uint64_t e) { srled64(in,inlen,out, n, e); bitxdec64(out, n/(64/8), 0); return n; }

  #ifdef _BITSHUFFLE //--------------------------bit transpose ----------------------------------------------------------------------
static void bitshuffle(uint8_t *in, unsigned n, uint8_t *out, unsigned esize) {
  bshuf_bitshuffle(  in, out, n/esize, esize, 0); memcpy((char *)out+(n&(~(8*esize-1))), (char *)in+(n&(~(8*esize-1))), n&(8*esize-1));
}
static void bitunshuffle(uint8_t *in, unsigned n, uint8_t *out, unsigned esize) {
  bshuf_bitunshuffle(in, out, n/esize, esize, 0); memcpy((char *)out+(n&(~(8*esize-1))), (char *)in+(n&(~(8*esize-1))), n&(8*esize-1));
}
  #endif

//-- lz --------------------------------------------------------
unsigned codid = 0, codlev = 1;
unsigned char codprm[256];

  #ifdef _SPDP
//-- SPDP (https://userweb.cs.txstate.edu/~burtscher/research/SPDPcompressor/) -------------
#define NMAIN
#include "ext/SPDP_10.c"
#define SPDPSIZE (1<<23)
char ibuf[SPDPSIZE*2];

unsigned spdpenc(unsigned char *in, size_t n, unsigned char *out, unsigned bsize, int codlev) {
  unsigned char *op = out,*ip;

  for(ip = in, in += n; ip < in;) {
    unsigned iplen = in - ip,l;
    if(iplen > bsize) iplen = bsize;
    memcpy(ibuf,ip,iplen); // SPDP is overwriting the input, copy to a tmp buffer
    l = spdp_compress(codlev, iplen, ibuf, op+4);
	ctou32(op) = l; op+=4+l; //AC(l <= bsize,"Compress Fatal=%d>%d\n", l, bsize);
    ip += iplen;
  }
  return op - out;
}

size_t spdpdec(unsigned char *in, size_t n, unsigned char *out, unsigned bsize, int codlev) {\
  unsigned char *ip = in,*op;

  for(op = out,out+=n; op < out;) { unsigned oplen = out - op,l;
    if(oplen > bsize) oplen = bsize;
    l = ctou32(ip);
    ip += 4;
    memcpy(ibuf,ip,l);
    spdp_decompress(codlev, l, ibuf, op); ip += l;
    op += oplen;
  }
  return ip - in;
}
  #endif

//------------------------------------- Benchmark -------------------------------------------------------------------

#define ID_MEMCPY 120
unsigned char *bestr(unsigned id, unsigned b, unsigned char *s, char *prms, int prmi) {
  static char *fmt[] = {
    "%3d:000              ",
    "%3d:p4nenc%d         TurboPFor           ",        //1
    "%3d:p4nenc128v%d     TurboPFor128        ",
    "%3d:p4nenc256v%d     TurboPFor256        ",
    "%3d:p4ndenc%d        TurboPFor    delta  ",
    "%3d:p4ndenc128v%d    TurboPFor128 delta  ",
    "%3d:p4ndenc256v%d    TurboPFor256 delta  ",
    "%3d:p4nd1enc%d       TurboPFor    delta1 ",
    "%3d:p4nd1enc128v%d   TurboPFor128 delta1 ",
    "%3d:p4nd1enc256v%d   TurboPFor256 delta1 ",

    "%3d:p4nzenc%d        TurboPFor    zigzag ",        //10
    "%3d:p4nzenc128v%d    TurboPFor128 zigzag ",
    "%3d:p4nzenc256v%d    TurboPFor256 zigzag ",
    "%3d:p4nzzenc128v%d   TurboPFor zzag/delta",
    "%3d:bics32           Interpolative Cod. S",
    "%3d:bic32            Interpolative Cod. L",
    "%3d:bicm32           Interpolative Cod. M",
    "%3d:vnenc16          IC16                ",
    "%3d:efano32          Elias Fano          ",
    "%3d:efano132         Elias Fano          ",

    "%3d:bitnpack%d       TurboPack           ",        //20
    "%3d:bitnpack128v%d   TurboPack128        ",
    "%3d:bitnpack256v%d   TurboPack256        ",
    "%3d:bitndpack%d      TurboPack    delta  ",
    "%3d:bitndpack128v%d  TurboPack128 delta  ",
    "%3d:bitndpack256v%d  TurboPack256 delta  ",
    "%3d:bitnd1pack%d     TurboPack    delta1 ",
    "%3d:bitnd1pack128v%d TurboPack128 delta1 ",
    "%3d:bitnd1pack256v%d TurboPack256 delta1 ",
    "%3d:bitnzpack%d      TurboPack    zigzag ",

    "%3d:bitnzpack128v%d  TurboPack128 zigzag ",        //30
    "%3d:bitnzpack256v%d  TurboPack256 zigzag ",
    "%3d:bitnfpack%d      TurboPack    FOR    ",
    "%3d:bitnfpack128v%d  TurboPack128 FOR    ",
    "%3d:bitnfpack256v%d  TurboPack256 FOR    ",
    "%3d:bitns1pack128v32 TurboPack256 delt4_1",
    "%3d:bitnxpack128v%d  TurboPack128 xor    ",
    "%3d:bitnxpack256v%d  TurboPack256 xor    ",
    "%3d:vsenc%d          TurboVSimple        ",
    "%3d:vszenc%d         TurboVSimple zigzag ",

    "%3d:vbenc%d          TurboVByte scalar   ",         //40
    "%3d:vbzenc%d         TurboVByte zigzag   ",
    "%3d:vbdenc%d         TurboVByte delta    ",
    "%3d:vbd1enc%d        TurboVByte delta1   ",
    "%3d:vbddenc%d        TurboVByte zzag delt",
    "%3d:v8enc%d          TurboByte SIMD      ",
    "%3d:v8denc%d         TurboByte delta     ",
    "%3d:v8d1enc%d        TurboByte delta1    ",
    "%3d:v8xenc%d         TurboByte xor       ",
    "%3d:v8zenc%d         TurboByte zigzag    ",

    "%3d:v8nenc128v%d     TurboBitByte        ",        //50    //TurboByte Hybrid
    "%3d:v8nzenc128v%d    TurboBitByte zigzag ",
    "%3d:v8ndenc128v%d    TurboBitByte delta  ",
    "%3d:v8nd1enc128v%d   TurboBitByte delta1 ",
    "%3d:v8nxenc128v%d    TurboBitByte xor    ",
    "%3d:v8nenc256v%d     TurboBitByte        ",
    "%3d:v8nzenc256v%d    TurboBitByte zigzag ",
    "%3d:v8ndenc256v%d    TurboBitByte delta  ",
    "%3d:v8nd1enc256v%d   TurboBitByte delta1 ",
    "%3d:v8nxenc256v%d    TurboBitByte xor    ",

    "%3d:bvzzenc%d        Zigzag/delta   bitio",       //60
    "%3d:bvzenc%d         Zigzag         bitio",
    "%3d:fpgenc%d         TurboGorilla   bitio",
    "%3d:fphenc%d         Chimp  algo    bitio",
    "%3d:fpcenc%d         TurboFloat LzX bitio",
    "%3d:fpxenc%d         TurboFloat XOR      ",        //bvzaenc%d        moving average pred.",
    "%3d:fpfcmenc%d       TurboFloat FCM      ",
    "%3d:fpdfcmenc%d      TurboFloat DFCM     ",
    "%3d:fp2dfcmenc%d     TurboFloat DFCM 2D  ",
    "%3d:69                                   ",

    "%3d:trle             TurboRLE            ",        //70
    "%3d:trlex            TurboRLE xor        ",
    "%3d:trlez            TurboRLE zigzag     ",
    "%3d:srle%d           TurboRLE ESC        ",
    "%3d:srlex%d          TurboRLE ESC xor    ",
    "%3d:srlez%d          TurboRLE ESC zigzag ",
    "%3d:TpRle            Transpose    +trle  ",
    "%3d:TpRlex           TransposeXor +trle  ",
    "%3d:TpRlez           TransposeZzag+trle  ",
    "%3d:79                                   ",

    "%3d:Lz               %s,%d               ",     //80
    "%3d:Lztp   Byte      Transpose  +%s,%d",
    "%3d:Lztpx  Byte      TransposeX +%s,%d",
    "%3d:Lztpz  Byte      TransposeZ +%s,%d",
    "%3d:Lztp4  Nibble    Transpose  +%s,%d",
    "%3d:Lztp4x Nibble    TransposeX +%s,%d",
    "%3d:Lztp4z Nibble    TransposeZ +%s,%d",
    "%3d:Lztp1  Bit       Bitshuffle +%s,%d",
    "%3d:Lztp1x Bit       BitshuffleX+%s,%d",
    "%3d:Lztp1z Bit       BitshuffleZ+%s,%d",

    "%3d:lztprle          Transpose +rle+%s,%d",     //90
    "%3d:lztprlex         TransposeX+rle+%s,%d",
    "%3d:lztprlez         TransposeZ+rle+%s,%d",
    "%3d:lzv8enc          TurboByte +%s,%d",
    "%3d:lzv8xenc         TurboByteX+%s,%d",
    "%3d:lzv8zenc         TurboByteZ+%s,%d",
    "%3d:vlccomp          TurboVLC  +%s,%d",
    "%3d:vhicomp          Hybrid Integer+%s,%d",
    "%3d:98                                   ",
    "%3d:99                                   ",

    "%3d:LztpD2byte       2D Transpose +%s,%d",    //100
    "%3d:LztpxD2byte      2D TransposeX+%s,%d",
    "%3d:LztpzD2byte      2D TransposeZ+%s,%d",
    "%3d:LztpD3byte       3D Transpose +%s,%d",
    "%3d:LztpxD3byte      3D TransposeX+%s,%d",
    "%3d:LztpzD3byte      3D TransposeZ+%s,%d",
    "%3d:LztpD4byte       4D Transpose +%s,%d",
    "%3d:LztpxD4byte      4D TransposeX+%s,%d",
    "%3d:LztpzD4byte      4D TransposeZ+%s,%d",
    "                                         ",

    "%3d:vlc              TurboVLC       bitio",    //110
    "%3d:vlcz             TurboVLC zzag  bitio",
    "%3d:112                                  ",
    "%3d:bitgenc          Gamma coding   bitio",
    "%3d:bitrenc          Rice  coding   bitio",
    "%3d:115                                  ",
    "%3d:116                                  ",
    "%3d:tpenc            Byte transpose      ",
    "%3d:tp4enc           Nibble transpose    ",
    "%3d:bitshuffle       Bit transpose       ",

    "%3d:memcpy           memcpy              ",    //120
    "%3d:vtenc            VTEnc lib           ",
    "%3d:vbz              vbz (nanopore)      ",
    "%3f:123                                  ",
    "%3d:124                                  ",
    "%3d:125                                  ",
    "%3d:126                                  ",
    "%3d:127                                  ",
    "%3d:128                                  ",
    "%3d:129                                  ",
	
    "%3d:streamvbyte      StreamVByte SIMD    ",  //130
    "%3d:streamvbyte delt StreamVByte delta   ",
    "%3d:streamvbyte zzag StreamVByte zigzag  ",
    "%3d:maskeydvbyte     MasedVByte SIMD     ",
    "%3d:FastPFor         FastPFor            ",
    "%3d:SimdFastPFor     FastPFor SIMD       ",
    "%3d:SimdOptPFor      FastPFor SIMD       ",
    "%3d:SPDP             SPDP Floating Point ",
    "%3d:138                                  ",
    "%3d:139                                  ",
	
    "%3d:zfp              zfp                 ",  //140
    "%3d:zfp              zfp 2D              ",
    "%3d:zfp              zfp 3D              ",
    "%3d:blosc  bitshuffle+%s,%d              ",   	
    "%3d:blosc  bitshuffle+delta+%s,%d        ",   	
    "%3d:blosc  bitshuffle+bytedelta+%s,%d    ",   	
    "%3d:blosc     shuffle+%s,%d              ",   	
    "%3d:blosc     shuffle+delta+%s,%d        ",   	
    "%3d:blosc     shuffle+bytedelta+%s,%d    ",   	
    "%3d:fprazor          Turbo Razor         ", 
	
    "%3d:gb               granular bitgroom   ",  //150
    "%3d:bitgrooming      bit grooming        ",
    "%3d:libdroundfast    float fast round    ",
    "%3d:tpzenc           transp+zzag integrat",   	
    "%3d:tpz0enc          transpose+zigzag    ",   	
    "%3d:tpxenc           tranp+xor integrated",   	
    "%3d:tpx0enc          tranp+xor           ",   	
    "%3d:tp4zenc  Nibble  transp+zzag integrat",   	
    "%3d:tp4z0enc Nibble  transpose+zigzag    ",  	
    "%3d:tp4xenc  Nibble  tranp+xor integrated", 
  	
    "%3d:tp4x0enc Nibble  transpose+xor       ",  //160
    "%3d:161              speed test          ",   	
    "%3d:162              speed test          ",   	
    "%3d:163              speed test          ",   	
    "%3d:164              speed test          ",   	
    "%3d:165              speed test          ",   	
    "%3d:166              speed test          ",   	
    "%3d:167              speed test          ",   	
    "%3d:168              speed test          ",   	
    "%3d:169              speed test          ",   	
	
    "%3d:meshoptimizer    3D lz%s,%d          ", //170
    "%3d:meshoptimizer    3D lz%s,%d          ",	
    "%3d:meshoptimizer    3D lz%s,%d          ",	
    "%3d:qcomp            quantile compress   ",   	
    "%3d:qcomp zigzag     quantile compress   ",   	
    "%3d:175              speed test          ",   	
    "%3d:176              speed test          ",   	
    "%3d:177              speed test          ",   	
    "%3d:178              speed test          ",   	
    "%3d:179              speed test          ",   	
	
};
  if(id < 80)
    sprintf((char *)s,fmt[id], id, b, prms, prmi);  // print bitsize
  else
    sprintf((char *)s,fmt[id], id, prms, prmi);     // no bitsize
  return s;
}


//------------------------ Floating point statistics ------------------------------------------------------------------
#define BR(b) ((b/8)*100.0)/(double)(n*esize)

#define CMPA(_a_,_b_,_t_)         ((*(_t_ *)(_a_) > *(_t_ *)(_b_)) - (*(_t_ *)(_a_) < *(_t_ *)(_b_)))
static int cmpua16(const void *a, const void *b) { return CMPA(a,b,uint16_t); }
static int cmpua32(const void *a, const void *b) { return CMPA(a,b,uint32_t); }
static int cmpua64(const void *a, const void *b) { return CMPA(a,b,uint64_t); }

void fpstat(unsigned char *in, size_t n, unsigned char *out, int s, unsigned char *_tmp) {												if(verbose>0) printf("Floating point statistics\n");
  double        imin  = DBL_MAX, imax  = DBL_MIN, isum  = 0,               //original data (input)             : minimum,maximum,sum
                eamin = DBL_MAX, eamax = DBL_MIN, easum = 0, easumsqr = 0, //absolute error                    : abs(input-output)
                ermin = DBL_MAX, ermax = DBL_MIN, ersum = 0, ersumsqr = 0, //relative error                    : abs(input-output)/abs(input)
                osum  = 0;                                                 //transformed lossy data (output)   : sum
  long long     xtb = 0, xlb = 0, zlb = 0, tb = 0, lb = 0, elb = 0, mtb = 0, itb = 0;
  size_t        idn = 0;
  unsigned char *ip, *op;
  unsigned      esize = s<0?-s:s, t = 0, dup=0,zero=0;
  long long     mant = 0;
  int           expo = 0,e;
  if(_tmp || verbose > 4) {
    unsigned char *tmp = _tmp;
    if(!tmp) { tmp = malloc(n*esize);  if(!tmp) die("malloc failed\n"); }  memcpy(tmp, out, n*esize);
    switch(esize) {
      case 2: { uint16_t *p,*t = tmp; qsort(tmp, n, 2, cmpua16); for(dup=0,p = t; p < t+n-1; p++) { if(p[0] != p[1]) dup++; if(!p[0]) zero++; } } break;
	  case 4: { uint32_t *p,*t = tmp; qsort(tmp, n, 4, cmpua32); for(dup=0,p = t; p < t+n-1; p++) { if(p[0] != p[1]) dup++; if(!p[0]) zero++; } } break;
	  case 8: { uint64_t *p,*t = tmp; qsort(tmp, n, 8, cmpua64); for(dup=0,p = t; p < t+n-1; p++) { if(p[0] != p[1]) dup++; if(!p[0]) zero++; } } break;
	  default: die("#fpstat");
    }
    if(!_tmp) free(tmp);
  }
  for(ip = in, op = out; ip < in+n*esize; ip += esize, op += esize)
    switch(s) {
        #if defined(FLT16_BUILTIN)
      case -2: isum += ctof16(ip); osum += ctof16(op); break;
        #endif
      case -4: isum += ctof32(ip); osum += ctof32(op); break;
      case -8: isum += ctof64(ip); osum += ctof64(op); break;
      case  1: isum += ctou8( ip); osum += ctou8( op); break;
      case  2: isum += ctou16(ip); osum += ctou16(op); break;
      case  4: isum += ctou32(ip); osum += ctou32(op); break;
      case  8: isum += ctou64(ip); osum += ctou64(op); break;
    }
  double iavg = isum/n, oavg = osum/n, isumpavg = 0, osumpavg = 0, iosumpavg = 0; uint64_t xstart = 0, zstart = 0;
  #define EXPO16(u) ((u>>10 &  0x1f) - 15 )
  #define EXPO32(u) ((u>>23 &  0xff) - 0x7e )
  #define EXPO64(u) ((u>>52 & 0x7ff) - 0x3fe)
  #define MANT16(u) (u & 0x83ffu)                //SeeeeeMMMMMMMMMM
  #define MANT32(u) (u & 0x807fffffu)
  #define MANT64(u) (u & 0x800fffffffffffffull)
  #define U(s) T3(uint, s, _t) u = T2(ctou,s)(op), v = T2(ctou,s)(ip);\
    itb +=  v?T2(ctz,s)(v):s;           tb +=      u?T2(ctz,s)(u):s;       lb += u?T2(clz,s)(u):s;          AC(t<=s,"Fatal t=%d ", t); \
    xstart ^= u;                       xtb += xstart?T2(ctz,s)(xstart):s; xlb += xstart?T2(clz,s)(xstart):0; xstart = u;\
    zstart  = T2(zigzagenc,s)(u - zstart);                       zlb += zstart?T2(clz,s)(zstart):s; zstart = u

  for(ip = in, op = out; ip < in+n*esize; ip += esize, op += esize) { 
    double id, od;
	unsigned e;	uint64_t m;
    switch(s) {
        #if defined(FLT16_BUILTIN)
      case -2: { unsigned e; uint16_t m;id = ctof16(ip); od = ctof16(op); U(16); e = EXPO16(u); expo = clz16(zigzagenc16(e-expo))/*-(16-(16-MANTF16-1))*/; elb+=expo; expo = e;
                                                          m = MANT16(u); mant = ctz16(            m^mant)                     ;     mtb+=mant; mant = m;//ctz16(zigzagenc16(m-mant))
                                                         } break;
        #endif                                                          
      case -4: { unsigned e; uint32_t m;id = ctof32(ip); od = ctof32(op); U(32); e = EXPO32(u); expo = clz32(zigzagenc32(e-expo))/*-(32-(32-MANTF32-1))*/; elb+=expo; expo = e;
                                                          m = MANT32(u); mant = ctz32(            m^mant)                     ;     mtb+=mant; mant = m;//ctz32(zigzagenc32(m-mant))
                                                         } break;
      case -8: { unsigned e; uint64_t m;id = ctof64(ip); od = ctof64(op); U(64); e = EXPO64(u); expo = clz32(zigzagenc32(e-expo))/*-(32-(64-MANTF64-1))*/; elb+=expo; expo = e;
                                                          m = MANT64(u); mant = ctz64(            m^mant)                     ; mtb+=mant; mant = m;//ctz64(zigzagenc64(m-mant))
                                                         } break;
      case  1: { id = ctou8( ip); od = ctou8( op); U( 8);} break;
      case  2: { id = ctou16(ip); od = ctou16(op); U(16);} break;
      case  4: { id = ctou32(ip); od = ctou32(op); U(32);} break;
      case  8: { id = ctou64(ip); od = ctou64(op); U(64);} break;
    }

    imax = max(id, imax);
    imin = min(id, imin);

      double ea = fabs(id - od); eamax = max(eamax,ea);  eamin = min(eamin,ea);  easum += ea;  easumsqr += ea*ea;  // absolute error
    if(id) { idn++;
      double er = ea/fabs(id);   ermax = max(ermax,er);  ermin = min(ermin,er);  ersum += er;  ersumsqr += er*er;   // relative error
    }
    isumpavg  += (id - iavg)*(id - iavg);
    osumpavg  += (od - oavg)*(od - oavg);
    iosumpavg += (id - iavg)*(od - oavg);   //bits      += ctz64(ctou64(&od)) - ctz64(ctou64(&id));
  } 
  double fb = 0;
       if(s == -2) fb = (double)elb*100/((double)n*5);
  else if(s == -4) fb = (double)elb*100/((double)n*8);
  else if(s == -8) fb = (double)elb*100/((double)n*11);

  double mse = easumsqr/n, irange = imax - imin;
  if(verbose >= 2) printf("\n");
  //printf("Leading/Trailing bits [%.2f%%,%.2f%%=%.2f%%]. XOR[%.2f%%,%.2f%%=%.2f%%] Zigzag[%.2f%%]\n", BR(lb), BR(tb), BR(lb+tb), BR(xlb), BR(xtb), BR(xlb+xtb), BR(zlb)/*BR(elb), BR(mtb), BR(elb+mtb)*/ );
  if(verbose >= 2)         printf("Range: [min=%g / max=%g] = %g. zeros=[%u,%.2f%%], Distinct=[%u=%.4f%%] ctz=%.1f%%\n", imin, imax, irange);
  if(verbose >  3 || _tmp) printf("zeros=[%u,%.2f%%], Distinct=[%u=%.4f%%] ctz=%.1f%%\n", zero,(double)zero*100.0/(double)n, dup, (double)dup*100.0/(double)n, (double)((tb-itb)/8)*100.0/(double)(n*esize));
  //printf("Min error: Absolute = %.12f, Relative = %f, pointwise relative = %f\n", eamin, eamin/irange, eamax/irange, ermax);
  //printf("Avg error: Absolute = %.12f, Relative = %f, pointwise relative = %f\n", easum/idn, (easum/idn)/irange, ersum/idn);
  if(verbose > 2) printf("Max error: Absolute = %g, Relative = %g, pointwise relative = %g\n", eamax, eamax/irange, ermax); else if(verbose==2) printf("e=%g ", ermax);
  double psnr=20*log10(irange)-10*log10(mse); 
  if(verbose > 2) printf("Peak Signal-to-Noise Ratio: PSNR         = %.1f\n", psnr);            else if(verbose==2) printf("PSNR=%.0f ", psnr);
  if(verbose > 2) printf("Normalized Root Mean Square Error: NRMSE = %g\n", sqrt(mse)/irange);  else if(verbose==2) printf("NRMSE=%g ", sqrt(mse)/irange);
  double std1 = sqrt(isumpavg/n), std2 = sqrt(osumpavg/n), ee = iosumpavg/n, acEff = (iosumpavg/n)/sqrt(isumpavg/n)/sqrt(osumpavg/n);
  if(verbose > 2) printf("Pearson Correlation Coefficient          = %f\n",    (iosumpavg/n)/sqrt(isumpavg/n)/sqrt(osumpavg/n));
}

//---------------------------------------------------------------------------------------
#define MINDELTA(_t_, _in_, _n_, _dm_) {\
  _t_ *_p = (_t_ *)_in_;\
  unsigned _i;\
  for(_dm_ = (_t_)-1, _i = 1; _i < _n_; _i++)\
    if(_p[_i] < _p[_i-1]) { dm = (_t_)-1; break; }\
    else { _t_ _d = _p[_i] - _p[_i-1]; if(_d < _dm_) { _dm_ = _d; /*printf("[%u]", _dm_);*/} }\
}

uint8_t  mindelta8( unsigned char *in, unsigned n) { uint8_t  dm; MINDELTA(uint8_t,  in, n, dm); return dm; }
uint16_t mindelta16(unsigned char *in, unsigned n) { uint16_t dm; MINDELTA(uint16_t, in, n, dm); return dm; }
uint32_t mindelta32(unsigned char *in, unsigned n) { uint32_t dm; MINDELTA(uint32_t, in, n, dm); return dm; }
uint64_t mindelta64(unsigned char *in, unsigned n) { uint64_t dm; MINDELTA(uint64_t, in, n, dm); return dm; }
uint64_t mindelta(  unsigned char *in, unsigned n, unsigned esize) {
  switch(esize) {
    case 1: { uint8_t  d = mindelta8( in,n); return d == (uint8_t )-1?(uint64_t)-1:d; }
    case 2: { uint16_t d = mindelta16(in,n); return d == (uint16_t)-1?(uint64_t)-1:d; };
    case 4: { uint32_t d = mindelta32(in,n); return d == (uint32_t)-1?(uint64_t)-1:d; };
    case 8: { uint64_t d = mindelta64(in,n); return d == (uint64_t)-1?(uint64_t)-1:d; };
  }
  return -2;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
//#define NEEDTMP (id == 39 || id >= 71)
unsigned nx,ny,nz,nw;

#define USIZE 1
unsigned bench8(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int codlev, int bsize) {
  unsigned      l = 0, m = n/(USIZE), rc = 0, ns = CBUF(n);
  uint8_t       dm = mindelta8(in,m), *p = NULL;
  unsigned char *tmp = NULL;

  if(!(tmp = (unsigned char*)malloc(ns))) die(stderr, "malloc error\n");
  memrcpy(cpy,in,n); 

  switch(id) {
    case   1: TM("",l=p4nenc8(    in, m, out),         n,l, p4ndec8(      out, m, cpy));   break;	
    case   4: TM("",l=p4ndenc8(   in, m, out),         n,l, p4nddec8(     out, m, cpy));   break;
    case   7: TM("",l=p4nd1enc8(  in, m, out),         n,l, p4nd1dec8(    out, m, cpy));   break;
    case  10: TM("",l=p4nzenc8(   in, m, out),         n,l, p4nzdec8(     out, m, cpy));   break;
    case  20: TM("",l=bitnpack8(  in, m, out),         n,l, bitnunpack8(  out, m, cpy));   break;
    case  23: TM("",l=bitndpack8( in, m, out),         n,l, bitndunpack8( out, m, cpy));   break;
    case  26: TM("",l=bitnd1pack8(in, m, out),         n,l, bitnd1unpack8(out, m, cpy));   break;
    case  29: TM("",l=bitnzpack8( in, m, out),         n,l, bitnzunpack8( out, m, cpy));   break;
    case  32: if(dm!=(uint8_t)-1) 
		      TM("",l=bitnfpack8( in, m, out),         n,l, bitnfunpack8( out, m, cpy));   break;
    case  38: TM("",l=vsenc8(     in, m, out)-out,     n,l, vsdec8(       out, m, cpy));   break; // vsimple : variable simple
    case  39: TM("",l=vszenc8(    in, m, out,tmp)-out, n,l, vszdec8(      out, m, cpy));   break;
  //case  40: TM("",l=vbenc8(     in, m, out)-out,     n,l, vbdec8(       out, m, cpy));   break; // TurboVbyte : variable byte
    case  41: TM("",l=vbzenc8(    in, m, out,0)-out,   n,l, vbzdec8(      out, m, cpy,0)); break;
  //case  42: TM("",l=vbdenc8(    in, m, out,0)-out,   n,l, vbddec8(      out, m, cpy,0)); break;
  //case  43: TM("",l=vbd1enc8(   in, m, out,0)-out,   n,l, vbd1dec8(     out, m, cpy,0)); break;
  //case  44: TM("",l=vbddenc8(   in, m, out,0)-out,   n,l, vbdddec8(     out, m, cpy,0)); break;

    case  60: TM("",l=bvzzenc8(   in, m, out,0),       n,l, bvzzdec8(     out, m, cpy,0)); break; // bitio
    case  61: TM("",l=bvzenc8(    in, m, out,0),       n,l, bvzdec8(      out, m, cpy,0)); break;
    case  62: TM("",l=fpgenc8(    in, m, out,0),       n,l, fpgdec8(      out, m, cpy,0)); break;
    case  63: TM("",l=fphenc8(    in, m, out,0),       n,l, fphdec8(      out, m, cpy,0)); break;

    case  65: TM("",l=fpxenc8(    in, m, out,0),       n,l, fpxdec8(      out, m, cpy,0)); break; //Floating point/Integer
    case  66: TM("",l=fpfcmenc8(  in, m, out,0),       n,l, fpfcmdec8(    out, m, cpy,0)); break;
    case  67: TM("",l=fpdfcmenc8( in, m, out,0),       n,l, fpdfcmdec8(   out, m, cpy,0)); break;
    case  68: TM("",l=fp2dfcmenc8(in, m, out,0),       n,l, fp2dfcmdec8(  out, m, cpy,0)); break;

    case  70: TM("",l=trlec(      in, n,out),          n,l, trled(        out,l,cpy, n));      break;  // TurboRLE
    case  71: TM("",l=trlexc(     in, n,out,tmp),      n,l, trlexd(       out,l,cpy, n));      break;
    case  72: TM("",l=trlezc(     in, n,out,tmp),      n,l, trlezd(       out,l,cpy, n));      break;
    case  73: TM("",l=srlec8(     in, n,out,RLE8),     n,l, srled8(       out,l,cpy, n,RLE8)); break;
    case  74: TM("",l=srlexc8(    in, n,out,tmp,RLE8), n,l, srlexd8(      out,l,cpy, n,RLE8)); break;
    case  75: TM("",l=srlezc8(    in, n,out,tmp,RLE8), n,l, srlezd8(      out,l,cpy, n,RLE8)); break;
      #ifdef _ICCODEC
    case  80: TM("",l=codecenc(   in,n,out,ns,          codid,codlev,codprm),       n,l, codecdec(   out,l,cpy,n,          codid,codlev,codprm));       break; //iccodecs
    case  81: TM("",l=lztpenc(    in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpdec(    out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case  82: TM("",l=lztpxenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpxdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case  83: TM("",l=lztpzenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpzdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case  84: TM("",l=lztp4enc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpd4ec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case  85: TM("",l=lztp4xenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztp4xdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case  86: TM("",l=lztp4zenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztp4zdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
      #ifdef _BITSHUFFLE
    case  87: TM("",l=lztp1enc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1dec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case  88: TM("",l=lztp1xenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1xdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case  89: TM("",l=lztp1zenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1zdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
      #endif
    case  90: TM("",l=lztprleenc( in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprledec( out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case  91: TM("",l=lztprlexenc(in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprlexdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case  92: TM("",l=lztprlezenc(in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprlezdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case  93: TM("",l=lzv8enc(    in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8dec(    out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case  94: TM("",l=lzv8xenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8xdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case  95: TM("",l=lzv8zenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8zdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;

    case 100: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2enc( in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm),n,l, lztpd2dec( out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm)); } break; //2D
    case 101: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2xenc(in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm),n,l, lztpd2xdec(out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm)); } break;
    case 102: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2zenc(in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm),n,l, lztpd2zdec(out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm)); } break;
	
    case 103: if(nz>0) { unsigned _nz = nz*(nw?nw:1);           TM("",l=lztpd3enc( in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm),n,l, lztpd3dec( out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm)); } break; //3D
    case 104: if(nz>0) { unsigned _nz = nz*(nw?nw:1);           TM("",l=lztpd3xenc(in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm),n,l, lztpd3xdec(out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm)); } break;
    case 105: if(nz>0) { unsigned _nz = nz*(nw?nw:1);           TM("",l=lztpd3zenc(in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm),n,l, lztpd3zdec(out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm)); } break;
	
    case 106: if(nw>0) TM("",l=lztpd4enc( in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm),n,l, lztpd4dec( out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm)); break; //4D
    case 107: if(nw>0) TM("",l=lztpd4xenc(in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm),n,l, lztpd4xdec(out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm)); break;
    case 108: if(nw>0) TM("",l=lztpd4zenc(in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm),n,l, lztpd4zdec(out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm)); break;
      #endif
	  
    case 117: TM("", tpenc( in, n, out,USIZE),      n,n, tpdec( out, n,cpy, USIZE)); l=n; break;
    case 118: TM("", tp4enc(in, n, out,USIZE),      n,n, tp4dec(out, n,cpy, USIZE)); l=n; break;
      #ifdef _BITSHUFFLE
    case 119: TM("", bitshuffle(in, n, out, USIZE), n,n, bitunshuffle(out, n,cpy, USIZE)); l=n; break;
      #endif
    case ID_MEMCPY: if(mcpy) TM("", libmemcpy(out,in,n), n,n, libmemcpy(cpy,out,n)); l=n; break;

      #ifdef _SPDP
    case 137: TM("",l=spdpenc(in,m*(USIZE),out,SPDPSIZE,codlev) ,n,l, spdpdec(out, m*(USIZE), cpy,SPDPSIZE,codlev)); break;
      #endif
    default: goto end;
  }
  if(l) {
      unsigned char s[65] = { 0 }; printf("%-30s ", bestr(id, 8, s, codstr(codid), codlev));
    if(cpy) rc = memcheck(in,m*(USIZE),cpy);
    if(!rc)
      printf("\t%s\n", inname?inname:"");
  }

  end:if(tmp) free(tmp);
  return l;
}

//--------------------------------------- 16 bits ------------------------------------------------------------------------------
#undef USIZE
#define USIZE 2
unsigned bench16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int codlev, unsigned bsize) {
  unsigned      l = 0,m = n/(USIZE),rc = 0, d = 0, ns = CBUF(n);
  uint16_t      dm = mindelta16(in,m);
  uint16_t      *p = NULL;
  unsigned char *tmp = NULL;

  if(!(tmp = (unsigned char*)malloc(ns))) die(stderr, "malloc error\n");
  memrcpy(cpy,in,n);

  switch(id) {
    case  1: TM("",l=p4nenc16(        in, m, out),  n,l, p4ndec16(          out, m, cpy)); break;
      #ifndef _NSSE
    case  2: TM("",l=p4nenc128v16(    in, m, out),  n,l, p4ndec128v16(      out, m, cpy)); break;
      #endif
    case  4: TM("",l=p4ndenc16(       in, m, out),  n,l, p4nddec16(         out, m, cpy)); break;
      #ifndef _NSSE
    case  5: TM("",l=p4ndenc128v16(   in, m, out),  n,l, p4nddec128v16(     out, m, cpy)); break;
      #endif
    case  7: TM("",l=p4nd1enc16(      in, m, out),  n,l, p4nd1dec16(        out, m, cpy)); break;
      #ifndef _NSSE
    case  8: TM("",l=p4nd1enc128v16(  in, m, out),  n,l, p4nd1dec128v16(    out, m, cpy)); break;
      #endif
    case 10: TM("",l=p4nzenc16(       in, m, out),  n,l, p4nzdec16(         out, m, cpy)); break;
      #ifndef _NSSE
    case 11: TM("",l=p4nzenc128v16(   in, m, out),  n,l, p4nzdec128v16(     out, m, cpy)); break;
    case 13: TM("",l=p4nzzenc128v16(  in, m, out,0),n,l, p4nzzdec128v16(    out, m, cpy,0)); break;
      #endif

      #ifndef _NAVX2
  //case 17: if(isa>=0x60) { TM("",l=vnenc16(in, m, out),n,l" 17",vndec16( out, m, cpy)); }break;
	  #endif
    case 20: TM("",l=bitnpack16(      in, m, out), n,l, bitnunpack16(      out, m, cpy)); break;
      #ifndef _NSSE
    case 21: TM("",l=bitnpack128v16(  in, m, out), n,l, bitnunpack128v16(  out, m, cpy)); break;
      #endif
    case 23: TM("",l=bitndpack16(     in, m, out), n,l, bitndunpack16(     out, m, cpy)); break;
      #ifndef _NSSE
    case 24: TM("",l=bitndpack128v16( in, m, out) ,n,l, bitndunpack128v16( out, m, cpy)); break;
      #endif
    case 26: TM("",l=bitnd1pack16(    in, m, out) ,n,l, bitnd1unpack16(    out, m, cpy)); break;
      #ifndef _NSSE
    case 27: TM("",l=bitnd1pack128v16(in, m, out) ,n,l, bitnd1unpack128v16(out, m, cpy)); break;
      #endif
    case 29: TM("",l=bitnzpack16(     in, m, out) ,n,l, bitnzunpack16(     out, m, cpy)); break;
      #ifndef _NSSE
    case 30: TM("",l=bitnzpack128v16( in, m, out) ,n,l, bitnzunpack128v16( out, m, cpy)); break;
      #endif
    case 32: if(dm!=(uint16_t)-1) TM("",l=bitnfpack16(     in, m, out), n,l, bitnfunpack16(     out, m, cpy)); break;
      #ifndef _NSSE
    case 33: if(dm!=(uint16_t)-1) TM("",l=bitnfpack128v16( in, m, out), n,l, bitnfunpack128v16( out, m, cpy)); break;
    case 35: if(dm!=(uint16_t)-1) TM("",l=bitns1pack128v16(in, m, out), n,l, bitns1unpack128v16(out, m, cpy)); break;
      #endif
    //case 35: if(dm!=-1 /*|| !dm*/) TM("",l=efanoenc16(     in, m, out,0),  n,l"efanoenc16       ",efanodec16( out, m, cpy,0)); break;
    case 36: TM("",l=bitnxpack128v16( in, m, out),         n,l, bitnxunpack128v16( out, m, cpy)); break;
    case 37: break;
    case 38: TM("",l=vsenc16(         in, m, out)-out,     n,l, vsdec16(           out, m, cpy  )); break;   // vsimple : variable simple
    case 39: TM("",l=vszenc16(        in, m, out,tmp)-out, n,l, vszdec16(          out, m, cpy  )); break;
    case 40: TM("",l=vbenc16(         in, m, out)-out,     n,l, vbdec16(           out, m, cpy  )); break; // TurboVbyte : variable byte
    case 41: TM("",l=vbzenc16(        in, m, out,0)-out,   n,l, vbzdec16(          out, m, cpy,0)); break;
    case 42: TM("",l=vbdenc16(        in, m, out,0)-out,   n,l, vbddec16(          out, m, cpy,0)); break;
    case 43: TM("",l=vbd1enc16(       in, m, out,0)-out,   n,l, vbd1dec16(         out, m, cpy,0)); break;
    case 44: TM("",l=vbddenc16(       in, m, out,0)-out,   n,l, vbdddec16(         out, m, cpy,0)); break;
    case 45: TM("",l=v8enc16(         in, m, out)-out,     n,l, v8dec16(           out, m, cpy  )); break; // Turbobyte : Group varint
    case 46: TM("",l=v8denc16(        in, m, out,0)-out,   n,l, v8ddec16(          out, m, cpy,0)); break;
    case 47: TM("",l=v8d1enc16(       in, m, out,0)-out,   n,l, v8d1dec16(         out, m, cpy,0)); break;
    case 48: TM("",l=v8xenc16(        in, m, out,0)-out,   n,l, v8xdec16(          out, m, cpy,0)); break;
    case 49: TM("",l=v8zenc16(        in, m, out,0)-out,   n,l, v8zdec16(          out, m, cpy,0)); break;

      #ifndef _NSSE
    case 50: TM("",l=v8nenc128v16(    in, m, out),         n,l, v8ndec128v16(      out, m, cpy)); break;
    case 51: TM("",l=v8nzenc128v16(   in, m, out),         n,l, v8nzdec128v16(     out, m, cpy)); break;
    case 52: TM("",l=v8ndenc128v16(   in, m, out),         n,l, v8nddec128v16(     out, m, cpy)); break;
    case 53: TM("",l=v8nd1enc128v16(  in, m, out),         n,l, v8nd1dec128v16(    out, m, cpy)); break;
    case 54: TM("",l=v8nxenc128v16(   in, m, out),         n,l, v8nxdec128v16(     out, m, cpy)); break;
      #endif
    case 60: TM("",l=bvzzenc16(       in, m, out,0),       n,l, bvzzdec16(         out, m, cpy,0)); break; // bitio
    case 61: TM("",l=bvzenc16(        in, m, out,0),       n,l, bvzdec16(          out, m, cpy,0)); break;
    case 62: TM("",l=fpgenc16(        in, m, out,0),       n,l, fpgdec16(          out, m, cpy,0)); break;
    case 63: TM("",l=fphenc16(        in, m, out,0),       n,l, fphdec16(          out, m, cpy,0)); break;
    case 64: TM("",l=fpcenc16(        in, m, out,0),       n,l, fpcdec16(          out, m, cpy,0)); break;

    case 65: TM("",l=fpxenc16(        in, m, out,0),       n,l, fpxdec16(          out, m, cpy,0)); break; //Floating point/Integer
    case 66: TM("",l=fpfcmenc16(      in, m, out,0),       n,l, fpfcmdec16(        out, m, cpy,0)); break;
    case 67: TM("",l=fpdfcmenc16(     in, m, out,0),       n,l, fpdfcmdec16(       out, m, cpy,0)); break;
    case 68: TM("",l=fp2dfcmenc16(    in, m, out,0),       n,l, fp2dfcmdec16(      out, m, cpy,0)); break;

    case 70: TM("",l=trlec(           in, n,out),          n,l, trled(             out,l,cpy, n));      break;  // TurboRLE
    case 71: TM("",l=trlexc(          in, n,out,tmp),      n,l, trlexd(            out,l,cpy, n));      break;
    case 72: TM("",l=trlezc(          in, n,out,tmp),      n,l, trlezd(            out,l,cpy, n));      break;
    case 73: TM("",l=srlec16(         in, n,out,RLE16),    n,l, srled16(           out,l,cpy, n,RLE16));break;
    case 74: TM("",l=srlexc16(        in, n,out,tmp,RLE16),n,l, srlexd16(          out,l,cpy, n,RLE16));break;
    case 75: TM("",l=srlezc16(        in, n,out,tmp,RLE16),n,l, srlezd16(          out,l,cpy, n,RLE16));break;

      #ifdef _ICCODEC
    case 80: TM("",l=codecenc(   in,n,out,ns,codid,codlev,codprm),                 n,l, codecdec(out,l,cpy,n,codid,codlev,codprm));                    break; // iccodecs
    case 81: TM("",l=lztpenc(    in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpdec(    out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 82: TM("",l=lztpxenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpxdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 83: TM("",l=lztpzenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpzdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 84: TM("",l=lztp4enc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpd4ec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 85: TM("",l=lztp4xenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztp4xdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 86: TM("",l=lztp4zenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztp4zdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
      #ifdef _BITSHUFFLE
    case 87: TM("",l=lztp1enc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1dec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 88: TM("",l=lztp1xenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1xdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 89: TM("",l=lztp1zenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1zdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
      #endif
    case 90: TM("",l=lztprleenc( in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprledec( out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 91: TM("",l=lztprlexenc(in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprlexdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 92: TM("",l=lztprlezenc(in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprlezdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 93: TM("",l=lzv8enc(    in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8dec(    out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 94: TM("",l=lzv8xenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8xdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 95: TM("",l=lzv8zenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8zdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;

    case 100: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2enc( in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm), n,l, lztpd2dec( out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm));	} break;
    case 101: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2xenc(in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm), n,l, lztpd2xdec(out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm));	} break;
    case 102: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2zenc(in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm), n,l, lztpd2zdec(out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm));	} break;
	
    case 103: if(nz>0) { unsigned _nz = nz*(nw?nw:1); TM("",l=lztpd3enc( in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm), n,l, lztpd3dec( out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm));	} break;
    case 104: if(nz>0) { unsigned _nz = nz*(nw?nw:1); TM("",l=lztpd3xenc(in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm), n,l, lztpd3xdec(out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm));	} break;
    case 105: if(nz>0) { unsigned _nz = nz*(nw?nw:1); TM("",l=lztpd3zenc(in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm), n,l, lztpd3zdec(out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm));	} break;
    case 106: if(nw>0) {                              TM("",l=lztpd4enc( in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm), n,l, lztpd4dec( out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm)); } break;
    case 107: if(nw>0) { TM("",l=lztpd4xenc(in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm), n,l, lztpd4xdec(out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm)); } break;
    case 108: if(nw>0) { TM("",l=lztpd4zenc(in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm), n,l, lztpd4zdec(out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm)); } break;
      #endif
    case 110: TM("", l=vlcenc16(in,  n, out), n,l, l==n?memcpy(cpy,in,n):(void*)vlcdec16( out,n,cpy)); break;
    case 111: TM("", l=vlczenc16(in, n, out), n,l, l==n?memcpy(cpy,in,n):(void*)vlczdec16(out,n,cpy)); break;

    case 117: TM("", tpenc( in, l=n, out,USIZE), n,l, tpdec( out, n,cpy, USIZE)); break;
    case 118: TM("", tp4enc(in, l=n, out,USIZE), n,l, tp4dec(out, n,cpy, USIZE)); break;
      #ifdef _BITSHUFFLE
    case 119: TM("", bitshuffle(in, l=n, out,USIZE), n,l, bitunshuffle(out, n,cpy, USIZE)); break;
      #endif
    case ID_MEMCPY: if(mcpy) TM("", libmemcpy(out,in,l=n), n,l, libmemcpy( cpy,out,n)); break;
	//121: VTENC
      #ifdef _VBZ
    case 122: { CompressionOptions opt; opt.perform_delta_zig_zag = 1; opt.integer_size = 2; opt.zstd_compression_level = 22; opt.vbz_version = VBZ_DEFAULT_VERSION;
              TM("", l = vbz_compress(in, n, out, ns, &opt), n,l, vbz_decompress(out, l, cpy, n, &opt));
      } break;
      #endif
      #if defined(FLT16_BUILTIN)  
    case 149: l=n; TM0("", fprazor16(in, m, out,zerrlim), n, l);                                         memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -2, tmp); break;
      #endif
    case 153: TM("", tpzenc(  in, n, out, USIZE),     n,n, tpzdec(  out, n,cpy, USIZE)); l=n; break;
    case 154: TM("", tpz0enc( in, n, out, USIZE, tmp),n,n, tpz0dec( out, n,cpy, USIZE)); l=n; break;
    case 155: TM("", tpxenc(  in, n, out, USIZE),     n,n, tpxdec(  out, n,cpy, USIZE)); l=n; break;
    case 156: TM("", tpx0enc( in, n, out, USIZE, tmp),n,n, tpx0dec( out, n,cpy, USIZE)); l=n; break;
    case 157: TM("", tp4zenc( in, n, out, USIZE),     n,n, tp4zdec( out, n,cpy, USIZE)); l=n; break;
    case 158: TM("", tp4z0enc(in, n, out, USIZE, tmp),n,n, tp4z0dec(out, n,cpy, USIZE)); l=n; break;
    case 159: TM("", tp4xenc( in, n, out, USIZE),     n,n, tp4xdec( out, n,cpy, USIZE)); l=n; break;
    case 160: TM("", tp4x0enc(in, n, out, USIZE, tmp),n,n, tp4x0dec(out, n,cpy, USIZE)); l=n; break;

   default: goto end;
  }
  if(l) {
    unsigned char s[65] = { 0 }; 
	printf("%-30s ", bestr(id, 16, s, codstr(codid), codlev));
    if(cpy) 
	  rc = memcheck(in,m*(USIZE),cpy);
    if(!rc) 
	  printf("\t%s\n", inname?inname:"");
  }
  end:if(tmp) free(tmp);
  return l;
}

//---------------------------------------- 32 bits ----------------------------------------------------------------------------------------------------------------
#undef USIZE
#define USIZE 4
unsigned bench32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int codlev, unsigned bsize) {
  unsigned      l = 0, m = n/(USIZE), rc = 0, d = 0, ns = CBUF(n);
  uint32_t      dm = mindelta32(in,m);
  unsigned char *tmp = NULL;
  if(/*NEEDTMP &&*/ !(tmp = (unsigned char*)malloc(ns))) 
	die(stderr, "malloc error\n");
  memrcpy(cpy,in,n);

  switch(id) {
    case  1:               TM("",l=p4nenc32(        in, m, out) ,n,l, p4ndec32(       out, m, cpy)); break;
      #ifndef _NSSE
    case  2:               TM("",l=p4nenc128v32(    in, m, out) ,n,l, p4ndec128v32(   out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case  3: if(isa>=0x60) TM("",l=p4nenc256v32(    in, m, out),  n,l, p4ndec256v32(  out, m, cpy)); break;
      #endif
    case  4:               TM("",l=p4ndenc32(       in, m, out),  n,l, p4nddec32(     out, m, cpy)); break;
      #ifndef _NSSE
    case  5:               TM("",l=p4ndenc128v32(   in, m, out),  n,l, p4nddec128v32( out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case  6: if(isa>=0x60) TM("",l=p4ndenc256v32(   in, m, out),  n,l, p4nddec256v32( out, m, cpy)); break;
      #endif
    case  7:               TM("",l=p4nd1enc32(      in, m, out),  n,l, p4nd1dec32(    out, m, cpy)); break;
      #ifndef _NSSE
    case  8:               TM("",l=p4nd1enc128v32(  in, m, out),  n,l, p4nd1dec128v32(out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case  9: if(isa>=0x60) TM("",l=p4nd1enc256v32(  in, m, out),  n,l, p4nd1dec256v32(out, m, cpy)); break;
      #endif

    case 10:               TM("",l=p4nzenc32(       in, m, out),  n,l, p4nzdec32(     out, m, cpy)); break;
      #ifndef _NSSE
    case 11:               TM("",l=p4nzenc128v32(   in, m, out),  n,l, p4nzdec128v32( out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case 12: if(isa>=0x60) TM("",l=p4nzenc256v32(   in, m, out),  n,l, p4nzdec256v32( out, m, cpy)); break;
      #endif
      #ifndef _NSSE
    case 13:               TM("",l=p4nzzenc128v32(  in, m, out,0),n,l, p4nzzdec128v32(out, m, cpy,0)); break;
      #endif
    case 14: if(dm!=(uint32_t)-1 && dm > 0) TM("",l=bicbenc32(  in, m, out),       n,l, bicbdec32(  out, m, cpy)); break;
    case 15: if(dm!=(uint32_t)-1 && dm > 0) TM("",l=bicenc32(   in, m, out),       n,l, bicdec32(   out, m, cpy)); break;
    case 16: if(dm!=(uint32_t)-1 && dm > 0) TM("",l=bicmenc32(  in, m, out),       n,l, bicmdec32(  out, m, cpy)); break;
    case 18: if(dm!=(uint32_t)-1 && dm > 0) TM("",l=efanoenc32( in, m, out,0)-out, n,l, efanodec32( out, m, cpy,0)); break;
    case 19: if(dm!=(uint32_t)-1 && dm > 0) TM("",l=efano1enc32(in, m, out,0)-out, n,l, efano1dec32(out, m, cpy,0));break;

    case 20:               TM("",l=bitnpack32(    in, m, out),   n,l, bitnunpack32(      out, m, cpy)); break;
      #ifndef _NSSE
    case 21:               TM("",l=bitnpack128v32(in, m, out),   n,l, bitnunpack128v32(  out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case 22: if(isa>=0x60) TM("",l=bitnpack256v32(in, m, out),   n,l, bitnunpack256v32(  out, m, cpy)); break;
      #endif
    case 23:               TM("",l=bitndpack32(     in, m, out), n,l, bitndunpack32(     out, m, cpy)); break;
      #ifndef _NSSE
    case 24:               TM("",l=bitndpack128v32( in, m, out), n,l, bitndunpack128v32( out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case 25: if(isa>=0x60) TM("",l=bitndpack256v32( in, m, out), n,l, bitndunpack256v32( out, m, cpy)); break;
      #endif
    case 26:               TM("",l=bitnd1pack32(    in, m, out), n,l, bitnd1unpack32(    out, m, cpy)); break;
      #ifndef _NSSE
    case 27:               TM("",l=bitnd1pack128v32(in, m, out), n,l, bitnd1unpack128v32(out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case 28: if(isa>=0x60) TM("",l=bitnd1pack256v32(in, m, out), n,l, bitnd1unpack256v32(out, m, cpy)); break;
      #endif
    case 29:               TM("",l=bitnzpack32(     in, m, out), n,l, bitnzunpack32(     out, m, cpy)); break;
      #ifndef _NSSE
    case 30:               TM("",l=bitnzpack128v32( in, m, out), n,l, bitnzunpack128v32( out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case 31: if(isa>=0x60) TM("",l=bitnzpack256v32( in, m, out), n,l, bitnzunpack256v32( out, m, cpy)); break;
      #endif

    case 32: if(dm!=(uint32_t)-1) TM("",l=bitnfpack32(    in, m, out), n,l, bitnfunpack32(     out, m, cpy)); break;
      #ifndef _NSSE
    case 33: if(dm!=(uint32_t)-1) TM("",l=bitnfpack128v32(in, m, out), n,l, bitnfunpack128v32( out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case 34: if(dm!=(uint32_t)-1 && isa>=0x60) TM("",l=bitnfpack256v32( in, m, out), n,l, bitnfunpack256v32( out, m, cpy)); break;
      #endif
      #ifndef _NSSE
    case 35: if(dm!=(uint32_t)-1) TM("",l=bitns1pack128v32( in, m, out), n,l, bitns1unpack128v32( out, m, cpy)); break;
      #endif
    case 36:               TM("",l=bitnxpack128v32( in, m, out),  n,l, bitnxunpack128v32(  out, m, cpy)); break;
      #ifndef _NAVX2
    case 37: if(isa>=0x60) TM("",l=bitnxpack256v32( in, m, out),  n,l, bitnxunpack256v32(  out, m, cpy)); break;
      #endif
    case 38:               TM("",l=vsenc32(         in, m, out)    -out, n,l, vsdec32(     out, m, cpy)); break;   // vsimple : variable simple
    case 39:               TM("",l=vszenc32(        in, m, out,tmp)-out, n,l, vszdec32(    out, m, cpy)); break;

    case 40:               TM("",l=vbenc32(         in, m, out)-out,   n,l, vbdec32(       out, m, cpy));   break; // TurboVbyte : variable byte
    case 41:               TM("",l=vbzenc32(        in, m, out,0)-out, n,l, vbzdec32(      out, m, cpy,0)); break;
    case 42:               TM("",l=vbdenc32(        in, m, out,0)-out, n,l, vbddec32(      out, m, cpy,0)); break;
    case 43:               TM("",l=vbd1enc32(       in, m, out,0)-out, n,l, vbd1dec32(     out, m, cpy,0)); break;
    case 44:               TM("",l=vbddenc32(       in, m, out,0)-out, n,l, vbdddec32(     out, m, cpy,0)); break;
    case 45:               TM("",l=v8enc32(         in, m, out)-out,   n,l, v8dec32(       out, m, cpy));   break; // Turbobyte : Group varint
    case 46:               TM("",l=v8denc32(        in, m, out,0)-out, n,l, v8ddec32(      out, m, cpy,0)); break;
    case 47:               TM("",l=v8d1enc32(       in, m, out,0)-out, n,l, v8d1dec32(     out, m, cpy,0)); break;
    case 48:               TM("",l=v8xenc32(        in, m, out,0)-out, n,l, v8xdec32(      out, m, cpy,0)); break;
    case 49:               TM("",l=v8zenc32(        in, m, out,0)-out, n,l, v8zdec32(      out, m, cpy,0)); break;
      #ifndef _NSSE
    case 50:               TM("",l=v8nenc128v32(    in, m, out),       n,l, v8ndec128v32(  out, m, cpy)); break;
    case 51:               TM("",l=v8nzenc128v32(   in, m, out),       n,l, v8nzdec128v32( out, m, cpy)); break;
    case 52:               TM("",l=v8ndenc128v32(   in, m, out),       n,l, v8nddec128v32( out, m, cpy)); break;
    case 53:               TM("",l=v8nd1enc128v32(  in, m, out),       n,l, v8nd1dec128v32(out, m, cpy)); break;
    case 54:               TM("",l=v8nxenc128v32(   in, m, out),       n,l, v8nxdec128v32( out, m, cpy)); break;
      #endif
      #ifndef _NAVX2
    case 55: if(isa>=0x60) TM("",l=v8nenc256v32(    in, m, out),       n,l, v8ndec256v32(  out, m, cpy)); break;
    case 56: if(isa>=0x60) TM("",l=v8nzenc256v32(   in, m, out),       n,l, v8nzdec256v32( out, m, cpy)); break;
    case 57: if(isa>=0x60) TM("",l=v8ndenc256v32(   in, m, out),       n,l, v8nddec256v32( out, m, cpy)); break;
    case 58: if(isa>=0x60) TM("",l=v8nd1enc256v32(  in, m, out),       n,l, v8nd1dec256v32(out, m, cpy)); break;
    case 59: if(isa>=0x60) TM("",l=v8nxenc256v32(   in, m, out),       n,l, v8nxdec256v32( out, m, cpy)); break;
      #endif
    case 60:               TM("",l=bvzzenc32(       in, m, out,0),     n,l, bvzzdec32(     out, m, cpy,0)); break; // bitio
    case 61:               TM("",l=bvzenc32(        in, m, out,0),     n,l, bvzdec32(      out, m, cpy,0)); break;
    case 62:               TM("",l=fpgenc32(        in, m, out,0),     n,l, fpgdec32(      out, m, cpy,0)); break;
    case 63:               TM("",l=fphenc32(        in, m, out,0),     n,l, fphdec32(      out, m, cpy,0)); break;
    case 64:               TM("",l=fpcenc32(        in, m, out,0),     n,l, fpcdec32(      out, m, cpy,0)); break;

    case 65:               TM("",l=fpxenc32(        in, m, out,0),     n,l, fpxdec32(      out, m, cpy,0)); break; //Floating point/Integer
    case 66:               TM("",l=fpfcmenc32(      in, m, out,0),     n,l, fpfcmdec32(    out, m, cpy,0)); break;
    case 67:               TM("",l=fpdfcmenc32(     in, m, out,0),     n,l, fpdfcmdec32(   out, m, cpy,0)); break;
    case 68:               TM("",l=fp2dfcmenc32(    in, m, out,0),     n,l, fp2dfcmdec32(  out, m, cpy,0)); break;
 
    case 70:               TM("",l=trlec(           in, n,out),        n,l, trled(         out,l,cpy, n)); break;  // TurboRLE
    case 71:               TM("",l=trlexc(          in, n,out,tmp),    n,l, trlexd(        out,l,cpy, n)); break;
    case 72:               TM("",l=trlezc(          in, n,out,tmp),    n,l, trlezd(        out,l,cpy, n));      break;
    case 73:               TM("",l=srlec32(         in, n,out,RLE32),  n,l, srled32(       out,l,cpy, n,RLE32));break;
    case 74:               TM("",l=srlexc32(        in, n,out,tmp,RLE32),n,l,srlexd32(     out,l,cpy, n,RLE32));break;
    case 75:               TM("",l=srlezc32(        in, n,out,tmp,RLE32),n,l,srlezd32(     out,l,cpy, n,RLE32));break;
      #ifdef _ICCODEC
    case 76:               TM("",l=tprleenc(   in,n,out,ns,USIZE,tmp) ,n,l, tprledec( out,l,cpy,n,USIZE,tmp)); break;
    case 77:               TM("",l=tprlexenc(  in,n,out,ns,USIZE,tmp) ,n,l, tprlexdec(out,l,cpy,n,USIZE,tmp)); break;
    case 78:               TM("",l=tprlezenc(  in,n,out,ns,USIZE,tmp) ,n,l, tprlezdec(out,l,cpy,n,USIZE,tmp)); break;

    case 80:               TM("",l=codecenc(   in,n,out,ns,          codid,codlev,codprm),       n,l, codecdec( out,l,cpy,n,          codid,codlev,codprm));       break;
    case 81:               TM("",l=lztpenc(    in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 82:               TM("",l=lztpxenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpxdec( out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 83:               TM("",l=lztpzenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpzdec( out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 84:               TM("",l=lztp4enc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpd4ec( out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 85:               TM("",l=lztp4xenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztp4xdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 86:               TM("",l=lztp4zenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztp4zdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
        #ifdef _BITSHUFFLE
    case 87:               TM("",l=lztp1enc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lztp1dec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 88:               TM("",l=lztp1xenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lztp1xdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 89:               TM("",l=lztp1zenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lztp1zdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
        #endif
    case 90:               TM("",l=lztprleenc( in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lztprledec( out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 91:               TM("",l=lztprlexenc(in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lztprlexdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 92:               TM("",l=lztprlezenc(in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lztprlezdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 93:               TM("",l=lzv8enc(    in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lzv8dec(    out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 94:               TM("",l=lzv8xenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lzv8xdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 95:               TM("",l=lzv8zenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm), n,l, lzv8zdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 96:               TM("",l=vlccomp32(  in,n,out,ns,      tmp,codid,codlev,codprm), n,l, l==n?memcpy(cpy,in,n):(void *)vlcdecomp32(out, l, cpy, n, tmp,codid,codlev,codprm)); break;
    case 97:               TM("",l=vhicomp32(  in,n,out,ns,      tmp,codid,codlev,codprm), n,l, l==n?memcpy(cpy,in,n):(void *)vhidecomp32(out, l, cpy, n, tmp,codid,codlev,codprm)); break;

    case 100: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1);  if(verbose) printf("2D=%dx%d ", nx,_ny);
                           TM("",l=lztpd2enc( in,n,out,ns,USIZE,tmp,nx,_ny,codid,codlev,codprm),n,l, lztpd2dec( out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm));
	} break;
    case 101: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1);
                           TM("",l=lztpd2xenc(in,n,out,ns,USIZE,tmp,nx,_ny,codid,codlev,codprm),n,l, lztpd2xdec(out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm));
	} break;
    case 102: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1);
                           TM("",l=lztpd2zenc(in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm),n,l, lztpd2zdec(out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm));
	} break;	
    case 103: if(nz>0) { unsigned _nz = nz*(nw?nw:1);
                           TM("",l=lztpd3enc( in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm),n,l, lztpd3dec( out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm));
	} break;
    case 104: if(nz>0) { unsigned _nz = nz*(nw?nw:1);
                           TM("",l=lztpd3xenc(in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm),n,l, lztpd3xdec(out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm));
	} break;
    case 105: if(nz>0) { unsigned _nz = nz*(nw?nw:1);
                           TM("",l=lztpd3zenc(in,n,out,ns,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm),n,l, lztpd3zdec(out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm));
	} break;	
    case 106: if(nw>0) {   TM("",l=lztpd4enc( in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm), n,l, lztpd4dec( out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm));} break;
    case 107: if(nw>0) {   TM("",l=lztpd4xenc(in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm), n,l, lztpd4xdec(out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm));} break;
    case 108: if(nw>0) {   TM("",l=lztpd4zenc(in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm), n,l, lztpd4zdec(out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm));} break;
      #endif
    case 110:              TM("",l=vlcenc32( in, n, out), n,l, l==n?memcpy(cpy,in,n):(void*)vlcdec32( out, n,cpy)); break;
    case 111:              TM("",l=vlczenc32(in, n, out), n,l, l==n?memcpy(cpy,in,n):(void*)vlczdec32(out, n,cpy)); break;
    case 113:              TM("",l=bitgenc32(in, n, out), n,l, l==n?memcpy(cpy,in,n):(void*)bitgdec32(out, n,cpy)); break;
    case 114:              TM("",l=bitrenc32(in, n, out), n,l, l==n?memcpy(cpy,in,n):(void*)bitrdec32(out, n,cpy)); break;
	
    case 117: l = n;       TM("", tpenc( in, n, out,USIZE), n,l, tpdec( out, n,cpy, USIZE)); break;
    case 118: l = n;       TM("", tp4enc(in, n, out,USIZE), n,l, tp4dec(out, n,cpy, USIZE)); break;
      #ifdef _BITSHUFFLE
    case 119: l = n;       TM("", bitshuffle(in, n, out, USIZE), n,l, bitunshuffle(out, n,cpy, USIZE)); break;
      #endif
    case ID_MEMCPY: if(mcpy) { l = n; TM("", libmemcpy(out,in,n), n, l, libmemcpy(cpy,out,n)); } break;

	  #ifdef _VTENC
    case 121: if(dm != (uint32_t)-1) { size_t _l; TM("",vtenc_list_encode_u32(in, m, out,ns,&_l), n,_l, vtenc_list_decode_u32(out, _l, cpy, m)); l = _l; } break;
	  #endif
	//122 vbz16
      #ifdef _STREAMVBYTE
    case 130:              TM("",l=streamvbyte_encode(in, m, out),          n,l, streamvbyte_decode(      out, cpy, m)); break;
    case 131:              TM("",l=streamvbyte_delta_encode(in,m,out,0),    n,l, streamvbyte_delta_decode(out, cpy, m,0)); break;
    case 132:              TM("",l=streamvbyte_zzag_encode( in,m,out,0,tmp),n,l, streamvbyte_zzag_decode( out, cpy, m,0,tmp)); break;
      #endif
	  #ifdef _FASTPFOR
    case 133:              TM("",l=vbyte_encode(in, m, out),n,l, masked_vbyte_decode(out, cpy, m)); break;
    case 134:              TM("",l=FastPFore32(    in, m, out,ns),n,l, FastPFord32(    out, m, cpy)); break;
    case 135:              TM("",l=FastPFore128v32(in, m, out,ns),n,l, FastPFord128v32(out, m, cpy)); break;
    case 136:              TM("",l=OptPFore128v32( in, m, out,ns),n,l, OptPFord128v32( out, m, cpy)); break;
	  #endif  
	  #ifdef _SPDP
    case 137:              TM("",l=spdpenc(in,m*(USIZE),out,SPDPSIZE,codlev),n,l, spdpdec(           out, m*(USIZE), cpy,SPDPSIZE,codlev)); break;
      #endif
	  
	  #ifdef _ZFP
    case 140:              TM("",l = zfpcompress(in,m,0,0,0, out, ns, zfp_type_float, zerrlim),n,l, zfpdecompress(out, l, cpy,m,0,0,0, zfp_type_float, zerrlim));
	  if(zerrlim > DBL_EPSILON && verbose) fpstat(in, m, cpy, -4, tmp); memcpy(cpy,in,n);   //lossy compression irreversible
	break;
    case 141: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1);
                           TM("",l = zfpcompress(in,nx,_ny,0,0, out, ns, zfp_type_float, zerrlim),n,l, zfpdecompress(out, l, cpy,nx,_ny,0,0, zfp_type_float, zerrlim));
	  if(zerrlim > DBL_EPSILON && verbose) fpstat(in, m, cpy, -4, tmp); memcpy(cpy,in,n); //lossy 
	} break;
    case 142: if(nz>0) { unsigned _nz = nz*(nw?nw:1);
                           TM("",l = zfpcompress(in,nx,ny,_nz,0, out, ns, zfp_type_float, zerrlim),n,l, zfpdecompress(out, l, cpy,nx,ny,_nz,0, zfp_type_float, zerrlim));
	  if(zerrlim > DBL_EPSILON && verbose) fpstat(in, m, cpy, -4, tmp); memcpy(cpy,in,n);   //lossy 
	} break;
	  #endif
  	  
	  #ifdef _BLOSC
    case 143: {                TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_BITSHUFFLE,           0,            0),n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); } break;
    case 144: {                TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_BITSHUFFLE, BLOSC_DELTA,            0),n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); } break;
    case 145: { blosc2_init(); TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_BITSHUFFLE, BLOSC_FILTER_BYTEDELTA, 0),n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); } break;
    case 146: {                TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_SHUFFLE,              0,            0),n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); } break;
    case 147: {                TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_SHUFFLE,    BLOSC_DELTA,            0),n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); } break;
    case 148: { blosc2_init(); TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_FILTER_BYTEDELTA, BLOSC_SHUFFLE,    0),n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); } break;
	  #endif
	// ----- speed test & lossy error bound analysis (with option -v1) -----------------------	
    case 149: l=n;             TM0("", fprazor32(  in, m, out,zerrlim), n, l);                                         memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -4, tmp); break;
	  #ifdef _BITGROOMING	
    case 150:ptr_unn p;l=n;    TM0("", memcpy(out,in,n);ccr_gbr(nsd, NC_FLOAT, m, 0, p, out),n,l);                     memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -4, tmp); break;
    case 151: l = n;           TM0("", BG_compress_args(BG_FLOAT, in, NULL, BITGROOM, BG_NSD, nsd, nsd, m, out), n,l); memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -4, tmp); break;
 	  #endif
	  #ifdef _LIBROUNDFAST // digirounding algo
    case 152: l = n;           TM0("", fround32(in, m, out, nsd),n,l);                                                 memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -4, tmp); break;
	  #endif
	// ----- speed test transpose integrated -----------------------
    case 153: TM("", tpzenc(  in, n, out, USIZE),     n,n, tpzdec(  out, n,cpy, USIZE)); l = n; break;
    case 154: TM("", tpz0enc( in, n, out, USIZE, tmp),n,n, tpz0dec( out, n,cpy, USIZE)); l = n; break;
    case 155: TM("", tpxenc(  in, n, out, USIZE),     n,n, tpxdec(  out, n,cpy, USIZE)); l = n; break;
    case 156: TM("", tpx0enc( in, n, out, USIZE, tmp),n,n, tpx0dec( out, n,cpy, USIZE)); l = n; break;
    case 157: TM("", tp4zenc( in, n, out, USIZE),     n,n, tp4zdec( out, n,cpy, USIZE)); l = n; break;
    case 158: TM("", tp4z0enc(in, n, out, USIZE, tmp),n,n, tp4z0dec(out, n,cpy, USIZE)); l = n; break;
    case 159: TM("", tp4xenc( in, n, out, USIZE),     n,n, tp4xdec( out, n,cpy, USIZE)); l = n; break;
    case 160: TM("", tp4x0enc(in, n, out, USIZE, tmp),n,n, tp4x0dec(out, n,cpy, USIZE)); l = n; break;
	  #ifdef _MESHOPT
    case 170: TM("", l = meshenc(in, m,0,0, out, ns, tmp,codid,codlev,codprm), n,l, meshdec(out, l,cpy, m,0,0, tmp,codid,codlev,codprm)); break;	
    case 171: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("", l = meshenc(in, nx,_ny,0, out, ns, tmp,codid,codlev,codprm),n,l, meshdec(out, l,cpy, nx,_ny,0, tmp,codid,codlev,codprm)); } break;
    case 172: if(nz>0) { unsigned _nz = nz*(nw?nw:1);           TM("", l = meshenc(in, nx,ny,_nz, out, ns, tmp,codid,codlev,codprm), n,l, meshdec(out, l,cpy, nx,ny,_nz, tmp,codid,codlev,codprm)); } break;
	  #endif
      #ifdef _QCOMPRESS
    case 173: if(codlev < 1) codlev = 1;if(codlev > 9) codlev = 9; TM("",l = qcomp32( in, n, out, codlev), n,l, qdecomp32( out, l, cpy,n)); break;
    case 174: if(codlev < 1) codlev = 1;if(codlev > 9) codlev = 9; TM("",l = qzcomp32(in, n, out, codlev,tmp), n,l, qzdecomp32(out, l, cpy,n)); break;
      #endif	  
    default: goto end;
  }
  if(l) {
    unsigned char s[128] = { 0 };                                                         AC(codstr(codid), "Fatal"); printf("%-40s ", bestr(id, 32, s, codstr(codid), codlev));
    if(cpy) rc = memcheck32(in,m,cpy);
    if(!rc)
      printf("\t%s\n", inname?inname:"");
  }
  end:if(tmp) free(tmp);
  return l;
}

#undef USIZE
#define USIZE 8
unsigned bench64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int codlev, unsigned bsize) {
  unsigned      l = 0,m = n/(USIZE), rc = 0, d = 0, ns = CBUF(n);
  uint64_t      dm = mindelta64(in,m);
  uint64_t      *p = NULL;
  unsigned char *tmp = NULL;
  if(/*NEEDTMP &&*/ !(tmp = (unsigned char*)malloc(ns))) die(stderr, "malloc error\n");
  memrcpy(cpy,in,n);

  switch(id) {
    case  1: TM("",l=p4nenc64(        in, m, out),  n,l, p4ndec64(          out, m, cpy)); break;
      #ifndef _NSSE
    case  2: TM("",l=p4nenc128v64(    in, m, out),  n,l, p4ndec128v64(      out, m, cpy)); break;
      #endif
    case  4: TM("",l=p4ndenc64(       in, m, out),  n,l, p4nddec64(         out, m, cpy)); break;
  //case  5: TM("",l=p4ndenc128v64(   in, m, out),  n,l, p4nddec128v64(     out, m, cpy)); break;

    case  7: TM("",l=p4nd1enc64(      in, m, out),  n,l, p4nd1dec64(        out, m, cpy)); break;
  //case  8: TM("",l=p4nd1enc128v64(  in, m, out),  n,l, p4nd1dec128v64(    out, m, cpy)); break;

    case 10: TM("",l=p4nzenc64(       in, m, out),  n,l, p4nzdec64(         out, m, cpy)); break;
  //case 11: TM("",l=p4nzenc128v64(   in, m, out),  n,l, p4nzdec128v64(     out, m, cpy)); break;

      #ifndef _NSSE
    case 13: TM("",l=p4nzzenc128v64(  in, m, out,0),n,l, p4nzzdec128v64(    out, m, cpy,0)); break;
      #endif
    case 20: TM("",l=bitnpack64(      in, m, out),  n,l, bitnunpack64(      out, m, cpy)); break;
      #ifndef _NSSE
    case 21: TM("",l=bitnpack128v64(  in, m, out),  n,l, bitnunpack128v64(  out, m, cpy)); break;
      #endif

    case 23: TM("",l=bitndpack64(     in, m, out),  n,l, bitndunpack64(     out, m, cpy)); break;
  //case 24: TM("",l=bitndpack128v64( in, m, out),  n,l, bitndunpack128v64( out, m, cpy)); break;

    case 26: TM("",l=bitnd1pack64(    in, m, out),  n,l, bitnd1unpack64(    out, m, cpy)); break;
  //case 27: TM("",l=bitnd1pack128v64(in, m, out),  n,l, bitnd1unpack128v64(out, m, cpy)); break;

    case 29: TM("",l=bitnzpack64(     in, m, out),  n,l, bitnzunpack64(     out, m, cpy)); break;
  //case 30: TM("",l=bitnzpack128v64( in, m, out),  n,l, bitnzunpack128v64( out, m, cpy)); break;

    case 32: if(dm!=(uint64_t)-1) TM("",l=bitnfpack64( in, m, out),  n,l, bitnfunpack64(     out, m, cpy)); break;
  //case 33: if(dm==-1) goto end;
  //         TM("",l=bitnfpack128v64( in, m, out),  n,l" 33",bitnfunpack128v64( out, m, cpy)); break;

  //case 35: if(dm==-1) goto end;
  //         TM("",l=bitns1pack128v64( in, m, out),  n,l" 35",bitns1unpack128v64( out, m, cpy)); break;
  //case 35: if(dm==-1 /*|| !dm*/) goto end;  TM("",l=efanoenc64(     in, m, out,0),  n,l"efanoenc64       ",efanodec64( out, m, cpy,0)); break;
    case 36: TM("",l=bitnxpack64( in, m, out),         n,l, bitnxunpack64(out, m, cpy)); break;
    case 38: TM("",l=vsenc64(     in, m, out)-out,     n,l, vsdec64(      out, m, cpy)); break;   // vsimple : variable simple
    case 39: TM("",l=vszenc64(    in, m, out,tmp)-out, n,l, vszdec64(     out, m, cpy)); break;

    case 40: TM("",l=vbenc64(     in, m, out)-out,     n,l, vbdec64(      out, m, cpy));   break; // TurboVbyte : variable byte
    case 41: TM("",l=vbzenc64(    in, m, out,0)-out,   n,l, vbzdec64(     out, m, cpy,0)); break;
    case 42: TM("",l=vbdenc64(    in, m, out,0)-out,   n,l, vbddec64(     out, m, cpy,0)); break;
    case 43: TM("",l=vbd1enc64(   in, m, out,0)-out,   n,l, vbd1dec64(    out, m, cpy,0)); break;
    case 44: TM("",l=vbddenc64(   in, m, out,0)-out,   n,l, vbdddec64(    out, m, cpy,0)); break;

    case 60: TM("",l=bvzzenc64(   in, m, out,0),       n,l, bvzzdec64(    out, m, cpy,0)); break; // bitio
    case 61: TM("",l=bvzenc64(    in, m, out,0),       n,l, bvzdec64(     out, m, cpy,0)); break;
    case 62: TM("",l=fpgenc64(    in, m, out,0),       n,l, fpgdec64(     out, m, cpy,0)); break;
    case 63: TM("",l=fphenc64(    in, m, out,0),       n,l, fphdec64(     out, m, cpy,0)); break;
    case 64: TM("",l=fpcenc64(    in, m, out,0),       n,l, fpcdec64(     out, m, cpy,0)); break;

    case 65: TM("",l=fpxenc64(    in, m, out,0),       n,l, fpxdec64(     out, m, cpy,0)); break; //Floating point/Integer
    case 66: TM("",l=fpfcmenc64(  in, m, out,0),       n,l, fpfcmdec64(   out, m, cpy,0)); break;
    case 67: TM("",l=fpdfcmenc64( in, m, out,0),       n,l, fpdfcmdec64(  out, m, cpy,0)); break;
    case 68: TM("",l=fp2dfcmenc64(in, m, out,0),       n,l, fp2dfcmdec64( out, m, cpy,0)); break;

    case 70: TM("",l=trlec(       in, n,out),          n,l, trled(        out,l,cpy, n));       break;  // TurboRLE
    case 71: TM("",l=trlexc(      in, n,out,tmp),      n,l, trlexd(       out,l,cpy, n));       break;
    case 72: TM("",l=trlezc(      in, n,out,tmp),      n,l, trlezd(       out,l,cpy, n));       break;
    case 73: TM("",l=srlec64(     in, n,out,RLE64),    n,l, srled64(      out,l,cpy, n,RLE64)); break;
    case 74: TM("",l=srlexc64(    in, n,out,tmp,RLE64),n,l, srlexd64(     out,l,cpy, n,RLE64)); break;
    case 75: TM("",l=srlezc64(    in, n,out,tmp,RLE64),n,l, srlezd64(     out,l,cpy, n,RLE64)); break;
      #ifdef _ICCODEC
    case 80: TM("",l=codecenc(   in,n,out,ns,codid,codlev,codprm) ,n,l, codecdec(out,l,cpy,n,codid,codlev,codprm)); break;
    case 81: TM("",l=lztpenc(    in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpdec(    out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 82: TM("",l=lztpxenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpxdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 83: TM("",l=lztpzenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpzdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 84: TM("",l=lztp4enc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztpd4ec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 85: TM("",l=lztp4xenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztp4xdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
    case 86: TM("",l=lztp4zenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm,bsize), n,l, lztp4zdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm,bsize)); break;
        #ifdef _BITSHUFFLE
    case 87: TM("",l=lztp1enc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1dec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 88: TM("",l=lztp1xenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1xdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 89: TM("",l=lztp1zenc(  in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztp1zdec(  out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
        #endif
    case 90: TM("",l=lztprleenc( in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprledec( out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 91: TM("",l=lztprlexenc(in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprlexdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 92: TM("",l=lztprlezenc(in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lztprlezdec(out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 93: TM("",l=lzv8enc(    in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8dec(    out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 94: TM("",l=lzv8xenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8xdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;
    case 95: TM("",l=lzv8zenc(   in,n,out,ns,USIZE,tmp,codid,codlev,codprm),       n,l, lzv8zdec(   out,l,cpy,n,USIZE,tmp,codid,codlev,codprm)); break;

    case 100: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2enc( in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm), n,l, lztpd2dec( out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm)); } break;
    case 101: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2xenc(in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm), n,l, lztpd2xdec(out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm)); } break;
    case 102: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l=lztpd2zenc(in,n,out,ns,USIZE,tmp, nx,_ny,codid,codlev,codprm), n,l, lztpd2zdec(out,l,cpy,n,USIZE,tmp, nx,_ny,codid,codlev,codprm)); } break;

    case 103: if(nz>0)   { unsigned _nz = nz*(nw?nw:1); TM("",l=lztpd3enc( in,n,out,ns,USIZE,tmp,nx,ny,_nz,codid,codlev,codprm),  n,l, lztpd3dec( out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm)); } break;
    case 104: if(nz>0)   { unsigned _nz = nz*(nw?nw:1); TM("",l=lztpd3xenc(in,n,out,ns,USIZE,tmp,nx,ny,_nz,codid,codlev,codprm),  n,l, lztpd3xdec(out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm)); } break;
    case 105: if(nz>0)   { unsigned _nz = nz*(nw?nw:1); TM("",l=lztpd3zenc(in,n,out,ns,USIZE,tmp,nx,ny,_nz,codid,codlev,codprm),  n,l, lztpd3zdec(out,l,cpy,n,USIZE,tmp, nx,ny,_nz,codid,codlev,codprm)); } break;
    case 106: if(nw>0)   {                              TM("",l=lztpd4enc( in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm),n,l, lztpd4dec( out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm)); } break;
    case 107: if(nw>0)   {                              TM("",l=lztpd4xenc(in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm),n,l, lztpd4xdec(out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm));} break;
    case 108: if(nw>0)   {                              TM("",l=lztpd4zenc(in,n,out,ns,USIZE,tmp,nx,ny,nz,nw,codid,codlev,codprm),n,l, lztpd4zdec(out,l,cpy,n,USIZE,tmp, nx,ny,nz,nw,codid,codlev,codprm)); } break;
      #endif
    case 117: l = n; TM("", tpenc( in, n, out,USIZE),n,l, tpdec( out, n,cpy, USIZE)); break;
    case 118: l = n; TM("", tp4enc(in, n, out,USIZE),n,l, tp4dec(out, n,cpy, USIZE)); break;
      #ifdef _BITSHUFFLE
    case 119: l = n; TM("", bitshuffle(in, n, out, USIZE),n,l, bitunshuffle(out, n,cpy, USIZE)); break;
      #endif
    case ID_MEMCPY: if(mcpy) { TM("", libmemcpy(out,in,n), n,n, libmemcpy( cpy,out,n)); l = n; } break;

      #ifdef _SPDP
    case 137: TM("",l=spdpenc(in,m*(USIZE),out,SPDPSIZE,codlev),n,l"109",spdpdec(           out, m*(USIZE), cpy,SPDPSIZE,codlev); ,n); break;
      #endif
	  
	  #ifdef _ZFP
    case 140: { TM("",l = zfpcompress(in,m,0,0,0, out, ns, zfp_type_double, zerrlim), n,l, zfpdecompress(out, l, cpy,m,0,0,0, zfp_type_double, zerrlim));
	  if(zerrlim > DBL_EPSILON) { if(verbose) fpstat(in, m, cpy, -8, tmp); memcpy(cpy,in,n); } //lossy compression irreversible
	} break;
    case 141: if(ny>0) { unsigned _ny = ny*(nz?nz:1)*(nw?nw:1); TM("",l = zfpcompress(in,nx,_ny,0,0, out, ns, zfp_type_double, zerrlim),n,l,zfpdecompress(out, l, cpy,nx,_ny,0,0, zfp_type_double, zerrlim));
	  if(zerrlim > DBL_EPSILON) { if(verbose) fpstat(in, m, cpy, -8, tmp); memcpy(cpy,in,n); } //lossy compression irreversible
	} break;
    case 142: if(nz>0) { unsigned _nz = nz*(nw?nw:1);           TM("",l = zfpcompress(in,nx,ny,_nz,0, out, ns, zfp_type_double, zerrlim),n,l,zfpdecompress(out, l, cpy,nx,ny,_nz,0, zfp_type_double, zerrlim));
	  if(zerrlim > DBL_EPSILON) { if(verbose) fpstat(in, m, cpy, -8, tmp); memcpy(cpy,in,n); } //lossy compression irreversible
	} break;
	  #endif
	  
	  #ifdef _BLOSC
    case 143: TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_BITSHUFFLE,           0,            0), n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); break;
    case 144: TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_BITSHUFFLE, BLOSC_DELTA,            0), n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); break;
    case 145: TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_BITSHUFFLE, BLOSC_FILTER_BYTEDELTA, 0), n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); break;
    case 146: TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_SHUFFLE,              0,            0), n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); break;
    case 147: TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_SHUFFLE,    BLOSC_DELTA,            0), n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); break;
    case 148: TM("",l = blosccomp(in, n, out, ns, codid, codlev, USIZE, BLOSC_FILTER_BYTEDELTA, BLOSC_SHUFFLE,    0), n,l, l==n?memcpy(cpy,in,n):bloscdecomp(out, l, cpy, n,USIZE)); break;
	  #endif
	// ----- speed test & lossy error bound analysis (with option -v1) -----------------------	
    case 149: TM("", fprazor64(in,m,out,zerrlim),                                       n,n, fprazor64( in, m, out,zerrlim));                                    memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -8, tmp); l=n; break;
	  #ifdef _BITGROOMING	
    case 150: ptr_unn p; 
	          TM("", memcpy(out,in,n);ccr_gbr(nsd, NC_DOUBLE, m, 0, p, out),            n,n, memcpy(out,in,n);ccr_gbr(nsd, NC_DOUBLE,m,0,p,out));                memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -8, tmp); l=n; break;
    case 151: TM("", BG_compress_args(BG_DOUBLE,in,NULL,BITGROOM,BG_NSD,nsd,nsd,m,out), n,n, BG_compress_args(BG_DOUBLE,in,NULL,BITGROOM,BG_NSD,nsd,nsd,m,out)); memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -8, tmp); l=n; break;
 	  #endif 
	  #ifdef _LIBROUNDFAST // digirounding algo
    case 152: TM("", fround64( in, m, out, nsd),                                        n,n, fround64(in, m, out, nsd));                                         memcpy(cpy,in,n); if(verbose) fpstat(in, m, out, -8, tmp); l=n; break;
	  #endif
    case 153: TM("", tpzenc(  in, n, out, USIZE),      n,n, tpzdec(  out, n,cpy, USIZE)); l = n; break;
    case 154: TM("", tpz0enc( in, n, out, USIZE, tmp), n,n, tpz0dec( out, n,cpy, USIZE)); l = n; break;
    case 155: TM("", tpxenc(  in, n, out, USIZE),      n,n, tpxdec(  out, n,cpy, USIZE)); l = n; break;
    case 156: TM("", tpx0enc( in, n, out, USIZE, tmp), n,n, tpx0dec( out, n,cpy, USIZE)); l = n; break;
    case 157: TM("", tp4zenc( in, n, out, USIZE),      n,n, tp4zdec( out, n,cpy, USIZE)); l = n; break;
    case 158: TM("", tp4z0enc(in, n, out, USIZE, tmp), n,n, tp4z0dec(out, n,cpy, USIZE)); l = n; break;
    case 159: TM("", tp4xenc( in, n, out, USIZE),      n,n, tp4xdec( out, n,cpy, USIZE)); l = n; break;
    case 160: TM("", tp4x0enc(in, n, out, USIZE, tmp), n,n, tp4x0dec(out, n,cpy, USIZE)); l = n; break;
      #ifdef _QCOMPRESS
    case 173: if(codlev < 1) codlev = 1;if(codlev > 9) codlev = 9; TM("",l = qcomp64( in, n, out, codlev),     n,l, qdecomp64( out, l, cpy,n)); break;
    case 174: if(codlev < 1) codlev = 1;if(codlev > 9) codlev = 9; TM("",l = qzcomp64(in, n, out, codlev,tmp), n,l, qzdecomp64(out, l, cpy,n)); break;
      #endif	  
    default: goto end;
  }
  if(l) {
    unsigned char s[65]; printf("%-30s ", bestr(id, 64,s, codstr(codid),codlev));
    if(cpy) rc = memcheck(in,m*(USIZE),cpy);
    if(!rc)
      printf("\t%s\n", inname?inname:"");
  }
  end:if(tmp) free(tmp);
  return l;
}

typedef struct len_t { unsigned id,cnt; uint64_t len; } len_t;
#define CMPSA(_a_,_b_, _t_, _v_)  (((((_t_ *)_a_)->_v_) > (((_t_ *)_b_)->_v_)) - ((((_t_ *)_a_)->_v_) < (((_t_ *)_b_)->_v_)))
static int cmpsna(const void *a, const void *b) { return CMPSA(a, b, len_t, len); }

void usage(char *pgm) {
  fprintf(stderr, "\nIcApp Copyright (c) 2013-2023 Powturbo %s\n", __DATE__);
  fprintf(stderr, "Usage: %s [options] [file]\n", pgm);
  //fprintf(stderr, " -b#s     # = blocksize (default filesize,). max=1GB\n");
  fprintf(stderr, " -B#s     # = max. benchmark filesize (default 1GB) ex. -B4G\n");
  fprintf(stderr, "          s = modifier s:K,M,G=(1000, 1.000.000, 1.000.000.000) s:k,m,h=(1024,1Mb,1Gb). (default m) ex. 64k or 64K\n");
  fprintf(stderr, "Benchmark:\n");
  fprintf(stderr, " -i#/-j#  # = Minimum  de/compression iterations per run (default=auto)\n");
  fprintf(stderr, " -I#/-J#  # = Number of de/compression runs (default=3)\n");
  fprintf(stderr, " -e#      # = function ids separated by ',' or ranges '#-#' (default='1-%d')\n", ID_MEMCPY);
  fprintf(stderr, "File format:\n");
  fprintf(stderr, " -F[Xx[k][H]][.d]\n");
  fprintf(stderr, "    X = file format:\n");
  fprintf(stderr, "        t = text:one integer per line, k=column number in multiple columns line\n");
  fprintf(stderr, "        c = text:integers separated by non digit char\n");
  fprintf(stderr, "        r = raw:binary format (default)\n");
  fprintf(stderr, "    x = entry format\n");
  fprintf(stderr, "        [b=int8], [s=int16], [u=int32(default)], [l=int64], [f:float] [d:double]\n");
  fprintf(stderr, "    .# = decimal digits (default 2). Convert dec. numbers to integers\n");
  fprintf(stderr, "    H  = skip first line(s)\n");
  fprintf(stderr, " -H# = skip first # line(s). Only for text files\n");
  fprintf(stderr, " -K# = #:column number in multiple columns/line. Only for text files\n");
  fprintf(stderr, " -ks = s:separator(s) (default , ; and TAB) in multiple columns/line. Only for text files\n");
  fprintf(stderr, " -s# = #:integer size 2,4,8\n");
  fprintf(stderr, " -f# = #:floating point size 4,8\n");
  fprintf(stderr, " -t# = #:Timestamp in iso-8601 converted to seconds (32 bits)\n");
  fprintf(stderr, " -T# = #:Timestamp in iso-8601 converted to milliseconds (64 bits)\n");
  fprintf(stderr, " -V# = #:divisor. Only for text files\n");
  fprintf(stderr, " -D# = #:decimals. Only for text files\n");
  fprintf(stderr, " -g# = allowed relative error for lossy floating point compression (ex. -g.00001)\n");
  fprintf(stderr, " -RD1xD2xD3xD4 = D1,D2,D3,D4 : dimensions (ex. -R128x64x1024)\n");
  fprintf(stderr, "Output:\n");
  fprintf(stderr, " -v#   # = verbosity 0..5 (default 1). 5=print first values read from text file\n");
  fprintf(stderr, "----- arg. ZIPF specified --------------\n");
  fprintf(stderr, " -aF     F = zipfian distribution alpha ex. -a1.0 uniform -a1.5 skewed\n");
  fprintf(stderr, " -m#s    # = minimum number. s=(k,K, m,M, g,G\n");
  fprintf(stderr, " -M#s    # = maximum number\n");
  fprintf(stderr, " -n#s    # = number of integers to generate\n");
  fprintf(stderr, " -d#     Generate a non decreasing sequence. #:delta 0=non decreasing 1=strictly non decreasing\n");
  fprintf(stderr, "Examples:\n");
  fprintf(stderr, "./icapp -a1.5 -m0 -M255 -n100M ZIPF\n");
  fprintf(stderr, "./icapp -e45-59,90,91 -I15 -J15 -d0 ZIPF\n");
  fprintf(stderr, "./icapp -e45-59 -I15 -J15 -d1 -M16 -Fs -n10000 ZIPF\n");
  fprintf(stderr, "./icapp file -e2,7-9,14,15\n");
  fprintf(stderr, "./icapp -Ft -D2 ibm.data\n");
  fprintf(stderr, "./icapp -Ft -f8 usages.dat\n");
  fprintf(stderr, "./icapp -Ft -f8 -HH tripdata.csv\n");
  fprintf(stderr, "./icapp -Fd -g.001 double.raw\n");
  exit(0);
}

#if 0
testrazor() {  
  static double errlima[] = { 5e-1,5e-2, 5e-3, 5e-4,5e-5,5e-6,5e-7,5e-8,5e-9  };
 
  double pi = M_PI; uint64_t u = ctou64(&pi); printf("pi=%.20g - ctz=%u\n", M_PI, ctz64(u)); 
  for(int i = 0; i < 9; i++) { 
    double e = errlima[i], d = pi; 
	int lg2e = -log(e)/log(2.0), tz;  
	uint64_t u;
	d = _fprazor64(M_PI, e, lg2e);                                                   u = ctou64(&d); tz = u?ctz64(u):64; printf("TR:%u %0e:%.10g - %.10g ctz=%u\n", i, e, d, M_PI - d, tz );
	ptr_unn p; 
	d = M_PI; ccr_gbr(i+1, NC_DOUBLE, 1, 0, p, &d);                                  u = ctou64(&d); tz = u?ctz64(u):64; printf("BR:%u %0e:%.10g - %.10g ctz=%u\n", i+1, e, d, M_PI - d, tz );
	          BG_compress_args(BG_DOUBLE, &pi, NULL, BITGROOM, BG_NSD, i, i, 1, &d); u = ctou64(&d); tz = u?ctz64(u):64; printf("BG:%u %0e:%.10g - %.10g ctz=%u\n", i, e, d, M_PI - d, tz );
  } 
  exit(0);
}
#endif
int main(int argc, char* argv[]) { //testrazor(); 
  unsigned      b = 1 << 31, lz=0, fno,m=1000000, bsize = (unsigned)-1, quantb = 0;
  int           isize=4,dfmt = 0,kid=1,skiph=0,decs=0,divs=1,dim0=0;
  uint64_t      be_mindelta=0;
  unsigned char *in = NULL, *out, *cpy;
  char          *scmd = NULL, *icmd = NULL;
  double        mdelta=-10, errlim=-1.0;

  tm_verbose = 3;
  len_t         lens[256] = { 0 };
  for(fno=0; fno < 255; fno++)
	lens[fno].id = 0, lens[fno].len = (uint64_t)-1;

  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"blocsize",  0, 0, 'b'}, {0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "a:B:b:C:d:D:d:e:E:f:F:g:G:I:J:k:K:hH:l:m:M:n:p:q:R:s:v:V:w:W:yz:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;
	  case 'b': bsize = argtoi(optarg,1); break;
      case 'B': b = argtoi(optarg,1);        break;
      case 'C': cmp    = atoi(optarg); break;
      case 'e': icmd   = optarg; break;
      case 'E': scmd   = optarg; break;
      case 'D': decs   = atoi(optarg);  break;

      case 'F': { char *s = optarg;
             if(*s=='c') { dfmt = T_CHAR; s++;}
        else if(*s=='t') { dfmt = T_TXT; if(*++s > '0' && *s <= '9') { kid = *s++ - '0'; if(*s > '0' && *s <= '9') kid = kid*10 + (*s++ - '0'); } }
        else if(*s=='e') { dfmt = T_TST; s++; }
        else if(*s=='r') { dfmt = 0;     s++; } // default

             if(*s=='g') isize = -2, s++; // float : 2 bytes
        else if(*s=='f') isize = -4, s++; // float : 4 bytes
        else if(*s=='d') isize = -8, s++; // double: 8 bytes
        else if(*s=='b') isize =  1, s++; // 1 byte
        else if(*s=='s') isize =  2, s++; // 2 bytes
        else if(*s=='u') isize =  4, s++; // 4 bytes
        else if(*s=='l') isize =  8, s++; // 8 bytes
        else if(*s=='t') isize =  4, s++, dfmt = T_TIM32; // 4 bytes, timestamp text
        else if(*s=='T') isize =  8, s++, dfmt = T_TIM64; // 8 bytes, timestamp text
        if(*s == '.') { if(*++s >= '0' && *s <= '9') { decs = s[0] - '0'; if(*++s >= '0' && *s <= '9') decs = decs * 10 + s[0] - '0'; } } // number of decimals after .
		else if(*s == 'A') autoraz++;
        if(*s == 'v') { divs = strtod(++s, &s); }
        //if(*s == 'H') { skiph++; s++; } // skip first line(s). ex.  HHH : skip 3 first lines
        //switch(*s) { case 's': be_mindelta = 0; break; case 'S': be_mindelta = 1; break; case 'z': be_mindelta = 2; break; }
      } break;

      case 'G': nsd    = atoi(optarg);  break;
      case 'g': errlim = strtod(optarg, NULL); break;
	  case 'h': usage(argv[0]); exit(0); break;
      case 'H': skiph = atoi(optarg);          break;
      case 'K': { kid = atoi(optarg); if(!keysep) keysep = ",;\t"; } break;
      case 'k': keysep = optarg; break;
      case 'I': if((tm_Rep  = atoi(optarg))<=0) tm_rep=tm_Rep=1; break;
      case 'J': if((tm_Rep2 = atoi(optarg))<=0) tm_rep=tm_Rep2=1; break;
      case 's': isize = argtoi(optarg,1);    break;
      //case 'c': cmp++;                         break;
      case 'l': codlev = atoi(optarg);           break;
      case 'y': mcpy = 0;                    break;

      case 'a': a = strtod(optarg, NULL);    break;
      case 'd': mdelta  = strtod(optarg, NULL); /*printf("MDELTA=%d ");*/break;
      case 'n': m       = argtoi(optarg,1);  break;
      case 'm': rm      = argtoi(optarg,1);  break;
      case 'M': rx      = argtoi(optarg,1);  break;
      case 'p': { unsigned m = atoi(optarg); if(m==4 || m==8) tpmodeset(m);else die("transpose mode (option -p) must be 4 or 8\n"); } break;
	  case 'q': quantb = atoi(optarg); if(quantb < 8) quantb = 8; break;
      case 'f': isize   = -argtoi(optarg,1); break;
      case 'R': { char *p;        nx = strtoul(optarg, &p, 10); if(!nx) dim0++;
                           if(*p) ny = strtoul(p+1,    &p, 10);
                           if(*p) nz = strtoul(p+1,    &p, 10);
                           if(*p) nw = strtoul(p+1,    &p, 10);                  if(verbose>1) printf("dim=%dx%dx%dx%d\n",  nx, ny, nz, nw);
                }   break;
      case 'w': tm_verbose = atoi(optarg); break;
      case 'v': verbose = atoi(optarg);    break;
      case 'V': tm_verbose = atoi(optarg);  break;
      case 'W': divs    = atoi(optarg);  break;
      case 'z': zerrlim = strtod(optarg, NULL); break;
      default:
	    fprintf(stderr, "type icapp -h for help\n");
        exit(0);
    }
  }
    #ifdef _LZTURBO
  beini(); 
    #endif
  if(argc - optind < 1) {
    usage(argv[0]);
    exit(0);
  }
  isa = cpuisa();
  cpuini(0); 																		if(verbose>1) printf("detected simd id=%x, %s\n\n", cpuini(0), cpustr(cpuini(0)));
  char _scmd[33];
    #ifdef _LZ4
  strcpy(_scmd, "lz4,1");
    #else
  strcpy(_scmd, "memcpy");
	#endif
  if(!scmd) scmd = _scmd;
  while(isspace(*scmd)) scmd++;
  char *q;
  int   i;
  if((q = strchr(scmd,',')) != NULL) *q = '\0';
  codid = lzidget(scmd);
  scmd = q?(q+1):"";
  codlev = strtoul(scmd, &scmd, 10);

  if(scmd) strcpy((char *)codprm,scmd);
  unsigned fcnt = 0;                                                                                  if(verbose>1) printf("dfmt=%d,size=%d\n", dfmt, isize);
  for(fno = optind; fno < argc; fno++) {
    char *inname = argv[fno];
    int       i=0,n;
    long long flen;
    FILE      *fi = NULL;
    if(!strcmp(inname,"ZIPF") || !strcmp(inname,"TMS")) flen = n = m*abs(isize);
    else {
      fi = fopen(inname, "rb");                             if(!fi) { perror(inname); continue; }
      if(dfmt) {
        n = flen = befgen(&in, 0, dfmt, isize, fi, kid, skiph, decs, divs, 0 /*be_mindelta*/, errlim);
      } else {
        fseek(fi, 0, SEEK_END);
        flen = ftell(fi);
        fseek(fi, 0, SEEK_SET);
        if(flen > b) flen = b;
        n = flen;
        if(dim0) { 
		  char *q,*p = &inname[strlen(inname)]; 
		  if((q = strrchr(inname, '.')) != NULL) p = q;
          nx = ny = nz = nw = 0;
          while(p > &inname[0] && (isdigit(p[-1]) || p[-1]=='x') ) p--;         if(verbose>1) printf("fn='%s' ", p);
                          nx = strtoul(p,   &p, 10);
           if(nx && *p) ny = strtoul(p+1, &p, 10);
           if(ny && *p) nz = strtoul(p+1, &p, 10);
           if(nz && *p) nw = strtoul(p+1, &p, 10);
		   if(verbose > 1 && nx) {       printf("dim=%u", nx);
		     if(ny) {     printf("x%u", ny);
               if(nz) {   printf("x%u", nz);
		         if(nw) { printf("x%u", nw); }}}
			 printf(" ");
		   }		 
        }
      }
    }
    //if(nw && !nz) nz = 1; if(nz && !ny) ny = 1; if(ny && !nx) nx = 1;
    //if(nx || ny || nz) printf("dim=%dx%dx%dx%d\n", nx, ny, nz, nw);

    if(!in && !(in  = (unsigned char*)malloc(n+64+1024        ))) die("malloc error 'in =%d'\n", n); cpy = in;
    if(!(out =        (unsigned char*)malloc(CBUF(n)))) die("malloc error 'out=%d'\n", n);
    if(cmp && !(cpy = (unsigned char*)malloc(CBUF(n)))) die("malloc error 'cpy=%d'\n", n);
    if(fi) {
      if(!dfmt) n = fread(in, 1, n, fi);
      fclose(fi);
	  int delta = mdelta;
	  if(delta>=0) { uint32_t *_in = in,*p,m = n/sizeof(_in[0]); for(p = _in+1; p < _in+m; p++) { uint64_t u = (uint64_t)p[0]+p[-1]+delta; if(u>0xffffffffull) { printf("delta overflow\n"); exit(0); } p[0]=u; }
                  printf("delta=%d in[m-1]=%u ", delta, _in[m-1]);
				  for(unsigned i = 1; i < m; i++) { AC(_in[i]>_in[i-1], "icapp: Not sorted at=%u,count=%d\n", i, n); }
      }
    } else if(!strcmp(inname,"TMS") && abs(isize) == 8)
      tms64(in, m, rm, rx, a);
    else
      datagen(in, m, isize, mdelta);
    if(n <= 0) exit(0);

    //if(fno == optind)
    tm_init(tm_Rep, tm_verbose /* 2 print id */);
    
    if(errlim > DBL_EPSILON/*|| nsd >= 0*/) {   // convert input for lossy floating point compression
      if(errlim > DBL_EPSILON && errlim < 0.0000009999) errlim = 0.000001;	    if(verbose>0) printf("Lossy compression float\n");
           if(isize == -4) fprazor32(in,n/4,out,errlim);  
      else if(isize == -8) fprazor64(in,n/8,out,errlim);
	    #if defined(FLT16_BUILTIN) 
      else if(isize == -2) fprazor16(in,n/2,out,errlim);
	    #endif
	  if(verbose>0) fpstat(in, n/abs(isize), out, isize, cpy); memcpy(in,out,n);   
    } else if(isize < 0 && quantb > 0) {                                        if(verbose>0) printf("Quantization=%d float\n", quantb);
	  if(isize == -4) { 																		
	    float fmin,fmax;
	    if(quantb > 32) quantb = 32;
	    fpquant32e32(in, n/4, out, quantb, &fmin, &fmax);                       if(verbose>0) fpquant32d32(out, n/4, cpy, quantb, fmin, fmax);   
	  } else if(isize == -8) {																	
	    double fmin,fmax;
	    if(quantb > 32) quantb = 32;
	    fpquant64e64(in, n/8, out, quantb, &fmin, &fmax);                       if(verbose>0) fpquant64d64(out,n/8,cpy, quantb, fmin, fmax); 
	  }	  
	    #if defined(FLT16_BUILTIN) 
	  else if(isize == -2) { _Float16 fmin,fmax;												
	    if(quantb > 16) quantb = 16;
	    fpquant16e16(in,n/2,out, quantb, &fmin, &fmax);                         if(verbose>0) { printf("Range=[%g-%g]=%g ", (double)fmin, (double)fmax, (double)fmax - (double)fmin); 
		                                                                          fpquant16d16(out,n/2,cpy, quantb, fmin, fmax); 
														                        }
	  }
	    #endif
	  if(verbose>0) fpstat(in, n/abs(isize), cpy, isize, NULL); memcpy(in, out, n);
	}
    be_mindelta = mindelta(in, n/abs(isize), abs(isize)); 

    if(fi && verbose>1) {
	  unsigned l;                                                                                // Calculate bits distributions
      switch(abs(isize)) {
        case 1: l=histl8( in,n);   stprint("file: max", xbits); if(histz8( in,n  )<l) stprint("file: delta max", zbits); break;
        case 2: l=histl16(in,n/2); stprint("file: max", xbits); if(histz16(in,n/2)<l) stprint("file: delta max", zbits); break;
        case 4: l=histl32(in,n/4); stprint("file: max", xbits); if(histz32(in,n/4)<l) stprint("file: delta max", zbits);
                //l=histt32(in,n/4); stprint("file: ctz", tbits); if(histx32(in,n/4)<l) stprint("file: xor max",   zbits);
		        break;
        case 8: l=histl64(in,n/8); stprint("file: max", xbits); if(histz64(in,n/8)<l) stprint("file: delta max", zbits); break;
      }
      switch(isize) {
        case -2: histt16(in,n/2); stprint("file: ctz", tbits); break;
        case -4: histt32(in,n/4); stprint("file: ctz", tbits); break;
        case -8: histt64(in,n/8); stprint("file: ctz", tbits); break;
      }
    }

    char *p = icmd?icmd:"1-120";
    if(verbose>1 || fno == optind) {
      printf("      size   ratio     E MB/s   D MB/s   function %s size=%d bits (lz=%s,%d%s) ", isize>0?"integer":"floating point", abs(isize)*8, codstr(codid), codlev, codprm);
      if(be_mindelta == (uint64_t)-1) printf("unsorted %lld ", be_mindelta);
      else printf("sorted(mindelta=%lld) ", be_mindelta);
      if(errlim > 0.0) printf("FP err=%f", errlim);
	  //if(nx) { printf("%d",  nx); if(ny) printf("x%d",  ny); if(nz) printf("x%d",  nz); if(nw) printf("x%d",  nw); }
      printf("\n");
    }
	fcnt++;
    do {
      unsigned id = strtoul(p, &p, 10), idx = id, i;
      while(isspace(*p)) p++;
	  if(*p == '-') {
	    if((idx = strtoul(p+1, &p, 10)) < id)
		  idx = id;
		if(id<=ID_MEMCPY && idx > ID_MEMCPY)
		  idx = ID_MEMCPY;
	  }
      for(i = id; i <= idx; i++) {
	    unsigned l = 0;
        switch(abs(isize)) {
          case 1: l = bench8( in, n, out, cpy, i, /*optind+1 == argc?NULL:*/inname, codlev, bsize); break;
          case 2: l = bench16(in, n, out, cpy, i, /*optind+1 == argc?NULL:*/inname, codlev, bsize); break;
          case 4: l = bench32(in, n, out, cpy, i, /*optind+1 == argc?NULL:*/inname, codlev, bsize); break;
          case 8: l = bench64(in, n, out, cpy, i, /*optind+1 == argc?NULL:*/inname, codlev, bsize); /*double *d = in; for(int i=0; i < 100; i++) printf("%.3f ", d[i]);*/break;
          default: die("integer size must be 1, 2, 4 or 8\n");
        }
		if(l > 0) {
		  lens[i].id    = i;
	      lens[i].len  += l;
          lens[i].cnt++;		  
		}
	  }
    } while(*p++);
    free(in); free(out); free(cpy);
	in = out = cpy = NULL;
  }

  qsort(lens, 255, sizeof(len_t), cmpsna);
  printf("Best methods =");
  { unsigned c = 0, l=0;
    for(i = 0; i < 30; i++)
      if(lens[i].len != (uint64_t)-1 && lens[i].cnt == fcnt) {
        c++;
		l = lens[i].len;
		printf("%d,", lens[i].id);
	  }
    if(c == 1 && argc -optind > 1)
	  printf("[%u]", l);
    printf("\n");
  }
}
