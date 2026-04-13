/**
    Copyright (C) powturbo 2013-2026
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
//-- "Integer Compression" variable length encoding with bitio ------------------

#ifdef __cplusplus
extern "C" {
#endif
//-- Gamma coding (length limited) : Full 32 bit range
size_t bitgenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);
size_t bitgdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

//-- Rice Coding (adaptive length limited) : Full 32 bit range ---
size_t bitrenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);
size_t bitrdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlcenc32(   unsigned char *_in, size_t _inlen,  unsigned char  *out);   // TurboVLC 32 bits
size_t vlcdec32(   unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlcenc16(   unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 16 bits
size_t vlcdec16(   unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlczenc16(  unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 16 bits zigzag
size_t vlczdec16(  unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlczenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 32 bits zigzag
size_t vlczdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

#ifdef __cplusplus
}
#endif

