/**
    Copyright (C) powturbo 2013-2017
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
//   transpose.h - Byte/Nibble transpose 
#ifdef __cplusplus
extern "C" {
#endif
// Transpose/Shuffle block for further compressing with lz77 or other compressors
void tpenc(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize);
void tpdec(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

// Nibble transpose
void tp4enc(     unsigned char *in, unsigned n, unsigned char *out, unsigned esize);
void tp4dec(     unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

//---------- low level functions ----------------------------------
void tpenc2(     unsigned char *in, unsigned n, unsigned char *out);
void tpenc3(     unsigned char *in, unsigned n, unsigned char *out);
void tpenc4(     unsigned char *in, unsigned n, unsigned char *out);
void tpenc8(     unsigned char *in, unsigned n, unsigned char *out);
void tpenc16(    unsigned char *in, unsigned n, unsigned char *out);

void tpdec2(     unsigned char *in, unsigned n, unsigned char *out);
void tpdec3(     unsigned char *in, unsigned n, unsigned char *out);
void tpdec4(     unsigned char *in, unsigned n, unsigned char *out);
void tpdec8(     unsigned char *in, unsigned n, unsigned char *out);
void tpdec16(    unsigned char *in, unsigned n, unsigned char *out);

void tpenc128v2( unsigned char *in, unsigned n, unsigned char *out);
void tpdec128v2( unsigned char *in, unsigned n, unsigned char *out);
void tpenc128v4( unsigned char *in, unsigned n, unsigned char *out);
void tpdec128v4( unsigned char *in, unsigned n, unsigned char *out);
void tpenc128v8( unsigned char *in, unsigned n, unsigned char *out);
void tpdec128v8( unsigned char *in, unsigned n, unsigned char *out);

void tpenc256v2( unsigned char *in, unsigned n, unsigned char *out);
void tpdec256v2( unsigned char *in, unsigned n, unsigned char *out);
void tpenc256v4( unsigned char *in, unsigned n, unsigned char *out);
void tpdec256v4( unsigned char *in, unsigned n, unsigned char *out);
void tpenc256v8( unsigned char *in, unsigned n, unsigned char *out);
void tpdec256v8( unsigned char *in, unsigned n, unsigned char *out);

void tp4enc4(unsigned char *in, unsigned n, unsigned char *out);
void tp4dec4(unsigned char *in, unsigned n, unsigned char *out);

void tp4enc128v2( unsigned char *in, unsigned n, unsigned char *out);
void tp4dec128v2( unsigned char *in, unsigned n, unsigned char *out);
void tp4enc128v4( unsigned char *in, unsigned n, unsigned char *out);
void tp4dec128v4( unsigned char *in, unsigned n, unsigned char *out);
void tp4enc128v8( unsigned char *in, unsigned n, unsigned char *out);
void tp4dec128v8( unsigned char *in, unsigned n, unsigned char *out);

void tp4enc256v2( unsigned char *in, unsigned n, unsigned char *out);
void tp4dec256v2( unsigned char *in, unsigned n, unsigned char *out);
void tp4enc256v4( unsigned char *in, unsigned n, unsigned char *out);
void tp4dec256v4( unsigned char *in, unsigned n, unsigned char *out);
void tp4enc256v8( unsigned char *in, unsigned n, unsigned char *out);
void tp4dec256v8( unsigned char *in, unsigned n, unsigned char *out);

#ifdef __cplusplus
}
#endif
