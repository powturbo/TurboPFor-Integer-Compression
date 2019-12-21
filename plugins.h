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
//     TurboPFor: plugins.h - settings
#define G_N  0x1
#define G_D0 0x2
#define G_D1 0x4
#define G_Z  0x8
#define G_D  (G_D1|G_D0)

struct codecs {
  int  coid;
  char *name,*url,*ver;
};

struct plugs {
  int  id;
  char *name;
  unsigned codec,blksize,flag;
  char *lev,*desc;
};

struct plugg {
  char id[17],*desc,*name;
};

  #ifdef __cplusplus
extern "C" {
  #endif
extern struct plugg plugg[];
extern struct codecs codecs[];
extern struct plugs plugs[];
int  codini(size_t insize, int codec);
void codexit(int codec);
int  codstart( unsigned char *in, int inlen, int codec);
unsigned char *codcomp(   unsigned char *in, unsigned n, unsigned char *out, int outsize, int codec, int lev, char *prm, int b);
unsigned char *coddecomp( unsigned char *in, unsigned n, unsigned char *out, int outlen,  int codec, int lev, char *prm, int b);

unsigned char *codcomps(  unsigned char *in, unsigned n, unsigned char *out, int outsize, int codec, int lev, char *prm, int mdelta);
unsigned char *coddecomps(unsigned char *in, unsigned n, unsigned char *out, int outlen,  int codec, int lev, char *prm, int mdelta);

unsigned char *codcompz(  unsigned char *in, unsigned n, unsigned char *out, int outsize, int codec, int lev, char *prm, int mdelta);
unsigned char *coddecompz(unsigned char *in, unsigned n, unsigned char *out, int outlen,  int codec, int lev, char *prm, int mdelta);

char *codver(int codec, char *v, char *s);
void *_valloc(size_t size, int a);
void _vfree(void *p, size_t size);

typedef unsigned char *(*CODCOMP)(  unsigned char *_in, unsigned _n, unsigned char *out, int outsize, int codec, int lev, char *prm, int b);
typedef unsigned char *(*CODDECOMP)(unsigned char *in,  unsigned _n, unsigned char *_out, int outlen, int codec, int lev, char *prm, int b);

  #ifdef __cplusplus
}
  #endif
