/**
    Copyright (C) powturbo 2013-2019
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

    - homepage : https://sites.google.com/site/powturbo/
    - github   : https://github.com/powturbo
    - twitter  : https://twitter.com/powturbo
    - email    : powturbo [_AT_] gmail [_DOT_] com
**/
//  Inverted Index - query evaluation
#define _LARGEFILE64_SOURCE 1
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

 #ifdef _WIN32
#include <windows.h>
  #else
#include <sys/mman.h>
  #endif
#include <getopt.h>

#include "conf.h"
#define VINT_IN
#include "vint.h"
#include "bitpack.h"
#include "vp4.h"
#include "idx.h"
#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#define max(x,y) (((x)>(y)) ? (x) : (y))
#endif

#ifdef NSIMD
#define bitd1unpack128v32 bitd1unpack32
#define _p4d1dec128v32    _p4d1dec32
#endif

//#define STATS
//------------------------------------- index file (created by idxcr) -------------------------------------------------------------
typedef struct {
  unsigned char      *fdp,  // posting
                     *fdm;  // mapping term id to offset in posting
  unsigned long long fdsize;
  unsigned           tnum;
    #ifdef _WIN32
  HANDLE hd;
    #endif
} idxrd_t;                  // Index

int idxopen(idxrd_t *idx, char *s) {
  unsigned char *p;

    #ifdef _WIN32
  HANDLE fd;
  if((fd = CreateFileA( s, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, 0 ))==INVALID_HANDLE_VALUE)
    die("can't open index file '%s' rc=%d\n", s, GetLastError());

  struct stat sbuf;  fstat((intptr_t)fd, &sbuf);
  ULARGE_INTEGER ul; ul.QuadPart = sbuf.st_size;
  if(!(idx->hd = CreateFileMapping(fd, NULL, PAGE_READONLY, ul.HighPart, ul.LowPart, NULL)))
    die("CreateFileMapping failed or file not found.rc=%d \n", GetLastError());
  ul.QuadPart = 0;
  if(!(p = MapViewOfFile(idx->hd, FILE_MAP_READ, ul.HighPart, ul.LowPart, sbuf.st_size)))
    die("MapViewOfFile failed.rc=%d\n", GetLastError());
  CloseHandle( fd );
    #else
  int fd;
  if((fd = open(s, O_RDONLY/*| O_LARGEFILE*/)) < 0)
    die("can't open index file '%s' rc=%d:%s\n", s, errno, strerror(errno));

  struct stat sbuf;  fstat((int)fd, &sbuf);
  if(sbuf.st_size > 0 && (p = mmap( NULL, sbuf.st_size , PROT_READ, MAP_SHARED|MAP_NORESERVE, fd, 0)) == (void *)-1)
    die("mmap errno=%d,'%s'\n", errno, strerror(errno) );
  close(fd);
    #endif

  idx->fdsize = sbuf.st_size;
  idx->fdp    = p;
  idx->fdm    = p + *(uint64_t *)p; p += sizeof(uint64_t);  // Termid map table. Termid->Posting
  idx->tnum   = *(unsigned *)p;
  return 0;
}

void idxclose(idxrd_t *idx) {
    #ifdef _WIN32
  UnmapViewOfFile(idx->fdp);
  CloseHandle(idx->hd);
    #else
  munmap(idx->fdp, idx->fdsize);
    #endif
}

//--------------------------------- Posting --------------------------------------------------------------
  #ifdef STATS
unsigned long long st_tot,st_dec,st_did,st_blk,st_ovl,st_noovl,st_skip,st_noi,st_terms,st_dids[8],st_decs[8],st_tots[8];
#define STATINI st_tot=st_dec=st_did=st_blk=st_ovl=st_noovl=st_skip=st_noi=st_terms=0;{int i; for(i=0;i<8;i++) st_dids[i]=st_decs[i]=st_tots[i]=0;}
#define STAT(a) a
  #else
#define STATINI
#define STAT(a)
  #endif

typedef struct {
  unsigned char *bp,*p;
  unsigned      f_t,_f_t, did,ldid;
  int           didno,didnum, bno, bnum;
    #if SKIP_SIZE == 1
  unsigned long long pofs;
    #endif
} post_t;

// Init posting for term id tid
int postinit( post_t *v, int tid, idxrd_t *idx, unsigned *dids) {
  unsigned long long o = TIDMAP(idx->fdm, tid); if(!o) return 0;
  unsigned char *p = idx->fdp + o;                      // start of posting;
  vbget32(p, v->f_t);                               // num docs
  v->didno   = v->bno = -1;
  v->bnum    = (v->f_t+BLK_DIDNUM-1)/BLK_DIDNUM;        // num blocks
  v->_f_t    = v->f_t;

  v->bp      = p;                                       // start skip block
  v->p       = p + v->bnum*sizeof(unsigned)*2;         // start posting block
  dids[0]    = INT_MAX;
  v->ldid    = v->did = 0;
  v->didnum  = min(v->f_t,BLK_DIDNUM);                                          STAT(if(v->f_t>BLK_DIDNUM) st_tot += v->f_t);STAT(if(v->f_t>BLK_DIDNUM) st_tots[st_terms] += v->f_t);
    #if SKIP_SIZE == 1
  v->pofs = 0;
    #endif
  return v->f_t;
}

static ALWAYS_INLINE unsigned postdec(post_t *v, int bno, unsigned *dids) {     if(v->didno == bno) die("Fatal postdec");
  unsigned char *p = v->bp;
  if(v->f_t > BLK_DIDNUM) {                                                     if(bno < 0 || bno >= v->bnum) die("Fatal bno\n");
    unsigned *pix = (unsigned *)p + bno;
                p = v->p + pix[v->bnum];        // o=offset to posting block
          dids[0] = *pix;                       // first did in block
    v->didnum = bno < v->bnum-1?BLK_DIDNUM:v->f_t - bno*BLK_DIDNUM;
  } else { v->didnum = v->f_t;  vbget32(p, dids[0]); }                          STAT(st_dec += v->didnum); STAT(st_decs[st_terms] += v->didnum);
    #ifdef SKIP_S
  unsigned b = dids[0] & SKIP_M; dids[0] >>= SKIP_S;
    #endif

  if(v->didnum > 1) {
      #ifdef _TURBOPFOR
    //unsigned bx = *p++; p = v->didnum == 129?_p4d1dec128v32(    p, 128, &dids[1], dids[0], b, bx):_p4d1dec32(   p, v->didnum-1, &dids[1], dids[0], b, bx);
    p = v->didnum == 129?p4d1dec128v32(    p, 128, &dids[1], dids[0]):p4d1dec32(   p, v->didnum-1, &dids[1], dids[0]);
      #else
        #ifndef SKIP_S
    unsigned b = *p++;
        #endif
    p = v->didnum == 129?bitd1unpack128v32( p, 128, &dids[1], dids[0], b    ):bitd1unpack32(p, v->didnum-1, &dids[1], dids[0], b);
      #endif
  }
  v->didno        = bno;
  dids[v->didnum] = INT_MAX;
  return v->didnum;
}

  #ifdef SKIP_S
#define QS(__x) ((__x)>>SKIP_S)
  #else
#define QS(__x) (__x)
  #endif

  #if SKIP_SIZE == 1
    #ifdef _TURBOPFOR
#error "implicit skip not implemented for turbopfor"
    #else
#define COFS(__ofs,__x) __ofs += (((__x)&SKIP_M)+7/8) // implicit skips: calculate the offset of the next block
    #endif
  #else
#define COFS(__ofs,__x)
  #endif

// Get next docid. Return value >= INT_MAX at end of posting
static ALWAYS_INLINE unsigned postnext(post_t *v, unsigned *dids) {
  if((v->did = dids[++v->didno]) < INT_MAX) return v->did;
  unsigned char *p = v->bp;

  if(v->f_t > BLK_DIDNUM) {
    if(++v->bno >= v->bnum) return INT_MAX;
    unsigned *pix = (unsigned *)p + v->bno;
                p = v->p + pix[v->bnum];        // o=offset to posting block
         dids[0] = *pix;                        // first did in block
  } else vbget32(p, dids[0]);
    #ifdef SKIP_S
  unsigned b = dids[0] & SKIP_M; dids[0] >>= SKIP_S;
    #endif

  v->didnum  = min(v->_f_t, BLK_DIDNUM);
  v->_f_t   -= v->didnum;                                                           //STAT(st_dec+=v->didnum);
  if(v->didnum > 1) {
      #ifdef _TURBOPFOR
    p = v->didnum == 129?p4d1dec128v32(     p, 128, &dids[1], dids[0]   ):p4d1dec32(    p, v->didnum-1, &dids[1], dids[0]);//unsigned bx = *p++;    p = v->didnum == 129?_p4d1dec128v32(    p, 128, &dids[1], dids[0], b, bx):_p4d1dec32(   p, v->didnum-1, &dids[1], dids[0], b, bx);
      #else
        #ifndef SKIP_S
    unsigned b = *p++;
        #endif
    p = v->didnum == 129?bitd1unpack128v32( p, 128, &dids[1], dids[0], b):bitd1unpack32(p, v->didnum-1, &dids[1], dids[0], b);
      #endif
  }
  dids[v->didnum] = INT_MAX;
  v->didno        = 0;
  return v->did   = dids[0];
}

#define DD if(v->did >= did) break; v->did = dids[++v->didno]
#define DS if(QS(q[1]) >= did || q >= qe) break; COFS(v->pofs, *q); q++

// Get next docid equal or greater than the parameter did
static ALWAYS_INLINE unsigned postget(post_t *v, unsigned did, unsigned *dids) {
  if(did >= v->ldid) goto b;
  for(;;) { a: DD;DD;DD;DD; DD;DD;DD;DD; }
  if(v->did < v->ldid) {                                                        STAT(st_did += (v->did<INT_MAX)); STAT(st_dids[st_terms] += (v->did<INT_MAX));
    return v->did;
  }
  b:; unsigned char *p;                         // Skip index
  if(v->f_t > BLK_DIDNUM) {
    unsigned *_q = (unsigned *)v->bp, *q=_q+(++v->bno), *qe=_q+v->bnum-1;
    for(;;) {  DS;DS;DS;DS; DS;DS;DS;DS; }
    v->bno = q - _q;
    if(q < qe) v->ldid = QS(q[1]);
    else {
      v->ldid   = UINT_MAX;
      v->didnum = v->f_t - v->bno*BLK_DIDNUM;
    }
    dids[0] = v->did = QS(*q);                  // first did in block
      #if SKIP_SIZE == 1
    p = v->p+v->pofs;                           // o=offset to posting block
      #else
    p = v->p+q[v->bnum];                        // o=offset to posting block
      #endif
  } else {
    p       = v->bp;
    vbget32(p, v->did);
    v->ldid = UINT_MAX;
  }
    #ifdef SKIP_S
  unsigned b = v->did&SKIP_M; v->did >>= SKIP_S;
    #endif
  dids[0]    = v->did;
                                                                                STAT(st_dec+=v->didnum); STAT(st_decs[st_terms] += v->didnum);
  if(v->didnum > 1) {                                                           STAT(st_blk++);
      #ifdef _TURBOPFOR
    p = v->didnum == 129?p4d1dec128v32(     p, 128, &dids[1], dids[0]   ):p4d1dec32(    p, v->didnum-1, &dids[1], dids[0]);//unsigned bx = *p++; p = v->didnum == 129?_p4d1dec128v32(    p, 128, &dids[1], dids[0], b, bx):_p4d1dec32(   p, v->didnum-1, &dids[1], dids[0], b, bx);
      #else
        #ifndef SKIP_S
    unsigned b = *p++;
        #endif
    p = v->didnum == 129?bitd1unpack128v32( p, 128, &dids[1], dids[0], b):bitd1unpack32(p, v->didnum-1, &dids[1], dids[0], b);
      #endif
  }
  dids[v->didnum] = v->ldid&INT_MAX; v->didno = 0; goto a;
}

/********************************************* query search *************************************************************************************/
//#define THREAD_MAX 32  //uncomment for parallel processing.
#define SKIP_INTERVALS

  #ifdef THREAD_MAX
#define QRYFIFOMAX (1<<14)
//--------------- thread/fifo -------------------------
#include <pthread.h>
typedef void *threadfunc_t;
typedef void *threadfuncarg_t;
typedef pthread_t thread_t;
typedef pthread_mutex_t thread_mutex_t;
#define thread_mutex_init(__mutex)    pthread_mutex_init(__mutex, NULL)
#define thread_mutex_lock(__mutex)    pthread_mutex_lock(__mutex)
#define thread_mutex_unlock(__mutex)  pthread_mutex_unlock(__mutex)
#define thread_mutex_destroy(__mutex) pthread_mutex_destroy(__mutex)
#define thread_join(__th) pthread_join(__th, NULL);

thread_t thread_create(threadfunc_t (*thread_func)(threadfuncarg_t), threadfuncarg_t arg) {
  thread_t th;
  int rc;
  rc = pthread_create(&th, NULL, thread_func, arg);
  return rc?-1:th;
}

#define _FIFOPUT(__fifo, __obj) do {           (__fifo)->buf[(++(__fifo)->tail) & ((__fifo)->size-1)] = (__obj); } while(0)
#define _FIFOGET(__fifo, __obj) do { (__obj) = (__fifo)->buf[(++(__fifo)->head) & ((__fifo)->size-1)];           } while(0)
#define FIFOEMPTY(__fifo) ((__fifo)->head == (__fifo)->tail)
#define FIFOFULL(__fifo)  ((__fifo)->tail == (__fifo)->head + (__fifo)->size)

#define FIFO_STRUCT_I pthread_mutex_t mutex; pthread_cond_t nofull, noempty;

#define FIFO_INIT_I(__fifo, __fifosize) \
  pthread_mutex_init(&(__fifo)->mutex, NULL);\
  pthread_cond_init(&(__fifo)->nofull, NULL);\
  pthread_cond_init(&(__fifo)->noempty, NULL);

#define FIFO_EXIT_I(__fifo) pthread_mutex_destroy(&(__fifo)->mutex); pthread_cond_destroy(&(__fifo)->nofull);   pthread_cond_destroy(&(__fifo)->noempty);

#define FIFOGET(__fifo, __obj) do {\
  pthread_mutex_lock(&(__fifo)->mutex);\
  while(FIFOEMPTY(__fifo)) pthread_cond_wait(&(__fifo)->noempty, &(__fifo)->mutex); _FIFOGET(__fifo, __obj);\
  pthread_mutex_unlock(&(__fifo)->mutex); pthread_cond_signal(&(__fifo)->nofull);\
} while(0)

#define FIFOPUT(__fifo, __obj) do {\
  pthread_mutex_lock(&(__fifo)->mutex);\
  while(FIFOFULL(__fifo)) pthread_cond_wait(&(__fifo)->nofull, &(__fifo)->mutex); _FIFOPUT(__fifo, __obj);\
  pthread_mutex_unlock(&(__fifo)->mutex); pthread_cond_signal(&(__fifo)->noempty);\
} while(0)

#define FIFO_STRUCT(__sfifo, __fifodata, __fifotype, __fifosize) \
struct __sfifo { unsigned size, head, tail; FIFO_STRUCT_I; __fifodata; __fifotype buf[__fifosize]; }

#define FIFOINIT(__fifo, __fifosize) do { memset(__fifo, 0, sizeof( typeof((__fifo)[0]))); (__fifo)->size = __fifosize; FIFO_INIT_I(__fifo, __fifosize); } while(0)
#define FIFOEXIT(__fifo) do { FIFO_EXIT_I(__fifo); } while(0)
//-----------------------------------------------------------
FIFO_STRUCT(ofifo, int sd,                                   struct rsp *, QRYFIFOMAX);
FIFO_STRUCT(ififo, int dno; int dnonum; struct ofifo *ofifo, struct qry *, QRYFIFOMAX);
  #endif

#define TERMNUM 32
typedef struct qry {
    #ifdef THREAD_MAX
  idxrd_t        *idx;
  unsigned        id,dno,results;
  thread_mutex_t  mutex;
    #endif
  int             term[TERMNUM], terms;
} qry_t;

int postcmp(post_t *a, post_t *b) {
  if(a->f_t < b->f_t) return -1;
  if(a->f_t > b->f_t) return  1;
  return 0;
}

int intersec_max;                                                               unsigned long long st_f_t;
#define IY(_x,_y) if(*++_y >= *_x) break
#define IX(_x,_y) if(*++_x >= *_y) break
#define INTERSECT(__x,__x_, __y,__y_) { __label__ icmp,end;\
  if (__x < __x_ && __y < __y_) \
  for(;;) {\
    if(*__y < *__x) { icmp: for(;;) { IY(__x,__y); IY(__x,__y); IY(__x,__y); IY(__x,__y); IY(__x,__y); IY(__x,__y); } if(__y >= __y_) goto end; }\
    if(*__x < *__y) {       for(;;) { IX(__x,__y); IX(__x,__y); IX(__x,__y); IX(__x,__y); IX(__x,__y); IX(__x,__y); } if(__x >= __x_) goto end; }\
    if(*__x == *__y) { _r++; if(++__x >= __x_ || ++__y >= __y_) break; } else goto icmp;\
  } end:;\
}

unsigned qrysearch(qry_t *q, idxrd_t *idx) {
  int       f_t = 0, i, intersec_mx = intersec_max;
  post_t   *p, *pe, v[TERMNUM];
  unsigned  did, elim, dids[TERMNUM][BLK_DIDNUM+31];                            STAT(st_terms = q->terms>=8?7:q->terms);

  if(q->terms == 1) {                                                           // 1 Term query
    if(!(f_t = postinit(v, q->term[0], idx, dids[0])))
      return 0;
    for(i = 0; i < min(f_t,intersec_mx); i++) {
      if((did = postnext(v, dids[0])) >= INT_MAX) break;
      f_t++;
    }
  } else if(q->terms == 2) {                                                    // optimized 2 terms query
    if(!postinit(&v[0], q->term[0], idx, dids[0]) || !postinit(&v[1], q->term[1], idx, dids[1]))
      return 0;
    if(v[1].f_t < v[0].f_t) { post_t t = v[0]; v[0] = v[1]; v[1] = t; } // swap
      #ifdef SKIP_INTERVALS
    unsigned *_xd = dids[0], xdnum;
    unsigned *_yd = dids[1], ydnum;
    if(v[0].f_t > BLK_DIDNUM) {
      unsigned *_x = (unsigned *)v[0].bp, *x_ = _x+v[0].bnum, *x = _x, *xd;
      unsigned *_y = (unsigned *)v[1].bp, *y_ = _y+v[1].bnum, *y = _y, *yd;
      _xd[0] = _yd[0] = UINT_MAX;
      for(;;) { unsigned x0 = x[0] == _xd[0], y0 = y[0] == _yd[0];
        if((x0?xd[0]:x[0]+1) <= y[1] && (y0?yd[0]+1:y[0]) <= x[1]) {
          if(!x0) xdnum = postdec(&v[0], x - _x, xd = _xd);
          if(!y0) ydnum = postdec(&v[1], y - _y, yd = _yd);
          unsigned _r = 0;
          INTERSECT(xd, _xd+xdnum, yd, _yd+ydnum);
          f_t += _r;                    STAT(_r?st_ovl++:st_noovl++);
        }
             if(y[1] < x[1]) { if(++y >= y_) break; }
        else if(x[1] < y[1]) { if(++x >= x_) break; }
        else if(++x == x_ || ++y == y_) break;                                  STAT(st_skip++);
      }
    } else if(v[1].f_t <= BLK_DIDNUM) {
      xdnum = postdec(&v[0], 0, _xd);
      ydnum = postdec(&v[1], 0, _yd);
      unsigned _r = 0,*xd_=_xd+xdnum,*yd_=_yd+ydnum;
      INTERSECT(_xd, xd_, _yd, yd_); f_t += _r;                                 STAT(_r?st_ovl++:st_noovl++);
    } else
      #endif
    for(did = 0;;) {
      if(unlikely((did = postget(&v[0], did, dids[0])) >= INT_MAX)) break;
      if((        elim = postget(&v[1], did, dids[1])) == did) {
        if(++f_t >= intersec_mx) break;
        did++;                          // top-k: doc scoring + heap insertmin
        continue;
      } else if(elim >= INT_MAX) break;
      did = elim;
    }
  } else {                                                                      // multiple terms conjunctive query
    pe = &v[q->terms];
    for(p = v; p < pe; p++)
      if(!postinit(p, q->term[p-v], idx, dids[p-v]))
        return 0;
    qsort(v, q->terms, sizeof(v[0]), (int(*)(const void*,const void*))postcmp); // sort by f_t

    for(did = 0;;did++) { a:
      if(unlikely((did = postget(v, did, dids[0])) >= INT_MAX))
        return f_t;
      for(p = &v[1]; p < pe; p++) {
        if((elim = postget(p, did, dids[p-v])) == did) continue;
        if(elim >= INT_MAX)
          return f_t;
        did = elim;
        goto a;
      }                                                                         // top-k: doc scoring + heap insertmin
      if(++f_t >= intersec_mx) break;
    }
  }
  return f_t;
}

  #ifdef THREAD_MAX
qry_t *qrynew() {
  qry_t *qry = calloc(1,sizeof(qry_t)); if(!qry) die("malloc failed\n");
  return qry;
}

void qrydestroy(qry_t *qry) { free(qry); }
//------------------------------------------------
typedef struct rsp {
  qry_t     *qry;
  unsigned   dno,dnonum,results;
} rsp_t;

rsp_t *rspnew() {
  rsp_t *rsp = calloc(1,sizeof(rsp_t)); if(!rsp) die("malloc failed\n");
  return rsp;
}

void rspdestroy(rsp_t *rsp) { free(rsp); }

//-----------------------------------------------
void thrqryini() {}
void thrqry_exit() {}

void *thrqrysearch( struct ififo *ififo ) {
  struct ofifo *ofifo = ififo->ofifo;
  for(;;) {
    struct qry *qry;
    FIFOGET(ififo, qry); if(!qry) return NULL;
    rsp_t *rsp          = rspnew();
    rsp->qry            = qry;
    rsp->dno            = ififo->dno;
    rsp->dnonum         = ififo->dnonum;
    rsp->results        = qrysearch(qry, &qry->idx[ififo->dno]);
    FIFOPUT(ofifo, rsp);
  }
}

void *thrqryout( struct ofifo *ofifo ) {
  for(;;) {
    rsp_t *rsp;
    FIFOGET(ofifo, rsp); if(!rsp) return NULL;
    qry_t *qry = rsp->qry;
    unsigned r = rsp->results;
    rspdestroy(rsp);                    //thread_mutex_lock(&qry->mutex);
    qry->results += r;
    if(++qry->dno == rsp->dnonum) {
      st_f_t += qry->results;
      qrydestroy(qry);
    }
  }
}
  #endif

//------------------------------ Test + Benchmark ----------------------------------------------------
#define QRYLEN 255
int qline, temin = 1,temax = TERMNUM,tem=32, tex=0, qmax=1<<30;

int qrybatch(idxrd_t *idx, char *fqname
     #ifdef THREAD_MAX
  ,struct ififo *ififos, int dnonum
     #endif
) {
  char  s[QRYLEN+1],*p,*q;
  int   id=0;
  FILE  *fq;

  if(!(fq = fopen(fqname, "r+")))
    die("can't open file '%s'\n", fqname);

  while(fgets(s, QRYLEN, fq)) { ++qline;
    s[strlen(s)-1]=0;
      #ifdef THREAD_MAX
    qry_t *qry = qrynew();
      #else
    qry_t _qry,*qry = &_qry;
      #endif
    for(qry->terms=0,p=s; *p && qry->terms < TERMNUM; ) {
      while(*p && (*p < '0' || *p > '9')) p++; if(!*p) break;
      q = p; while(*p >= '0' && *p <= '9') p++;
      qry->term[qry->terms++] = strtol(q, NULL, 10);
    }
    if(qry->terms >= temin && qry->terms <= temax) {  ++id;                         tex = max(qry->terms,tex);tem = min(qry->terms,tem);
        #ifdef THREAD_MAX
      qry->id     = id;
      qry->idx    = idx;
      thread_mutex_init(&qry->mutex);
      struct ififo *ififo;
      for(ififo = ififos; ififo < ififos+dnonum; ififo++)
        FIFOPUT(ififo, qry);
        #else
      st_f_t += qrysearch(qry, idx);
        #endif
      if(id >= qmax) break;
    }
      #ifdef THREAD_MAX
    else qrydestroy(qry);
      #endif
  }
  fclose(fq);
  return id;
}

void usage() {
  fprintf(stderr, "\nTurboPFor Copyright (c) 2013-2019 Powturbo  %s\n", __DATE__);
  fprintf(stderr, "https://github.com/powturbo/TurboPFor\n\n");
    #ifdef THREAD_MAX
  fprintf(stderr, "Benchmark: parallel intersections in compressed inverted index\n\n");
    #else
  fprintf(stderr, "Benchmark: compressed intersections in inverted index\n\n");
    #endif
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

//---------------------------------------- Time ---------------------------------------------------------------------
typedef unsigned long long tm_t;
#define TM_T 1000000.0

  #ifdef _WIN32
#include <windows.h>
static LARGE_INTEGER tps;
static tm_t tmtime(void) { LARGE_INTEGER tm; QueryPerformanceCounter(&tm); return (tm_t)(tm.QuadPart*1000000/tps.QuadPart); }
static tm_t tminit() { QueryPerformanceFrequency(&tps); tm_t t0=tmtime(),ts; while((ts = tmtime())==t0); return ts; }
  #else
#include <time.h>
//static   tm_t tmtime(void)    { struct timeval tm; gettimeofday(&tm, NULL); return (tm_t)tm.tv_sec*1000000ull + tm.tv_usec; }
static   tm_t tmtime(void)    { struct timespec tm; clock_gettime(CLOCK_MONOTONIC, &tm); return (tm_t)tm.tv_sec*1000000ull + tm.tv_nsec/1000; }
static   tm_t tminit()        { tm_t t0=tmtime(),ts; while((ts = tmtime())==t0) {}; return ts; }
  #endif
static double tmsec( tm_t tm) { return (double)tm/1000000.0; }
static double tmmsec(tm_t tm) { return (double)tm/1000.0; }
//--------------
unsigned argtoi(char *s) {
  char *p; unsigned n = strtol(s, &p, 10),f=1;
  switch(*p) {
     case 'k': f = 1000;       break;
     case 'm': f = 1000000;    break;
     case 'g': f = 1000000000; break;
     case 'K': f = 1<<10;      break;
     case 'M': f = 1<<20;      break;
     case 'G': f = 1<<30;      break;
  }
  return n*f;
}

int main(int argc, char **argv ) {
  int reps = 3,r;
  tminit();
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"",  0, 0, 'r'}, {0,0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "n:m:M:q:r:s:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name);
                if(optarg) printf (" with arg %s", optarg); printf ("\n"); break;
      case 'q': qmax = atoi(optarg);           break;
      case 'r': reps = atoi(optarg);           break;
      case 'm': temin = argtoi(optarg);        break;
      case 'M': temax = argtoi(optarg);        break;
      case 'n': intersec_max = argtoi(optarg); break;
       default:  usage();
    }
  }
  if(argc <= optind) usage();
  char *fqname = argv[--argc];
  if(intersec_max) printf("Max. Intersections/query=%d\n", intersec_max);
  else intersec_max=1<<30;

  idxrd_t idx[64];
  int fno, dnonum = 0;
  for(fno = optind; fno < argc; fno++) {                                    printf("%s\n", argv[fno]);
    if(idxopen(&idx[dnonum++], argv[fno]))
      die("can't open idx file '%s'\n", argv[optind]);
      #ifndef THREAD_MAX
    break;
      #endif
  }

  for(r=0; r < reps; r++) {                                                 STATINI; printf("#");fflush(stdout);
      #ifdef THREAD_MAX
    thrqryini();
    thread_t ithreads[THREAD_MAX]; int i;                                   printf("!");fflush(stdout);

    struct ofifo _ofifo,*ofifo = &_ofifo;
    FIFOINIT(ofifo, QRYFIFOMAX);
    thread_t othread;
    if(!(othread = thread_create((void *)thrqryout, ofifo))) die("Error thread_create.\n");

    struct ififo _ififo[THREAD_MAX],*ififo;
    for(i = 0; i < THREAD_MAX; i++) _ififo[i].dno = -1;
    for(i = 0; i < dnonum; i++) {
      ififo = &_ififo[i];
      FIFOINIT(ififo, QRYFIFOMAX);
      ififo->dno    = i;
      ififo->dnonum = dnonum;
      ififo->ofifo  = ofifo;
      if(!(ithreads[i] = thread_create((void *)thrqrysearch, ififo)))
        die("Error thread_create.\n");
    }
      #endif
                                                                            st_f_t = 0; tm_t t0 = tminit();
    int id = qrybatch(idx, fqname
      #ifdef THREAD_MAX
    , _ififo, dnonum
      #endif
    );
      #ifdef THREAD_MAX
    end:for(i = 0; i < dnonum; i++) { ififo = &_ififo[i]; FIFOPUT(ififo, NULL); }
    for(i = 0; i < dnonum; i++) thread_join(ithreads[i]);
    for(i = 0; i < dnonum; i++) { ififo = &_ififo[i]; FIFOEXIT(ififo); }
    FIFOPUT(ofifo, NULL);
    thread_join(othread);
    FIFOEXIT(ofifo);
    thrqry_exit();
      #endif
                                                                            tm_t t1 = tmtime()-t0;
                                                                            printf("qry=%d/%.2fs. [%.1f q/s] [%.3f ms/q].%llu\n", id, tmsec(t1), (double)id/tmsec(t1), tmmsec(t1)/(double)id, st_f_t );
                                                                            if(r<reps-1) sleep(tmsec(t1) > 20?20:5);
  }
  for(r = 0; r < dnonum; r++) idxclose(&idx[r]);
                                                                              #ifdef STATS
                                                                            if(st_tot) { printf("Terms=[%d-%d] Integers: total=%llu decompressed=%llu ratio=%.2f%%. %lld docs found\n", tem, tex, st_tot, st_dec, (double)st_dec*100/(double)st_tot, st_f_t);
                                                                              int i; printf("Ratio:");for(i=2;i<8;i++) if(st_tots[i]) printf("%d:%.2f ", i, (double)st_decs[i]*100/(double)st_tots[i]);
                                                                            }
                                                                            printf("ovl=%llu,novl=%llu,skip=%lld stno=%lld\n", st_ovl, st_noovl, st_skip, st_noi);
                                                                              #endif
}
