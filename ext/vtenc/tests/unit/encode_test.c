/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include <stdlib.h>
#include <string.h>

#include "unit_tests.h"
#include "../../vtenc.h"

int test_vtenc_list_encode_u8_1(void)
{
  const uint8_t in[] = {};
  const size_t in_len = 0;
  const size_t out_cap = vtenc_list_max_encoded_size_u8(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 8);
  EXPECT_TRUE(memcmp(out, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0);

  return 1;
}

int test_vtenc_list_encode_u8_2(void)
{
  const uint8_t in[] = {5, 22, 23, 44, 62, 69, 109, 113, 178, 194, 206};
  const size_t in_len = 11;
  const size_t out_cap = vtenc_list_max_encoded_size_u8(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 17);
  EXPECT_TRUE(memcmp(out, "\x0b\x00\x00\x00\x00\x00\x00\xb0\x56\x09\xc5\xd8\x8a\x2e\x56\x2b\x01", 17) == 0);

  return 1;
}

int test_vtenc_list_encode_u8_3(void)
{
  const uint8_t in[] = {57, 57, 57, 111, 111, 111, 111, 208, 208};
  const size_t in_len = 9;
  const size_t out_cap = vtenc_list_max_encoded_size_u8(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 14);
  EXPECT_TRUE(memcmp(out, "\x09\x00\x00\x00\x00\x00\x00\x6e\xc0\x03\x02\x00\xa2\x0a", 14) == 0);

  return 1;
}

int test_vtenc_list_encode_u16_1(void)
{
  const uint16_t in[] = {};
  const size_t in_len = 0;
  const size_t out_cap = vtenc_list_max_encoded_size_u16(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u16(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 8);
  EXPECT_TRUE(memcmp(out, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0);

  return 1;
}

int test_vtenc_list_encode_u16_2(void)
{
  const uint16_t in[] = {1099, 2227, 8102, 27654, 29001, 35511, 50083};
  const size_t in_len = 7;
  const size_t out_cap = vtenc_list_max_encoded_size_u16(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u16(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 21);
  EXPECT_TRUE(memcmp(out,
    "\x07\x00\x00\x00\x00\x00\x00\xba\xcd\x2d\x97\x01\x4d\xe2\xcf\x6b"
    "\x6b\x95\x78\x74\x00", 21) == 0);

  return 1;
}

int test_vtenc_list_encode_u16_3(void)
{
  const uint16_t in[] = {677, 677, 677, 8881, 8881, 8881, 8881, 8881};
  const size_t in_len = 8;
  const size_t out_cap = vtenc_list_max_encoded_size_u16(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u16(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 17);
  EXPECT_TRUE(memcmp(out, "\x08\x00\x00\x00\x00\x00\x00\x10\xe7\x67\xe6\x99\xb6\x28\x0a\xb4\x05", 17) == 0);

  return 1;
}

int test_vtenc_list_encode_u32_1(void)
{
  const uint32_t in[] = {};
  const size_t in_len = 0;
  const size_t out_cap = vtenc_list_max_encoded_size_u32(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u32(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 8);
  EXPECT_TRUE(memcmp(out, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0);

  return 1;
}

int test_vtenc_list_encode_u32_2(void)
{
  const uint32_t in[] = {5348, 13089333, 88199704, 271008013, 1451881090};
  const size_t in_len = 5;
  const size_t out_cap = vtenc_list_max_encoded_size_u32(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u32(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 27);
  EXPECT_TRUE(memcmp(out,
    "\x05\x00\x00\x00\x00\x00\x00\x4a\x6e\xb5\xff\x35\xb6\x43\x74\xaa"
    "\x3e\xda\xf3\xdf\xe8\x7b\x2a\xdd\x60\x7d\x01", 27) == 0);

  return 1;
}

int test_vtenc_list_encode_u32_3(void)
{
  const uint32_t in[] = {77865901, 77865901, 77865901, 77865901, 314976310, 314976310};
  const size_t in_len = 6;
  const size_t out_cap = vtenc_list_max_encoded_size_u32(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u32(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 27);
  EXPECT_TRUE(memcmp(out,
    "\x06\x00\x00\x00\x00\x00\x00\x6c\x93\x20\x41\x90\x20\x49\x90\x04"
    "\x40\x10\x20\x14\x41\x05\x15\x51\x15\x04\x01", 27) == 0);

  return 1;
}

int test_vtenc_list_encode_u64_1(void)
{
  const uint64_t in[] = {};
  const size_t in_len = 0;
  const size_t out_cap = vtenc_list_max_encoded_size_u64(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u64(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 8);
  EXPECT_TRUE(memcmp(out, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0);

  return 1;
}

int test_vtenc_list_encode_u64_2(void)
{
  const uint64_t in[] = {0x12a6, 0x8addf0, 0xffa1b4bb, 0x21258ee39aaa};
  const size_t in_len = 4;
  const size_t out_cap = vtenc_list_max_encoded_size_u64(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u64(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 35);
  EXPECT_TRUE(memcmp(out,
    "\x04\x00\x00\x00\x00\x00\x00\x48\x92\x24\x49\x92\x24\xc9\xfd\xff"
    "\xff\xaf\xaa\xda\xff\x56\xf3\x4a\x04\x0f\xd0\x93\x16\x79\x5b\x8e"
    "\x38\xa6\xaa", 35) == 0);

  return 1;
}

int test_vtenc_list_encode_u64_3(void)
{
  const uint64_t in[] = {0x55555555, 0x55555555, 0x55555555, 0x111111111111, 0x111111111111};
  const size_t in_len = 5;
  const size_t out_cap = vtenc_list_max_encoded_size_u64(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_list_encode_u64(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 37);
  EXPECT_TRUE(memcmp(out,
    "\x05\x00\x00\x00\x00\x00\x00\xda\xb6\x6d\xdb\xb6\x6d\xdb\xee\xff"
    "\xff\x7f\x66\x66\x66\x66\x66\x66\x66\x46\x45\x45\x45\x45\x45\x45"
    "\x45\x45\x45\x45\x05", 37) == 0);

  return 1;
}

int test_vtenc_set_encode_u8_1(void)
{
  const uint8_t in[] = {};
  const size_t in_len = 0;
  const size_t out_cap = 0;
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorInputTooSmall);

  return 1;
}

int test_vtenc_set_encode_u8_2(void)
{
  const uint8_t in[] = {};
  const size_t in_len = 257;
  const size_t out_cap = 0;
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorInputTooBig);

  return 1;
}

int test_vtenc_set_encode_u8_3(void)
{
  const size_t in_len = 256;
  uint8_t in[in_len];
  const size_t out_cap = vtenc_set_max_encoded_size_u8(in_len);
  uint8_t out[out_cap];
  size_t out_len, i;

  for (i=0; i<in_len; ++i) in[i] = (uint8_t)i;

  EXPECT_TRUE(vtenc_set_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 1);
  EXPECT_TRUE(memcmp(out, "\xff", 1) == 0);

  return 1;
}

int test_vtenc_set_encode_u8_4(void)
{
  const uint8_t in[] = {33};
  const size_t in_len = 1;
  const size_t out_cap = vtenc_set_max_encoded_size_u8(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 2);
  EXPECT_TRUE(memcmp(out, "\x00\x7b", 2) == 0);

  return 1;
}

int test_vtenc_set_encode_u8_5(void)
{
  const uint8_t in[] = {0, 1, 2, 3, 4, 5, 6, 7, 160, 161, 162, 163};
  const size_t in_len = 12;
  const size_t out_cap = vtenc_set_max_encoded_size_u8(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 6);
  EXPECT_TRUE(memcmp(out, "\x0b\x88\x88\x48\x90\x04", 6) == 0);

  return 1;
}

int test_vtenc_set_encode_u8_6(void)
{
  const uint8_t in[] = {13, 77, 88, 93, 149, 212};
  const size_t in_len = 6;
  const size_t out_cap = vtenc_set_max_encoded_size_u8(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u8(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 7);
  EXPECT_TRUE(memcmp(out, "\x05\xcc\x74\x44\x57\x55\x03", 7) == 0);

  return 1;
}

int test_vtenc_set_encode_u16_1(void)
{
  const uint16_t in[] = {};
  const size_t in_len = 0;
  const size_t out_cap = 0;
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u16(in, in_len, out, out_cap, &out_len) == VtencErrorInputTooSmall);

  return 1;
}

int test_vtenc_set_encode_u16_2(void)
{
  const uint16_t in[] = {};
  const size_t in_len = 65537;
  const size_t out_cap = 0;
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u16(in, in_len, out, out_cap, &out_len) == VtencErrorInputTooBig);

  return 1;
}

int test_vtenc_set_encode_u16_3(void)
{
  const uint16_t in[] = {1655, 3391, 4111, 8770, 29006, 32712, 32993, 58042};
  const size_t in_len = 8;
  const size_t out_cap = vtenc_set_max_encoded_size_u16(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u16(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 17);
  EXPECT_TRUE(memcmp(out, "\x07\x00\xc6\x19\x23\x34\xf0\x0f\xb7\x17\x5a\x23\xd8\xfb\xf1\x5c\x51", 17) == 0);

  return 1;
}

int test_vtenc_set_encode_u16_4(void)
{
  const uint16_t in[] = {
    14000, 14001, 14002, 14003, 14004, 14005, 14006, 14007,
    20000, 20001, 20002, 20003
  };
  const size_t in_len = 12;
  const size_t out_cap = vtenc_set_max_encoded_size_u16(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u16(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 13);
  EXPECT_TRUE(memcmp(out, "\x0b\x00\x8c\x00\x08\x80\x80\x00\x48\x02\x20\x09\x92", 13) == 0);

  return 1;
}

int test_vtenc_set_encode_u32_1(void)
{
  const uint32_t in[] = {};
  const size_t in_len = 0;
  const size_t out_cap = 0;
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u32(in, in_len, out, out_cap, &out_len) == VtencErrorInputTooSmall);

  return 1;
}

int test_vtenc_set_encode_u32_2(void)
{
  const uint32_t in[] = {};
  const size_t in_len = 0x200000000;
  const size_t out_cap = 0;
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u32(in, in_len, out, out_cap, &out_len) == VtencErrorInputTooBig);

  return 1;
}

int test_vtenc_set_encode_u32_3(void)
{
  const uint32_t in[] = {0x98b204, 0x122fabb4, 0x378ecef0, 0x77ccab8f, 0xa40609bb};
  const size_t in_len = 5;
  const size_t out_cap = vtenc_set_max_encoded_size_u32(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u32(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 24);
  EXPECT_TRUE(memcmp(out,
    "\x04\x00\x00\x00\x9c\xbd\xb9\xec\xf7\x2e\xa8\x48\x0b\x47\x46\x78"
    "\x82\x59\xc5\xa1\xfd\xf9\x26\x02", 24) == 0);

  return 1;
}

int test_vtenc_set_encode_u32_4(void)
{
  const uint32_t in[] = {
    0xa500, 0xa501, 0xa502, 0xa503,
    0x4bbb00, 0x4bbb01,
    0xffff00, 0xffff01, 0xffff02, 0xffff03, 0xffff04, 0xffff05, 0xffff06, 0xffff07
  };
  const size_t in_len = 14;
  const size_t out_cap = vtenc_set_max_encoded_size_u32(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u32(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 32);
  EXPECT_TRUE(memcmp(out,
    "\x0d\x00\x00\x00\xee\xee\xee\xee\x46\x92\x24\x41\x90\x20\x48\x92"
    "\x54\x04\x04\x04\x55\x15\x00\x00\x00\x00\x00\x00\x00\x10\x11\x11", 32) == 0);

  return 1;
}

int test_vtenc_set_encode_u64_1(void)
{
  const uint64_t in[] = {};
  const size_t in_len = 0;
  const size_t out_cap = 0;
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u64(in, in_len, out, out_cap, &out_len) == VtencErrorInputTooSmall);

  return 1;
}

int test_vtenc_set_encode_u64_2(void)
{
  const uint64_t in[] = {
    0x11223344ULL, 0xaabbccddULL, 0x1010101010ULL, 0x5555555555ULL, 0xf0f0f0f0f0ULL,
    0x998877665544ULL, 0xffeeffeeffeeULL
  };
  const size_t in_len = 7;
  const size_t out_cap = vtenc_set_max_encoded_size_u64(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u64(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 53);
  EXPECT_TRUE(memcmp(out,
    "\x06\x00\x00\x00\x00\x00\x00\xfe\xff\xff\xff\xff\xff\xdb\xb6\x6d"
    "\xb9\x55\x6d\xb7\x3b\xd3\x5d\x15\x61\x26\xfa\xfb\xfb\xfb\xfb\xaa"
    "\xaa\xaa\xaa\x0a\x0f\x0f\x0f\x0f\x5f\xe6\x1e\x91\x59\xd5\x0d\x20"
    "\x02\x20\x02\x20\x02", 53) == 0);

  return 1;
}

int test_vtenc_set_encode_u64_3(void)
{
  const uint64_t in[] = {
    0x20000000ULL, 0x20000001ULL, 0x20000002ULL, 0x20000003ULL,
    0x80000000ULL, 0x80000001ULL,
    0x2000000000ULL, 0x2000000001ULL
  };
  const size_t in_len = 8;
  const size_t out_cap = vtenc_set_max_encoded_size_u64(in_len);
  uint8_t out[out_cap];
  size_t out_len;

  EXPECT_TRUE(vtenc_set_encode_u64(in, in_len, out, out_cap, &out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 51);
  EXPECT_TRUE(memcmp(out,
    "\x07\x00\x00\x00\x00\x00\x00\x10\x11\x11\x11\x11\x11\x11\x11\x11"
    "\x11\x11\x11\x11\xcd\xb6\x4d\x82\x24\x49\x92\x24\x49\x92\x24\x49"
    "\x92\xa4\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
    "\xaa\xaa\x02", 51) == 0);

  return 1;
}

int test_vtenc_list_max_encoded_size_u8(void)
{
  EXPECT_TRUE(vtenc_list_max_encoded_size_u8(0) == 8);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u8(1) == 16);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u8(5) == 16);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u8(10) == 24);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u8(100) == 112);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u8(1000) == 1008);

  return 1;
}

int test_vtenc_list_max_encoded_size_u16(void)
{
  EXPECT_TRUE(vtenc_list_max_encoded_size_u16(0) == 16);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u16(1) == 16);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u16(5) == 24);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u16(10) == 32);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u16(100) == 216);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u16(1000) == 2016);

  return 1;
}

int test_vtenc_list_max_encoded_size_u32(void)
{
  EXPECT_TRUE(vtenc_list_max_encoded_size_u32(0) == 16);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u32(1) == 16);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u32(5) == 32);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u32(10) == 56);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u32(100) == 416);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u32(1000) == 4016);

  return 1;
}

int test_vtenc_list_max_encoded_size_u64(void)
{
  EXPECT_TRUE(vtenc_list_max_encoded_size_u64(0) == 16);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u64(1) == 24);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u64(5) == 56);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u64(10) == 96);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u64(100) == 816);
  EXPECT_TRUE(vtenc_list_max_encoded_size_u64(1000) == 8016);

  return 1;
}

int test_vtenc_set_max_encoded_size_u8(void)
{
  EXPECT_TRUE(vtenc_set_max_encoded_size_u8(0) == 8);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u8(1) == 8);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u8(5) == 8);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u8(10) == 16);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u8(100) == 104);

  return 1;
}

int test_vtenc_set_max_encoded_size_u16(void)
{
  EXPECT_TRUE(vtenc_set_max_encoded_size_u16(0) == 8);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u16(1) == 8);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u16(5) == 16);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u16(10) == 24);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u16(100) == 208);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u16(1000) == 2008);

  return 1;
}

int test_vtenc_set_max_encoded_size_u32(void)
{
  EXPECT_TRUE(vtenc_set_max_encoded_size_u32(0) == 8);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u32(1) == 16);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u32(5) == 32);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u32(10) == 48);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u32(100) == 408);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u32(1000) == 4008);

  return 1;
}

int test_vtenc_set_max_encoded_size_u64(void)
{
  EXPECT_TRUE(vtenc_set_max_encoded_size_u64(0) == 16);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u64(1) == 24);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u64(5) == 56);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u64(10) == 96);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u64(100) == 816);
  EXPECT_TRUE(vtenc_set_max_encoded_size_u64(1000) == 8016);

  return 1;
}
