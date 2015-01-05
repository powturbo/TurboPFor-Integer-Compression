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

    vp4dd.c - "Integer Compression" Turbo PforDelta 
**/

#include <string.h>
#include "conf.h"
#include "bitpack.h"

#include "vp4dc.h"

#define PAD8(__x) ( (((__x)+8-1)/8) )
#include <stdint.h>
//------------------------------------------
#define P4DSIZE 128 //64 //
#define P4DENC p4denc

#define USIZE 32
#include "vp4dc_.h"
#undef USIZE

#define USIZE 16
#include "vp4dc_.h"

