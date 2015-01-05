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

    vsimple.c - "Integer Compression" variable simple
**/

#include "vsimple.h"
 
#define USE_RLE
                                     //   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32 
#define SV_LIM unsigned char s_lim[] = {  0, 28, 28, 28, 28, 36, 36, 36, 36, 36, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 0 };
#define SV_ITM unsigned      s_itm[] = {  0, 28, 14,  9,  7,  7,  6,  5,  4,  4,  6,  5,  5,  4,  4,  4,  3,  3,  3,  3,  3,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1, -1 }
static SV_ITM;
static SV_LIM;

#include <stdint.h>
#define USIZE 32
#include "vsimple_.h"
#undef USIZE

#define USIZE 16
#include "vsimple_.h"
#undef USIZE


