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
//    idxseg.c - Inverted Index - Create partitions from DocId file for prallel query evaluation
#define _LARGEFILE64_SOURCE 1
#define _FILE_OFFSET_BITS 64
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif

#include <getopt.h>
#include "conf.h"

#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#define max(x,y) (((x)>(y)) ? (x) : (y))
#endif

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

void usage() {
  fprintf(stderr, "\nTurboPFor Copyright (c) 2013-2019 Powturbo  %s\n", __DATE__);
  fprintf(stderr, "Partitioning\n");
  fprintf(stderr, "Usage: idxseg -nNs -sPs <docid file> <destination dir>\n");
  fprintf(stderr, "Ns=total number of documents. Ps=number of partitions\n");
  fprintf(stderr, " s = modifier s:k,m,g=(1000,1 million,1 billion) s:K,M,G=(1024,1MB,1GB) ex. 64k or 64K\n");
  fprintf(stderr, "ex. ./idxseg gov2.sorted . -n26m -s8\n");
  fprintf(stderr, "ex. ./idxseg clueweb09.sorted . -n60m -s8\n\n");
  fprintf(stderr, "ex. parallel query evaluation\n\n");
  fprintf(stderr, "./idxqry ~/gov2.sorted.s*.i ~/aol.txt\n\n");
  exit(-1);
}

#define SEGMAX 64
int main(int argc, char *argv[]) { unsigned sb = 8,fno,n=25300000; char *path="";
  int c, digit_optind = 0, this_option_optind = optind ? optind : 1, option_index = 0;
  static struct option long_options[] = { {"r",     0, 0, 'r'}, {0,0, 0, 0}  };
  for(;;) {
    if((c = getopt_long(argc, argv, "s:n:", long_options, &option_index)) == -1) break;
    switch(c) {
      case  0 : printf("Option %s", long_options[option_index].name); if(optarg) printf (" with arg %s", optarg);  printf ("\n"); break;
      case 's': sb = atoi(optarg); break;
      case 'n': n  = argtoi(optarg); break;
      default:  usage();
    }
  }
  if(argc - optind < 2) usage();
  path = argv[--argc];
    #ifndef SPOW2
  sb = (n+sb-1) / sb;
    #endif
  for(fno = optind; fno < argc; fno++) {
    unsigned           snum = 0;
    unsigned long long inum=0;
    char               outname[257], *inname = argv[fno];
    strcpy(outname, path);
    char *p = strrchr(inname,'/'); if(!p) p = strrchr(inname,'\\'); if(!p) p=inname; strcat(outname, p); strcat(outname,".s");

    FILE *fi = fopen(inname, "rb"); if(!fi) { fprintf(stderr, "open error '%s'", inname); perror(inname); exit(-1); }
    FILE *fo[SEGMAX] = {0};
    unsigned as[SEGMAX] = {0}, an[SEGMAX] = {0},s;

    unsigned *in = NULL,*ip, num, numx = 0, tid = 0,didmax=0;
    while(fread(&num, 1, 4, fi) == 4) {                                     inum+=num;
      if(num > numx) {
        numx = num;
        if(!(in = realloc(in,  num*4+64))) die("malloc err=%u", num);
      }

      if(fread(in, 4, num, fi) != num) break;                               // read docid list
      for(ip = in; ip < in+num; ip++) {                                     if(*ip > didmax) didmax=*ip;
          #ifdef SPOW2
        s = (*ip) >> sb;
          #else
        s = (*ip) / sb;
          #endif
        snum = max(snum,s+1); as[s] = as[s]?as[s]:(ip - in); an[s]++;
      }

      for(s = 0; s < snum; s++) {
        FILE *f = fo[s];
        if(!f) {
          char oname[257]; sprintf(oname, "%s%.2d", outname, s);
          f = fopen(oname,"wb"); if(!f) { fprintf(stderr, "creat error '%s'", oname); perror(oname); exit(-1); }
          fo[s] = f;
          int i; for(i = 0; i < tid; i++) { unsigned z = 0; if(fwrite(&z, 1, 4, f) != 4) die("write error"); printf("#");fflush(stdout); }
        }
        unsigned n = an[s];
        if(fwrite(&n,            1, 4, f) != 4 || (n && fwrite(in+as[s], 4, n, f) != n)) die("write error");
        as[s] = an[s] = 0;
      }
      tid++;
    }
    for(s = 0; s < snum; s++) if(fo[s]) fclose(fo[s]); printf("didmax=%u num docid=%llu\n", didmax, inum);
    free(in);
  }
}
