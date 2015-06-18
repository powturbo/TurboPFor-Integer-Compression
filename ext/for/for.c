/*
 * Copyright (C) 2005-2015 Christoph Rupp (chris@crupp.de).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "for.h"

#include <assert.h>
#include <string.h> /* for memcpy */

#if defined(_MSC_VER) && _MSC_VER < 1600
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef signed char int8_t;
#else
#  include <stdint.h>
#endif

#define METADATA 5  /* size of metadata overhead */

#ifdef _MSC_VER
#  define INLINE __inline
#  include <intrin.h>

uint32_t __inline CLZ(uint32_t value) {
  uint32_t leading_zero = 0;
  _BitScanReverse(&leading_zero, value);
  return 31 - leading_zero;
}
#else
#  define INLINE inline
#  define CLZ __builtin_clz
#endif

typedef uint32_t(*for_unpackfunc_t) (uint32_t, const uint8_t *, uint32_t *);
typedef uint32_t(*for_packfunc_t)   (uint32_t, const uint32_t *, uint8_t *);
typedef uint32_t(*for_unpackxfunc_t) (uint32_t, const uint8_t *, uint32_t *,
	uint32_t);
typedef uint32_t(*for_packxfunc_t)   (uint32_t, const uint32_t *, uint8_t *,
	uint32_t);
typedef uint32_t(*for_linsearchfunc_t)(uint32_t, const uint8_t *, uint32_t,
	int *);
typedef uint32_t(*for_linsearchxfunc_t)(uint32_t, const uint8_t *, uint32_t,
	uint32_t, int *);

/* include the generated file */
#include "for-gen.c"

static INLINE uint32_t
bits(const uint32_t v)
{
  return v == 0 ? 0 : 32 - CLZ(v);
}

uint32_t
for_compressed_size_bits(uint32_t length, uint32_t bits)
{
  uint32_t c = 0;
  uint32_t b;

  assert(bits <= 32);

  /* each block is byte-aligned */
  if (length >= 32) {
    b = length / 32;
    c += ((b * 32 * bits) + 7) / 8;
    length %= 32;
  }

  if (length >= 16) {
    b = length / 16;
    c += ((b * 16 * bits) + 7) / 8;
    length %= 16;
  }

  if (length >= 8) {
    b = length / 8;
    c += ((b * 8 * bits) + 7) / 8;
    length %= 8;
  }

  return c + ((length * bits) + 7) / 8;
}

uint32_t
for_compressed_size_unsorted(const uint32_t *in, uint32_t length)
{
  uint32_t i, b, m, M;

  if (length == 0)
    return 0;

  /* calculate min/max */
  m = in[0];
  M = m;

  for (i = 1; i < length; i++) {
    if (in[i] < m)
      m = in[i];
    if (in[i] > M)
      M = in[i];
  }

  /* calculate the bits */
  b = bits(M - m);

  return METADATA + for_compressed_size_bits(length, b);
}

uint32_t
for_compressed_size_sorted(const uint32_t *in, uint32_t length)
{
  uint32_t b, m, M;

  if (length == 0)
    return 0;

  /* calculate min/max */
  m = in[0];
  M = in[length - 1];

  /* calculate the bits */
  b = bits(M - m);

  return METADATA + for_compressed_size_bits(length, b);
}

uint32_t
for_compress_bits(const uint32_t *in, uint8_t *out, uint32_t length,
                uint32_t base, uint32_t bits)
{
  uint32_t i = 0;
  uint32_t written = 0;

  assert(bits <= 32);

  for (; i + 32 <= length; i += 32, in += 32)
    written += for_pack32[bits](base, in, out + written);

  for (; i + 16 <= length; i += 16, in += 16)
    written += for_pack16[bits](base, in, out + written);

  for (; i + 8 <= length; i += 8, in += 8)
    written += for_pack8[bits](base, in, out + written);

  return written + for_packx[bits](base, in, out + written, length - i);
}

uint32_t
for_compress_unsorted(const uint32_t *in, uint8_t *out, uint32_t length)
{
  uint32_t i, b, m, M;

  if (length == 0)
    return 0;

  /* calculate min/max */
  m = in[0];
  M = m;

  for (i = 1; i < length; i++) {
    if (in[i] < m)
      m = in[i];
    if (in[i] > M)
      M = in[i];
  }

  /* calculate the bits */
  b = bits(M - m);

  /* store m and the bits */
  *(uint32_t *)(out + 0) = m;
  *(uint32_t *)(out + 4) = b;
  return METADATA + for_compress_bits(in, out + METADATA, length, m, b);
}

uint32_t
for_compress_sorted(const uint32_t *in, uint8_t *out, uint32_t length)
{
  uint32_t m, M, b;

  if (length == 0)
    return 0;

  /* fetch min/max */
  m = in[0];
  M = in[length - 1];

  /* calculate the bits */
  b = bits(M - m);

  /* store m and the bits */
  *(uint32_t *)(out + 0) = m;
  *(uint32_t *)(out + 4) = b;

  return METADATA + for_compress_bits(in, out + METADATA, length, m, b);
}

uint32_t
for_uncompress_bits(const uint8_t *in, uint32_t *out, uint32_t length,
                uint32_t base, uint32_t bits)
{
  uint32_t i = 0;
  const uint8_t *bin = in;

  assert(bits <= 32);

  for (; i + 32 <= length; i += 32, out += 32)
    in += for_unpack32[bits](base, in, out);

  for (; i + 16 <= length; i += 16, out += 16)
    in += for_unpack16[bits](base, in, out);

  for (; i + 8 <= length; i += 8, out += 8)
    in += for_unpack8[bits](base, in, out);

  return (in - bin) + for_unpackx[bits](base, in, out, length - i);
}

uint32_t
for_uncompress(const uint8_t *in, uint32_t *out, uint32_t length)
{
  uint32_t m, b;

  if (length == 0)
    return 0;

  /* load min and the bits */
  m = *(uint32_t *)(in + 0);
  b = *(in + 4);

  return METADATA + for_uncompress_bits(in + METADATA, out, length, m, b);
}

uint32_t
for_select_bits(const uint8_t *in, uint32_t base, uint32_t bits,
                uint32_t index)
{
  uint32_t b, start;
  const uint32_t *in32;

  assert(bits <= 32);

  if (bits == 32) {
    in32 = (uint32_t *)in;
    return base + in32[index];
  }

  if (index > 32) {
    b = index / 32;
    in += (b * 32 * bits) / 8;
    index %= 32;
  }

  if (index > 16) {
    b = index / 16;
    in += (b * 16 * bits) / 8;
    index %= 16;
  }

  if (index > 8) {
    b = index / 8;
    in += (b * 8 * bits) / 8;
    index %= 8;
  }

  start = index * bits;

  in += start / 8;
  start %= 8;

  /* |in| now points to the byte where the requested index is stored */
  /* |start| is the bit position where the compressed value starts */

  in32 = (uint32_t *)in;

  /* easy common case: the compressed value is not split between words */
  if (start + bits < 32) {
    uint32_t mask = (1 << bits) - 1;
    return base + ((*in32 >> start) & mask);
  }
  /* not so easy: restore value from two words */
  else {
    uint32_t mask1 = (1 << bits) - 1;
    uint32_t mask2 = (1 << (bits - (32 - start))) - 1;
    uint32_t v1 = (*(in32 + 0) >> start) & mask1;
    uint32_t v2 =  *(in32 + 1) & mask2;
    return base + ((v2 << (32 - start)) | v1);
  }
}

uint32_t
for_select(const uint8_t *in, uint32_t index)
{
  /* load min and the bits */
  uint32_t m = *(uint32_t *)(in + 0);
  uint32_t b = *(in + 4);

  return for_select_bits(in + METADATA, m, b, index);
}

uint32_t
for_linear_search(const uint8_t *in, uint32_t length, uint32_t value)
{
  /* load min and the bits */
  uint32_t m = *(uint32_t *)(in + 0);
  uint32_t b = *(in + 4);

  return for_linear_search_bits(in + METADATA, length, m, b, value);
}

uint32_t
for_linear_search_bits(const uint8_t *in, uint32_t length, uint32_t base,
                uint32_t bits, uint32_t value)
{
  uint32_t i = 0;
  int found = -1;

  assert(bits <= 32);
  if (bits == 0)
    return (value == base ? 0 : length);

  for (; i + 32 <= length; i += 32) {
    in += for_linsearch32[bits](base, in, value, &found);
    if (found >= 0)
      return i + found;
  }

  for (; i + 16 <= length; i += 16) {
    in += for_linsearch16[bits](base, in, value, &found);
    if (found >= 0)
      return i + found;
  }

  for (; i + 8 <= length; i += 8) {
    in += for_linsearch8[bits](base, in, value, &found);
    if (found >= 0)
      return i + found;
  }

  for_linsearchx[bits](base, in, length - i, value, &found);
  if (found >= 0)
    return i + found;

  /* not found */
  return length;
}

uint32_t
for_lower_bound_search(const uint8_t *in, uint32_t length, uint32_t value,
                uint32_t *actual)
{
  /* load min and the bits */
  uint32_t m = *(uint32_t *)(in + 0);
  uint32_t b = *(in + 4);

  return for_lower_bound_search_bits(in + METADATA, length, m, b,
                  value, actual);
}

/* adapted from wikipedia */
uint32_t
for_lower_bound_search_bits(const uint8_t *in, uint32_t length, uint32_t base,
                uint32_t bits, uint32_t value, uint32_t *actual)
{
  uint32_t imid;
  uint32_t imin = 0;
  uint32_t imax = length - 1;
  uint32_t v;

  while (imin + 1 < imax) {
    imid = imin + ((imax - imin) / 2);

    v = for_select_bits(in, base, bits, imid);
    if (v >= value) {
      imax = imid;
    }
    else if (v < value) {
      imin = imid;
    }
  }

  v = for_select_bits(in, base, bits, imin);
  if (v >= value) {
    *actual = v;
    return imin;
  }

  v = for_select_bits(in, base, bits, imax);
  *actual = v;
  return imax;
}
