/**
    Copyright (C) powturbo 2013-2023
    SPDX-License-Identifier: GPL v2 License

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
//-- "Interpolative Coding" --------------------------------------------------------------
#if defined(_MSC_VER) && (_MSC_VER < 1600)
  #if !defined(_STDINT) && !defined(_MSC_STDINT_H_)
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
  #endif
#else
#include <stdint.h>
#endif
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

//-- maximum output size for encoding
size_t bicbound16(size_t n);
size_t bicbound32(size_t n);

//-- Simple binary
unsigned bicbenc16(uint16_t      *in, unsigned n, unsigned char *out);
unsigned bicbdec16(unsigned char *in, unsigned n, uint16_t      *out);
unsigned bicbenc32(uint32_t      *in, unsigned n, unsigned char *out);
unsigned bicbdec32(unsigned char *in, unsigned n, uint32_t      *out);

//-- Leftmost minimal 
unsigned bicenc16( uint16_t      *in, unsigned n, unsigned char *out);
unsigned bicdec16( unsigned char *in, unsigned n, uint16_t      *out);
unsigned bicenc32( uint32_t      *in, unsigned n, unsigned char *out);
unsigned bicdec32( unsigned char *in, unsigned n, uint32_t      *out);

//-- Center Minimal
unsigned bicmenc16(uint16_t      *in, unsigned n, unsigned char *out);
unsigned bicmdec16(unsigned char *in, unsigned n, uint16_t      *out);
unsigned bicmenc32(uint32_t      *in, unsigned n, unsigned char *out);
unsigned bicmdec32(unsigned char *in, unsigned n, uint32_t      *out);

#ifdef __cplusplus
}
#endif

