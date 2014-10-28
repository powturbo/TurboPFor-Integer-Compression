/**
    Copyright (C) powturbo 2013-2014
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

    - email    : powturbo@gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo

    icbench.c - "Integer Compression" benchmark program
**/

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#include <getopt.h>
#include <sys/stat.h>
#define PGM_FD(__f) struct stat sbuf; fstat(__f, &sbuf); __off64_t vlen = sbuf.st_size, vtel = 0; int pgm = 0; time_t t0 = time(NULL);
#define PGM_FDPUT(__f) vtel = lseek(__f, 0, SEEK_CUR);if(vtel*10/vlen != pgm) { double secs = time(NULL) - t0; pgm = vtel*10/vlen; printf("%d%%%.1f ", pgm, ((secs/60.0) * (vlen - vtel))/vtel); fflush(stdout); }
//-------------------------------------------------------------------------------------------------------------
typedef unsigned long long tm_t;
#define TM_TMAX (1ull<<63)

  #ifdef _MSC_VER  // __rdtsc
#include <intrin.h>
  #else
#include <x86intrin.h>
  #endif

  #ifdef _WIN32
#include <windows.h>
#define TM_T 1

static tm_t tmtime(void) {
  LARGE_INTEGER tm;
  QueryPerformanceCounter(&tm);
  return (tm_t)(tm.QuadPart/tps.QuadPart);
}

LARGE_INTEGER tps;
static tm_t tminit() { QueryPerformanceFrequency(&tps); tm_t t0=tmtime(),ts; while((ts = tmtime())==t0); return ts; } 
  #else
#include <sys/time.h>
#define TM_T 1000000.0
static tm_t tmtime(void) {
  struct timeval tm;
  gettimeofday(&tm, NULL);
  return (tm_t)tm.tv_sec*1000000ull + tm.tv_usec;
}

static tm_t tminit() {                                  tm_t t0=tmtime(),ts; while((ts = tmtime())==t0); return ts; }
  #endif
//--------------------------------------------------------------------------------------------------------
#include "vint.h"
#include "vsimple.h"

#include "bitpack.h"
#include "bitunpack.h"
#include "vp4dc.h"
#include "vp4dd.h"

#include "aux/vas16c.h"
#include "aux/vas16d.h"
#include "aux/OPT_PFD/opt_p4.h"
#include "aux/vabyte.h"
#include "aux/simple8b.h"
#include "aux/varintg8iu.h"

unsigned char *simdpackwn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpackwithoutmask(in, (__m128i *)out, b);
  return out;
}

unsigned char *simdpackn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpack(in, (__m128i *)out, b);
  return out;
}

unsigned char *simdunpackn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {  
  uint32_t k, *out_; 
  for(out_ = out + n; out + 128 <= out_; out += 128, in += 4 * b) simdunpack(in, out, b); 
  return in;
}

unsigned char *simdpackwn1(uint32_t *in, uint32_t n, uint32_t b, uint32_t start, uint32_t *out) {//checkifdivisibleby(n, 128); const uint32_t * const initout(out);  //while(needPaddingTo128Bits(out)) *out++ = 123456;
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpackwithoutmaskd1(start, in, (__m128i *)out, b); //simdpackwithoutmaskd1(x, ip+1, (__m128i *)out, b); 
  return out;
}

unsigned char *simdunpackn1(uint32_t *in, uint32_t n, uint32_t b, uint32_t start, uint32_t *out) {
  uint32_t k, *out_; 
  for(out_ = out + n; out + 128 <= out_; out += 128, in += 4 * b) simdunpackd1(start, in, out, b); 
  return in;
}

unsigned char *u32enc(unsigned *__restrict__ in, int n, unsigned *__restrict__ out) { unsigned *in_   = in +n; while(in  <  in_) *out++ = *in++; return out;}
unsigned char *u32dec(unsigned *__restrict__ in, int n, unsigned *__restrict__ out) { unsigned *out_  = out+n; while(out < out_) *out++ = *in++; return in;}

#include "aux/vbyte_poly.h"
unsigned char *vavbyte1enc(int *in, int n, unsigned char *out) {
  int i; for(i = 0; i < n; i++) { unsigned x = in[i]; VBYTE_ENC(out, x); } return out;
}
void vavbyte1dec(unsigned char *in, int n, int *out) {
  int i; for(i = 0; i < n; i++) { unsigned x; VBYTE_DEC(in, x); out[i] = x; } return out;
}

//-------------------------------------------------------------------------------------------------
#define VBLIM 64
enum { 
  P_CPY, 
  P_VB, 		P_VBL, P_VG8,			
  P_PCK, P_PCKR, 	P_SIMDH,
  P_SV, 		P_S16, P_S8BO, 
  P_P4D, P_P4DR, 	P_OPTP4
};

unsigned char *beenc(unsigned *in, size_t n, unsigned char *out, int id, int bb) { 
  unsigned *ip=in; 
  int i,b;

  switch(id) {
    case P_CPY:  
      out = u32enc(   ip, n, out); break;
    case P_VB:   
      out = vbenc(    ip, n, out); break; 
    case P_VBL:  
      out = vbyteenc( ip, n, out); break;
    case P_VG8:  
      out = vintg8enc(ip, n, out); break;

    //----------- simple ------------------- 
    case P_SV:   
      out = vsenc32(  ip, n, out); break;
    case P_S16:  
      { unsigned *c=ip,*ce=c+n; 
        while(c < ce) S16ENC(out, c, ce - c); 
      } 
      break;
    case P_S8BO: 
      out = s8benco(  ip, n, out); 
      break;

    //----------- PFOR ------------------- 
    case P_P4DR:
    case P_P4D:   
      if(n<VBLIM) 
        out = vbyteenc(ip, n, out); 
      else { 
        vbput(out, *ip); 
        out = p4denc32(ip+1, n-1, out); 
      } 
      break;
    case P_OPTP4: 
      if(n < VBLIM) 
        out = vbyteenc(ip, n, out); 
      else { 
        unsigned tmp[0x400],j;
        for(j = 0; j < n; j++) tmp[j]=ip[j]; 
        out +=  OPT4(tmp, n, out); 
      } 
      break;

    //----------- Binary packing ------------------- 
    case P_PCKR: 
    case P_PCK:  
      { unsigned x = *ip; 
        if(bb<0) { 
          for(b=0,i=1; i < n; i++) 
            b |= ip[i]; 
          b = bsr32(b); 
          x = x <<5 | b; 
        } 
        vbput(out, x); 
        out = bitpack32(ip+1, n-1, b, out); 
      } break;

    case P_SIMDH:
      if(n < 129) out = vbyteenc(ip, n, out); 
      else { 
        unsigned x = *ip; 
        if(bb < 0) { 
          for(b=0,i=1; i < n; i++) 
            b |= ip[i]; 
          b = bsr32(b); x = x <<5 | b; 
        } 
        vbput(out, x); 
        out = simdpackwn(ip+1, n-1, b, out); 
      } break; 
    default: printf("Fatal- Not entry %d", id); exit(0);
  } 
  return out;
}

unsigned char *bedec(unsigned char *in, size_t n, unsigned *out, int id, int bb) { unsigned *op=out; int i,b;
  switch(id) {
    case P_CPY:  
      return u32dec(in, n, op);
    case P_VB:   
      in = vbdec(  in, n, op); 
      break; 
    case P_VBL:  
      in = vbytedec( in, n, op); 
      break;										
    case P_VG8:
      in = vintg8dec(in, n, op); break;	
	
    case P_SV:  
      in = vsdec32(   in, n, op); 
      break;
    case P_S16:  
      { 
        unsigned *d = op,*de = d+n; 
        while(d < de) S16DEC(in, d, ;); 
      }
      break;
    case P_S8BO: 
      in = s8bdeco(   in, n, op); 
      break;
    case P_P4D :   
      if(n<VBLIM) 
        in = vbytedec( in, n, op);
      else { 
        unsigned x; 
        vbgeta(in, x, *op = x); 
        in = p4ddec32(  in, n-1,    op+1); 
      } 
      break;
    case P_P4DR :  
      if(n < VBLIM) 
        in = vbytedec( in, n, op);
      else { 
        unsigned x; 
        vbgeta(in, x, *op = x); 
        in = p4ddecx32( in, n-1,    op+1); 
      } 
      break;
    case P_OPTP4 : 
      if(n < VBLIM) 
        in = vbytedec( in, n, op); 
      else { 
        unsigned all_array[2048]; 
        in = detailed_p4_decode(  op, in, all_array); 
      } 
      break;
    case P_PCK:   
      { unsigned x; 
        vbgeta(in, x, ;); 
        if(bb < 0) { 
          b = x & 0x1f; x >>= 5; 
        } 
        *op = x; 
        in = bitunpack32( in, n-1, b, op+1);
      } 
      break;				          
    case P_PCKR:  
      { 
        unsigned x; 
        vbgeta(in, x, ;); 
        if(bb < 0) { 
          b = x & 0x1f; x >>= 5; 
        } 
        *op = x; 
        in = _bitunpackx32(in, n-1, b, op+1);
      } 
      break;				          
    case P_SIMDH: 
      if(n <129) in = vbytedec(in, n, op); 
      else { 
        unsigned x; 
        vbgeta(in, x, ;); 
        if(bb < 0) { 
          b = x & 0x1f; x >>= 5; 
        } 
        *op = x; 
        in = simdunpackn( in, n-1, b, op+1);
      } 
      break;
    default: printf("Fatal- Not entry %d", id); exit(0);
  }
  return in;
}

struct libss { int id; char *s,*v; };

struct libss libss[] = { 
  { P_CPY,   "copy",            },
  { P_VB,    "TurboVbyte"	},
  { P_VBL,   "Vbyte FPF" 	},
  { P_VG8,   "vg8iu" 		},

  { P_SV,    "simpleV" 		},
  { P_S8BO,  "simple 8b" 	},
  { P_S16,   "simple16" 	},

  { P_P4DR,  "TurboPFor DA" 	},
  { P_P4D,   "TurboPFor"	},	
  { P_OPTP4, "OptP4"		},

  { P_PCK,   "TurboPack"	},
  { P_PCKR,  "TurboPack DA"	},
  { P_SIMDH, "SIMDBitPack FPF"	},
  { -1, "" },
};

//---------------------------------------------------------------------------------------------
#define MAXT 8
#define BLK_SIZE 129
#define MB (1024*1024)

int verb = 0, reps = 100000, trips = 3;
enum { T_ZIPF=1, T_ID };

struct libs { int id,err; char *s,*v; unsigned long long l; double tc,td; };
struct libs libs[64];

int l_cmp(struct libs *a, struct libs *b) {
  if(a->l  < b->l || a->l == b->l && a->td < b->td) return -1;
  if(a->l  > b->l || a->l == b->l && a->td > b->td) return  1;
  return 0;
}

void check(unsigned *in, unsigned n, unsigned *out, char *s) {
  unsigned k,j; 
  for(k = 0; k < n; k++) 
    if(in[k] != out[k]) { 
      printf("\nFATAL in check %x,%x at %u[%u] in %s\n", in[k], out[k], k, n, s); 
   for(j=k & 0xffffff80u; j < k+128;j++) 
     printf("%d:%x,%x ", j, in[j], out[j]  );printf("\n");
   exit(0); 
  } 
}

void print(unsigned long long n, char *s) { 
  int m, k; 
  for(k = 0; libs[k].id >= 0; k++);
    qsort(libs, k, sizeof(libs[0]), l_cmp);	

  for(m = 0; m < k; m++) 
    if(libs[m].l) { 
      struct libs *lb = &libs[m]; 
      printf("%-16s%12llu\t%5.2f\t%5.2f\t%8.2f\t%8.2f\t%s\n", s, lb->l, (double)lb->l*100.0/((double)n*4.0), (double)lb->l*8.0/(double)n, 
        lb->tc>=0.000001?((double)n/1000000.0) / (lb->tc/TM_T):0.0,
        lb->td>=0.000001?((double)n/1000000.0) / (lb->td/TM_T):0.0,
        lb->s ); 
    }
}

//int libini() { int m; for(m = 0; libs[m].id >= 0; m++) libs[m].l = libs[m].tc = libs[m].td = 0; }

unsigned bench(unsigned *__restrict__ _in, unsigned _inlen, int blksize, unsigned char *__restrict__ _out, unsigned long long outsize, char *inname, tm_t tx, unsigned *__restrict__ cpy, int bb) { int m,id,b=bb,i; if(verb) { printf(":%d,", _inlen);  fflush(stdout);}
  unsigned cn; 																						tm_t tt0 = tminit();
  for(i = 0; i < 10; i++) memcpy(_out, _in, _inlen); 																																			
  for(m = 0; (id=libs[m].id) >= 0; m++) { int r,insize=(id==P_OPTP4)?blksize-1:blksize; 
    struct libs *lb = &libs[m]; unsigned cl;	if(verb) { printf("%s", libs[m].s);fflush(stdout);	} int t,tj; tm_t t0,tc=TM_TMAX,td=TM_TMAX,tt;
    for(t = 0; t < trips; t++) { 												t0 = tminit();
      for(r = 0; r < reps; ) { 
        cn=cl=0; 
        unsigned *in; 
        unsigned char *out,*sout; 					        //vsini();
        for(out = _out, in = _in; in < _in+_inlen; ) { 
          unsigned n,inlen = *in++,*ip=in; in += inlen; 
           *(unsigned *)out = inlen; out+=4;/*out++=0x5a;*/ 
          for(;ip < in; ip += n) { n = ip+insize<=in?insize:in-ip; cn += n; unsigned char *sout=out; 		//printf("%d ", n);
            out = beenc(ip,n,out,id,bb);
            cl +=out-sout;	
          } 															if(out > _out+outsize) { fprintf(stderr, "Overflow error %lld, %lld in %s\n", outsize, (ptrdiff_t)(out - _out), lb->s); exit(0); }  
        } r++;															if((tt = tmtime() - t0) > tx) break;
      } 															if(tt < tc) { tc = tt; tj = r; }
																if(tmtime() - tt0 > tx*trips) { /*printf("#");fflush(stdout);*/ /*sleep(1);*/tt0 = tminit(); }
    }
    lb->l += cl; lb->tc += tc/tj; memset(cpy, 0xf, _inlen*4);									if(verb) { printf("+ ");fflush(stdout);} 
																tt0 = tminit();
    for(t = 0; t < trips; t++) {												t0 = tminit();
      for(r = 0; r < reps; ) { unsigned *out; unsigned char *in;
        for(out = cpy, in = _out; out < cpy+_inlen;) { 
          unsigned n,*op, outlen=*(unsigned *)in; in+=4; 
          *out++ = outlen;
          for(op=out,out += outlen; op < out; op += n) { 
            n = op + insize<=out?insize:out-op;
            in = bedec(in,n,op,id,bb);
          }
        } 
	r++;												
        if((tt = tmtime() - t0) > tx) 
          break;
      }
      if(tt < td) { 
        td = tt; 
        tj = r; 
      } 
      if(tmtime() - tt0 > tx*trips) { 
        tt0 = tminit(); 
      }
    } lb->td += td/tj;
    check(_in, _inlen, cpy, lb->s);
  }
  return cn;
}

int z_cmp(double **a, double **b) {
  if(*a < *b) return -1;
  if(*a > *b) return  1;
  return 0;
}

void zipfgen(unsigned *a, double alpha, unsigned x1, unsigned x2, int n) { 
  int i,m = x2 - x1 + 1; 
  double prob, cum, *zmap;
  if(!(zmap = malloc(m*sizeof(zmap[0])))) { 
    fprintf(stderr, "mallo error\n"); 
    exit(-1); 
  };

  srand48(1); 
  for(cum =0.0,i = 0; i < m; i++) 
    cum += 1.0 / pow(i+1, alpha); 
  cum = 1.0 / cum;
  for(prob=0.0,i = 0; i < m; i++) 
    zmap[i] = prob += cum / pow(i+1, alpha);
  qsort(zmap, m, sizeof(zmap[0]), (int(*)(const void*,const void*))z_cmp);

  for(i = 0; i < n; i++) { 
    double r = drand48(); 
    int l = 0, h = m-1; 
    while(l < h) { 
      int k = (l + h) >> 1; 
      if(r > zmap[k]) l = k + 1; 
      else h = k; 
    }
    a[i] = x1 + l;	 
  } 
  free(zmap);
}

#define OVD (10*MB)
int main(int argc, char *argv[]) { 
  char fname[0x100], *cmd=NULL;
  unsigned  bp=0,ftype = T_ID, rm=0,rx=30,n=10000000;  
  long long rdmax = 1<<30; tm_t tx=1*1000000; 
  double    a = 1.5;

  tminit();
  VarIntG8IU();

  int c, digit_optind = 0; 
  int this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"repeat", 	0, 0, 'r'}, {0,0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "Ac:TBR:ys:r:n:b:c:e:t:r:M:v:m:x:a:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n");   break;								
      case 'r': reps = atoi(optarg); break;
      case 'R': trips = atoi(optarg); break;
      case 'v': verb = atoi(optarg);verb++; break;
      case 't': tx = atoi(optarg)*1000000; break;
      case 'c': ftype = atoi(optarg); break;
      case 'b': rdmax = atoi(optarg)*MB; break;
      case 'e': cmd=optarg; break;
      case 'm': rm = atoi(optarg); break;
      case 'x': rx = atoi(optarg); break;      //
      case 'B': bp++; break;
      case 'n': n = atoi(optarg); break;
      case 'a': a = strtod(optarg, NULL); break;
      default:  fprintf(stdout,"unknown option: %c \n", optopt); exit(1);
    }
  }																														
  int fno,i=0; //libini(); 
  if(!bp) { rm = (1<<rm)-1; rx = (1ull<<rx)-1; if(rx > n) rx = n; } else if(!rm) rm = 1; 
  //printf("range=(%d,%d,%d)\n", rm, rx, n);fflush(stdout);
  struct libss *ls; 
  if(cmd) {
    unsigned char *q=NULL;
    for(i=0,libs[0].id = -1;;) { 
      if(cmd) { 
        if(!*cmd) break; 							//printf("cmd='%s'", cmd); 
        q = strchr(cmd,','); 
        if(q) *q=' '; 
        if(q = strchr(cmd,'/')) 
          *q = '\0';
        for(ls = libss; ls->id >= 0; ls++) 
          if(!strcasecmp(ls->s, cmd)) { 
            memset(&libs[i], 0, sizeof(struct libs)); 
            libs[i].id = ls->id; 
            libs[i].err = 0; 
            libs[i].s = ls->s; 
            libs[i++].v = ls->v; 
            break; 
          }
        if(ls->id < 0) { 
          printf("library: '%s' not found\n", cmd); 
          exit(-1); 
        }
        cmd = q?(q+1):"";
      }
    }
  } else for(ls = libss; ls->id >= 0; ls++) { 
    libs[i].id = ls->id; 
    libs[i].err = 0; 
    libs[i].s = ls->s;  //printf("%s\n", ls->s);fflush(stdout);
    libs[i++].v = ls->v;    
  }
  libs[i].id = -1;

  if(argc <= optind) {
    unsigned *in, *out, *cpy,*ip; unsigned long long totlen=0; 
    in  = malloc(n*4+OVD);     if(!in)  { printf("malloc err=%u", n); exit(0); }
    out = malloc(n*4+OVD);     if(!out) { printf("malloc err=%u", n); exit(0); }
    cpy = malloc(n*4+OVD);     if(!cpy) { printf("malloc err=%u", n); exit(0); }
    char s[33]; s[0]=0; 
    if(bp) { 
      int b; 
      printf("bittest\n"); fflush(stdout);
      for(b = rm; b <= rx; b++) { 
        sprintf(s,"b=%d", b);        
        *in = n; 
        for(i = 1; i <= n; i++) 
          in[i] = (1ull << b)-1;
        totlen = bench(in, n+1, BLK_SIZE, out, n*4+OVD, s, tx, cpy, b);
        print(totlen, s);
      }
    } else { 
      printf("zipf a=%3.1f [%u,%u]\n", a, rm, rx); 
      *in = n; 
      zipfgen(in+1, a, rm, rx, n);   //stprint();
      totlen = bench(in, n+1, BLK_SIZE, out, n*4+OVD, s, tx, cpy, -1);
      print(totlen, s); 
    }
    free(in);
    free(cpy);
    free(out);
  } else for(fno = optind; fno < argc; fno++) { 
    char *inname = argv[fno]; 
    FILE *fi = fopen64(inname, "r"); 
    if(!fi) { 
      fprintf(stderr, "open error '%s'", inname); perror(inname); 
      exit(-1); 
    } 
    fseek(fi, 0, SEEK_END); 
    unsigned long long fisize = ftell(fi);
    fseek(fi, 0, SEEK_SET); 
    if(fisize > rdmax) 
      fisize = rdmax; 
    fisize /= 4;				    //setvbuf(fi, NULL, _IOFBF, 1000*MB);																		
    unsigned *in, *out, *cpy,*ip; 
    unsigned long long totlen=0; 
    int rc; 
    out = malloc(fisize*4+OVD);   if(!out) { printf("malloc err=%u", fisize); exit(0); }  
    cpy = malloc(fisize*4+OVD);   if(!cpy) { printf("malloc err=%u", fisize); exit(0); } 																		    
    in  = malloc(fisize*4+1024);  if(!in)  { printf("malloc err=%u", fisize); exit(0); } 	PGM_FD(fileno(fi));	
    int r; fread(&r, 4, 1, fi);
    while(r > 0) { 
      for(ip = in; ip+r <= in+fisize;) { 
        int rc; 										PGM_FDPUT(fileno(fi)); 
        if((rc = fread(ip+1, 4, r, fi)) <= 0) 
          goto a;																											
																
        if(r >= rm && r <= rx) { 
          *ip++ = r;
          int j;										
          if(verb) 
            printf("%d ", r, ftype==T_ID?"I":"N");
          fflush(stdout);
          if(ftype == T_ID) {
            for(j = 0; j < r; ) { 
              unsigned m = j+BLK_SIZE>r?r-j:BLK_SIZE;
              int i,did,dido = -1; 
              for(i = 0; i < m; i++) { 
                did = ip[i]; 
                if(did < dido) { 
                  printf("IDs in '%s' not sorted.did=%d,dido=%d ", inname, did, dido);
                  exit(0);
                }
                ip[i] = did - dido - 1; 
                dido = /*ip[0]*/did; 				//printf("%d,", ip[i]); xbits[bsr32(ip[i])]++; 
              }
              j += m; ip += m;								//printf("\r");
            }
          } else 
            ip += r; 
        }
        r = rc = 0; 
        if(ftype == T_ID) 
          rc = fread(&r, 4, 1, fi); 
        if(rc <= 0 || !r) 
          break;
      }
      totlen += bench(in, ip-in, BLK_SIZE, out, fisize*4+OVD, inname, tx, cpy, -1); 
      if(totlen > n) 
        break;
    }
    a:fclose(fi);			//stprint();
    print(totlen,inname); 
    free(in);
    free(cpy);
    free(out);
  }															
}

