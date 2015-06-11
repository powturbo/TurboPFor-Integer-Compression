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
// bitpack include 
#define BITBLK32_1(ip, i, op, parm) { ; register uint32_t w;;\
  IPPB(ip, i*32+ 0, parm); w  = (uint32_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); w |= (uint32_t)SRC(ip, i*32+ 1) <<  1;\
  IPPB(ip, i*32+ 2, parm); w |= (uint32_t)SRC(ip, i*32+ 2) <<  2;\
  IPPB(ip, i*32+ 3, parm); w |= (uint32_t)SRC(ip, i*32+ 3) <<  3;\
  IPPB(ip, i*32+ 4, parm); w |= (uint32_t)SRC(ip, i*32+ 4) <<  4;\
  IPPB(ip, i*32+ 5, parm); w |= (uint32_t)SRC(ip, i*32+ 5) <<  5;\
  IPPB(ip, i*32+ 6, parm); w |= (uint32_t)SRC(ip, i*32+ 6) <<  6;\
  IPPB(ip, i*32+ 7, parm); w |= (uint32_t)SRC(ip, i*32+ 7) <<  7;\
  IPPB(ip, i*32+ 8, parm); w |= (uint32_t)SRC(ip, i*32+ 8) <<  8;\
  IPPB(ip, i*32+ 9, parm); w |= (uint32_t)SRC(ip, i*32+ 9) <<  9;\
  IPPB(ip, i*32+10, parm); w |= (uint32_t)SRC(ip, i*32+10) << 10;\
  IPPB(ip, i*32+11, parm); w |= (uint32_t)SRC(ip, i*32+11) << 11;\
  IPPB(ip, i*32+12, parm); w |= (uint32_t)SRC(ip, i*32+12) << 12;\
  IPPB(ip, i*32+13, parm); w |= (uint32_t)SRC(ip, i*32+13) << 13;\
  IPPB(ip, i*32+14, parm); w |= (uint32_t)SRC(ip, i*32+14) << 14;\
  IPPB(ip, i*32+15, parm); w |= (uint32_t)SRC(ip, i*32+15) << 15;\
  IPPB(ip, i*32+16, parm); w |= (uint32_t)SRC(ip, i*32+16) << 16;\
  IPPB(ip, i*32+17, parm); w |= (uint32_t)SRC(ip, i*32+17) << 17;\
  IPPB(ip, i*32+18, parm); w |= (uint32_t)SRC(ip, i*32+18) << 18;\
  IPPB(ip, i*32+19, parm); w |= (uint32_t)SRC(ip, i*32+19) << 19;\
  IPPB(ip, i*32+20, parm); w |= (uint32_t)SRC(ip, i*32+20) << 20;\
  IPPB(ip, i*32+21, parm); w |= (uint32_t)SRC(ip, i*32+21) << 21;\
  IPPB(ip, i*32+22, parm); w |= (uint32_t)SRC(ip, i*32+22) << 22;\
  IPPB(ip, i*32+23, parm); w |= (uint32_t)SRC(ip, i*32+23) << 23;\
  IPPB(ip, i*32+24, parm); w |= (uint32_t)SRC(ip, i*32+24) << 24;\
  IPPB(ip, i*32+25, parm); w |= (uint32_t)SRC(ip, i*32+25) << 25;\
  IPPB(ip, i*32+26, parm); w |= (uint32_t)SRC(ip, i*32+26) << 26;\
  IPPB(ip, i*32+27, parm); w |= (uint32_t)SRC(ip, i*32+27) << 27;\
  IPPB(ip, i*32+28, parm); w |= (uint32_t)SRC(ip, i*32+28) << 28;\
  IPPB(ip, i*32+29, parm); w |= (uint32_t)SRC(ip, i*32+29) << 29;\
  IPPB(ip, i*32+30, parm); w |= (uint32_t)SRC(ip, i*32+30) << 30;\
  IPPB(ip, i*32+31, parm); w |= (uint32_t)SRC(ip, i*32+31) << 31;*((uint32_t *)op+i*1+ 0) = w;;\
}

#define BITPACK64_1(ip,  op, parm) { \
  BITBLK32_1(ip, 0, op, parm);  SRCI(ip); op += 1*4/sizeof(op[0]);\
}

#define BITBLK64_2(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*32+ 0, parm); w  = (uint64_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); w |= (uint64_t)SRC(ip, i*32+ 1) <<  2;\
  IPPB(ip, i*32+ 2, parm); w |= (uint64_t)SRC(ip, i*32+ 2) <<  4;\
  IPPB(ip, i*32+ 3, parm); w |= (uint64_t)SRC(ip, i*32+ 3) <<  6;\
  IPPB(ip, i*32+ 4, parm); w |= (uint64_t)SRC(ip, i*32+ 4) <<  8;\
  IPPB(ip, i*32+ 5, parm); w |= (uint64_t)SRC(ip, i*32+ 5) << 10;\
  IPPB(ip, i*32+ 6, parm); w |= (uint64_t)SRC(ip, i*32+ 6) << 12;\
  IPPB(ip, i*32+ 7, parm); w |= (uint64_t)SRC(ip, i*32+ 7) << 14;\
  IPPB(ip, i*32+ 8, parm); w |= (uint64_t)SRC(ip, i*32+ 8) << 16;\
  IPPB(ip, i*32+ 9, parm); w |= (uint64_t)SRC(ip, i*32+ 9) << 18;\
  IPPB(ip, i*32+10, parm); w |= (uint64_t)SRC(ip, i*32+10) << 20;\
  IPPB(ip, i*32+11, parm); w |= (uint64_t)SRC(ip, i*32+11) << 22;\
  IPPB(ip, i*32+12, parm); w |= (uint64_t)SRC(ip, i*32+12) << 24;\
  IPPB(ip, i*32+13, parm); w |= (uint64_t)SRC(ip, i*32+13) << 26;\
  IPPB(ip, i*32+14, parm); w |= (uint64_t)SRC(ip, i*32+14) << 28;\
  IPPB(ip, i*32+15, parm); w |= (uint64_t)SRC(ip, i*32+15) << 30;\
  IPPB(ip, i*32+16, parm); w |= (uint64_t)SRC(ip, i*32+16) << 32;\
  IPPB(ip, i*32+17, parm); w |= (uint64_t)SRC(ip, i*32+17) << 34;\
  IPPB(ip, i*32+18, parm); w |= (uint64_t)SRC(ip, i*32+18) << 36;\
  IPPB(ip, i*32+19, parm); w |= (uint64_t)SRC(ip, i*32+19) << 38;\
  IPPB(ip, i*32+20, parm); w |= (uint64_t)SRC(ip, i*32+20) << 40;\
  IPPB(ip, i*32+21, parm); w |= (uint64_t)SRC(ip, i*32+21) << 42;\
  IPPB(ip, i*32+22, parm); w |= (uint64_t)SRC(ip, i*32+22) << 44;\
  IPPB(ip, i*32+23, parm); w |= (uint64_t)SRC(ip, i*32+23) << 46;\
  IPPB(ip, i*32+24, parm); w |= (uint64_t)SRC(ip, i*32+24) << 48;\
  IPPB(ip, i*32+25, parm); w |= (uint64_t)SRC(ip, i*32+25) << 50;\
  IPPB(ip, i*32+26, parm); w |= (uint64_t)SRC(ip, i*32+26) << 52;\
  IPPB(ip, i*32+27, parm); w |= (uint64_t)SRC(ip, i*32+27) << 54;\
  IPPB(ip, i*32+28, parm); w |= (uint64_t)SRC(ip, i*32+28) << 56;\
  IPPB(ip, i*32+29, parm); w |= (uint64_t)SRC(ip, i*32+29) << 58;\
  IPPB(ip, i*32+30, parm); w |= (uint64_t)SRC(ip, i*32+30) << 60;\
  IPPB(ip, i*32+31, parm); w |= (uint64_t)SRC(ip, i*32+31) << 62;*((uint64_t *)op+i*1+ 0) = w;;\
}

#define BITPACK64_2(ip,  op, parm) { \
  BITBLK64_2(ip, 0, op, parm);  SRCI(ip); op += 2*4/sizeof(op[0]);\
}

#define BITBLK64_3(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*64+ 0, parm); w  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); w |= (uint64_t)SRC(ip, i*64+ 1) <<  3;\
  IPPB(ip, i*64+ 2, parm); w |= (uint64_t)SRC(ip, i*64+ 2) <<  6;\
  IPPB(ip, i*64+ 3, parm); w |= (uint64_t)SRC(ip, i*64+ 3) <<  9;\
  IPPB(ip, i*64+ 4, parm); w |= (uint64_t)SRC(ip, i*64+ 4) << 12;\
  IPPB(ip, i*64+ 5, parm); w |= (uint64_t)SRC(ip, i*64+ 5) << 15;\
  IPPB(ip, i*64+ 6, parm); w |= (uint64_t)SRC(ip, i*64+ 6) << 18;\
  IPPB(ip, i*64+ 7, parm); w |= (uint64_t)SRC(ip, i*64+ 7) << 21;\
  IPPB(ip, i*64+ 8, parm); w |= (uint64_t)SRC(ip, i*64+ 8) << 24;\
  IPPB(ip, i*64+ 9, parm); w |= (uint64_t)SRC(ip, i*64+ 9) << 27;\
  IPPB(ip, i*64+10, parm); w |= (uint64_t)SRC(ip, i*64+10) << 30;\
  IPPB(ip, i*64+11, parm); w |= (uint64_t)SRC(ip, i*64+11) << 33;\
  IPPB(ip, i*64+12, parm); w |= (uint64_t)SRC(ip, i*64+12) << 36;\
  IPPB(ip, i*64+13, parm); w |= (uint64_t)SRC(ip, i*64+13) << 39;\
  IPPB(ip, i*64+14, parm); w |= (uint64_t)SRC(ip, i*64+14) << 42;\
  IPPB(ip, i*64+15, parm); w |= (uint64_t)SRC(ip, i*64+15) << 45;\
  IPPB(ip, i*64+16, parm); w |= (uint64_t)SRC(ip, i*64+16) << 48;\
  IPPB(ip, i*64+17, parm); w |= (uint64_t)SRC(ip, i*64+17) << 51;\
  IPPB(ip, i*64+18, parm); w |= (uint64_t)SRC(ip, i*64+18) << 54;\
  IPPB(ip, i*64+19, parm); w |= (uint64_t)SRC(ip, i*64+19) << 57;\
  IPPB(ip, i*64+20, parm); w |= (uint64_t)SRC(ip, i*64+20) << 60 | (uint64_t)SRC1(ip, i*64+21) << 63;*((uint64_t *)op+i*3+ 0) = w;\
  IPPB(ip, i*64+21, parm); w  = (uint64_t)SRC(ip, i*64+21) >>  1;\
  IPPB(ip, i*64+22, parm); w |= (uint64_t)SRC(ip, i*64+22) <<  2;\
  IPPB(ip, i*64+23, parm); w |= (uint64_t)SRC(ip, i*64+23) <<  5;\
  IPPB(ip, i*64+24, parm); w |= (uint64_t)SRC(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+25, parm); w |= (uint64_t)SRC(ip, i*64+25) << 11;\
  IPPB(ip, i*64+26, parm); w |= (uint64_t)SRC(ip, i*64+26) << 14;\
  IPPB(ip, i*64+27, parm); w |= (uint64_t)SRC(ip, i*64+27) << 17;\
  IPPB(ip, i*64+28, parm); w |= (uint64_t)SRC(ip, i*64+28) << 20;\
  IPPB(ip, i*64+29, parm); w |= (uint64_t)SRC(ip, i*64+29) << 23;\
  IPPB(ip, i*64+30, parm); w |= (uint64_t)SRC(ip, i*64+30) << 26;\
  IPPB(ip, i*64+31, parm); w |= (uint64_t)SRC(ip, i*64+31) << 29;*((uint64_t *)op+i*3+ 1) = w;;\
}

#define BITPACK64_3(ip,  op, parm) { \
  BITBLK64_3(ip, 0, op, parm);  SRCI(ip); op += 3*4/sizeof(op[0]);\
}

#define BITBLK64_4(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*16+ 0, parm); w  = (uint64_t)SRC(ip, i*16+ 0)      ;\
  IPPB(ip, i*16+ 1, parm); w |= (uint64_t)SRC(ip, i*16+ 1) <<  4;\
  IPPB(ip, i*16+ 2, parm); w |= (uint64_t)SRC(ip, i*16+ 2) <<  8;\
  IPPB(ip, i*16+ 3, parm); w |= (uint64_t)SRC(ip, i*16+ 3) << 12;\
  IPPB(ip, i*16+ 4, parm); w |= (uint64_t)SRC(ip, i*16+ 4) << 16;\
  IPPB(ip, i*16+ 5, parm); w |= (uint64_t)SRC(ip, i*16+ 5) << 20;\
  IPPB(ip, i*16+ 6, parm); w |= (uint64_t)SRC(ip, i*16+ 6) << 24;\
  IPPB(ip, i*16+ 7, parm); w |= (uint64_t)SRC(ip, i*16+ 7) << 28;\
  IPPB(ip, i*16+ 8, parm); w |= (uint64_t)SRC(ip, i*16+ 8) << 32;\
  IPPB(ip, i*16+ 9, parm); w |= (uint64_t)SRC(ip, i*16+ 9) << 36;\
  IPPB(ip, i*16+10, parm); w |= (uint64_t)SRC(ip, i*16+10) << 40;\
  IPPB(ip, i*16+11, parm); w |= (uint64_t)SRC(ip, i*16+11) << 44;\
  IPPB(ip, i*16+12, parm); w |= (uint64_t)SRC(ip, i*16+12) << 48;\
  IPPB(ip, i*16+13, parm); w |= (uint64_t)SRC(ip, i*16+13) << 52;\
  IPPB(ip, i*16+14, parm); w |= (uint64_t)SRC(ip, i*16+14) << 56;\
  IPPB(ip, i*16+15, parm); w |= (uint64_t)SRC(ip, i*16+15) << 60;*((uint64_t *)op+i*1+ 0) = w;;\
}

#define BITPACK64_4(ip,  op, parm) { \
  BITBLK64_4(ip, 0, op, parm);\
  BITBLK64_4(ip, 1, op, parm);  SRCI(ip); op += 4*4/sizeof(op[0]);\
}

#define BITBLK64_5(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*64+ 0, parm); w  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); w |= (uint64_t)SRC(ip, i*64+ 1) <<  5;\
  IPPB(ip, i*64+ 2, parm); w |= (uint64_t)SRC(ip, i*64+ 2) << 10;\
  IPPB(ip, i*64+ 3, parm); w |= (uint64_t)SRC(ip, i*64+ 3) << 15;\
  IPPB(ip, i*64+ 4, parm); w |= (uint64_t)SRC(ip, i*64+ 4) << 20;\
  IPPB(ip, i*64+ 5, parm); w |= (uint64_t)SRC(ip, i*64+ 5) << 25;\
  IPPB(ip, i*64+ 6, parm); w |= (uint64_t)SRC(ip, i*64+ 6) << 30;\
  IPPB(ip, i*64+ 7, parm); w |= (uint64_t)SRC(ip, i*64+ 7) << 35;\
  IPPB(ip, i*64+ 8, parm); w |= (uint64_t)SRC(ip, i*64+ 8) << 40;\
  IPPB(ip, i*64+ 9, parm); w |= (uint64_t)SRC(ip, i*64+ 9) << 45;\
  IPPB(ip, i*64+10, parm); w |= (uint64_t)SRC(ip, i*64+10) << 50;\
  IPPB(ip, i*64+11, parm); w |= (uint64_t)SRC(ip, i*64+11) << 55 | (uint64_t)SRC1(ip, i*64+12) << 60;*((uint64_t *)op+i*5+ 0) = w;\
  IPPB(ip, i*64+12, parm); w  = (uint64_t)SRC(ip, i*64+12) >>  4;\
  IPPB(ip, i*64+13, parm); w |= (uint64_t)SRC(ip, i*64+13) <<  1;\
  IPPB(ip, i*64+14, parm); w |= (uint64_t)SRC(ip, i*64+14) <<  6;\
  IPPB(ip, i*64+15, parm); w |= (uint64_t)SRC(ip, i*64+15) << 11;\
  IPPB(ip, i*64+16, parm); w |= (uint64_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); w |= (uint64_t)SRC(ip, i*64+17) << 21;\
  IPPB(ip, i*64+18, parm); w |= (uint64_t)SRC(ip, i*64+18) << 26;\
  IPPB(ip, i*64+19, parm); w |= (uint64_t)SRC(ip, i*64+19) << 31;\
  IPPB(ip, i*64+20, parm); w |= (uint64_t)SRC(ip, i*64+20) << 36;\
  IPPB(ip, i*64+21, parm); w |= (uint64_t)SRC(ip, i*64+21) << 41;\
  IPPB(ip, i*64+22, parm); w |= (uint64_t)SRC(ip, i*64+22) << 46;\
  IPPB(ip, i*64+23, parm); w |= (uint64_t)SRC(ip, i*64+23) << 51;\
  IPPB(ip, i*64+24, parm); w |= (uint64_t)SRC(ip, i*64+24) << 56 | (uint64_t)SRC1(ip, i*64+25) << 61;*((uint64_t *)op+i*5+ 1) = w;\
  IPPB(ip, i*64+25, parm); w  = (uint64_t)SRC(ip, i*64+25) >>  3;\
  IPPB(ip, i*64+26, parm); w |= (uint64_t)SRC(ip, i*64+26) <<  2;\
  IPPB(ip, i*64+27, parm); w |= (uint64_t)SRC(ip, i*64+27) <<  7;\
  IPPB(ip, i*64+28, parm); w |= (uint64_t)SRC(ip, i*64+28) << 12;\
  IPPB(ip, i*64+29, parm); w |= (uint64_t)SRC(ip, i*64+29) << 17;\
  IPPB(ip, i*64+30, parm); w |= (uint64_t)SRC(ip, i*64+30) << 22;\
  IPPB(ip, i*64+31, parm); w |= (uint64_t)SRC(ip, i*64+31) << 27;*((uint64_t *)op+i*5+ 2) = w;;\
}

#define BITPACK64_5(ip,  op, parm) { \
  BITBLK64_5(ip, 0, op, parm);  SRCI(ip); op += 5*4/sizeof(op[0]);\
}

#define BITBLK64_6(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*32+ 0, parm); w  = (uint64_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); w |= (uint64_t)SRC(ip, i*32+ 1) <<  6;\
  IPPB(ip, i*32+ 2, parm); w |= (uint64_t)SRC(ip, i*32+ 2) << 12;\
  IPPB(ip, i*32+ 3, parm); w |= (uint64_t)SRC(ip, i*32+ 3) << 18;\
  IPPB(ip, i*32+ 4, parm); w |= (uint64_t)SRC(ip, i*32+ 4) << 24;\
  IPPB(ip, i*32+ 5, parm); w |= (uint64_t)SRC(ip, i*32+ 5) << 30;\
  IPPB(ip, i*32+ 6, parm); w |= (uint64_t)SRC(ip, i*32+ 6) << 36;\
  IPPB(ip, i*32+ 7, parm); w |= (uint64_t)SRC(ip, i*32+ 7) << 42;\
  IPPB(ip, i*32+ 8, parm); w |= (uint64_t)SRC(ip, i*32+ 8) << 48;\
  IPPB(ip, i*32+ 9, parm); w |= (uint64_t)SRC(ip, i*32+ 9) << 54 | (uint64_t)SRC1(ip, i*32+10) << 60;*((uint64_t *)op+i*3+ 0) = w;\
  IPPB(ip, i*32+10, parm); w  = (uint64_t)SRC(ip, i*32+10) >>  4;\
  IPPB(ip, i*32+11, parm); w |= (uint64_t)SRC(ip, i*32+11) <<  2;\
  IPPB(ip, i*32+12, parm); w |= (uint64_t)SRC(ip, i*32+12) <<  8;\
  IPPB(ip, i*32+13, parm); w |= (uint64_t)SRC(ip, i*32+13) << 14;\
  IPPB(ip, i*32+14, parm); w |= (uint64_t)SRC(ip, i*32+14) << 20;\
  IPPB(ip, i*32+15, parm); w |= (uint64_t)SRC(ip, i*32+15) << 26;\
  IPPB(ip, i*32+16, parm); w |= (uint64_t)SRC(ip, i*32+16) << 32;\
  IPPB(ip, i*32+17, parm); w |= (uint64_t)SRC(ip, i*32+17) << 38;\
  IPPB(ip, i*32+18, parm); w |= (uint64_t)SRC(ip, i*32+18) << 44;\
  IPPB(ip, i*32+19, parm); w |= (uint64_t)SRC(ip, i*32+19) << 50;\
  IPPB(ip, i*32+20, parm); w |= (uint64_t)SRC(ip, i*32+20) << 56 | (uint64_t)SRC1(ip, i*32+21) << 62;*((uint64_t *)op+i*3+ 1) = w;\
  IPPB(ip, i*32+21, parm); w  = (uint64_t)SRC(ip, i*32+21) >>  2;\
  IPPB(ip, i*32+22, parm); w |= (uint64_t)SRC(ip, i*32+22) <<  4;\
  IPPB(ip, i*32+23, parm); w |= (uint64_t)SRC(ip, i*32+23) << 10;\
  IPPB(ip, i*32+24, parm); w |= (uint64_t)SRC(ip, i*32+24) << 16;\
  IPPB(ip, i*32+25, parm); w |= (uint64_t)SRC(ip, i*32+25) << 22;\
  IPPB(ip, i*32+26, parm); w |= (uint64_t)SRC(ip, i*32+26) << 28;\
  IPPB(ip, i*32+27, parm); w |= (uint64_t)SRC(ip, i*32+27) << 34;\
  IPPB(ip, i*32+28, parm); w |= (uint64_t)SRC(ip, i*32+28) << 40;\
  IPPB(ip, i*32+29, parm); w |= (uint64_t)SRC(ip, i*32+29) << 46;\
  IPPB(ip, i*32+30, parm); w |= (uint64_t)SRC(ip, i*32+30) << 52;\
  IPPB(ip, i*32+31, parm); w |= (uint64_t)SRC(ip, i*32+31) << 58;*((uint64_t *)op+i*3+ 2) = w;;\
}

#define BITPACK64_6(ip,  op, parm) { \
  BITBLK64_6(ip, 0, op, parm);  SRCI(ip); op += 6*4/sizeof(op[0]);\
}

#define BITBLK64_7(ip, i, op, parm) { ; register uint64_t w;;\
  IPPB(ip, i*64+ 0, parm); w  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); w |= (uint64_t)SRC(ip, i*64+ 1) <<  7;\
  IPPB(ip, i*64+ 2, parm); w |= (uint64_t)SRC(ip, i*64+ 2) << 14;\
  IPPB(ip, i*64+ 3, parm); w |= (uint64_t)SRC(ip, i*64+ 3) << 21;\
  IPPB(ip, i*64+ 4, parm); w |= (uint64_t)SRC(ip, i*64+ 4) << 28;\
  IPPB(ip, i*64+ 5, parm); w |= (uint64_t)SRC(ip, i*64+ 5) << 35;\
  IPPB(ip, i*64+ 6, parm); w |= (uint64_t)SRC(ip, i*64+ 6) << 42;\
  IPPB(ip, i*64+ 7, parm); w |= (uint64_t)SRC(ip, i*64+ 7) << 49;\
  IPPB(ip, i*64+ 8, parm); w |= (uint64_t)SRC(ip, i*64+ 8) << 56 | (uint64_t)SRC1(ip, i*64+9) << 63;*((uint64_t *)op+i*7+ 0) = w;\
  IPPB(ip, i*64+ 9, parm); w  = (uint64_t)SRC(ip, i*64+ 9) >>  1;\
  IPPB(ip, i*64+10, parm); w |= (uint64_t)SRC(ip, i*64+10) <<  6;\
  IPPB(ip, i*64+11, parm); w |= (uint64_t)SRC(ip, i*64+11) << 13;\
  IPPB(ip, i*64+12, parm); w |= (uint64_t)SRC(ip, i*64+12) << 20;\
  IPPB(ip, i*64+13, parm); w |= (uint64_t)SRC(ip, i*64+13) << 27;\
  IPPB(ip, i*64+14, parm); w |= (uint64_t)SRC(ip, i*64+14) << 34;\
  IPPB(ip, i*64+15, parm); w |= (uint64_t)SRC(ip, i*64+15) << 41;\
  IPPB(ip, i*64+16, parm); w |= (uint64_t)SRC(ip, i*64+16) << 48;\
  IPPB(ip, i*64+17, parm); w |= (uint64_t)SRC(ip, i*64+17) << 55 | (uint64_t)SRC1(ip, i*64+18) << 62;*((uint64_t *)op+i*7+ 1) = w;\
  IPPB(ip, i*64+18, parm); w  = (uint64_t)SRC(ip, i*64+18) >>  2;\
  IPPB(ip, i*64+19, parm); w |= (uint64_t)SRC(ip, i*64+19) <<  5;\
  IPPB(ip, i*64+20, parm); w |= (uint64_t)SRC(ip, i*64+20) << 12;\
  IPPB(ip, i*64+21, parm); w |= (uint64_t)SRC(ip, i*64+21) << 19;\
  IPPB(ip, i*64+22, parm); w |= (uint64_t)SRC(ip, i*64+22) << 26;\
  IPPB(ip, i*64+23, parm); w |= (uint64_t)SRC(ip, i*64+23) << 33;\
  IPPB(ip, i*64+24, parm); w |= (uint64_t)SRC(ip, i*64+24) << 40;\
  IPPB(ip, i*64+25, parm); w |= (uint64_t)SRC(ip, i*64+25) << 47;\
  IPPB(ip, i*64+26, parm); w |= (uint64_t)SRC(ip, i*64+26) << 54 | (uint64_t)SRC1(ip, i*64+27) << 61;*((uint64_t *)op+i*7+ 2) = w;\
  IPPB(ip, i*64+27, parm); w  = (uint64_t)SRC(ip, i*64+27) >>  3;\
  IPPB(ip, i*64+28, parm); w |= (uint64_t)SRC(ip, i*64+28) <<  4;\
  IPPB(ip, i*64+29, parm); w |= (uint64_t)SRC(ip, i*64+29) << 11;\
  IPPB(ip, i*64+30, parm); w |= (uint64_t)SRC(ip, i*64+30) << 18;\
  IPPB(ip, i*64+31, parm); w |= (uint64_t)SRC(ip, i*64+31) << 25;*((uint64_t *)op+i*7+ 3) = w;;\
}

#define BITPACK64_7(ip,  op, parm) { \
  BITBLK64_7(ip, 0, op, parm);  SRCI(ip); op += 7*4/sizeof(op[0]);\
}

#define BITBLK64_8(ip, i, op, parm) { ;\
  IPPB(ip, i*8+ 0, parm); *((uint64_t *)op+i*1+ 0)  = (uint64_t)SRC(ip, i*8+ 0)      ;\
  IPPB(ip, i*8+ 1, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 1) <<  8;\
  IPPB(ip, i*8+ 2, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 2) << 16;\
  IPPB(ip, i*8+ 3, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 3) << 24;\
  IPPB(ip, i*8+ 4, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 4) << 32;\
  IPPB(ip, i*8+ 5, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 5) << 40;\
  IPPB(ip, i*8+ 6, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 6) << 48;\
  IPPB(ip, i*8+ 7, parm); *((uint64_t *)op+i*1+ 0) |= (uint64_t)SRC(ip, i*8+ 7) << 56;\
}

#define BITPACK64_8(ip,  op, parm) { \
  BITBLK64_8(ip, 0, op, parm);\
  BITBLK64_8(ip, 1, op, parm);\
  BITBLK64_8(ip, 2, op, parm);\
  BITBLK64_8(ip, 3, op, parm);  SRCI(ip); op += 8*4/sizeof(op[0]);\
}

#define BITBLK64_9(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*9+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 1) <<  9;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 18;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 3) << 27;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 4) << 36;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 5) << 45;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*64+ 6) << 54 | (uint64_t)SRC1(ip, i*64+7) << 63;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*9+ 1)  = (uint64_t)SRC(ip, i*64+ 7) >>  1;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+ 8) <<  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+ 9) << 17;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+10) << 26;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+11) << 35;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+12) << 44;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*64+13) << 53 | (uint64_t)SRC1(ip, i*64+14) << 62;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*9+ 2)  = (uint64_t)SRC(ip, i*64+14) >>  2;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+15) <<  7;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+17) << 25;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+18) << 34;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+19) << 43;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*64+20) << 52 | (uint64_t)SRC1(ip, i*64+21) << 61;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*9+ 3)  = (uint64_t)SRC(ip, i*64+21) >>  3;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+22) <<  6;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+23) << 15;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+24) << 24;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+25) << 33;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+26) << 42;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*64+27) << 51 | (uint64_t)SRC1(ip, i*64+28) << 60;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*9+ 4)  = (uint64_t)SRC(ip, i*64+28) >>  4;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*9+ 4) |= (uint64_t)SRC(ip, i*64+29) <<  5;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*9+ 4) |= (uint64_t)SRC(ip, i*64+30) << 14;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*9+ 4) |= (uint64_t)SRC(ip, i*64+31) << 23;\
}

#define BITPACK64_9(ip,  op, parm) { \
  BITBLK64_9(ip, 0, op, parm);  SRCI(ip); op += 9*4/sizeof(op[0]);\
}

#define BITBLK64_10(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*5+ 0)  = (uint64_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 10;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*32+ 2) << 20;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*32+ 3) << 30;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*32+ 4) << 40;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*32+ 5) << 50 | (uint64_t)SRC1(ip, i*32+6) << 60;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*5+ 1)  = (uint64_t)SRC(ip, i*32+ 6) >>  4;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*32+ 7) <<  6;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*32+ 8) << 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*32+ 9) << 26;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*32+10) << 36;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*32+11) << 46 | (uint64_t)SRC1(ip, i*32+12) << 56;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*5+ 2)  = (uint64_t)SRC(ip, i*32+12) >>  8;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+13) <<  2;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+14) << 12;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+15) << 22;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+16) << 32;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+17) << 42;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*32+18) << 52 | (uint64_t)SRC1(ip, i*32+19) << 62;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*5+ 3)  = (uint64_t)SRC(ip, i*32+19) >>  2;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*32+20) <<  8;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*32+21) << 18;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*32+22) << 28;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*32+23) << 38;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*32+24) << 48 | (uint64_t)SRC1(ip, i*32+25) << 58;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*5+ 4)  = (uint64_t)SRC(ip, i*32+25) >>  6;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+26) <<  4;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+27) << 14;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+28) << 24;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+29) << 34;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+30) << 44;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*32+31) << 54;\
}

#define BITPACK64_10(ip,  op, parm) { \
  BITBLK64_10(ip, 0, op, parm);  SRCI(ip); op += 10*4/sizeof(op[0]);\
}

#define BITBLK64_11(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*11+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 11;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 22;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*64+ 3) << 33;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*64+ 4) << 44 | (uint64_t)SRC1(ip, i*64+5) << 55;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*11+ 1)  = (uint64_t)SRC(ip, i*64+ 5) >>  9;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*64+ 6) <<  2;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*64+ 7) << 13;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*64+ 8) << 24;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*64+ 9) << 35;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*64+10) << 46 | (uint64_t)SRC1(ip, i*64+11) << 57;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*11+ 2)  = (uint64_t)SRC(ip, i*64+11) >>  7;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*64+12) <<  4;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*64+13) << 15;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*64+14) << 26;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*64+15) << 37;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*64+16) << 48 | (uint64_t)SRC1(ip, i*64+17) << 59;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*11+ 3)  = (uint64_t)SRC(ip, i*64+17) >>  5;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*64+18) <<  6;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*64+19) << 17;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*64+20) << 28;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*64+21) << 39;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*64+22) << 50 | (uint64_t)SRC1(ip, i*64+23) << 61;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*11+ 4)  = (uint64_t)SRC(ip, i*64+23) >>  3;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*64+25) << 19;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*64+26) << 30;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*64+27) << 41;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*64+28) << 52 | (uint64_t)SRC1(ip, i*64+29) << 63;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*11+ 5)  = (uint64_t)SRC(ip, i*64+29) >>  1;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*11+ 5) |= (uint64_t)SRC(ip, i*64+30) << 10;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*11+ 5) |= (uint64_t)SRC(ip, i*64+31) << 21;\
}

#define BITPACK64_11(ip,  op, parm) { \
  BITBLK64_11(ip, 0, op, parm);  SRCI(ip); op += 11*4/sizeof(op[0]);\
}

#define BITBLK64_12(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*3+ 0)  = (uint64_t)SRC(ip, i*16+ 0)      ;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*16+ 1) << 12;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*16+ 2) << 24;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*16+ 3) << 36;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*16+ 4) << 48 | (uint64_t)SRC1(ip, i*16+5) << 60;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*3+ 1)  = (uint64_t)SRC(ip, i*16+ 5) >>  4;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*16+ 6) <<  8;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*16+ 7) << 20;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*16+ 8) << 32;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*16+ 9) << 44 | (uint64_t)SRC1(ip, i*16+10) << 56;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*3+ 2)  = (uint64_t)SRC(ip, i*16+10) >>  8;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*16+11) <<  4;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*16+12) << 16;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*16+13) << 28;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*16+14) << 40;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*16+15) << 52;\
}

#define BITPACK64_12(ip,  op, parm) { \
  BITBLK64_12(ip, 0, op, parm);\
  BITBLK64_12(ip, 1, op, parm);  SRCI(ip); op += 12*4/sizeof(op[0]);\
}

#define BITBLK64_13(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*13+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*13+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 13;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*13+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 26;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*13+ 0) |= (uint64_t)SRC(ip, i*64+ 3) << 39 | (uint64_t)SRC1(ip, i*64+4) << 52;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*13+ 1)  = (uint64_t)SRC(ip, i*64+ 4) >> 12;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*13+ 1) |= (uint64_t)SRC(ip, i*64+ 5) <<  1;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*13+ 1) |= (uint64_t)SRC(ip, i*64+ 6) << 14;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*13+ 1) |= (uint64_t)SRC(ip, i*64+ 7) << 27;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*13+ 1) |= (uint64_t)SRC(ip, i*64+ 8) << 40 | (uint64_t)SRC1(ip, i*64+9) << 53;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*13+ 2)  = (uint64_t)SRC(ip, i*64+ 9) >> 11;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*64+10) <<  2;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*64+11) << 15;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*64+12) << 28;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*64+13) << 41 | (uint64_t)SRC1(ip, i*64+14) << 54;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*13+ 3)  = (uint64_t)SRC(ip, i*64+14) >> 10;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*13+ 3) |= (uint64_t)SRC(ip, i*64+15) <<  3;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*13+ 3) |= (uint64_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*13+ 3) |= (uint64_t)SRC(ip, i*64+17) << 29;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*13+ 3) |= (uint64_t)SRC(ip, i*64+18) << 42 | (uint64_t)SRC1(ip, i*64+19) << 55;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*13+ 4)  = (uint64_t)SRC(ip, i*64+19) >>  9;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*64+20) <<  4;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*64+21) << 17;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*64+22) << 30;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*64+23) << 43 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*13+ 5)  = (uint64_t)SRC(ip, i*64+24) >>  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*13+ 5) |= (uint64_t)SRC(ip, i*64+25) <<  5;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*13+ 5) |= (uint64_t)SRC(ip, i*64+26) << 18;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*13+ 5) |= (uint64_t)SRC(ip, i*64+27) << 31;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*13+ 5) |= (uint64_t)SRC(ip, i*64+28) << 44 | (uint64_t)SRC1(ip, i*64+29) << 57;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*13+ 6)  = (uint64_t)SRC(ip, i*64+29) >>  7;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*13+ 6) |= (uint64_t)SRC(ip, i*64+30) <<  6;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*13+ 6) |= (uint64_t)SRC(ip, i*64+31) << 19;\
}

#define BITPACK64_13(ip,  op, parm) { \
  BITBLK64_13(ip, 0, op, parm);  SRCI(ip); op += 13*4/sizeof(op[0]);\
}

#define BITBLK64_14(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*7+ 0)  = (uint64_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*7+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 14;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*7+ 0) |= (uint64_t)SRC(ip, i*32+ 2) << 28;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*7+ 0) |= (uint64_t)SRC(ip, i*32+ 3) << 42 | (uint64_t)SRC1(ip, i*32+4) << 56;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*7+ 1)  = (uint64_t)SRC(ip, i*32+ 4) >>  8;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*32+ 5) <<  6;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*32+ 6) << 20;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*32+ 7) << 34;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*32+ 8) << 48 | (uint64_t)SRC1(ip, i*32+9) << 62;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*7+ 2)  = (uint64_t)SRC(ip, i*32+ 9) >>  2;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*7+ 2) |= (uint64_t)SRC(ip, i*32+10) << 12;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*7+ 2) |= (uint64_t)SRC(ip, i*32+11) << 26;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*7+ 2) |= (uint64_t)SRC(ip, i*32+12) << 40 | (uint64_t)SRC1(ip, i*32+13) << 54;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*7+ 3)  = (uint64_t)SRC(ip, i*32+13) >> 10;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*32+14) <<  4;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*32+15) << 18;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*32+16) << 32;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*32+17) << 46 | (uint64_t)SRC1(ip, i*32+18) << 60;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*7+ 4)  = (uint64_t)SRC(ip, i*32+18) >>  4;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*7+ 4) |= (uint64_t)SRC(ip, i*32+19) << 10;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*7+ 4) |= (uint64_t)SRC(ip, i*32+20) << 24;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*7+ 4) |= (uint64_t)SRC(ip, i*32+21) << 38 | (uint64_t)SRC1(ip, i*32+22) << 52;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*7+ 5)  = (uint64_t)SRC(ip, i*32+22) >> 12;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*7+ 5) |= (uint64_t)SRC(ip, i*32+23) <<  2;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*7+ 5) |= (uint64_t)SRC(ip, i*32+24) << 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*7+ 5) |= (uint64_t)SRC(ip, i*32+25) << 30;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*7+ 5) |= (uint64_t)SRC(ip, i*32+26) << 44 | (uint64_t)SRC1(ip, i*32+27) << 58;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*7+ 6)  = (uint64_t)SRC(ip, i*32+27) >>  6;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*32+28) <<  8;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*32+29) << 22;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*32+30) << 36;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*32+31) << 50;\
}

#define BITPACK64_14(ip,  op, parm) { \
  BITBLK64_14(ip, 0, op, parm);  SRCI(ip); op += 14*4/sizeof(op[0]);\
}

#define BITBLK64_15(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*15+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*15+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 15;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*15+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 30;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*15+ 0) |= (uint64_t)SRC(ip, i*64+ 3) << 45 | (uint64_t)SRC1(ip, i*64+4) << 60;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*15+ 1)  = (uint64_t)SRC(ip, i*64+ 4) >>  4;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*15+ 1) |= (uint64_t)SRC(ip, i*64+ 5) << 11;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*15+ 1) |= (uint64_t)SRC(ip, i*64+ 6) << 26;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*15+ 1) |= (uint64_t)SRC(ip, i*64+ 7) << 41 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*15+ 2)  = (uint64_t)SRC(ip, i*64+ 8) >>  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*15+ 2) |= (uint64_t)SRC(ip, i*64+ 9) <<  7;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*15+ 2) |= (uint64_t)SRC(ip, i*64+10) << 22;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*15+ 2) |= (uint64_t)SRC(ip, i*64+11) << 37 | (uint64_t)SRC1(ip, i*64+12) << 52;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*15+ 3)  = (uint64_t)SRC(ip, i*64+12) >> 12;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*64+13) <<  3;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*64+14) << 18;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*64+15) << 33;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*64+16) << 48 | (uint64_t)SRC1(ip, i*64+17) << 63;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*15+ 4)  = (uint64_t)SRC(ip, i*64+17) >>  1;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*15+ 4) |= (uint64_t)SRC(ip, i*64+18) << 14;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*15+ 4) |= (uint64_t)SRC(ip, i*64+19) << 29;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*15+ 4) |= (uint64_t)SRC(ip, i*64+20) << 44 | (uint64_t)SRC1(ip, i*64+21) << 59;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*15+ 5)  = (uint64_t)SRC(ip, i*64+21) >>  5;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*15+ 5) |= (uint64_t)SRC(ip, i*64+22) << 10;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*15+ 5) |= (uint64_t)SRC(ip, i*64+23) << 25;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*15+ 5) |= (uint64_t)SRC(ip, i*64+24) << 40 | (uint64_t)SRC1(ip, i*64+25) << 55;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*15+ 6)  = (uint64_t)SRC(ip, i*64+25) >>  9;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*15+ 6) |= (uint64_t)SRC(ip, i*64+26) <<  6;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*15+ 6) |= (uint64_t)SRC(ip, i*64+27) << 21;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*15+ 6) |= (uint64_t)SRC(ip, i*64+28) << 36 | (uint64_t)SRC1(ip, i*64+29) << 51;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*15+ 7)  = (uint64_t)SRC(ip, i*64+29) >> 13;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*15+ 7) |= (uint64_t)SRC(ip, i*64+30) <<  2;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*15+ 7) |= (uint64_t)SRC(ip, i*64+31) << 17;\
}

#define BITPACK64_15(ip,  op, parm) { \
  BITBLK64_15(ip, 0, op, parm);  SRCI(ip); op += 15*4/sizeof(op[0]);\
}

#define BITBLK64_16(ip, i, op, parm) { \
  IPPB(ip, i*4+ 0, parm); *(uint16_t *)(op+i*8+ 0) = SRC(ip, i*4+ 0);\
  IPPB(ip, i*4+ 1, parm); *(uint16_t *)(op+i*8+ 2) = SRC(ip, i*4+ 1);\
  IPPB(ip, i*4+ 2, parm); *(uint16_t *)(op+i*8+ 4) = SRC(ip, i*4+ 2);\
  IPPB(ip, i*4+ 3, parm); *(uint16_t *)(op+i*8+ 6) = SRC(ip, i*4+ 3);;\
}

#define BITPACK64_16(ip,  op, parm) { \
  BITBLK64_16(ip, 0, op, parm);\
  BITBLK64_16(ip, 1, op, parm);\
  BITBLK64_16(ip, 2, op, parm);\
  BITBLK64_16(ip, 3, op, parm);\
  BITBLK64_16(ip, 4, op, parm);\
  BITBLK64_16(ip, 5, op, parm);\
  BITBLK64_16(ip, 6, op, parm);\
  BITBLK64_16(ip, 7, op, parm);  SRCI(ip); op += 16*4/sizeof(op[0]);\
}

#define BITBLK64_17(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*17+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*17+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 17;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*17+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 34 | (uint64_t)SRC1(ip, i*64+3) << 51;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*17+ 1)  = (uint64_t)SRC(ip, i*64+ 3) >> 13;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*17+ 1) |= (uint64_t)SRC(ip, i*64+ 4) <<  4;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*17+ 1) |= (uint64_t)SRC(ip, i*64+ 5) << 21;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*17+ 1) |= (uint64_t)SRC(ip, i*64+ 6) << 38 | (uint64_t)SRC1(ip, i*64+7) << 55;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*17+ 2)  = (uint64_t)SRC(ip, i*64+ 7) >>  9;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*17+ 2) |= (uint64_t)SRC(ip, i*64+ 8) <<  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*17+ 2) |= (uint64_t)SRC(ip, i*64+ 9) << 25;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*17+ 2) |= (uint64_t)SRC(ip, i*64+10) << 42 | (uint64_t)SRC1(ip, i*64+11) << 59;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*17+ 3)  = (uint64_t)SRC(ip, i*64+11) >>  5;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*17+ 3) |= (uint64_t)SRC(ip, i*64+12) << 12;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*17+ 3) |= (uint64_t)SRC(ip, i*64+13) << 29;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*17+ 3) |= (uint64_t)SRC(ip, i*64+14) << 46 | (uint64_t)SRC1(ip, i*64+15) << 63;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*17+ 4)  = (uint64_t)SRC(ip, i*64+15) >>  1;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*17+ 4) |= (uint64_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*17+ 4) |= (uint64_t)SRC(ip, i*64+17) << 33 | (uint64_t)SRC1(ip, i*64+18) << 50;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*17+ 5)  = (uint64_t)SRC(ip, i*64+18) >> 14;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*17+ 5) |= (uint64_t)SRC(ip, i*64+19) <<  3;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*17+ 5) |= (uint64_t)SRC(ip, i*64+20) << 20;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*17+ 5) |= (uint64_t)SRC(ip, i*64+21) << 37 | (uint64_t)SRC1(ip, i*64+22) << 54;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*17+ 6)  = (uint64_t)SRC(ip, i*64+22) >> 10;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*17+ 6) |= (uint64_t)SRC(ip, i*64+23) <<  7;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*17+ 6) |= (uint64_t)SRC(ip, i*64+24) << 24;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*17+ 6) |= (uint64_t)SRC(ip, i*64+25) << 41 | (uint64_t)SRC1(ip, i*64+26) << 58;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*17+ 7)  = (uint64_t)SRC(ip, i*64+26) >>  6;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*17+ 7) |= (uint64_t)SRC(ip, i*64+27) << 11;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*17+ 7) |= (uint64_t)SRC(ip, i*64+28) << 28;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*17+ 7) |= (uint64_t)SRC(ip, i*64+29) << 45 | (uint64_t)SRC1(ip, i*64+30) << 62;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*17+ 8)  = (uint64_t)SRC(ip, i*64+30) >>  2;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*17+ 8) |= (uint64_t)SRC(ip, i*64+31) << 15;\
}

#define BITPACK64_17(ip,  op, parm) { \
  BITBLK64_17(ip, 0, op, parm);  SRCI(ip); op += 17*4/sizeof(op[0]);\
}

#define BITBLK64_18(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*9+ 0)  = (uint64_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 18;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*9+ 0) |= (uint64_t)SRC(ip, i*32+ 2) << 36 | (uint64_t)SRC1(ip, i*32+3) << 54;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*9+ 1)  = (uint64_t)SRC(ip, i*32+ 3) >> 10;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*32+ 4) <<  8;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*32+ 5) << 26;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*32+ 6) << 44 | (uint64_t)SRC1(ip, i*32+7) << 62;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*9+ 2)  = (uint64_t)SRC(ip, i*32+ 7) >>  2;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*32+ 8) << 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*32+ 9) << 34 | (uint64_t)SRC1(ip, i*32+10) << 52;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*9+ 3)  = (uint64_t)SRC(ip, i*32+10) >> 12;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*32+11) <<  6;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*32+12) << 24;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*32+13) << 42 | (uint64_t)SRC1(ip, i*32+14) << 60;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*9+ 4)  = (uint64_t)SRC(ip, i*32+14) >>  4;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*9+ 4) |= (uint64_t)SRC(ip, i*32+15) << 14;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*9+ 4) |= (uint64_t)SRC(ip, i*32+16) << 32 | (uint64_t)SRC1(ip, i*32+17) << 50;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*9+ 5)  = (uint64_t)SRC(ip, i*32+17) >> 14;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*9+ 5) |= (uint64_t)SRC(ip, i*32+18) <<  4;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*9+ 5) |= (uint64_t)SRC(ip, i*32+19) << 22;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*9+ 5) |= (uint64_t)SRC(ip, i*32+20) << 40 | (uint64_t)SRC1(ip, i*32+21) << 58;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*9+ 6)  = (uint64_t)SRC(ip, i*32+21) >>  6;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*9+ 6) |= (uint64_t)SRC(ip, i*32+22) << 12;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*9+ 6) |= (uint64_t)SRC(ip, i*32+23) << 30 | (uint64_t)SRC1(ip, i*32+24) << 48;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*9+ 7)  = (uint64_t)SRC(ip, i*32+24) >> 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*9+ 7) |= (uint64_t)SRC(ip, i*32+25) <<  2;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*9+ 7) |= (uint64_t)SRC(ip, i*32+26) << 20;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*9+ 7) |= (uint64_t)SRC(ip, i*32+27) << 38 | (uint64_t)SRC1(ip, i*32+28) << 56;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*9+ 8)  = (uint64_t)SRC(ip, i*32+28) >>  8;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*9+ 8) |= (uint64_t)SRC(ip, i*32+29) << 10;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*9+ 8) |= (uint64_t)SRC(ip, i*32+30) << 28;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*9+ 8) |= (uint64_t)SRC(ip, i*32+31) << 46;\
}

#define BITPACK64_18(ip,  op, parm) { \
  BITBLK64_18(ip, 0, op, parm);  SRCI(ip); op += 18*4/sizeof(op[0]);\
}

#define BITBLK64_19(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*19+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*19+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 19;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*19+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 38 | (uint64_t)SRC1(ip, i*64+3) << 57;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*19+ 1)  = (uint64_t)SRC(ip, i*64+ 3) >>  7;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*19+ 1) |= (uint64_t)SRC(ip, i*64+ 4) << 12;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*19+ 1) |= (uint64_t)SRC(ip, i*64+ 5) << 31 | (uint64_t)SRC1(ip, i*64+6) << 50;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*19+ 2)  = (uint64_t)SRC(ip, i*64+ 6) >> 14;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*19+ 2) |= (uint64_t)SRC(ip, i*64+ 7) <<  5;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*19+ 2) |= (uint64_t)SRC(ip, i*64+ 8) << 24;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*19+ 2) |= (uint64_t)SRC(ip, i*64+ 9) << 43 | (uint64_t)SRC1(ip, i*64+10) << 62;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*19+ 3)  = (uint64_t)SRC(ip, i*64+10) >>  2;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*19+ 3) |= (uint64_t)SRC(ip, i*64+11) << 17;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*19+ 3) |= (uint64_t)SRC(ip, i*64+12) << 36 | (uint64_t)SRC1(ip, i*64+13) << 55;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*19+ 4)  = (uint64_t)SRC(ip, i*64+13) >>  9;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*19+ 4) |= (uint64_t)SRC(ip, i*64+14) << 10;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*19+ 4) |= (uint64_t)SRC(ip, i*64+15) << 29 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*19+ 5)  = (uint64_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*19+ 5) |= (uint64_t)SRC(ip, i*64+17) <<  3;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*19+ 5) |= (uint64_t)SRC(ip, i*64+18) << 22;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*19+ 5) |= (uint64_t)SRC(ip, i*64+19) << 41 | (uint64_t)SRC1(ip, i*64+20) << 60;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*19+ 6)  = (uint64_t)SRC(ip, i*64+20) >>  4;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*19+ 6) |= (uint64_t)SRC(ip, i*64+21) << 15;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*19+ 6) |= (uint64_t)SRC(ip, i*64+22) << 34 | (uint64_t)SRC1(ip, i*64+23) << 53;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*19+ 7)  = (uint64_t)SRC(ip, i*64+23) >> 11;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*19+ 7) |= (uint64_t)SRC(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*19+ 7) |= (uint64_t)SRC(ip, i*64+25) << 27 | (uint64_t)SRC1(ip, i*64+26) << 46;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*19+ 8)  = (uint64_t)SRC(ip, i*64+26) >> 18;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*19+ 8) |= (uint64_t)SRC(ip, i*64+27) <<  1;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*19+ 8) |= (uint64_t)SRC(ip, i*64+28) << 20;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*19+ 8) |= (uint64_t)SRC(ip, i*64+29) << 39 | (uint64_t)SRC1(ip, i*64+30) << 58;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*19+ 9)  = (uint64_t)SRC(ip, i*64+30) >>  6;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*19+ 9) |= (uint64_t)SRC(ip, i*64+31) << 13;\
}

#define BITPACK64_19(ip,  op, parm) { \
  BITBLK64_19(ip, 0, op, parm);  SRCI(ip); op += 19*4/sizeof(op[0]);\
}

#define BITBLK64_20(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*5+ 0)  = (uint64_t)SRC(ip, i*16+ 0)      ;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*16+ 1) << 20;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*5+ 0) |= (uint64_t)SRC(ip, i*16+ 2) << 40 | (uint64_t)SRC1(ip, i*16+3) << 60;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*5+ 1)  = (uint64_t)SRC(ip, i*16+ 3) >>  4;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*16+ 4) << 16;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*16+ 5) << 36 | (uint64_t)SRC1(ip, i*16+6) << 56;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*5+ 2)  = (uint64_t)SRC(ip, i*16+ 6) >>  8;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*16+ 7) << 12;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*5+ 2) |= (uint64_t)SRC(ip, i*16+ 8) << 32 | (uint64_t)SRC1(ip, i*16+9) << 52;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*5+ 3)  = (uint64_t)SRC(ip, i*16+ 9) >> 12;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*16+10) <<  8;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*16+11) << 28 | (uint64_t)SRC1(ip, i*16+12) << 48;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*5+ 4)  = (uint64_t)SRC(ip, i*16+12) >> 16;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*16+13) <<  4;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*16+14) << 24;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*16+15) << 44;\
}

#define BITPACK64_20(ip,  op, parm) { \
  BITBLK64_20(ip, 0, op, parm);\
  BITBLK64_20(ip, 1, op, parm);  SRCI(ip); op += 20*4/sizeof(op[0]);\
}

#define BITBLK64_21(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*21+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*21+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 21;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*21+ 0) |= (uint64_t)SRC(ip, i*64+ 2) << 42 | (uint64_t)SRC1(ip, i*64+3) << 63;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*21+ 1)  = (uint64_t)SRC(ip, i*64+ 3) >>  1;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*21+ 1) |= (uint64_t)SRC(ip, i*64+ 4) << 20;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*21+ 1) |= (uint64_t)SRC(ip, i*64+ 5) << 41 | (uint64_t)SRC1(ip, i*64+6) << 62;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*21+ 2)  = (uint64_t)SRC(ip, i*64+ 6) >>  2;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*21+ 2) |= (uint64_t)SRC(ip, i*64+ 7) << 19;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*21+ 2) |= (uint64_t)SRC(ip, i*64+ 8) << 40 | (uint64_t)SRC1(ip, i*64+9) << 61;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*21+ 3)  = (uint64_t)SRC(ip, i*64+ 9) >>  3;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*21+ 3) |= (uint64_t)SRC(ip, i*64+10) << 18;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*21+ 3) |= (uint64_t)SRC(ip, i*64+11) << 39 | (uint64_t)SRC1(ip, i*64+12) << 60;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*21+ 4)  = (uint64_t)SRC(ip, i*64+12) >>  4;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*21+ 4) |= (uint64_t)SRC(ip, i*64+13) << 17;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*21+ 4) |= (uint64_t)SRC(ip, i*64+14) << 38 | (uint64_t)SRC1(ip, i*64+15) << 59;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*21+ 5)  = (uint64_t)SRC(ip, i*64+15) >>  5;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*21+ 5) |= (uint64_t)SRC(ip, i*64+16) << 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*21+ 5) |= (uint64_t)SRC(ip, i*64+17) << 37 | (uint64_t)SRC1(ip, i*64+18) << 58;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*21+ 6)  = (uint64_t)SRC(ip, i*64+18) >>  6;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*21+ 6) |= (uint64_t)SRC(ip, i*64+19) << 15;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*21+ 6) |= (uint64_t)SRC(ip, i*64+20) << 36 | (uint64_t)SRC1(ip, i*64+21) << 57;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*21+ 7)  = (uint64_t)SRC(ip, i*64+21) >>  7;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*21+ 7) |= (uint64_t)SRC(ip, i*64+22) << 14;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*21+ 7) |= (uint64_t)SRC(ip, i*64+23) << 35 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*21+ 8)  = (uint64_t)SRC(ip, i*64+24) >>  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*21+ 8) |= (uint64_t)SRC(ip, i*64+25) << 13;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*21+ 8) |= (uint64_t)SRC(ip, i*64+26) << 34 | (uint64_t)SRC1(ip, i*64+27) << 55;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*21+ 9)  = (uint64_t)SRC(ip, i*64+27) >>  9;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*21+ 9) |= (uint64_t)SRC(ip, i*64+28) << 12;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*21+ 9) |= (uint64_t)SRC(ip, i*64+29) << 33 | (uint64_t)SRC1(ip, i*64+30) << 54;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*21+10)  = (uint64_t)SRC(ip, i*64+30) >> 10;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*21+10) |= (uint64_t)SRC(ip, i*64+31) << 11;\
}

#define BITPACK64_21(ip,  op, parm) { \
  BITBLK64_21(ip, 0, op, parm);  SRCI(ip); op += 21*4/sizeof(op[0]);\
}

#define BITBLK64_22(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*11+ 0)  = (uint64_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*11+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 22 | (uint64_t)SRC1(ip, i*32+2) << 44;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*11+ 1)  = (uint64_t)SRC(ip, i*32+ 2) >> 20;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*32+ 3) <<  2;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*11+ 1) |= (uint64_t)SRC(ip, i*32+ 4) << 24 | (uint64_t)SRC1(ip, i*32+5) << 46;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*11+ 2)  = (uint64_t)SRC(ip, i*32+ 5) >> 18;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*32+ 6) <<  4;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*32+ 7) << 26 | (uint64_t)SRC1(ip, i*32+8) << 48;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*11+ 3)  = (uint64_t)SRC(ip, i*32+ 8) >> 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*32+ 9) <<  6;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*11+ 3) |= (uint64_t)SRC(ip, i*32+10) << 28 | (uint64_t)SRC1(ip, i*32+11) << 50;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*11+ 4)  = (uint64_t)SRC(ip, i*32+11) >> 14;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*32+12) <<  8;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*32+13) << 30 | (uint64_t)SRC1(ip, i*32+14) << 52;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*11+ 5)  = (uint64_t)SRC(ip, i*32+14) >> 12;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*11+ 5) |= (uint64_t)SRC(ip, i*32+15) << 10;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*11+ 5) |= (uint64_t)SRC(ip, i*32+16) << 32 | (uint64_t)SRC1(ip, i*32+17) << 54;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*11+ 6)  = (uint64_t)SRC(ip, i*32+17) >> 10;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*11+ 6) |= (uint64_t)SRC(ip, i*32+18) << 12;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*11+ 6) |= (uint64_t)SRC(ip, i*32+19) << 34 | (uint64_t)SRC1(ip, i*32+20) << 56;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*11+ 7)  = (uint64_t)SRC(ip, i*32+20) >>  8;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*11+ 7) |= (uint64_t)SRC(ip, i*32+21) << 14;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*11+ 7) |= (uint64_t)SRC(ip, i*32+22) << 36 | (uint64_t)SRC1(ip, i*32+23) << 58;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*11+ 8)  = (uint64_t)SRC(ip, i*32+23) >>  6;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*11+ 8) |= (uint64_t)SRC(ip, i*32+24) << 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*11+ 8) |= (uint64_t)SRC(ip, i*32+25) << 38 | (uint64_t)SRC1(ip, i*32+26) << 60;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*11+ 9)  = (uint64_t)SRC(ip, i*32+26) >>  4;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*11+ 9) |= (uint64_t)SRC(ip, i*32+27) << 18;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*11+ 9) |= (uint64_t)SRC(ip, i*32+28) << 40 | (uint64_t)SRC1(ip, i*32+29) << 62;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*11+10)  = (uint64_t)SRC(ip, i*32+29) >>  2;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*11+10) |= (uint64_t)SRC(ip, i*32+30) << 20;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*11+10) |= (uint64_t)SRC(ip, i*32+31) << 42;\
}

#define BITPACK64_22(ip,  op, parm) { \
  BITBLK64_22(ip, 0, op, parm);  SRCI(ip); op += 22*4/sizeof(op[0]);\
}

#define BITBLK64_23(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*23+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*23+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 23 | (uint64_t)SRC1(ip, i*64+2) << 46;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*23+ 1)  = (uint64_t)SRC(ip, i*64+ 2) >> 18;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*23+ 1) |= (uint64_t)SRC(ip, i*64+ 3) <<  5;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*23+ 1) |= (uint64_t)SRC(ip, i*64+ 4) << 28 | (uint64_t)SRC1(ip, i*64+5) << 51;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*23+ 2)  = (uint64_t)SRC(ip, i*64+ 5) >> 13;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*23+ 2) |= (uint64_t)SRC(ip, i*64+ 6) << 10;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*23+ 2) |= (uint64_t)SRC(ip, i*64+ 7) << 33 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*23+ 3)  = (uint64_t)SRC(ip, i*64+ 8) >>  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*23+ 3) |= (uint64_t)SRC(ip, i*64+ 9) << 15;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*23+ 3) |= (uint64_t)SRC(ip, i*64+10) << 38 | (uint64_t)SRC1(ip, i*64+11) << 61;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*23+ 4)  = (uint64_t)SRC(ip, i*64+11) >>  3;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*23+ 4) |= (uint64_t)SRC(ip, i*64+12) << 20 | (uint64_t)SRC1(ip, i*64+13) << 43;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*23+ 5)  = (uint64_t)SRC(ip, i*64+13) >> 21;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*23+ 5) |= (uint64_t)SRC(ip, i*64+14) <<  2;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*23+ 5) |= (uint64_t)SRC(ip, i*64+15) << 25 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*23+ 6)  = (uint64_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*23+ 6) |= (uint64_t)SRC(ip, i*64+17) <<  7;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*23+ 6) |= (uint64_t)SRC(ip, i*64+18) << 30 | (uint64_t)SRC1(ip, i*64+19) << 53;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*23+ 7)  = (uint64_t)SRC(ip, i*64+19) >> 11;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*23+ 7) |= (uint64_t)SRC(ip, i*64+20) << 12;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*23+ 7) |= (uint64_t)SRC(ip, i*64+21) << 35 | (uint64_t)SRC1(ip, i*64+22) << 58;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*23+ 8)  = (uint64_t)SRC(ip, i*64+22) >>  6;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*23+ 8) |= (uint64_t)SRC(ip, i*64+23) << 17;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*23+ 8) |= (uint64_t)SRC(ip, i*64+24) << 40 | (uint64_t)SRC1(ip, i*64+25) << 63;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*23+ 9)  = (uint64_t)SRC(ip, i*64+25) >>  1;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*23+ 9) |= (uint64_t)SRC(ip, i*64+26) << 22 | (uint64_t)SRC1(ip, i*64+27) << 45;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*23+10)  = (uint64_t)SRC(ip, i*64+27) >> 19;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*23+10) |= (uint64_t)SRC(ip, i*64+28) <<  4;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*23+10) |= (uint64_t)SRC(ip, i*64+29) << 27 | (uint64_t)SRC1(ip, i*64+30) << 50;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*23+11)  = (uint64_t)SRC(ip, i*64+30) >> 14;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*23+11) |= (uint64_t)SRC(ip, i*64+31) <<  9;\
}

#define BITPACK64_23(ip,  op, parm) { \
  BITBLK64_23(ip, 0, op, parm);  SRCI(ip); op += 23*4/sizeof(op[0]);\
}

#define BITBLK64_24(ip, i, op, parm) { ;\
  IPPB(ip, i*8+ 0, parm); *((uint64_t *)op+i*3+ 0)  = (uint64_t)SRC(ip, i*8+ 0)      ;\
  IPPB(ip, i*8+ 1, parm); *((uint64_t *)op+i*3+ 0) |= (uint64_t)SRC(ip, i*8+ 1) << 24 | (uint64_t)SRC1(ip, i*8+2) << 48;\
  IPPB(ip, i*8+ 2, parm); *((uint64_t *)op+i*3+ 1)  = (uint64_t)SRC(ip, i*8+ 2) >> 16;\
  IPPB(ip, i*8+ 3, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*8+ 3) <<  8;\
  IPPB(ip, i*8+ 4, parm); *((uint64_t *)op+i*3+ 1) |= (uint64_t)SRC(ip, i*8+ 4) << 32 | (uint64_t)SRC1(ip, i*8+5) << 56;\
  IPPB(ip, i*8+ 5, parm); *((uint64_t *)op+i*3+ 2)  = (uint64_t)SRC(ip, i*8+ 5) >>  8;\
  IPPB(ip, i*8+ 6, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*8+ 6) << 16;\
  IPPB(ip, i*8+ 7, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*8+ 7) << 40;\
}

#define BITPACK64_24(ip,  op, parm) { \
  BITBLK64_24(ip, 0, op, parm);\
  BITBLK64_24(ip, 1, op, parm);\
  BITBLK64_24(ip, 2, op, parm);\
  BITBLK64_24(ip, 3, op, parm);  SRCI(ip); op += 24*4/sizeof(op[0]);\
}

#define BITBLK64_25(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*25+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*25+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 25 | (uint64_t)SRC1(ip, i*64+2) << 50;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*25+ 1)  = (uint64_t)SRC(ip, i*64+ 2) >> 14;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*25+ 1) |= (uint64_t)SRC(ip, i*64+ 3) << 11;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*25+ 1) |= (uint64_t)SRC(ip, i*64+ 4) << 36 | (uint64_t)SRC1(ip, i*64+5) << 61;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*25+ 2)  = (uint64_t)SRC(ip, i*64+ 5) >>  3;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*25+ 2) |= (uint64_t)SRC(ip, i*64+ 6) << 22 | (uint64_t)SRC1(ip, i*64+7) << 47;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*25+ 3)  = (uint64_t)SRC(ip, i*64+ 7) >> 17;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*25+ 3) |= (uint64_t)SRC(ip, i*64+ 8) <<  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*25+ 3) |= (uint64_t)SRC(ip, i*64+ 9) << 33 | (uint64_t)SRC1(ip, i*64+10) << 58;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*25+ 4)  = (uint64_t)SRC(ip, i*64+10) >>  6;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*25+ 4) |= (uint64_t)SRC(ip, i*64+11) << 19 | (uint64_t)SRC1(ip, i*64+12) << 44;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*25+ 5)  = (uint64_t)SRC(ip, i*64+12) >> 20;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*25+ 5) |= (uint64_t)SRC(ip, i*64+13) <<  5;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*25+ 5) |= (uint64_t)SRC(ip, i*64+14) << 30 | (uint64_t)SRC1(ip, i*64+15) << 55;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*25+ 6)  = (uint64_t)SRC(ip, i*64+15) >>  9;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*25+ 6) |= (uint64_t)SRC(ip, i*64+16) << 16 | (uint64_t)SRC1(ip, i*64+17) << 41;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*25+ 7)  = (uint64_t)SRC(ip, i*64+17) >> 23;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*25+ 7) |= (uint64_t)SRC(ip, i*64+18) <<  2;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*25+ 7) |= (uint64_t)SRC(ip, i*64+19) << 27 | (uint64_t)SRC1(ip, i*64+20) << 52;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*25+ 8)  = (uint64_t)SRC(ip, i*64+20) >> 12;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*25+ 8) |= (uint64_t)SRC(ip, i*64+21) << 13;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*25+ 8) |= (uint64_t)SRC(ip, i*64+22) << 38 | (uint64_t)SRC1(ip, i*64+23) << 63;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*25+ 9)  = (uint64_t)SRC(ip, i*64+23) >>  1;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*25+ 9) |= (uint64_t)SRC(ip, i*64+24) << 24 | (uint64_t)SRC1(ip, i*64+25) << 49;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*25+10)  = (uint64_t)SRC(ip, i*64+25) >> 15;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*25+10) |= (uint64_t)SRC(ip, i*64+26) << 10;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*25+10) |= (uint64_t)SRC(ip, i*64+27) << 35 | (uint64_t)SRC1(ip, i*64+28) << 60;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*25+11)  = (uint64_t)SRC(ip, i*64+28) >>  4;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*25+11) |= (uint64_t)SRC(ip, i*64+29) << 21 | (uint64_t)SRC1(ip, i*64+30) << 46;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*25+12)  = (uint64_t)SRC(ip, i*64+30) >> 18;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*25+12) |= (uint64_t)SRC(ip, i*64+31) <<  7;\
}

#define BITPACK64_25(ip,  op, parm) { \
  BITBLK64_25(ip, 0, op, parm);  SRCI(ip); op += 25*4/sizeof(op[0]);\
}

#define BITBLK64_26(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*13+ 0)  = (uint64_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*13+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 26 | (uint64_t)SRC1(ip, i*32+2) << 52;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*13+ 1)  = (uint64_t)SRC(ip, i*32+ 2) >> 12;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*13+ 1) |= (uint64_t)SRC(ip, i*32+ 3) << 14 | (uint64_t)SRC1(ip, i*32+4) << 40;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*13+ 2)  = (uint64_t)SRC(ip, i*32+ 4) >> 24;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*32+ 5) <<  2;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*13+ 2) |= (uint64_t)SRC(ip, i*32+ 6) << 28 | (uint64_t)SRC1(ip, i*32+7) << 54;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*13+ 3)  = (uint64_t)SRC(ip, i*32+ 7) >> 10;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*13+ 3) |= (uint64_t)SRC(ip, i*32+ 8) << 16 | (uint64_t)SRC1(ip, i*32+9) << 42;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*13+ 4)  = (uint64_t)SRC(ip, i*32+ 9) >> 22;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*32+10) <<  4;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*32+11) << 30 | (uint64_t)SRC1(ip, i*32+12) << 56;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*13+ 5)  = (uint64_t)SRC(ip, i*32+12) >>  8;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*13+ 5) |= (uint64_t)SRC(ip, i*32+13) << 18 | (uint64_t)SRC1(ip, i*32+14) << 44;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*13+ 6)  = (uint64_t)SRC(ip, i*32+14) >> 20;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*13+ 6) |= (uint64_t)SRC(ip, i*32+15) <<  6;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*13+ 6) |= (uint64_t)SRC(ip, i*32+16) << 32 | (uint64_t)SRC1(ip, i*32+17) << 58;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*13+ 7)  = (uint64_t)SRC(ip, i*32+17) >>  6;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*13+ 7) |= (uint64_t)SRC(ip, i*32+18) << 20 | (uint64_t)SRC1(ip, i*32+19) << 46;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*13+ 8)  = (uint64_t)SRC(ip, i*32+19) >> 18;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*13+ 8) |= (uint64_t)SRC(ip, i*32+20) <<  8;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*13+ 8) |= (uint64_t)SRC(ip, i*32+21) << 34 | (uint64_t)SRC1(ip, i*32+22) << 60;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*13+ 9)  = (uint64_t)SRC(ip, i*32+22) >>  4;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*13+ 9) |= (uint64_t)SRC(ip, i*32+23) << 22 | (uint64_t)SRC1(ip, i*32+24) << 48;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*13+10)  = (uint64_t)SRC(ip, i*32+24) >> 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*13+10) |= (uint64_t)SRC(ip, i*32+25) << 10;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*13+10) |= (uint64_t)SRC(ip, i*32+26) << 36 | (uint64_t)SRC1(ip, i*32+27) << 62;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*13+11)  = (uint64_t)SRC(ip, i*32+27) >>  2;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*13+11) |= (uint64_t)SRC(ip, i*32+28) << 24 | (uint64_t)SRC1(ip, i*32+29) << 50;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*13+12)  = (uint64_t)SRC(ip, i*32+29) >> 14;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*13+12) |= (uint64_t)SRC(ip, i*32+30) << 12;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*13+12) |= (uint64_t)SRC(ip, i*32+31) << 38;\
}

#define BITPACK64_26(ip,  op, parm) { \
  BITBLK64_26(ip, 0, op, parm);  SRCI(ip); op += 26*4/sizeof(op[0]);\
}

#define BITBLK64_27(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*27+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*27+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 27 | (uint64_t)SRC1(ip, i*64+2) << 54;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*27+ 1)  = (uint64_t)SRC(ip, i*64+ 2) >> 10;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*27+ 1) |= (uint64_t)SRC(ip, i*64+ 3) << 17 | (uint64_t)SRC1(ip, i*64+4) << 44;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*27+ 2)  = (uint64_t)SRC(ip, i*64+ 4) >> 20;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*27+ 2) |= (uint64_t)SRC(ip, i*64+ 5) <<  7;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*27+ 2) |= (uint64_t)SRC(ip, i*64+ 6) << 34 | (uint64_t)SRC1(ip, i*64+7) << 61;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*27+ 3)  = (uint64_t)SRC(ip, i*64+ 7) >>  3;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*27+ 3) |= (uint64_t)SRC(ip, i*64+ 8) << 24 | (uint64_t)SRC1(ip, i*64+9) << 51;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*27+ 4)  = (uint64_t)SRC(ip, i*64+ 9) >> 13;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*27+ 4) |= (uint64_t)SRC(ip, i*64+10) << 14 | (uint64_t)SRC1(ip, i*64+11) << 41;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*27+ 5)  = (uint64_t)SRC(ip, i*64+11) >> 23;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*27+ 5) |= (uint64_t)SRC(ip, i*64+12) <<  4;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*27+ 5) |= (uint64_t)SRC(ip, i*64+13) << 31 | (uint64_t)SRC1(ip, i*64+14) << 58;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*27+ 6)  = (uint64_t)SRC(ip, i*64+14) >>  6;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*27+ 6) |= (uint64_t)SRC(ip, i*64+15) << 21 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*27+ 7)  = (uint64_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*27+ 7) |= (uint64_t)SRC(ip, i*64+17) << 11 | (uint64_t)SRC1(ip, i*64+18) << 38;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*27+ 8)  = (uint64_t)SRC(ip, i*64+18) >> 26;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*27+ 8) |= (uint64_t)SRC(ip, i*64+19) <<  1;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*27+ 8) |= (uint64_t)SRC(ip, i*64+20) << 28 | (uint64_t)SRC1(ip, i*64+21) << 55;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*27+ 9)  = (uint64_t)SRC(ip, i*64+21) >>  9;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*27+ 9) |= (uint64_t)SRC(ip, i*64+22) << 18 | (uint64_t)SRC1(ip, i*64+23) << 45;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*27+10)  = (uint64_t)SRC(ip, i*64+23) >> 19;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*27+10) |= (uint64_t)SRC(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*27+10) |= (uint64_t)SRC(ip, i*64+25) << 35 | (uint64_t)SRC1(ip, i*64+26) << 62;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*27+11)  = (uint64_t)SRC(ip, i*64+26) >>  2;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*27+11) |= (uint64_t)SRC(ip, i*64+27) << 25 | (uint64_t)SRC1(ip, i*64+28) << 52;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*27+12)  = (uint64_t)SRC(ip, i*64+28) >> 12;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*27+12) |= (uint64_t)SRC(ip, i*64+29) << 15 | (uint64_t)SRC1(ip, i*64+30) << 42;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*27+13)  = (uint64_t)SRC(ip, i*64+30) >> 22;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*27+13) |= (uint64_t)SRC(ip, i*64+31) <<  5;\
}

#define BITPACK64_27(ip,  op, parm) { \
  BITBLK64_27(ip, 0, op, parm);  SRCI(ip); op += 27*4/sizeof(op[0]);\
}

#define BITBLK64_28(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*7+ 0)  = (uint64_t)SRC(ip, i*16+ 0)      ;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*7+ 0) |= (uint64_t)SRC(ip, i*16+ 1) << 28 | (uint64_t)SRC1(ip, i*16+2) << 56;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*7+ 1)  = (uint64_t)SRC(ip, i*16+ 2) >>  8;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*7+ 1) |= (uint64_t)SRC(ip, i*16+ 3) << 20 | (uint64_t)SRC1(ip, i*16+4) << 48;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*7+ 2)  = (uint64_t)SRC(ip, i*16+ 4) >> 16;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*7+ 2) |= (uint64_t)SRC(ip, i*16+ 5) << 12 | (uint64_t)SRC1(ip, i*16+6) << 40;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*7+ 3)  = (uint64_t)SRC(ip, i*16+ 6) >> 24;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*16+ 7) <<  4;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*7+ 3) |= (uint64_t)SRC(ip, i*16+ 8) << 32 | (uint64_t)SRC1(ip, i*16+9) << 60;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*7+ 4)  = (uint64_t)SRC(ip, i*16+ 9) >>  4;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*7+ 4) |= (uint64_t)SRC(ip, i*16+10) << 24 | (uint64_t)SRC1(ip, i*16+11) << 52;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*7+ 5)  = (uint64_t)SRC(ip, i*16+11) >> 12;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*7+ 5) |= (uint64_t)SRC(ip, i*16+12) << 16 | (uint64_t)SRC1(ip, i*16+13) << 44;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*7+ 6)  = (uint64_t)SRC(ip, i*16+13) >> 20;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*16+14) <<  8;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*16+15) << 36;\
}

#define BITPACK64_28(ip,  op, parm) { \
  BITBLK64_28(ip, 0, op, parm);\
  BITBLK64_28(ip, 1, op, parm);  SRCI(ip); op += 28*4/sizeof(op[0]);\
}

#define BITBLK64_29(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*29+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*29+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 29 | (uint64_t)SRC1(ip, i*64+2) << 58;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*29+ 1)  = (uint64_t)SRC(ip, i*64+ 2) >>  6;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*29+ 1) |= (uint64_t)SRC(ip, i*64+ 3) << 23 | (uint64_t)SRC1(ip, i*64+4) << 52;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*29+ 2)  = (uint64_t)SRC(ip, i*64+ 4) >> 12;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*29+ 2) |= (uint64_t)SRC(ip, i*64+ 5) << 17 | (uint64_t)SRC1(ip, i*64+6) << 46;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*29+ 3)  = (uint64_t)SRC(ip, i*64+ 6) >> 18;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*29+ 3) |= (uint64_t)SRC(ip, i*64+ 7) << 11 | (uint64_t)SRC1(ip, i*64+8) << 40;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*29+ 4)  = (uint64_t)SRC(ip, i*64+ 8) >> 24;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*29+ 4) |= (uint64_t)SRC(ip, i*64+ 9) <<  5;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*29+ 4) |= (uint64_t)SRC(ip, i*64+10) << 34 | (uint64_t)SRC1(ip, i*64+11) << 63;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*29+ 5)  = (uint64_t)SRC(ip, i*64+11) >>  1;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*29+ 5) |= (uint64_t)SRC(ip, i*64+12) << 28 | (uint64_t)SRC1(ip, i*64+13) << 57;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*29+ 6)  = (uint64_t)SRC(ip, i*64+13) >>  7;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*29+ 6) |= (uint64_t)SRC(ip, i*64+14) << 22 | (uint64_t)SRC1(ip, i*64+15) << 51;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*29+ 7)  = (uint64_t)SRC(ip, i*64+15) >> 13;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*29+ 7) |= (uint64_t)SRC(ip, i*64+16) << 16 | (uint64_t)SRC1(ip, i*64+17) << 45;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*29+ 8)  = (uint64_t)SRC(ip, i*64+17) >> 19;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*29+ 8) |= (uint64_t)SRC(ip, i*64+18) << 10 | (uint64_t)SRC1(ip, i*64+19) << 39;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*29+ 9)  = (uint64_t)SRC(ip, i*64+19) >> 25;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*29+ 9) |= (uint64_t)SRC(ip, i*64+20) <<  4;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*29+ 9) |= (uint64_t)SRC(ip, i*64+21) << 33 | (uint64_t)SRC1(ip, i*64+22) << 62;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*29+10)  = (uint64_t)SRC(ip, i*64+22) >>  2;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*29+10) |= (uint64_t)SRC(ip, i*64+23) << 27 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*29+11)  = (uint64_t)SRC(ip, i*64+24) >>  8;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*29+11) |= (uint64_t)SRC(ip, i*64+25) << 21 | (uint64_t)SRC1(ip, i*64+26) << 50;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*29+12)  = (uint64_t)SRC(ip, i*64+26) >> 14;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*29+12) |= (uint64_t)SRC(ip, i*64+27) << 15 | (uint64_t)SRC1(ip, i*64+28) << 44;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*29+13)  = (uint64_t)SRC(ip, i*64+28) >> 20;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*29+13) |= (uint64_t)SRC(ip, i*64+29) <<  9 | (uint64_t)SRC1(ip, i*64+30) << 38;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*29+14)  = (uint64_t)SRC(ip, i*64+30) >> 26;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*29+14) |= (uint64_t)SRC(ip, i*64+31) <<  3;\
}

#define BITPACK64_29(ip,  op, parm) { \
  BITBLK64_29(ip, 0, op, parm);  SRCI(ip); op += 29*4/sizeof(op[0]);\
}

#define BITBLK64_30(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*15+ 0)  = (uint64_t)SRC(ip, i*32+ 0)      ;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*15+ 0) |= (uint64_t)SRC(ip, i*32+ 1) << 30 | (uint64_t)SRC1(ip, i*32+2) << 60;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*15+ 1)  = (uint64_t)SRC(ip, i*32+ 2) >>  4;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*15+ 1) |= (uint64_t)SRC(ip, i*32+ 3) << 26 | (uint64_t)SRC1(ip, i*32+4) << 56;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*15+ 2)  = (uint64_t)SRC(ip, i*32+ 4) >>  8;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*15+ 2) |= (uint64_t)SRC(ip, i*32+ 5) << 22 | (uint64_t)SRC1(ip, i*32+6) << 52;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*15+ 3)  = (uint64_t)SRC(ip, i*32+ 6) >> 12;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*15+ 3) |= (uint64_t)SRC(ip, i*32+ 7) << 18 | (uint64_t)SRC1(ip, i*32+8) << 48;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*15+ 4)  = (uint64_t)SRC(ip, i*32+ 8) >> 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*15+ 4) |= (uint64_t)SRC(ip, i*32+ 9) << 14 | (uint64_t)SRC1(ip, i*32+10) << 44;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*15+ 5)  = (uint64_t)SRC(ip, i*32+10) >> 20;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*15+ 5) |= (uint64_t)SRC(ip, i*32+11) << 10 | (uint64_t)SRC1(ip, i*32+12) << 40;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*15+ 6)  = (uint64_t)SRC(ip, i*32+12) >> 24;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*15+ 6) |= (uint64_t)SRC(ip, i*32+13) <<  6 | (uint64_t)SRC1(ip, i*32+14) << 36;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*15+ 7)  = (uint64_t)SRC(ip, i*32+14) >> 28;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*15+ 7) |= (uint64_t)SRC(ip, i*32+15) <<  2;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*15+ 7) |= (uint64_t)SRC(ip, i*32+16) << 32 | (uint64_t)SRC1(ip, i*32+17) << 62;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*15+ 8)  = (uint64_t)SRC(ip, i*32+17) >>  2;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*15+ 8) |= (uint64_t)SRC(ip, i*32+18) << 28 | (uint64_t)SRC1(ip, i*32+19) << 58;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*15+ 9)  = (uint64_t)SRC(ip, i*32+19) >>  6;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*15+ 9) |= (uint64_t)SRC(ip, i*32+20) << 24 | (uint64_t)SRC1(ip, i*32+21) << 54;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*15+10)  = (uint64_t)SRC(ip, i*32+21) >> 10;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*15+10) |= (uint64_t)SRC(ip, i*32+22) << 20 | (uint64_t)SRC1(ip, i*32+23) << 50;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*15+11)  = (uint64_t)SRC(ip, i*32+23) >> 14;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*15+11) |= (uint64_t)SRC(ip, i*32+24) << 16 | (uint64_t)SRC1(ip, i*32+25) << 46;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*15+12)  = (uint64_t)SRC(ip, i*32+25) >> 18;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*15+12) |= (uint64_t)SRC(ip, i*32+26) << 12 | (uint64_t)SRC1(ip, i*32+27) << 42;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*15+13)  = (uint64_t)SRC(ip, i*32+27) >> 22;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*15+13) |= (uint64_t)SRC(ip, i*32+28) <<  8 | (uint64_t)SRC1(ip, i*32+29) << 38;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*15+14)  = (uint64_t)SRC(ip, i*32+29) >> 26;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*15+14) |= (uint64_t)SRC(ip, i*32+30) <<  4;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*15+14) |= (uint64_t)SRC(ip, i*32+31) << 34;\
}

#define BITPACK64_30(ip,  op, parm) { \
  BITBLK64_30(ip, 0, op, parm);  SRCI(ip); op += 30*4/sizeof(op[0]);\
}

#define BITBLK64_31(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*31+ 0)  = (uint64_t)SRC(ip, i*64+ 0)      ;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*31+ 0) |= (uint64_t)SRC(ip, i*64+ 1) << 31 | (uint64_t)SRC1(ip, i*64+2) << 62;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*31+ 1)  = (uint64_t)SRC(ip, i*64+ 2) >>  2;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*31+ 1) |= (uint64_t)SRC(ip, i*64+ 3) << 29 | (uint64_t)SRC1(ip, i*64+4) << 60;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*31+ 2)  = (uint64_t)SRC(ip, i*64+ 4) >>  4;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*31+ 2) |= (uint64_t)SRC(ip, i*64+ 5) << 27 | (uint64_t)SRC1(ip, i*64+6) << 58;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*31+ 3)  = (uint64_t)SRC(ip, i*64+ 6) >>  6;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*31+ 3) |= (uint64_t)SRC(ip, i*64+ 7) << 25 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*31+ 4)  = (uint64_t)SRC(ip, i*64+ 8) >>  8;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*31+ 4) |= (uint64_t)SRC(ip, i*64+ 9) << 23 | (uint64_t)SRC1(ip, i*64+10) << 54;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*31+ 5)  = (uint64_t)SRC(ip, i*64+10) >> 10;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*31+ 5) |= (uint64_t)SRC(ip, i*64+11) << 21 | (uint64_t)SRC1(ip, i*64+12) << 52;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*31+ 6)  = (uint64_t)SRC(ip, i*64+12) >> 12;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*31+ 6) |= (uint64_t)SRC(ip, i*64+13) << 19 | (uint64_t)SRC1(ip, i*64+14) << 50;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*31+ 7)  = (uint64_t)SRC(ip, i*64+14) >> 14;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*31+ 7) |= (uint64_t)SRC(ip, i*64+15) << 17 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*31+ 8)  = (uint64_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*31+ 8) |= (uint64_t)SRC(ip, i*64+17) << 15 | (uint64_t)SRC1(ip, i*64+18) << 46;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*31+ 9)  = (uint64_t)SRC(ip, i*64+18) >> 18;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*31+ 9) |= (uint64_t)SRC(ip, i*64+19) << 13 | (uint64_t)SRC1(ip, i*64+20) << 44;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*31+10)  = (uint64_t)SRC(ip, i*64+20) >> 20;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*31+10) |= (uint64_t)SRC(ip, i*64+21) << 11 | (uint64_t)SRC1(ip, i*64+22) << 42;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*31+11)  = (uint64_t)SRC(ip, i*64+22) >> 22;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*31+11) |= (uint64_t)SRC(ip, i*64+23) <<  9 | (uint64_t)SRC1(ip, i*64+24) << 40;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*31+12)  = (uint64_t)SRC(ip, i*64+24) >> 24;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*31+12) |= (uint64_t)SRC(ip, i*64+25) <<  7 | (uint64_t)SRC1(ip, i*64+26) << 38;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*31+13)  = (uint64_t)SRC(ip, i*64+26) >> 26;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*31+13) |= (uint64_t)SRC(ip, i*64+27) <<  5 | (uint64_t)SRC1(ip, i*64+28) << 36;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*31+14)  = (uint64_t)SRC(ip, i*64+28) >> 28;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*31+14) |= (uint64_t)SRC(ip, i*64+29) <<  3 | (uint64_t)SRC1(ip, i*64+30) << 34;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*31+15)  = (uint64_t)SRC(ip, i*64+30) >> 30;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*31+15) |= (uint64_t)SRC(ip, i*64+31) <<  1;\
}

#define BITPACK64_31(ip,  op, parm) { \
  BITBLK64_31(ip, 0, op, parm);  SRCI(ip); op += 31*4/sizeof(op[0]);\
}

#define BITBLK64_32(ip, i, op, parm) { \
  IPPB(ip, i*2+ 0, parm); *(uint32_t *)(op+i*8+ 0) = SRC(ip, i*2+ 0);\
  IPPB(ip, i*2+ 1, parm); *(uint32_t *)(op+i*8+ 4) = SRC(ip, i*2+ 1);;\
}

#define BITPACK64_32(ip,  op, parm) { \
  BITBLK64_32(ip, 0, op, parm);\
  BITBLK64_32(ip, 1, op, parm);\
  BITBLK64_32(ip, 2, op, parm);\
  BITBLK64_32(ip, 3, op, parm);\
  BITBLK64_32(ip, 4, op, parm);\
  BITBLK64_32(ip, 5, op, parm);\
  BITBLK64_32(ip, 6, op, parm);\
  BITBLK64_32(ip, 7, op, parm);\
  BITBLK64_32(ip, 8, op, parm);\
  BITBLK64_32(ip, 9, op, parm);\
  BITBLK64_32(ip, 10, op, parm);\
  BITBLK64_32(ip, 11, op, parm);\
  BITBLK64_32(ip, 12, op, parm);\
  BITBLK64_32(ip, 13, op, parm);\
  BITBLK64_32(ip, 14, op, parm);\
  BITBLK64_32(ip, 15, op, parm);  SRCI(ip); op += 32*4/sizeof(op[0]);\
}

#define BITBLK64_33(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*33+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 33;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*33+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 31;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*33+ 1) |= (uint64_t)SRC(ip, i*64+ 2) <<  2 | (uint64_t)SRC1(ip, i*64+3) << 35;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*33+ 2)  = (uint64_t)SRC(ip, i*64+ 3) >> 29;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*33+ 2) |= (uint64_t)SRC(ip, i*64+ 4) <<  4 | (uint64_t)SRC1(ip, i*64+5) << 37;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*33+ 3)  = (uint64_t)SRC(ip, i*64+ 5) >> 27;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*33+ 3) |= (uint64_t)SRC(ip, i*64+ 6) <<  6 | (uint64_t)SRC1(ip, i*64+7) << 39;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*33+ 4)  = (uint64_t)SRC(ip, i*64+ 7) >> 25;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*33+ 4) |= (uint64_t)SRC(ip, i*64+ 8) <<  8 | (uint64_t)SRC1(ip, i*64+9) << 41;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*33+ 5)  = (uint64_t)SRC(ip, i*64+ 9) >> 23;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*33+ 5) |= (uint64_t)SRC(ip, i*64+10) << 10 | (uint64_t)SRC1(ip, i*64+11) << 43;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*33+ 6)  = (uint64_t)SRC(ip, i*64+11) >> 21;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*33+ 6) |= (uint64_t)SRC(ip, i*64+12) << 12 | (uint64_t)SRC1(ip, i*64+13) << 45;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*33+ 7)  = (uint64_t)SRC(ip, i*64+13) >> 19;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*33+ 7) |= (uint64_t)SRC(ip, i*64+14) << 14 | (uint64_t)SRC1(ip, i*64+15) << 47;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*33+ 8)  = (uint64_t)SRC(ip, i*64+15) >> 17;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*33+ 8) |= (uint64_t)SRC(ip, i*64+16) << 16 | (uint64_t)SRC1(ip, i*64+17) << 49;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*33+ 9)  = (uint64_t)SRC(ip, i*64+17) >> 15;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*33+ 9) |= (uint64_t)SRC(ip, i*64+18) << 18 | (uint64_t)SRC1(ip, i*64+19) << 51;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*33+10)  = (uint64_t)SRC(ip, i*64+19) >> 13;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*33+10) |= (uint64_t)SRC(ip, i*64+20) << 20 | (uint64_t)SRC1(ip, i*64+21) << 53;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*33+11)  = (uint64_t)SRC(ip, i*64+21) >> 11;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*33+11) |= (uint64_t)SRC(ip, i*64+22) << 22 | (uint64_t)SRC1(ip, i*64+23) << 55;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*33+12)  = (uint64_t)SRC(ip, i*64+23) >>  9;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*33+12) |= (uint64_t)SRC(ip, i*64+24) << 24 | (uint64_t)SRC1(ip, i*64+25) << 57;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*33+13)  = (uint64_t)SRC(ip, i*64+25) >>  7;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*33+13) |= (uint64_t)SRC(ip, i*64+26) << 26 | (uint64_t)SRC1(ip, i*64+27) << 59;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*33+14)  = (uint64_t)SRC(ip, i*64+27) >>  5;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*33+14) |= (uint64_t)SRC(ip, i*64+28) << 28 | (uint64_t)SRC1(ip, i*64+29) << 61;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*33+15)  = (uint64_t)SRC(ip, i*64+29) >>  3;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*33+15) |= (uint64_t)SRC(ip, i*64+30) << 30 | (uint64_t)SRC1(ip, i*64+31) << 63;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*33+16)  = (uint64_t)SRC(ip, i*64+31) >>  1;\
}

#define BITPACK64_33(ip,  op, parm) { \
  BITBLK64_33(ip, 0, op, parm);  SRCI(ip); op += 33*4/sizeof(op[0]);\
}

#define BITBLK64_34(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*17+ 0)  = (uint64_t)SRC(ip, i*32+ 0)       | (uint64_t)SRC1(ip, i*32+1) << 34;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*17+ 1)  = (uint64_t)SRC(ip, i*32+ 1) >> 30;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*17+ 1) |= (uint64_t)SRC(ip, i*32+ 2) <<  4 | (uint64_t)SRC1(ip, i*32+3) << 38;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*17+ 2)  = (uint64_t)SRC(ip, i*32+ 3) >> 26;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*17+ 2) |= (uint64_t)SRC(ip, i*32+ 4) <<  8 | (uint64_t)SRC1(ip, i*32+5) << 42;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*17+ 3)  = (uint64_t)SRC(ip, i*32+ 5) >> 22;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*17+ 3) |= (uint64_t)SRC(ip, i*32+ 6) << 12 | (uint64_t)SRC1(ip, i*32+7) << 46;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*17+ 4)  = (uint64_t)SRC(ip, i*32+ 7) >> 18;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*17+ 4) |= (uint64_t)SRC(ip, i*32+ 8) << 16 | (uint64_t)SRC1(ip, i*32+9) << 50;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*17+ 5)  = (uint64_t)SRC(ip, i*32+ 9) >> 14;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*17+ 5) |= (uint64_t)SRC(ip, i*32+10) << 20 | (uint64_t)SRC1(ip, i*32+11) << 54;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*17+ 6)  = (uint64_t)SRC(ip, i*32+11) >> 10;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*17+ 6) |= (uint64_t)SRC(ip, i*32+12) << 24 | (uint64_t)SRC1(ip, i*32+13) << 58;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*17+ 7)  = (uint64_t)SRC(ip, i*32+13) >>  6;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*17+ 7) |= (uint64_t)SRC(ip, i*32+14) << 28 | (uint64_t)SRC1(ip, i*32+15) << 62;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*17+ 8)  = (uint64_t)SRC(ip, i*32+15) >>  2 | (uint64_t)SRC1(ip, i*32+16) << 32;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*17+ 9)  = (uint64_t)SRC(ip, i*32+16) >> 32;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*17+ 9) |= (uint64_t)SRC(ip, i*32+17) <<  2 | (uint64_t)SRC1(ip, i*32+18) << 36;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*17+10)  = (uint64_t)SRC(ip, i*32+18) >> 28;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*17+10) |= (uint64_t)SRC(ip, i*32+19) <<  6 | (uint64_t)SRC1(ip, i*32+20) << 40;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*17+11)  = (uint64_t)SRC(ip, i*32+20) >> 24;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*17+11) |= (uint64_t)SRC(ip, i*32+21) << 10 | (uint64_t)SRC1(ip, i*32+22) << 44;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*17+12)  = (uint64_t)SRC(ip, i*32+22) >> 20;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*17+12) |= (uint64_t)SRC(ip, i*32+23) << 14 | (uint64_t)SRC1(ip, i*32+24) << 48;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*17+13)  = (uint64_t)SRC(ip, i*32+24) >> 16;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*17+13) |= (uint64_t)SRC(ip, i*32+25) << 18 | (uint64_t)SRC1(ip, i*32+26) << 52;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*17+14)  = (uint64_t)SRC(ip, i*32+26) >> 12;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*17+14) |= (uint64_t)SRC(ip, i*32+27) << 22 | (uint64_t)SRC1(ip, i*32+28) << 56;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*17+15)  = (uint64_t)SRC(ip, i*32+28) >>  8;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*17+15) |= (uint64_t)SRC(ip, i*32+29) << 26 | (uint64_t)SRC1(ip, i*32+30) << 60;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*17+16)  = (uint64_t)SRC(ip, i*32+30) >>  4;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*17+16) |= (uint64_t)SRC(ip, i*32+31) << 30;\
}

#define BITPACK64_34(ip,  op, parm) { \
  BITBLK64_34(ip, 0, op, parm);  SRCI(ip); op += 34*4/sizeof(op[0]);\
}

#define BITBLK64_35(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*35+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 35;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*35+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 29;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*35+ 1) |= (uint64_t)SRC(ip, i*64+ 2) <<  6 | (uint64_t)SRC1(ip, i*64+3) << 41;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*35+ 2)  = (uint64_t)SRC(ip, i*64+ 3) >> 23;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*35+ 2) |= (uint64_t)SRC(ip, i*64+ 4) << 12 | (uint64_t)SRC1(ip, i*64+5) << 47;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*35+ 3)  = (uint64_t)SRC(ip, i*64+ 5) >> 17;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*35+ 3) |= (uint64_t)SRC(ip, i*64+ 6) << 18 | (uint64_t)SRC1(ip, i*64+7) << 53;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*35+ 4)  = (uint64_t)SRC(ip, i*64+ 7) >> 11;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*35+ 4) |= (uint64_t)SRC(ip, i*64+ 8) << 24 | (uint64_t)SRC1(ip, i*64+9) << 59;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*35+ 5)  = (uint64_t)SRC(ip, i*64+ 9) >>  5 | (uint64_t)SRC1(ip, i*64+10) << 30;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*35+ 6)  = (uint64_t)SRC(ip, i*64+10) >> 34;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*35+ 6) |= (uint64_t)SRC(ip, i*64+11) <<  1 | (uint64_t)SRC1(ip, i*64+12) << 36;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*35+ 7)  = (uint64_t)SRC(ip, i*64+12) >> 28;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*35+ 7) |= (uint64_t)SRC(ip, i*64+13) <<  7 | (uint64_t)SRC1(ip, i*64+14) << 42;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*35+ 8)  = (uint64_t)SRC(ip, i*64+14) >> 22;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*35+ 8) |= (uint64_t)SRC(ip, i*64+15) << 13 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*35+ 9)  = (uint64_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*35+ 9) |= (uint64_t)SRC(ip, i*64+17) << 19 | (uint64_t)SRC1(ip, i*64+18) << 54;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*35+10)  = (uint64_t)SRC(ip, i*64+18) >> 10;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*35+10) |= (uint64_t)SRC(ip, i*64+19) << 25 | (uint64_t)SRC1(ip, i*64+20) << 60;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*35+11)  = (uint64_t)SRC(ip, i*64+20) >>  4 | (uint64_t)SRC1(ip, i*64+21) << 31;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*35+12)  = (uint64_t)SRC(ip, i*64+21) >> 33;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*35+12) |= (uint64_t)SRC(ip, i*64+22) <<  2 | (uint64_t)SRC1(ip, i*64+23) << 37;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*35+13)  = (uint64_t)SRC(ip, i*64+23) >> 27;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*35+13) |= (uint64_t)SRC(ip, i*64+24) <<  8 | (uint64_t)SRC1(ip, i*64+25) << 43;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*35+14)  = (uint64_t)SRC(ip, i*64+25) >> 21;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*35+14) |= (uint64_t)SRC(ip, i*64+26) << 14 | (uint64_t)SRC1(ip, i*64+27) << 49;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*35+15)  = (uint64_t)SRC(ip, i*64+27) >> 15;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*35+15) |= (uint64_t)SRC(ip, i*64+28) << 20 | (uint64_t)SRC1(ip, i*64+29) << 55;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*35+16)  = (uint64_t)SRC(ip, i*64+29) >>  9;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*35+16) |= (uint64_t)SRC(ip, i*64+30) << 26 | (uint64_t)SRC1(ip, i*64+31) << 61;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*35+17)  = (uint64_t)SRC(ip, i*64+31) >>  3;\
}

#define BITPACK64_35(ip,  op, parm) { \
  BITBLK64_35(ip, 0, op, parm);  SRCI(ip); op += 35*4/sizeof(op[0]);\
}

#define BITBLK64_36(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*9+ 0)  = (uint64_t)SRC(ip, i*16+ 0)       | (uint64_t)SRC1(ip, i*16+1) << 36;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*9+ 1)  = (uint64_t)SRC(ip, i*16+ 1) >> 28;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*9+ 1) |= (uint64_t)SRC(ip, i*16+ 2) <<  8 | (uint64_t)SRC1(ip, i*16+3) << 44;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*9+ 2)  = (uint64_t)SRC(ip, i*16+ 3) >> 20;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*9+ 2) |= (uint64_t)SRC(ip, i*16+ 4) << 16 | (uint64_t)SRC1(ip, i*16+5) << 52;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*9+ 3)  = (uint64_t)SRC(ip, i*16+ 5) >> 12;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*9+ 3) |= (uint64_t)SRC(ip, i*16+ 6) << 24 | (uint64_t)SRC1(ip, i*16+7) << 60;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*9+ 4)  = (uint64_t)SRC(ip, i*16+ 7) >>  4 | (uint64_t)SRC1(ip, i*16+8) << 32;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*9+ 5)  = (uint64_t)SRC(ip, i*16+ 8) >> 32;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*9+ 5) |= (uint64_t)SRC(ip, i*16+ 9) <<  4 | (uint64_t)SRC1(ip, i*16+10) << 40;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*9+ 6)  = (uint64_t)SRC(ip, i*16+10) >> 24;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*9+ 6) |= (uint64_t)SRC(ip, i*16+11) << 12 | (uint64_t)SRC1(ip, i*16+12) << 48;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*9+ 7)  = (uint64_t)SRC(ip, i*16+12) >> 16;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*9+ 7) |= (uint64_t)SRC(ip, i*16+13) << 20 | (uint64_t)SRC1(ip, i*16+14) << 56;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*9+ 8)  = (uint64_t)SRC(ip, i*16+14) >>  8;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*9+ 8) |= (uint64_t)SRC(ip, i*16+15) << 28;\
}

#define BITPACK64_36(ip,  op, parm) { \
  BITBLK64_36(ip, 0, op, parm);\
  BITBLK64_36(ip, 1, op, parm);  SRCI(ip); op += 36*4/sizeof(op[0]);\
}

#define BITBLK64_37(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*37+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 37;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*37+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 27;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*37+ 1) |= (uint64_t)SRC(ip, i*64+ 2) << 10 | (uint64_t)SRC1(ip, i*64+3) << 47;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*37+ 2)  = (uint64_t)SRC(ip, i*64+ 3) >> 17;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*37+ 2) |= (uint64_t)SRC(ip, i*64+ 4) << 20 | (uint64_t)SRC1(ip, i*64+5) << 57;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*37+ 3)  = (uint64_t)SRC(ip, i*64+ 5) >>  7 | (uint64_t)SRC1(ip, i*64+6) << 30;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*37+ 4)  = (uint64_t)SRC(ip, i*64+ 6) >> 34;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*37+ 4) |= (uint64_t)SRC(ip, i*64+ 7) <<  3 | (uint64_t)SRC1(ip, i*64+8) << 40;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*37+ 5)  = (uint64_t)SRC(ip, i*64+ 8) >> 24;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*37+ 5) |= (uint64_t)SRC(ip, i*64+ 9) << 13 | (uint64_t)SRC1(ip, i*64+10) << 50;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*37+ 6)  = (uint64_t)SRC(ip, i*64+10) >> 14;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*37+ 6) |= (uint64_t)SRC(ip, i*64+11) << 23 | (uint64_t)SRC1(ip, i*64+12) << 60;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*37+ 7)  = (uint64_t)SRC(ip, i*64+12) >>  4 | (uint64_t)SRC1(ip, i*64+13) << 33;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*37+ 8)  = (uint64_t)SRC(ip, i*64+13) >> 31;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*37+ 8) |= (uint64_t)SRC(ip, i*64+14) <<  6 | (uint64_t)SRC1(ip, i*64+15) << 43;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*37+ 9)  = (uint64_t)SRC(ip, i*64+15) >> 21;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*37+ 9) |= (uint64_t)SRC(ip, i*64+16) << 16 | (uint64_t)SRC1(ip, i*64+17) << 53;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*37+10)  = (uint64_t)SRC(ip, i*64+17) >> 11;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*37+10) |= (uint64_t)SRC(ip, i*64+18) << 26 | (uint64_t)SRC1(ip, i*64+19) << 63;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*37+11)  = (uint64_t)SRC(ip, i*64+19) >>  1 | (uint64_t)SRC1(ip, i*64+20) << 36;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*37+12)  = (uint64_t)SRC(ip, i*64+20) >> 28;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*37+12) |= (uint64_t)SRC(ip, i*64+21) <<  9 | (uint64_t)SRC1(ip, i*64+22) << 46;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*37+13)  = (uint64_t)SRC(ip, i*64+22) >> 18;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*37+13) |= (uint64_t)SRC(ip, i*64+23) << 19 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*37+14)  = (uint64_t)SRC(ip, i*64+24) >>  8 | (uint64_t)SRC1(ip, i*64+25) << 29;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*37+15)  = (uint64_t)SRC(ip, i*64+25) >> 35;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*37+15) |= (uint64_t)SRC(ip, i*64+26) <<  2 | (uint64_t)SRC1(ip, i*64+27) << 39;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*37+16)  = (uint64_t)SRC(ip, i*64+27) >> 25;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*37+16) |= (uint64_t)SRC(ip, i*64+28) << 12 | (uint64_t)SRC1(ip, i*64+29) << 49;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*37+17)  = (uint64_t)SRC(ip, i*64+29) >> 15;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*37+17) |= (uint64_t)SRC(ip, i*64+30) << 22 | (uint64_t)SRC1(ip, i*64+31) << 59;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*37+18)  = (uint64_t)SRC(ip, i*64+31) >>  5;\
}

#define BITPACK64_37(ip,  op, parm) { \
  BITBLK64_37(ip, 0, op, parm);  SRCI(ip); op += 37*4/sizeof(op[0]);\
}

#define BITBLK64_38(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*19+ 0)  = (uint64_t)SRC(ip, i*32+ 0)       | (uint64_t)SRC1(ip, i*32+1) << 38;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*19+ 1)  = (uint64_t)SRC(ip, i*32+ 1) >> 26;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*19+ 1) |= (uint64_t)SRC(ip, i*32+ 2) << 12 | (uint64_t)SRC1(ip, i*32+3) << 50;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*19+ 2)  = (uint64_t)SRC(ip, i*32+ 3) >> 14;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*19+ 2) |= (uint64_t)SRC(ip, i*32+ 4) << 24 | (uint64_t)SRC1(ip, i*32+5) << 62;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*19+ 3)  = (uint64_t)SRC(ip, i*32+ 5) >>  2 | (uint64_t)SRC1(ip, i*32+6) << 36;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*19+ 4)  = (uint64_t)SRC(ip, i*32+ 6) >> 28;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*19+ 4) |= (uint64_t)SRC(ip, i*32+ 7) << 10 | (uint64_t)SRC1(ip, i*32+8) << 48;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*19+ 5)  = (uint64_t)SRC(ip, i*32+ 8) >> 16;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*19+ 5) |= (uint64_t)SRC(ip, i*32+ 9) << 22 | (uint64_t)SRC1(ip, i*32+10) << 60;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*19+ 6)  = (uint64_t)SRC(ip, i*32+10) >>  4 | (uint64_t)SRC1(ip, i*32+11) << 34;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*19+ 7)  = (uint64_t)SRC(ip, i*32+11) >> 30;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*19+ 7) |= (uint64_t)SRC(ip, i*32+12) <<  8 | (uint64_t)SRC1(ip, i*32+13) << 46;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*19+ 8)  = (uint64_t)SRC(ip, i*32+13) >> 18;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*19+ 8) |= (uint64_t)SRC(ip, i*32+14) << 20 | (uint64_t)SRC1(ip, i*32+15) << 58;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*19+ 9)  = (uint64_t)SRC(ip, i*32+15) >>  6 | (uint64_t)SRC1(ip, i*32+16) << 32;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*19+10)  = (uint64_t)SRC(ip, i*32+16) >> 32;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*19+10) |= (uint64_t)SRC(ip, i*32+17) <<  6 | (uint64_t)SRC1(ip, i*32+18) << 44;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*19+11)  = (uint64_t)SRC(ip, i*32+18) >> 20;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*19+11) |= (uint64_t)SRC(ip, i*32+19) << 18 | (uint64_t)SRC1(ip, i*32+20) << 56;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*19+12)  = (uint64_t)SRC(ip, i*32+20) >>  8 | (uint64_t)SRC1(ip, i*32+21) << 30;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*19+13)  = (uint64_t)SRC(ip, i*32+21) >> 34;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*19+13) |= (uint64_t)SRC(ip, i*32+22) <<  4 | (uint64_t)SRC1(ip, i*32+23) << 42;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*19+14)  = (uint64_t)SRC(ip, i*32+23) >> 22;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*19+14) |= (uint64_t)SRC(ip, i*32+24) << 16 | (uint64_t)SRC1(ip, i*32+25) << 54;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*19+15)  = (uint64_t)SRC(ip, i*32+25) >> 10 | (uint64_t)SRC1(ip, i*32+26) << 28;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*19+16)  = (uint64_t)SRC(ip, i*32+26) >> 36;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*19+16) |= (uint64_t)SRC(ip, i*32+27) <<  2 | (uint64_t)SRC1(ip, i*32+28) << 40;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*19+17)  = (uint64_t)SRC(ip, i*32+28) >> 24;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*19+17) |= (uint64_t)SRC(ip, i*32+29) << 14 | (uint64_t)SRC1(ip, i*32+30) << 52;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*19+18)  = (uint64_t)SRC(ip, i*32+30) >> 12;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*19+18) |= (uint64_t)SRC(ip, i*32+31) << 26;\
}

#define BITPACK64_38(ip,  op, parm) { \
  BITBLK64_38(ip, 0, op, parm);  SRCI(ip); op += 38*4/sizeof(op[0]);\
}

#define BITBLK64_39(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*39+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 39;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*39+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 25;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*39+ 1) |= (uint64_t)SRC(ip, i*64+ 2) << 14 | (uint64_t)SRC1(ip, i*64+3) << 53;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*39+ 2)  = (uint64_t)SRC(ip, i*64+ 3) >> 11 | (uint64_t)SRC1(ip, i*64+4) << 28;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*39+ 3)  = (uint64_t)SRC(ip, i*64+ 4) >> 36;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*39+ 3) |= (uint64_t)SRC(ip, i*64+ 5) <<  3 | (uint64_t)SRC1(ip, i*64+6) << 42;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*39+ 4)  = (uint64_t)SRC(ip, i*64+ 6) >> 22;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*39+ 4) |= (uint64_t)SRC(ip, i*64+ 7) << 17 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*39+ 5)  = (uint64_t)SRC(ip, i*64+ 8) >>  8 | (uint64_t)SRC1(ip, i*64+9) << 31;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*39+ 6)  = (uint64_t)SRC(ip, i*64+ 9) >> 33;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*39+ 6) |= (uint64_t)SRC(ip, i*64+10) <<  6 | (uint64_t)SRC1(ip, i*64+11) << 45;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*39+ 7)  = (uint64_t)SRC(ip, i*64+11) >> 19;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*39+ 7) |= (uint64_t)SRC(ip, i*64+12) << 20 | (uint64_t)SRC1(ip, i*64+13) << 59;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*39+ 8)  = (uint64_t)SRC(ip, i*64+13) >>  5 | (uint64_t)SRC1(ip, i*64+14) << 34;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*39+ 9)  = (uint64_t)SRC(ip, i*64+14) >> 30;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*39+ 9) |= (uint64_t)SRC(ip, i*64+15) <<  9 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*39+10)  = (uint64_t)SRC(ip, i*64+16) >> 16;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*39+10) |= (uint64_t)SRC(ip, i*64+17) << 23 | (uint64_t)SRC1(ip, i*64+18) << 62;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*39+11)  = (uint64_t)SRC(ip, i*64+18) >>  2 | (uint64_t)SRC1(ip, i*64+19) << 37;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*39+12)  = (uint64_t)SRC(ip, i*64+19) >> 27;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*39+12) |= (uint64_t)SRC(ip, i*64+20) << 12 | (uint64_t)SRC1(ip, i*64+21) << 51;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*39+13)  = (uint64_t)SRC(ip, i*64+21) >> 13 | (uint64_t)SRC1(ip, i*64+22) << 26;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*39+14)  = (uint64_t)SRC(ip, i*64+22) >> 38;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*39+14) |= (uint64_t)SRC(ip, i*64+23) <<  1 | (uint64_t)SRC1(ip, i*64+24) << 40;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*39+15)  = (uint64_t)SRC(ip, i*64+24) >> 24;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*39+15) |= (uint64_t)SRC(ip, i*64+25) << 15 | (uint64_t)SRC1(ip, i*64+26) << 54;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*39+16)  = (uint64_t)SRC(ip, i*64+26) >> 10 | (uint64_t)SRC1(ip, i*64+27) << 29;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*39+17)  = (uint64_t)SRC(ip, i*64+27) >> 35;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*39+17) |= (uint64_t)SRC(ip, i*64+28) <<  4 | (uint64_t)SRC1(ip, i*64+29) << 43;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*39+18)  = (uint64_t)SRC(ip, i*64+29) >> 21;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*39+18) |= (uint64_t)SRC(ip, i*64+30) << 18 | (uint64_t)SRC1(ip, i*64+31) << 57;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*39+19)  = (uint64_t)SRC(ip, i*64+31) >>  7;\
}

#define BITPACK64_39(ip,  op, parm) { \
  BITBLK64_39(ip, 0, op, parm);  SRCI(ip); op += 39*4/sizeof(op[0]);\
}

#define BITBLK64_40(ip, i, op, parm) { ;\
  IPPB(ip, i*8+ 0, parm); *((uint64_t *)op+i*5+ 0)  = (uint64_t)SRC(ip, i*8+ 0)       | (uint64_t)SRC1(ip, i*8+1) << 40;\
  IPPB(ip, i*8+ 1, parm); *((uint64_t *)op+i*5+ 1)  = (uint64_t)SRC(ip, i*8+ 1) >> 24;\
  IPPB(ip, i*8+ 2, parm); *((uint64_t *)op+i*5+ 1) |= (uint64_t)SRC(ip, i*8+ 2) << 16 | (uint64_t)SRC1(ip, i*8+3) << 56;\
  IPPB(ip, i*8+ 3, parm); *((uint64_t *)op+i*5+ 2)  = (uint64_t)SRC(ip, i*8+ 3) >>  8 | (uint64_t)SRC1(ip, i*8+4) << 32;\
  IPPB(ip, i*8+ 4, parm); *((uint64_t *)op+i*5+ 3)  = (uint64_t)SRC(ip, i*8+ 4) >> 32;\
  IPPB(ip, i*8+ 5, parm); *((uint64_t *)op+i*5+ 3) |= (uint64_t)SRC(ip, i*8+ 5) <<  8 | (uint64_t)SRC1(ip, i*8+6) << 48;\
  IPPB(ip, i*8+ 6, parm); *((uint64_t *)op+i*5+ 4)  = (uint64_t)SRC(ip, i*8+ 6) >> 16;\
  IPPB(ip, i*8+ 7, parm); *((uint64_t *)op+i*5+ 4) |= (uint64_t)SRC(ip, i*8+ 7) << 24;\
}

#define BITPACK64_40(ip,  op, parm) { \
  BITBLK64_40(ip, 0, op, parm);\
  BITBLK64_40(ip, 1, op, parm);\
  BITBLK64_40(ip, 2, op, parm);\
  BITBLK64_40(ip, 3, op, parm);  SRCI(ip); op += 40*4/sizeof(op[0]);\
}

#define BITBLK64_41(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*41+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 41;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*41+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 23;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*41+ 1) |= (uint64_t)SRC(ip, i*64+ 2) << 18 | (uint64_t)SRC1(ip, i*64+3) << 59;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*41+ 2)  = (uint64_t)SRC(ip, i*64+ 3) >>  5 | (uint64_t)SRC1(ip, i*64+4) << 36;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*41+ 3)  = (uint64_t)SRC(ip, i*64+ 4) >> 28;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*41+ 3) |= (uint64_t)SRC(ip, i*64+ 5) << 13 | (uint64_t)SRC1(ip, i*64+6) << 54;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*41+ 4)  = (uint64_t)SRC(ip, i*64+ 6) >> 10 | (uint64_t)SRC1(ip, i*64+7) << 31;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*41+ 5)  = (uint64_t)SRC(ip, i*64+ 7) >> 33;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*41+ 5) |= (uint64_t)SRC(ip, i*64+ 8) <<  8 | (uint64_t)SRC1(ip, i*64+9) << 49;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*41+ 6)  = (uint64_t)SRC(ip, i*64+ 9) >> 15 | (uint64_t)SRC1(ip, i*64+10) << 26;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*41+ 7)  = (uint64_t)SRC(ip, i*64+10) >> 38;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*41+ 7) |= (uint64_t)SRC(ip, i*64+11) <<  3 | (uint64_t)SRC1(ip, i*64+12) << 44;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*41+ 8)  = (uint64_t)SRC(ip, i*64+12) >> 20;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*41+ 8) |= (uint64_t)SRC(ip, i*64+13) << 21 | (uint64_t)SRC1(ip, i*64+14) << 62;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*41+ 9)  = (uint64_t)SRC(ip, i*64+14) >>  2 | (uint64_t)SRC1(ip, i*64+15) << 39;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*41+10)  = (uint64_t)SRC(ip, i*64+15) >> 25;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*41+10) |= (uint64_t)SRC(ip, i*64+16) << 16 | (uint64_t)SRC1(ip, i*64+17) << 57;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*41+11)  = (uint64_t)SRC(ip, i*64+17) >>  7 | (uint64_t)SRC1(ip, i*64+18) << 34;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*41+12)  = (uint64_t)SRC(ip, i*64+18) >> 30;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*41+12) |= (uint64_t)SRC(ip, i*64+19) << 11 | (uint64_t)SRC1(ip, i*64+20) << 52;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*41+13)  = (uint64_t)SRC(ip, i*64+20) >> 12 | (uint64_t)SRC1(ip, i*64+21) << 29;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*41+14)  = (uint64_t)SRC(ip, i*64+21) >> 35;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*41+14) |= (uint64_t)SRC(ip, i*64+22) <<  6 | (uint64_t)SRC1(ip, i*64+23) << 47;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*41+15)  = (uint64_t)SRC(ip, i*64+23) >> 17 | (uint64_t)SRC1(ip, i*64+24) << 24;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*41+16)  = (uint64_t)SRC(ip, i*64+24) >> 40;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*41+16) |= (uint64_t)SRC(ip, i*64+25) <<  1 | (uint64_t)SRC1(ip, i*64+26) << 42;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*41+17)  = (uint64_t)SRC(ip, i*64+26) >> 22;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*41+17) |= (uint64_t)SRC(ip, i*64+27) << 19 | (uint64_t)SRC1(ip, i*64+28) << 60;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*41+18)  = (uint64_t)SRC(ip, i*64+28) >>  4 | (uint64_t)SRC1(ip, i*64+29) << 37;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*41+19)  = (uint64_t)SRC(ip, i*64+29) >> 27;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*41+19) |= (uint64_t)SRC(ip, i*64+30) << 14 | (uint64_t)SRC1(ip, i*64+31) << 55;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*41+20)  = (uint64_t)SRC(ip, i*64+31) >>  9;\
}

#define BITPACK64_41(ip,  op, parm) { \
  BITBLK64_41(ip, 0, op, parm);  SRCI(ip); op += 41*4/sizeof(op[0]);\
}

#define BITBLK64_42(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*21+ 0)  = (uint64_t)SRC(ip, i*32+ 0)       | (uint64_t)SRC1(ip, i*32+1) << 42;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*21+ 1)  = (uint64_t)SRC(ip, i*32+ 1) >> 22;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*21+ 1) |= (uint64_t)SRC(ip, i*32+ 2) << 20 | (uint64_t)SRC1(ip, i*32+3) << 62;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*21+ 2)  = (uint64_t)SRC(ip, i*32+ 3) >>  2 | (uint64_t)SRC1(ip, i*32+4) << 40;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*21+ 3)  = (uint64_t)SRC(ip, i*32+ 4) >> 24;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*21+ 3) |= (uint64_t)SRC(ip, i*32+ 5) << 18 | (uint64_t)SRC1(ip, i*32+6) << 60;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*21+ 4)  = (uint64_t)SRC(ip, i*32+ 6) >>  4 | (uint64_t)SRC1(ip, i*32+7) << 38;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*21+ 5)  = (uint64_t)SRC(ip, i*32+ 7) >> 26;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*21+ 5) |= (uint64_t)SRC(ip, i*32+ 8) << 16 | (uint64_t)SRC1(ip, i*32+9) << 58;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*21+ 6)  = (uint64_t)SRC(ip, i*32+ 9) >>  6 | (uint64_t)SRC1(ip, i*32+10) << 36;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*21+ 7)  = (uint64_t)SRC(ip, i*32+10) >> 28;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*21+ 7) |= (uint64_t)SRC(ip, i*32+11) << 14 | (uint64_t)SRC1(ip, i*32+12) << 56;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*21+ 8)  = (uint64_t)SRC(ip, i*32+12) >>  8 | (uint64_t)SRC1(ip, i*32+13) << 34;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*21+ 9)  = (uint64_t)SRC(ip, i*32+13) >> 30;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*21+ 9) |= (uint64_t)SRC(ip, i*32+14) << 12 | (uint64_t)SRC1(ip, i*32+15) << 54;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*21+10)  = (uint64_t)SRC(ip, i*32+15) >> 10 | (uint64_t)SRC1(ip, i*32+16) << 32;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*21+11)  = (uint64_t)SRC(ip, i*32+16) >> 32;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*21+11) |= (uint64_t)SRC(ip, i*32+17) << 10 | (uint64_t)SRC1(ip, i*32+18) << 52;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*21+12)  = (uint64_t)SRC(ip, i*32+18) >> 12 | (uint64_t)SRC1(ip, i*32+19) << 30;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*21+13)  = (uint64_t)SRC(ip, i*32+19) >> 34;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*21+13) |= (uint64_t)SRC(ip, i*32+20) <<  8 | (uint64_t)SRC1(ip, i*32+21) << 50;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*21+14)  = (uint64_t)SRC(ip, i*32+21) >> 14 | (uint64_t)SRC1(ip, i*32+22) << 28;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*21+15)  = (uint64_t)SRC(ip, i*32+22) >> 36;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*21+15) |= (uint64_t)SRC(ip, i*32+23) <<  6 | (uint64_t)SRC1(ip, i*32+24) << 48;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*21+16)  = (uint64_t)SRC(ip, i*32+24) >> 16 | (uint64_t)SRC1(ip, i*32+25) << 26;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*21+17)  = (uint64_t)SRC(ip, i*32+25) >> 38;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*21+17) |= (uint64_t)SRC(ip, i*32+26) <<  4 | (uint64_t)SRC1(ip, i*32+27) << 46;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*21+18)  = (uint64_t)SRC(ip, i*32+27) >> 18 | (uint64_t)SRC1(ip, i*32+28) << 24;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*21+19)  = (uint64_t)SRC(ip, i*32+28) >> 40;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*21+19) |= (uint64_t)SRC(ip, i*32+29) <<  2 | (uint64_t)SRC1(ip, i*32+30) << 44;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*21+20)  = (uint64_t)SRC(ip, i*32+30) >> 20;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*21+20) |= (uint64_t)SRC(ip, i*32+31) << 22;\
}

#define BITPACK64_42(ip,  op, parm) { \
  BITBLK64_42(ip, 0, op, parm);  SRCI(ip); op += 42*4/sizeof(op[0]);\
}

#define BITBLK64_43(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*43+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 43;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*43+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 21 | (uint64_t)SRC1(ip, i*64+2) << 22;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*43+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 42;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*43+ 2) |= (uint64_t)SRC(ip, i*64+ 3) <<  1 | (uint64_t)SRC1(ip, i*64+4) << 44;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*43+ 3)  = (uint64_t)SRC(ip, i*64+ 4) >> 20 | (uint64_t)SRC1(ip, i*64+5) << 23;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*43+ 4)  = (uint64_t)SRC(ip, i*64+ 5) >> 41;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*43+ 4) |= (uint64_t)SRC(ip, i*64+ 6) <<  2 | (uint64_t)SRC1(ip, i*64+7) << 45;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*43+ 5)  = (uint64_t)SRC(ip, i*64+ 7) >> 19 | (uint64_t)SRC1(ip, i*64+8) << 24;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*43+ 6)  = (uint64_t)SRC(ip, i*64+ 8) >> 40;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*43+ 6) |= (uint64_t)SRC(ip, i*64+ 9) <<  3 | (uint64_t)SRC1(ip, i*64+10) << 46;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*43+ 7)  = (uint64_t)SRC(ip, i*64+10) >> 18 | (uint64_t)SRC1(ip, i*64+11) << 25;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*43+ 8)  = (uint64_t)SRC(ip, i*64+11) >> 39;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*43+ 8) |= (uint64_t)SRC(ip, i*64+12) <<  4 | (uint64_t)SRC1(ip, i*64+13) << 47;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*43+ 9)  = (uint64_t)SRC(ip, i*64+13) >> 17 | (uint64_t)SRC1(ip, i*64+14) << 26;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*43+10)  = (uint64_t)SRC(ip, i*64+14) >> 38;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*43+10) |= (uint64_t)SRC(ip, i*64+15) <<  5 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*43+11)  = (uint64_t)SRC(ip, i*64+16) >> 16 | (uint64_t)SRC1(ip, i*64+17) << 27;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*43+12)  = (uint64_t)SRC(ip, i*64+17) >> 37;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*43+12) |= (uint64_t)SRC(ip, i*64+18) <<  6 | (uint64_t)SRC1(ip, i*64+19) << 49;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*43+13)  = (uint64_t)SRC(ip, i*64+19) >> 15 | (uint64_t)SRC1(ip, i*64+20) << 28;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*43+14)  = (uint64_t)SRC(ip, i*64+20) >> 36;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*43+14) |= (uint64_t)SRC(ip, i*64+21) <<  7 | (uint64_t)SRC1(ip, i*64+22) << 50;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*43+15)  = (uint64_t)SRC(ip, i*64+22) >> 14 | (uint64_t)SRC1(ip, i*64+23) << 29;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*43+16)  = (uint64_t)SRC(ip, i*64+23) >> 35;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*43+16) |= (uint64_t)SRC(ip, i*64+24) <<  8 | (uint64_t)SRC1(ip, i*64+25) << 51;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*43+17)  = (uint64_t)SRC(ip, i*64+25) >> 13 | (uint64_t)SRC1(ip, i*64+26) << 30;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*43+18)  = (uint64_t)SRC(ip, i*64+26) >> 34;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*43+18) |= (uint64_t)SRC(ip, i*64+27) <<  9 | (uint64_t)SRC1(ip, i*64+28) << 52;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*43+19)  = (uint64_t)SRC(ip, i*64+28) >> 12 | (uint64_t)SRC1(ip, i*64+29) << 31;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*43+20)  = (uint64_t)SRC(ip, i*64+29) >> 33;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*43+20) |= (uint64_t)SRC(ip, i*64+30) << 10 | (uint64_t)SRC1(ip, i*64+31) << 53;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*43+21)  = (uint64_t)SRC(ip, i*64+31) >> 11;\
}

#define BITPACK64_43(ip,  op, parm) { \
  BITBLK64_43(ip, 0, op, parm);  SRCI(ip); op += 43*4/sizeof(op[0]);\
}

#define BITBLK64_44(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*11+ 0)  = (uint64_t)SRC(ip, i*16+ 0)       | (uint64_t)SRC1(ip, i*16+1) << 44;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*11+ 1)  = (uint64_t)SRC(ip, i*16+ 1) >> 20 | (uint64_t)SRC1(ip, i*16+2) << 24;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*11+ 2)  = (uint64_t)SRC(ip, i*16+ 2) >> 40;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*11+ 2) |= (uint64_t)SRC(ip, i*16+ 3) <<  4 | (uint64_t)SRC1(ip, i*16+4) << 48;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*11+ 3)  = (uint64_t)SRC(ip, i*16+ 4) >> 16 | (uint64_t)SRC1(ip, i*16+5) << 28;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*11+ 4)  = (uint64_t)SRC(ip, i*16+ 5) >> 36;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*11+ 4) |= (uint64_t)SRC(ip, i*16+ 6) <<  8 | (uint64_t)SRC1(ip, i*16+7) << 52;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*11+ 5)  = (uint64_t)SRC(ip, i*16+ 7) >> 12 | (uint64_t)SRC1(ip, i*16+8) << 32;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*11+ 6)  = (uint64_t)SRC(ip, i*16+ 8) >> 32;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*11+ 6) |= (uint64_t)SRC(ip, i*16+ 9) << 12 | (uint64_t)SRC1(ip, i*16+10) << 56;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*11+ 7)  = (uint64_t)SRC(ip, i*16+10) >>  8 | (uint64_t)SRC1(ip, i*16+11) << 36;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*11+ 8)  = (uint64_t)SRC(ip, i*16+11) >> 28;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*11+ 8) |= (uint64_t)SRC(ip, i*16+12) << 16 | (uint64_t)SRC1(ip, i*16+13) << 60;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*11+ 9)  = (uint64_t)SRC(ip, i*16+13) >>  4 | (uint64_t)SRC1(ip, i*16+14) << 40;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*11+10)  = (uint64_t)SRC(ip, i*16+14) >> 24;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*11+10) |= (uint64_t)SRC(ip, i*16+15) << 20;\
}

#define BITPACK64_44(ip,  op, parm) { \
  BITBLK64_44(ip, 0, op, parm);\
  BITBLK64_44(ip, 1, op, parm);  SRCI(ip); op += 44*4/sizeof(op[0]);\
}

#define BITBLK64_45(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*45+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 45;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*45+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 19 | (uint64_t)SRC1(ip, i*64+2) << 26;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*45+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 38;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*45+ 2) |= (uint64_t)SRC(ip, i*64+ 3) <<  7 | (uint64_t)SRC1(ip, i*64+4) << 52;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*45+ 3)  = (uint64_t)SRC(ip, i*64+ 4) >> 12 | (uint64_t)SRC1(ip, i*64+5) << 33;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*45+ 4)  = (uint64_t)SRC(ip, i*64+ 5) >> 31;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*45+ 4) |= (uint64_t)SRC(ip, i*64+ 6) << 14 | (uint64_t)SRC1(ip, i*64+7) << 59;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*45+ 5)  = (uint64_t)SRC(ip, i*64+ 7) >>  5 | (uint64_t)SRC1(ip, i*64+8) << 40;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*45+ 6)  = (uint64_t)SRC(ip, i*64+ 8) >> 24 | (uint64_t)SRC1(ip, i*64+9) << 21;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*45+ 7)  = (uint64_t)SRC(ip, i*64+ 9) >> 43;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*45+ 7) |= (uint64_t)SRC(ip, i*64+10) <<  2 | (uint64_t)SRC1(ip, i*64+11) << 47;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*45+ 8)  = (uint64_t)SRC(ip, i*64+11) >> 17 | (uint64_t)SRC1(ip, i*64+12) << 28;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*45+ 9)  = (uint64_t)SRC(ip, i*64+12) >> 36;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*45+ 9) |= (uint64_t)SRC(ip, i*64+13) <<  9 | (uint64_t)SRC1(ip, i*64+14) << 54;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*45+10)  = (uint64_t)SRC(ip, i*64+14) >> 10 | (uint64_t)SRC1(ip, i*64+15) << 35;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*45+11)  = (uint64_t)SRC(ip, i*64+15) >> 29;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*45+11) |= (uint64_t)SRC(ip, i*64+16) << 16 | (uint64_t)SRC1(ip, i*64+17) << 61;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*45+12)  = (uint64_t)SRC(ip, i*64+17) >>  3 | (uint64_t)SRC1(ip, i*64+18) << 42;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*45+13)  = (uint64_t)SRC(ip, i*64+18) >> 22 | (uint64_t)SRC1(ip, i*64+19) << 23;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*45+14)  = (uint64_t)SRC(ip, i*64+19) >> 41;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*45+14) |= (uint64_t)SRC(ip, i*64+20) <<  4 | (uint64_t)SRC1(ip, i*64+21) << 49;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*45+15)  = (uint64_t)SRC(ip, i*64+21) >> 15 | (uint64_t)SRC1(ip, i*64+22) << 30;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*45+16)  = (uint64_t)SRC(ip, i*64+22) >> 34;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*45+16) |= (uint64_t)SRC(ip, i*64+23) << 11 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*45+17)  = (uint64_t)SRC(ip, i*64+24) >>  8 | (uint64_t)SRC1(ip, i*64+25) << 37;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*45+18)  = (uint64_t)SRC(ip, i*64+25) >> 27;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*45+18) |= (uint64_t)SRC(ip, i*64+26) << 18 | (uint64_t)SRC1(ip, i*64+27) << 63;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*45+19)  = (uint64_t)SRC(ip, i*64+27) >>  1 | (uint64_t)SRC1(ip, i*64+28) << 44;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*45+20)  = (uint64_t)SRC(ip, i*64+28) >> 20 | (uint64_t)SRC1(ip, i*64+29) << 25;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*45+21)  = (uint64_t)SRC(ip, i*64+29) >> 39;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*45+21) |= (uint64_t)SRC(ip, i*64+30) <<  6 | (uint64_t)SRC1(ip, i*64+31) << 51;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*45+22)  = (uint64_t)SRC(ip, i*64+31) >> 13;\
}

#define BITPACK64_45(ip,  op, parm) { \
  BITBLK64_45(ip, 0, op, parm);  SRCI(ip); op += 45*4/sizeof(op[0]);\
}

#define BITBLK64_46(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*23+ 0)  = (uint64_t)SRC(ip, i*32+ 0)       | (uint64_t)SRC1(ip, i*32+1) << 46;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*23+ 1)  = (uint64_t)SRC(ip, i*32+ 1) >> 18 | (uint64_t)SRC1(ip, i*32+2) << 28;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*23+ 2)  = (uint64_t)SRC(ip, i*32+ 2) >> 36;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*23+ 2) |= (uint64_t)SRC(ip, i*32+ 3) << 10 | (uint64_t)SRC1(ip, i*32+4) << 56;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*23+ 3)  = (uint64_t)SRC(ip, i*32+ 4) >>  8 | (uint64_t)SRC1(ip, i*32+5) << 38;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*23+ 4)  = (uint64_t)SRC(ip, i*32+ 5) >> 26 | (uint64_t)SRC1(ip, i*32+6) << 20;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*23+ 5)  = (uint64_t)SRC(ip, i*32+ 6) >> 44;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*23+ 5) |= (uint64_t)SRC(ip, i*32+ 7) <<  2 | (uint64_t)SRC1(ip, i*32+8) << 48;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*23+ 6)  = (uint64_t)SRC(ip, i*32+ 8) >> 16 | (uint64_t)SRC1(ip, i*32+9) << 30;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*23+ 7)  = (uint64_t)SRC(ip, i*32+ 9) >> 34;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*23+ 7) |= (uint64_t)SRC(ip, i*32+10) << 12 | (uint64_t)SRC1(ip, i*32+11) << 58;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*23+ 8)  = (uint64_t)SRC(ip, i*32+11) >>  6 | (uint64_t)SRC1(ip, i*32+12) << 40;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*23+ 9)  = (uint64_t)SRC(ip, i*32+12) >> 24 | (uint64_t)SRC1(ip, i*32+13) << 22;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*23+10)  = (uint64_t)SRC(ip, i*32+13) >> 42;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*23+10) |= (uint64_t)SRC(ip, i*32+14) <<  4 | (uint64_t)SRC1(ip, i*32+15) << 50;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*23+11)  = (uint64_t)SRC(ip, i*32+15) >> 14 | (uint64_t)SRC1(ip, i*32+16) << 32;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*23+12)  = (uint64_t)SRC(ip, i*32+16) >> 32;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*23+12) |= (uint64_t)SRC(ip, i*32+17) << 14 | (uint64_t)SRC1(ip, i*32+18) << 60;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*23+13)  = (uint64_t)SRC(ip, i*32+18) >>  4 | (uint64_t)SRC1(ip, i*32+19) << 42;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*23+14)  = (uint64_t)SRC(ip, i*32+19) >> 22 | (uint64_t)SRC1(ip, i*32+20) << 24;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*23+15)  = (uint64_t)SRC(ip, i*32+20) >> 40;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*23+15) |= (uint64_t)SRC(ip, i*32+21) <<  6 | (uint64_t)SRC1(ip, i*32+22) << 52;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*23+16)  = (uint64_t)SRC(ip, i*32+22) >> 12 | (uint64_t)SRC1(ip, i*32+23) << 34;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*23+17)  = (uint64_t)SRC(ip, i*32+23) >> 30;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*23+17) |= (uint64_t)SRC(ip, i*32+24) << 16 | (uint64_t)SRC1(ip, i*32+25) << 62;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*23+18)  = (uint64_t)SRC(ip, i*32+25) >>  2 | (uint64_t)SRC1(ip, i*32+26) << 44;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*23+19)  = (uint64_t)SRC(ip, i*32+26) >> 20 | (uint64_t)SRC1(ip, i*32+27) << 26;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*23+20)  = (uint64_t)SRC(ip, i*32+27) >> 38;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*23+20) |= (uint64_t)SRC(ip, i*32+28) <<  8 | (uint64_t)SRC1(ip, i*32+29) << 54;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*23+21)  = (uint64_t)SRC(ip, i*32+29) >> 10 | (uint64_t)SRC1(ip, i*32+30) << 36;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*23+22)  = (uint64_t)SRC(ip, i*32+30) >> 28;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*23+22) |= (uint64_t)SRC(ip, i*32+31) << 18;\
}

#define BITPACK64_46(ip,  op, parm) { \
  BITBLK64_46(ip, 0, op, parm);  SRCI(ip); op += 46*4/sizeof(op[0]);\
}

#define BITBLK64_47(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*47+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 47;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*47+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 17 | (uint64_t)SRC1(ip, i*64+2) << 30;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*47+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 34;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*47+ 2) |= (uint64_t)SRC(ip, i*64+ 3) << 13 | (uint64_t)SRC1(ip, i*64+4) << 60;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*47+ 3)  = (uint64_t)SRC(ip, i*64+ 4) >>  4 | (uint64_t)SRC1(ip, i*64+5) << 43;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*47+ 4)  = (uint64_t)SRC(ip, i*64+ 5) >> 21 | (uint64_t)SRC1(ip, i*64+6) << 26;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*47+ 5)  = (uint64_t)SRC(ip, i*64+ 6) >> 38;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*47+ 5) |= (uint64_t)SRC(ip, i*64+ 7) <<  9 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*47+ 6)  = (uint64_t)SRC(ip, i*64+ 8) >>  8 | (uint64_t)SRC1(ip, i*64+9) << 39;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*47+ 7)  = (uint64_t)SRC(ip, i*64+ 9) >> 25 | (uint64_t)SRC1(ip, i*64+10) << 22;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*47+ 8)  = (uint64_t)SRC(ip, i*64+10) >> 42;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*47+ 8) |= (uint64_t)SRC(ip, i*64+11) <<  5 | (uint64_t)SRC1(ip, i*64+12) << 52;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*47+ 9)  = (uint64_t)SRC(ip, i*64+12) >> 12 | (uint64_t)SRC1(ip, i*64+13) << 35;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*47+10)  = (uint64_t)SRC(ip, i*64+13) >> 29 | (uint64_t)SRC1(ip, i*64+14) << 18;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*47+11)  = (uint64_t)SRC(ip, i*64+14) >> 46;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*47+11) |= (uint64_t)SRC(ip, i*64+15) <<  1 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*47+12)  = (uint64_t)SRC(ip, i*64+16) >> 16 | (uint64_t)SRC1(ip, i*64+17) << 31;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*47+13)  = (uint64_t)SRC(ip, i*64+17) >> 33;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*47+13) |= (uint64_t)SRC(ip, i*64+18) << 14 | (uint64_t)SRC1(ip, i*64+19) << 61;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*47+14)  = (uint64_t)SRC(ip, i*64+19) >>  3 | (uint64_t)SRC1(ip, i*64+20) << 44;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*47+15)  = (uint64_t)SRC(ip, i*64+20) >> 20 | (uint64_t)SRC1(ip, i*64+21) << 27;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*47+16)  = (uint64_t)SRC(ip, i*64+21) >> 37;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*47+16) |= (uint64_t)SRC(ip, i*64+22) << 10 | (uint64_t)SRC1(ip, i*64+23) << 57;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*47+17)  = (uint64_t)SRC(ip, i*64+23) >>  7 | (uint64_t)SRC1(ip, i*64+24) << 40;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*47+18)  = (uint64_t)SRC(ip, i*64+24) >> 24 | (uint64_t)SRC1(ip, i*64+25) << 23;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*47+19)  = (uint64_t)SRC(ip, i*64+25) >> 41;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*47+19) |= (uint64_t)SRC(ip, i*64+26) <<  6 | (uint64_t)SRC1(ip, i*64+27) << 53;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*47+20)  = (uint64_t)SRC(ip, i*64+27) >> 11 | (uint64_t)SRC1(ip, i*64+28) << 36;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*47+21)  = (uint64_t)SRC(ip, i*64+28) >> 28 | (uint64_t)SRC1(ip, i*64+29) << 19;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*47+22)  = (uint64_t)SRC(ip, i*64+29) >> 45;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*47+22) |= (uint64_t)SRC(ip, i*64+30) <<  2 | (uint64_t)SRC1(ip, i*64+31) << 49;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*47+23)  = (uint64_t)SRC(ip, i*64+31) >> 15;\
}

#define BITPACK64_47(ip,  op, parm) { \
  BITBLK64_47(ip, 0, op, parm);  SRCI(ip); op += 47*4/sizeof(op[0]);\
}

#define BITBLK64_48(ip, i, op, parm) { ;\
  IPPB(ip, i*4+ 0, parm); *((uint64_t *)op+i*3+ 0)  = (uint64_t)SRC(ip, i*4+ 0)       | (uint64_t)SRC1(ip, i*4+1) << 48;\
  IPPB(ip, i*4+ 1, parm); *((uint64_t *)op+i*3+ 1)  = (uint64_t)SRC(ip, i*4+ 1) >> 16 | (uint64_t)SRC1(ip, i*4+2) << 32;\
  IPPB(ip, i*4+ 2, parm); *((uint64_t *)op+i*3+ 2)  = (uint64_t)SRC(ip, i*4+ 2) >> 32;\
  IPPB(ip, i*4+ 3, parm); *((uint64_t *)op+i*3+ 2) |= (uint64_t)SRC(ip, i*4+ 3) << 16;\
}

#define BITPACK64_48(ip,  op, parm) { \
  BITBLK64_48(ip, 0, op, parm);\
  BITBLK64_48(ip, 1, op, parm);\
  BITBLK64_48(ip, 2, op, parm);\
  BITBLK64_48(ip, 3, op, parm);\
  BITBLK64_48(ip, 4, op, parm);\
  BITBLK64_48(ip, 5, op, parm);\
  BITBLK64_48(ip, 6, op, parm);\
  BITBLK64_48(ip, 7, op, parm);  SRCI(ip); op += 48*4/sizeof(op[0]);\
}

#define BITBLK64_49(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*49+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 49;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*49+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 15 | (uint64_t)SRC1(ip, i*64+2) << 34;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*49+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 30 | (uint64_t)SRC1(ip, i*64+3) << 19;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*49+ 3)  = (uint64_t)SRC(ip, i*64+ 3) >> 45;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*49+ 3) |= (uint64_t)SRC(ip, i*64+ 4) <<  4 | (uint64_t)SRC1(ip, i*64+5) << 53;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*49+ 4)  = (uint64_t)SRC(ip, i*64+ 5) >> 11 | (uint64_t)SRC1(ip, i*64+6) << 38;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*49+ 5)  = (uint64_t)SRC(ip, i*64+ 6) >> 26 | (uint64_t)SRC1(ip, i*64+7) << 23;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*49+ 6)  = (uint64_t)SRC(ip, i*64+ 7) >> 41;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*49+ 6) |= (uint64_t)SRC(ip, i*64+ 8) <<  8 | (uint64_t)SRC1(ip, i*64+9) << 57;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*49+ 7)  = (uint64_t)SRC(ip, i*64+ 9) >>  7 | (uint64_t)SRC1(ip, i*64+10) << 42;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*49+ 8)  = (uint64_t)SRC(ip, i*64+10) >> 22 | (uint64_t)SRC1(ip, i*64+11) << 27;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*49+ 9)  = (uint64_t)SRC(ip, i*64+11) >> 37;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*49+ 9) |= (uint64_t)SRC(ip, i*64+12) << 12 | (uint64_t)SRC1(ip, i*64+13) << 61;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*49+10)  = (uint64_t)SRC(ip, i*64+13) >>  3 | (uint64_t)SRC1(ip, i*64+14) << 46;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*49+11)  = (uint64_t)SRC(ip, i*64+14) >> 18 | (uint64_t)SRC1(ip, i*64+15) << 31;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*49+12)  = (uint64_t)SRC(ip, i*64+15) >> 33 | (uint64_t)SRC1(ip, i*64+16) << 16;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*49+13)  = (uint64_t)SRC(ip, i*64+16) >> 48;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*49+13) |= (uint64_t)SRC(ip, i*64+17) <<  1 | (uint64_t)SRC1(ip, i*64+18) << 50;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*49+14)  = (uint64_t)SRC(ip, i*64+18) >> 14 | (uint64_t)SRC1(ip, i*64+19) << 35;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*49+15)  = (uint64_t)SRC(ip, i*64+19) >> 29 | (uint64_t)SRC1(ip, i*64+20) << 20;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*49+16)  = (uint64_t)SRC(ip, i*64+20) >> 44;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*49+16) |= (uint64_t)SRC(ip, i*64+21) <<  5 | (uint64_t)SRC1(ip, i*64+22) << 54;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*49+17)  = (uint64_t)SRC(ip, i*64+22) >> 10 | (uint64_t)SRC1(ip, i*64+23) << 39;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*49+18)  = (uint64_t)SRC(ip, i*64+23) >> 25 | (uint64_t)SRC1(ip, i*64+24) << 24;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*49+19)  = (uint64_t)SRC(ip, i*64+24) >> 40;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*49+19) |= (uint64_t)SRC(ip, i*64+25) <<  9 | (uint64_t)SRC1(ip, i*64+26) << 58;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*49+20)  = (uint64_t)SRC(ip, i*64+26) >>  6 | (uint64_t)SRC1(ip, i*64+27) << 43;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*49+21)  = (uint64_t)SRC(ip, i*64+27) >> 21 | (uint64_t)SRC1(ip, i*64+28) << 28;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*49+22)  = (uint64_t)SRC(ip, i*64+28) >> 36;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*49+22) |= (uint64_t)SRC(ip, i*64+29) << 13 | (uint64_t)SRC1(ip, i*64+30) << 62;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*49+23)  = (uint64_t)SRC(ip, i*64+30) >>  2 | (uint64_t)SRC1(ip, i*64+31) << 47;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*49+24)  = (uint64_t)SRC(ip, i*64+31) >> 17;\
}

#define BITPACK64_49(ip,  op, parm) { \
  BITBLK64_49(ip, 0, op, parm);  SRCI(ip); op += 49*4/sizeof(op[0]);\
}

#define BITBLK64_50(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*25+ 0)  = (uint64_t)SRC(ip, i*32+ 0)       | (uint64_t)SRC1(ip, i*32+1) << 50;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*25+ 1)  = (uint64_t)SRC(ip, i*32+ 1) >> 14 | (uint64_t)SRC1(ip, i*32+2) << 36;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*25+ 2)  = (uint64_t)SRC(ip, i*32+ 2) >> 28 | (uint64_t)SRC1(ip, i*32+3) << 22;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*25+ 3)  = (uint64_t)SRC(ip, i*32+ 3) >> 42;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*25+ 3) |= (uint64_t)SRC(ip, i*32+ 4) <<  8 | (uint64_t)SRC1(ip, i*32+5) << 58;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*25+ 4)  = (uint64_t)SRC(ip, i*32+ 5) >>  6 | (uint64_t)SRC1(ip, i*32+6) << 44;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*25+ 5)  = (uint64_t)SRC(ip, i*32+ 6) >> 20 | (uint64_t)SRC1(ip, i*32+7) << 30;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*25+ 6)  = (uint64_t)SRC(ip, i*32+ 7) >> 34 | (uint64_t)SRC1(ip, i*32+8) << 16;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*25+ 7)  = (uint64_t)SRC(ip, i*32+ 8) >> 48;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*25+ 7) |= (uint64_t)SRC(ip, i*32+ 9) <<  2 | (uint64_t)SRC1(ip, i*32+10) << 52;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*25+ 8)  = (uint64_t)SRC(ip, i*32+10) >> 12 | (uint64_t)SRC1(ip, i*32+11) << 38;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*25+ 9)  = (uint64_t)SRC(ip, i*32+11) >> 26 | (uint64_t)SRC1(ip, i*32+12) << 24;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*25+10)  = (uint64_t)SRC(ip, i*32+12) >> 40;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*25+10) |= (uint64_t)SRC(ip, i*32+13) << 10 | (uint64_t)SRC1(ip, i*32+14) << 60;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*25+11)  = (uint64_t)SRC(ip, i*32+14) >>  4 | (uint64_t)SRC1(ip, i*32+15) << 46;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*25+12)  = (uint64_t)SRC(ip, i*32+15) >> 18 | (uint64_t)SRC1(ip, i*32+16) << 32;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*25+13)  = (uint64_t)SRC(ip, i*32+16) >> 32 | (uint64_t)SRC1(ip, i*32+17) << 18;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*25+14)  = (uint64_t)SRC(ip, i*32+17) >> 46;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*25+14) |= (uint64_t)SRC(ip, i*32+18) <<  4 | (uint64_t)SRC1(ip, i*32+19) << 54;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*25+15)  = (uint64_t)SRC(ip, i*32+19) >> 10 | (uint64_t)SRC1(ip, i*32+20) << 40;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*25+16)  = (uint64_t)SRC(ip, i*32+20) >> 24 | (uint64_t)SRC1(ip, i*32+21) << 26;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*25+17)  = (uint64_t)SRC(ip, i*32+21) >> 38;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*25+17) |= (uint64_t)SRC(ip, i*32+22) << 12 | (uint64_t)SRC1(ip, i*32+23) << 62;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*25+18)  = (uint64_t)SRC(ip, i*32+23) >>  2 | (uint64_t)SRC1(ip, i*32+24) << 48;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*25+19)  = (uint64_t)SRC(ip, i*32+24) >> 16 | (uint64_t)SRC1(ip, i*32+25) << 34;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*25+20)  = (uint64_t)SRC(ip, i*32+25) >> 30 | (uint64_t)SRC1(ip, i*32+26) << 20;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*25+21)  = (uint64_t)SRC(ip, i*32+26) >> 44;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*25+21) |= (uint64_t)SRC(ip, i*32+27) <<  6 | (uint64_t)SRC1(ip, i*32+28) << 56;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*25+22)  = (uint64_t)SRC(ip, i*32+28) >>  8 | (uint64_t)SRC1(ip, i*32+29) << 42;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*25+23)  = (uint64_t)SRC(ip, i*32+29) >> 22 | (uint64_t)SRC1(ip, i*32+30) << 28;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*25+24)  = (uint64_t)SRC(ip, i*32+30) >> 36;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*25+24) |= (uint64_t)SRC(ip, i*32+31) << 14;\
}

#define BITPACK64_50(ip,  op, parm) { \
  BITBLK64_50(ip, 0, op, parm);  SRCI(ip); op += 50*4/sizeof(op[0]);\
}

#define BITBLK64_51(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*51+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 51;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*51+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 13 | (uint64_t)SRC1(ip, i*64+2) << 38;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*51+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 26 | (uint64_t)SRC1(ip, i*64+3) << 25;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*51+ 3)  = (uint64_t)SRC(ip, i*64+ 3) >> 39;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*51+ 3) |= (uint64_t)SRC(ip, i*64+ 4) << 12 | (uint64_t)SRC1(ip, i*64+5) << 63;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*51+ 4)  = (uint64_t)SRC(ip, i*64+ 5) >>  1 | (uint64_t)SRC1(ip, i*64+6) << 50;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*51+ 5)  = (uint64_t)SRC(ip, i*64+ 6) >> 14 | (uint64_t)SRC1(ip, i*64+7) << 37;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*51+ 6)  = (uint64_t)SRC(ip, i*64+ 7) >> 27 | (uint64_t)SRC1(ip, i*64+8) << 24;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*51+ 7)  = (uint64_t)SRC(ip, i*64+ 8) >> 40;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*51+ 7) |= (uint64_t)SRC(ip, i*64+ 9) << 11 | (uint64_t)SRC1(ip, i*64+10) << 62;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*51+ 8)  = (uint64_t)SRC(ip, i*64+10) >>  2 | (uint64_t)SRC1(ip, i*64+11) << 49;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*51+ 9)  = (uint64_t)SRC(ip, i*64+11) >> 15 | (uint64_t)SRC1(ip, i*64+12) << 36;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*51+10)  = (uint64_t)SRC(ip, i*64+12) >> 28 | (uint64_t)SRC1(ip, i*64+13) << 23;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*51+11)  = (uint64_t)SRC(ip, i*64+13) >> 41;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*51+11) |= (uint64_t)SRC(ip, i*64+14) << 10 | (uint64_t)SRC1(ip, i*64+15) << 61;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*51+12)  = (uint64_t)SRC(ip, i*64+15) >>  3 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*51+13)  = (uint64_t)SRC(ip, i*64+16) >> 16 | (uint64_t)SRC1(ip, i*64+17) << 35;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*51+14)  = (uint64_t)SRC(ip, i*64+17) >> 29 | (uint64_t)SRC1(ip, i*64+18) << 22;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*51+15)  = (uint64_t)SRC(ip, i*64+18) >> 42;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*51+15) |= (uint64_t)SRC(ip, i*64+19) <<  9 | (uint64_t)SRC1(ip, i*64+20) << 60;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*51+16)  = (uint64_t)SRC(ip, i*64+20) >>  4 | (uint64_t)SRC1(ip, i*64+21) << 47;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*51+17)  = (uint64_t)SRC(ip, i*64+21) >> 17 | (uint64_t)SRC1(ip, i*64+22) << 34;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*51+18)  = (uint64_t)SRC(ip, i*64+22) >> 30 | (uint64_t)SRC1(ip, i*64+23) << 21;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*51+19)  = (uint64_t)SRC(ip, i*64+23) >> 43;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*51+19) |= (uint64_t)SRC(ip, i*64+24) <<  8 | (uint64_t)SRC1(ip, i*64+25) << 59;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*51+20)  = (uint64_t)SRC(ip, i*64+25) >>  5 | (uint64_t)SRC1(ip, i*64+26) << 46;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*51+21)  = (uint64_t)SRC(ip, i*64+26) >> 18 | (uint64_t)SRC1(ip, i*64+27) << 33;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*51+22)  = (uint64_t)SRC(ip, i*64+27) >> 31 | (uint64_t)SRC1(ip, i*64+28) << 20;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*51+23)  = (uint64_t)SRC(ip, i*64+28) >> 44;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*51+23) |= (uint64_t)SRC(ip, i*64+29) <<  7 | (uint64_t)SRC1(ip, i*64+30) << 58;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*51+24)  = (uint64_t)SRC(ip, i*64+30) >>  6 | (uint64_t)SRC1(ip, i*64+31) << 45;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*51+25)  = (uint64_t)SRC(ip, i*64+31) >> 19;\
}

#define BITPACK64_51(ip,  op, parm) { \
  BITBLK64_51(ip, 0, op, parm);  SRCI(ip); op += 51*4/sizeof(op[0]);\
}

#define BITBLK64_52(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*13+ 0)  = (uint64_t)SRC(ip, i*16+ 0)       | (uint64_t)SRC1(ip, i*16+1) << 52;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*13+ 1)  = (uint64_t)SRC(ip, i*16+ 1) >> 12 | (uint64_t)SRC1(ip, i*16+2) << 40;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*13+ 2)  = (uint64_t)SRC(ip, i*16+ 2) >> 24 | (uint64_t)SRC1(ip, i*16+3) << 28;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*13+ 3)  = (uint64_t)SRC(ip, i*16+ 3) >> 36 | (uint64_t)SRC1(ip, i*16+4) << 16;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*13+ 4)  = (uint64_t)SRC(ip, i*16+ 4) >> 48;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*13+ 4) |= (uint64_t)SRC(ip, i*16+ 5) <<  4 | (uint64_t)SRC1(ip, i*16+6) << 56;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*13+ 5)  = (uint64_t)SRC(ip, i*16+ 6) >>  8 | (uint64_t)SRC1(ip, i*16+7) << 44;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*13+ 6)  = (uint64_t)SRC(ip, i*16+ 7) >> 20 | (uint64_t)SRC1(ip, i*16+8) << 32;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*13+ 7)  = (uint64_t)SRC(ip, i*16+ 8) >> 32 | (uint64_t)SRC1(ip, i*16+9) << 20;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*13+ 8)  = (uint64_t)SRC(ip, i*16+ 9) >> 44;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*13+ 8) |= (uint64_t)SRC(ip, i*16+10) <<  8 | (uint64_t)SRC1(ip, i*16+11) << 60;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*13+ 9)  = (uint64_t)SRC(ip, i*16+11) >>  4 | (uint64_t)SRC1(ip, i*16+12) << 48;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*13+10)  = (uint64_t)SRC(ip, i*16+12) >> 16 | (uint64_t)SRC1(ip, i*16+13) << 36;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*13+11)  = (uint64_t)SRC(ip, i*16+13) >> 28 | (uint64_t)SRC1(ip, i*16+14) << 24;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*13+12)  = (uint64_t)SRC(ip, i*16+14) >> 40;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*13+12) |= (uint64_t)SRC(ip, i*16+15) << 12;\
}

#define BITPACK64_52(ip,  op, parm) { \
  BITBLK64_52(ip, 0, op, parm);\
  BITBLK64_52(ip, 1, op, parm);  SRCI(ip); op += 52*4/sizeof(op[0]);\
}

#define BITBLK64_53(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*53+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 53;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*53+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >> 11 | (uint64_t)SRC1(ip, i*64+2) << 42;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*53+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 22 | (uint64_t)SRC1(ip, i*64+3) << 31;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*53+ 3)  = (uint64_t)SRC(ip, i*64+ 3) >> 33 | (uint64_t)SRC1(ip, i*64+4) << 20;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*53+ 4)  = (uint64_t)SRC(ip, i*64+ 4) >> 44;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*53+ 4) |= (uint64_t)SRC(ip, i*64+ 5) <<  9 | (uint64_t)SRC1(ip, i*64+6) << 62;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*53+ 5)  = (uint64_t)SRC(ip, i*64+ 6) >>  2 | (uint64_t)SRC1(ip, i*64+7) << 51;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*53+ 6)  = (uint64_t)SRC(ip, i*64+ 7) >> 13 | (uint64_t)SRC1(ip, i*64+8) << 40;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*53+ 7)  = (uint64_t)SRC(ip, i*64+ 8) >> 24 | (uint64_t)SRC1(ip, i*64+9) << 29;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*53+ 8)  = (uint64_t)SRC(ip, i*64+ 9) >> 35 | (uint64_t)SRC1(ip, i*64+10) << 18;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*53+ 9)  = (uint64_t)SRC(ip, i*64+10) >> 46;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*53+ 9) |= (uint64_t)SRC(ip, i*64+11) <<  7 | (uint64_t)SRC1(ip, i*64+12) << 60;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*53+10)  = (uint64_t)SRC(ip, i*64+12) >>  4 | (uint64_t)SRC1(ip, i*64+13) << 49;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*53+11)  = (uint64_t)SRC(ip, i*64+13) >> 15 | (uint64_t)SRC1(ip, i*64+14) << 38;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*53+12)  = (uint64_t)SRC(ip, i*64+14) >> 26 | (uint64_t)SRC1(ip, i*64+15) << 27;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*53+13)  = (uint64_t)SRC(ip, i*64+15) >> 37 | (uint64_t)SRC1(ip, i*64+16) << 16;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*53+14)  = (uint64_t)SRC(ip, i*64+16) >> 48;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*53+14) |= (uint64_t)SRC(ip, i*64+17) <<  5 | (uint64_t)SRC1(ip, i*64+18) << 58;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*53+15)  = (uint64_t)SRC(ip, i*64+18) >>  6 | (uint64_t)SRC1(ip, i*64+19) << 47;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*53+16)  = (uint64_t)SRC(ip, i*64+19) >> 17 | (uint64_t)SRC1(ip, i*64+20) << 36;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*53+17)  = (uint64_t)SRC(ip, i*64+20) >> 28 | (uint64_t)SRC1(ip, i*64+21) << 25;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*53+18)  = (uint64_t)SRC(ip, i*64+21) >> 39 | (uint64_t)SRC1(ip, i*64+22) << 14;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*53+19)  = (uint64_t)SRC(ip, i*64+22) >> 50;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*53+19) |= (uint64_t)SRC(ip, i*64+23) <<  3 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*53+20)  = (uint64_t)SRC(ip, i*64+24) >>  8 | (uint64_t)SRC1(ip, i*64+25) << 45;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*53+21)  = (uint64_t)SRC(ip, i*64+25) >> 19 | (uint64_t)SRC1(ip, i*64+26) << 34;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*53+22)  = (uint64_t)SRC(ip, i*64+26) >> 30 | (uint64_t)SRC1(ip, i*64+27) << 23;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*53+23)  = (uint64_t)SRC(ip, i*64+27) >> 41 | (uint64_t)SRC1(ip, i*64+28) << 12;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*53+24)  = (uint64_t)SRC(ip, i*64+28) >> 52;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*53+24) |= (uint64_t)SRC(ip, i*64+29) <<  1 | (uint64_t)SRC1(ip, i*64+30) << 54;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*53+25)  = (uint64_t)SRC(ip, i*64+30) >> 10 | (uint64_t)SRC1(ip, i*64+31) << 43;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*53+26)  = (uint64_t)SRC(ip, i*64+31) >> 21;\
}

#define BITPACK64_53(ip,  op, parm) { \
  BITBLK64_53(ip, 0, op, parm);  SRCI(ip); op += 53*4/sizeof(op[0]);\
}

#define BITBLK64_54(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*27+ 0)  = (uint64_t)SRC(ip, i*32+ 0)       | (uint64_t)SRC1(ip, i*32+1) << 54;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*27+ 1)  = (uint64_t)SRC(ip, i*32+ 1) >> 10 | (uint64_t)SRC1(ip, i*32+2) << 44;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*27+ 2)  = (uint64_t)SRC(ip, i*32+ 2) >> 20 | (uint64_t)SRC1(ip, i*32+3) << 34;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*27+ 3)  = (uint64_t)SRC(ip, i*32+ 3) >> 30 | (uint64_t)SRC1(ip, i*32+4) << 24;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*27+ 4)  = (uint64_t)SRC(ip, i*32+ 4) >> 40 | (uint64_t)SRC1(ip, i*32+5) << 14;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*27+ 5)  = (uint64_t)SRC(ip, i*32+ 5) >> 50;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*27+ 5) |= (uint64_t)SRC(ip, i*32+ 6) <<  4 | (uint64_t)SRC1(ip, i*32+7) << 58;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*27+ 6)  = (uint64_t)SRC(ip, i*32+ 7) >>  6 | (uint64_t)SRC1(ip, i*32+8) << 48;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*27+ 7)  = (uint64_t)SRC(ip, i*32+ 8) >> 16 | (uint64_t)SRC1(ip, i*32+9) << 38;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*27+ 8)  = (uint64_t)SRC(ip, i*32+ 9) >> 26 | (uint64_t)SRC1(ip, i*32+10) << 28;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*27+ 9)  = (uint64_t)SRC(ip, i*32+10) >> 36 | (uint64_t)SRC1(ip, i*32+11) << 18;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*27+10)  = (uint64_t)SRC(ip, i*32+11) >> 46;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*27+10) |= (uint64_t)SRC(ip, i*32+12) <<  8 | (uint64_t)SRC1(ip, i*32+13) << 62;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*27+11)  = (uint64_t)SRC(ip, i*32+13) >>  2 | (uint64_t)SRC1(ip, i*32+14) << 52;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*27+12)  = (uint64_t)SRC(ip, i*32+14) >> 12 | (uint64_t)SRC1(ip, i*32+15) << 42;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*27+13)  = (uint64_t)SRC(ip, i*32+15) >> 22 | (uint64_t)SRC1(ip, i*32+16) << 32;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*27+14)  = (uint64_t)SRC(ip, i*32+16) >> 32 | (uint64_t)SRC1(ip, i*32+17) << 22;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*27+15)  = (uint64_t)SRC(ip, i*32+17) >> 42 | (uint64_t)SRC1(ip, i*32+18) << 12;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*27+16)  = (uint64_t)SRC(ip, i*32+18) >> 52;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*27+16) |= (uint64_t)SRC(ip, i*32+19) <<  2 | (uint64_t)SRC1(ip, i*32+20) << 56;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*27+17)  = (uint64_t)SRC(ip, i*32+20) >>  8 | (uint64_t)SRC1(ip, i*32+21) << 46;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*27+18)  = (uint64_t)SRC(ip, i*32+21) >> 18 | (uint64_t)SRC1(ip, i*32+22) << 36;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*27+19)  = (uint64_t)SRC(ip, i*32+22) >> 28 | (uint64_t)SRC1(ip, i*32+23) << 26;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*27+20)  = (uint64_t)SRC(ip, i*32+23) >> 38 | (uint64_t)SRC1(ip, i*32+24) << 16;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*27+21)  = (uint64_t)SRC(ip, i*32+24) >> 48;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*27+21) |= (uint64_t)SRC(ip, i*32+25) <<  6 | (uint64_t)SRC1(ip, i*32+26) << 60;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*27+22)  = (uint64_t)SRC(ip, i*32+26) >>  4 | (uint64_t)SRC1(ip, i*32+27) << 50;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*27+23)  = (uint64_t)SRC(ip, i*32+27) >> 14 | (uint64_t)SRC1(ip, i*32+28) << 40;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*27+24)  = (uint64_t)SRC(ip, i*32+28) >> 24 | (uint64_t)SRC1(ip, i*32+29) << 30;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*27+25)  = (uint64_t)SRC(ip, i*32+29) >> 34 | (uint64_t)SRC1(ip, i*32+30) << 20;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*27+26)  = (uint64_t)SRC(ip, i*32+30) >> 44;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*27+26) |= (uint64_t)SRC(ip, i*32+31) << 10;\
}

#define BITPACK64_54(ip,  op, parm) { \
  BITBLK64_54(ip, 0, op, parm);  SRCI(ip); op += 54*4/sizeof(op[0]);\
}

#define BITBLK64_55(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*55+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 55;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*55+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >>  9 | (uint64_t)SRC1(ip, i*64+2) << 46;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*55+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 18 | (uint64_t)SRC1(ip, i*64+3) << 37;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*55+ 3)  = (uint64_t)SRC(ip, i*64+ 3) >> 27 | (uint64_t)SRC1(ip, i*64+4) << 28;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*55+ 4)  = (uint64_t)SRC(ip, i*64+ 4) >> 36 | (uint64_t)SRC1(ip, i*64+5) << 19;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*55+ 5)  = (uint64_t)SRC(ip, i*64+ 5) >> 45 | (uint64_t)SRC1(ip, i*64+6) << 10;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*55+ 6)  = (uint64_t)SRC(ip, i*64+ 6) >> 54;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*55+ 6) |= (uint64_t)SRC(ip, i*64+ 7) <<  1 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*55+ 7)  = (uint64_t)SRC(ip, i*64+ 8) >>  8 | (uint64_t)SRC1(ip, i*64+9) << 47;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*55+ 8)  = (uint64_t)SRC(ip, i*64+ 9) >> 17 | (uint64_t)SRC1(ip, i*64+10) << 38;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*55+ 9)  = (uint64_t)SRC(ip, i*64+10) >> 26 | (uint64_t)SRC1(ip, i*64+11) << 29;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*55+10)  = (uint64_t)SRC(ip, i*64+11) >> 35 | (uint64_t)SRC1(ip, i*64+12) << 20;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*55+11)  = (uint64_t)SRC(ip, i*64+12) >> 44 | (uint64_t)SRC1(ip, i*64+13) << 11;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*55+12)  = (uint64_t)SRC(ip, i*64+13) >> 53;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*55+12) |= (uint64_t)SRC(ip, i*64+14) <<  2 | (uint64_t)SRC1(ip, i*64+15) << 57;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*55+13)  = (uint64_t)SRC(ip, i*64+15) >>  7 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*55+14)  = (uint64_t)SRC(ip, i*64+16) >> 16 | (uint64_t)SRC1(ip, i*64+17) << 39;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*55+15)  = (uint64_t)SRC(ip, i*64+17) >> 25 | (uint64_t)SRC1(ip, i*64+18) << 30;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*55+16)  = (uint64_t)SRC(ip, i*64+18) >> 34 | (uint64_t)SRC1(ip, i*64+19) << 21;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*55+17)  = (uint64_t)SRC(ip, i*64+19) >> 43 | (uint64_t)SRC1(ip, i*64+20) << 12;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*55+18)  = (uint64_t)SRC(ip, i*64+20) >> 52;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*55+18) |= (uint64_t)SRC(ip, i*64+21) <<  3 | (uint64_t)SRC1(ip, i*64+22) << 58;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*55+19)  = (uint64_t)SRC(ip, i*64+22) >>  6 | (uint64_t)SRC1(ip, i*64+23) << 49;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*55+20)  = (uint64_t)SRC(ip, i*64+23) >> 15 | (uint64_t)SRC1(ip, i*64+24) << 40;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*55+21)  = (uint64_t)SRC(ip, i*64+24) >> 24 | (uint64_t)SRC1(ip, i*64+25) << 31;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*55+22)  = (uint64_t)SRC(ip, i*64+25) >> 33 | (uint64_t)SRC1(ip, i*64+26) << 22;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*55+23)  = (uint64_t)SRC(ip, i*64+26) >> 42 | (uint64_t)SRC1(ip, i*64+27) << 13;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*55+24)  = (uint64_t)SRC(ip, i*64+27) >> 51;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*55+24) |= (uint64_t)SRC(ip, i*64+28) <<  4 | (uint64_t)SRC1(ip, i*64+29) << 59;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*55+25)  = (uint64_t)SRC(ip, i*64+29) >>  5 | (uint64_t)SRC1(ip, i*64+30) << 50;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*55+26)  = (uint64_t)SRC(ip, i*64+30) >> 14 | (uint64_t)SRC1(ip, i*64+31) << 41;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*55+27)  = (uint64_t)SRC(ip, i*64+31) >> 23;\
}

#define BITPACK64_55(ip,  op, parm) { \
  BITBLK64_55(ip, 0, op, parm);  SRCI(ip); op += 55*4/sizeof(op[0]);\
}

#define BITBLK64_56(ip, i, op, parm) { ;\
  IPPB(ip, i*8+ 0, parm); *((uint64_t *)op+i*7+ 0)  = (uint64_t)SRC(ip, i*8+ 0)       | (uint64_t)SRC1(ip, i*8+1) << 56;\
  IPPB(ip, i*8+ 1, parm); *((uint64_t *)op+i*7+ 1)  = (uint64_t)SRC(ip, i*8+ 1) >>  8 | (uint64_t)SRC1(ip, i*8+2) << 48;\
  IPPB(ip, i*8+ 2, parm); *((uint64_t *)op+i*7+ 2)  = (uint64_t)SRC(ip, i*8+ 2) >> 16 | (uint64_t)SRC1(ip, i*8+3) << 40;\
  IPPB(ip, i*8+ 3, parm); *((uint64_t *)op+i*7+ 3)  = (uint64_t)SRC(ip, i*8+ 3) >> 24 | (uint64_t)SRC1(ip, i*8+4) << 32;\
  IPPB(ip, i*8+ 4, parm); *((uint64_t *)op+i*7+ 4)  = (uint64_t)SRC(ip, i*8+ 4) >> 32 | (uint64_t)SRC1(ip, i*8+5) << 24;\
  IPPB(ip, i*8+ 5, parm); *((uint64_t *)op+i*7+ 5)  = (uint64_t)SRC(ip, i*8+ 5) >> 40 | (uint64_t)SRC1(ip, i*8+6) << 16;\
  IPPB(ip, i*8+ 6, parm); *((uint64_t *)op+i*7+ 6)  = (uint64_t)SRC(ip, i*8+ 6) >> 48;\
  IPPB(ip, i*8+ 7, parm); *((uint64_t *)op+i*7+ 6) |= (uint64_t)SRC(ip, i*8+ 7) <<  8;\
}

#define BITPACK64_56(ip,  op, parm) { \
  BITBLK64_56(ip, 0, op, parm);\
  BITBLK64_56(ip, 1, op, parm);\
  BITBLK64_56(ip, 2, op, parm);\
  BITBLK64_56(ip, 3, op, parm);  SRCI(ip); op += 56*4/sizeof(op[0]);\
}

#define BITBLK64_57(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*57+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 57;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*57+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >>  7 | (uint64_t)SRC1(ip, i*64+2) << 50;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*57+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 14 | (uint64_t)SRC1(ip, i*64+3) << 43;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*57+ 3)  = (uint64_t)SRC(ip, i*64+ 3) >> 21 | (uint64_t)SRC1(ip, i*64+4) << 36;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*57+ 4)  = (uint64_t)SRC(ip, i*64+ 4) >> 28 | (uint64_t)SRC1(ip, i*64+5) << 29;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*57+ 5)  = (uint64_t)SRC(ip, i*64+ 5) >> 35 | (uint64_t)SRC1(ip, i*64+6) << 22;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*57+ 6)  = (uint64_t)SRC(ip, i*64+ 6) >> 42 | (uint64_t)SRC1(ip, i*64+7) << 15;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*57+ 7)  = (uint64_t)SRC(ip, i*64+ 7) >> 49 | (uint64_t)SRC1(ip, i*64+8) <<  8;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*57+ 8)  = (uint64_t)SRC(ip, i*64+ 8) >> 56;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*57+ 8) |= (uint64_t)SRC(ip, i*64+ 9) <<  1 | (uint64_t)SRC1(ip, i*64+10) << 58;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*57+ 9)  = (uint64_t)SRC(ip, i*64+10) >>  6 | (uint64_t)SRC1(ip, i*64+11) << 51;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*57+10)  = (uint64_t)SRC(ip, i*64+11) >> 13 | (uint64_t)SRC1(ip, i*64+12) << 44;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*57+11)  = (uint64_t)SRC(ip, i*64+12) >> 20 | (uint64_t)SRC1(ip, i*64+13) << 37;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*57+12)  = (uint64_t)SRC(ip, i*64+13) >> 27 | (uint64_t)SRC1(ip, i*64+14) << 30;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*57+13)  = (uint64_t)SRC(ip, i*64+14) >> 34 | (uint64_t)SRC1(ip, i*64+15) << 23;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*57+14)  = (uint64_t)SRC(ip, i*64+15) >> 41 | (uint64_t)SRC1(ip, i*64+16) << 16;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*57+15)  = (uint64_t)SRC(ip, i*64+16) >> 48 | (uint64_t)SRC1(ip, i*64+17) <<  9;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*57+16)  = (uint64_t)SRC(ip, i*64+17) >> 55;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*57+16) |= (uint64_t)SRC(ip, i*64+18) <<  2 | (uint64_t)SRC1(ip, i*64+19) << 59;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*57+17)  = (uint64_t)SRC(ip, i*64+19) >>  5 | (uint64_t)SRC1(ip, i*64+20) << 52;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*57+18)  = (uint64_t)SRC(ip, i*64+20) >> 12 | (uint64_t)SRC1(ip, i*64+21) << 45;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*57+19)  = (uint64_t)SRC(ip, i*64+21) >> 19 | (uint64_t)SRC1(ip, i*64+22) << 38;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*57+20)  = (uint64_t)SRC(ip, i*64+22) >> 26 | (uint64_t)SRC1(ip, i*64+23) << 31;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*57+21)  = (uint64_t)SRC(ip, i*64+23) >> 33 | (uint64_t)SRC1(ip, i*64+24) << 24;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*57+22)  = (uint64_t)SRC(ip, i*64+24) >> 40 | (uint64_t)SRC1(ip, i*64+25) << 17;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*57+23)  = (uint64_t)SRC(ip, i*64+25) >> 47 | (uint64_t)SRC1(ip, i*64+26) << 10;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*57+24)  = (uint64_t)SRC(ip, i*64+26) >> 54;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*57+24) |= (uint64_t)SRC(ip, i*64+27) <<  3 | (uint64_t)SRC1(ip, i*64+28) << 60;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*57+25)  = (uint64_t)SRC(ip, i*64+28) >>  4 | (uint64_t)SRC1(ip, i*64+29) << 53;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*57+26)  = (uint64_t)SRC(ip, i*64+29) >> 11 | (uint64_t)SRC1(ip, i*64+30) << 46;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*57+27)  = (uint64_t)SRC(ip, i*64+30) >> 18 | (uint64_t)SRC1(ip, i*64+31) << 39;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*57+28)  = (uint64_t)SRC(ip, i*64+31) >> 25;\
}

#define BITPACK64_57(ip,  op, parm) { \
  BITBLK64_57(ip, 0, op, parm);  SRCI(ip); op += 57*4/sizeof(op[0]);\
}

#define BITBLK64_58(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*29+ 0)  = (uint64_t)SRC(ip, i*32+ 0)       | (uint64_t)SRC1(ip, i*32+1) << 58;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*29+ 1)  = (uint64_t)SRC(ip, i*32+ 1) >>  6 | (uint64_t)SRC1(ip, i*32+2) << 52;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*29+ 2)  = (uint64_t)SRC(ip, i*32+ 2) >> 12 | (uint64_t)SRC1(ip, i*32+3) << 46;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*29+ 3)  = (uint64_t)SRC(ip, i*32+ 3) >> 18 | (uint64_t)SRC1(ip, i*32+4) << 40;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*29+ 4)  = (uint64_t)SRC(ip, i*32+ 4) >> 24 | (uint64_t)SRC1(ip, i*32+5) << 34;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*29+ 5)  = (uint64_t)SRC(ip, i*32+ 5) >> 30 | (uint64_t)SRC1(ip, i*32+6) << 28;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*29+ 6)  = (uint64_t)SRC(ip, i*32+ 6) >> 36 | (uint64_t)SRC1(ip, i*32+7) << 22;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*29+ 7)  = (uint64_t)SRC(ip, i*32+ 7) >> 42 | (uint64_t)SRC1(ip, i*32+8) << 16;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*29+ 8)  = (uint64_t)SRC(ip, i*32+ 8) >> 48 | (uint64_t)SRC1(ip, i*32+9) << 10;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*29+ 9)  = (uint64_t)SRC(ip, i*32+ 9) >> 54;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*29+ 9) |= (uint64_t)SRC(ip, i*32+10) <<  4 | (uint64_t)SRC1(ip, i*32+11) << 62;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*29+10)  = (uint64_t)SRC(ip, i*32+11) >>  2 | (uint64_t)SRC1(ip, i*32+12) << 56;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*29+11)  = (uint64_t)SRC(ip, i*32+12) >>  8 | (uint64_t)SRC1(ip, i*32+13) << 50;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*29+12)  = (uint64_t)SRC(ip, i*32+13) >> 14 | (uint64_t)SRC1(ip, i*32+14) << 44;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*29+13)  = (uint64_t)SRC(ip, i*32+14) >> 20 | (uint64_t)SRC1(ip, i*32+15) << 38;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*29+14)  = (uint64_t)SRC(ip, i*32+15) >> 26 | (uint64_t)SRC1(ip, i*32+16) << 32;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*29+15)  = (uint64_t)SRC(ip, i*32+16) >> 32 | (uint64_t)SRC1(ip, i*32+17) << 26;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*29+16)  = (uint64_t)SRC(ip, i*32+17) >> 38 | (uint64_t)SRC1(ip, i*32+18) << 20;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*29+17)  = (uint64_t)SRC(ip, i*32+18) >> 44 | (uint64_t)SRC1(ip, i*32+19) << 14;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*29+18)  = (uint64_t)SRC(ip, i*32+19) >> 50 | (uint64_t)SRC1(ip, i*32+20) <<  8;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*29+19)  = (uint64_t)SRC(ip, i*32+20) >> 56;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*29+19) |= (uint64_t)SRC(ip, i*32+21) <<  2 | (uint64_t)SRC1(ip, i*32+22) << 60;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*29+20)  = (uint64_t)SRC(ip, i*32+22) >>  4 | (uint64_t)SRC1(ip, i*32+23) << 54;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*29+21)  = (uint64_t)SRC(ip, i*32+23) >> 10 | (uint64_t)SRC1(ip, i*32+24) << 48;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*29+22)  = (uint64_t)SRC(ip, i*32+24) >> 16 | (uint64_t)SRC1(ip, i*32+25) << 42;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*29+23)  = (uint64_t)SRC(ip, i*32+25) >> 22 | (uint64_t)SRC1(ip, i*32+26) << 36;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*29+24)  = (uint64_t)SRC(ip, i*32+26) >> 28 | (uint64_t)SRC1(ip, i*32+27) << 30;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*29+25)  = (uint64_t)SRC(ip, i*32+27) >> 34 | (uint64_t)SRC1(ip, i*32+28) << 24;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*29+26)  = (uint64_t)SRC(ip, i*32+28) >> 40 | (uint64_t)SRC1(ip, i*32+29) << 18;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*29+27)  = (uint64_t)SRC(ip, i*32+29) >> 46 | (uint64_t)SRC1(ip, i*32+30) << 12;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*29+28)  = (uint64_t)SRC(ip, i*32+30) >> 52;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*29+28) |= (uint64_t)SRC(ip, i*32+31) <<  6;\
}

#define BITPACK64_58(ip,  op, parm) { \
  BITBLK64_58(ip, 0, op, parm);  SRCI(ip); op += 58*4/sizeof(op[0]);\
}

#define BITBLK64_59(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*59+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 59;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*59+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >>  5 | (uint64_t)SRC1(ip, i*64+2) << 54;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*59+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >> 10 | (uint64_t)SRC1(ip, i*64+3) << 49;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*59+ 3)  = (uint64_t)SRC(ip, i*64+ 3) >> 15 | (uint64_t)SRC1(ip, i*64+4) << 44;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*59+ 4)  = (uint64_t)SRC(ip, i*64+ 4) >> 20 | (uint64_t)SRC1(ip, i*64+5) << 39;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*59+ 5)  = (uint64_t)SRC(ip, i*64+ 5) >> 25 | (uint64_t)SRC1(ip, i*64+6) << 34;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*59+ 6)  = (uint64_t)SRC(ip, i*64+ 6) >> 30 | (uint64_t)SRC1(ip, i*64+7) << 29;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*59+ 7)  = (uint64_t)SRC(ip, i*64+ 7) >> 35 | (uint64_t)SRC1(ip, i*64+8) << 24;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*59+ 8)  = (uint64_t)SRC(ip, i*64+ 8) >> 40 | (uint64_t)SRC1(ip, i*64+9) << 19;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*59+ 9)  = (uint64_t)SRC(ip, i*64+ 9) >> 45 | (uint64_t)SRC1(ip, i*64+10) << 14;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*59+10)  = (uint64_t)SRC(ip, i*64+10) >> 50 | (uint64_t)SRC1(ip, i*64+11) <<  9;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*59+11)  = (uint64_t)SRC(ip, i*64+11) >> 55;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*59+11) |= (uint64_t)SRC(ip, i*64+12) <<  4 | (uint64_t)SRC1(ip, i*64+13) << 63;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*59+12)  = (uint64_t)SRC(ip, i*64+13) >>  1 | (uint64_t)SRC1(ip, i*64+14) << 58;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*59+13)  = (uint64_t)SRC(ip, i*64+14) >>  6 | (uint64_t)SRC1(ip, i*64+15) << 53;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*59+14)  = (uint64_t)SRC(ip, i*64+15) >> 11 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*59+15)  = (uint64_t)SRC(ip, i*64+16) >> 16 | (uint64_t)SRC1(ip, i*64+17) << 43;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*59+16)  = (uint64_t)SRC(ip, i*64+17) >> 21 | (uint64_t)SRC1(ip, i*64+18) << 38;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*59+17)  = (uint64_t)SRC(ip, i*64+18) >> 26 | (uint64_t)SRC1(ip, i*64+19) << 33;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*59+18)  = (uint64_t)SRC(ip, i*64+19) >> 31 | (uint64_t)SRC1(ip, i*64+20) << 28;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*59+19)  = (uint64_t)SRC(ip, i*64+20) >> 36 | (uint64_t)SRC1(ip, i*64+21) << 23;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*59+20)  = (uint64_t)SRC(ip, i*64+21) >> 41 | (uint64_t)SRC1(ip, i*64+22) << 18;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*59+21)  = (uint64_t)SRC(ip, i*64+22) >> 46 | (uint64_t)SRC1(ip, i*64+23) << 13;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*59+22)  = (uint64_t)SRC(ip, i*64+23) >> 51 | (uint64_t)SRC1(ip, i*64+24) <<  8;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*59+23)  = (uint64_t)SRC(ip, i*64+24) >> 56;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*59+23) |= (uint64_t)SRC(ip, i*64+25) <<  3 | (uint64_t)SRC1(ip, i*64+26) << 62;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*59+24)  = (uint64_t)SRC(ip, i*64+26) >>  2 | (uint64_t)SRC1(ip, i*64+27) << 57;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*59+25)  = (uint64_t)SRC(ip, i*64+27) >>  7 | (uint64_t)SRC1(ip, i*64+28) << 52;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*59+26)  = (uint64_t)SRC(ip, i*64+28) >> 12 | (uint64_t)SRC1(ip, i*64+29) << 47;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*59+27)  = (uint64_t)SRC(ip, i*64+29) >> 17 | (uint64_t)SRC1(ip, i*64+30) << 42;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*59+28)  = (uint64_t)SRC(ip, i*64+30) >> 22 | (uint64_t)SRC1(ip, i*64+31) << 37;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*59+29)  = (uint64_t)SRC(ip, i*64+31) >> 27;\
}

#define BITPACK64_59(ip,  op, parm) { \
  BITBLK64_59(ip, 0, op, parm);  SRCI(ip); op += 59*4/sizeof(op[0]);\
}

#define BITBLK64_60(ip, i, op, parm) { ;\
  IPPB(ip, i*16+ 0, parm); *((uint64_t *)op+i*15+ 0)  = (uint64_t)SRC(ip, i*16+ 0)       | (uint64_t)SRC1(ip, i*16+1) << 60;\
  IPPB(ip, i*16+ 1, parm); *((uint64_t *)op+i*15+ 1)  = (uint64_t)SRC(ip, i*16+ 1) >>  4 | (uint64_t)SRC1(ip, i*16+2) << 56;\
  IPPB(ip, i*16+ 2, parm); *((uint64_t *)op+i*15+ 2)  = (uint64_t)SRC(ip, i*16+ 2) >>  8 | (uint64_t)SRC1(ip, i*16+3) << 52;\
  IPPB(ip, i*16+ 3, parm); *((uint64_t *)op+i*15+ 3)  = (uint64_t)SRC(ip, i*16+ 3) >> 12 | (uint64_t)SRC1(ip, i*16+4) << 48;\
  IPPB(ip, i*16+ 4, parm); *((uint64_t *)op+i*15+ 4)  = (uint64_t)SRC(ip, i*16+ 4) >> 16 | (uint64_t)SRC1(ip, i*16+5) << 44;\
  IPPB(ip, i*16+ 5, parm); *((uint64_t *)op+i*15+ 5)  = (uint64_t)SRC(ip, i*16+ 5) >> 20 | (uint64_t)SRC1(ip, i*16+6) << 40;\
  IPPB(ip, i*16+ 6, parm); *((uint64_t *)op+i*15+ 6)  = (uint64_t)SRC(ip, i*16+ 6) >> 24 | (uint64_t)SRC1(ip, i*16+7) << 36;\
  IPPB(ip, i*16+ 7, parm); *((uint64_t *)op+i*15+ 7)  = (uint64_t)SRC(ip, i*16+ 7) >> 28 | (uint64_t)SRC1(ip, i*16+8) << 32;\
  IPPB(ip, i*16+ 8, parm); *((uint64_t *)op+i*15+ 8)  = (uint64_t)SRC(ip, i*16+ 8) >> 32 | (uint64_t)SRC1(ip, i*16+9) << 28;\
  IPPB(ip, i*16+ 9, parm); *((uint64_t *)op+i*15+ 9)  = (uint64_t)SRC(ip, i*16+ 9) >> 36 | (uint64_t)SRC1(ip, i*16+10) << 24;\
  IPPB(ip, i*16+10, parm); *((uint64_t *)op+i*15+10)  = (uint64_t)SRC(ip, i*16+10) >> 40 | (uint64_t)SRC1(ip, i*16+11) << 20;\
  IPPB(ip, i*16+11, parm); *((uint64_t *)op+i*15+11)  = (uint64_t)SRC(ip, i*16+11) >> 44 | (uint64_t)SRC1(ip, i*16+12) << 16;\
  IPPB(ip, i*16+12, parm); *((uint64_t *)op+i*15+12)  = (uint64_t)SRC(ip, i*16+12) >> 48 | (uint64_t)SRC1(ip, i*16+13) << 12;\
  IPPB(ip, i*16+13, parm); *((uint64_t *)op+i*15+13)  = (uint64_t)SRC(ip, i*16+13) >> 52 | (uint64_t)SRC1(ip, i*16+14) <<  8;\
  IPPB(ip, i*16+14, parm); *((uint64_t *)op+i*15+14)  = (uint64_t)SRC(ip, i*16+14) >> 56;\
  IPPB(ip, i*16+15, parm); *((uint64_t *)op+i*15+14) |= (uint64_t)SRC(ip, i*16+15) <<  4;\
}

#define BITPACK64_60(ip,  op, parm) { \
  BITBLK64_60(ip, 0, op, parm);\
  BITBLK64_60(ip, 1, op, parm);  SRCI(ip); op += 60*4/sizeof(op[0]);\
}

#define BITBLK64_61(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*61+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 61;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*61+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >>  3 | (uint64_t)SRC1(ip, i*64+2) << 58;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*61+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >>  6 | (uint64_t)SRC1(ip, i*64+3) << 55;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*61+ 3)  = (uint64_t)SRC(ip, i*64+ 3) >>  9 | (uint64_t)SRC1(ip, i*64+4) << 52;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*61+ 4)  = (uint64_t)SRC(ip, i*64+ 4) >> 12 | (uint64_t)SRC1(ip, i*64+5) << 49;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*61+ 5)  = (uint64_t)SRC(ip, i*64+ 5) >> 15 | (uint64_t)SRC1(ip, i*64+6) << 46;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*61+ 6)  = (uint64_t)SRC(ip, i*64+ 6) >> 18 | (uint64_t)SRC1(ip, i*64+7) << 43;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*61+ 7)  = (uint64_t)SRC(ip, i*64+ 7) >> 21 | (uint64_t)SRC1(ip, i*64+8) << 40;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*61+ 8)  = (uint64_t)SRC(ip, i*64+ 8) >> 24 | (uint64_t)SRC1(ip, i*64+9) << 37;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*61+ 9)  = (uint64_t)SRC(ip, i*64+ 9) >> 27 | (uint64_t)SRC1(ip, i*64+10) << 34;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*61+10)  = (uint64_t)SRC(ip, i*64+10) >> 30 | (uint64_t)SRC1(ip, i*64+11) << 31;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*61+11)  = (uint64_t)SRC(ip, i*64+11) >> 33 | (uint64_t)SRC1(ip, i*64+12) << 28;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*61+12)  = (uint64_t)SRC(ip, i*64+12) >> 36 | (uint64_t)SRC1(ip, i*64+13) << 25;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*61+13)  = (uint64_t)SRC(ip, i*64+13) >> 39 | (uint64_t)SRC1(ip, i*64+14) << 22;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*61+14)  = (uint64_t)SRC(ip, i*64+14) >> 42 | (uint64_t)SRC1(ip, i*64+15) << 19;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*61+15)  = (uint64_t)SRC(ip, i*64+15) >> 45 | (uint64_t)SRC1(ip, i*64+16) << 16;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*61+16)  = (uint64_t)SRC(ip, i*64+16) >> 48 | (uint64_t)SRC1(ip, i*64+17) << 13;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*61+17)  = (uint64_t)SRC(ip, i*64+17) >> 51 | (uint64_t)SRC1(ip, i*64+18) << 10;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*61+18)  = (uint64_t)SRC(ip, i*64+18) >> 54 | (uint64_t)SRC1(ip, i*64+19) <<  7;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*61+19)  = (uint64_t)SRC(ip, i*64+19) >> 57 | (uint64_t)SRC1(ip, i*64+20) <<  4;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*61+20)  = (uint64_t)SRC(ip, i*64+20) >> 60;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*61+20) |= (uint64_t)SRC(ip, i*64+21) <<  1 | (uint64_t)SRC1(ip, i*64+22) << 62;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*61+21)  = (uint64_t)SRC(ip, i*64+22) >>  2 | (uint64_t)SRC1(ip, i*64+23) << 59;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*61+22)  = (uint64_t)SRC(ip, i*64+23) >>  5 | (uint64_t)SRC1(ip, i*64+24) << 56;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*61+23)  = (uint64_t)SRC(ip, i*64+24) >>  8 | (uint64_t)SRC1(ip, i*64+25) << 53;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*61+24)  = (uint64_t)SRC(ip, i*64+25) >> 11 | (uint64_t)SRC1(ip, i*64+26) << 50;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*61+25)  = (uint64_t)SRC(ip, i*64+26) >> 14 | (uint64_t)SRC1(ip, i*64+27) << 47;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*61+26)  = (uint64_t)SRC(ip, i*64+27) >> 17 | (uint64_t)SRC1(ip, i*64+28) << 44;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*61+27)  = (uint64_t)SRC(ip, i*64+28) >> 20 | (uint64_t)SRC1(ip, i*64+29) << 41;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*61+28)  = (uint64_t)SRC(ip, i*64+29) >> 23 | (uint64_t)SRC1(ip, i*64+30) << 38;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*61+29)  = (uint64_t)SRC(ip, i*64+30) >> 26 | (uint64_t)SRC1(ip, i*64+31) << 35;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*61+30)  = (uint64_t)SRC(ip, i*64+31) >> 29;\
}

#define BITPACK64_61(ip,  op, parm) { \
  BITBLK64_61(ip, 0, op, parm);  SRCI(ip); op += 61*4/sizeof(op[0]);\
}

#define BITBLK64_62(ip, i, op, parm) { ;\
  IPPB(ip, i*32+ 0, parm); *((uint64_t *)op+i*31+ 0)  = (uint64_t)SRC(ip, i*32+ 0)       | (uint64_t)SRC1(ip, i*32+1) << 62;\
  IPPB(ip, i*32+ 1, parm); *((uint64_t *)op+i*31+ 1)  = (uint64_t)SRC(ip, i*32+ 1) >>  2 | (uint64_t)SRC1(ip, i*32+2) << 60;\
  IPPB(ip, i*32+ 2, parm); *((uint64_t *)op+i*31+ 2)  = (uint64_t)SRC(ip, i*32+ 2) >>  4 | (uint64_t)SRC1(ip, i*32+3) << 58;\
  IPPB(ip, i*32+ 3, parm); *((uint64_t *)op+i*31+ 3)  = (uint64_t)SRC(ip, i*32+ 3) >>  6 | (uint64_t)SRC1(ip, i*32+4) << 56;\
  IPPB(ip, i*32+ 4, parm); *((uint64_t *)op+i*31+ 4)  = (uint64_t)SRC(ip, i*32+ 4) >>  8 | (uint64_t)SRC1(ip, i*32+5) << 54;\
  IPPB(ip, i*32+ 5, parm); *((uint64_t *)op+i*31+ 5)  = (uint64_t)SRC(ip, i*32+ 5) >> 10 | (uint64_t)SRC1(ip, i*32+6) << 52;\
  IPPB(ip, i*32+ 6, parm); *((uint64_t *)op+i*31+ 6)  = (uint64_t)SRC(ip, i*32+ 6) >> 12 | (uint64_t)SRC1(ip, i*32+7) << 50;\
  IPPB(ip, i*32+ 7, parm); *((uint64_t *)op+i*31+ 7)  = (uint64_t)SRC(ip, i*32+ 7) >> 14 | (uint64_t)SRC1(ip, i*32+8) << 48;\
  IPPB(ip, i*32+ 8, parm); *((uint64_t *)op+i*31+ 8)  = (uint64_t)SRC(ip, i*32+ 8) >> 16 | (uint64_t)SRC1(ip, i*32+9) << 46;\
  IPPB(ip, i*32+ 9, parm); *((uint64_t *)op+i*31+ 9)  = (uint64_t)SRC(ip, i*32+ 9) >> 18 | (uint64_t)SRC1(ip, i*32+10) << 44;\
  IPPB(ip, i*32+10, parm); *((uint64_t *)op+i*31+10)  = (uint64_t)SRC(ip, i*32+10) >> 20 | (uint64_t)SRC1(ip, i*32+11) << 42;\
  IPPB(ip, i*32+11, parm); *((uint64_t *)op+i*31+11)  = (uint64_t)SRC(ip, i*32+11) >> 22 | (uint64_t)SRC1(ip, i*32+12) << 40;\
  IPPB(ip, i*32+12, parm); *((uint64_t *)op+i*31+12)  = (uint64_t)SRC(ip, i*32+12) >> 24 | (uint64_t)SRC1(ip, i*32+13) << 38;\
  IPPB(ip, i*32+13, parm); *((uint64_t *)op+i*31+13)  = (uint64_t)SRC(ip, i*32+13) >> 26 | (uint64_t)SRC1(ip, i*32+14) << 36;\
  IPPB(ip, i*32+14, parm); *((uint64_t *)op+i*31+14)  = (uint64_t)SRC(ip, i*32+14) >> 28 | (uint64_t)SRC1(ip, i*32+15) << 34;\
  IPPB(ip, i*32+15, parm); *((uint64_t *)op+i*31+15)  = (uint64_t)SRC(ip, i*32+15) >> 30 | (uint64_t)SRC1(ip, i*32+16) << 32;\
  IPPB(ip, i*32+16, parm); *((uint64_t *)op+i*31+16)  = (uint64_t)SRC(ip, i*32+16) >> 32 | (uint64_t)SRC1(ip, i*32+17) << 30;\
  IPPB(ip, i*32+17, parm); *((uint64_t *)op+i*31+17)  = (uint64_t)SRC(ip, i*32+17) >> 34 | (uint64_t)SRC1(ip, i*32+18) << 28;\
  IPPB(ip, i*32+18, parm); *((uint64_t *)op+i*31+18)  = (uint64_t)SRC(ip, i*32+18) >> 36 | (uint64_t)SRC1(ip, i*32+19) << 26;\
  IPPB(ip, i*32+19, parm); *((uint64_t *)op+i*31+19)  = (uint64_t)SRC(ip, i*32+19) >> 38 | (uint64_t)SRC1(ip, i*32+20) << 24;\
  IPPB(ip, i*32+20, parm); *((uint64_t *)op+i*31+20)  = (uint64_t)SRC(ip, i*32+20) >> 40 | (uint64_t)SRC1(ip, i*32+21) << 22;\
  IPPB(ip, i*32+21, parm); *((uint64_t *)op+i*31+21)  = (uint64_t)SRC(ip, i*32+21) >> 42 | (uint64_t)SRC1(ip, i*32+22) << 20;\
  IPPB(ip, i*32+22, parm); *((uint64_t *)op+i*31+22)  = (uint64_t)SRC(ip, i*32+22) >> 44 | (uint64_t)SRC1(ip, i*32+23) << 18;\
  IPPB(ip, i*32+23, parm); *((uint64_t *)op+i*31+23)  = (uint64_t)SRC(ip, i*32+23) >> 46 | (uint64_t)SRC1(ip, i*32+24) << 16;\
  IPPB(ip, i*32+24, parm); *((uint64_t *)op+i*31+24)  = (uint64_t)SRC(ip, i*32+24) >> 48 | (uint64_t)SRC1(ip, i*32+25) << 14;\
  IPPB(ip, i*32+25, parm); *((uint64_t *)op+i*31+25)  = (uint64_t)SRC(ip, i*32+25) >> 50 | (uint64_t)SRC1(ip, i*32+26) << 12;\
  IPPB(ip, i*32+26, parm); *((uint64_t *)op+i*31+26)  = (uint64_t)SRC(ip, i*32+26) >> 52 | (uint64_t)SRC1(ip, i*32+27) << 10;\
  IPPB(ip, i*32+27, parm); *((uint64_t *)op+i*31+27)  = (uint64_t)SRC(ip, i*32+27) >> 54 | (uint64_t)SRC1(ip, i*32+28) <<  8;\
  IPPB(ip, i*32+28, parm); *((uint64_t *)op+i*31+28)  = (uint64_t)SRC(ip, i*32+28) >> 56 | (uint64_t)SRC1(ip, i*32+29) <<  6;\
  IPPB(ip, i*32+29, parm); *((uint64_t *)op+i*31+29)  = (uint64_t)SRC(ip, i*32+29) >> 58 | (uint64_t)SRC1(ip, i*32+30) <<  4;\
  IPPB(ip, i*32+30, parm); *((uint64_t *)op+i*31+30)  = (uint64_t)SRC(ip, i*32+30) >> 60;\
  IPPB(ip, i*32+31, parm); *((uint64_t *)op+i*31+30) |= (uint64_t)SRC(ip, i*32+31) <<  2;\
}

#define BITPACK64_62(ip,  op, parm) { \
  BITBLK64_62(ip, 0, op, parm);  SRCI(ip); op += 62*4/sizeof(op[0]);\
}

#define BITBLK64_63(ip, i, op, parm) { ;\
  IPPB(ip, i*64+ 0, parm); *((uint64_t *)op+i*63+ 0)  = (uint64_t)SRC(ip, i*64+ 0)       | (uint64_t)SRC1(ip, i*64+1) << 63;\
  IPPB(ip, i*64+ 1, parm); *((uint64_t *)op+i*63+ 1)  = (uint64_t)SRC(ip, i*64+ 1) >>  1 | (uint64_t)SRC1(ip, i*64+2) << 62;\
  IPPB(ip, i*64+ 2, parm); *((uint64_t *)op+i*63+ 2)  = (uint64_t)SRC(ip, i*64+ 2) >>  2 | (uint64_t)SRC1(ip, i*64+3) << 61;\
  IPPB(ip, i*64+ 3, parm); *((uint64_t *)op+i*63+ 3)  = (uint64_t)SRC(ip, i*64+ 3) >>  3 | (uint64_t)SRC1(ip, i*64+4) << 60;\
  IPPB(ip, i*64+ 4, parm); *((uint64_t *)op+i*63+ 4)  = (uint64_t)SRC(ip, i*64+ 4) >>  4 | (uint64_t)SRC1(ip, i*64+5) << 59;\
  IPPB(ip, i*64+ 5, parm); *((uint64_t *)op+i*63+ 5)  = (uint64_t)SRC(ip, i*64+ 5) >>  5 | (uint64_t)SRC1(ip, i*64+6) << 58;\
  IPPB(ip, i*64+ 6, parm); *((uint64_t *)op+i*63+ 6)  = (uint64_t)SRC(ip, i*64+ 6) >>  6 | (uint64_t)SRC1(ip, i*64+7) << 57;\
  IPPB(ip, i*64+ 7, parm); *((uint64_t *)op+i*63+ 7)  = (uint64_t)SRC(ip, i*64+ 7) >>  7 | (uint64_t)SRC1(ip, i*64+8) << 56;\
  IPPB(ip, i*64+ 8, parm); *((uint64_t *)op+i*63+ 8)  = (uint64_t)SRC(ip, i*64+ 8) >>  8 | (uint64_t)SRC1(ip, i*64+9) << 55;\
  IPPB(ip, i*64+ 9, parm); *((uint64_t *)op+i*63+ 9)  = (uint64_t)SRC(ip, i*64+ 9) >>  9 | (uint64_t)SRC1(ip, i*64+10) << 54;\
  IPPB(ip, i*64+10, parm); *((uint64_t *)op+i*63+10)  = (uint64_t)SRC(ip, i*64+10) >> 10 | (uint64_t)SRC1(ip, i*64+11) << 53;\
  IPPB(ip, i*64+11, parm); *((uint64_t *)op+i*63+11)  = (uint64_t)SRC(ip, i*64+11) >> 11 | (uint64_t)SRC1(ip, i*64+12) << 52;\
  IPPB(ip, i*64+12, parm); *((uint64_t *)op+i*63+12)  = (uint64_t)SRC(ip, i*64+12) >> 12 | (uint64_t)SRC1(ip, i*64+13) << 51;\
  IPPB(ip, i*64+13, parm); *((uint64_t *)op+i*63+13)  = (uint64_t)SRC(ip, i*64+13) >> 13 | (uint64_t)SRC1(ip, i*64+14) << 50;\
  IPPB(ip, i*64+14, parm); *((uint64_t *)op+i*63+14)  = (uint64_t)SRC(ip, i*64+14) >> 14 | (uint64_t)SRC1(ip, i*64+15) << 49;\
  IPPB(ip, i*64+15, parm); *((uint64_t *)op+i*63+15)  = (uint64_t)SRC(ip, i*64+15) >> 15 | (uint64_t)SRC1(ip, i*64+16) << 48;\
  IPPB(ip, i*64+16, parm); *((uint64_t *)op+i*63+16)  = (uint64_t)SRC(ip, i*64+16) >> 16 | (uint64_t)SRC1(ip, i*64+17) << 47;\
  IPPB(ip, i*64+17, parm); *((uint64_t *)op+i*63+17)  = (uint64_t)SRC(ip, i*64+17) >> 17 | (uint64_t)SRC1(ip, i*64+18) << 46;\
  IPPB(ip, i*64+18, parm); *((uint64_t *)op+i*63+18)  = (uint64_t)SRC(ip, i*64+18) >> 18 | (uint64_t)SRC1(ip, i*64+19) << 45;\
  IPPB(ip, i*64+19, parm); *((uint64_t *)op+i*63+19)  = (uint64_t)SRC(ip, i*64+19) >> 19 | (uint64_t)SRC1(ip, i*64+20) << 44;\
  IPPB(ip, i*64+20, parm); *((uint64_t *)op+i*63+20)  = (uint64_t)SRC(ip, i*64+20) >> 20 | (uint64_t)SRC1(ip, i*64+21) << 43;\
  IPPB(ip, i*64+21, parm); *((uint64_t *)op+i*63+21)  = (uint64_t)SRC(ip, i*64+21) >> 21 | (uint64_t)SRC1(ip, i*64+22) << 42;\
  IPPB(ip, i*64+22, parm); *((uint64_t *)op+i*63+22)  = (uint64_t)SRC(ip, i*64+22) >> 22 | (uint64_t)SRC1(ip, i*64+23) << 41;\
  IPPB(ip, i*64+23, parm); *((uint64_t *)op+i*63+23)  = (uint64_t)SRC(ip, i*64+23) >> 23 | (uint64_t)SRC1(ip, i*64+24) << 40;\
  IPPB(ip, i*64+24, parm); *((uint64_t *)op+i*63+24)  = (uint64_t)SRC(ip, i*64+24) >> 24 | (uint64_t)SRC1(ip, i*64+25) << 39;\
  IPPB(ip, i*64+25, parm); *((uint64_t *)op+i*63+25)  = (uint64_t)SRC(ip, i*64+25) >> 25 | (uint64_t)SRC1(ip, i*64+26) << 38;\
  IPPB(ip, i*64+26, parm); *((uint64_t *)op+i*63+26)  = (uint64_t)SRC(ip, i*64+26) >> 26 | (uint64_t)SRC1(ip, i*64+27) << 37;\
  IPPB(ip, i*64+27, parm); *((uint64_t *)op+i*63+27)  = (uint64_t)SRC(ip, i*64+27) >> 27 | (uint64_t)SRC1(ip, i*64+28) << 36;\
  IPPB(ip, i*64+28, parm); *((uint64_t *)op+i*63+28)  = (uint64_t)SRC(ip, i*64+28) >> 28 | (uint64_t)SRC1(ip, i*64+29) << 35;\
  IPPB(ip, i*64+29, parm); *((uint64_t *)op+i*63+29)  = (uint64_t)SRC(ip, i*64+29) >> 29 | (uint64_t)SRC1(ip, i*64+30) << 34;\
  IPPB(ip, i*64+30, parm); *((uint64_t *)op+i*63+30)  = (uint64_t)SRC(ip, i*64+30) >> 30 | (uint64_t)SRC1(ip, i*64+31) << 33;\
  IPPB(ip, i*64+31, parm); *((uint64_t *)op+i*63+31)  = (uint64_t)SRC(ip, i*64+31) >> 31;\
}

#define BITPACK64_63(ip,  op, parm) { \
  BITBLK64_63(ip, 0, op, parm);  SRCI(ip); op += 63*4/sizeof(op[0]);\
}

#define BITBLK64_64(ip, i, op, parm) { ;\
  IPPB(ip, i*1+ 0, parm); *((uint64_t *)op+i*1+ 0)  = (uint64_t)SRC(ip, i*1+ 0)      ;\
}

#define BITPACK64_64(ip,  op, parm) { \
  BITBLK64_64(ip, 0, op, parm);\
  BITBLK64_64(ip, 1, op, parm);\
  BITBLK64_64(ip, 2, op, parm);\
  BITBLK64_64(ip, 3, op, parm);\
  BITBLK64_64(ip, 4, op, parm);\
  BITBLK64_64(ip, 5, op, parm);\
  BITBLK64_64(ip, 6, op, parm);\
  BITBLK64_64(ip, 7, op, parm);\
  BITBLK64_64(ip, 8, op, parm);\
  BITBLK64_64(ip, 9, op, parm);\
  BITBLK64_64(ip, 10, op, parm);\
  BITBLK64_64(ip, 11, op, parm);\
  BITBLK64_64(ip, 12, op, parm);\
  BITBLK64_64(ip, 13, op, parm);\
  BITBLK64_64(ip, 14, op, parm);\
  BITBLK64_64(ip, 15, op, parm);\
  BITBLK64_64(ip, 16, op, parm);\
  BITBLK64_64(ip, 17, op, parm);\
  BITBLK64_64(ip, 18, op, parm);\
  BITBLK64_64(ip, 19, op, parm);\
  BITBLK64_64(ip, 20, op, parm);\
  BITBLK64_64(ip, 21, op, parm);\
  BITBLK64_64(ip, 22, op, parm);\
  BITBLK64_64(ip, 23, op, parm);\
  BITBLK64_64(ip, 24, op, parm);\
  BITBLK64_64(ip, 25, op, parm);\
  BITBLK64_64(ip, 26, op, parm);\
  BITBLK64_64(ip, 27, op, parm);\
  BITBLK64_64(ip, 28, op, parm);\
  BITBLK64_64(ip, 29, op, parm);\
  BITBLK64_64(ip, 30, op, parm);\
  BITBLK64_64(ip, 31, op, parm);  SRCI(ip); op += 64*4/sizeof(op[0]);\
}
