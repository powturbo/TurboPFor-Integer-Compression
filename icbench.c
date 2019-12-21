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
//      IcBench: main program
#define _CRT_SECURE_NO_WARNINGS
#define _GNU_SOURCE
#define _LARGEFILE64_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <float.h>
#include <errno.h>
#include <sys/types.h>
#include <ctype.h>
#include <math.h>
#ifdef __APPLE__
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif
  #ifndef _WIN32
#include <sys/resource.h>
  #endif
  #ifdef _MSC_VER
#define PRId64       "I64d"
#include "vs/getopt.h"
  #else
#include <inttypes.h>
#include <getopt.h>
#include <unistd.h>
  #endif

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

#include <time.h>
#include "conf.h"
#define VINT_IN
#include "vint.h"
#include "bitpack.h"
#include "vsimple.h"
#include "eliasfano.h"
#include "vp4.h"
#include "plugins.h"

//#define RDTSC_ON
#define TM_F 4
#include "time_.h"

#ifndef min
#define min(x,y) (((x)<(y)) ? (x) : (y))
#endif

int strpref(char **str, int n, char sep1, char sep2) {
  int i, j=0;
  for(;;j++)
    for(i = 0; i < n; i++)
      if(!str[i][j] || str[i][j] != str[0][j]) {
        while (j > 0 && str[0][j-1] != sep1 && str[0][j-1] != sep2) j--;
        return j;
      }
  return 0;
}

int memcheck(unsigned char *_in, unsigned _n, unsigned char *_cpy, int cmp) {
  unsigned *in = (unsigned *)_in,*cpy=_cpy,n = (_n+3)/4;
  int i;
  if(cmp <= 1)
    return 0;
  for(i = 0; i < n; i++)
    if(in[i] != cpy[i]) {
      if(cmp > 4) abort(); // crash (AFL) fuzzing
      printf("ERROR in[%d]=%x,%d dec[%d]=%x,%d\n", i, in[i], bsr64(in[i]), i, cpy[i], bsr64(cpy[i]) );
      if(cmp>3) {
        int j;
        for(j=i & 0xffffff80u; j < i+128;j++) { unsigned e = in[j] != cpy[j];
          if(e) printf("#%d:%x,%x ", j, in[j], cpy[j]);else printf("%d:%x ", j, in[j]);
        }
        printf("\n");
      }
      if(cmp > 2) exit(EXIT_FAILURE);
      return i+1;
    }
  return 0;
}

int memcheck64(unsigned char *_in, unsigned _n, unsigned char *_cpy, int cmp) {
  uint64_t *in = (uint64_t *)_in,*cpy=_cpy,n = (_n+7)/8;
  int i;
  if(cmp <= 1)
    return 0;
  for(i = 0; i < n; i++)
    if(in[i] != cpy[i]) {
      if(cmp > 4) abort(); // crash (AFL) fuzzing
      printf("ERROR in[%d]=%llx,%d dec[%d]=%llx,%d\n", i, in[i], bsr64(in[i]), i, cpy[i], bsr64(cpy[i]) );
      if(cmp>3) {
        int j;
        for(j=i & 0xffffff80u; j < i+128;j++) { unsigned e = in[j] != cpy[j];
          if(e) printf("#%d:%x,%x ", j, in[j], cpy[j]);else printf("%d:%x ", j, in[j]);
        }
        printf("\n");
      }
      if(cmp > 2) exit(EXIT_FAILURE);
      return i+1;
    }
  return 0;
}
//------------------------------- malloc ------------------------------------------------
#define USE_MMAP
  #if __WORDSIZE == 64
#define MAP_BITS 31
  #else
#define MAP_BITS 28
  #endif

void *_valloc(size_t size, int a) {
  if(!size) return NULL;
    #if defined(_WIN32)
  return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    #elif defined(USE_MMAP)
  void *ptr = mmap((size_t)a<<MAP_BITS, size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if(ptr == MAP_FAILED) return NULL;
  return ptr;
    #else
  return malloc(size);
    #endif
}

void _vfree(void *p, size_t size) {
  if(!p) return;
    #if defined(_WIN32)
  VirtualFree(p, 0, MEM_RELEASE);
    #elif defined(USE_MMAP)
  munmap(p, size);
    #else
  free(p);
    #endif
}

//--------------------------------------- IcBench ------------------------------------------------------------------
enum {
  FMT_TEXT=1,
  FMT_HTML,
  FMT_HTMLT,
  FMT_MARKDOWN,
  FMT_VBULLETIN,  // ex. post to encode.ru
  FMT_CSV,
  FMT_TSV,
  FMT_SQUASH
};

char *fmtext[] = { "txt", "txt", "html", "htm", "md", "vbul", "csv", "tsv", "squash" };

//------------- plugin : usage ---------------------------------
//extern struct plugg plugg[];

void plugsprt(void) {
  struct plugs *gs;

  struct plugg *pg;
  printf("Codec group:\n");
  for(pg = plugg; pg->id[0]; pg++)
    printf("%-16s %s %s\n", pg->id, pg->desc, pg->name);

  printf("\n\n%-16s %-48s %s\n", "Function","Description", "level");
  printf("\n%-16s %-48s %s\n", "--------","-----------", "-----");
  for(gs = plugs; gs->id >= 0; gs++)
    if(gs->codec)
      printf("%-16s %-48s %s\n", gs->name, gs->desc?gs->desc:"", gs->lev?gs->lev:"");
}

void plugsprtv(FILE *f, int fmt) {
  struct codecs *co;
  char         *pv = "";

  switch(fmt) {
    case FMT_HTMLT:
    case FMT_HTML:
      printf("%s\n", "<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"><title>IcBench</title></head><body><pre><ul>");
      break;
    case FMT_VBULLETIN:
      fprintf(f,"[list]\n");
      break;
  }

  for(co = codecs; co->coid >= 0; co++)
    if(co->coid) {
      char name[65],ver[33];
      pv = co->name;
      ver[0] = 0;
      codver(co->coid, co->ver, ver);
      sprintf(name, "%s %c%s", co->name, co->ver?'v':' ', ver);
      switch(fmt) {
        case FMT_VBULLETIN:
          fprintf(f, "[*][URL=\"%s\"]%s[/URL]\n", co->url, name );
          break;
        case FMT_HTML     :
          fprintf(f, "<li><a href=\"%s\">%s</a>\n", co->url, name );
          break;
        case FMT_MARKDOWN :
          fprintf(f, " - [%s](%s)\n", name, co->url);
          break;
        default:
          fprintf(f, "%-24s\t%s\n", name, co->url );
      }
    }

  switch(fmt) {
    case FMT_VBULLETIN:
      fprintf(f,"[/list]\n");
      break;
    case FMT_HTML:
      fprintf(f,"</ul></pre></body></html>");
      break;
  }
}

//------------------ plugin: process ----------------------------------
struct plug {
  int       id,err,blksize,lev;
  char      *name,prm[17],tms[20];
  int64_t len,memc,memd;
  double    tc,td;
};

struct plug plug[255];
int         seg_ans = 32*1024, seg_huf = 32*1024, seg_anx = 12*1024, seg_hufx=11*1024;
static int  cmp = 2,trans;
int verbose=1;
double      fac = 1.3;

int plugins(struct plug *plug, struct plugs *gs, int *pk, unsigned bsize, int bsizex, int lev, char *prm) {
  int i,k = *pk;
  struct plug *p = &plug[k];
  for(i = 0; i < k; i++)
    if(plug[i].id == gs->id && plug[i].lev == lev && !strcmp(plug[i].prm,prm))
      return -1;
  memset(p, 0, sizeof(struct plug));
  p->id  = gs->id;
  p->err = 0;
  p->name = gs->name;
  p->lev  = lev;
  strncpy(p->prm, prm?prm:(char *)"", 16);
  p->prm[16] = 0;
  p->tms[0]  = 0;
  p->blksize = gs->blksize?gs->blksize:bsize;
  *pk = ++k;
  return 0;
}

int plugreg(struct plug *plug, char *cmd, int k, int bsize, int bsizex) {
  static char *cempty="";
  int ignore = 0;

  while(*cmd) {
    char *name;
    while(isspace(*cmd))
      cmd++;
    name = cmd;
    while(isalnum(*cmd) || *cmd == '_' || *cmd == '-' || *cmd == '.')
      cmd++;
    if(*cmd) *cmd++ = 0;

    if(!strcmp(name, "ON" )) {
      ignore = 1;
      continue;
    }
    else if(!strcmp(name, "OFF")) {
      ignore = 0;
      continue;
    }

    for(;;) {
      char *prm;
      int lev;
      while(isspace(*cmd) || *cmd == ',')
        cmd++;

      prm = cmd;
      lev = strtol(cmd, &cmd, 10);
      if(prm == cmd) {
        lev = -1;
        prm = cempty;
      }
      else if(isalnum(*cmd)) {
        prm = cmd;
        while(isalnum(*cmd) || *cmd == '_' || *cmd == '-' || *cmd == '.')
          cmd++;
        if(*cmd)
          *cmd++ = 0;
      } else
        prm = cempty;

      { int found = 0;
        struct plugs *gs,*gfs = NULL;
        if(!*name)
          break;
        for(gs = plugs; gs->id >= 0; gs++)
          if(!strcasecmp(gs->name, name) ) {
            char s[33],*q;
            sprintf(s,"%d", lev);
            found++;
            if(lev<0 && gs->lev && !gs->lev[0] || gs->lev && (q=strstr(gs->lev, s)) && (q==gs->lev || *(q-1) == ',')) {
              found++;
              plugins(plug, gs, &k, bsize, bsizex, lev, prm);
            }
            break;
          }
        if((found<2 || !gs->codec) && !ignore) {
          if(!found)
            fprintf(stderr, "codec '%s' not found\n", name);
          else if(!gs->codec)
            fprintf(stderr, "codec '%s' not compiled in icbench\n", name);
          else if(lev<0)
            fprintf(stderr, "level [%s] not specified for codec '%s'\n", gs->lev, name );
          else if(gs->lev && gs->lev[0])
            fprintf(stderr, "level '%d' for codec '%s' not in range [%s]\n", lev, name, gs->lev);
          else
            fprintf(stderr, "codec '%s' has no levels\n", name);
          exit(0);
        }
      }

      while(isspace(*cmd))
        cmd++;
      if(*cmd != ',' && (*cmd < '0' || *cmd > '9'))
        break;
    }
  }
  a:plug[k].id = -1;
  return k;
}

//------------------ plugin: print/plot -----------------------------
struct bandw {
  uint64_t bw;
  unsigned           rtt;
  char               *s;
};

static struct bandw bw[] = {
  {    7*KB, 500, "GPRS 56"  },//56kbps
  {   57*KB, 150, "2G 456"   },
  {  125*KB,  40, "3G 1M"    },
  {  250*KB,   5, "DSL 2M"   },//DSL 2000
  {  500*KB,  20, "4G 4M"    },
  { 3750*KB,   5, "WIFI 30M" },
  {12500*KB,   5, "CAB 100M" },
  {   40*MB,   0, "USB2 40MB"},
  {  125*MB,   0, "ETH 1000" },
  {  200*MB,   0, "HDD 200MB"},
  {  550*MB,   0, "SSD 550MB"},
  {   1u*GB,   0, "SSD 1GB"  },
  {   2u*GB,   0, "SSD 2GB"  },
  { 4ull*GB,   0, "4GB/s"    },
  { 8ull*GB,   0, "8GB/s"    }
};
#define BWSIZE (sizeof(bw)/sizeof(struct bandw))

void plugprth(FILE *f, int fmt, char *t) {
  char *plot  = "<script src=https://cdn.plot.ly/plotly-latest.min.js></script>";
  char *jquery = "<script src=\"http://ajax.googleapis.com/ajax/libs/jquery/1/jquery.min.js\"></script>";
  char *tstyle = "<link rel=\"stylesheet\" href=\"http://tablesorter.com/themes/blue/style.css\" type=\"text/css\" media=\"print, projection, screen\" />";
  char *table  = "<script type=\"text/javascript\" src=\"http://tablesorter.com/__jquery.tablesorter.min.js\"></script>";
  char *code   = "<script type=\"text/javascript\">$(function() {       $(\"#myTable\").tablesorter({sortList:[[0,0],[2,1]], widgets: ['zebra']});      $(\"#options\").tablesorter({sortList: [[0,0]], headers: { 3:{sorter: false}, 4:{sorter: false}}}); }); </script><script type=\"text/javascript\" src=\"http://tablesorter.com/__jquery.tablesorter.min.js\"></script><script type=\"text/javascript\">$(function() {       $(\"#myTable2\").tablesorter({sortList:[[0,0],[2,1]], widgets: ['zebra']});     $(\"#options\").tablesorter({sortList: [[0,0]], headers: { 3:{sorter: false}, 4:{sorter: false}}}); }); </script>";
  char s[128];
  time_t tm;
  time(&tm);
  sprintf(s, "IcBench: %s - %s", t, asctime(localtime(&tm)));

  switch(fmt) {
    case FMT_TEXT:
      fprintf(f,"%s\n", s );
      break;
    case FMT_VBULLETIN:
      fprintf(f,"%s\n", s);
      break;
    case FMT_HTMLT:
      fprintf(f,"<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"><title>IcBench: %s - </title></head><body>\n", s);
      break;
    case FMT_HTML:
      fprintf(f,"<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"><title>IcBench: %s - </title>%s%s%s%s%s</head><body>\n", s, plot, jquery, tstyle, table, code);
       break;
    case FMT_MARKDOWN:
      fprintf(f,"#### %s (bold = pareto)  MB=1.000.000\n", s);
      break;
  }
}

void plugprtf(FILE *f, int fmt) {
  switch(fmt) {
    case FMT_HTML:
      fprintf(f,"</body></html>\n");
      break;
  }
}

void plugprtth(FILE *f, int fmt) {
  char *head =  "      C Size  ratio%%  Bits/Int  C MI/s     D MI/s   Name            File              (bold = pareto)";

  switch(fmt) {
    case FMT_TEXT:
      fprintf(f,"      C Size  ratio%% Bits/Int   C MI/s     D MI/s   Name            File\n");
      break;
    case FMT_VBULLETIN:
      fprintf(f,"[CODE][B]%s[/B] MB=1.000.0000\n", head);
      break;
    case FMT_HTMLT:
      fprintf(f,"<pre><b>%s</b> MB=1.000.0000\n", head);
      break;
    case FMT_HTML:
      fprintf(f,"<h3>IcBench: Compressor Benchmark</h3><table id='myTable' class='tablesorter' style=\"width:35%%\"><thead><tr><th>C Size</th><th>ratio%%</th><th>C MI/s</th><th>D MI/s</th><th>Name</th><th>C Mem</th><th>D Mem</th><th>File</th></tr></thead><tbody>\n");
      break;
    case FMT_MARKDOWN:
      fprintf(f,"|C Size|ratio%%|Bits/Integer|C MI/s|D MI/s|Name|File|\n|--------:|-----:|--------:|--------:|----------------|----------------|\n");
      break;
    case FMT_CSV:
      fprintf(f,"size,csize,ratio,bpi,ctime,dtime,name,file\n");
      break;
    case FMT_TSV:
      fprintf(f,"size\tcsize\tratio\tbpi\tctime\tdtime\tname\tfile\n");
      break;
    case FMT_SQUASH:
      fprintf(f,"dataset,plugin,codec,level,compressed_size,compress_cpu,compress_wall,decompress_cpu,decompress_wall\n");
      break;
  }
}

void plugprttf(FILE *f, int fmt) {
  switch(fmt) {
    case FMT_VBULLETIN:
      fprintf(f,"[/CODE]\n");
      break;
    case FMT_HTMLT:
      fprintf(f,"</pre>\n");
      break;
    case FMT_HTML:
      fprintf(f,"</tbody></table>\n");
      break;
    case FMT_MARKDOWN:
      fprintf(f,"\n\n");
      break;
  }
}

#define RATIO(_clen_, _len_)  ((double)_clen_*100.0/_len_)
#define RATIOI(_clen_, _len_) ((double)_clen_*32.0/_len_)
#define FACTOR(_clen_, _len_) ((double)_len_*100.0/(double)_clen_)
#define RATIOF(_clen_, _len_, _mode_) _mode_?FACTOR(_clen_, _len_):RATIO(_clen_, _len_)
int be_factor=0;
void plugprt(struct plug *plug, int64_t totinlen, char *finame, int fmt, FILE *f) {
  double ratio  = RATIOF(plug->len,totinlen, be_factor),
         ratioi = RATIOI(plug->len,totinlen),
         tc     = TMBS(totinlen,plug->tc), td = TMBS(totinlen,plug->td);
  char   name[65];
  int c = 0, d = 0, n = 0;
  if(plug->lev >= 0)
    sprintf(name, "%s%s %d%s", plug->err?"?":"", plug->name, plug->lev, plug->prm);
  else
    sprintf(name, "%s%s%s",    plug->err?"?":"", plug->name,            plug->prm);

  switch(fmt) {
    case FMT_TEXT:
      fprintf(f,"%12"PRId64"   %5.1f   %5.2f  %8.2f   %8.2f   %-16s%s\n",
        plug->len, ratio, ratioi, tc, td, name, finame);
      break;
    case FMT_VBULLETIN:
      fprintf(f, "%12"PRId64"   %5.1f   %5.2f   %s%8.2f%s   %s%8.2f%s   %s%-16s%s%s\n",
        plug->len, ratio, ratioi, c?"[B]":"", tc, c?"[/B]":"",  d?"[B]":"", td, d?"[/B]":"", n?"[B]":"", name, n?"[/B]":"", finame);
      break;
    case FMT_HTMLT:
      fprintf(f, "%12"PRId64"   %5.1f   %5.2f   %s%8.2f%s   %s%8.2f%s   %s%-16s%s%s\n",
        plug->len, ratio, ratioi, c?"<b>":"", tc, c?"</b>":"",  d?"<b>":"", td, d?"</b>":"", n?"<b>":"", name, n?"</b>":"", finame);
      break;
    case FMT_HTML:
      fprintf(f, "<tr><td align=\"right\">%11"PRId64"</td><td align=\"right\">%5.1f</td><td align=\"right\">%5.2f</td><td align=\"right\">%s%8.2f%s</td><td align=\"right\">%s%8.2f%s</td><td>%s%-16s%s</td><td align=\"right\">%"PRId64"</td><td align=\"right\">%"PRId64"</td><td>%s</td></tr>\n",
        plug->len, ratio, ratioi, c?"<b>":"", tc, c?"</b>":"",  d?"<b>":"", td, d?"</b>":"", n?"<b>":"", name, n?"</b>":"",
//        SIZE_ROUNDUP(plug->memc, Kb)/Kb, SIZE_ROUNDUP(plug->memd,Kb)/Kb,
        plug->memc, plug->memd,
        finame);
      break;
    case FMT_MARKDOWN:
      fprintf(f, "|%"PRId64"|%5.1f|%5.2f|%s%.2f%s|%s%.2f%s|%s%s%s|%s|\n",
        plug->len, ratio, ratioi, c?"**":"",  tc, c?"**":"",    d?"**":"",  td, d?"**":"",   n?"**":"",  name, n?"**":"",   finame);
      break;
    case FMT_CSV:
      fprintf(f, "%12"PRId64",%11"PRId64",%5.1f,%5.2f,%8.2f,%8.2f,%-16s,%s\n",
        totinlen, plug->len, ratio, ratioi, tc, td, name, finame);
      break;
    case FMT_TSV:
      fprintf(f,"%12"PRId64"\t%11"PRId64"\t%5.1f\t5.2f\t%8.2f\t%8.2f\t%-16s\t%s\n",
        totinlen, plug->len, ratio, ratioi, tc, td, name, finame);
      break;
    case FMT_SQUASH:
      fprintf(f,"%12"PRId64",%11"PRId64",%5.1f,%8.2f,%8.2f,%-16s,%s\n",
        finame, name, name, plug->len,        tc, tc, td, td);
      break;
  }
}

static int blknum, speedup;
enum { SP_SPEEDUPC=1, SP_SPEEDUPD, SP_TRANSFERC, SP_TRANSFERD };

void plugprtph(FILE *f, int fmt) {
  int i;

  switch(fmt) {
    case FMT_HTML:
      fprintf(f,"<p><h3>IcBench: Speedup %s sheet</h3><table id='myTable2' class='tablesorter' style=\"width:80%%\"><thead><tr><th>Name</th>", (speedup&1)?"compression":"decompression");
      for(i = 0; i < BWSIZE; i++)
        fprintf(f, "<th>%s</th>", bw[i].s);
      fprintf(f, "<td>File");
      if(blknum)
        fprintf(f, " blknum=%d ", blknum);
      fprintf(f, "</td></tr></thead><tbody>\n");
      break;
    case FMT_MARKDOWN:
      fprintf(f,"#### IcBench: Speedup %s sheet\n\n", (speedup&1)?"compression":"decompression");
      fprintf(f, "|Name");
      for(i = 0; i < BWSIZE; i++)
        fprintf(f, "|%s", bw[i].s);
      fprintf(f, "|File");
      if(blknum)
        fprintf(f, " blknum=%d ", blknum);
      fprintf(f, "|\n");
      fprintf(f, "|-------------");
      for(i = 0; i < BWSIZE; i++)
        fprintf(f, "|---------:");
      fprintf(f, "|-------------|\n");
      break;
    case FMT_VBULLETIN:
      fprintf(f,"IcBench: Speedup %s sheet\n\n", (speedup&1)?"compression":"decompression");
      fprintf(f,"[CODE][B]\n");
    default:
      fprintf(f,"Name           ");
      for(i = 0; i < BWSIZE; i++)
        fprintf(f, "%10s", bw[i].s);
      if(blknum)
        fprintf(f, " blknum=%d ", blknum);
      fprintf(f, "\n");
    if(fmt == FMT_VBULLETIN)
      fprintf(f,"[/B]\n");
  }
}

static inline double spmbs(double td, int64_t len, int i, int64_t totinlen) {
  double t = td + len*TM_T/(double)bw[i].bw + blknum*(bw[i].rtt*1000.0);
  return TMBS(totinlen,t);
}

//static inline double spdup(double td, int64_t len, int i, int64_t totinlen) { double t = td + len*TM_T/(double)bw[i].bw + blknum*(bw[i].rtt*1000.0); return ((double)totinlen*TM_T*100.0/t)/(double)bw[i].bw;}
static inline double spdup(double td, int64_t len, int i, int64_t totinlen) {
  return (double)totinlen*100.0 / ((double)len + ((td+blknum*bw[i].rtt*1000.0)/TM_T)*(double)bw[i].bw );
}

void plugprtp(struct plug *plug, int64_t totinlen, char *finame, int fmt, int speedup, FILE *f) {
  int  i;
  char name[65];
  if(plug->lev>=0)
    sprintf(name, "%s%s%s%d%s", plug->err?"?":"", plug->name, fmt==FMT_MARKDOWN?"_":" ", plug->lev, plug->prm);
  else
    sprintf(name, "%s%s%s",    plug->err?"?":"", plug->name,            plug->prm);
  if(fmt == FMT_HTML)
    fprintf(f, "<tr><td>%s</td>", name);
  else
    fprintf(f, "%-16s", name);

  for(i = 0; i < BWSIZE; i++) {
    switch(fmt) {
      case FMT_HTMLT:
      case FMT_HTML:
        fprintf(f, "<td align=\"right\">");
        break;
      case FMT_MARKDOWN:
        fprintf(f, "|");
        break;
    }
    switch(speedup) {
      case SP_TRANSFERD:
        fprintf(f,"%9.3f ", spmbs(plug->td, plug->len, i, totinlen));
        break;
      case SP_SPEEDUPD:
        fprintf(f,"%9d ", (int)(spdup(plug->td, plug->len, i, totinlen)+0.5));
        break;
      case SP_TRANSFERC:
        fprintf(f,"%9.3f ", spmbs(plug->td, plug->len, i, totinlen));
        break;
      case SP_SPEEDUPC:
        fprintf(f,"%9d ", (int)(spdup(plug->td, plug->len, i, totinlen)+0.5));
        break;
    }
    switch(fmt) {
      case FMT_HTMLT:
      case FMT_HTML:
        fprintf(f, "</td>");
        break;
      case FMT_MARKDOWN:
        break;
    }
  }
  switch(fmt) {
    case FMT_HTMLT:
    case FMT_HTML:
      fprintf(f, "<td>%s</td></tr>\n", finame);
      break;
    case FMT_MARKDOWN:
      fprintf(f, "|%s|\n", finame);
      break;
    default:
      fprintf(f, "%s\n", finame);
      break;
  }
}

struct { unsigned x,y; } divplot[] = {
  { 1920, 1080}, // 16:9
  { 1600,  900},
  { 1280,  720},
  {  800,  600}
};

static unsigned divxy = 1, xlog = 1, xlog2, ylog, ylog2, plotmcpy;

void plugplotb(FILE *f, int fmt, int idiv) {
  fprintf(f, "<div id='myDiv%d' style='width: %dpx; height: %dpx;'></div><script>", idiv, divplot[divxy].x, divplot[divxy].y);
}

void plugplot(struct plug *plug, int64_t totinlen, int fmt, int speedup, char *s, FILE *f) {
  int  i;
  char name[65];
  if(plug->lev>=0)
    sprintf(name, "%s%s_%d%s", plug->err?"?":"", plug->name, plug->lev, plug->prm);
  else
    sprintf(name, "%s%s%s",    plug->err?"?":"", plug->name,            plug->prm);
  strcat(s,name); strcat(s,",");

  fprintf(f, "var %s = { x: [", name);
  for(i = 0; i < BWSIZE; i++)
    fprintf(f,"%llu%s", bw[i].bw, i+1 < BWSIZE?",":"");
  fprintf(f, "],\ny: [");

  for(i = 0; i < BWSIZE; i++)
    switch(speedup) {
      case SP_TRANSFERD:
        fprintf(f,"%9.3f%s",    spmbs(plug->td, plug->len, i, totinlen), i+1 < BWSIZE?",":"");
        break;
      case SP_SPEEDUPD:
        fprintf(f,"%9d%s", (int)(spdup(plug->td, plug->len, i, totinlen)+0.5), i+1 < BWSIZE?",":"");
        break;
      case SP_TRANSFERC:
        fprintf(f,"%9.3f%s",    spmbs(plug->tc, plug->len, i, totinlen), i+1 < BWSIZE?",":"");
        break;
      case SP_SPEEDUPC:
        fprintf(f,"%9d%s", (int)(spdup(plug->tc, plug->len, i, totinlen)+0.5), i+1 < BWSIZE?",":"");
        break;
    }
  fprintf(f, "],\ntype: 'scatter',\nmode: 'lines+markers',\nline: {shape: 'spline'},\nname: '%s'\n};\n", name);
}

void plugplote(FILE *f, int fmt, char *s) {
  fprintf(f, "var data = [%s];\nvar layout = {\ntitle:'IcBench Speedup: Transfer+%s Speed',\nxaxis: {\ntitle: '%s Transfer Speed (M=MI/s B=GB/s)',\n%s    autorange: true\n  }, \n  yaxis: {\n\ntitle: 'Speedup %%',\n%sautorange: true\n  }\n};\nPlotly.plot('myDiv1', data, layout);</script>\n",
    s, (speedup&1)?"Compression":"Decompression", xlog?"log":"", xlog?"type: 'log',\n":"", ylog?"type: 'log',\n":"");
}

int libcmp(const struct plug *e1, const struct plug *e2) {
  if(e1->len < e2->len)
    return -1;
  else if(e1->len > e2->len)
    return 1;
  else if(e1->td < e2->td)
    return -1;
  else if(e1->td > e2->td)
    return 1;
  return 0;
}

int libcmpn(const struct plug *e1, const struct plug *e2) {
  int c = strcmp(e1->name, e2->name);
  if(c < 0)
    return -1;
  else if(c > 0)
    return 1;
  else if(e1->lev < e2->lev)
    return -1;
  else if(e1->lev > e2->lev)
    return 1;
  return 0;
}

#define P_MCPY 1  // memcpy id
void plugplotc(struct plug *plug, int k, int64_t totinlen, int fmt, int speedup, char *s, FILE *f) {
  int  i, n = 0;
  char name[65],txt[256];
  struct plug *g,*gs=plug,*p;
  qsort(plug, k, sizeof(struct plug), (int(*)(const void*,const void*))libcmpn);

  for(txt[0] = name[0] = 0, g = plug; g < plug+k; g++)
  if(g->id <= P_MCPY && !plotmcpy)
    continue;
  else {
    if(strcmp(g->name, name)) {
      if(name[0]) {
        fprintf(f, "],\ny: [");
        for(p = gs; p < g; p++)
          fprintf(f, "%.2f%s", RATIOF(p->len,totinlen, be_factor), p+1<g?",":"");
        fprintf(f, "],\nmode: 'markers+text',\ntype: 'scatter',\nname: '%s',\ntextposition: 'top center', textfont: { family:  'Raleway, sans-serif' }, marker: { size: 12 }\n", name, txt);
        if(txt[0])
          fprintf(f, "\n,text: [%s]\n", txt);
        fprintf(f, "};\n");
        strcat(s,",");
        txt[0] = 0;
      }
      gs = g;
      strcpy(name, g->name);
      fprintf(f, "var %s = {\n x: [", g->name);
      strcat(s, g->name);
    } else {
      fprintf(f, ",");
      strcat(txt, ",");
    }
    if(g->lev >= 0) {
      char ts[33];
      sprintf(ts, "'%s%s%d%s'", divxy>=2?"":g->name, divxy>=2?"":",", g->lev, g->prm);
      strcat(txt, ts);
    }
    { double t = (speedup&1)?g->tc:g->td;  fprintf(f, "%.2f", TMBS(totinlen,t)); }
  }
  fprintf(f, "],\ny: [");
  for(p = gs; p < g; p++)
    fprintf(f, "%.2f%s", RATIOF(p->len,totinlen, be_factor), p+1<g?",":"");
  fprintf(f, "],\nmode: 'markers+text',\ntype: 'scatter',\nname: '%s',\ntextposition: 'top center', textfont: { family:  'Raleway, sans-serif' }, marker: { size: 12 }\n", name, txt);
  if(txt[0])
    fprintf(f, "\n,text:[%s]\n", txt);
  fprintf(f, "};\n");
}

void plugplotce(FILE *f, int fmt, char *s) {
  fprintf(f, "var data = [%s];\nvar layout = {\ntitle:'IcBench: %s',\nxaxis: {\ntitle: '%s speed MI/s',\n%s    autorange: true\n  }, \n  yaxis: {\n\ntitle: 'Ratio (factor)',\n%sautorange: true\n  }\n};\nPlotly.plot('myDiv2', data, layout);</script>\n",
    s, (speedup&1)?"Compression":"Decompression", xlog2?"log":"", xlog2?"type: 'log',\n":"", ylog2?"type: 'log',\n":"");
}

int plugprts(struct plug *plug, int k, char *finame, int xstdout, uint64_t totlen, int fmt, char *t) {
  struct plug *g;
  char s[257];
  FILE *fo;
  if(!totlen) return 0;                                                                             if(verbose>1) printf("'%s'\n", finame);

  qsort(plug, k, sizeof(struct plug), (int(*)(const void*,const void*))libcmp);
  sprintf(s, "%s.%s", finame, fmtext[fmt]);
  fo = xstdout>=0?stdout:fopen(s, "w");
  if(!fo)
    die("file create error for '%s'\n", finame);

  plugprth( fo, fmt, t);
  plugprtth(fo, fmt);
  for(g = plug; g < plug+k; g++)
    plugprt(g, totlen, finame, fmt, fo);
  plugprttf(fo, fmt);

  if(speedup) {
    switch(fmt) {
      case FMT_TEXT :
        fprintf(fo, "\n");
        break;
      case FMT_HTML :
        break;
      case FMT_HTMLT:
        fprintf(fo, "<pre>\n");
        break;
      case FMT_MARKDOWN :
        fprintf(fo, "\n");
        break;
    }
    plugprtph(fo, fmt);
    for(g = plug; g < plug+k; g++)
      plugprtp(g, totlen, finame, fmt, speedup, fo);
    fprintf(fo, "\n");
    switch(fmt) {
      case FMT_TEXT :
        fprintf(fo, "\n"); break;
      case FMT_HTML :
        fprintf(fo, "</tbody></table>\n"); break;
      case FMT_HTMLT:
        fprintf(fo, "</pre>\n");
        break;
      case FMT_VBULLETIN:
        fprintf(fo,"[/CODE]\n");
        break;
      case FMT_MARKDOWN :
        fprintf(fo, "\n");
        break;
    }
    if(fmt == FMT_HTML) {
      char s[1025];
      s[0] = 0;                                                             if(verbose>1) printf("generate speedup plot\n");
      plugplotb(fo, fmt, 1);
      for(g = plug; g < plug+k; g++)
        if(g->id > P_MCPY || plotmcpy)
          plugplot(g, totlen, fmt, speedup, s, fo);
      plugplote(fo, fmt, s);

      s[0] = 0;                                                             if(verbose>1) printf("generate speed/ratio plot\n");
      plugplotb(fo, fmt, 2);
      plugplotc(plug, k, totlen, fmt, speedup, s, fo);
      plugplotce(fo, fmt, s);

    }
  }
  plugprtf(fo, fmt);
  if(xstdout<0) fclose(fo);
}

int plugread(struct plug *plug, char *finame, int64_t *totinlen) {
  char s[256],name[33];
  struct plug *p=plug;
  FILE *fi = fopen(finame, "r");
  if(!fi) return -1;

  fgets(s, 255, fi);
  for(p = plug;;) {
    char ss[256],*t = ss,*q;
    int i;
    memset(p, 0, sizeof(p[0]));
    p->tms[0] = 0;
    if(!fgets(ss, 255, fi)) break;
    for(q = t;  *q && *q != '\t'; q++);  *q++ = 0; strcpy(s, t); t = q;
    *totinlen = strtoull(t, &t, 10);
    p->len    = strtoull(++t, &t, 10);
    p->td     = strtod(  ++t, &t);
    p->tc     = strtod(  ++t, &t);
    for(q = ++t; *q && *q != '\t'; q++); *q++ = 0; strcpy(name,t); t=q;
    p->lev    = strtoul(t, &t, 10);
    for(q = t; *q && *q != '\t'; q++);   *q++ = 0; strcpy(p->prm,t); t = q;
    p->memc   = strtoull(t, &t, 10);
    p->memd   = strtoull(++t, &t, 10);
    for(q = ++t; *q && *q != '\t'; q++); *q++ = 0; strcpy(p->tms,t); t = q;
    if(p->prm[0]=='!')
      p->prm[0]=0;
    for(i = 0; plugs[i].id >=0; i++)
      if(!strcmp(name, plugs[i].name)) {
        p->name  = plugs[i].name;
        p->id = plugs[i].id;                                            if(verbose>1) { fprintf(stdout, "%s\t%"PRId64"\t%"PRId64"\t%.6f\t%.6f\t%s\t%d%s\t%s\t%"PRId64"\t%"PRId64"\n", s, *totinlen, p->len, p->td, p->tc, p->name, p->lev, p->prm, p->tms, p->memc, p->memd); fflush(stdout); }
        p++;
        break;
      }
  }
  fclose(fi);
  return p - plug;
}

//----------------------------------- Benchmark -----------------------------------------------------------------------------
static int be_mcpy, be_nblocks, fuzz;
int be_mindelta=-1,mdelta=0;

int checksort(unsigned *in, unsigned n) {
  if((be_mindelta == 0 || be_mindelta == 1) && !be_nblocks) {
    int i;
    for(i = 1; i < n/4; i++)
      if(in[i] < in[i-1]+be_mindelta) {
        fprintf(stderr, "WARNING: IDs not sorted %d:%u,%u\n", i, in[i-1], in[i]);fflush(stderr);
        return -1;
      }
  }
  return 0;
}
unsigned becomp(unsigned char *_in, unsigned _inlen, unsigned char *_out, unsigned outsize, unsigned bsize, int id, int lev, char *prm, int be_mindelta, CODCOMP codcomp) {
  unsigned char *op,*oe = _out + outsize;
  unsigned char *in,*ip;
  if(!_inlen) return 0;
  TMBEG(tm_rep,tm_Rep);
  for(op = _out, in = _in; in < _in+_inlen; ) {
    unsigned inlen = _inlen,bs;
    if(be_nblocks) {                                                blknum++;
      inlen = ctou32(in); in+=4;
      vbput32(op, inlen);                                           // ctou32(op) = inlen; op+=4;
      inlen *= 4;                                                   if(in+inlen>_in+_inlen) die("FATAL buffer overflow error %d", in - _inlen);
    }

    for(ip = in, in += inlen; ip < in; ) {
      unsigned iplen = in - ip; iplen = min(iplen, bsize);       //   cnt++; csize += iplen;
      op = codcomp(ip, iplen, op, oe-op, id, lev, prm, be_mindelta);
      ip += iplen;
      if(op > _out+outsize)
        die("Compress overflow error %llu, %u in lib=%d\n", outsize, (int)(ptrdiff_t)(op - _out), id);
    }
  }
  TMEND(_inlen);                   //  printf("cnt=%d, csize=%d\n", cnt, csize);
  return op - _out;
}

int bedecomp(unsigned char *_in, int _inlen, unsigned char *_out, unsigned _outlen, unsigned bsize, int id, int lev, char *prm, int be_mindelta, CODDECOMP coddecomp) {
  unsigned char *ip;
  unsigned char *out,*op;

  TMBEG(tm_rep2,tm_Rep2);
  for(ip = _in, out = _out; out < _out+_outlen;) {
    unsigned outlen=_outlen,bs;
    if(be_nblocks) {
      vbget32(ip, outlen);                                                //outlen      = ctou32(ip);  ip  += 4;
      ctou32(out) = outlen;      out += 4;
      outlen *= 4;                                                        if(out+outlen >_out+_outlen) die("FATAL: decompress overflow output error %d ", outlen);
    }
    for(op = out, out += outlen; op < out; ) {
      unsigned oplen = out - op;
      oplen = min(oplen, bsize);
      ip = coddecomp(ip, 0, op, oplen, id, lev, prm, be_mindelta);                if(ip-_in>_inlen) die("FATAL inlen %d,%d,bsize=%d ", _inlen, ip-_in, bsize);
      op += oplen;
    }
  }
  if(!(ip - _in))
    return 0;
  TMEND(_outlen);
  return ip - _in;
}

struct plug plugr[32];
int tid;
#define BEPRE
#define BEINI
#define BEPOST
#define BEOPT
#define BEUSAGE
#define BEFILE
#define BENCHSTA

#define INOVD 4*1024

  #if defined(_WIN32) && !defined(__MINGW__)
int getpagesize() {
  static int pagesize = 0;
  if (pagesize == 0) {
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    pagesize = max(system_info.dwPageSize, system_info.dwAllocationGranularity);
  }
  return pagesize;
}
  #endif

struct cod { CODCOMP comp; CODDECOMP decomp; };
struct cod cods[] = { { codcomp, coddecomp }, {codcomps, coddecomps }, {codcompz, coddecompz } };

unsigned mininlen;
uint64_t filen;
size_t insizem;
char name[65];

uint64_t plugbench(struct plug *plug, unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize,
                             unsigned char *_cpy, unsigned bsize, struct plug *plugr, int tid, char *finame) {
  double tc = 0.0, td = 0.0;
  int    be_mode = be_mindelta<0?0:(be_mindelta>1?2:1);
  unsigned outlen = becomp(in, inlen, out, outsize, bsize, plug->id, plug->lev, plug->prm, be_mindelta, cods[be_mode].comp);
  plug->len += outlen;
  plug->tc  += (tc += (double)tm_tm/(double)tm_rm);
  if(!outlen) plug->tc = 0;
                                                                                                if(outlen && verbose /*&& inlen == filen*/) { printf("%12u   %5.1f   %5.2f  %8.2f   ", outlen, RATIOF(outlen,inlen, be_factor), RATIOI(outlen,inlen), TMBS(inlen,tc)); fflush(stdout); }
  if(cmp && outlen) {
    unsigned char *cpy = _cpy;
    unsigned cpylen,e;
    if(_cpy != in) memrcpy(cpy, in, inlen);
    cpylen = bedecomp(out, outlen, cpy, inlen, bsize, plug->id, plug->lev, plug->prm, be_mindelta, cods[be_mode].decomp);
    td = (double)tm_tm/(double)tm_rm;                                                           if(verbose /*&& inlen == filen*/) { printf("%8.2f   %c%-16s%s\n", TMBS(inlen,td), outlen?' ':'?',name, finame); }
    e = memcheck(in, inlen, cpy, cmp);
    plug->err = plug->err?plug->err:e;
    plug->td += td;
  } else                                                                                        if(outlen && verbose /*&& inlen == filen*/) { printf("%8.2f   %c%-16s%s\n", 0.0, outlen?' ':'?', name,  finame); }
  if(!plug->len) plug->err++;
  return outlen;
}

//--------------------------------------- Zipfian generator --------------------------------------------------------
double    a = 1.5;
                                                                                 unsigned xbits[33];
void stprint(char *s) {
  int m;
  uint64_t t=0;
  for(m = 0; m < 33; m++)
    t += xbits[m];
  printf("\n%s bits histogram:",s);
  for(m = 0; m < 33; m++)
    if(xbits[m]) printf("%d:%.2f%% ", m, (double)xbits[m]*100/t); printf("\n");
}

int dcmp(double *a, double *b) {
  if(*a < *b) return -1;
  if(*a > *b) return  1;
  return 0;
}

void zipfgen(unsigned *a, unsigned n, double alpha, unsigned x1, unsigned x2) {
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

//       0       1        2         3         4         5         6         7,       8
enum { T_0, T_UINT8, T_UINT16, T_UINT24, T_UINT32, T_UINT40, T_UINT48, T_UINT56, T_UINT64, T_CHAR, T_TXT, T_TST };
int mdelta;
unsigned rm=0,rx=255;
#define OVD (10*MB)
#define IPUSH(in,n,isize, nmax,u) { if(n >= nmax) { nmax = nmax?(nmax << 1):(1<<20); in = realloc(in, nmax*isize+OVD); if(!in) die("malloc err=%u", nmax); }\
                                  ctou32(in+n*isize) = u; n++; if(verbose>5) printf("%u,",(unsigned)u );\
                                }
unsigned befgen(unsigned char **_in, unsigned n, int fmt, unsigned isize, FILE *fi, int kid, int skiph, int decs, int divs) {
  unsigned char *in = *_in,*ip;
  unsigned nmax = 0;
  #define LSIZE 1024
  char s[LSIZE+1];
  double pre;

  if(!fi) {                                                                 printf("zipf alpha=%.2f range[%u..%u].n=%u\n ", a, rm, rx, n);
    in = malloc(n*isize+OVD);                                               if(!in) die("malloc err=%u", nmax);

    zipfgen((unsigned *)in, n, a, rm, rx);                                  //{ int i; for(i = 0; i < n; i++) in[i] = i; }
                                                                            int i;for(i = 1; i <= n; i++) xbits[bsr32(ctou32(in+i*4))]++;
    if(be_mindelta == 0 || be_mindelta == 1) {
      unsigned *ip = (unsigned *)in, v;                                     stprint("delta");
      for(ip[0]=0,v = 1; v < n; v++) {
        ip[v] += ip[v-1] + be_mindelta;                                     if(ip[v]>=(1u<<31)) die("overflow generating sorted array %d\n", ip[v]);
      }
    } else stprint("");
    *_in = in;
    return n*isize;
  }

  n = 0;                                                                    //printf("fmt=%d,", fmt);fflush(stdout);
  if(skiph) {
    fgets(s, LSIZE, fi);                                                    if(verbose>5) printf("skip first line\n");
  }
  pre = decs?pow(10.0f,(float)decs):1;
  pre /= divs;
  switch(fmt) {

    case T_TXT:                                                             if(verbose) printf("reading text file\n");

      while(fgets(s, LSIZE, fi)) {
        unsigned char *p = s,*q;
        int k;
        s[strlen(s) - 1] = 0;
        for(k = 0; *p; ) {
          uint64_t u;
          while(*p && (*p < '0' || *p > '9') ) *p++;
          u = strtoull(p, &q, 10) - mdelta;
          if(*q == '.') {
            u = round(strtod(p, &q)*pre) - mdelta;
          }
          p = q;
          if(++k == kid) {
            IPUSH(in,n,isize,nmax,u);
            break;
          }
          while(*p && (*p >= '0' && *p <= '9' || *p == '.') ) *p++;
        }
      }
      break;
    case T_CHAR:
      for(;;) {
        char *p = s,*q;
        int c;
        uint64_t u;
        while((c = getc(fi)) >= '0' && c <= '9')
          if(p - s < LSIZE) *p++ = c;
        *p = 0;
        u = strtoull(s, &p, 10) - mdelta;
        if(*p == '.')
          u = round(strtod(s, &q)*pre) - mdelta;
        IPUSH(in,n,isize,nmax,u);
        if(c == EOF) break;
      }
      break;
    case T_UINT8: {
      unsigned char u;
      while(fread(&u, 1, 1, fi)>0)
        IPUSH(in,n,isize,nmax, u-mdelta);
    } break;
    case T_UINT16: {
      unsigned short u;
      while(fread(&u, sizeof(u), 1, fi)>0)
        IPUSH(in,n,isize,nmax, u-mdelta);
    } break;
    /*case T_DBL:
      double d,*din = NULL; n=0;
      while(fread(&u, sizeof(u), 1, fi)>0)
        IPUSH(din,n,isize,nmax, d);
     }*/
    /*case T_UINT32: {
      unsigned u;
      while(fread(&u, sizeof(u), 1, fi)>0)
        IPUSH(in,n,isize,nmax, u-mdelta);
    } break;*/
    default: die("unknown data format %d\n", fmt);
  }
  *_in = in;
  return n*isize;
}

void usage(char *pgm) {
  fprintf(stderr, "\nIcBench Copyright (c) 2013-2019 Powturbo %s\n", __DATE__);
  fprintf(stderr, "Usage: %s [options] [file]\n", pgm);
  fprintf(stderr, " -eS      S = compressors/groups separated by '/' Parameter can be specified after ','\n");
  fprintf(stderr, " -b#s     # = blocksize (default filesize,). max=1GB\n");
  fprintf(stderr, " -B#s     # = max. benchmark filesize (default 1GB) ex. -B4G\n");
  fprintf(stderr, " -s#s     # = min. buffer size to duplicate & test small files (ex. -s50)\n");
  fprintf(stderr, "          s = modifier s:K,M,G=(1000, 1.000.000, 1.000.000.000) s:k,m,h=(1024,1Mb,1Gb). (default m) ex. 64k or 64K\n");
  fprintf(stderr, "Benchmark:\n");
  fprintf(stderr, " -i#/-j#  # = Minimum  de/compression iterations per run (default=auto)\n");
  fprintf(stderr, " -I#/-J#  # = Number of de/compression runs (default=3)\n");
  fprintf(stderr, " -t#      # = min. time in seconds per run.(default=1sec)\n");
  fprintf(stderr, " -S#      Sleep # min. after 2 min. processing mimizing CPU trottling\n");
  fprintf(stderr, "          t = M:millisecond s:second m:minute h:hour. ex. 3h\n");
  fprintf(stderr, " -D       No process real-time priority setting\n");
  fprintf(stderr, "Check:\n");
  fprintf(stderr, " -C#      #=0 compress only, #=1 ignore errors, #=2 exit on error, #=3 crash on error\n");
  fprintf(stderr, " -f[X[c][H]][.d][s]  X = file format:\n");
  fprintf(stderr, "              [1=int8], [2=int16], [4=int32(default)], [f:float] [d:double]\n");
  fprintf(stderr, "              t = text:one integer per line, c=column number in multiple columns line\n");
  fprintf(stderr, "              c = text:integers separated by non digit char\n");
   fprintf(stderr, "         . = decimal digits (default 2)\n");
  fprintf(stderr, "          H = skip first line\n");
  fprintf(stderr, "          s = sorted (increasing), S=sorted (strictly increasing, no duplicates), z=ZigZag\n");
//  fprintf(stderr, " -z#      check reading/writing outside bounds: #=1 compress, #=2 decompress, #3:both\n");
  fprintf(stderr, "Output:\n");
  fprintf(stderr, " -v#      # = verbosity 0..3 (default 1)\n");
  fprintf(stderr, " -l#      # = 1 : print all groups/plugins, # = 2 : print all codecs\n");
  fprintf(stderr, " -S#      Plot transfer speed: #=1 Comp speedup #=2 Decomp speedup #=3 Comp 'MI/s' #=4 Decomp 'MI/s'\n");
  fprintf(stderr, " -p#      #='print format' 1=text 2=html 3=htm 4=markdown 5:vBulletin 6:csv(comma) 7=tsv(tab)\n");
  fprintf(stderr, " -Q#      # Plot window 0:1920x1080, 1:1600x900, 2:1280x720, 3:800x600 (default=1)\n");
  fprintf(stderr, " -g       -g:no merge w/ old result 'file.tbb', -gg:process w/o output (use for fuzzing)\n");
  fprintf(stderr, " -o       print on standard output\n");
  fprintf(stderr, " -G       plot memcpy\n");
  fprintf(stderr, " -1       Plot Speedup linear x-axis (default log)\n");
  fprintf(stderr, " -3       Plot Ratio/Speed logarithmic x-axis (default linear)\n");
  fprintf(stderr, "Multiblock (0=number of integers followed by integer array):\n");
  fprintf(stderr, " -Moutput concatenate all input files to multiple blocks file output\n");\
  fprintf(stderr, " -r       process multiple blocks per file (ex. gov2.sorted).\n");
  fprintf(stderr, "----- arg. ZIPF specified --------------\n");
  fprintf(stderr, " -aF     F = zipfian distribution alpha ex. -a1.0 uniform -a1.5 skewed\n");
  fprintf(stderr, " -mNs    N = minimum number. s=(k,K, m,M, g,G\n");
  fprintf(stderr, " -MNs    N = maximum number\n");
  fprintf(stderr, " -nNs    N = number of integers to generate\n");
  fprintf(stderr, "Ex. ./icbench -a1.5 -m0 -M255 -n100M ZIPF\n");
  BEUSAGE;
  fprintf(stderr, "ex. ./icbench file -eVBYTE/turbopfor ZIPF\n");
  fprintf(stderr, "ex. ./icbench -eTurboPFor -fS -r gov2.sorted\n");
  fprintf(stderr, "ex. ./icbench -eTurboPFor -fS -ft3.1H ratings.csv \n");
  exit(0);
}

struct plug pluga[255];

void printfile(char *finame, int xstdout, int fmt, char *rem) {
  int64_t totinlen;
  int       k = plugread(plug, finame, &totinlen);
  char      *p, s[256];
  if(k < 0)
    die("file open error for '%s'\n", finame);
  if(!k) return;
  strncpy(s, finame, 255);
  s[255]=0;
  if((p = strrchr(s,'.')) && !strcmp(p, ".tbb"))
    *p=0;
  plugprts(plug, k, s, xstdout, totinlen, fmt, rem);
}

int be_rand=1;
void ftest(struct plug *plug, unsigned k,unsigned n, unsigned bsize) {
  unsigned      *in,*cpy,b,i;
  unsigned char *out;
  char s[33];

  if(!n)
    n = 25000000;
  in  = malloc(n*4+OVD);    if(!in)  die("malloc err=%u", n*4);
  out = malloc(n*5+OVD);    if(!out) die("malloc err=%u", n*5);
  cpy = malloc(n*4+OVD);    if(!cpy) die("malloc err=%u", n*4);
  s[0] = 0;                                                             printf("bittest: %u-%u, n=%u\n", rm, rx, n); fflush(stdout);
  for(b = rm; b <= min(rx,64); b++) {
    struct plug *p;
    srand(time(NULL));                                                  sprintf(s,"b=%d", b);
    for(i = 0; i < n; i++) in[i] = be_rand?(rand()&((1ull << b)-1)):((1ull << b)-1);
    in[n-1] = ((1ull << b)-1);
    if(be_mindelta == 0 || be_mindelta == 1)
      for(in[0]=0,i = 1; i < n; i++) {
        uint64_t v = (uint64_t)in[i-1] + in[i] + be_mindelta;   if(v >= (1ull<<32)) die("overflow generating sorted array at %d,%x+%x->v=%llx\n", i, in[i], in[i-1], v );
        in[i] = v;
      }
    for(p = plug; p < plug+k; p++) {
      unsigned pbsize = p->blksize?p->blksize:bsize,outlen;
      if(p->lev >= 0)
        sprintf(name, "%s %d%s", p->name, p->lev, p->prm);
      else
        sprintf(name, "%s%s",    p->name,         p->prm);

      codini(n, p->id);
      if(be_mindelta >= 0)
        pbsize += 4; // start stored w. variable byte for delta coding
      p->len = p->tc = p->td = p->err = 0;                                          sprintf(s, "%d, %d", b, pbsize/4);
      outlen = plugbench(p, in, n*4, out, n*5, cpy, pbsize, plugr,tid, s);
      if(!outlen) die("Codec error or codec not available %d\n", outlen);
    }
  }
}

#define TEST64
  #ifdef TEST64

#define R64 ((uint64_t)rand())
#define RND64 ( (R64<<60) ^ (R64<<45) ^ (R64<<30) ^ (R64<<15) ^ (R64<<0) )
#define NN (4*1024*1024)

//uint64_t in[NN+256],cpy[NN+256];
uint64_t _in[NN+256],_cpy[NN+256];
unsigned char out[NN*16];

void vstest64(int id, int rm,int rx, unsigned n) {
  unsigned b,i;                                                                             fprintf(stderr,"64 bits test.id=%d n=%d [%d-%d] bits", id, n, rm, min(rx,64));
  uint64_t *in = _in,*cpy = _cpy;
  if(n > NN) n = NN;                                                                        //if(id==5) n = 128;
  for(b = rm; b <= min(rx,64); b++) {
    uint64_t start = 0, msk = b==64?0xffffffffffffffffull:(((uint64_t)1 << b)-1);
    unsigned char *op;                                                                      fprintf(stderr,"\nb=%d:", b);
    for(i = 0; i < n; i++) {
      in[i] = be_rand?(RND64&msk)>>(rand() & 3):msk; //(/*start +=*/ RND64 & msk);
                                                                                            //fprintf(stderr, "%llx,", in[i]);
      if(bsr64(in[i]) > b) die("Fatal error at b=%d ", b);
    }                                                                                       //fprintf(stderr,"\n");
    in[0]   = msk;
    in[n-1] = msk;
    switch(id) {
      case  0: fprintf(stderr,"vbenc64 ");        op = vbenc64(           in, n, out);   break;
      case  1: fprintf(stderr,"bitpack64 ");      op = bitpack64(         in, n, out, b);break;
      case  2: fprintf(stderr,"bitnpack64 ");     op = out+bitnpack64(    in, n, out);   break;
      case  3: fprintf(stderr,"bitnpack128v64 "); op = out+bitnpack128v64(in, n, out);   break;
      case  4: fprintf(stderr,"p4nenc64 ");       op = out+p4nenc64(      in, n, out);   break;
      case  5: fprintf(stderr,"p4nenc128v64 ");   op = out+p4nenc128v64(  in, n, out);   break;
      case  6: fprintf(stderr,"vsenc64 ");        op = vsenc64(           in, n, out);   break;
      case  7: fprintf(stderr,"bitndpack64 ");    op = out+bitndpack64(   in, n, out);   break;
      case  8: fprintf(stderr,"bitnd1pack64 ");   op = out+bitnd1pack64(  in, n, out);   break;
      case  9: fprintf(stderr,"bitnzpack64 ");    op = out+bitnzpack64(   in, n, out);   break;
      case 10: fprintf(stderr,"vbzenc64 ");       op = vbzenc64(          in, n, out, 0);break;
      case 11: fprintf(stderr,"p4nzenc64 ");      op = out+p4nzenc64(     in, n, out);   break;
      default: die("Fatal error: function %d not found\n", id);
      //case 5: op = efanoenc64(in, n, out, 0); break;
    }
    fprintf(stderr,"%d %.2f%% ", (int)(op-out),  ((double)(op-out)*100.0)/(double)(n*8));                                               if(op-out>sizeof(out)) die("vstest64:Overflow %d\n", op-out);
    memrcpy(cpy, in, n*sizeof(in[0]));
    switch(id) {
      case  0: vbdec64(         out, n, cpy);   break;
      case  1: bitunpack64(     out, n, cpy, b);break;
      case  2: bitnunpack64(    out, n, cpy);   break;
      case  3: bitnunpack128v64(out, n, cpy);   break;
      case  4: p4ndec64(        out, n, cpy);   break;
      case  5: p4ndec128v64(    out, n, cpy);   break;
      case  6: vsdec64(         out, n, cpy);   break;
      case  7: bitndunpack64(   out, n, cpy);   break;
      case  8: bitnd1unpack64(  out, n, cpy);   break;
      case  9: bitnzunpack64(   out, n, cpy);   break;
      case 10: vbzdec64(        out, n, cpy, 0);break;
      case 11: p4nzdec64(       out, n, cpy);   break;
      //case 5: efanodec64( out, n, cpy, 0);   break;
    }
    for(i = 0; i < n; i++) {
      if(in[i] != cpy[i]) {
        fprintf(stderr, "Error b=%d at '%d' (in=%llx,%llx,%llx,%llx,%llx,%llx,%llx,%llx cpy=%llx,%llx,%llx,%llx,%llx,%llx,%llx,%llx, out=%x,%x,%x,%x,%x,%x,%x,%x)", b, i,
          in[i],  in[i+1],  in[i+2], in[i+3], in[i+4], in[i+5], in[i+6], in[i+7],
         cpy[i], cpy[i+1], cpy[i+2],cpy[i+3],cpy[i+4],cpy[i+5],cpy[i+6],cpy[i+7],
         out[i], out[i+1], out[i+2],out[i+3],out[i+4],out[i+5],out[i+6],out[i+7]);
         break;
      }
    }
  }
  fprintf(stderr,"\n");
  exit(0);
}


void vstest16(int id, int rm,int rx, unsigned n) {
  unsigned b,i;                                                                             fprintf(stderr,"16 bits test.n=%d ", n);
  uint16_t *in = _in,*cpy = _cpy;

  if(n > NN) n = NN;                                                                        //if(id==5) n = 128;
  for(b = rm; b <= min(rx,16); b++) {
    uint64_t start = 0, msk = (1u << b)-1;
    unsigned char *op;                                                                      fprintf(stderr,"\nb=%d:", b);
    for(i = 0; i < n; i++) {
      in[i] = be_rand?rand()&msk:msk; //(/*start +=*/ RND64 & msk);                     //fprintf(stderr, ".%llx ", in[0]);
      if(bsr16(in[i]) > b) die("Fatal error at b=%d ", b);
    }
    in[0]   = msk;
    in[n-1] = msk;
    switch(id) {
      case  0: fprintf(stderr,"vbenc16 ");        op = vbenc16(           in, n, out);   break;
      case  1: fprintf(stderr,"bitpack16 ");      op = bitpack16(         in, n, out, b);break;
      case  2: fprintf(stderr,"bitnpack16 ");     op = out+bitnpack16(    in, n, out);   break;
      case  3: fprintf(stderr,"bitnpack128v16 "); op = out+bitnpack128v16(in, n, out);   break;
      case  4: fprintf(stderr,"p4nenc16 ");       op = out+p4nenc16(      in, n, out);   break;
      case  5: fprintf(stderr,"p4nenc128v16 ");   op = out+p4nenc128v16(  in, n, out);   break;
      case  6: fprintf(stderr,"vsenc16 ");        op = vsenc16(           in, n, out);   break;
      case  7: fprintf(stderr,"bitndpack16 ");    op = out+bitndpack16(   in, n, out);   break;
      case  8: fprintf(stderr,"bitnd1pack16 ");   op = out+bitnd1pack16(  in, n, out);   break;
      case  9: fprintf(stderr,"bitnzpack16 ");    op = out+bitnzpack16(   in, n, out);   break;
      case 10: fprintf(stderr,"vbzenc16 ");       op = vbzenc16(          in, n, out, 0);break;
      case 11: fprintf(stderr,"p4nzenc16 ");      op = out+p4nzenc16(     in, n, out);   break;
      default: die("Fatal error: function %d not found\n", id);
    }
    fprintf(stderr,"%d %.2f%% ", (int)(op-out),  ((double)(op-out)*100.0)/(double)(n*8));                               if(op-out>sizeof(out)) die("vstest16:Overflow %d\n", op-out);
    memrcpy(cpy, in, n*sizeof(in[0]));
    switch(id) {
      case  0: vbdec16(         out, n, cpy);   break;
      case  1: bitunpack16(     out, n, cpy, b);break;
      case  2: bitnunpack16(    out, n, cpy);   break;
      case  3: bitnunpack128v16(out, n, cpy);   break;
      case  4: p4ndec16(        out, n, cpy);   break;
      case  5: p4ndec128v16(    out, n, cpy);   break;
      case  6: vsdec16(         out, n, cpy);   break;
      case  7: bitndunpack16(   out, n, cpy);   break;
      case  8: bitnd1unpack16(  out, n, cpy);   break;
      case  9: bitnzunpack16(   out, n, cpy);   break;
      case 10: vbzdec16(        out, n, cpy, 0);break;
      case 11: p4nzdec16(       out, n, cpy);   break;
    }
    uint16_t *p = cpy;
    for(i = 0; i < n; i++)
      if(in[i] != p[i]) {
        fprintf(stderr, "Error b=%d at '%d' (in=%x,%x cpy=%x,%x)", b, i, in[i], in[i+1], p[i], p[i+1]); break;
      }
  }
  fprintf(stderr,"\n");
  exit(0);
}
  #else
#define vstest64(id,rm,rx,n)
  #endif
//extern unsigned xbits[];
char *sifmt[] = {"","s","i","z"};
  #ifdef __MINGW32__
extern int _CRT_glob=1;
  #endif
int main(int argc, char* argv[]) {
  int                xstdout=-1,xstdin=-1;
  int                recurse  = 0, xplug = 0,tm_Repk=1,plot=-1,fmt=0,fno,merge=0,rprio=1,dfmt = 0,kid=1,skiph=0,decs=2,divs=1,k,vtest=-1,id;
  unsigned           bsize    = 128*4, bsizex=0, n=25000000;
  uint64_t filenmax = 0;
  char               *scmd = NULL,*trans=NULL,*beb=NULL,*rem="",s[2049];
  char               *_argvx[1], **argvx=_argvx;

  int c, digit_optind = 0;
  for(;;) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      { "help",     0, 0, 'h'},
      { 0,          0, 0, 0}
    };
    if((c = getopt_long(argc, argv, "1234a:A:b:B:cC:d:De:E:F:f:gGHi:I:j:J:k:K:l:L:m:M:n:N:oOPp:Q:q:rRs:S:t:T:Uv:V:W:X:Y:Z:y:z:", long_options, &option_index)) == -1) break;
    switch(c) {
      case 0:
        printf("Option %s", long_options[option_index].name);
        if(optarg) printf (" with arg %s", optarg);  printf ("\n");
        break;
      case 'a': a = strtod(optarg, NULL);   break;
      case 'b': bsize    = argtoi(optarg,1)*4; bsizex++; break;
      case 'B': filenmax = argtol(optarg);           break;
      case 'C': cmp      = atoi(optarg);             break;
      case 'd': mdelta   = atoi(optarg);             break;
      case 'e': scmd     = optarg;                   break;
      case 'f': { char *s = optarg;
        dfmt = 0;
             if(*s=='c') { dfmt = T_CHAR; s++;}
        else if(*s=='t') { dfmt = T_TXT; if(*++s > '0' && *s <= '9') { kid = s[0] - '0'; s++; } }
        else if(*s=='u') { dfmt = T_TST; s++;}
        //else if(*s=='f') { dfmt = T_FLT; s++;}
        //else if(*s=='d') { dfmt = T_DBL; s++;}
        else if(*s=='1' || *s=='2') { dfmt = s[0]-'0'; s++; }
        if(*s == '.') { if(*++s >= '0' && *s <= '9') { decs = s[0] - '0'; s++; } }
        if(*s == 'v') { divs = strtod(++s, &s); }
        if(*s == 'H') { skiph++; s++; }
        switch(*s) { case 's': be_mindelta = 0; break; case 'S': be_mindelta = 1;break; case 'z': be_mindelta = 2; break; }
      } break;
      case 'F': fac      = strtod(optarg, NULL);     break;
//      case 'f': fuzz     = atoi(optarg);               break;
      case 'g': merge++;                             break;
      case 'G': plotmcpy++;                          break;

      case 'H': be_factor++;                         break;
      case 'i': if((tm_rep  = atoi(optarg))<=0)
                  tm_rep=tm_Rep=1;               break;
      case 'I': tm_Rep  = atoi(optarg);              break;
      case 'j': if((tm_rep2  = atoi(optarg))<=0)
                  tm_rep2=tm_Rep2=1;                 break;
      case 'J': tm_Rep2  = atoi(optarg);             break;
      case 'k': if((tm_Repk  = atoi(optarg))<=0) tm_rep=tm_Rep=tm_rep2=tm_Rep2=tm_Repk=1; break;
      case 'L': tm_slp   = atoi(optarg);             break;
      case 't': tm_tx    = atoi(optarg)*TM_T;        break;
      case 'T': tm_TX    = atoi(optarg)*TM_T;        break;
      case 'S': speedup  = atoi(optarg);             break;

      case 'l': xplug    = atoi(optarg);             break;
      case 'r': be_nblocks++;                            break;
      case 'o': xstdout++;                           break;
      case 'p': fmt      = atoi(optarg);             break;
      case 'P': be_mcpy++;                           break;
      case 'Q': divxy    = atoi(optarg);
                if(divxy>3) divxy=3;                 break;
      case 'D': rprio=0;                             break;
      case 's': mininlen = argtoi(optarg,Mb);        break;
      case 'v': verbose  = atoi(optarg);             break;
      case 'Y': seg_ans  = argtoi(optarg,Kb);        break;
      case 'Z': seg_huf  = argtoi(optarg,Kb);        break;
      case '1': xlog     =  xlog?0:1;                break;
      case '2': ylog     =  ylog?0:1;                break;
      case '3': xlog2    = xlog2?0:1;                break;
      case '4': ylog2    = ylog2?0:1;                break;
      case 'R': be_rand=0; break;
      case 'q' : cpuini(atoi(optarg)); break;
        #ifdef LZTURBO
      case 'c': beb      = optarg;                   break;
        #else
      case 'c': fprintf(stderr, "Option M: only in binary package available"); exit(0);
        #endif
      case 'n': n       = argtoi(optarg,1);   break;
      case 'm': rm      = argtoi(optarg,1);   break;
      case 'M': rx      = argtoi(optarg,1);  break;
      case 'y': vtest = 16; id=atoi(optarg); break;
      case 'z': vtest = 64; id=atoi(optarg); break;
      BEOPT;
      case 'h':
      default:
        usage(argv[0]);
        exit(0);
    }
  }
        if(vtest==64) vstest64(id,rm,rx,n);
   else if(vtest==16) vstest16(id,rm,rx,n);
  if(xplug) {
    xplug==1?plugsprt():plugsprtv(stdout, fmt);
    exit(0);
  }

  if(argc <= optind) {
      #ifdef _WIN32
    setmode( fileno(stdin), O_BINARY );
      #endif
    argvx[0] = "stdin";
    optind   = 0;
    argc     = 1;
    recurse  = 0;
  } else
    argvx = argv;

  if(fmt) {
    if(argc <= optind)
      die("no input file specified%s", "");
    for(fno = optind; fno < argc; fno++)
      printfile(argvx[fno], xstdout, fmt, rem);
    exit(0);
  }
  if(rprio) {
      #ifdef _WIN32
    SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
      #else
    setpriority(PRIO_PROCESS, 0, -19);
      #endif
  }
  if(!scmd) scmd = "DEFAULT";
  for(s[0] = 0;;) {
    char *q; int i;
    if(!*scmd) break;
    if(q = strchr(scmd,'/')) *q = '\0';
    for(i = 0; plugg[i].id[0]; i++)
      if(!strcmp(scmd, plugg[i].id)) {
        strcat(s, "ON/");
        strcat(s, plugg[i].name);
        strcat(s, "/OFF");
        break;
      }
    if(!plugg[i].id[0]) {
      strcat(s,scmd);
      strcat(s,"/");
    }
    scmd = q?(q+1):(char*)"";
  }
  k = plugreg(plug, s, 0, bsize, bsizex);
  if(k > 1 && argc == 1 && !strcmp(argvx[0],"stdin")) { printf("multiple codecs not allowed when reading from stdin"); exit(0); }

  { //tm_init(15,1);
  int                krep=1;
  char               *finame = "";
  int                pagesize = getpagesize();
  tm_t               tmk0 = tminit();
  struct plug *g;
  char sfiname[65];
  uint64_t totinlen=0;
  BEINI;
  if(!filenmax) filenmax = Gb;

  if(dfmt == T_TST) {     // Unit test for fixed bit sizes
    ftest(plug, k, n, bsize);
    exit(0);
  }

  for(fno = optind; fno < argc; fno++) {
    FILE *fi = NULL;
    char *q;
    size_t        outsize,insize;
    unsigned char *_in, *_cpy, *out;
    //int           inlen;
    int64_t inlen, ftotinlen=0;
    struct plug *p;
    int checks = 0;
    finame = argvx[fno];                                                                            if(verbose > 1) printf("%s\n", finame);
    if(!strcmp(finame,"ZIPF")) { sprintf(sfiname, "ZIPF%.2f_%u-%u-%u", a,rm,rx, n);
      strcat(sfiname,sifmt[be_mindelta+1]);
      finame = sfiname;
      if(!dfmt) dfmt = T_UINT32;
    } else {
      fi = strcmp(finame,"stdin")?fopen(finame, "rb"):stdin;
      if(!fi) { perror(finame); continue; /*die("open error '%s'\n", finame);*/ }
    }

    if((q = strrchr(finame, '\\')) || (q = strrchr(finame, '/'))) finame = q+1;                     if(verbose>1) printf("'%s'\n", finame);

    _in = NULL;
    if(dfmt) {
      if(!_in) { filen = inlen = befgen(&_in, n, dfmt, 4, fi, kid, skiph, decs, divs);
        insize  = min(filen,(1u<<MAP_BITS));                                            if(filen < mininlen) insize = mininlen;
        insizem = (fuzz&3)?SIZE_ROUNDUP(insize, pagesize):(insize+INOVD);
      }
    } else {
      if(finame) {
        fseeko(fi, 0, SEEK_END); filen = ftello(fi); fseeko(fi , 0 , SEEK_SET);         //if(filen > filenmax) filen = filenmax;
      } else
        filen = filenmax;
      //insize  = filen;
      insize  = min(filen,(1u<<MAP_BITS));                                          if(filen < mininlen) insize = mininlen;
      if(insize > filenmax) insize = filenmax;
      insizem = (fuzz&3)?SIZE_ROUNDUP(insize, pagesize):(insize+INOVD);

      if(insizem && !(_in = _valloc(insizem,1)))
        die("malloc error in size=%u\n", insizem);
    }
    outsize             = insize*fac + 10*Mb;
    _cpy = _in;
    out  = (unsigned char*)_valloc(outsize,2);                                      if(!out)  die("malloc error out size=%u\n", outsize);

    if((cmp || tid) && insizem && !(_cpy = _valloc(insizem,3)))
      die("malloc error cpy size=%u\n", insizem);

    for(p = plug; p < plug+k; p++) {                                                                  //{ int i; for(i=0;i<=64;i++) xbits[i]=0;}
      unsigned pbsize = p->blksize?p->blksize:bsize;
      int64_t outlen = 0;
      if(p->lev >= 0)
        sprintf(name, "%s %d%s", p->name, p->lev, p->prm);
      else
        sprintf(name, "%s%s",    p->name,         p->prm);

      codini(insize, p->id);
      if(be_mindelta >= 0)
        pbsize += 4; // start stored w. variable byte for delta coding
                                                                                    //printf("bsize=%d, ", pbsize);
      //p->len = p->tc = p->td = p->err = 0;
      blknum = 0;
      if(dfmt) {
        ftotinlen = inlen;                                             memrcpy(out, _in, inlen); if(!checks) checksort(_in,inlen/4),checks++;
        outlen    = plugbench(p, _in, inlen, out, outsize, _cpy, pbsize, plugr,tid, finame);
      } else {
        ftotinlen = 0;
        fseek(fi, 0, SEEK_SET);
        if(be_nblocks) {
          unsigned char *ip = _in;
          unsigned num;
          while(fread(&num, 1, 4, fi) == 4 && num) {           //if(num < rm || num > rx) { fseeko(fi, num*4, SEEK_CUR); continue; }
            num *= 4;
            if(ip+num >= _in+insize) {
              inlen      = ip - _in;                                    memrcpy(out, _in, inlen);if(!checks) checksort(_in,inlen/4),checks++;
              ftotinlen += inlen;
              outlen += plugbench(p, _in, inlen, out, outsize, _cpy, pbsize, plugr,tid, finame);          //if(n && outlen > n) break;
              ip = _in;
            }
            ctou32(ip) = num/4; ip += 4;
            if(fread(ip, 1, num, fi) != num)
              break;
            if(be_mindelta >=0) {
              unsigned *p = (unsigned *)ip,i;
              for(i = 1; i < num/4; i++) if(p[i] < p[i-1]+be_mindelta) { printf("Warning: IDs not sorted :%d:%d,%d\n", i, p[i-1], p[i] );break; }
            }
            ip += num;
          }
         if(inlen = ip - _in) {
            ftotinlen += inlen;
            outlen += plugbench(p, _in, inlen, out, outsize, _cpy, pbsize, plugr,tid, finame);
          }
          //if(ftotinlen >= filen) break;
        } else  while((inlen = fread(_in, 1, insize, fi)) > 0) {        memrcpy(out, _in, inlen);if(!checks) checksort(_in,inlen/4),checks++;
          ftotinlen += inlen;
          outlen += plugbench(p, _in, inlen, out, outsize, _cpy, pbsize, plugr,tid, finame);
          if(ftotinlen >= filenmax)
            break;
        }
      }
      codexit(p->id);                                                                       //if(verbose && filen > insize) plugprt(p, ftotinlen, finame, FMT_TEXT,stdout);
      //{ int i; for(i=0;i<=64;i++) if(xbits[i]) printf("%d:%d ", i, xbits[i]);printf("\n");}
    }
    totinlen += ftotinlen;
    if(fi) fclose(fi);
    dfmt?free(_in):_vfree(_in, insizem); _in = NULL;
    _vfree(out, outsize);
    if(_cpy && _cpy != _in)
      _vfree(_cpy, insizem);
  }
  BENCHSTA;

  if(argc - optind > 1) {
    unsigned clen = strpref(&argvx[optind], argc-optind, '\\', '/');
    strncpy(s, argvx[optind], clen);
    if(clen && (s[clen-1] == '/' || s[clen-1] == '\\'))
      clen--;
    s[clen] = 0;
    finame = strrchr(s,'/');
    if(!finame)
      finame = strrchr(s, '\\');
    if(!finame)
      finame = s;
    else finame++;
  } else {
    char *p;
    if((p = strrchr(finame, '\\')) || (p = strrchr(finame, '/')))
      finame = p+1;
  }
  if(!strcmp(finame,"ZIPF")) finame = sfiname;
  sprintf(s, "%s.tbb", finame);
  if(merge) {
    if(merge == 1)
      plugprts(plug, k, s, 1, totinlen, FMT_TEXT, rem);
    exit(0);
  }

  {
    int64_t _totinlen;
    int       gk = plugread(pluga, s, &_totinlen);
    FILE *fo = fopen(s, "w");
    if(_totinlen != totinlen)
      gk = 0;
    if(fo) {
      char tms[20];
      time_t tm;
      struct tm *ltm;
      struct plug *g,*p;
      time(&tm);
      ltm = localtime(&tm);
      sprintf(tms, "%.4d-%.2d-%.2d.%.2d:%.2d:%.2d", 1900 + ltm->tm_year, ltm->tm_mon+1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);

      fprintf(fo, "dataset\tsize\tcsize\tdtime\tctime\tcodec\tlevel\tparam\tcmem\tdmem\ttime\n");
      for(p = plug; p < plug+k; p++) {
        for(g = pluga; g < pluga+gk; g++)
          if(g->id >= 0 && !strcmp(g->name, p->name) && g->lev == p->lev && !strcmp(g->prm, p->prm)) {
            if(g->len == p->len) {
              int u=0;
              if(g->td < p->td || p->td < 0.01)
                p->td = g->td,u++;
              if(g->tc < p->tc || p->tc < 0.01)
                p->tc = g->tc,u++;
              if(g->memd != p->memd) u++;
              if(g->memc != p->memc) u++;
              strcpy(p->tms, u?tms:g->tms);
            }
            g->id = -1;
            break;
          }
        if(p->len)
          fprintf(fo,   "%s\t%"PRId64"\t%"PRId64"\t%.6f\t%.6f\t%s\t%d\t%s\t%"PRId64"\t%"PRId64"\t%s\n", finame, totinlen, p->len, p->td, p->tc, p->name, p->lev, p->prm[0]?p->prm:"!", p->memc, p->memd, p->tms[0]?p->tms:tms);
      }
      for(g = pluga; g < pluga+gk; g++)
        if(g->id >= 0 && g->len)
          fprintf(fo, "%s\t%"PRId64"\t%"PRId64"\t%.6f\t%.6f\t%s\t%d\t%s\t%"PRId64"\t%"PRId64"\t%s\n", finame, totinlen, g->len, g->td, g->tc, g->name, g->lev, g->prm[0]?g->prm:"?", g->memc, g->memd, g->tms[0]?g->tms:tms);
      fclose(fo);
      printfile(s, 0, FMT_TEXT, rem);
    }
  }
  }
}
