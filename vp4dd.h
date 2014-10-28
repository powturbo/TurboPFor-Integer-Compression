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

    vp4dd.h - "Integer Compression" Turbo PforDelta 
**/
unsigned char *p4ddec32( unsigned char *__restrict__ in, int n, unsigned *__restrict__ out);
unsigned char *p4ddecx32(unsigned char *__restrict__ in, int n, unsigned *__restrict__ out);

//-----------------------------------------------------------------------
#define P4D_PAD8(__x) 		( (((__x)+8-1)/8) )
#define P4D_XB(__x) 		((__x & 1)?(__x >> 8):0)
#define P4D_B(__x)  		((__x >> 1) & 0x3f)
#define P4D_ININC(__in, __x) 	__in += 1+(__x & 1)

static inline unsigned vp4dbits(unsigned char *__restrict__ in, int *xb) { unsigned i = *(unsigned short *)in; *xb = P4D_XB(i); return P4D_B(i); }

struct p4d {
  unsigned long long *xmap;
  unsigned char *ex;
  unsigned i,xb,cum[2];
  int oval,idx;
};

static inline void p4dini(struct p4d *p4d, unsigned char **__restrict__ pin, int n, unsigned *b) { unsigned char *in = *pin;
  static unsigned long long xmap[2] = { 0 };

  unsigned i  = *(unsigned short *)in;
  p4d->i      = i;
  *b = P4D_B(i);
  p4d->xb     = P4D_XB(i); 
  P4D_ININC(in,i);
  *pin = in;
 
  p4d->ex     = in + P4D_PAD8(n*(*b));
  p4d->xmap   = (i&1)?p4d->ex:xmap;
  p4d->ex    += (i&1)?16:0;
  p4d->cum[0] = 0;
  p4d->cum[1] = popcnt64(p4d->xmap[0]);  
  p4d->oval   = p4d->idx  = -1;
}

static ALWAYS_INLINE unsigned vp4dget32(struct p4d p4d, unsigned char *__restrict__ in, unsigned b, unsigned idx) { unsigned bi, cl, u = _bitgetx32(in, b, idx*b);																
  if(unlikely(p4d.xmap[bi = idx>>6] & (1ull<<(cl = idx & 0x3f)))) u |= _bitgetx32(p4d.ex, p4d.xb, (p4d.cum[bi] + popcnt64(p4d.xmap[bi] & ~((~0ull)<<cl)))*p4d.xb ) << b;
  return u;
}

static ALWAYS_INLINE unsigned vp4dget16(struct p4d p4d, unsigned char *__restrict__ in, unsigned b, unsigned idx) { unsigned bi, cl, u = _bitgetx16(in, b, idx*b);
  if(unlikely(p4d.xmap[bi = idx>>6] & (1ull<<(cl = idx & 0x3f)))) u |= _bitgetx32(p4d.ex, p4d.xb, (p4d.cum[bi] + popcnt64(p4d.xmap[bi] & ~((~0ull)<<cl)))*p4d.xb ) << b;
  return u;
}

static ALWAYS_INLINE int vp4dgeq(struct p4d *p4d, unsigned char *__restrict__ in, unsigned b, int val) { do p4d->oval += vp4dget(*p4d, in, b, ++p4d->idx)+1; while(p4d->oval < val); return p4d->oval; }

