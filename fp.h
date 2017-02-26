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
//     "Floating Point Compression"
#ifdef __cplusplus
extern "C" {
#endif

unsigned char *fppenc64(    double        *in, unsigned n, unsigned char *out);
unsigned char *fppdec64(    unsigned char *in, unsigned n, double        *out);
unsigned char *fpfcmenc64( double        *in, unsigned n, unsigned char *out);
unsigned char *fpfcmdec64( unsigned char *in, unsigned n, double        *out);
unsigned char *fpdfcmenc64(double        *in, unsigned n, unsigned char *out);
unsigned char *fpdfcmdec64(unsigned char *in, unsigned n, double        *out);

#ifdef __cplusplus
}
#endif
