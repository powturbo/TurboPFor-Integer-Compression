/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include "unit_tests.h"
#include "../../bits.h"

int test_bits_swap_u16(void)
{
  EXPECT_TRUE(bits_swap_u16(0) == 0);
  EXPECT_TRUE(bits_swap_u16(0xaabb) == 0xbbaa);
  EXPECT_TRUE(bits_swap_u16(0xffff) == 0xffff);

  return 1;
}

int test_bits_swap_u32(void)
{
  EXPECT_TRUE(bits_swap_u32(0) == 0);
  EXPECT_TRUE(bits_swap_u32(0xaaaabbbb) == 0xbbbbaaaa);
  EXPECT_TRUE(bits_swap_u32(0xffffffff) == 0xffffffff);

  return 1;
}

int test_bits_swap_u64(void)
{
  EXPECT_TRUE(bits_swap_u64(0) == 0);
  EXPECT_TRUE(bits_swap_u64(0xaaaaaaaabbbbbbbbULL) == 0xbbbbbbbbaaaaaaaaULL);
  EXPECT_TRUE(bits_swap_u64(0xffffffffffffffffULL) == 0xffffffffffffffffULL);

  return 1;
}

int test_bits_len_u8(void)
{
  EXPECT_TRUE(bits_len_u8(0) == 1);
  EXPECT_TRUE(bits_len_u8(1) == 1);
  EXPECT_TRUE(bits_len_u8(3) == 2);
  EXPECT_TRUE(bits_len_u8(7) == 3);
  EXPECT_TRUE(bits_len_u8(11) == 4);
  EXPECT_TRUE(bits_len_u8(25) == 5);
  EXPECT_TRUE(bits_len_u8(50) == 6);
  EXPECT_TRUE(bits_len_u8(100) == 7);
  EXPECT_TRUE(bits_len_u8(200) == 8);
  EXPECT_TRUE(bits_len_u8(255) == 8);

  return 1;
}

int test_bits_len_u16(void)
{
  EXPECT_TRUE(bits_len_u16(0) == 1);
  EXPECT_TRUE(bits_len_u16(1) == 1);
  EXPECT_TRUE(bits_len_u16(3) == 2);
  EXPECT_TRUE(bits_len_u16(7) == 3);
  EXPECT_TRUE(bits_len_u16(11) == 4);
  EXPECT_TRUE(bits_len_u16(25) == 5);
  EXPECT_TRUE(bits_len_u16(50) == 6);
  EXPECT_TRUE(bits_len_u16(100) == 7);
  EXPECT_TRUE(bits_len_u16(200) == 8);
  EXPECT_TRUE(bits_len_u16(255) == 8);
  EXPECT_TRUE(bits_len_u16(256) == 9);
  EXPECT_TRUE(bits_len_u16(2000) == 11);
  EXPECT_TRUE(bits_len_u16(50000) == 16);
  EXPECT_TRUE(bits_len_u16(65535) == 16);

  return 1;
}

int test_bits_len_u32(void)
{
  EXPECT_TRUE(bits_len_u32(0) == 1);
  EXPECT_TRUE(bits_len_u32(1) == 1);
  EXPECT_TRUE(bits_len_u32(50) == 6);
  EXPECT_TRUE(bits_len_u32(100) == 7);
  EXPECT_TRUE(bits_len_u32(255) == 8);
  EXPECT_TRUE(bits_len_u32(256) == 9);
  EXPECT_TRUE(bits_len_u32(10000) == 14);
  EXPECT_TRUE(bits_len_u32(65535) == 16);
  EXPECT_TRUE(bits_len_u32(65536) == 17);
  EXPECT_TRUE(bits_len_u32(1000000) == 20);
  EXPECT_TRUE(bits_len_u32(16777215) == 24);
  EXPECT_TRUE(bits_len_u32(16777216) == 25);
  EXPECT_TRUE(bits_len_u32(1000000000) == 30);
  EXPECT_TRUE(bits_len_u32(4294967295) == 32);

  return 1;
}

int test_bits_len_u64(void)
{
  EXPECT_TRUE(bits_len_u64(0) == 1);
  EXPECT_TRUE(bits_len_u64(1) == 1);
  EXPECT_TRUE(bits_len_u64(50) == 6);
  EXPECT_TRUE(bits_len_u64(100) == 7);
  EXPECT_TRUE(bits_len_u64(255) == 8);
  EXPECT_TRUE(bits_len_u64(256) == 9);
  EXPECT_TRUE(bits_len_u64(10000) == 14);
  EXPECT_TRUE(bits_len_u64(65535) == 16);
  EXPECT_TRUE(bits_len_u64(65536) == 17);
  EXPECT_TRUE(bits_len_u64(1000000) == 20);
  EXPECT_TRUE(bits_len_u64(16777215) == 24);
  EXPECT_TRUE(bits_len_u64(16777216) == 25);
  EXPECT_TRUE(bits_len_u64(1000000000) == 30);
  EXPECT_TRUE(bits_len_u64(4294967295) == 32);
  EXPECT_TRUE(bits_len_u64(0x000000ffffffffffULL) == 40);
  EXPECT_TRUE(bits_len_u64(0x0000ffffffffffffULL) == 48);
  EXPECT_TRUE(bits_len_u64(0x00ffffffffffffffULL) == 56);
  EXPECT_TRUE(bits_len_u64(0xffffffffffffffffULL) == 64);

  return 1;
}
