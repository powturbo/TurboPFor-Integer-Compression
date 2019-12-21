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
//    idxcr.c - "Integer Compression" Create inverted index for using by idxqry for benchmarking
#define _LARGEFILE64_SOURCE 1
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <getopt.h>
#include <sys/stat.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

#include "conf.h"
#define VINT_IN
#include "vint.h"
#include "bitpack.h"
#include "vp4.h"
#include "idx.h"
#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#endif

#ifdef NSIMD
#define bitpack128v32 bitpack32
#endif

//---------------------------------------------------------------------------------------------------------------
#define DELTA( __in, __n, __b) do { unsigned _v; for(__b=0,_v = __n-1; _v >   0; --_v) __in[_v] = (__in[_v] - __in[_v-1]) - 1, __b |= __in[_v]; __b = bsr32(__b); } while(0)

#define TERMNUM 2000000
int verb;

void usage() {
  fprintf(stderr, "\nTurboPFor Copyright (c) 2013-2019 Powturbo  %s\n", __DATE__);
  fprintf(stderr, "https://github.com/powturbo/TurboPFor\n\n");
  fprintf(stderr, "Create inverted index from 'Document identifier data set' format\n");
  fprintf(stderr, "See http://lemire.me/data/integercompression2014.html'\n");
  fprintf(stderr, "Usage: idxcr <docid file> <destination dir>\n");
  fprintf(stderr, "ex. idxcr clueweb09.sorted idxdir\n\n");
  fprintf(stderr, "ex. index partitions generated from idxseg\n\n");
  fprintf(stderr, "ex. idxcr ./idxcr gov2.sorted.s* .\n\n");
  exit(-1);
}

int main(int argc, char *argv[]) {
  int fno,c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0; char *path="";
  static struct option long_options[] = { {"r",     0, 0, 'r'}, {0,0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "xv:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name);
                if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;
      case 'v': verb = atoi(optarg);        break;
      default:  die("unknown option: %c \n", optopt);
    }
  }
  if(argc - optind < 2) usage();
  path = argv[--argc];

  for(fno = optind; fno < argc; fno++) {
    char outname[257], *inname = argv[fno];
    strcpy(outname, path);
    char *p = strrchr(inname,'/');
    if(!p) p = strrchr(inname,'\\'); if(!p) p=inname;
    strcat(outname, p); strcat(outname,".i");

    FILE *fi = fopen(inname, "rb");     if(!fi) { fprintf(stderr, "open error '%s'", inname);   perror(inname); exit(-1); }  int fdi = fileno(fi);
    FILE *fo = fopen(outname,"wb"),*fm; if(!fo) { fprintf(stderr, "creat error '%s'", outname); perror(outname); exit(-1); } fprintf(stderr, "file='%s'", outname);
    fseeko(fo, sizeof(unsigned)+sizeof(unsigned long long), SEEK_SET);

    tmap_t *tmap = calloc(1, TERMNUM*sizeof(tmap_t));
    if(!tmap) die("malloc error\n");
    unsigned *in = NULL,*ip,*ep,num,tid=0,numx=0,outsize;
    unsigned char *out = NULL;
    unsigned long long fofs;                                                unsigned long long postsize=0,skipsize = 0;

    while(fread(&num, 1, 4, fi) == 4) {                                     // read number of docid in term
      if(!num) { ++tid; continue; }
      unsigned bnum = (num+BLK_DIDNUM-1)/BLK_DIDNUM;
      if(num > numx) {
        numx    = num;
        in      = realloc(in,  num*4+64);
        outsize = num*4+bnum*sizeof(unsigned)*SKIP_SIZE+1024;
        out     = realloc(out, outsize);
        if(!in || !out) die("malloc err=%u", num);
      }
      if(fread(in, 4, num, fi) != num) break;                               // read docid list

      unsigned char *op = out,*_op;
      vbput32(op, num);                                                     // store f_t

      unsigned *pix = (unsigned *)op;
      if(num > BLK_DIDNUM) {
        op += bnum*sizeof(unsigned)*SKIP_SIZE;                              skipsize += op-out;
      }
      for(_op = op, ip = in, ep = ip+num; ip < ep; ) {
        unsigned n = min(ep-ip, BLK_DIDNUM), b = 0,bx;                      if(op+5*n > out+outsize) die("output buffer too small\n");
        if(n > 1) {
            #ifndef _TURBOPFOR
          DELTA(ip, n, b); //bitdenc32( in+1, --n, pa, in[0], mode);
          //  #ifdef _TURBOPFOR
          //b = _p4bits32(ip+1, n-1, &bx);
            #endif
        }
            #ifdef SKIP_S
          unsigned u = ip[0]<<SKIP_S|b;                     // save width
            #else
          unsigned u = ip[0];                               // First docid
            #endif

        if(num > BLK_DIDNUM) {                              // skip/index. docid[0] and offset to compressed block
          *pix = u;
            #if SKIP_SIZE == 2
          pix[bnum] = op - _op;                             // save posting offset
            #endif
          pix++;
        } else vbput32(op, u);                              // skip not needed

        if(n > 1) {
            #ifdef _TURBOPFOR
          op = n==129?p4d1enc128v32(  ip+1, n-1, op, u):p4d1enc32(   ip+1, n-1, op, u);//*op++ = bx; op = n==129?_p4enc128v32(  ip+1, n-1, op, b, bx):_p4enc32(   ip+1, n-1, op, b, bx);
            #else
              #ifndef SKIP_S
          *op++ = b;
              #endif
          op = n==129?bitpack128v32( ip+1, n-1, op, b)    :bitpack32(ip+1, n-1, op, b);
            #endif
        }
        ip += n;
      }
      fofs      = ftello(fo);
      tmap_t *t = &tmap[tid++];
      TIDMAPSET(t, fofs);
      if(fwrite(out, 1, op-out, fo) != op-out) die("fwrite error\n");                   postsize += op-out;
    }
    fofs = ftello(fo);                                                          // write termmap
    if(fwrite(tmap, sizeof(tmap_t), tid, fo) != tid) die("fwrite error\n");

    fseeko(fo, 0, SEEK_SET);
    if(fwrite(&fofs, sizeof(unsigned long long), 1, fo) != 1) die("fwrite error\n");
    if(fwrite(&tid,  sizeof(unsigned), 1, fo) != 1) die("fwrite error\n");      fofs = ftello(fi);
    fclose(fi); fclose(fo);
    if(in) { free(in); free(out); }
    free(tmap);
                                                                                printf("\nterms=%u size=[tmap=%u skip=%llu post=%llu total=%llu, ratio=%.2f %%\n", tid, (unsigned)(tid*sizeof(tmap_t)),
                                                                                  skipsize, postsize-skipsize, postsize+tid*sizeof(tmap_t)+12, (double)postsize*100/(double)fofs );
  }
}
