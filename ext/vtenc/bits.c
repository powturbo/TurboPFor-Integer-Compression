/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include "vtenc.h"

static const unsigned int BITS_LEN8[] = {
  1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4,
  5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

uint16_t bits_swap_u16(uint16_t value)
{
  return (value << 8) | (value >> 8);
}

uint32_t bits_swap_u32(uint32_t value)
{
  return  ((value << 24) & 0xff000000 ) |
          ((value <<  8) & 0x00ff0000 ) |
          ((value >>  8) & 0x0000ff00 ) |
          ((value >> 24) & 0x000000ff );
}

uint64_t bits_swap_u64(uint64_t value)
{
  return  ((value << 56) & 0xff00000000000000ULL) |
          ((value << 40) & 0x00ff000000000000ULL) |
          ((value << 24) & 0x0000ff0000000000ULL) |
          ((value << 8)  & 0x000000ff00000000ULL) |
          ((value >> 8)  & 0x00000000ff000000ULL) |
          ((value >> 24) & 0x0000000000ff0000ULL) |
          ((value >> 40) & 0x000000000000ff00ULL) |
          ((value >> 56) & 0x00000000000000ffULL);
}

unsigned int bits_len_u8(uint8_t value)
{
  return BITS_LEN8[value];
}

unsigned int bits_len_u16(uint16_t value)
{
  unsigned int n = 0;

  if (value > 0x00ff) {
    value >>= 8;
    n = 8;
  }

  return n + BITS_LEN8[value];
}

unsigned int bits_len_u32(uint32_t value)
{
  unsigned int n = 0;

  if (value > 0x0000ffff) {
    value >>= 16;
    n = 16;
  }
  if (value > 0x000000ff) {
    value >>= 8;
    n += 8;
  }

  return n + BITS_LEN8[value];
}

unsigned int bits_len_u64(uint64_t value)
{
  unsigned int n = 0;

  if (value > 0x00000000ffffffffULL) {
    value >>= 32;
    n = 32;
  }
  if (value > 0x000000000000ffffULL) {
    value >>= 16;
    n += 16;
  }
  if (value > 0x00000000000000ffULL) {
    value >>= 8;
    n += 8;
  }

  return n + BITS_LEN8[value];
}
