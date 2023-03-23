// Copyright (c) 2008, WEST, Polytechnic Institute of NYU.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of WEST, Polytechnic Institute of NYU. nor the names 
// of its contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: Torsten Suel, Jiangong Zhang, Jinru He
// 
// If you have any questions or problems about our codes, please contact:
// jhe@cis.poly.edu
// 
// 

//#include "rice_coding2.h"
//#include <stdio.h>

/*rc_rice_coding2() {
	// TODO Auto-generated constructor stub
	cnum[0] = 0;
	cnum[1] = 1;
	cnum[2] = 2;
	cnum[3] = 3;
	cnum[4] = 4;
	cnum[5] = 5;
	cnum[6] = 6;
	cnum[7] = 7;
	cnum[8] = 8;
	cnum[9] = 9;
	cnum[10] = 10;
	cnum[11] = 11;
	cnum[12] = 12;
	cnum[13] = 13;
	cnum[14] = 16;
	cnum[15] = 20;
	cnum[16] = 32;
}*/
#define	coding_type 3
#define block_size 128

static int cnum[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,16,20,32 };

/*rc_~rice_coding2() {
	// TODO Auto-generated destructor stub
}*/

/*int rc_get_type()
{
	return coding_type;
}

void rc_set_size(int size)
{
	this->block_size = size;
}*/
//void pack(unsigned int *v, unsigned int b, unsigned int n, unsigned int *w);
#include "../bitpack.h"
#include "rc.h"

	void setBit(unsigned char *buf, unsigned int *bp, unsigned int val)
	{                                     
	  unsigned int bPtr;
	  unsigned int w;

	  bPtr = (*bp)&7;
	  if (bPtr == 0)  buf[(*bp)>>3] = 0;
	  if (val == 1)  buf[(*bp)>>3] |= (1<<bPtr);
	  (*bp)++;
	}

/**********************
 * w: output buffer
 * buf: input buffer
 * bits: b value;
 * BS: block size
 */
unsigned char *rc_turbo_rice_encode(unsigned *w, unsigned int **buf, unsigned int bits)
{
  unsigned int bp;
  unsigned int val;
  unsigned int i;
  int s;
  unsigned int out[block_size];

  if (bits > 0)
  {
    s = ((bits * block_size)>>5);
    for (i = 0; i < s; i++)  w[i] = 0;
    for (i = 0; i < block_size; i++)  out[i] = (*buf)[i] & ((1u<<bits)-1);// MASK[bits];
    //pack(out, bits, block_size, *w);
	w = bitpack32(out, block_size, w, bits); 
    //*w += s;
  }

  if (bits < 32)
  {
    for (i = 0, bp = 0; i < block_size; i++)
    {
      for (val = (((*buf)[i])>>bits); val > 0; val--)
        setBit((unsigned char *)(w), &bp, 1);

      setBit((unsigned char *)(w), &bp, 0);
    }
    w += (bp>>5);
    if (bp&31) w += 1;
  }
  *buf += block_size;
  return w;
}

unsigned char *rcenc32(unsigned* input, int size, unsigned* output)
{
	int i,s, f = 0;
	int fres;
	unsigned int bb = 0;
	unsigned int bp;
	int m = 0;
	//block_size = size;
	for (i = 0, bb = 0; i < block_size; i++)
	{
		//printf("%d\n", input[i]);
		bb += input[i];
	}

	bb = 100 * (bb / block_size) / 100;
		//printf("bb=%d, ", bb);
	int b;
	for (b = 0; bb > 0; bb = (bb>>1)) b++;



	if (b > 0) b--;

	//cout << b << endl;
	for (fres = 0; cnum[fres] < b; fres++);//{printf("%d, %d\n", cnum[fres], fres);}

	b = cnum[fres];
	//printf("b:%d, f:%d\n", b,fres);

	unsigned* tmp = input;
	unsigned* tmp2 = output+1;
	tmp2 = rc_turbo_rice_encode(tmp2, &tmp, b);
	*output = (unsigned)((b<<6)|fres);

	return tmp2;// - output;
}

unsigned char *rc_turbo_rice_decode(unsigned int *w, unsigned int *buf,
                       unsigned int bits, unsigned int flag)

{
  unsigned int i;
  unsigned char b;
  unsigned char *ww;
  unsigned int val;

  //(unpack[flag])(buf, *w, block_size);
  w = bitunpack32(w, block_size, (unsigned char *)buf, flag);
  //*w += ((bits * block_size)>>5);

if (bits < 32)
{
  i = 0;
  ww = (unsigned char *)(w);
  val = 1<<bits;
  while (i < block_size)
  {
    b = *(ww++);
    switch (b)
    {
      case 0:
        i += 8;
        break;

      case 1:
        buf[i] += val;
        i += 7;
        break;

      case 2:
        buf[i+1] += val;
        i += 7;
        break;

      case 3:
        buf[i] += 2 * val;
        i += 6;
        break;

      case 4:
        buf[i+2] += val;
        i += 7;
        break;

      case 5:
        buf[i] += val;
        buf[i+1] += val;
        i += 6;
        break;

      case 6:
        buf[i+1] += 2 * val;
        i += 6;
        break;

      case 7:
        buf[i] += 3 * val;
        i += 5;
        break;

      case 8:
        buf[i+3] += val;
        i += 7;
        break;

      case 9:
        buf[i] += val;
        buf[i+2] += val;
        i += 6;
        break;

      case 10:
        buf[i+1] += val;
        buf[i+2] += val;
        i += 6;
        break;

      case 11:
        buf[i] += 2 * val;
        buf[i+1] += val;
        i += 5;
        break;

      case 12:
        buf[i+2] += 2 * val;
        i += 6;
        break;

      case 13:
        buf[i] += val;
        buf[i+1] += 2 * val;
        i += 5;
        break;

      case 14:
        buf[i+1] += 3 * val;
        i += 5;
        break;

      case 15:
        buf[i] += 4 * val;
        i += 4;
        break;

      case 16:
        buf[i+4] += val;
        i += 7;
        break;

      case 17:
        buf[i] += val;
        buf[i+3] += val;
        i += 6;
        break;

      case 18:
        buf[i+1] += val;
        buf[i+3] += val;
        i += 6;
        break;

      case 19:
        buf[i] += 2 * val;
        buf[i+2] += val;
        i += 5;
        break;

      case 20:
        buf[i+2] += val;
        buf[i+3] += val;
        i += 6;
        break;

      case 21:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+2] += val;
        i += 5;
        break;

      case 22:
        buf[i+1] += 2 * val;
        buf[i+2] += val;
        i += 5;
        break;

      case 23:
        buf[i] += 3 * val;
        buf[i+1] += val;
        i += 4;
        break;

      case 24:
        buf[i+3] += 2 * val;
        i += 6;
        break;

      case 25:
        buf[i] += val;
        buf[i+2] += 2 * val;
        i += 5;
        break;

      case 26:
        buf[i+1] += val;
        buf[i+2] += 2 * val;
        i += 5;
        break;

      case 27:
        buf[i] += 2 * val;
        buf[i+1] += 2 * val;
        i += 4;
        break;

      case 28:
        buf[i+2] += 3 * val;
        i += 5;
        break;

      case 29:
        buf[i] += val;
        buf[i+1] += 3 * val;
        i += 4;
        break;

      case 30:
        buf[i+1] += 4 * val;
        i += 4;
        break;

      case 31:
        buf[i] += 5 * val;
        i += 3;
        break;

      case 32:
        buf[i+5] += val;
        i += 7;
        break;

      case 33:
        buf[i] += val;
        buf[i+4] += val;
        i += 6;
        break;

      case 34:
        buf[i+1] += val;
        buf[i+4] += val;
        i += 6;
        break;

      case 35:
        buf[i] += 2 * val;
        buf[i+3] += val;
        i += 5;
        break;

      case 36:
        buf[i+2] += val;
        buf[i+4] += val;
        i += 6;
        break;

      case 37:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+3] += val;
        i += 5;
        break;

      case 38:
        buf[i+1] += 2 * val;
        buf[i+3] += val;
        i += 5;
        break;

      case 39:
        buf[i] += 3 * val;
        buf[i+2] += val;
        i += 4;
        break;

      case 40:
        buf[i+3] += val;
        buf[i+4] += val;
        i += 6;
        break;

      case 41:
        buf[i] += val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 5;
        break;

      case 42:
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 5;
        break;

      case 43:
        buf[i] += 2 * val;
        buf[i+1] += val;
        buf[i+2] += val;
        i += 4;
        break;

      case 44:
        buf[i+2] += 2 * val;
        buf[i+3] += val;
        i += 5;
        break;

      case 45:
        buf[i] += val;
        buf[i+1] += 2 * val;
        buf[i+2] += val;
        i += 4;
        break;

      case 46:
        buf[i+1] += 3 * val;
        buf[i+2] += val;
        i += 4;
        break;

      case 47:
        buf[i] += 4 * val;
        buf[i+1] += val;
        i += 3;
        break;

      case 48:
        buf[i+4] += 2 * val;
        i += 6;
        break;

      case 49:
        buf[i] += val;
        buf[i+3] += 2 * val;
        i += 5;
        break;

      case 50:
        buf[i+1] += val;
        buf[i+3] += 2 * val;
        i += 5;
        break;

      case 51:
        buf[i] += 2 * val;
        buf[i+2] += 2 * val;
        i += 4;
        break;

      case 52:
        buf[i+2] += val;
        buf[i+3] += 2 * val;
        i += 5;
        break;

      case 53:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+2] += 2 * val;
        i += 4;
        break;

      case 54:
        buf[i+1] += 2 * val;
        buf[i+2] += 2 * val;
        i += 4;
        break;

      case 55:
        buf[i] += 3 * val;
        buf[i+1] += 2 * val;
        i += 3;
        break;

      case 56:
        buf[i+3] += 3 * val;
        i += 5;
        break;

      case 57:
        buf[i] += val;
        buf[i+2] += 3 * val;
        i += 4;
        break;

      case 58:
        buf[i+1] += val;
        buf[i+2] += 3 * val;
        i += 4;
        break;

      case 59:
        buf[i] += 2 * val;
        buf[i+1] += 3 * val;
        i += 3;
        break;

      case 60:
        buf[i+2] += 4 * val;
        i += 4;
        break;

      case 61:
        buf[i] += val;
        buf[i+1] += 4 * val;
        i += 3;
        break;

      case 62:
        buf[i+1] += 5 * val;
        i += 3;
        break;

      case 63:
        buf[i] += 6 * val;
        i += 2;
        break;

      case 64:
        buf[i+6] += val;
        i += 7;
        break;

      case 65:
        buf[i] += val;
        buf[i+5] += val;
        i += 6;
        break;

      case 66:
        buf[i+1] += val;
        buf[i+5] += val;
        i += 6;
        break;

      case 67:
        buf[i] += 2 * val;
        buf[i+4] += val;
        i += 5;
        break;

      case 68:
        buf[i+2] += val;
        buf[i+5] += val;
        i += 6;
        break;

      case 69:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+4] += val;
        i += 5;
        break;

      case 70:
        buf[i+1] += 2 * val;
        buf[i+4] += val;
        i += 5;
        break;

      case 71:
        buf[i] += 3 * val;
        buf[i+3] += val;
        i += 4;
        break;

      case 72:
        buf[i+3] += val;
        buf[i+5] += val;
        i += 6;
        break;

      case 73:
        buf[i] += val;
        buf[i+2] += val;
        buf[i+4] += val;
        i += 5;
        break;

      case 74:
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+4] += val;
        i += 5;
        break;

      case 75:
        buf[i] += 2 * val;
        buf[i+1] += val;
        buf[i+3] += val;
        i += 4;
        break;

      case 76:
        buf[i+2] += 2 * val;
        buf[i+4] += val;
        i += 5;
        break;

      case 77:
        buf[i] += val;
        buf[i+1] += 2 * val;
        buf[i+3] += val;
        i += 4;
        break;

      case 78:
        buf[i+1] += 3 * val;
        buf[i+3] += val;
        i += 4;
        break;

      case 79:
        buf[i] += 4 * val;
        buf[i+2] += val;
        i += 3;
        break;

      case 80:
        buf[i+4] += val;
        buf[i+5] += val;
        i += 6;
        break;

      case 81:
        buf[i] += val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 5;
        break;

      case 82:
        buf[i+1] += val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 5;
        break;

      case 83:
        buf[i] += 2 * val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 4;
        break;

      case 84:
        buf[i+2] += val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 5;
        break;

      case 85:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 4;
        break;

      case 86:
        buf[i+1] += 2 * val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 4;
        break;

      case 87:
        buf[i] += 3 * val;
        buf[i+1] += val;
        buf[i+2] += val;
        i += 3;
        break;

      case 88:
        buf[i+3] += 2 * val;
        buf[i+4] += val;
        i += 5;
        break;

      case 89:
        buf[i] += val;
        buf[i+2] += 2 * val;
        buf[i+3] += val;
        i += 4;
        break;

      case 90:
        buf[i+1] += val;
        buf[i+2] += 2 * val;
        buf[i+3] += val;
        i += 4;
        break;

      case 91:
        buf[i] += 2 * val;
        buf[i+1] += 2 * val;
        buf[i+2] += val;
        i += 3;
        break;

      case 92:
        buf[i+2] += 3 * val;
        buf[i+3] += val;
        i += 4;
        break;

      case 93:
        buf[i] += val;
        buf[i+1] += 3 * val;
        buf[i+2] += val;
        i += 3;
        break;

      case 94:
        buf[i+1] += 4 * val;
        buf[i+2] += val;
        i += 3;
        break;

      case 95:
        buf[i] += 5 * val;
        buf[i+1] += val;
        i += 2;
        break;

      case 96:
        buf[i+5] += 2 * val;
        i += 6;
        break;

      case 97:
        buf[i] += val;
        buf[i+4] += 2 * val;
        i += 5;
        break;

      case 98:
        buf[i+1] += val;
        buf[i+4] += 2 * val;
        i += 5;
        break;

      case 99:
        buf[i] += 2 * val;
        buf[i+3] += 2 * val;
        i += 4;
        break;

      case 100:
        buf[i+2] += val;
        buf[i+4] += 2 * val;
        i += 5;
        break;

      case 101:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+3] += 2 * val;
        i += 4;
        break;

      case 102:
        buf[i+1] += 2 * val;
        buf[i+3] += 2 * val;
        i += 4;
        break;

      case 103:
        buf[i] += 3 * val;
        buf[i+2] += 2 * val;
        i += 3;
        break;

      case 104:
        buf[i+3] += val;
        buf[i+4] += 2 * val;
        i += 5;
        break;

      case 105:
        buf[i] += val;
        buf[i+2] += val;
        buf[i+3] += 2 * val;
        i += 4;
        break;

      case 106:
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+3] += 2 * val;
        i += 4;
        break;

      case 107:
        buf[i] += 2 * val;
        buf[i+1] += val;
        buf[i+2] += 2 * val;
        i += 3;
        break;

      case 108:
        buf[i+2] += 2 * val;
        buf[i+3] += 2 * val;
        i += 4;
        break;

      case 109:
        buf[i] += val;
        buf[i+1] += 2 * val;
        buf[i+2] += 2 * val;
        i += 3;
        break;

      case 110:
        buf[i+1] += 3 * val;
        buf[i+2] += 2 * val;
        i += 3;
        break;

      case 111:
        buf[i] += 4 * val;
        buf[i+1] += 2 * val;
        i += 2;
        break;

      case 112:
        buf[i+4] += 3 * val;
        i += 5;
        break;

      case 113:
        buf[i] += val;
        buf[i+3] += 3 * val;
        i += 4;
        break;

      case 114:
        buf[i+1] += val;
        buf[i+3] += 3 * val;
        i += 4;
        break;

      case 115:
        buf[i] += 2 * val;
        buf[i+2] += 3 * val;
        i += 3;
        break;

      case 116:
        buf[i+2] += val;
        buf[i+3] += 3 * val;
        i += 4;
        break;

      case 117:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+2] += 3 * val;
        i += 3;
        break;

      case 118:
        buf[i+1] += 2 * val;
        buf[i+2] += 3 * val;
        i += 3;
        break;

      case 119:
        buf[i] += 3 * val;
        buf[i+1] += 3 * val;
        i += 2;
        break;

      case 120:
        buf[i+3] += 4 * val;
        i += 4;
        break;

      case 121:
        buf[i] += val;
        buf[i+2] += 4 * val;
        i += 3;
        break;

      case 122:
        buf[i+1] += val;
        buf[i+2] += 4 * val;
        i += 3;
        break;

      case 123:
        buf[i] += 2 * val;
        buf[i+1] += 4 * val;
        i += 2;
        break;

      case 124:
        buf[i+2] += 5 * val;
        i += 3;
        break;

      case 125:
        buf[i] += val;
        buf[i+1] += 5 * val;
        i += 2;
        break;

      case 126:
        buf[i+1] += 6 * val;
        i += 2;
        break;

      case 127:
        buf[i] += 7 * val;
        i += 1;
        break;

      case 128:
        buf[i+7] += val;
        i += 7;
        break;

      case 129:
        buf[i] += val;
        buf[i+6] += val;
        i += 6;
        break;

      case 130:
        buf[i+1] += val;
        buf[i+6] += val;
        i += 6;
        break;

      case 131:
        buf[i] += 2 * val;
        buf[i+5] += val;
        i += 5;
        break;

      case 132:
        buf[i+2] += val;
        buf[i+6] += val;
        i += 6;
        break;

      case 133:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 134:
        buf[i+1] += 2 * val;
        buf[i+5] += val;
        i += 5;
        break;

      case 135:
        buf[i] += 3 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 136:
        buf[i+3] += val;
        buf[i+6] += val;
        i += 6;
        break;

      case 137:
        buf[i] += val;
        buf[i+2] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 138:
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 139:
        buf[i] += 2 * val;
        buf[i+1] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 140:
        buf[i+2] += 2 * val;
        buf[i+5] += val;
        i += 5;
        break;

      case 141:
        buf[i] += val;
        buf[i+1] += 2 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 142:
        buf[i+1] += 3 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 143:
        buf[i] += 4 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 144:
        buf[i+4] += val;
        buf[i+6] += val;
        i += 6;
        break;

      case 145:
        buf[i] += val;
        buf[i+3] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 146:
        buf[i+1] += val;
        buf[i+3] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 147:
        buf[i] += 2 * val;
        buf[i+2] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 148:
        buf[i+2] += val;
        buf[i+3] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 149:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 150:
        buf[i+1] += 2 * val;
        buf[i+2] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 151:
        buf[i] += 3 * val;
        buf[i+1] += val;
        buf[i+3] += val;
        i += 3;
        break;

      case 152:
        buf[i+3] += 2 * val;
        buf[i+5] += val;
        i += 5;
        break;

      case 153:
        buf[i] += val;
        buf[i+2] += 2 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 154:
        buf[i+1] += val;
        buf[i+2] += 2 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 155:
        buf[i] += 2 * val;
        buf[i+1] += 2 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 156:
        buf[i+2] += 3 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 157:
        buf[i] += val;
        buf[i+1] += 3 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 158:
        buf[i+1] += 4 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 159:
        buf[i] += 5 * val;
        buf[i+2] += val;
        i += 2;
        break;

      case 160:
        buf[i+5] += val;
        buf[i+6] += val;
        i += 6;
        break;

      case 161:
        buf[i] += val;
        buf[i+4] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 162:
        buf[i+1] += val;
        buf[i+4] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 163:
        buf[i] += 2 * val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 164:
        buf[i+2] += val;
        buf[i+4] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 165:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 166:
        buf[i+1] += 2 * val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 167:
        buf[i] += 3 * val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 3;
        break;

      case 168:
        buf[i+3] += val;
        buf[i+4] += val;
        buf[i+5] += val;
        i += 5;
        break;

      case 169:
        buf[i] += val;
        buf[i+2] += val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 170:
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 171:
        buf[i] += 2 * val;
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 3;
        break;

      case 172:
        buf[i+2] += 2 * val;
        buf[i+3] += val;
        buf[i+4] += val;
        i += 4;
        break;

      case 173:
        buf[i] += val;
        buf[i+1] += 2 * val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 3;
        break;

      case 174:
        buf[i+1] += 3 * val;
        buf[i+2] += val;
        buf[i+3] += val;
        i += 3;
        break;

      case 175:
        buf[i] += 4 * val;
        buf[i+1] += val;
        buf[i+2] += val;
        i += 2;
        break;

      case 176:
        buf[i+4] += 2 * val;
        buf[i+5] += val;
        i += 5;
        break;

      case 177:
        buf[i] += val;
        buf[i+3] += 2 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 178:
        buf[i+1] += val;
        buf[i+3] += 2 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 179:
        buf[i] += 2 * val;
        buf[i+2] += 2 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 180:
        buf[i+2] += val;
        buf[i+3] += 2 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 181:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+2] += 2 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 182:
        buf[i+1] += 2 * val;
        buf[i+2] += 2 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 183:
        buf[i] += 3 * val;
        buf[i+1] += 2 * val;
        buf[i+2] += val;
        i += 2;
        break;

      case 184:
        buf[i+3] += 3 * val;
        buf[i+4] += val;
        i += 4;
        break;

      case 185:
        buf[i] += val;
        buf[i+2] += 3 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 186:
        buf[i+1] += val;
        buf[i+2] += 3 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 187:
        buf[i] += 2 * val;
        buf[i+1] += 3 * val;
        buf[i+2] += val;
        i += 2;
        break;

      case 188:
        buf[i+2] += 4 * val;
        buf[i+3] += val;
        i += 3;
        break;

      case 189:
        buf[i] += val;
        buf[i+1] += 4 * val;
        buf[i+2] += val;
        i += 2;
        break;

      case 190:
        buf[i+1] += 5 * val;
        buf[i+2] += val;
        i += 2;
        break;

      case 191:
        buf[i] += 6 * val;
        buf[i+1] += val;
        i += 1;
        break;

      case 192:
        buf[i+6] += 2 * val;
        i += 6;
        break;

      case 193:
        buf[i] += val;
        buf[i+5] += 2 * val;
        i += 5;
        break;

      case 194:
        buf[i+1] += val;
        buf[i+5] += 2 * val;
        i += 5;
        break;

      case 195:
        buf[i] += 2 * val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 196:
        buf[i+2] += val;
        buf[i+5] += 2 * val;
        i += 5;
        break;

      case 197:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 198:
        buf[i+1] += 2 * val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 199:
        buf[i] += 3 * val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 200:
        buf[i+3] += val;
        buf[i+5] += 2 * val;
        i += 5;
        break;

      case 201:
        buf[i] += val;
        buf[i+2] += val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 202:
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 203:
        buf[i] += 2 * val;
        buf[i+1] += val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 204:
        buf[i+2] += 2 * val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 205:
        buf[i] += val;
        buf[i+1] += 2 * val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 206:
        buf[i+1] += 3 * val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 207:
        buf[i] += 4 * val;
        buf[i+2] += 2 * val;
        i += 2;
        break;

      case 208:
        buf[i+4] += val;
        buf[i+5] += 2 * val;
        i += 5;
        break;

      case 209:
        buf[i] += val;
        buf[i+3] += val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 210:
        buf[i+1] += val;
        buf[i+3] += val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 211:
        buf[i] += 2 * val;
        buf[i+2] += val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 212:
        buf[i+2] += val;
        buf[i+3] += val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 213:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 214:
        buf[i+1] += 2 * val;
        buf[i+2] += val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 215:
        buf[i] += 3 * val;
        buf[i+1] += val;
        buf[i+2] += 2 * val;
        i += 2;
        break;

      case 216:
        buf[i+3] += 2 * val;
        buf[i+4] += 2 * val;
        i += 4;
        break;

      case 217:
        buf[i] += val;
        buf[i+2] += 2 * val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 218:
        buf[i+1] += val;
        buf[i+2] += 2 * val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 219:
        buf[i] += 2 * val;
        buf[i+1] += 2 * val;
        buf[i+2] += 2 * val;
        i += 2;
        break;

      case 220:
        buf[i+2] += 3 * val;
        buf[i+3] += 2 * val;
        i += 3;
        break;

      case 221:
        buf[i] += val;
        buf[i+1] += 3 * val;
        buf[i+2] += 2 * val;
        i += 2;
        break;

      case 222:
        buf[i+1] += 4 * val;
        buf[i+2] += 2 * val;
        i += 2;
        break;

      case 223:
        buf[i] += 5 * val;
        buf[i+1] += 2 * val;
        i += 1;
        break;

      case 224:
        buf[i+5] += 3 * val;
        i += 5;
        break;

      case 225:
        buf[i] += val;
        buf[i+4] += 3 * val;
        i += 4;
        break;

      case 226:
        buf[i+1] += val;
        buf[i+4] += 3 * val;
        i += 4;
        break;

      case 227:
        buf[i] += 2 * val;
        buf[i+3] += 3 * val;
        i += 3;
        break;

      case 228:
        buf[i+2] += val;
        buf[i+4] += 3 * val;
        i += 4;
        break;

      case 229:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+3] += 3 * val;
        i += 3;
        break;

      case 230:
        buf[i+1] += 2 * val;
        buf[i+3] += 3 * val;
        i += 3;
        break;

      case 231:
        buf[i] += 3 * val;
        buf[i+2] += 3 * val;
        i += 2;
        break;

      case 232:
        buf[i+3] += val;
        buf[i+4] += 3 * val;
        i += 4;
        break;

      case 233:
        buf[i] += val;
        buf[i+2] += val;
        buf[i+3] += 3 * val;
        i += 3;
        break;

      case 234:
        buf[i+1] += val;
        buf[i+2] += val;
        buf[i+3] += 3 * val;
        i += 3;
        break;

      case 235:
        buf[i] += 2 * val;
        buf[i+1] += val;
        buf[i+2] += 3 * val;
        i += 2;
        break;

      case 236:
        buf[i+2] += 2 * val;
        buf[i+3] += 3 * val;
        i += 3;
        break;

      case 237:
        buf[i] += val;
        buf[i+1] += 2 * val;
        buf[i+2] += 3 * val;
        i += 2;
        break;

      case 238:
        buf[i+1] += 3 * val;
        buf[i+2] += 3 * val;
        i += 2;
        break;

      case 239:
        buf[i] += 4 * val;
        buf[i+1] += 3 * val;
        i += 1;
        break;

      case 240:
        buf[i+4] += 4 * val;
        i += 4;
        break;

      case 241:
        buf[i] += val;
        buf[i+3] += 4 * val;
        i += 3;
        break;

      case 242:
        buf[i+1] += val;
        buf[i+3] += 4 * val;
        i += 3;
        break;

      case 243:
        buf[i] += 2 * val;
        buf[i+2] += 4 * val;
        i += 2;
        break;

      case 244:
        buf[i+2] += val;
        buf[i+3] += 4 * val;
        i += 3;
        break;

      case 245:
        buf[i] += val;
        buf[i+1] += val;
        buf[i+2] += 4 * val;
        i += 2;
        break;

      case 246:
        buf[i+1] += 2 * val;
        buf[i+2] += 4 * val;
        i += 2;
        break;

      case 247:
        buf[i] += 3 * val;
        buf[i+1] += 4 * val;
        i += 1;
        break;

      case 248:
        buf[i+3] += 5 * val;
        i += 3;
        break;

      case 249:
        buf[i] += val;
        buf[i+2] += 5 * val;
        i += 2;
        break;

      case 250:
        buf[i+1] += val;
        buf[i+2] += 5 * val;
        i += 2;
        break;

      case 251:
        buf[i] += 2 * val;
        buf[i+1] += 5 * val;
        i += 1;
        break;

      case 252:
        buf[i+2] += 6 * val;
        i += 2;
        break;

      case 253:
        buf[i] += val;
        buf[i+1] += 6 * val;
        i += 1;
        break;

      case 254:
        buf[i+1] += 7 * val;
        i += 1;
        break;

      case 255:
        buf[i] += 8 * val;
        i += 0;
    }
  }
  i = ww - (unsigned char *)(w);
  w += (i>>2);
  if (i & 3)  w += 1;
}
  return w;
}

unsigned char *rcdec32(unsigned* input, int size, unsigned* output)
{
	unsigned* tmp = input;
	int flag = (*tmp)&0x3f;
	int b = ((*tmp)>>6);
	tmp++;
	return rc_turbo_rice_decode(tmp, output, b, flag);
	//return tmp; // - input;

}
