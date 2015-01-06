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

    - email    : powturbo [AT] gmail.com
    - github   : https://github.com/powturbo
    - homepage : https://sites.google.com/site/powturbo/
    - twitter  : https://twitter.com/powturbo
**/   
#define _LARGEFILE64_SOURCE 1 
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
  #ifndef _WIN32
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
  #endif
#include <getopt.h>

#include "conf.h"
#include "vint.h"
#include "bitunpack.h"
#include "vp4dd.h"
#include "idx.h"

#define STATS
//---------------------------------------- Time ---------------------------------------------------------------------
typedef unsigned long long tm_t;
#define TM_TMAX (1ull<<63)

#include <sys/time.h>
#define TM_T 1000000.0
static   tm_t tmtime(void)    { struct timeval tm; gettimeofday(&tm, NULL); return (tm_t)tm.tv_sec*1000000ull + tm.tv_usec; }
static   tm_t tminit()        { tm_t t0=tmtime(),ts; while((ts = tmtime())==t0); return ts; }
static double tmsec( tm_t tm) { return (double)tm/1000000.0; }
static double tmmsec(tm_t tm) { return (double)tm/1000.0; }

//--------------------------------------- Simdcomp -------------------------------------------------------------------
#include "ext/simdcomp/include/simdbitpacking.h"
unsigned char *simdunpackn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {  
  uint32_t k, *out_; 
  for(out_ = out + n; out + 128 <= out_; out += 128, in += 4 * b) simdunpack((const __m128i *)in, out, b); 
  return (unsigned char *)in;
}
unsigned char *simdunpackn1(uint32_t *in, uint32_t n, uint32_t b, uint32_t start, uint32_t *out) {
  uint32_t k, *out_; 
  for(out_ = out + n; out + 128 <= out_; out += 128, in += 4 * b) simdunpackd1(start, in, out, b); 
  return (unsigned char *)in;
}

//------------------------------------- index file (created by idxcr) -------------------------------------------------------------
typedef struct {         	// Index 
  unsigned char      *fdp, 	// posting
                     *fdm; 	// mapping term id to offset in posting
  unsigned long long fdsize;
  unsigned           tnum;
} idxrd_t;

int idxopen(idxrd_t *idx, char *s) {
  int fd; char *p;
  if((fd = open(s, O_RDONLY| O_LARGEFILE)) < 0) 
    die("can't open index file '%s' rc=%d:%s\n", s, errno, strerror(errno));
  struct stat sbuf;  // Memory mapped access
  fstat(fd, &sbuf); 
  if(sbuf.st_size > 0 && (p = mmap( NULL, sbuf.st_size , PROT_READ, MAP_SHARED|MAP_NORESERVE, fd, 0)) == (void *)-1) 
    die("mmap errno=%d,'%s'\n", errno, strerror(errno) );
  close(fd);

  idx->fdsize = sbuf.st_size;
  idx->fdp    = p;
  idx->fdm    = p + *(uint64_t *)p; p += sizeof(uint64_t);  // Termid map table. Termid->Posting 
  idx->tnum   = *(unsigned *)p;
  return 0;
}

int idxclose(idxrd_t *idx) { 
  munmap(idx->fdp, idx->fdsize); 
}

//--------------------------------- Posting --------------------------------------------------------------
#ifdef STATS
unsigned long long st_tot,st_dec;
#define STATINI st_tot=st_dec=0
#define STAT(a) a
#else
#define STATINI
#define STAT(a)
#endif

typedef struct {
  unsigned char *bp,*p;
  unsigned      f_t,_f_t, did,ldid;
  int           didno,didnum, bno, bnum;
} post_t;

// Init posting for term id tid
int postinit( post_t *post, int tid, idxrd_t *idx, unsigned *dids) { 
  unsigned long long o = TIDMAP(idx->fdm, tid); if(!o) return 0;  			
  unsigned char *p = idx->fdp + o;  					// start of posting;
  post->f_t     = vbget(p); 							// num docs  
  post->bnum    = (post->f_t+BLK_DIDNUM-1)/BLK_DIDNUM;  // num blocks
  post->_f_t    = post->f_t;
  post->didno   = post->bno = -1;
  post->bp      = p;                                    // start skip block
  post->p       = p + post->bnum*sizeof(unsigned)*2;         // start posting block
  dids[0]       = INT_MAX; 
  post->ldid    = 0; post->did = -1;  
  post->didnum  = min(post->f_t,BLK_DIDNUM);											STAT(st_tot += post->f_t);
  if(post->f_t <= BLK_DIDNUM) post->bno=post->bnum;										
  return post->f_t;
}

// Get next docid. Return value >= INT_MAX at end of posting
static inline ALWAYS_INLINE unsigned postnext(post_t *post, unsigned *dids) { 
  if((post->did += dids[++post->didno] + 1) < INT_MAX) return post->did;

  unsigned char *p = post->bp;
  if(post->f_t > BLK_DIDNUM) {  
    if(++post->bno >= post->bnum) return INT_MAX;
    unsigned *pix = (unsigned *)p + post->bno; 													
    dids[0] = *pix; 				// first did in block
	p = post->p + pix[post->bnum];	// o=offset to posting block	    			
  } else dids[0] = vbget(p);
  
  post->didnum = min(post->_f_t, BLK_DIDNUM); 	
  post->_f_t  -= post->didnum;															//STAT(st_dec+=post->didnum);
  if(post->didnum > 1) { 																
      #if defined(USE_SIMDPACK)
    unsigned b = *p++; 
    if(post->didnum < 129) p = bitunpack32(p, post->didnum-1, b, &dids[1]); //p = vbdec(p, post->didnum-1,    &dids[1]);
    else {    p = simdunpackn( (unsigned *)p, post->didnum-1, b, &dids[1]); }
      #elif defined(USE_TURBOPFOR)
                       p = p4ddec32(   p, post->didnum-1,    &dids[1]);
      #else 
    unsigned b = *p++; p = bitunpack32(p, post->didnum-1, b,  &dids[1]); 
      #endif
  }
  dids[post->didnum] = INT_MAX; 
  post->didno        = 0; 
  return post->did   = dids[0]; 
}

// Get next docid equal or greater than the parameter did
static inline ALWAYS_INLINE unsigned postget(post_t *post, unsigned did, unsigned *dids) {
  if(did < post->ldid) {														// pending dids
    for(;;) {
      if(post->did >= did) break; post->did += dids[++post->didno]+1;
      if(post->did >= did) break; post->did += dids[++post->didno]+1;
      if(post->did >= did) break; post->did += dids[++post->didno]+1;
      if(post->did >= did) break; post->did += dids[++post->didno]+1;
    }																					
    if(post->did < INT_MAX) return post->did;
  } 	
																							
  unsigned char *p = post->bp;													//Skip index
  if(post->f_t > BLK_DIDNUM) {   					              
    unsigned *_q = (unsigned *)p,*q=_q+(++post->bno),*qe=_q+post->bnum-1;
    for(;;) {
      if(q[1] >= did || q >= qe) break; q++; 
      if(q[1] >= did || q >= qe) break; q++; 
      if(q[1] >= did || q >= qe) break; q++; 
      if(q[1] >= did || q >= qe) break; q++; 
    } 		
    post->bno = q - _q;  
    if(q < qe) {
      if(did < _q[0]) { post->bno=-1;post->ldid = _q[0]; return _q[0]; }
      post->ldid = q[1];
    } else { 
      post->ldid   = INT_MAX; 
      post->didnum = post->f_t - post->bno*BLK_DIDNUM;
      q = qe;    
    }
    post->bno  = q-_q;   																	
    dids[0]    = post->did = *q; 		            // first did in block 
    p          = post->p+q[post->bnum];	  			// o=offset to posting block  
  } else {																					
    post->ldid = INT_MAX; 
    dids[0]    = post->did = vbget(p); 																		
  }
  																						STAT(st_dec+=post->didnum);
  if(post->didnum > 1) {																	
      #if defined(USE_SIMDPACK)
    unsigned b = *p++; 
    if(post->didnum < 129) p = bitunpack32(p, post->didnum-1, b, &dids[1]);    //p = vbdec(p, post->didnum-1,    &dids[1]);
    else {    p = simdunpackn( (unsigned *)p, post->didnum-1, b, &dids[1]); }  
      #elif defined(USE_TURBOPFOR)
    p = p4ddec32(   p, post->didnum-1,    &dids[1]);
      #else
    unsigned b = *p++; p = bitunpack32(p, post->didnum-1, b, &dids[1]);
      #endif 
  }
  dids[post->didnum] = INT_MAX; 
  for(post->didno=0; ; ) {
    if(post->did >= did) break; post->did += dids[++post->didno]+1; 
    if(post->did >= did) break; post->did += dids[++post->didno]+1; 
    if(post->did >= did) break; post->did += dids[++post->didno]+1; 
    if(post->did >= did) break; post->did += dids[++post->didno]+1; 
  }												 
  return (post->did >= INT_MAX)?post->ldid:post->did;
}
//----------------------------------------- query search ------------------------------------------
#define TERMNUM 32

typedef struct {
  int term[TERMNUM], terms, id;
} qry_t;

int postcmp(post_t *a, post_t *b) {
  if(a->f_t < b->f_t) return -1;
  if(a->f_t > b->f_t) return  1;
  return 0;
}

int intersec_max;

unsigned qrysearch(qry_t *q, idxrd_t *idx) {
  int f_t = 0, i;
  post_t   *p, *pe, post[TERMNUM]; 
  unsigned did, elim, dids[TERMNUM][BLK_DIDNUM+31];
  
  if(q->terms == 1) { 															// 1 Term query											
    if(!(f_t = postinit(post, q->term[0], idx, dids[0]))) 
      return 0;																	
    for(i = 0; i < min(f_t,intersec_max); i++) { 
      if((did = postnext(post, dids[0])) >= INT_MAX) break; 
      f_t++;
    }
  } else if(q->terms == 2) {  													// optimized 2 terms query 
    if(!postinit(&post[0], q->term[0], idx, dids[0]) || !postinit(&post[1], q->term[1], idx, dids[1])) 
      return 0;
    if(post[1].f_t < post[0].f_t) { post_t t = post[0]; post[0] = post[1]; post[1] = t; } // swap
    for(elim=did=0,f_t=0;;) {   
      if(unlikely((did = postget(&post[0], did, dids[0])) >= INT_MAX)) break;
      if((        elim = postget(&post[1], did, dids[1])) == did) { 																	
        if(++f_t >= intersec_max) break;       																				
        did++;
        continue;
      } else if(elim >= INT_MAX) break; 
      did = elim;
    }
  } else {																	// multiple terms conjunctive query		
    pe = &post[q->terms];																		
    for(p = post; p < pe; p++)
      if(!postinit(p, q->term[p-post], idx, dids[p-post])) return 0;     
    qsort(post, q->terms, sizeof(post[0]), (int(*)(const void*,const void*))postcmp);   // sort by f_t
																													
    for(did = 0;;did++) {   
      a:if(unlikely((did = postget(post, did, dids[0])) >= INT_MAX)) return f_t;
      for(p = &post[1]; p < pe; p++) {     
        if((elim = postget(p, did, dids[p-post])) == did) continue;
        if(elim >= INT_MAX) return f_t; 												
        did = elim;
        goto a;
      }																					   		
	  if(++f_t >= intersec_max) break;      																							
   }
  }
  return f_t; 
}

//------------------------------ Test + Benchmark ----------------------------------------------------
#define QRYLEN 255
int qline, temin = 1,temax = TERMNUM,tex=0,qmax=1<<30;
unsigned long long   qrybatch(idxrd_t *idx, char *fqname, int *qid) {
  char               s[QRYLEN+1],*p,*q;
  int                id=0;
  unsigned long long f_t=0;
  FILE               *fq; 

  if(!(fq = fopen(fqname, "r+"))) 
    die("can't open file '%s'\n", fqname);

  while(fgets(s, QRYLEN, fq)) { ++qline; 
    s[strlen(s)-1]=0;
    qry_t qry;
    for(qry.terms=0,p=s; *p && qry.terms < TERMNUM; ) {   
      while(*p && (*p < '0' || *p > '9')) p++; if(!*p) break; 
      q = p; while(*p >= '0' && *p <= '9') p++;     
      qry.term[qry.terms++] = strtol(q, NULL, 10);
    }																								 														
    if(qry.terms >= temin && qry.terms <= temax) {  									//int j; for(j=0;j < qry.terms;j++) { if(j) printf(" "); printf("%u", qry.term[j]); }  printf(" %d \n", qry.terms);
      qry.id  = ++id;																	tex = max(qry.terms,tex);
      f_t += qrysearch(&qry, idx); 														if(id >= qmax) break;
    }
  }
  fclose(fq);
  *qid = id;
  return f_t;
}

void usage() {
  fprintf(stderr, "\nTurboPFor Copyright (c) 2013-2015 Powturbo  %s\n", __DATE__);
  fprintf(stderr, "https://github.com/powturbo/TurboPFor\n\n");
  fprintf(stderr, "Benchmark for intersections in inverted index\n\n");
  fprintf(stderr, "Usage: idxqry [options] <index file> <query file>\n");
  fprintf(stderr, "<options>\n");
  fprintf(stderr, " -nN     N = max. intersections/query. ex. -n1k=100.000 -n1m=1.000.000\n");
  fprintf(stderr, " -mN     N = minimum query terms (default 1)\n");
  fprintf(stderr, " -MN     N = maximum query terms (default 16)\n");
  fprintf(stderr, " -rN     N = number of iterations (default 3)\n");
  fprintf(stderr, " -qN     N = max. number of queries\n");
  fprintf(stderr, "<index file> index created by 'idxcr' program\n");
  fprintf(stderr, "Ex. idxqry -n100k -m2 clueweb.sorted.i aol.txt\n");
  fprintf(stderr, "Ex. idxqry gov2.sorted.i 1mq.txt\n");
  fprintf(stderr, "8-16 GB RAM recommended\n\n");
  exit(-1);
}

int main(int argc, char **argv ) {
  int reps = 3,i;

  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"", 	0, 0, 'r'}, {0,0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "n:m:M:q:r:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;								
      case 'q': qmax = atoi(optarg); break;
      case 'r': reps = atoi(optarg); break;
      case 'm': temin = atoi(optarg); break;
      case 'M': temax = atoi(optarg); break;
      case 'n': { char *p; intersec_max = strtol(optarg, &p, 10); if(*p == 'k' || *p == 'K') intersec_max *= 1000; else if(*p == 'm' || *p == 'M') intersec_max *= 1000000; } break;
       default:  usage();
    }
  }	  																			
  if(argc <= optind) usage();
  if(intersec_max) printf("Max. Intersections/query=%d\n", intersec_max);
  else intersec_max=1<<30;

  idxrd_t idx;  																			
  if(idxopen(&idx, argv[optind])) 
    die("can't open idx file '%s'\n", argv[optind]);
  for(i=0; i < reps; i++) {													STATINI; 
    int id; 																tm_t t0 = tminit();
    unsigned long long inum = qrybatch(&idx, argv[optind+1], &id );			tm_t t1 = tmtime()-t0;
  																			printf("qry=%d/%.2fs. [%.1f q/s] [%.3f ms/q] %llu docs found\n", id, tmsec(t1), (double)id/tmsec(t1), tmmsec(t1)/(double)id, inum );
                                                                            if(i<reps-1 && tmsec(t1) > 30) sleep(20);
  }
  idxclose(&idx);
                                                                              #ifdef STATS
                                                                            if(st_tot) printf("Terms=[%d-%d] Integers: total=%llu decoded=%llu ratio=%.2f%%\n", temin, tex, st_tot, st_dec, (double)st_dec*100/(double)st_tot);
                                                                              #endif 
}

