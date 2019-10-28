/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "unit_tests.h"
#include "../../vtenc.h"

int test_vtenc_list_decode_u8_1(void)
{
  const uint8_t *in = (const uint8_t *)"\xff\xff\xff\xff\xff\xff\xff\xff";
  const size_t in_len = 8;
  size_t out_len = 1;
  uint8_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u8(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u8_2(void)
{
  const uint8_t *in = (const uint8_t *)"\x09\x00\x00\x00\x00\x00\x00\x00";
  const size_t in_len = 8;
  size_t out_len = 4;
  uint8_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u8(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u8_3(void)
{
  const uint8_t *in = (const uint8_t *)"\x04\x00\x00\x00\x00\x00\x00\x0e";
  const size_t in_len = 8;
  size_t out_len = vtenc_list_decoded_size_u8(in, in_len);
  uint8_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u8(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u8_4(void)
{
  const uint8_t *in = (const uint8_t *)"\x0b\x00\x00\x00\x00\x00\x00\xb0\x56\x09\xc5\xd8\x8a\x2e\x56\x2b\x01";
  const size_t in_len = 17;
  size_t out_len = vtenc_list_decoded_size_u8(in, in_len);
  uint8_t out[out_len];
  uint8_t expected[] = {5, 22, 23, 44, 62, 69, 109, 113, 178, 194, 206};

  EXPECT_TRUE(vtenc_list_decode_u8(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 11);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_list_decode_u8_5(void)
{
  const uint8_t *in = (const uint8_t *)"\x09\x00\x00\x00\x00\x00\x00\x6e\xc0\x03\x02\x00\xa2\x0a";
  const size_t in_len = 14;
  size_t out_len = vtenc_list_decoded_size_u8(in, in_len);
  uint8_t out[out_len];
  uint8_t expected[] = {57, 57, 57, 111, 111, 111, 111, 208, 208};

  EXPECT_TRUE(vtenc_list_decode_u8(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 9);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_list_decode_u16_1(void)
{
  const uint8_t *in = (const uint8_t *)"\xff\xff\xff\xff\xff\xff\xff\xff";
  const size_t in_len = 8;
  size_t out_len = 1;
  uint16_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u16(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u16_2(void)
{
  const uint8_t *in = (const uint8_t *)"\x09\x00\x00\x00\x00\x00\x00\x00";
  const size_t in_len = 8;
  size_t out_len = 4;
  uint16_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u16(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u16_3(void)
{
  const uint8_t *in = (const uint8_t *)"\x04\x00\x00\x00\x00\x00\x00\x0e";
  const size_t in_len = 8;
  size_t out_len = vtenc_list_decoded_size_u16(in, in_len);
  uint16_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u16(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u16_4(void)
{
  const uint8_t *in = (const uint8_t *)"\x07\x00\x00\x00\x00\x00\x00\xba\xcd\x2d\x97\x01\x4d\xe2\xcf\x6b\x6b\x95\x78\x74\x00";
  const size_t in_len = 21;
  size_t out_len = vtenc_list_decoded_size_u16(in, in_len);
  uint16_t out[out_len];
  uint16_t expected[] = {1099, 2227, 8102, 27654, 29001, 35511, 50083};

  EXPECT_TRUE(vtenc_list_decode_u16(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 7);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_list_decode_u16_5(void)
{
  const uint8_t *in = (const uint8_t *)"\x08\x00\x00\x00\x00\x00\x00\x10\xe7\x67\xe6\x99\xb6\x28\x0a\xb4\x05";
  const size_t in_len = 17;
  size_t out_len = vtenc_list_decoded_size_u16(in, in_len);
  uint16_t out[out_len];
  uint16_t expected[] = {677, 677, 677, 8881, 8881, 8881, 8881, 8881};

  EXPECT_TRUE(vtenc_list_decode_u16(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 8);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_list_decode_u32_1(void)
{
  const uint8_t *in = (const uint8_t *)"\xff\xff\xff\xff\xff\xff\xff\xff";
  const size_t in_len = 8;
  size_t out_len = 1;
  uint32_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u32(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u32_2(void)
{
  const uint8_t *in = (const uint8_t *)"\x09\x00\x00\x00\x00\x00\x00\x00";
  const size_t in_len = 8;
  size_t out_len = 4;
  uint32_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u32(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u32_3(void)
{
  const uint8_t *in = (const uint8_t *)"\x04\x00\x00\x00\x00\x00\x00\x0e";
  const size_t in_len = 8;
  size_t out_len = vtenc_list_decoded_size_u32(in, in_len);
  uint32_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u32(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u32_4(void)
{
  const uint8_t *in = (const uint8_t *)"\x05\x00\x00\x00\x00\x00\x00\x4a\x6e\xb5"
    "\xff\x35\xb6\x43\x74\xaa\x3e\xda\xf3\xdf\xe8\x7b\x2a\xdd\x60\x7d\x01";
  const size_t in_len = 27;
  size_t out_len = vtenc_list_decoded_size_u32(in, in_len);
  uint32_t out[out_len];
  uint32_t expected[] = {5348, 13089333, 88199704, 271008013, 1451881090};

  EXPECT_TRUE(vtenc_list_decode_u32(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 5);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_list_decode_u32_5(void)
{
  const uint8_t *in = (const uint8_t *)"\x06\x00\x00\x00\x00\x00\x00\x6c\x93\x20"
    "\x41\x90\x20\x49\x90\x04\x40\x10\x20\x14\x41\x05\x15\x51\x15\x04\x01";
  const size_t in_len = 27;
  size_t out_len = vtenc_list_decoded_size_u32(in, in_len);
  uint32_t out[out_len];
  uint32_t expected[] = {77865901, 77865901, 77865901, 77865901, 314976310, 314976310};

  EXPECT_TRUE(vtenc_list_decode_u32(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 6);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_list_decode_u64_1(void)
{
  const uint8_t *in = (const uint8_t *)"\xff\xff\xff\xff\xff\xff\xff\xff";
  const size_t in_len = 8;
  size_t out_len = 1;
  uint64_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u64(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u64_2(void)
{
  const uint8_t *in = (const uint8_t *)"\x09\x00\x00\x00\x00\x00\x00\x00";
  const size_t in_len = 8;
  size_t out_len = 4;
  uint64_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u64(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u64_3(void)
{
  const uint8_t *in = (const uint8_t *)"\x04\x00\x00\x00\x00\x00\x00\x0e";
  const size_t in_len = 8;
  size_t out_len = vtenc_list_decoded_size_u64(in, in_len);
  uint64_t out[out_len];

  EXPECT_TRUE(vtenc_list_decode_u64(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_list_decode_u64_4(void)
{
  const uint8_t *in = (const uint8_t *)"\x04\x00\x00\x00\x00\x00\x00\x48\x92\x24"
    "\x49\x92\x24\xc9\xfd\xff\xff\xaf\xaa\xda\xff\x56\xf3\x4a\x04\x0f\xd0\x93\x16"
    "\x79\x5b\x8e\x38\xa6\xaa";
  const size_t in_len = 35;
  size_t out_len = vtenc_list_decoded_size_u64(in, in_len);
  uint64_t out[out_len];
  uint64_t expected[] = {0x12a6, 0x8addf0, 0xffa1b4bb, 0x21258ee39aaa};

  EXPECT_TRUE(vtenc_list_decode_u64(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 4);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_list_decode_u64_5(void)
{
  const uint8_t *in = (const uint8_t *)"\x05\x00\x00\x00\x00\x00\x00\xda\xb6\x6d"
    "\xdb\xb6\x6d\xdb\xee\xff\xff\x7f\x66\x66\x66\x66\x66\x66\x66\x46\x45\x45\x45"
    "\x45\x45\x45\x45\x45\x45\x45\x05";
  const size_t in_len = 37;
  size_t out_len = vtenc_list_decoded_size_u64(in, in_len);
  uint64_t out[out_len];
  uint64_t expected[] = {0x55555555, 0x55555555, 0x55555555, 0x111111111111, 0x111111111111};

  EXPECT_TRUE(vtenc_list_decode_u64(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 5);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u8_1(void)
{
  const uint8_t *in = (const uint8_t *)"\x00";
  const size_t in_len = 1;
  size_t out_len = 0;
  uint8_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u8(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u8_2(void)
{
  const uint8_t *in = (const uint8_t *)"\x03\xff";
  const size_t in_len = 2;
  size_t out_len = vtenc_set_decoded_size_u8(in, in_len);
  uint8_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u8(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u8_3(void)
{
  const uint8_t *in = (const uint8_t *)"\xff";
  const size_t in_len = 1;
  size_t out_len = vtenc_set_decoded_size_u8(in, in_len);
  uint8_t out[out_len];
  size_t expected_len = 256;
  uint8_t expected[expected_len];
  size_t i;

  for (i=0; i<expected_len; ++i) expected[i] = (uint8_t)i;

  EXPECT_TRUE(vtenc_set_decode_u8(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == expected_len);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u8_4(void)
{
  const uint8_t *in = (const uint8_t *)"\x0b\x88\x88\x48\x90\x04";
  const size_t in_len = 6;
  size_t out_len = vtenc_set_decoded_size_u8(in, in_len);
  uint8_t out[out_len];
  uint8_t expected[] = {0, 1, 2, 3, 4, 5, 6, 7, 160, 161, 162, 163};

  EXPECT_TRUE(vtenc_set_decode_u8(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 12);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u8_5(void)
{
  const uint8_t *in = (const uint8_t *)"\x05\xcc\x74\x44\x57\x55\x03";
  const size_t in_len = 7;
  size_t out_len = vtenc_set_decoded_size_u8(in, in_len);
  uint8_t out[out_len];
  uint8_t expected[] = {13, 77, 88, 93, 149, 212};

  EXPECT_TRUE(vtenc_set_decode_u8(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 6);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u16_1(void)
{
  const uint8_t *in = (const uint8_t *)"\x00\x00";
  const size_t in_len = 2;
  size_t out_len = 0;
  uint16_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u16(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u16_2(void)
{
  const uint8_t *in = (const uint8_t *)"\x03\x00\xff";
  const size_t in_len = 3;
  size_t out_len = vtenc_set_decoded_size_u16(in, in_len);
  uint16_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u16(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u16_3(void)
{
  const uint8_t *in = (const uint8_t *)"\x07\x00\xc6\x19\x23\x34\xf0\x0f\xb7\x17\x5a\x23\xd8\xfb\xf1\x5c\x51";
  const size_t in_len = 17;
  size_t out_len = vtenc_set_decoded_size_u16(in, in_len);
  uint16_t out[out_len];
  uint16_t expected[] = {1655, 3391, 4111, 8770, 29006, 32712, 32993, 58042};

  EXPECT_TRUE(vtenc_set_decode_u16(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 8);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u16_4(void)
{
  const uint8_t *in = (const uint8_t *)"\x0b\x00\x8c\x00\x08\x80\x80\x00\x48\x02\x20\x09\x92";
  const size_t in_len = 13;
  size_t out_len = vtenc_set_decoded_size_u16(in, in_len);
  uint16_t out[out_len];
  uint16_t expected[] = {
    14000, 14001, 14002, 14003, 14004, 14005, 14006, 14007,
    20000, 20001, 20002, 20003
  };

  EXPECT_TRUE(vtenc_set_decode_u16(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 12);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u32_1(void)
{
  const uint8_t *in = (const uint8_t *)"\x00\x00\x00\x00";
  const size_t in_len = 4;
  size_t out_len = 0;
  uint32_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u32(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u32_2(void)
{
  const uint8_t *in = (const uint8_t *)"\x03\x00\x00\x00\xff";
  const size_t in_len = 5;
  size_t out_len = vtenc_set_decoded_size_u32(in, in_len);
  uint32_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u32(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u32_3(void)
{
  const uint8_t *in = (const uint8_t *)"\x04\x00\x00\x00\x9c\xbd\xb9\xec\xf7\x2e"
    "\xa8\x48\x0b\x47\x46\x78\x82\x59\xc5\xa1\xfd\xf9\x26\x02";
  const size_t in_len = 24;
  size_t out_len = vtenc_set_decoded_size_u32(in, in_len);
  uint32_t out[out_len];
  uint32_t expected[] = {0x98b204, 0x122fabb4, 0x378ecef0, 0x77ccab8f, 0xa40609bb};

  EXPECT_TRUE(vtenc_set_decode_u32(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 5);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u32_4(void)
{
  const uint8_t *in = (const uint8_t *)
  "\x0d\x00\x00\x00\xee\xee\xee\xee\x46\x92\x24\x41\x90\x20\x48\x92"
  "\x54\x04\x04\x04\x55\x15\x00\x00\x00\x00\x00\x00\x00\x10\x11\x11";
  const size_t in_len = 32;
  size_t out_len = vtenc_set_decoded_size_u32(in, in_len);
  uint32_t out[out_len];
  uint32_t expected[] = {
    0xa500, 0xa501, 0xa502, 0xa503,
    0x4bbb00, 0x4bbb01,
    0xffff00, 0xffff01, 0xffff02, 0xffff03, 0xffff04, 0xffff05, 0xffff06, 0xffff07
  };

  EXPECT_TRUE(vtenc_set_decode_u32(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 14);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u64_1(void)
{
  const uint8_t *in = (const uint8_t *)"\xff\xff\xff\xff\xff\xff\xff\xff";
  const size_t in_len = 8;
  size_t out_len = 0;
  uint64_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u64(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u64_2(void)
{
  const uint8_t *in = (const uint8_t *)"\x00\x00\x00\x00\x00\x00\x00\x00";
  const size_t in_len = 8;
  size_t out_len = 0;
  uint64_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u64(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u64_3(void)
{
  const uint8_t *in = (const uint8_t *)"\x03\x00\x00\x00\x00\x00\x00\xfe";
  const size_t in_len = 8;
  size_t out_len = vtenc_set_decoded_size_u64(in, in_len);
  uint64_t out[out_len];

  EXPECT_TRUE(vtenc_set_decode_u64(in, in_len, out, out_len) == VtencErrorWrongFormat);

  return 1;
}

int test_vtenc_set_decode_u64_4(void)
{
  const uint8_t *in = (const uint8_t *)
    "\x06\x00\x00\x00\x00\x00\x00\xfe\xff\xff\xff\xff\xff\xdb\xb6\x6d"
    "\xb9\x55\x6d\xb7\x3b\xd3\x5d\x15\x61\x26\xfa\xfb\xfb\xfb\xfb\xaa"
    "\xaa\xaa\xaa\x0a\x0f\x0f\x0f\x0f\x5f\xe6\x1e\x91\x59\xd5\x0d\x20"
    "\x02\x20\x02\x20\x02";
  const size_t in_len = 53;
  size_t out_len = vtenc_set_decoded_size_u64(in, in_len);
  uint64_t out[out_len];
  uint64_t expected[] = {
    0x11223344ULL, 0xaabbccddULL, 0x1010101010ULL, 0x5555555555ULL, 0xf0f0f0f0f0ULL,
    0x998877665544ULL, 0xffeeffeeffeeULL
  };

  EXPECT_TRUE(vtenc_set_decode_u64(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 7);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_set_decode_u64_5(void)
{
  const uint8_t *in = (const uint8_t *)
    "\x07\x00\x00\x00\x00\x00\x00\x10\x11\x11\x11\x11\x11\x11\x11\x11"
    "\x11\x11\x11\x11\xcd\xb6\x4d\x82\x24\x49\x92\x24\x49\x92\x24\x49"
    "\x92\xa4\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
    "\xaa\xaa\x02";
  const size_t in_len = 51;
  size_t out_len = vtenc_set_decoded_size_u64(in, in_len);
  uint64_t out[out_len];
  uint64_t expected[] = {
    0x20000000ULL, 0x20000001ULL, 0x20000002ULL, 0x20000003ULL,
    0x80000000ULL, 0x80000001ULL,
    0x2000000000ULL, 0x2000000001ULL
  };

  EXPECT_TRUE(vtenc_set_decode_u64(in, in_len, out, out_len) == VtencErrorNoError);
  EXPECT_TRUE(out_len == 8);
  EXPECT_TRUE(memcmp(out, expected, sizeof(out)) == 0);

  return 1;
}

int test_vtenc_list_decoded_size_1(void)
{
  const uint8_t *in = (const uint8_t *)"";
  const size_t in_len = 0;

  EXPECT_TRUE(vtenc_list_decoded_size_u8(in, in_len) == 0);
  EXPECT_TRUE(vtenc_list_decoded_size_u16(in, in_len) == 0);
  EXPECT_TRUE(vtenc_list_decoded_size_u32(in, in_len) == 0);
  EXPECT_TRUE(vtenc_list_decoded_size_u64(in, in_len) == 0);

  return 1;
}

int test_vtenc_list_decoded_size_2(void)
{
  const uint8_t *in = (const uint8_t *)"\xff\xff";
  const size_t in_len = 1;

  EXPECT_TRUE(vtenc_list_decoded_size_u8(in, in_len) == 0);
  EXPECT_TRUE(vtenc_list_decoded_size_u16(in, in_len) == 0);
  EXPECT_TRUE(vtenc_list_decoded_size_u32(in, in_len) == 0);
  EXPECT_TRUE(vtenc_list_decoded_size_u64(in, in_len) == 0);

  return 1;
}

int test_vtenc_list_decoded_size_3(void)
{
  const uint8_t *in = (const uint8_t *)"\x07\x00\x00\x00\x00\x00\x00\x00";
  const size_t in_len = 8;

  EXPECT_TRUE(vtenc_list_decoded_size_u8(in, in_len) == 7);
  EXPECT_TRUE(vtenc_list_decoded_size_u16(in, in_len) == 7);
  EXPECT_TRUE(vtenc_list_decoded_size_u32(in, in_len) == 7);
  EXPECT_TRUE(vtenc_list_decoded_size_u64(in, in_len) == 7);

  return 1;
}

int test_vtenc_list_decoded_size_4(void)
{
  const uint8_t *in = (const uint8_t *)"\xff\xff\x0f\x00\x00\x00\x00\x00";
  const size_t in_len = 8;

  EXPECT_TRUE(vtenc_list_decoded_size_u8(in, in_len) == 1048575);
  EXPECT_TRUE(vtenc_list_decoded_size_u16(in, in_len) == 1048575);
  EXPECT_TRUE(vtenc_list_decoded_size_u32(in, in_len) == 1048575);
  EXPECT_TRUE(vtenc_list_decoded_size_u64(in, in_len) == 1048575);

  return 1;
}

int test_vtenc_set_decoded_size_with_wrong_input(void)
{
  const uint8_t *in = (const uint8_t *)"";
  const size_t in_len = 0;

  EXPECT_TRUE(vtenc_set_decoded_size_u8(in, in_len) == 0);
  EXPECT_TRUE(vtenc_set_decoded_size_u16(in, in_len) == 0);
  EXPECT_TRUE(vtenc_set_decoded_size_u32(in, in_len) == 0);
  EXPECT_TRUE(vtenc_set_decoded_size_u64(in, in_len) == 0);

  return 1;
}

int test_vtenc_set_decoded_size_u8(void)
{
  const uint8_t *in;
  const size_t in_len = 1;

  in = (const uint8_t *)"\x00"; EXPECT_TRUE(vtenc_set_decoded_size_u8(in, in_len) == 1);
  in = (const uint8_t *)"\x05"; EXPECT_TRUE(vtenc_set_decoded_size_u8(in, in_len) == 6);
  in = (const uint8_t *)"\x34"; EXPECT_TRUE(vtenc_set_decoded_size_u8(in, in_len) == 53);
  in = (const uint8_t *)"\xb3"; EXPECT_TRUE(vtenc_set_decoded_size_u8(in, in_len) == 180);
  in = (const uint8_t *)"\xff"; EXPECT_TRUE(vtenc_set_decoded_size_u8(in, in_len) == 256);

  return 1;
}

int test_vtenc_set_decoded_size_u16(void)
{
  const uint8_t *in;
  const size_t in_len = 2;

  in = (const uint8_t *)"\x00\x00"; EXPECT_TRUE(vtenc_set_decoded_size_u16(in, in_len) == 1);
  in = (const uint8_t *)"\x01\x01"; EXPECT_TRUE(vtenc_set_decoded_size_u16(in, in_len) == 258);
  in = (const uint8_t *)"\x25\x33"; EXPECT_TRUE(vtenc_set_decoded_size_u16(in, in_len) == 13094);
  in = (const uint8_t *)"\x00\xff"; EXPECT_TRUE(vtenc_set_decoded_size_u16(in, in_len) == 65281);
  in = (const uint8_t *)"\xff\xff"; EXPECT_TRUE(vtenc_set_decoded_size_u16(in, in_len) == 65536);

  return 1;
}

int test_vtenc_set_decoded_size_u32(void)
{
  const uint8_t *in;
  const size_t in_len = 4;

  in = (const uint8_t *)"\x00\x00\x00\x00"; EXPECT_TRUE(vtenc_set_decoded_size_u32(in, in_len) == 1);
  in = (const uint8_t *)"\x01\x01\x01\x01"; EXPECT_TRUE(vtenc_set_decoded_size_u32(in, in_len) == 16843010);
  in = (const uint8_t *)"\x55\x55\x55\x55"; EXPECT_TRUE(vtenc_set_decoded_size_u32(in, in_len) == 1431655766);
  in = (const uint8_t *)"\x00\xff\xff\x00"; EXPECT_TRUE(vtenc_set_decoded_size_u32(in, in_len) == 16776961);
  in = (const uint8_t *)"\xff\xff\xff\xff"; EXPECT_TRUE(vtenc_set_decoded_size_u32(in, in_len) == 4294967296);

  return 1;
}

int test_vtenc_set_decoded_size_u64(void)
{
  const uint8_t *in;
  const size_t in_len = 8;

  in = (const uint8_t *)"\x00\x00\x00\x00\x00\x00\x00\x00"; EXPECT_TRUE(vtenc_set_decoded_size_u64(in, in_len) == 1);
  in = (const uint8_t *)"\x01\x01\x01\x01\x01\x01\x01\x01"; EXPECT_TRUE(vtenc_set_decoded_size_u64(in, in_len) == 0x0101010101010102ULL);
  in = (const uint8_t *)"\x55\x55\x55\x55\x55\x55\x55\x55"; EXPECT_TRUE(vtenc_set_decoded_size_u64(in, in_len) == 0x0155555555555556ULL);
  in = (const uint8_t *)"\x00\xff\xff\x00\x00\xff\xff\x00"; EXPECT_TRUE(vtenc_set_decoded_size_u64(in, in_len) == 0x00ffff0000ffff01ULL);
  in = (const uint8_t *)"\xff\xff\xff\xff\xff\xff\xff\xff"; EXPECT_TRUE(vtenc_set_decoded_size_u64(in, in_len) == 0x0200000000000000ULL);

  return 1;
}
