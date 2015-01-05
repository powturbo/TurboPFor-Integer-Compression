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

    idxcr.c - "Integer Compression" Create inverted index for using by idxqry for benchmarking
**/
#define _LARGEFILE64_SOURCE 1 
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
 
#include <getopt.h>
#include <sys/stat.h>

#include "vint.h"
#include "vp4dc.h"

#include "bitpack.h"
#include "idx.h"
//-------------------------------------- Simdcomp --------------------------------------------------------------------------
#include "ext/simdcomp/include/simdbitpacking.h"

unsigned char *simdpackwn(uint32_t *in, uint32_t n, uint32_t b, uint32_t *out) {
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpackwithoutmask(in, (__m128i *)out, b);
  return (unsigned char *)out;
}
unsigned char *simdpackwn1(uint32_t *in, uint32_t n, uint32_t b, uint32_t start, uint32_t *out) {
  uint32_t *in_;
  for(in_ = in + n; in + 128 <= in_; in += 128, out += 4 * b) simdpackwithoutmaskd1(start, in, (__m128i *)out, b);
  return (unsigned char *)out;
}
//---------------------------------------------------------------------------------------------------------------
#define DELTA( __in, __n, __b) { unsigned _v; for(__b=0,_v = __n-1; _v >   0; --_v) __in[_v] = (__in[_v] - __in[_v-1]) - 1, __b |= __in[_v]; __b = bsr32(__b); }

#define TERMNUM 2000000
int verb;

void usage() {
  fprintf(stderr, "\nTurboPFor Copyright (c) 2013-2015 Powturbo  %s\n", __DATE__);
  fprintf(stderr, "https://github.com/powturbo/TurboPFor\n\n");
  fprintf(stderr, "Create inverted index from 'Document identifier data set' format\n");
  fprintf(stderr, "See http://lemire.me/data/integercompression2014.html'\n");
  fprintf(stderr, "Usage: idxcr <docid file> <destination dir>\n");
  fprintf(stderr, "ex. idxcr clueweb09.sorted idxdir\n\n");
  exit(-1);
}

int main(int argc, char *argv[]) { 
  int fno,c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0; unsigned char *path="";
  static struct option long_options[] = { {"r", 	0, 0, 'r'}, {0,0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "xv:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;								
      case 'v': verb = atoi(optarg); 		break;
      default:  die("unknown option: %c \n", optopt);
    }
  }	
  if(argc - optind < 2) usage();
  tmap_t *tmap = malloc(TERMNUM*sizeof(tmap_t)); if(!tmap) die("malloc error\n"); 
  path = argv[--argc];

  for(fno = optind; fno < argc; fno++) { 
    char outname[257], *inname = argv[fno]; 
    strcpy(outname, path); 
    unsigned char *p = strrchr(inname,'/'); 
    if(!p) p = strrchr(inname,'\\'); if(!p) p=inname; 
    strcat(outname, p); strcat(outname,".i");

    FILE *fi = fopen64(inname, "rb");     if(!fi) { fprintf(stderr, "open error '%s'", inname);   perror(inname); exit(-1); }  int fdi = fileno(fi);
    FILE *fo = fopen64(outname,"wb"),*fm; if(!fo) { fprintf(stderr, "creat error '%s'", outname); perror(outname); exit(-1); }					fprintf(stderr, "file='%s'", outname); 
    fseeko(fo, sizeof(unsigned)+sizeof(unsigned long long), SEEK_SET);

    unsigned *in = NULL,*ip,*ep,num,tid=0,numx=0,outsize;
    unsigned char *out = NULL; 
    unsigned long long fofs; 																

    while(fread(&num, 1, 4, fi) == 4 && num) {				// read number of docid in term
      unsigned bnum = (num+BLK_DIDNUM-1)/BLK_DIDNUM; 
	  if(num > numx) { numx = num;  						
	    in  = realloc(in,  num*4+64); 
	    outsize = num*4+bnum*sizeof(unsigned)*2+1024; 
        out = realloc(out, outsize);
		if(!in || !out) die("malloc err=%u", num); 
	  }

      if(fread(in, 4, num, fi) != num) break;				// read docid list
      unsigned char *op = out,*_op; 
      vbput(op, num);										// store f_t

      unsigned *pix = (unsigned *)op; 
      if(num > BLK_DIDNUM) op += bnum*sizeof(unsigned)*2;
      for(_op = op, ip = in, ep = ip+num; ip < ep; ) {
        if(num > BLK_DIDNUM) {                             	// skip/index. docid[0] and offset to compressed block
          *pix = ip[0]; 									// First docid
          pix[bnum] = op-_op; 								// offset
          pix++;
        } else vbput(op, ip[0]);						   	// skip not needed

        unsigned n = min(ep-ip, BLK_DIDNUM),b=0;					        if(op+5*n > out+outsize) die("output buffer too small\n"); 
	    if(n > 1) {
          DELTA(ip, n, b);           
            #ifdef USE_SIMDPACK
          if(n < 129) { *op++ = b; op = bitpack32( ip+1, n-1, b, op); } 			//op = vbenc(ip+1, n-1, op); 
          else {        *op++ = b; op = simdpackwn(ip+1, n-1, b, (unsigned *)op); }
            #elif defined(USE_TURBOPFOR)
                     op = p4denc32( ip+1, n-1, op); 
            #else            
          *op++ = b; op = bitpack32(ip+1, n-1, b, op); 
            #endif
        }
		ip += n;	
      }
	  fofs 		= ftello(fo);
      tmap_t *t = &tmap[tid++]; 
      TIDMAPSET(t, fofs);
	  if(fwrite(out, 1, op-out, fo) < 0) die("fwrite error\n");
    }
    fofs = ftello(fo);															// write termmap
	if(fwrite(tmap, 1, tid*sizeof(tmap_t), fo) < 0) die("fwrite error\n");

    fseeko(fo, 0, SEEK_SET);
	if(fwrite(&fofs, 1, sizeof(unsigned long long), fo) < 0) die("fwrite error\n");
	if(fwrite(&tid,  1, sizeof(unsigned), fo) < 0) die("fwrite error\n");
   
	fclose(fi); fclose(fo);
	if(in) { free(in); free(out); }
  }
}
