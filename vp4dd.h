/**
    Copyright (C) powturbo 2013-2015
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
//     vp4dd.h - "Integer Compression" Turbo PforDelta 

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#define P4DSIZE 128 //64 //
#define P4DN 	(P4DSIZE/64)

//---------------- Bulk decompress of TurboPFor compressed integer array -------------------------------------------------------
// decompress a previously (with p4denc32) 32 bits packed array. Return value = end of packed buffer in 
unsigned char *p4ddec32(   unsigned char *__restrict in, unsigned n, unsigned *__restrict out);
unsigned char *p4ddec64(   unsigned char *__restrict in, unsigned n, uint64_t *__restrict out);

unsigned char *p4dd32(     unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned b, unsigned bx);
unsigned char *p4dd64(     unsigned char *__restrict in, unsigned n, uint64_t *__restrict out, unsigned b, unsigned bx);

unsigned char *p4ddecv32(  unsigned char *__restrict in, unsigned n, unsigned *__restrict out);  // SIMD

//-- delta min = 0 
unsigned char *p4dddec32(  unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start);
unsigned char *p4dddecv32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start);
//-- delta min = 1
unsigned char *p4dd1dec32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start);
unsigned char *p4dd1decv32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start);

// same as abose, b and bx not stored within the compressed stream header (see idxcr.c/idxqry.c for an example)
unsigned char *p4ddv32(    unsigned char *__restrict in, unsigned n, unsigned *__restrict out,                 unsigned b, unsigned bx);

unsigned char *p4ddd32(    unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned bx);
unsigned char *p4dddv32(   unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned bx);

unsigned char *p4dd1d32(   unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned bx);
unsigned char *p4dd1dv32(  unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start, unsigned b, unsigned bx);

//---------------- Direct Access functions to compressed TurboPFor array -------------------------------------------------------
#define P4D_PAD8(__x) 		( (((__x)+8-1)/8) )
#define P4D_B(__x)  		(((__x) >> 1) & 0x3f)
#define P4D_XB(__x) 		(((__x) & 1)?((__x) >> 8):0)
#define P4D_ININC(__in, __x) __in += 1+(__x & 1)

static inline unsigned vp4dbits(unsigned char *__restrict in, int *bx) { unsigned i = *(unsigned short *)in; *bx = P4D_XB(i); return P4D_B(i); }

struct p4d {
  unsigned long long *xmap;
  unsigned char *ex;
  unsigned i,bx,cum[P4DN+1];
  int oval,idx;
};

// prepare direct access usage
static inline void p4dini(struct p4d *p4d, unsigned char *__restrict *pin, unsigned n, unsigned *b) { unsigned char *in = *pin;
  static unsigned long long xmap[P4DN+1] = { 0 };

  unsigned i  = *(unsigned short *)in;
  p4d->i      = i;
  *b = P4D_B(i);
  p4d->bx     = P4D_XB(i); 
  P4D_ININC(in,i);
  *pin = in;
 
  p4d->ex     = in + P4D_PAD8(n*(*b));
  p4d->xmap   = (i&1)?(unsigned long long *)p4d->ex:xmap;
  p4d->ex    += (i&1)?8*P4DN:0;
  p4d->cum[0] = 0;
  for(i=1; i < P4DN; i++) p4d->cum[i] = p4d->cum[i-1] + popcnt64(p4d->xmap[i-1]);  
  p4d->oval   = p4d->idx  = -1;
}

// Get a single value with index "idx" from a p4denc32 packed array
static ALWAYS_INLINE unsigned vp4dget32(struct p4d *p4d, unsigned char *__restrict in, unsigned b, unsigned idx) { unsigned bi, cl, u = _bitgetx32(in, b, idx*b);																
  if(unlikely(p4d->xmap[bi = idx>>6] & (1ull<<(cl = (idx & 0x3f))))) u |= _bitgetx32(p4d->ex, p4d->bx, (p4d->cum[bi] + popcnt64(p4d->xmap[bi] & ~((~0ull)<<cl)))*p4d->bx ) << b;
  return u;
}

// like vp4dget32 but for 16 bits packed array (with p4denc16)	
static ALWAYS_INLINE unsigned vp4dget16(struct p4d *p4d, unsigned char *__restrict in, unsigned b, unsigned idx) { unsigned bi, cl, u = _bitgetx16(in, b, idx*b);
  if(unlikely(p4d->xmap[bi = idx>>6] & (1ull<<(cl = (idx & 0x3f))))) u |= _bitgetx32(p4d->ex, p4d->bx, (p4d->cum[bi] + popcnt64(p4d->xmap[bi] & ~((~0ull)<<cl)))*p4d->bx ) << b;
  return u;
}

// Get the next single value greater of equal to val
static ALWAYS_INLINE int vp4dgeq(struct p4d *p4d, unsigned char *__restrict in, unsigned b, int val) { do p4d->oval += vp4dget32(p4d, in, b, ++p4d->idx)+1; while(p4d->oval < val); return p4d->oval; }

/* like p4ddec32 but using direct access. This is only a demo showing direct access usage. Use p4ddec32 for instead for decompressing entire blocks */
unsigned char *p4ddecx32(  unsigned char *__restrict in, unsigned n, unsigned *__restrict out);
unsigned char *p4dfdecx32( unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start);
unsigned char *p4df0decx32(unsigned char *__restrict in, unsigned n, unsigned *__restrict out, unsigned start);

#define P4DSIZE 128 //64 //
#define P4DN 	(P4DSIZE/64)
#ifdef __cplusplus
}
#endif
