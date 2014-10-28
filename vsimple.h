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

    vsimple.h - "Integer Compression" variable simple 
**/

#ifdef __cplusplus
extern "C" {
#endif

unsigned char *vsenc32(unsigned       *__restrict__ in, int n, unsigned char  *__restrict__ out);
unsigned char *vsdec32(unsigned char  *__restrict__ in, int n, unsigned       *__restrict__ out);

unsigned char *vsenc16(unsigned short *__restrict__ in, int n, unsigned char  *__restrict__ out);
unsigned char *vsdec16(unsigned char  *__restrict__ in, int n, unsigned short *__restrict__ out);

#ifdef __cplusplus
}
#endif



