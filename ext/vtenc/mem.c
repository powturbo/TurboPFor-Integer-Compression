/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include "mem.h"

#include <string.h>

#include "bits.h"

unsigned mem_is_little_endian(void)
{
  const union { uint32_t u; uint8_t c[4]; } one = { 1 };
  return one.c[0];
}

/* Generic read functions */

uint16_t mem_read_u16(const void* mem_ptr)
{
  uint16_t value;
  memcpy(&value, mem_ptr, sizeof(value));
  return value;
}

uint32_t mem_read_u32(const void* mem_ptr)
{
  uint32_t value;
  memcpy(&value, mem_ptr, sizeof(value));
  return value;
}

uint64_t mem_read_u64(const void* mem_ptr)
{
  uint64_t value;
  memcpy(&value, mem_ptr, sizeof(value));
  return value;
}

/* Generic write functions */

void mem_write_u16(void* mem_ptr, uint16_t value)
{
  memcpy(mem_ptr, &value, sizeof(value));
}

void mem_write_u32(void* mem_ptr, uint32_t value)
{
  memcpy(mem_ptr, &value, sizeof(value));
}

void mem_write_u64(void* mem_ptr, uint64_t value)
{
  memcpy(mem_ptr, &value, sizeof(value));
}

/* Little endian read functions */

uint16_t mem_read_le_u16(const void* mem_ptr)
{
  if (mem_is_little_endian())
    return mem_read_u16(mem_ptr);
  else
    return bits_swap_u16(mem_read_u16(mem_ptr));
}

uint32_t mem_read_le_u32(const void* mem_ptr)
{
  if (mem_is_little_endian())
    return mem_read_u32(mem_ptr);
  else
    return bits_swap_u32(mem_read_u32(mem_ptr));
}

uint64_t mem_read_le_u64(const void* mem_ptr)
{
  if (mem_is_little_endian())
    return mem_read_u64(mem_ptr);
  else
    return bits_swap_u64(mem_read_u64(mem_ptr));
}

/* Little endian write functions */

void mem_write_le_u16(void* mem_ptr, uint16_t value)
{
  if (mem_is_little_endian())
    mem_write_u16(mem_ptr, value);
  else
    mem_write_u16(mem_ptr, bits_swap_u16(value));
}

void mem_write_le_u32(void* mem_ptr, uint32_t value)
{
  if (mem_is_little_endian())
    mem_write_u32(mem_ptr, value);
  else
    mem_write_u32(mem_ptr, bits_swap_u32(value));
}

void mem_write_le_u64(void* mem_ptr, uint64_t value)
{
  if (mem_is_little_endian())
    mem_write_u64(mem_ptr, value);
  else
    mem_write_u64(mem_ptr, bits_swap_u64(value));
}
