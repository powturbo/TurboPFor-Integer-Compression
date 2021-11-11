/**
    Copyright (C) powturbo 2015-2019
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
**/
//  TurboPFor - "IcApp: Benchmark application"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
  #if defined(_WIN32)
#include <windows.h>
#define srand48(x)  srand(x)
#define drand48()   ((double)(rand()) / RAND_MAX)
#define __off64_t   _off64_t
  #endif
#include <math.h> // pow,fabs
#include <float.h>

#include "conf.h"
#include "time_.h"
#define BITUTIL_IN
#include "bitutil.h"

  #if defined(__i386__) || defined(__x86_64__)
#define SSE
#define AVX2
  #elif defined(__ARM_NEON) || defined(__powerpc64__)
#define SSE
  #endif

#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#define max(x,y) (((x)>(y)) ? (x) : (y))
#endif

int verbose = 1,isa;

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

//----------------------------- Convert iso-8601 and similar formats to timestamp -------------------------
// Date separator : '.'. '/' or '-'
// Hour separator : ':'
// Fraction sep.: '.' or ','
// examples: "2020" "20211203" "20211022 11:09:45.1234",
uint64_t strtots(char *p, char **pq, int type) {  // string to timestamp
  struct tm tm;
  uint64_t  u;
  char     *s=p;
  int       frac = 0,c;

  memset(&tm, 0, sizeof(tm)); tm.tm_mday = 1;
  while(!isdigit(*p)) p++;
  u = strtoull(p, &p, 10);                  // first number

  if(     u <= 99) u += 2000;               // year  "yy": 00-99 -> 2000-2099
  else if(u >= 19710101 && u < 20381212) {  // date: "yyyymmdd"
    tm.tm_year =  u/10000;
    tm.tm_mon  = (u%10000)/100; if(!tm.tm_mon  || tm.tm_mon  > 12) goto a; tm.tm_mon--;
    tm.tm_mday = u%10;          if(!tm.tm_mday || tm.tm_mday > 31) goto a;
    goto h;
  } else if(u < 1971 || u > 2099) goto a;   // invalid
  tm.tm_year = u;                           // year       "yyyy"
  c = *p;                                   // month,day: "mm.dd", "mm-dd", "mm/dd"
  if(c != '.' && c != '-' && c != '/') goto b; tm.tm_mon    = strtoul(p+1, &p, 10); if(!tm.tm_mon  || tm.tm_mon  > 12) goto a; tm.tm_mon--;
  if(c != '.' && c != '-' && c != '/') goto b; tm.tm_mday   = strtoul(p+1, &p, 10); if(!tm.tm_mday || tm.tm_mday > 31) goto a;
  if(c != '.' && c != '-' && c != '/') goto b; h:tm.tm_hour = strtoul(p+1, &p, 10);
  if(tm.tm_hour <= 24 && *p == ':') {       // time ":hh:mm:ss.frac", ":hh:mm:ss,frac"
    tm.tm_min = strtoul(p+1, &p, 10); if(tm.tm_min > 60) tm.tm_hour = tm.tm_min = 0;
    tm.tm_sec = strtoul(p+1, &p, 10); if(tm.tm_sec > 60) tm.tm_hour = tm.tm_min = tm.tm_sec = 0;
    if(type > 0 && (*p == '.' || *p == ',')) { frac = strtoul(p+1, &p, 10); if((c=p-(p+1)) > 6) frac /= 1000000;else if(c > 3) frac /= 1000; }
  } else tm.tm_hour = 0;
  b:u = mktime(&tm);
  u = u * 1000 + frac;                      // milliseconds
  a:*pq = p;                                               //if(verbose >= 9) printf("[%d-%d-%d %.2d:%.2d:%.2d.%d]\n", tm.tm_year, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, frac, u);exit(0);
  return u;
}

//----------------------------- Zipfian + Timestamps generator --------------------------------------------------------
static double   a = 1.5;

void stprint(char *s, unsigned *xbits) {
  int i;
  uint64_t t = 0;
  for(i = 0; i <= 64; i++)
    t += xbits[i];
  printf("%s bits histogram:",s);
  if(!t) { printf("ALL zero\n"); return; }
  printf("\n");
  for(i = 0; i <= 64; i++)
    if(xbits[i]) {
      double f = (double)xbits[i]*100/(double)t;
      unsigned u = round(f);
      printf("%.2d:", i); for(int j=0; j < u; j++) printf("#");
      if     (f >  10)    printf(" %.0f%% ", f);
      else if(f > 0.1)    printf(" %.1f%% ", f);
      else if(f > 0.01)   printf(" %.2f%% ", f);
      else if(f > 0.001)  printf(" %.3f%% ", f);
      else if(f > 0.0001) printf(" %.4f%% ", f);
      else if(f > 0.00001) printf(" %.5f%% ", f);
      else                printf(" %.6f%% ", f);
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
  x2 = x2>0xffu?0xffu:x2; if(x1 > x2) x1 = x2;
  unsigned m = x2 - x1 + 1;
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
    a[i] = x1 + l; if(a[i]>x2) a[i]=x2;
  }
  free(zmap);
}

void zipu16(uint16_t *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
  int      i;
  double   prob, cum, *zmap;
  x2 = x2>0xffffu?0xffffu:x2; if(x1 > x2) x1 = x2;
  unsigned m = x2 - x1 + 1;
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
  unsigned char *ip;                                                            printf("zipf alpha=%.2f range[%u..%u].n=%u\n ", a, rm, rx, n);
  int mindelta = be_mindelta,i;  //in = malloc(n*isize+OVD);                    if(!in) die("malloc err=%u", n*isize);
  switch(isize) {
    case 1: zipu8((uint8_t *)in, n, a, rm, rx);                                 //{ int i; for(i = 0; i < n; i++) in[i] = i; }  //
                                                                                for(i = 1; i <= n; i++) xbits[bsr32(ctou8(in+i))]++;
      if(mindelta == 0 || mindelta == 1) {
        uint8_t *ip = (uint8_t *)in, v;                                         stprint("delta", xbits);
        for(ip[0]=0,v = 1; v < n; v++) {
          ip[v] += ip[v-1] + mindelta;                                          if(ip[v]>=(1u<<8)) die("overflow generating sorted array %d\n", ip[v]);
        }
      } else stprint("",xbits);
      break;
    case 2: zipu16((unsigned short *)in, n, a, rm, rx);                                 //{ int i; for(i = 0; i < n; i++) in[i] = i; }  //
                                                                                for(i = 1; i <= n; i++) xbits[bsr32(ctou16(in+i*2))]++;
      if(mindelta == 0 || mindelta == 1) {
        unsigned short *ip = (unsigned short *)in, v;                           stprint("delta", xbits);
        for(ip[0]=0,v = 1; v < n; v++) {
          ip[v] += ip[v-1] + mindelta;                                          if(ip[v]>=(1u<<16)) die("overflow generating sorted array %d\n", ip[v]);
        }
      } else stprint("",xbits);
      break;
    case 4: zipu32((unsigned *)in, n, a, rm, rx);                                   //{ int i; for(i = 0; i < n; i++) in[i] = i; }  //
                                                                                for(i = 1; i <= n; i++) xbits[bsr32(ctou32(in+i*4))]++;
      if(mindelta == 0 || mindelta == 1) {
        unsigned *ip = (unsigned *)in, v;                                       stprint(mindelta?"delta=1":"delta=0", xbits);
        for(ip[0]=0,v = 1; v < n; v++) {
          ip[v] += ip[v-1] + mindelta;                                          if(ip[v]>=(1u<<31)) die("overflow generating sorted array %d\n", ip[v]);
        }
      } else stprint("",xbits);
      break;
    case 8: zipu64((uint64_t *)in, n, a, rm, rx);                                   //{ int i; for(i = 0; i < n; i++) in[i] = i; }  //
                                                                                for(i = 1; i <= n; i++) xbits[bsr64(ctou64(in+i*8))]++;
      if(mindelta == 0 || mindelta == 1) {
        uint64_t *ip = (uint64_t *)in, v;                                       stprint("delta", xbits);
        for(ip[0]=0,v = 1; v < n; v++) {
          ip[v] += ip[v-1] + mindelta;                                          if(ip[v]>=(1u<<31)) die("overflow generating sorted array %lld\n", ip[v]);
        }
      } else stprint("",xbits);
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

int mdelta;
char *keysep;

unsigned befgen(unsigned char **_in, unsigned n, int fmt, int isize, FILE *fi, int kid, int skiph, int decs, int divs, int mdelta) {
  unsigned char *in = *_in,*ip;
  unsigned nmax = 0, ovf=0;
  #define LSIZE (1024*16)
  char s[LSIZE+1];
  double pre;

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
        unsigned char *p = s,*q;
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
          switch(isize) {
            case 1: if(u >       0xffu) ovf++; break;
            case 2: if(u >     0xffffu) ovf++; break;
            case 4: if(u > 0xffffffffu) ovf++; break;
          }
          IPUSH(in,n,isize,nmax,u);                             c=*q;   *q=0; if(verbose>=5 && n < 100 || verbose>=9) printf("\'%s\'->%lld ", p, u); *q = c;
        } else {
          while(*p && !isdigit(*p) && *p != '-' && *p != '.' && *p != '+') {  if(keysep && strchr(keysep,*p)) keyid++; p++; }
          double d = strtod(p, &q) - mdelta;
          uint64_t u;
          memcpy(&u,&d,sizeof(u));
          IPUSH(in,n,-isize,nmax,u);                                          if(verbose>=5 && n < 100 || verbose>=9) { c=*q; *q=0; double d; memcpy(&d,&u,sizeof(d)); printf("\'%s\'->%f  ", p, d); *q = c; }
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
static int mcpy=1, cmp=2;

#define CBUF(_n_) (((size_t)(_n_))*5/3+1024/*1024*/)
int memcheck(unsigned char *in, unsigned n, unsigned char *cpy) {
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
#include "bitpack.h"
#include "vp4.h"
#include "vint.h"
#include "fp.h"
#include "eliasfano.h"
#include "vsimple.h"
#include "transpose.h"
#include "trle.h"
//#include "bic.h"

  #ifdef STREAMVBYTE
#include "streamvbyte/include/streamvbyte.h"
#include "streamvbyte/include/streamvbytedelta.h"
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

  #ifdef MASKEDVBYTE
#undef VARINTDECODE_H_
#include "ext/fastpfor.h"
#include "MaskedVByte/include/varintencode.h"
    #undef VARINTDECODE_H_
#include "MaskedVByte/include/varintdecode.h"
  #endif

  #ifdef BITSHUFFLE
#include "bitshuffle/src/bitshuffle.h"
    #ifndef LZ4
#include "bitshuffle/lz4/lz4.h"
    #endif
  #endif

  #ifdef BLOSC
#include "c-blosc2/blosc/shuffle.h"
#include "c-blosc2/blosc/blosc2.h"
  #endif

  #ifdef VBZ
#define bool int
#include "vbz_compression/vbz/vbz.h"
  #endif

  #ifdef VTENC
#include "VTEnc/vtenc.h"
  #endif


void pr(unsigned l, unsigned n) {
  double r = (double)l*100.0/n;
  if(r>0.1)  printf("%10u %6.2f%%   ", l, r);
  else if(r>0.01) printf("%10u %7.3f%%  ", l, r);
  else printf("%10u %8.4f%% ", l, r); fflush(stdout); }

#define CPYR(in,n,esize,out) memcpy(out+((n)&(~(esize-1))),in+((n)&(~(esize-1))),(n)&(esize-1))  //, out+((n)&(8*esize-1))
/*unsigned char *p4menc32(uint32_t      *in, unsigned n, unsigned char *out, uint32_t start, unsigned char *tmp) {
  uint32_t x, mdelta,*pa = (unsigned *)tmp;
  bitfm32(in, n, &x);                                           // determine minimum value
  mdelta = bitdi32(in, n, x); if(mdelta>(1<<27)) mdelta=1<<27;  // determine minimum delta
  bitdienc32(in, n, pa, x, mdelta);                         // delta transform
  mdelta = mdelta<<5|(x&31); x>>=5; vbxput32(out, x); vbput32(out, mdelta);
  return p4nenc32(pa, n, out);
}*/

#if 0 //def __AVX2__
void ictste32(uint32_t *in, size_t _n, uint32_t *out) { uint32_t *ip,*op,n=_n/4;
  for(ip = in,op=out; ip != in+(n&~(8-1)); ip += 8, op += 8) {
    __m256i vi = _mm256_loadu_si256((__m256i *)ip);
            vi = mm256_zzage_epi32(vi);
    _mm256_storeu_si256((__m256i *)op, vi);
  }
}

void ictstd32(uint32_t *in, size_t _n, uint32_t *out) { uint32_t *ip,*op,n=_n/4;
  for(ip = in,op=out; ip != in+(n&~(8-1)); ip += 8, op += 8) {
    __m256i vi = _mm256_loadu_si256((__m256i *)ip);
            vi = mm256_zzagd_epi32(vi);
    _mm256_storeu_si256((__m256i *)op,vi);
  }
}
#endif

//------------------ TurboVSsimple zigzag------------------------------------------
unsigned char *vszenc8( uint8_t  *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc8( in, n, tmp, 0, 0); return vsenc8( tmp, n, out); }
unsigned char *vszenc16(uint16_t *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc16(in, n, tmp, 0, 0); return vsenc16(tmp, n, out); }
unsigned char *vszenc32(uint32_t *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc32(in, n, tmp, 0, 0); return vsenc32(tmp, n, out); }
unsigned char *vszenc64(uint64_t *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc64(in, n, tmp, 0, 0); return vsenc64(tmp, n, out); }

unsigned char *vszdec8( unsigned char *in, unsigned n, unsigned char *out) { unsigned char *p = vsdec8( in,n,out); bitzdec8( out, n, 0); return p; }
unsigned char *vszdec16(unsigned char *in, unsigned n, unsigned char *out) { unsigned char *p = vsdec16(in,n,out); bitzdec16(out, n, 0); return p; }
unsigned char *vszdec32(unsigned char *in, unsigned n, unsigned char *out) { unsigned char *p = vsdec32(in,n,out); bitzdec32(out, n, 0); return p; }
unsigned char *vszdec64(unsigned char *in, unsigned n, unsigned char *out) { unsigned char *p = vsdec64(in,n,out); bitzdec64(out, n, 0); return p; }

//------------------ TurboRLE (Run Length Encoding) + zigzag/xor -------------------------
#define RLE8  0xdau
#define RLE16 0xdadau
#define RLE32 0xdadadadau
#define RLE64 0xdadadadadadadadaull
unsigned trlezc( uint8_t      *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitzenc8(in, n, tmp, 0, 0); unsigned rc = trlec(tmp, n, out); if(rc >=n) { rc = n; memcpy(out,in,n); } return rc; }
unsigned trlezd(unsigned char *in, unsigned inlen, uint8_t *out, unsigned n) { if(inlen >= n) { memcpy(out,in,n); return inlen; } trled(in, inlen, out, n); bitzdec8(out, n, 0); return n; }

unsigned trlexc( uint8_t      *in, unsigned n, unsigned char *out, unsigned char *tmp) { bitxenc8(in, n, tmp, 0); unsigned rc = trlec(tmp, n, out); if(rc >=n) { rc = n; memcpy(out,in,n); } return rc; }
unsigned trlexd(unsigned char *in, unsigned inlen, uint8_t *out, unsigned n) { if(inlen >= n) { memcpy(out,in,n); return inlen; } trled(in, inlen, out, n); bitxdec8(out, n, 0); return n; }

unsigned srlezc8( uint8_t  *in, unsigned n, unsigned char *out, unsigned char *tmp, uint8_t  e) { bitzenc8( in, n/( 8/8), tmp, 0, 0); return srlec8( tmp, n, out, e); }
unsigned srlezc16(uint16_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint16_t e) { bitzenc16(in, n/(16/8), tmp, 0, 0); return srlec16(tmp, n, out, e); }
unsigned srlezc32(uint32_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint32_t e) { bitzenc32(in, n/(32/8), tmp, 0, 0); return srlec32(tmp, n, out, e); }
unsigned srlezc64(uint64_t *in, unsigned n, unsigned char *out, unsigned char *tmp, uint64_t e) { bitzenc64(in, n/(64/8), tmp, 0, 0); return srlec64(tmp, n, out, e); }

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

void bitxenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case 1 : bitxenc8( in, n/1, out, 0); break;
    case 2 : bitxenc16(in, n/2, out, 0); break;
    case 4 : bitxenc32(in, n/4, out, 0); break;
    case 8 : bitxenc64(in, n/8, out, 0); break;
  }
}

void bitxdec(unsigned char *in, unsigned n, unsigned esize) {
  switch(esize) {
    case 1: bitxdec8( in, n/1, 0);break;
    case 2: bitxdec16(in, n/2, 0);break;
    case 4: bitxdec32(in, n/4, 0);break;
    case 8: bitxdec64(in, n/8, 0);break;
  }
}

void bitzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case 1 : bitzenc8( in, n/1, out, 0, 0); break;
    case 2 : bitzenc16(in, n/2, out, 0, 0); break;
    case 4 : bitzenc32(in, n/4, out, 0, 0); break;
    case 8 : bitzenc64(in, n/8, out, 0, 0); break;
  }
}

void bitzdec(unsigned char *in, unsigned n, unsigned esize) {
  switch(esize) {
    case 1: bitzdec8( in, n/1, 0);break;
    case 2: bitzdec16(in, n/2, 0);break;
    case 4: bitzdec32(in, n/4, 0);break;
    case 8: bitzdec64(in, n/8, 0);break;
  }
}

#if 0
void bitzzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize) {
  switch(esize) {
    case 1 : bitzzenc8( in, n/1, out, 0, 0); break;
    case 2 : bitzzenc16(in, n/2, out, 0, 0); break;
    case 4 : bitzzenc32(in, n/4, out, 0, 0); break;
    case 8 : bitzzenc64(in, n/8, out, 0, 0); break;
  }
}

void bitzzdec(unsigned char *in, unsigned n, unsigned esize) {
  switch(esize) {
    case 1: bitzzdec8( in, n/1, 0);break;
    case 2: bitzzdec16(in, n/2, 0);break;
    case 4: bitzzdec32(in, n/4, 0);break;
    case 8: bitzzdec64(in, n/8, 0);break;
  }
}
#endif

  #ifdef BITSHUFFLE //--------------------------bit transpose ----------------------------------------------------------------------
static void bitshuffle(uint8_t *in, unsigned n, uint8_t *out, unsigned esize) {
  bshuf_bitshuffle(  in, out, n/esize, esize, 0); memcpy((char *)out+(n&(~(8*esize-1))), (char *)in+(n&(~(8*esize-1))), n&(8*esize-1));
}
static void bitunshuffle(uint8_t *in, unsigned n, uint8_t *out, unsigned esize) {
  bshuf_bitunshuffle(in, out, n/esize, esize, 0); memcpy((char *)out+(n&(~(8*esize-1))), (char *)in+(n&(~(8*esize-1))), n&(8*esize-1));
}
  #endif

//----------------------- lz --------------------------------------------------------------------------------------
unsigned codid = 0, codlev = 1, dsize = 0;
unsigned char codprm[256];

#include "lz.c"

//---------------------SPDP (https://userweb.cs.txstate.edu/~burtscher/research/SPDPcompressor/) -----------------------
#ifdef SPDP
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
    l = spdp_compress(codlev, iplen, ibuf, op+4); ctou32(op)=l; op+=4+l; //AC(l <= bsize,"Compress Fatal=%d>%d\n", l, bsize);
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
    "%3d:p4nenc%d         TurboPFor           ",                //1
    "%3d:p4nenc128v%d     TurboPForV          ",
    "%3d:p4nenc256v%d     TurboPFor256        ",
    "%3d:p4ndenc%d        TurboPFor    delta  ",
    "%3d:p4ndenc128v%d    TurboPForV   delta  ",
    "%3d:p4ndenc256v%d    TurboPFor256 delta  ",
    "%3d:p4nd1enc%d       TurboPFor    delta1 ",
    "%3d:p4nd1enc128v%d   TurboPForV   delta1 ",
    "%3d:p4nd1enc256v%d   TurboPFor256 delta1 ",

    "%3d:p4nzenc%d        TurboPFor    zigzag ",        //10
    "%3d:p4nzenc128v%d    TurboPForV   zigzag ",
    "%3d:p4nzenc256v%d    TurboPFor256 zigzag ",
    "%3d:p4nzzenc128v%d   TurboPFor zzag/delta",
    "%3d:bics32           Interpolative Cod. S",
    "%3d:bic32            Interpolative Cod. L",
    "%3d:bicm32           Interpolative Cod. M",
    "%3d:17               ",
    "%3d:18               ",
    "%3d:19               ",

    "%3d:bitnpack%d       TurboPack           ",                //20
    "%3d:bitnpack128v%d   TurboPackV          ",
    "%3d:bitnpack256v%d   TurboPack256        ",
    "%3d:bitndpack%d      TurboPack    delta  ",
    "%3d:bitndpack128v%d  TurboPackV   delta  ",
    "%3d:bitndpack256v%d  TurboPack256 delta  ",
    "%3d:bitnd1pack%d     TurboPack    delta1 ",
    "%3d:bitnd1pack128v%d TurboPackV   delta1 ",
    "%3d:bitnd1pack256v%d TurboPack256 delta1 ",
    "%3d:bitnzpack%d      TurboPack    zigzag ",

    "%3d:bitnzpack128v%d  TurboPackV   zigzag ",        //30
    "%3d:bitnzpack256v%d  TurboPack256 zigzag ",
    "%3d:bitnfpack%d      TurboPack    FOR    ",
    "%3d:bitnfpack128v%d  TurboPackV   FOR    ",
    "%3d:bitnfpack256v%d  TurboPack256 FOR    ",
    "%3d:bitns1pack128v32 TurboPack256 delt4_1",
    "%3d:36               ",
    "%3d:37               ",
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

    "%3d:v8nenc128v%d     TurboByte+TbPackV   ",        //50    //TurboByte Hybrid
    "%3d:v8nzenc128v%d    TByte+TPackV zigzag ",
    "%3d:v8ndenc128v%d    TByte+TPackV delta  ",
    "%3d:v8nd1enc128v%d   TByte+TPackV delta1 ",
    "%3d:54                                   ",
    "%3d:v8nenc256v%d     TurboByte+TbPackV   ",
    "%3d:v8nzenc256v%d    TByte+TPackV zigzag ",
    "%3d:v8ndenc256v%d    TByte+TPackV delta  ",
    "%3d:v8nd1enc256v%d   TByte+TPackV delta1 ",
    "%3d:59                                   ",

    "%3d:bvzzenc%d        bitio zigzag/delta  ",       //60
    "%3d:bvzenc%d         bitio zigzag        ",
    "%3d:fpgenc%d         bitio TurboGorilla  ",
    "%3d:63                                   ",
    "%3d:64                                   ",                                                //"fphenc%d         slope predictor",
    "%3d:fpxenc%d         TurboFloat XOR      ",        //bvzaenc%d        moving average pred.",
    "%3d:fpfcmenc%d       TurboFloat FCM      ",
    "%3d:fpdfcmenc%d      TurboFloat DFCM     ",
    "%3d:fp2dfcmenc%d     TurboFloat DFCM 2D  ",
    "%3d:69                 ",

    "%3d:trle             TurboRLE            ",        //70
    "%3d:trlex            TurboRLE xor        ",
    "%3d:trlez            TurboRLE zigzag     ",
    "%3d:srle%d           TurboRLE ESC        ",
    "%3d:srlex%d          TurboRLE ESC xor    ",
    "%3d:srlez%d          TurboRLE ESC zigzag ",
    "%3d:76                                   ",
    "%3d:77                                   ",
    "%3d:78                                   ",
    "%3d:79                                   ",

    "%3d:Lz               %s,%d               ",        //80
    "%3d:Lztp   Byte      Transpose+%s,%d        ",
    "%3d:Lztpx  Byte      Transpose+xor+%s,%d    ",
    "%3d:Lztpz  Byte      Transpose+zzag+%s,%d   ",
    "%3d:Lztp4  Nibble    Transpose+%s,%d        ",
    "%3d:Lztp4x Nibble    Transpose+xor+%s,%d    ",
    "%3d:Lztp4z Nibble    Transpose+zigzag+%s,%d ",
    "%3d:Lztp1  Bit       Bitshuffle+%s,%d       ",
    "%3d:Lztp1x Bit       Bitshuffle+xor+%s,%d   ",
    "%3d:Lztp1z Bit       Bitshuffle+zigzag+%s,%d",

    "%3d:lztprle          Transpose+rle+%s,%d    ",     //90
    "%3d:lztprlex         Transpose+xor+rle+%s,%d",
    "%3d:lztprlez         Transpose+zzg+rle+%s,%d",
    "%3d:lzv8enc          TurboByte+%s,%d        ",
    "%3d:lzv8xenc         TurboByte+xor+%s,%d    ",
    "%3d:lzv8zenc         TurboByte+zzag+%s,%d   ",
    "%3d:96                                   ",
    "%3d:97                                   ",
    "%3d:98                                   ",
    "%3d:99                                   ",

    "%3d:LztpD2byte       2D Transpose+%s,%d     ",    //100
    "%3d:LztpxD2byte      2D Transpose+xor+%s,%d ",
    "%3d:LztpzD2byte      2D Transpose+zzag+%s,%d",
    "%3d:LztpD3byte       3D Transpose+%s,%d     ",
    "%3d:LztpxD3byte      3D Transpose+xor+%s,%d ",
    "%3d:LztpzD3byte      3D Transpose+zzag+%s,%d",
    "%3d:LztpD4byte       4D Transpose+%s,%d     ",
    "%3d:LztpxD4byte      4D Transpose+xor+%s,%d ",
    "%3d:LztpzD4byte      4D Transpose+zzag+%s,%d",
    "%3d:SPDP             SPDP Floating Point ",

    "%3d:streamvbyte      StreamVByte SIMD    ",    //110
    "%3d:streamvbyte delt StreamVByte delta   ",
    "%3d:streamvbyte zzag StreamVByte zigzag  ",
    "%3d:maskeydvbyte     MasedVByte SIMD     ",
    "%3d:FastPFor         FastPFor            ",
    "%3d:SimdFastPFor     FastPFor SIMD       ",
    "%3d:SimdOptPFor      FastPFor SIMD       ",
    "%3d:tpenc            Byte transpose      ",
    "%3d:tp4enc           Nibble transpose    ",
    "%3d:bitshuffle       Bit transpose       ",

    "%3d:memcpy           memcpy              ",        //120
    "%3d:vtenc            VTEnc lib           ",
    "%3d:vbz              vbz (nanopore)      "
  }; 
  #define CID_BEG  76
  #define CID_END 108
  if(id >= CID_BEG && id <= CID_END)
    sprintf(s,fmt[id], id, prms, prmi); 
  else
    sprintf(s,fmt[id], id, b, prms, prmi);  
  return s;
}

//------------------------ Floating point statistics ------------------------------------------------------------------
//#define BR(b) (double)(((double)(b)*100.0)/(double)(n*abs(s)*8))
#define BR(b) (((b)/8)*100.0/(double)(n*esize))

void fpstat(unsigned char *in, size_t n, unsigned char *out, int s) {
  double             imin  = DBL_MAX, imax  = DBL_MIN, isum  = 0,               //original data (input)         : minimum,maximum,sum
                     eamin = DBL_MAX, eamax = DBL_MIN, easum = 0, easumsqr = 0, //absolute error                    : abs(input-output)
                     ermin = DBL_MAX, ermax = DBL_MIN, ersum = 0, ersumsqr = 0, //relative error                    : abs(input-output)/abs(input)
                     osum  = 0;                                                 //transformed lossy data (output) : sum
  unsigned long long xtb = 0, xlb = 0, tb = 0, lb = 0, elb=0, mtb=0;
  size_t             idn = 0;
  unsigned char      *ip, *op;
  unsigned           esize = s<0?-s:s,t;
  long long          mant = 0,m;
  int                expo = 0,e;

  for(ip = in, op = out; ip < in+n*esize; ip += esize, op += esize)
    switch(s) {
      case -4: isum += ctof32(ip); osum += ctof32(op); break;
      case -8: isum += ctof64(ip); osum += ctof64(op); break;
      case  1: isum += ctou8( ip); osum += ctou8( op); break;
      case  2: isum += ctou16(ip); osum += ctou16(op); break;
      case  4: isum += ctou32(ip); osum += ctou32(op); break;
      case  8: isum += ctou64(ip); osum += ctou64(op); break;
    }
  double iavg = isum/n, oavg = osum/n, isumpavg = 0, osumpavg = 0, iosumpavg = 0; uint64_t start = 0;
  #define EXPO32(u) ((u>>23 &  0xff) - 0x7e )
  #define EXPO64(u) ((u>>52 & 0x7ff) - 0x3fe)
  #define MANT32(u) (u & 0x807fffffu)
  #define MANT64(u) (u & 0x800fffffffffffffull)
  #define U(s) TEMPLATE3(uint, s, _t) u = TEMPLATE2(ctou,s)(op);\
                t = TEMPLATE2(ctz,s)(u);      tb += t; if(u)      lb += TEMPLATE2(clz,s)(u); AC(t<=s,"Fatal t=%d ", t); \
    start ^= u; t = TEMPLATE2(ctz,s)(start); xtb += t; if(start) xlb += TEMPLATE2(clz,s)(start); start = u

  for(ip = in, op = out; ip < in+n*esize; ip += esize, op += esize) {
    double id, od;
    switch(s) {
      case -4: { id = ctof32(ip); od = ctof32(op); U(32); e = EXPO32(u); expo = clz32(zigzagenc32(e-expo))/*-(32-(32-MANTF32-1))*/; elb+=expo; expo = e;
                                                          m = MANT32(u); mant = ctz32(            m^mant)                     ;     mtb+=mant; mant = m;//ctz32(zigzagenc32(m-mant))
                                                         } break;
      case -8: { id = ctof64(ip); od = ctof64(op); U(64); e = EXPO64(u); expo = clz32(zigzagenc32(e-expo))/*-(32-(64-MANTF64-1))*/; elb+=expo; expo = e;
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
       if(s == -4) fb = (double)elb*100/((double)n*8);
  else if(s == -8) fb = (double)elb*100/((double)n*11);

  double mse = easumsqr/n, irange = imax - imin;
  //printf("Leading/Trailing bits [%.2f%%,%.2f%%=%.2f%%]. XOR[%.2f%%,%.2f%%=%.2f%%] Zigzag[%.2f%%,%.2f%%=%.2f%%]\n", BR(lb), BR(tb), BR(lb+tb), BR(xlb), BR(xtb), BR(xlb+xtb), BR(elb), BR(mtb), BR(elb+mtb) );
  printf("Range: [%g - %g] = %g\n", imin, imax, irange);
  //printf("Min error: Absolute = %.12f, Relative = %f, pointwise relative = %f\n", eamin, eamin/irange, eamax/irange, ermax);
  //printf("Avg error: Absolute = %.12f, Relative = %f, pointwise relative = %f\n", easum/idn, (easum/idn)/irange, ersum/idn);
  printf("Max error: Absolute = %g, Relative = %g, pointwise relative = %g\n", eamax, eamax/irange, ermax);
  printf("Peak Signal-to-Noise Ratio: PSNR         = %f\n", 20*log10(irange)-10*log10(mse));
  printf("Normalized Root Mean Square Error: NRMSE = %g\n", sqrt(mse)/irange);
  double std1 = sqrt(isumpavg/n), std2 = sqrt(osumpavg/n), ee = iosumpavg/n, acEff = (iosumpavg/n)/sqrt(isumpavg/n)/sqrt(osumpavg/n);
  printf("Pearson Correlation Coefficient          = %f\n",    (iosumpavg/n)/sqrt(isumpavg/n)/sqrt(osumpavg/n));
}

//---------------------------------------------------------------------------------------
#define MINDELTA(_t_, _in_, _n_, _dmin_) {\
  _t_ *_p = (_t_ *)_in_, _i;\
  for(_dmin_ = (_t_)-1, _i = 1; _i < _n_; _i++)\
    if(_p[_i] < _p[_i-1]) { dmin = (_t_)-1; break; }\
    else { _t_ _d = _p[_i] - _p[_i-1]; if(_d < _dmin_) { _dmin_ = _d; /*printf("[%u]", _dmin_);*/} }\
}

uint8_t  mindelta8( unsigned char *in, unsigned n) { uint8_t  dmin; MINDELTA(uint8_t,  in, n, dmin); return dmin; }
uint16_t mindelta16(unsigned char *in, unsigned n) { uint16_t dmin; MINDELTA(uint16_t, in, n, dmin); return dmin; }
uint32_t mindelta32(unsigned char *in, unsigned n) { uint32_t dmin; MINDELTA(uint32_t, in, n, dmin); return dmin; }
uint64_t mindelta64(unsigned char *in, unsigned n) { uint64_t dmin; MINDELTA(uint64_t, in, n, dmin); return dmin; }
uint64_t mindelta(  unsigned char *in, unsigned n, unsigned siz) {
  switch(siz) {
    case 1: { uint8_t  d = mindelta8( in,n); return d == (uint8_t )-1?(uint64_t)-1:d; }
    case 2: { uint16_t d = mindelta16(in,n); return d == (uint16_t)-1?(uint64_t)-1:d; };
    case 4: { uint32_t d = mindelta32(in,n); return d == (uint32_t)-1?(uint64_t)-1:d; };
    case 8: { uint64_t d = mindelta64(in,n); return d == (uint64_t)-1?(uint64_t)-1:d; };
  }
  return -2;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
#define NEEDTMP (id == 39 || id == 102 || id == 112 || id>=70 && id <= 99)
unsigned dim1,dim2,dim3,dim4;

#define USIZE 1
unsigned bench8(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int codlev) {
  unsigned l=0,m = n/(USIZE), rc = 0, ns = CBUF(n);  uint8_t dmin = mindelta8(in,m);
  uint8_t  *p;
  char     *tmp = NULL;
  if(NEEDTMP && !(tmp = (unsigned char*)malloc(ns))) die(stderr, "malloc error\n");
  memrcpy(cpy,in,n); l = 0;

  switch(id) {
    case  1: TMBENCH("",l=p4nenc8(     in, m, out)  ,n);        pr(l,n); TMBENCH2("  1",p4ndec8(          out, m, cpy)   ,n); break;

    case  4: TMBENCH("",l=p4ndenc8(    in, m, out)  ,n);        pr(l,n); TMBENCH2("  4",p4nddec8(         out, m, cpy)   ,n); break;

    case  7: TMBENCH("",l=p4nd1enc8(   in, m, out)  ,n);        pr(l,n); TMBENCH2("  7",p4nd1dec8(        out, m, cpy)   ,n); break;

    case 10: TMBENCH("",l=p4nzenc8(    in, m, out)  ,n);        pr(l,n); TMBENCH2(" 10",p4nzdec8(         out, m, cpy)   ,n); break;

    case 20: TMBENCH("",l=bitnpack8(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 20",bitnunpack8(      out, m, cpy)   ,n); break;

    case 23: TMBENCH("",l=bitndpack8(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 23",bitndunpack8(     out, m, cpy)   ,n); break;

    case 26: TMBENCH("",l=bitnd1pack8( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 26",bitnd1unpack8(    out, m, cpy)   ,n); break;

    case 29: TMBENCH("",l=bitnzpack8(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 29",bitnzunpack8(     out, m, cpy)   ,n); break;

    case 32: if(dmin==(uint8_t)-1) goto end;
             TMBENCH("",l=bitnfpack8(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 32",bitnfunpack8(     out, m, cpy)   ,n); break;

    case 38: TMBENCH("",l=vsenc8(      in, m, out)-out,n);      pr(l,n); TMBENCH2(" 38",vsdec8(           out, m, cpy) ,n); break;   // vsimple : variable simple
    case 39: TMBENCH("",l=vszenc8(     in, m, out,tmp)-out,n);  pr(l,n); TMBENCH2(" 39",vszdec8(          out, m, cpy) ,n); break;

    //case 40: TMBENCH("",l=vbenc8(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 40",vbdec8(           out, m, cpy) ,n); break; // TurboVbyte : variable byte
    case 41: TMBENCH("",l=vbzenc8(     in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 41",vbzdec8(          out, m, cpy,0) ,n); break;
    //case 42: TMBENCH("",l=vbdenc8(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 42",vbddec8(          out, m, cpy,0) ,n); break;
    //case 43: TMBENCH("",l=vbd1enc8(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 43",vbd1dec8(         out, m, cpy,0) ,n); break;
    //case 44: TMBENCH("",l=vbddenc8(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 44",vbdddec8(         out, m, cpy,0) ,n); break;

    case 60: TMBENCH("",l=bvzzenc8(    in, m, out,0),n);        pr(l,n); TMBENCH2(" 60",bvzzdec8(         out, m, cpy,0) ,n); break; // bitio
    case 61: TMBENCH("",l=bvzenc8(     in, m, out,0),n);        pr(l,n); TMBENCH2(" 61",bvzdec8(          out, m, cpy,0) ,n); break;
    case 62: TMBENCH("",l=fpgenc8(     in, m, out,0),n);        pr(l,n); TMBENCH2(" 62",fpgdec8(          out, m, cpy,0) ,n); break;

    case 65: TMBENCH("",l=fpxenc8(     in, m, out,0),n);        pr(l,n); TMBENCH2(" 65",fpxdec8(          out, m, cpy,0) ,n); break; //Floating point/Integer
    case 66: TMBENCH("",l=fpfcmenc8(   in, m, out,0),n);        pr(l,n); TMBENCH2(" 66",fpfcmdec8(        out, m, cpy,0) ,n); break;
    case 67: TMBENCH("",l=fpdfcmenc8(  in, m, out,0),n);        pr(l,n); TMBENCH2(" 67",fpdfcmdec8(       out, m, cpy,0) ,n); break;
    case 68: TMBENCH("",l=fp2dfcmenc8( in, m, out,0),n);        pr(l,n); TMBENCH2(" 68",fp2dfcmdec8(      out, m, cpy,0) ,n); break;

    case 70: TMBENCH("",l=trlec(       in, n,out),n);           pr(l,n); TMBENCH2(" 70",trled(            out,l,cpy, n),n);      break;  // TurboRLE
    case 71: TMBENCH("",l=trlexc(      in, n,out,tmp),n);       pr(l,n); TMBENCH2(" 71",trlexd(           out,l,cpy, n),n);      break;
    case 72: TMBENCH("",l=trlezc(      in, n,out,tmp),n);       pr(l,n); TMBENCH2(" 72",trlezd(           out,l,cpy, n),n);      break;
    case 73: TMBENCH("",l=srlec8(      in, n,out,RLE8),n);      pr(l,n); TMBENCH2(" 73",srled8(           out,l,cpy, n,RLE8),n);break;
    case 74: TMBENCH("",l=srlexc8(     in, n,out,tmp,RLE8),n);  pr(l,n); TMBENCH2(" 74",srlexd8(          out,l,cpy, n,RLE8),n);break;
    case 75: TMBENCH("",l=srlezc8(     in, n,out,tmp,RLE8),n);  pr(l,n); TMBENCH2(" 75",srlezd8(          out,l,cpy, n,RLE8),n);break;

      #ifdef USE_LZ
    case 80: TMBENCH("",l=lzenc(      in,n,out,ns,codid,codlev,codprm) ,n); pr(l,n); TMBENCH2(" 80",lzdec(out,l,cpy,n,codid,codlev,codprm),n); break;
    case 81: TMBENCH("",l=lztpenc(    in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 81",lztpdec(    out,l,cpy,n,USIZE,tmp) ,n); break;
    case 82: TMBENCH("",l=lztpxenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 82",lztpxdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 83: TMBENCH("",l=lztpzenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 83",lztpzdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 84: TMBENCH("",l=lztp4enc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 84",lztpd4ec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 85: TMBENCH("",l=lztp4xenc(  in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 85",lztp4xdec(  out,l,cpy,n,USIZE,tmp) ,n); break;
    case 86: TMBENCH("",l=lztp4zenc(  in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 86",lztp4zdec(  out,l,cpy,n,USIZE,tmp) ,n); break;
        #ifdef BITSHUFFLE
    case 87: TMBENCH("",l=lztp1enc(   in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 87",lztp1dec(   out,l,cpy,n,USIZE,tmp), n); break;
    case 88: TMBENCH("",l=lztp1xenc(  in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 88",lztp1xdec(  out,l,cpy,n,USIZE,tmp), n); break;
    case 89: TMBENCH("",l=lztp1zenc(  in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 89",lztp1zdec(  out,l,cpy,n,USIZE,tmp), n); break;
        #endif
    case 90: TMBENCH("",l=lztprleenc( in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 90",lztprledec( out,l,cpy,n,USIZE,tmp) ,n); break;
    case 91: TMBENCH("",l=lztprlexenc(in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 91",lztprlexdec(out,l,cpy,n,USIZE,tmp) ,n); break;
    case 92: TMBENCH("",l=lztprlezenc(in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 92",lztprlezdec(out,l,cpy,n,USIZE,tmp) ,n); break;
    case 93: TMBENCH("",l=lzv8enc(    in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 93",lzv8dec(    out,l,cpy,n,USIZE,tmp) ,n); break;
    case 94: TMBENCH("",l=lzv8xenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 94",v8lzxdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 95: TMBENCH("",l=lzv8zenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 95",lzv8zdec(   out,l,cpy,n,USIZE,tmp) ,n); break;

    case 100: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2enc( in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("100",lztpd2dec( out,l,cpy,n,USIZE,tmp, dim1,d2),n);  } break;
    case 101: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2xenc(in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("101",lztpd2xdec(out,l,cpy,n,USIZE,tmp, dim1,d2),n);      } break;
    case 102: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2zenc(in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("102",lztpd2zdec(out,l,cpy,n,USIZE,tmp, dim1,d2),n);      } break;

    case 103: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3enc( in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("103",lztpd3dec( out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 104: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3xenc(in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("104",lztpd3xdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 105: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3zenc(in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("105",lztpd3zdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 106: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4enc( in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("106",lztpd4dec( out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n); } break;
    case 107: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4xenc(in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("107",lztpd4xdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n);} break;
    case 108: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4zenc(in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("108",lztpd4zdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n); } break;
      #endif

        #ifdef CODEC1
    case 109: TMBENCH("",l=spdpenc(in,m*(USIZE),out,SPDPSIZE,codlev),n);pr(l,n); TMBENCH2("109",spdpdec(           out, m*(USIZE), cpy,SPDPSIZE,codlev); ,n); break;
        #endif
    case 117: l = n; TMBENCH("", tpenc( in, n, out,USIZE),n);       pr(l,n); TMBENCH2("117", tpdec( out, n,cpy, USIZE),n); break;
    case 118: l = n; TMBENCH("", tp4enc(in, n, out,USIZE),n);       pr(l,n); TMBENCH2("118", tp4dec(out, n,cpy, USIZE),n); break;
      #ifdef BITSHUFFLE
    case 119: l = n; TMBENCH("", bitshuffle(in, n, out, USIZE),n);  pr(l,n); TMBENCH2("119", bitunshuffle(out, n,cpy, USIZE),n); break;
      #endif
    case ID_MEMCPY: if(!mcpy) goto end;
                     TMBENCH( "", libmemcpy(out,in,n) ,n); l=n;     pr(l,n); pr(n,n); TMBENCH2("120", libmemcpy( cpy,out,n) ,n); break;
    default: goto end;
  }
  if(l) {
    char s[65]; printf("%-35 ", bestr(id, 8, s, codstr(codid), codlev));
    if(cpy) rc = memcheck(in,m*(USIZE),cpy);
    if(!rc)
      printf("\t%s\n", inname?inname:"");
  }
  end:if(tmp) free(tmp);
  return l;
}

#undef USIZE
#define USIZE 2
unsigned bench16(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int codlev) {
  unsigned l=0,m = n/(USIZE),rc = 0, d,ns=CBUF(n);  uint16_t dmin = mindelta16(in,m);
  uint16_t *p;
  char     *tmp = NULL;
  if(NEEDTMP && !(tmp = (unsigned char*)malloc(ns))) die(stderr, "malloc error\n");
  memrcpy(cpy,in,n); l=0;

  switch(id) {
    case  1: TMBENCH("",l=p4nenc16(        in, m, out)  ,n);        pr(l,n); TMBENCH2("  1",p4ndec16(          out, m, cpy)   ,n); break;
      #ifdef SSE
    case  2: TMBENCH("",l=p4nenc128v16(    in, m, out)  ,n);        pr(l,n); TMBENCH2("  2",p4ndec128v16(      out, m, cpy)   ,n); break;
      #endif
    case  4: TMBENCH("",l=p4ndenc16(       in, m, out)  ,n);        pr(l,n); TMBENCH2("  4",p4nddec16(         out, m, cpy)   ,n); break;
      #ifdef SSE
    case  5: TMBENCH("",l=p4ndenc128v16(   in, m, out)  ,n);        pr(l,n); TMBENCH2("  5",p4nddec128v16(     out, m, cpy)   ,n); break;
      #endif
    case  7: TMBENCH("",l=p4nd1enc16(      in, m, out)  ,n);        pr(l,n); TMBENCH2("  7",p4nd1dec16(        out, m, cpy)   ,n); break;
      #ifdef SSE
    case  8: TMBENCH("",l=p4nd1enc128v16(  in, m, out)  ,n);        pr(l,n); TMBENCH2("  8",p4nd1dec128v16(    out, m, cpy)   ,n); break;
      #endif
    case 10: TMBENCH("",l=p4nzenc16(       in, m, out)  ,n);        pr(l,n); TMBENCH2(" 10",p4nzdec16(         out, m, cpy)   ,n); break;
      #ifdef SSE
    case 11: TMBENCH("",l=p4nzenc128v16(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 11",p4nzdec128v16(     out, m, cpy)   ,n); break;
    case 13: TMBENCH("",l=p4nzzenc128v16(  in, m, out,0),n);        pr(l,n); TMBENCH2(" 13",p4nzzdec128v16(    out, m, cpy,0) ,n); break;
      #endif

    case 20: TMBENCH("",l=bitnpack16(      in, m, out)  ,n);        pr(l,n); TMBENCH2(" 20",bitnunpack16(      out, m, cpy)   ,n); break;
      #ifdef SSE
    case 21: TMBENCH("",l=bitnpack128v16(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 21",bitnunpack128v16(  out, m, cpy)   ,n); break;
      #endif
    case 23: TMBENCH("",l=bitndpack16(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 23",bitndunpack16(     out, m, cpy)   ,n); break;
      #ifdef SSE
    case 24: TMBENCH("",l=bitndpack128v16( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 24",bitndunpack128v16( out, m, cpy)   ,n); break;
      #endif
    case 26: TMBENCH("",l=bitnd1pack16(    in, m, out)  ,n);        pr(l,n); TMBENCH2(" 26",bitnd1unpack16(    out, m, cpy)   ,n); break;
      #ifdef SSE
    case 27: TMBENCH("",l=bitnd1pack128v16(in, m, out)  ,n);        pr(l,n); TMBENCH2(" 27",bitnd1unpack128v16(out, m, cpy)   ,n); break;
      #endif
    case 29: TMBENCH("",l=bitnzpack16(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 29",bitnzunpack16(     out, m, cpy)   ,n); break;
      #ifdef SSE
    case 30: TMBENCH("",l=bitnzpack128v16( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 30",bitnzunpack128v16( out, m, cpy)   ,n); break;
      #endif
    case 32: if(dmin==(uint16_t)-1) goto end;
             TMBENCH("",l=bitnfpack16(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 32",bitnfunpack16(     out, m, cpy)   ,n); break;
      #ifdef SSE
    case 33: if(dmin==(uint16_t)-1) goto end;
             TMBENCH("",l=bitnfpack128v16( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 33",bitnfunpack128v16( out, m, cpy)   ,n); break;

             case 35: if(dmin==(uint16_t)-1) goto end;
             TMBENCH("",l=bitns1pack128v16( in, m, out)  ,n);       pr(l,n); TMBENCH2(" 35",bitns1unpack128v16( out, m, cpy)   ,n); break;
      #endif
    //case 35: if(dmin==-1 /*|| !dmin*/) goto end; TMBENCH("",l=efanoenc16(     in, m, out,0)  ,n); pr(l,n); TMBENCH2("efanoenc16       ",efanodec16( out, m, cpy,0)   ,n); break;
    case 38: TMBENCH("",l=vsenc16(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 38",vsdec16(           out, m, cpy) ,n); break;   // vsimple : variable simple
    case 39: TMBENCH("",l=vszenc16(        in, m, out,tmp)-out,n);  pr(l,n); TMBENCH2(" 39",vszdec16(          out, m, cpy) ,n); break;

    case 40: TMBENCH("",l=vbenc16(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 40",vbdec16(           out, m, cpy) ,n); break; // TurboVbyte : variable byte
    case 41: TMBENCH("",l=vbzenc16(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 41",vbzdec16(          out, m, cpy,0) ,n); break;
    case 42: TMBENCH("",l=vbdenc16(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 42",vbddec16(          out, m, cpy,0) ,n); break;
    case 43: TMBENCH("",l=vbd1enc16(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 43",vbd1dec16(         out, m, cpy,0) ,n); break;
    case 44: TMBENCH("",l=vbddenc16(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 44",vbdddec16(         out, m, cpy,0) ,n); break;
    case 45: TMBENCH("",l=v8enc16(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 45",v8dec16(           out, m, cpy) ,n); break; // Turbobyte : Group varint
    case 46: TMBENCH("",l=v8denc16(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 46",v8ddec16(          out, m, cpy,0) ,n); break;
    case 47: TMBENCH("",l=v8d1enc16(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 47",v8d1dec16(         out, m, cpy,0) ,n); break;
    case 48: TMBENCH("",l=v8xenc16(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 48",v8xdec16(          out, m, cpy,0) ,n); break;
    case 49: TMBENCH("",l=v8zenc16(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 49",v8zdec16(          out, m, cpy,0) ,n); break;

      #ifdef SSE
    case 50: TMBENCH("",l=v8nenc128v16(    in, m, out)  ,n);        pr(l,n); TMBENCH2(" 50",v8ndec128v16(      out, m, cpy)   ,n); break;
    case 51: TMBENCH("",l=v8nzenc128v16(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 51",v8nzdec128v16(     out, m, cpy)   ,n); break;
    case 52: TMBENCH("",l=v8ndenc128v16(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 52",v8nddec128v16(     out, m, cpy)   ,n); break;
    case 53: TMBENCH("",l=v8nd1enc128v16(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 53",v8nd1dec128v16(    out, m, cpy)   ,n); break;
      #endif
    case 60: TMBENCH("",l=bvzzenc16(       in, m, out,0),n);        pr(l,n); TMBENCH2(" 60",bvzzdec16(         out, m, cpy,0) ,n); break; // bitio
    case 61: TMBENCH("",l=bvzenc16(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 61",bvzdec16(          out, m, cpy,0) ,n); break;
    case 62: TMBENCH("",l=fpgenc16(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 62",fpgdec16(          out, m, cpy,0) ,n); break;

    case 65: TMBENCH("",l=fpxenc16(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 65",fpxdec16(          out, m, cpy,0) ,n); break; //Floating point/Integer
    case 66: TMBENCH("",l=fpfcmenc16(      in, m, out,0),n);        pr(l,n); TMBENCH2(" 66",fpfcmdec16(        out, m, cpy,0) ,n); break;
    case 67: TMBENCH("",l=fpdfcmenc16(     in, m, out,0),n);        pr(l,n); TMBENCH2(" 67",fpdfcmdec16(       out, m, cpy,0) ,n); break;
    case 68: TMBENCH("",l=fp2dfcmenc16(    in, m, out,0),n);        pr(l,n); TMBENCH2(" 68",fp2dfcmdec16(      out, m, cpy,0) ,n); break;

    case 70: TMBENCH("",l=trlec(           in, n,out),n);           pr(l,n); TMBENCH2(" 70",trled(             out,l,cpy, n),n);      break;  // TurboRLE
    case 71: TMBENCH("",l=trlexc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2(" 71",trlexd(            out,l,cpy, n),n);      break;
    case 72: TMBENCH("",l=trlezc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2(" 72",trlezd(            out,l,cpy, n),n);      break;
    case 73: TMBENCH("",l=srlec16(         in, n,out,RLE16),n);     pr(l,n); TMBENCH2(" 73",srled16(           out,l,cpy, n,RLE16),n);break;
    case 74: TMBENCH("",l=srlexc16(        in, n,out,tmp,RLE16),n); pr(l,n); TMBENCH2(" 74",srlexd16(          out,l,cpy, n,RLE16),n);break;
    case 75: TMBENCH("",l=srlezc16(        in, n,out,tmp,RLE16),n); pr(l,n); TMBENCH2(" 75",srlezd16(          out,l,cpy, n,RLE16),n);break;

      #ifdef USE_LZ
    case 80: TMBENCH("",l=lzenc(      in,n,out,ns,codid,codlev,codprm) ,n); pr(l,n); TMBENCH2(" 80",lzdec(out,l,cpy,n,codid,codlev,codprm),n); break;
    case 81: TMBENCH("",l=lztpenc(    in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 81",lztpdec(    out,l,cpy,n,USIZE,tmp) ,n); break;
    case 82: TMBENCH("",l=lztpxenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 82",lztpxdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 83: TMBENCH("",l=lztpzenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 83",lztpzdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 84: TMBENCH("",l=lztp4enc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 84",lztpd4ec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 85: TMBENCH("",l=lztp4xenc(  in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 85",lztp4xdec(  out,l,cpy,n,USIZE,tmp) ,n); break;
    case 86: TMBENCH("",l=lztp4zenc(  in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 86",lztp4zdec(  out,l,cpy,n,USIZE,tmp) ,n); break;
        #ifdef BITSHUFFLE
    case 87: TMBENCH("",l=lztp1enc(   in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 87",lztp1dec(   out,l,cpy,n,USIZE,tmp), n); break;
    case 88: TMBENCH("",l=lztp1xenc(  in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 88",lztp1xdec(  out,l,cpy,n,USIZE,tmp), n); break;
    case 89: TMBENCH("",l=lztp1zenc(  in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 89",lztp1zdec(  out,l,cpy,n,USIZE,tmp), n); break;
        #endif
    case 90: TMBENCH("",l=lztprleenc( in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 90",lztprledec( out,l,cpy,n,USIZE,tmp) ,n); break;
    case 91: TMBENCH("",l=lztprlexenc(in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 91",lztprlexdec(out,l,cpy,n,USIZE,tmp) ,n); break;
    case 92: TMBENCH("",l=lztprlezenc(in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 92",lztprlezdec(out,l,cpy,n,USIZE,tmp) ,n); break;
    case 93: TMBENCH("",l=lzv8enc(    in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 93",lzv8dec(    out,l,cpy,n,USIZE,tmp) ,n); break;
    case 94: TMBENCH("",l=lzv8xenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 94",v8lzxdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 95: TMBENCH("",l=lzv8zenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 95",lzv8zdec(   out,l,cpy,n,USIZE,tmp) ,n); break;

    case 100: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2enc( in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("100",lztpd2dec( out,l,cpy,n,USIZE,tmp, dim1,d2),n);  } break;
    case 101: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2xenc(in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("101",lztpd2xdec(out,l,cpy,n,USIZE,tmp, dim1,d2),n);      } break;
    case 102: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2zenc(in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("102",lztpd2zdec(out,l,cpy,n,USIZE,tmp, dim1,d2),n);      } break;

    case 103: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3enc( in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("103",lztpd3dec( out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 104: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3xenc(in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("104",lztpd3xdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 105: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3zenc(in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("105",lztpd3zdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 106: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4enc( in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("106",lztpd4dec( out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n); } break;
    case 107: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4xenc(in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("107",lztpd4xdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n);} break;
    case 108: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4zenc(in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("108",lztpd4zdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n); } break;
      #endif

        #ifdef CODEC1
    case 109: TMBENCH("",l=spdpenc(in,m*(USIZE),out,SPDPSIZE,codlev),n);pr(l,n); TMBENCH2("109",spdpdec(           out, m*(USIZE), cpy,SPDPSIZE,codlev); ,n); break;
        #endif
    case 117: l = n; TMBENCH("", tpenc( in, n, out,USIZE),n);       pr(l,n); TMBENCH2("117", tpdec( out, n,cpy, USIZE),n); break;
    case 118: l = n; TMBENCH("", tp4enc(in, n, out,USIZE),n);       pr(l,n); TMBENCH2("118", tp4dec(out, n,cpy, USIZE),n); break;
      #ifdef BITSHUFFLE
    case 119: l = n; TMBENCH("", bitshuffle(in, n, out, USIZE),n);  pr(l,n); TMBENCH2("119", bitunshuffle(out, n,cpy, USIZE),n); break;
      #endif
    case ID_MEMCPY: if(!mcpy) goto end;
                     TMBENCH( "", libmemcpy(out,in,n) ,n); l=n;      pr(l,n); TMBENCH2("120", libmemcpy( cpy,out,n) ,n); break;
      #ifdef VBZ
    case 122: { CompressionOptions opt; opt.perform_delta_zig_zag = 1; opt.integer_size = 2; opt.zstd_compression_level = 22; opt.vbz_version = VBZ_DEFAULT_VERSION;
            TMBENCH("", l = vbz_compress(in, n, out, ns, &opt),n); pr(l,n); TMBENCH2("122", vbz_decompress(out, l, cpy, n, &opt),n); 
      } break;
      #endif
   default: goto end;
  }
  if(l) {
    char s[65]; printf("%-30s ", bestr(id, 16,s, codstr(codid),codlev));
    if(cpy) rc = memcheck(in,m*(USIZE),cpy);
    if(!rc)
      printf("\t%s\n", inname?inname:"");
  }
  end:if(tmp) free(tmp);
  return l;
}

#undef USIZE
#define USIZE 4
unsigned bench32(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int codlev) {
  unsigned l=0,m = n/(USIZE),rc = 0, d,ns=CBUF(n);  
  uint32_t *p;
  char     *tmp = NULL;
  uint32_t dmin = mindelta32(in,m);
  if(NEEDTMP && !(tmp = (unsigned char*)malloc(ns))) die(stderr, "malloc error\n");
  memrcpy(cpy,in,n);

  switch(id) {
    case  1: TMBENCH("",l=p4nenc32(        in, m, out)  ,n);        pr(l,n); TMBENCH2("  1",p4ndec32(          out, m, cpy)   ,n); break;
      #ifdef SSE
    case  2: TMBENCH("",l=p4nenc128v32(    in, m, out)  ,n);        pr(l,n); TMBENCH2("  2",p4ndec128v32(      out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case  3: if(isa>=0x60) { TMBENCH("",l=p4nenc256v32(    in, m, out)  ,n);        pr(l,n); TMBENCH2("  3",p4ndec256v32(      out, m, cpy)   ,n); } break;
      #endif
    case  4: TMBENCH("",l=p4ndenc32(       in, m, out)  ,n);        pr(l,n); TMBENCH2("  4",p4nddec32(         out, m, cpy)   ,n); break;
      #ifdef SSE
    case  5: TMBENCH("",l=p4ndenc128v32(   in, m, out)  ,n);        pr(l,n); TMBENCH2("  5",p4nddec128v32(     out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case  6: if(isa>=0x60) { TMBENCH("",l=p4ndenc256v32( in, m, out)  ,n);          pr(l,n); TMBENCH2("  6", p4nddec256v32( out, m, cpy)   ,n); } break;
      #endif
    case  7: TMBENCH("",l=p4nd1enc32(      in, m, out)  ,n);        pr(l,n); TMBENCH2("  7",p4nd1dec32(        out, m, cpy)   ,n); break;
      #ifdef SSE
    case  8: TMBENCH("",l=p4nd1enc128v32(  in, m, out)  ,n);        pr(l,n); TMBENCH2("  8",p4nd1dec128v32(    out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case  9: if(isa>=0x60) { TMBENCH("",l=p4nd1enc256v32(  in, m, out)  ,n);        pr(l,n); TMBENCH2("  9",p4nd1dec256v32(     out, m, cpy)   ,n); } break;
      #endif

    case 10: TMBENCH("",l=p4nzenc32(       in, m, out)  ,n);        pr(l,n); TMBENCH2(" 10",p4nzdec32(         out, m, cpy)   ,n); break;
      #ifdef SSE
    case 11: TMBENCH("",l=p4nzenc128v32(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 11",p4nzdec128v32(     out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case 12: if(isa>=0x60) { TMBENCH("",l=p4nzenc256v32(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 12",p4nzdec256v32(     out, m, cpy)   ,n); } break;
      #endif
      #ifdef SSE
    case 13: TMBENCH("",l=p4nzzenc128v32(  in, m, out,0),n);        pr(l,n); TMBENCH2(" 13",p4nzzdec128v32(    out, m, cpy,0) ,n); break;
      #endif
  //case 13: TMBENCH("",l=p4nsenc32(       in, m, out)  ,n);        pr(l,n); TMBENCH2("",p4nsdec32(         out, m, cpy)   ,n); break;
  //  case 14: if(dmin!=(uint32_t)-1 && dmin > 0) { TMBENCH("",l=bicsenc32(     in, m, out)  ,n);          pr(l,n); TMBENCH2(" 14",bicsdec32(out, m, cpy)   ,n); } break;
  //  case 15: if(dmin!=(uint32_t)-1 && dmin > 0) { TMBENCH("",l=bicenc32(      in, m, out)  ,n);          pr(l,n); TMBENCH2(" 15",bicdec32(out, m, cpy)   ,n); } break;
  //  case 16: if(dmin!=(uint32_t)-1 && dmin > 0) { TMBENCH("",l=bicmenc32(     in, m, out)  ,n);          pr(l,n); TMBENCH2(" 16",bicmdec32(out, m, cpy)   ,n); }break;

    case 20: TMBENCH("",l=bitnpack32(      in, m, out)  ,n);        pr(l,n); TMBENCH2(" 20",bitnunpack32(      out, m, cpy)   ,n); break;
      #ifdef SSE
    case 21: TMBENCH("",l=bitnpack128v32(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 21",bitnunpack128v32(  out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case 22: if(isa>=0x60) { TMBENCH("",l=bitnpack256v32(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 22",bitnunpack256v32(  out, m, cpy)   ,n); } break;
      #endif
    case 23: TMBENCH("",l=bitndpack32(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 23",bitndunpack32(     out, m, cpy)   ,n); break;
      #ifdef SSE
    case 24: TMBENCH("",l=bitndpack128v32( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 24",bitndunpack128v32( out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case 25: if(isa>=0x60) { TMBENCH("",l=bitndpack256v32( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 25",bitndunpack256v32( out, m, cpy)   ,n); } break;
      #endif
    case 26: TMBENCH("",l=bitnd1pack32(    in, m, out)  ,n);        pr(l,n); TMBENCH2(" 26",bitnd1unpack32(    out, m, cpy)   ,n); break;
      #ifdef SSE
    case 27: TMBENCH("",l=bitnd1pack128v32(in, m, out)  ,n);        pr(l,n); TMBENCH2(" 27",bitnd1unpack128v32(out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case 28: if(isa>=0x60) { TMBENCH("",l=bitnd1pack256v32(in, m, out)  ,n);        pr(l,n); TMBENCH2(" 28",bitnd1unpack256v32(out, m, cpy)   ,n); } break;
      #endif
    case 29: TMBENCH("",l=bitnzpack32(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 29",bitnzunpack32(     out, m, cpy)   ,n); break;
      #ifdef SSE
    case 30: TMBENCH("",l=bitnzpack128v32( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 30",bitnzunpack128v32( out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case 31: if(isa>=0x60) { TMBENCH("",l=bitnzpack256v32( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 31",bitnzunpack256v32( out, m, cpy)   ,n); } break;
      #endif
    case 32: if(dmin==(uint32_t)-1) goto end; printf("DMIN=%d ", dmin);
             TMBENCH("",l=bitnfpack32(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 32",bitnfunpack32(     out, m, cpy)   ,n); break;
      #ifdef SSE
    case 33: if(dmin==(uint32_t)-1) goto end;
             TMBENCH("",l=bitnfpack128v32( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 33",bitnfunpack128v32( out, m, cpy)   ,n); break;
      #endif
      #ifdef AVX2
    case 34: if(dmin==(uint32_t)-1) goto end;
             if(isa>=0x60) { TMBENCH("",l=bitnfpack256v32( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 34",bitnfunpack256v32( out, m, cpy)   ,n);} break;
      #endif
      #ifdef SSE
    case 35: if(dmin==(uint32_t)-1) goto end;
             TMBENCH("",l=bitns1pack128v32( in, m, out)  ,n);       pr(l,n); TMBENCH2(" 35",bitns1unpack128v32( out, m, cpy)   ,n); break;
      #endif
   //case 35: if(dmin==-1 /*|| !dmin*/) goto end;  TMBENCH("",l=efanoenc32(     in, m, out,0)  ,n); pr(l,n); TMBENCH2("efanoenc32       ",efanodec32( out, m, cpy,0)   ,n); break;
    case 38: TMBENCH("",l=vsenc32(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 38",vsdec32(           out, m, cpy) ,n); break;   // vsimple : variable simple
    case 39: TMBENCH("",l=vszenc32(        in, m, out,tmp)-out,n);  pr(l,n); TMBENCH2(" 39",vszdec32(          out, m, cpy) ,n); break;

    case 40: TMBENCH("",l=vbenc32(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 40",vbdec32(           out, m, cpy) ,n); break; // TurboVbyte : variable byte
    case 41: TMBENCH("",l=vbzenc32(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 41",vbzdec32(          out, m, cpy,0) ,n); break;
    case 42: TMBENCH("",l=vbdenc32(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 42",vbddec32(          out, m, cpy,0) ,n); break;
    case 43: TMBENCH("",l=vbd1enc32(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 43",vbd1dec32(         out, m, cpy,0) ,n); break;
    case 44: TMBENCH("",l=vbddenc32(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 44",vbdddec32(         out, m, cpy,0) ,n); break;
    case 45: TMBENCH("",l=v8enc32(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 45",v8dec32(           out, m, cpy) ,n); break; // Turbobyte : Group varint
    case 46: TMBENCH("",l=v8denc32(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 46",v8ddec32(          out, m, cpy,0) ,n); break;
    case 47: TMBENCH("",l=v8d1enc32(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 47",v8d1dec32(         out, m, cpy,0) ,n); break;
    case 48: TMBENCH("",l=v8xenc32(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 48",v8xdec32(          out, m, cpy,0) ,n); break;
    case 49: TMBENCH("",l=v8zenc32(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 49",v8zdec32(          out, m, cpy,0) ,n); break;

      #ifdef SSE
    case 50: TMBENCH("",l=v8nenc128v32(    in, m, out)  ,n);        pr(l,n); TMBENCH2(" 50",v8ndec128v32(      out, m, cpy)   ,n); break;
    case 51: TMBENCH("",l=v8nzenc128v32(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 51",v8nzdec128v32(     out, m, cpy)   ,n); break;
    case 52: TMBENCH("",l=v8ndenc128v32(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 52",v8nddec128v32(     out, m, cpy)   ,n); break;
    case 53: TMBENCH("",l=v8nd1enc128v32(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 53",v8nd1dec128v32(    out, m, cpy)   ,n); break;
      #endif

      #ifdef AVX2
    case 55: if(isa>=0x60) { TMBENCH("",l=v8nenc256v32(    in, m, out)  ,n);        pr(l,n); TMBENCH2(" 55",v8ndec256v32(      out, m, cpy)   ,n);} break;
    case 56: if(isa>=0x60) { TMBENCH("",l=v8nzenc256v32(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 56",v8nzdec256v32(     out, m, cpy)   ,n);} break;
    case 57: if(isa>=0x60) { TMBENCH("",l=v8ndenc256v32(   in, m, out)  ,n);        pr(l,n); TMBENCH2(" 57",v8nddec256v32(     out, m, cpy)   ,n);} break;
    case 58: if(isa>=0x60) { TMBENCH("",l=v8nd1enc256v32(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 58",v8nd1dec256v32(    out, m, cpy)   ,n);} break;
      #endif

    case 60: TMBENCH("",l=bvzzenc32(       in, m, out,0),n);        pr(l,n); TMBENCH2(" 60",bvzzdec32(         out, m, cpy,0) ,n); break; // bitio
    case 61: TMBENCH("",l=bvzenc32(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 61",bvzdec32(          out, m, cpy,0) ,n); break;
    case 62: TMBENCH("",l=fpgenc32(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 62",fpgdec32(          out, m, cpy,0) ,n); break;

    case 65: TMBENCH("",l=fpxenc32(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 65",fpxdec32(          out, m, cpy,0) ,n); break; //Floating point/Integer
    case 66: TMBENCH("",l=fpfcmenc32(      in, m, out,0),n);        pr(l,n); TMBENCH2(" 66",fpfcmdec32(        out, m, cpy,0) ,n); break;
    case 67: TMBENCH("",l=fpdfcmenc32(     in, m, out,0),n);        pr(l,n); TMBENCH2(" 67",fpdfcmdec32(       out, m, cpy,0) ,n); break;
    case 68: TMBENCH("",l=fp2dfcmenc32(    in, m, out,0),n);        pr(l,n); TMBENCH2(" 68",fp2dfcmdec32(      out, m, cpy,0) ,n); break;

    case 70: TMBENCH("",l=trlec(           in, n,out),n);           pr(l,n); TMBENCH2(" 70",trled(             out,l,cpy, n),n);      break;  // TurboRLE
    case 71: TMBENCH("",l=trlexc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2(" 71",trlexd(            out,l,cpy, n),n);      break;
    case 72: TMBENCH("",l=trlezc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2(" 72",trlezd(            out,l,cpy, n),n);      break;
    case 73: TMBENCH("",l=srlec32(         in, n,out,RLE32),n);     pr(l,n); TMBENCH2(" 73",srled32(           out,l,cpy, n,RLE32),n);break;
    case 74: TMBENCH("",l=srlexc32(        in, n,out,tmp,RLE32),n); pr(l,n); TMBENCH2(" 74",srlexd32(          out,l,cpy, n,RLE32),n);break;
    case 75: TMBENCH("",l=srlezc32(        in, n,out,tmp,RLE32),n); pr(l,n); TMBENCH2(" 75",srlezd32(          out,l,cpy, n,RLE32),n);break;

      #ifdef USE_LZ
    case 80: TMBENCH("",l=lzenc(      in,n,out,ns,codid,codlev,codprm) ,n); pr(l,n); TMBENCH2(" 80",lzdec(out,l,cpy,n,codid,codlev,codprm),n); break;
    case 81: TMBENCH("",l=lztpenc(    in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 81",lztpdec(    out,l,cpy,n,USIZE,tmp) ,n); break;
    case 82: TMBENCH("",l=lztpxenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 82",lztpxdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 83: TMBENCH("",l=lztpzenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 83",lztpzdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 84: TMBENCH("",l=lztp4enc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 84",lztpd4ec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 85: TMBENCH("",l=lztp4xenc(  in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 85",lztp4xdec(  out,l,cpy,n,USIZE,tmp) ,n); break;
    case 86: TMBENCH("",l=lztp4zenc(  in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 86",lztp4zdec(  out,l,cpy,n,USIZE,tmp) ,n); break;
        #ifdef BITSHUFFLE
    case 87: TMBENCH("",l=lztp1enc(   in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 87",lztp1dec(   out,l,cpy,n,USIZE,tmp), n); break;
    case 88: TMBENCH("",l=lztp1xenc(  in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 88",lztp1xdec(  out,l,cpy,n,USIZE,tmp), n); break;
    case 89: TMBENCH("",l=lztp1zenc(  in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 89",lztp1zdec(  out,l,cpy,n,USIZE,tmp), n); break;
        #endif
    case 90: TMBENCH("",l=lztprleenc( in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 90",lztprledec( out,l,cpy,n,USIZE,tmp) ,n); break;
    case 91: TMBENCH("",l=lztprlexenc(in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 91",lztprlexdec(out,l,cpy,n,USIZE,tmp) ,n); break;
    case 92: TMBENCH("",l=lztprlezenc(in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 92",lztprlezdec(out,l,cpy,n,USIZE,tmp) ,n); break;
    case 93: TMBENCH("",l=lzv8enc(    in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 93",lzv8dec(    out,l,cpy,n,USIZE,tmp) ,n); break;
    case 94: TMBENCH("",l=lzv8xenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 94",v8lzxdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 95: TMBENCH("",l=lzv8zenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 95",lzv8zdec(   out,l,cpy,n,USIZE,tmp) ,n); break;

    case 100: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2enc( in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("100",lztpd2dec( out,l,cpy,n,USIZE,tmp, dim1,d2),n);  } break;
    case 101: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2xenc(in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("101",lztpd2xdec(out,l,cpy,n,USIZE,tmp, dim1,d2),n);      } break;
    case 102: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2zenc(in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("102",lztpd2zdec(out,l,cpy,n,USIZE,tmp, dim1,d2),n);      } break;

    case 103: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3enc( in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("103",lztpd3dec( out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 104: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3xenc(in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("104",lztpd3xdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 105: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3zenc(in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("105",lztpd3zdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 106: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4enc( in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("106",lztpd4dec( out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n); } break;
    case 107: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4xenc(in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("107",lztpd4xdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n);} break;
    case 108: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4zenc(in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("108",lztpd4zdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n); } break;
      #endif

        #ifdef CODEC1
    case 109: TMBENCH("",l=spdpenc(in,m*(USIZE),out,SPDPSIZE,codlev),n);pr(l,n); TMBENCH2("109",spdpdec(           out, m*(USIZE), cpy,SPDPSIZE,codlev); ,n); break;
    case 110: TMBENCH("",l=streamvbyte_encode(in, m, out),n);           pr(l,n); TMBENCH2("110", streamvbyte_decode(       out, cpy, m),n); break;
    case 111: TMBENCH("",l=streamvbyte_delta_encode(in,m,out,0),n);     pr(l,n); TMBENCH2("111", streamvbyte_delta_decode( out, cpy, m,0),n); break;
    case 112: TMBENCH("",l=streamvbyte_zzag_encode( in,m,out,0,tmp),n); pr(l,n); TMBENCH2("112", streamvbyte_zzag_decode(  out, cpy, m,0,tmp),n); break;
        #endif
        #if defined(CODEC2) && defined(__SSE4_1__)
    case 113: TMBENCH("",l=vbyte_encode(in, m, out),n);                 pr(l,n); TMBENCH2("113", masked_vbyte_decode(out, cpy, m),n); break;
    case 114: TMBENCH("",l=FastPFore32(    in, m, out,ns),n);           pr(l,n); TMBENCH2("114", FastPFord32(    out, m, cpy),n); break;
    case 115: TMBENCH("",l=FastPFore128v32(in, m, out,ns),n);           pr(l,n); TMBENCH2("115", FastPFord128v32(out, m, cpy),n); break;
    case 116: TMBENCH("",l=OptPFore128v32( in, m, out,ns),n);           pr(l,n); TMBENCH2("116", OptPFord128v32( out, m, cpy),n); break;
        #endif
    case 117: l = n; TMBENCH("", tpenc( in, n, out,USIZE),n);       pr(l,n); TMBENCH2("117", tpdec( out, n,cpy, USIZE),n); break;
    case 118: l = n; TMBENCH("", tp4enc(in, n, out,USIZE),n);       pr(l,n); TMBENCH2("118", tp4dec(out, n,cpy, USIZE),n); break;
      #ifdef BITSHUFFLE
    case 119: l = n; TMBENCH("", bitshuffle(in, n, out, USIZE),n);  pr(l,n); TMBENCH2("119", bitunshuffle(out, n,cpy, USIZE),n); break;
      #endif
    case ID_MEMCPY: if(!mcpy) goto end;
                     TMBENCH( "", libmemcpy(out,in,n) ,n); l=n;     pr(l,n); TMBENCH2("120", libmemcpy( cpy,out,n) ,n); break;
    //case 121: if(dmin == (uint32_t)-1) goto end;
    // { size_t _l; TMBENCH("",vtenc_list_encode_u32(in, m, out,ns,&_l),n); l = _l; pr(l,n); TMBENCH2("104", vtenc_list_decode_u32(out, _l, cpy, m),n); } break;
    //case 112: TMBENCH("",fppad32(in, m, out,errlim),n);              pr(n,n); TMBENCH2("fppad32      ", fppad32(in, m, out,errlim),n); break;
    default: goto end;
  }
  if(l) {
    char s[128];AC(codstr(codid), "Fatal"); printf("%-40s ", bestr(id,32,s, codstr(codid),codlev));
    if(cpy) rc = memcheck32(in,m,cpy);
    if(!rc)
      printf("\t%s\n", inname?inname:"");
  }
  end:if(tmp) free(tmp);
  return l;
}

#undef USIZE
#define USIZE 8
unsigned bench64(unsigned char *in, unsigned n, unsigned char *out, unsigned char *cpy, int id, char *inname, int codlev) {
  unsigned l,m = n/(USIZE),rc = 0, d,ns=CBUF(n); uint64_t dmin = mindelta64(in,m);
  uint64_t *p;
  char     *tmp = NULL;
  if(NEEDTMP && !(tmp = (unsigned char*)malloc(ns))) die(stderr, "malloc error\n");
  memrcpy(cpy,in,n);

  switch(id) {
    case  1: TMBENCH("",l=p4nenc64(        in, m, out)  ,n);        pr(l,n); TMBENCH2("  1",p4ndec64(          out, m, cpy)   ,n); break;
      #ifdef SSE
    case  2: TMBENCH("",l=p4nenc128v64(    in, m, out)  ,n);        pr(l,n); TMBENCH2("  2",p4ndec128v64(      out, m, cpy)   ,n); break;
      #endif
    case  4: TMBENCH("",l=p4ndenc64(       in, m, out)  ,n);        pr(l,n); TMBENCH2("  4",p4nddec64(         out, m, cpy)   ,n); break;
    //case  5: TMBENCH("",l=p4ndenc128v64(   in, m, out)  ,n);      pr(l,n); TMBENCH2("  5",p4nddec128v64(     out, m, cpy)   ,n); break;

    case  7: TMBENCH("",l=p4nd1enc64(      in, m, out)  ,n);        pr(l,n); TMBENCH2("  7",p4nd1dec64(        out, m, cpy)   ,n); break;
    //case  8: TMBENCH("",l=p4nd1enc128v64(  in, m, out)  ,n);      pr(l,n); TMBENCH2("  8",p4nd1dec128v64(    out, m, cpy)   ,n); break;

    case 10: TMBENCH("",l=p4nzenc64(       in, m, out)  ,n);        pr(l,n); TMBENCH2(" 10",p4nzdec64(         out, m, cpy)   ,n); break;
    //case 11: TMBENCH("",l=p4nzenc128v64(   in, m, out)  ,n);      pr(l,n); TMBENCH2(" 11",p4nzdec128v64(     out, m, cpy)   ,n); break;

      #ifdef SSE
    case 13: TMBENCH("",l=p4nzzenc128v64(  in, m, out,0),n);        pr(l,n); TMBENCH2(" 13",p4nzzdec128v64(    out, m, cpy,0) ,n); break;
      #endif

    case 20: TMBENCH("",l=bitnpack64(      in, m, out)  ,n);        pr(l,n); TMBENCH2(" 20",bitnunpack64(      out, m, cpy)   ,n); break;
      #ifdef SSE
    case 21: TMBENCH("",l=bitnpack128v64(  in, m, out)  ,n);        pr(l,n); TMBENCH2(" 21",bitnunpack128v64(  out, m, cpy)   ,n); break;
      #endif

    case 23: TMBENCH("",l=bitndpack64(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 23",bitndunpack64(     out, m, cpy)   ,n); break;
    //case 24: TMBENCH("",l=bitndpack128v64( in, m, out)  ,n);      pr(l,n); TMBENCH2(" 24",bitndunpack128v64( out, m, cpy)   ,n); break;

    case 26: TMBENCH("",l=bitnd1pack64(    in, m, out)  ,n);        pr(l,n); TMBENCH2(" 26",bitnd1unpack64(    out, m, cpy)   ,n); break;
    //case 27: TMBENCH("",l=bitnd1pack128v64(in, m, out)  ,n);      pr(l,n); TMBENCH2(" 27",bitnd1unpack128v64(out, m, cpy)   ,n); break;

    case 29: TMBENCH("",l=bitnzpack64(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 29",bitnzunpack64(     out, m, cpy)   ,n); break;
    //case 30: TMBENCH("",l=bitnzpack128v64( in, m, out)  ,n);      pr(l,n); TMBENCH2(" 30",bitnzunpack128v64( out, m, cpy)   ,n); break;

    case 32: if(dmin==(uint64_t)-1) goto end;
             TMBENCH("",l=bitnfpack64(     in, m, out)  ,n);        pr(l,n); TMBENCH2(" 32",bitnfunpack64(     out, m, cpy)   ,n); break;
    //case 33: if(dmin==-1) goto end;
    //         TMBENCH("",l=bitnfpack128v64( in, m, out)  ,n);        pr(l,n); TMBENCH2(" 33",bitnfunpack128v64( out, m, cpy)   ,n); break;

    //case 35: if(dmin==-1) goto end;
    //         TMBENCH("",l=bitns1pack128v64( in, m, out)  ,n);     pr(l,n); TMBENCH2(" 35",bitns1unpack128v64( out, m, cpy)   ,n); break;
    //case 35: if(dmin==-1 /*|| !dmin*/) goto end;  TMBENCH("",l=efanoenc64(     in, m, out,0)  ,n); pr(l,n); TMBENCH2("efanoenc64       ",efanodec64( out, m, cpy,0)   ,n); break;
    case 38: TMBENCH("",l=vsenc64(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 38",vsdec64(           out, m, cpy) ,n); break;   // vsimple : variable simple
    case 39: TMBENCH("",l=vszenc64(        in, m, out,tmp)-out,n);  pr(l,n); TMBENCH2(" 39",vszdec64(          out, m, cpy) ,n); break;

    case 40: TMBENCH("",l=vbenc64(         in, m, out)-out,n);      pr(l,n); TMBENCH2(" 40",vbdec64(           out, m, cpy) ,n); break; // TurboVbyte : variable byte
    case 41: TMBENCH("",l=vbzenc64(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 41",vbzdec64(          out, m, cpy,0) ,n); break;
    case 42: TMBENCH("",l=vbdenc64(        in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 42",vbddec64(          out, m, cpy,0) ,n); break;
    case 43: TMBENCH("",l=vbd1enc64(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 43",vbd1dec64(         out, m, cpy,0) ,n); break;
    case 44: TMBENCH("",l=vbddenc64(       in, m, out,0)-out,n);    pr(l,n); TMBENCH2(" 44",vbdddec64(         out, m, cpy,0) ,n); break;

    case 60: TMBENCH("",l=bvzzenc64(       in, m, out,0),n);        pr(l,n); TMBENCH2(" 60",bvzzdec64(         out, m, cpy,0) ,n); break; // bitio
    case 61: TMBENCH("",l=bvzenc64(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 61",bvzdec64(          out, m, cpy,0) ,n); break;
    case 62: TMBENCH("",l=fpgenc64(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 62",fpgdec64(          out, m, cpy,0) ,n); break;

    case 65: TMBENCH("",l=fpxenc64(        in, m, out,0),n);        pr(l,n); TMBENCH2(" 65",fpxdec64(          out, m, cpy,0) ,n); break; //Floating point/Integer
    case 66: TMBENCH("",l=fpfcmenc64(      in, m, out,0),n);        pr(l,n); TMBENCH2(" 66",fpfcmdec64(        out, m, cpy,0) ,n); break;
    case 67: TMBENCH("",l=fpdfcmenc64(     in, m, out,0),n);        pr(l,n); TMBENCH2(" 67",fpdfcmdec64(       out, m, cpy,0) ,n); break;
    case 68: TMBENCH("",l=fp2dfcmenc64(    in, m, out,0),n);        pr(l,n); TMBENCH2(" 68",fp2dfcmdec64(      out, m, cpy,0) ,n); break;

    case 70: TMBENCH("",l=trlec(           in, n,out),n);           pr(l,n); TMBENCH2(" 70",trled(             out,l,cpy, n),n);      break;  // TurboRLE
    case 71: TMBENCH("",l=trlexc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2(" 71",trlexd(            out,l,cpy, n),n);      break;
    case 72: TMBENCH("",l=trlezc(          in, n,out,tmp),n);       pr(l,n); TMBENCH2(" 72",trlezd(            out,l,cpy, n),n);      break;
    case 73: TMBENCH("",l=srlec64(         in, n,out,RLE64),n);     pr(l,n); TMBENCH2(" 73",srled64(           out,l,cpy, n,RLE64),n);break;
    case 74: TMBENCH("",l=srlexc64(        in, n,out,tmp,RLE64),n); pr(l,n); TMBENCH2(" 74",srlexd64(          out,l,cpy, n,RLE64),n);break;
    case 75: TMBENCH("",l=srlezc64(        in, n,out,tmp,RLE64),n); pr(l,n); TMBENCH2(" 75",srlezd64(          out,l,cpy, n,RLE64),n);break;

      #ifdef USE_LZ
    case 80: TMBENCH("",l=lzenc(      in,n,out,ns,codid,codlev,codprm) ,n); pr(l,n); TMBENCH2(" 80",lzdec(out,l,cpy,n,codid,codlev,codprm),n); break;
    case 81: TMBENCH("",l=lztpenc(    in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 81",lztpdec(    out,l,cpy,n,USIZE,tmp) ,n); break;
    case 82: TMBENCH("",l=lztpxenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 82",lztpxdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 83: TMBENCH("",l=lztpzenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 83",lztpzdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 84: TMBENCH("",l=lztp4enc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 84",lztpd4ec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 85: TMBENCH("",l=lztp4xenc(  in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 85",lztp4xdec(  out,l,cpy,n,USIZE,tmp) ,n); break;
    case 86: TMBENCH("",l=lztp4zenc(  in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 86",lztp4zdec(  out,l,cpy,n,USIZE,tmp) ,n); break;
        #ifdef BITSHUFFLE
    case 87: TMBENCH("",l=lztp1enc(   in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 87",lztp1dec(   out,l,cpy,n,USIZE,tmp), n); break;
    case 88: TMBENCH("",l=lztp1xenc(  in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 88",lztp1xdec(  out,l,cpy,n,USIZE,tmp), n); break;
    case 89: TMBENCH("",l=lztp1zenc(  in,n,out,ns,USIZE,tmp), n); pr(l,n); TMBENCH2(" 89",lztp1zdec(  out,l,cpy,n,USIZE,tmp), n); break;
        #endif
    case 90: TMBENCH("",l=lztprleenc( in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 90",lztprledec( out,l,cpy,n,USIZE,tmp) ,n); break;
    case 91: TMBENCH("",l=lztprlexenc(in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 91",lztprlexdec(out,l,cpy,n,USIZE,tmp) ,n); break;
    case 92: TMBENCH("",l=lztprlezenc(in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 92",lztprlezdec(out,l,cpy,n,USIZE,tmp) ,n); break;
    case 93: TMBENCH("",l=lzv8enc(    in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 93",lzv8dec(    out,l,cpy,n,USIZE,tmp) ,n); break;
    case 94: TMBENCH("",l=lzv8xenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 94",v8lzxdec(   out,l,cpy,n,USIZE,tmp) ,n); break;
    case 95: TMBENCH("",l=lzv8zenc(   in,n,out,ns,USIZE,tmp) ,n); pr(l,n); TMBENCH2(" 95",lzv8zdec(   out,l,cpy,n,USIZE,tmp) ,n); break;

    case 100: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2enc( in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("100",lztpd2dec( out,l,cpy,n,USIZE,tmp, dim1,d2),n);  } break;
    case 101: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2xenc(in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("101",lztpd2xdec(out,l,cpy,n,USIZE,tmp, dim1,d2),n);      } break;
    case 102: if(dim2<=0) goto end; { unsigned d2 = dim2*(dim3?dim3:1)*(dim4?dim4:1);
      TMBENCH("",l=lztpd2zenc(in,n,out,ns,USIZE,tmp, dim1,d2),n);           pr(l,n); TMBENCH2("102",lztpd2zdec(out,l,cpy,n,USIZE,tmp, dim1,d2),n);      } break;

    case 103: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3enc( in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("103",lztpd3dec( out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 104: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3xenc(in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("104",lztpd3xdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 105: if(dim3<=0) goto end; { unsigned d3 = dim3*(dim4?dim4:1);
      TMBENCH("",l=lztpd3zenc(in,n,out,ns,USIZE,tmp, dim1,dim2,d3),n);      pr(l,n); TMBENCH2("105",lztpd3zdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,d3),n); } break;
    case 106: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4enc( in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("106",lztpd4dec( out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n); } break;
    case 107: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4xenc(in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("107",lztpd4xdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n);} break;
    case 108: if(dim4<=0) goto end; {
      TMBENCH("",l=lztpd4zenc(in,n,out,ns,USIZE,tmp,dim1,dim2,dim3,dim4),n);pr(l,n); TMBENCH2("108",lztpd4zdec(out,l,cpy,n,USIZE,tmp, dim1,dim2,dim3,dim4),n); } break;
      #endif

        #ifdef CODEC1
    case 109: TMBENCH("",l=spdpenc(in,m*(USIZE),out,SPDPSIZE,codlev),n);pr(l,n); TMBENCH2("109",spdpdec(           out, m*(USIZE), cpy,SPDPSIZE,codlev); ,n); break;
        #endif

    case 117: l = n; TMBENCH("", tpenc( in, n, out,USIZE),n);       pr(l,n); TMBENCH2("107", tpdec( out, n,cpy, USIZE),n); break;
    case 118: l = n; TMBENCH("", tp4enc(in, n, out,USIZE),n);       pr(l,n); TMBENCH2("108", tp4dec(out, n,cpy, USIZE),n); break;
      #ifdef BITSHUFFLE
    case 119: l = n; TMBENCH("", bitshuffle(in, n, out, USIZE),n);  pr(l,n); TMBENCH2("109", bitunshuffle(out, n,cpy, USIZE),n); break;
      #endif
    case ID_MEMCPY: if(!mcpy) goto end;
                     TMBENCH( "", libmemcpy(out,in,n) ,n); l=n;     pr(l,n); TMBENCH2("110", libmemcpy( cpy,out,n) ,n); break;
    default: goto end;
  }
  if(l) {
    char s[65]; printf("%-30s ", bestr(id, 64,s, codstr(codid),codlev));
    if(cpy) rc = memcheck(in,m*(USIZE),cpy);
    if(!rc)
      printf("\t%s\n", inname?inname:"");
  }
  end:if(tmp) free(tmp);
  return l;
}

void usage(char *pgm) {
  fprintf(stderr, "\nIcApp Copyright (c) 2013-2021 Powturbo %s\n", __DATE__);
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
  fprintf(stderr, "    x = entry format\n");
  fprintf(stderr, "        [b=int8], [s=int16], [u=int32(default)], [l=int64], [f:float] [d:double]\n");
  fprintf(stderr, "    .# = decimal digits (default 2). Convert dec. numbers to integers\n");
  fprintf(stderr, "    H  = skip first line(s)\n");
  fprintf(stderr, " -H = skip first line(s). Only for text files\n");
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

int main(int argc, char* argv[]) {
  unsigned      b = 1 << 30, lz=0, fno,m=1000000;
  int           isize=4,dfmt = 0,kid=1,skiph=0,decs=0,divs=1,nsd=-1,dim0=0;//tp2test();tp3test();exit(0);
  uint64_t      be_mindelta=0;
  unsigned char *in = NULL, *out, *cpy, *scmd = NULL, *icmd = NULL;
  double        mdelta=-10,errlim=0.0;
  tm_verbose = 1;

  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"blocsize",  0, 0, 'b'}, {0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "a:B:C:d:D:d:e:E:f:F:g:G:I:J:k:K:Hl:m:M:n:R:s:v:V:w:y", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;
      case 'C': cmp    = atoi(optarg); break;
      case 'e': icmd   = optarg; break;
      case 'E': scmd   = optarg; break;
      case 'G': nsd    = atoi(optarg);  break;
      case 'D': decs   = atoi(optarg);  break;

      case 'F': { char *s = optarg;
             if(*s=='c') { dfmt = T_CHAR; s++;}
        else if(*s=='t') { dfmt = T_TXT; if(*++s > '0' && *s <= '9') { kid = *s++ - '0'; if(*s > '0' && *s <= '9') kid = kid*10 + (*s++ - '0'); } }
        else if(*s=='e') { dfmt = T_TST; s++; }
        else if(*s=='r') { dfmt = 0;     s++; } // default

             if(*s=='f') isize = -4, s++; // float : 4 bytes
        else if(*s=='d') isize = -8, s++; // double: 8 bytes
        else if(*s=='b') isize =  1, s++; // 1 byte
        else if(*s=='s') isize =  2, s++; // 2 bytes
        else if(*s=='u') isize =  4, s++; // 4 bytes
        else if(*s=='l') isize =  8, s++; // 8 bytes
        else if(*s=='t') isize =  4, s++, dfmt = T_TIM32; // 4 bytes, timestamp
        else if(*s=='T') isize =  8, s++, dfmt = T_TIM64; // 8 bytes, timestamp
        if(*s == '.') { if(*++s >= '0' && *s <= '9') { decs = s[0] - '0'; s++; } } // number of decimals after .
        if(*s == 'v') { divs = strtod(++s, &s); }
        if(*s == 'H') { skiph++; s++; } // skip first line(s). ex.  HHH : skip 3 first lines
        //switch(*s) { case 's': be_mindelta = 0; break; case 'S': be_mindelta = 1; break; case 'z': be_mindelta = 2; break; }
      } break;

      case 'g': errlim = strtod(optarg, NULL); break;
      case 'H': skiph++;                    break;
      case 'K': { kid = atoi(optarg); if(!keysep) keysep = ",;\t"; } break;
      case 'k': keysep = optarg; break;
      case 'I': if((tm_Rep  = atoi(optarg))<=0) tm_rep=tm_Rep=1; break;
      case 'J': if((tm_Rep2 = atoi(optarg))<=0) tm_rep=tm_Rep2=1; break;
      case 's': isize = argtoi(optarg,1);    break;
      case 'B': b = argtoi(optarg,1);        break;
      //case 'c': cmp++;                         break;
      case 'l': codlev = atoi(optarg);           break;
      case 'y': mcpy = 0;                    break;

      case 'a': a = strtod(optarg, NULL);    break;
      case 'd': mdelta  = strtod(optarg, NULL); /*printf("MDELTA=%d ");*/break;
      case 'n': m       = argtoi(optarg,1);  break;
      case 'm': rm      = argtoi(optarg,1);  break;
      case 'M': rx      = argtoi(optarg,1);  break;
      case 'f': isize   = -argtoi(optarg,1); break;
      case 'R': { char *p;        dim1 = strtoul(optarg, &p, 10); if(!dim1) dim0++;
                           if(*p) dim2 = strtoul(p+1,    &p, 10);
                           if(*p) dim3 = strtoul(p+1,    &p, 10);
                           if(*p) dim4 = strtoul(p+1,    &p, 10); printf("dim=%dx%dx%dx%d ", dim1, dim2, dim3, dim4);
                }   break;
      case 'w': tm_verbose = atoi(optarg); break;
      case 'v': verbose = atoi(optarg);    break;
      case 'V': divs    = atoi(optarg);  break;
      default:
        usage(argv[0]);
        exit(0);
    }
  }
    #ifdef LZTURBO
  beini();
    #endif
  if(argc - optind < 1) {
    usage(argv[0]);
    exit(0);
  }
  isa = cpuisa();
  printf("detected simd id=%x, %s\n\n", cpuini(0), cpustr(cpuini(0)));
  unsigned char _scmd[33]; strcpy(_scmd, "lz4,1");
  if(!scmd) scmd = _scmd;
  while(isspace(*scmd)) scmd++;
  char *q;
  int   i;
  if(q = strchr(scmd,',')) *q = '\0';
  for(i = 0; _codstr[i]; i++)
    if(!strcasecmp(scmd, _codstr[i])) { codid = i;  break; }
  if(!_codstr[i]) { printf("compressor '%s' not implemented\n", scmd); exit(-1); }
  scmd = q?(q+1):(char*)"";
  codlev = strtoul(scmd, &scmd, 10);

  if(scmd) strcpy(codprm,scmd);
                                                                                    if(verbose>1) printf("dfmt=%d,size=%d\n", dfmt, isize);
  for(fno = optind; fno < argc; fno++) {
    char      *inname = argv[fno];
    int       i,n;
    long long flen;
    FILE      *fi = NULL;
    if(!strcmp(inname,"ZIPF") || !strcmp(inname,"TMS")) flen = n = m*abs(isize);
    else {
      fi = fopen(inname, "rb");                             if(!fi) { perror(inname); continue; }
      if(dfmt) {
        n = flen = befgen(&in, 0, dfmt, isize, fi, kid, skiph, decs, divs, be_mindelta);
      } else {
        fseek(fi, 0, SEEK_END);
        flen = ftell(fi);
        fseek(fi, 0, SEEK_SET);
        if(flen > b) flen = b;
        n = flen;
        if(dim0) { char *p; if(!(p = strrchr(inname, '\\')) && !(p = strrchr(inname, '/'))) p=inname;
          dim1=dim2=dim3=dim4=0;
          while(!isdigit(*p)) p++;                                                                      if(verbose>1) printf("fn='%s' ", p);
                          dim1 = strtoul(p,   &p, 10);
           if(dim1 && *p) dim2 = strtoul(p+1, &p, 10);
           if(dim2 && *p) dim3 = strtoul(p+1, &p, 10);
           if(dim3 && *p) dim4 = strtoul(p+1, &p, 10);
        }
      }
    }

    if(dim4 && !dim3) dim3 = 1; if(dim3 && !dim2) dim2 = 1; if(dim2 && !dim1) dim1 = 1;
    if(dim1 || dim2 || dim3) printf("dim=%dx%dx%dx%d\n", dim1, dim2, dim3, dim4);

    if(!in && !(in  = (unsigned char*)malloc(n+64+1024        ))) die("malloc error 'in =%d'\n", n); cpy = in;
    if(!(out =        (unsigned char*)malloc(CBUF(n)+1024*1024))) die("malloc error 'out=%d'\n", n);
    if(cmp && !(cpy = (unsigned char*)malloc(CBUF(n)+1024*1024))) die("malloc error 'cpy=%d'\n", n);
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

    if(errlim > 0.0 || nsd>=0) {  // convert input for lossy floating point compression
      if(errlim < 0.000009999) errlim = 0.00001;
      if(isize == -4) {
                             fppad32(in,n/4,out,errlim); if(verbose>0) fpstat(in, n/4, out, -4); //if(nsd >= 0) fprnd32(in,n/4,out,nsd); else //
                             fppad32(in,n/4, in,errlim);                                         /*if(nsd >= 0) fprnd32(in,n/4,in, nsd); else */
                                                                                                 //TMBENCH("fppad32",fppad32(in,n/8,out,errlim),n);printf("\n");
      } else if(isize == -8) {                                                                   //TMBENCH("fppad64",fppad64(in,n/8,out,errlim),n);printf("\n");
          fppad64(in,n/8,out,errlim); if(verbose>0) fpstat(in, n/8, out, -8);                    /*if(nsd>=0) fprnd64(in,n/8,out,nsd); else*/
          fppad64(in,n/8, in,errlim);                                                            /*if(nsd>=0) fprnd64(in,n/8, in,nsd); else*/
      }
        #ifdef USE_FLOAT16
      else if(isize == -2) {
          fppad16(in,n/2,out,errlim); if(verbose>0) fpstat(in, n/2, out, -2);                    /*if(nsd>=0) fprnd64(in,n/8,out,nsd); else*/
          fppad16(in,n/2, in,errlim);                                                            /*if(nsd>=0) fprnd64(in,n/8, in,nsd); else*/
      }
        #endif
    }
    be_mindelta = mindelta(in, n/abs(isize), abs(isize)); 

    if(fi && verbose) { unsigned l;                                                                     // Calculate bits distributions
      switch(abs(isize)) {
        case 1: l=histl8( in,n);   stprint("file: max", xbits); if(histz8( in,n  )<l) stprint("file: delta max", zbits); break;
        case 2: l=histl16(in,n/2); stprint("file: max", xbits); if(histz16(in,n/2)<l) stprint("file: delta max", zbits); break;
        case 4: l=histl32(in,n/4); stprint("file: max", xbits); if(histz32(in,n/4)<l) stprint("file: delta max", zbits); break;
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
      printf("  E MB/s     size     ratio     D MB/s function %s size=%d bits (lz=%s,%d%s) ", isize>0?"integer":"floating point", abs(isize)*8, codstr(codid), codlev, codprm);
      if(be_mindelta == (uint64_t)-1) printf("unsorted %lld ", be_mindelta);
      else printf("sorted(mindelta=%lld) ", be_mindelta);
      if(errlim>0.0) printf("FP err=%f", errlim);
      printf("\n");
    }
    do {
      unsigned id = strtoul(p, &p, 10),idx = id, i;
      while(isspace(*p)) p++; if(*p == '-') { if((idx = strtoul(p+1, &p, 10)) < id) idx = id; if(idx > ID_MEMCPY) idx = ID_MEMCPY; }
      for(i = id; i <= idx; i++)
        switch(abs(isize)) {
          case 1: bench8( in,n,out,cpy,i,optind+1==argc?NULL:inname,codlev); break;
          case 2: bench16(in,n,out,cpy,i,optind+1==argc?NULL:inname,codlev); break;
          case 4: bench32(in,n,out,cpy,i,optind+1==argc?NULL:inname,codlev); break;
          case 8: bench64(in,n,out,cpy,i,optind+1==argc?NULL:inname,codlev); break;
          default: die("integer size must be 1, 2, 4 or 8\n");
        }
    } while(*p++);
    printf("\n");
    free(in); free(out); free(cpy); in = out = cpy = NULL;
  }
}
