/**
    Copyright (C) powturbo 2013-2026
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
//  "Integer Compression" TurboPFor - length checked decoding

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ic.h"

/* p4ndec32() has no input length: it decodes the groups its stream describes
   and cannot tell a truncated buffer from a complete one. For callers that do
   have a length, decode from a zero padded copy instead. Every read then stays
   inside our own allocation, and the number of bytes the decoder consumed
   tells us whether the supplied data was enough.

   A group reads at most CSIZE*USIZE/8 bytes of packed bits plus one varint and
   one position byte per exception, so P4NDEC32_PAD covers the group that
   straddles the end of the input. Every further group consumes at least its
   control byte, so the number of remaining groups is added as well. */
#define P4NDEC32_CSIZE 128
#define P4NDEC32_PAD   4096

size_t p4ndec32len(const unsigned char *__restrict in, size_t inlen, size_t n, uint32_t *__restrict out)
{
    unsigned char *buf;
    uint32_t *tmp;
    size_t groups, slack, cap, used;

    if(!n || !in || !out)
        return 0;
    if(n > (size_t)-1 - (P4NDEC32_CSIZE - 1))
        return 0;
    groups = (n + (P4NDEC32_CSIZE - 1)) / P4NDEC32_CSIZE;
    if(inlen > (size_t)-1 - groups - P4NDEC32_PAD)
        return 0;
    if(n > (size_t)-1 / sizeof(uint32_t))
        return 0;

    slack = groups + P4NDEC32_PAD;
    cap   = inlen + slack;
    buf = (unsigned char *)malloc(cap);
    tmp = (uint32_t *)malloc(n * sizeof(uint32_t));
    if(!buf || !tmp) {
        free(buf);
        free(tmp);
        return 0;
    }
    memcpy(buf, in, inlen);
    memset(buf + inlen, 0, slack);

    used = p4ndec32(buf, n, tmp);
    /* A complete stream consumes at least one byte per group and never walks
       past the last byte it was given, so anything else was truncated. */
    if(used > inlen) {
        free(buf);
        free(tmp);
        return 0;
    }
    memcpy(out, tmp, n * sizeof(uint32_t));
    free(buf);
    free(tmp);
    return used;
}
