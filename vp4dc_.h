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

    vp4dc_.c - "Integer Compression" Turbo PforDelta 
**/
#define uint_t TEMPLATE3(uint, USIZE, _t)

unsigned char *TEMPLATE2(p4denc, USIZE)(uint_t *__restrict__ in, int n, unsigned char *__restrict__ out) { 
  int i; unsigned cnt[USIZE+1] = {0}; uint_t b = 0; 
  for(i = 0; i < n; i++) b |= in[i], ++cnt[TEMPLATE2(bsr, USIZE)(in[i])]; 
  b = TEMPLATE2(bsr, USIZE)(b); 

  unsigned xb=b, ml = PAD8(n*b)+1,x = cnt[b];
  for(i = b-1; i >= 0; i--) { 
    unsigned l = PAD8(n*i) + (x?(2+16+PAD8(x*(xb-i))):1); 
    if(l < ml) b = i, ml = l; 
    x += cnt[i]; /*if(x >= 64) break;*/ 
  }
  if(xb == b) { 
    *out++ = b << 1; 
    return TEMPLATE2(bitpack, USIZE)(in,  n,  b, out); 
  }
  xb-=b;
  uint_t _in[0x100], inx[0x100]; unsigned miss[0x100];
  unsigned long long xmap[2]; xmap[0] = xmap[1] = 0; unsigned xn, msk = (1ull<<b)-1;
  for(xn = i = 0; i < n; i++) {
    _in[i]    = in[i] & msk; 
    miss[xn]  = i; 
    xn       += in[i] > msk; 
  }  
  for(i = 0; i < xn; i++) { 
    unsigned c = miss[i]; 
    inx[i] = in[c] >> b; 
    xmap[c>>6] |= (1ull<<(c&0x3f)); 
  }
  *(unsigned short *)out = xb << 8 | b << 1 | 1; out += 2; out = TEMPLATE2(bitpack, USIZE)(_in,  n,  b, out);					
  *(unsigned long long *)out = xmap[0]; 	 out += 8; 
  *(unsigned long long *)out = xmap[1]; 	 out += 8; 
  memset(&inx[xn],0,128); 
  return TEMPLATE2(bitpack, USIZE)(inx, xn, xb, out);
}

