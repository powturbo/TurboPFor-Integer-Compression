/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "unit_tests.h"
#include "../../mem.h"

int test_little_endian_read_and_write_u16(void)
{
  const char *in_data = "\x11\x22";
  char out_data[2];
  uint16_t value;

  value = mem_read_le_u16(in_data);
  mem_write_le_u16(out_data, value);

  EXPECT_TRUE(memcmp(in_data, out_data, 2) == 0);

  return 1;
}

int test_little_endian_read_and_write_u32(void)
{
  const char *in_data = "\x11\x22\x33\x44";
  char out_data[4];
  uint32_t value;

  value = mem_read_le_u32(in_data);
  mem_write_le_u32(out_data, value);

  EXPECT_TRUE(memcmp(in_data, out_data, 4) == 0);

  return 1;
}

int test_little_endian_read_and_write_u64(void)
{
  const char *in_data = "\x11\x22\x33\x44\x55\x66\x77\x88";
  char out_data[8];
  uint64_t value;

  value = mem_read_le_u64(in_data);
  mem_write_le_u64(out_data, value);

  EXPECT_TRUE(memcmp(in_data, out_data, 8) == 0);

  return 1;
}
