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

    vp4dc.h - "Integer Compression" TurboPfor  (see vp4dd.h for decompression) 
**/
#ifdef __cplusplus
extern "C" {
#endif

// compress integer array with n values to the buffer out. Return value = end of compressed buffer out
unsigned char *p4denc32(unsigned       *__restrict in, int n, unsigned char *__restrict out);
unsigned char *p4denc16(unsigned short *__restrict in, int n, unsigned char *__restrict out);

#ifdef __cplusplus
}
#endif
