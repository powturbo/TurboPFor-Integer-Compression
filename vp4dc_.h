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

    vp4dc_.c - "Integer Compression" Turbo PforDelta 
**/
#define uint_t TEMPLATE3(uint, USIZE, _t)
#define P4DN 	(P4DSIZE/64)

unsigned char *TEMPLATE2(P4DENC, USIZE)(uint_t *__restrict in, int n, unsigned char *__restrict out) {  unsigned char *op = out;
  int i,b=0; unsigned cnt[USIZE+1] = {0}; uint_t *ip; 
  
  for(ip = in; ip < in+(n&~3); ) { 
    ++cnt[TEMPLATE2(bsr, USIZE)(*ip)]; b |= *ip++;
    ++cnt[TEMPLATE2(bsr, USIZE)(*ip)]; b |= *ip++;
    ++cnt[TEMPLATE2(bsr, USIZE)(*ip)]; b |= *ip++;
    ++cnt[TEMPLATE2(bsr, USIZE)(*ip)]; b |= *ip++; 
  }
  while(ip < in+n) b |= *ip, ++cnt[TEMPLATE2(bsr, USIZE)(*ip++)];
  b = TEMPLATE2(bsr, USIZE)(b); 

  unsigned xb = b, ml = PAD8(n*b)+1,x = cnt[b];
  for(i = b-1; i >= 0; i--) { 
    unsigned l = PAD8(n*i) + 2+P4DN*8+PAD8(x*(xb-i)); 
    if(l < ml) b = i, ml = l; 
    x += cnt[i]; /*if(x >= 64) break;*/ 
  }
  if(xb == b) { 
    *out++ = b << 1; 
    return TEMPLATE2(bitpack, USIZE)(in,  n,  b, out); 
  }
  xb -= b;
  uint_t _in[P4DSIZE], inx[P4DSIZE*2]; 
  unsigned miss[P4DSIZE],xn, msk = (1ull<<b)-1;
  unsigned long long xmap[P4DN+1];  for(i = 0; i < P4DN; i++) xmap[i] = 0;
  for(xn = i = 0; i < n; i++) {
    _in[i]    = in[i] & msk; 
    miss[xn]  = i; 
    xn       += in[i] > msk; 
  }  
  for(i = 0; i < xn; i++) { 
    unsigned c = miss[i]; 
    inx[i] = in[c] >> b; 
    xmap[c>>6] |= (1ull << (c&0x3f)); 
  }
  *(unsigned short *)out = xb << 8 | b << 1 | 1; out += 2; out = TEMPLATE2(bitpack, USIZE)(_in,  n,  b, out);	
  for(i=0;i < P4DN; i++) { *(unsigned long long *)out = xmap[i]; out += 8; }   //memset(&inx[xn],0,P4DSIZE); 
  return TEMPLATE2(bitpack, USIZE)(inx, xn, xb, out);  //if(op-out >= PAD8(n*b)+1) { printf("Fatal error b=%d,xb=%d\n", b, xb); exit(0); }  return out;
}

