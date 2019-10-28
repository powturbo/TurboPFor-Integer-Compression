/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include "unit_tests.h"

#include <stdio.h>

#define RUN_TEST(fn_name)           \
do {                                \
  printf(""#fn_name"...");          \
  if ((fn_name)()) printf(" OK\n"); \
  else printf(" KO\n");             \
} while (0)

int main()
{
  RUN_TEST(test_bits_swap_u16);
  RUN_TEST(test_bits_swap_u32);
  RUN_TEST(test_bits_swap_u64);

  RUN_TEST(test_bits_len_u8);
  RUN_TEST(test_bits_len_u16);
  RUN_TEST(test_bits_len_u32);
  RUN_TEST(test_bits_len_u64);

  RUN_TEST(test_little_endian_read_and_write_u16);
  RUN_TEST(test_little_endian_read_and_write_u32);
  RUN_TEST(test_little_endian_read_and_write_u64);

  RUN_TEST(test_bswriter_align_buffer_size);
  RUN_TEST(test_bswriter_init_1);
  RUN_TEST(test_bswriter_init_2);
  RUN_TEST(test_bswriter_write_1);
  RUN_TEST(test_bswriter_write_2);
  RUN_TEST(test_bswriter_write_3);
  RUN_TEST(test_bswriter_close_1);
  RUN_TEST(test_bswriter_close_2);

  RUN_TEST(test_bsreader_init_1);
  RUN_TEST(test_bsreader_init_2);
  RUN_TEST(test_bsreader_read_1);
  RUN_TEST(test_bsreader_read_2);
  RUN_TEST(test_bsreader_read_3);
  RUN_TEST(test_bsreader_read_4);
  RUN_TEST(test_bsreader_read_5);
  RUN_TEST(test_bsreader_read_6);
  RUN_TEST(test_bsreader_size);

  RUN_TEST(test_vtenc_list_encode_u8_1);
  RUN_TEST(test_vtenc_list_encode_u8_2);
  RUN_TEST(test_vtenc_list_encode_u8_3);
  RUN_TEST(test_vtenc_list_encode_u16_1);
  RUN_TEST(test_vtenc_list_encode_u16_2);
  RUN_TEST(test_vtenc_list_encode_u16_3);
  RUN_TEST(test_vtenc_list_encode_u32_1);
  RUN_TEST(test_vtenc_list_encode_u32_2);
  RUN_TEST(test_vtenc_list_encode_u32_3);
  RUN_TEST(test_vtenc_list_encode_u64_1);
  RUN_TEST(test_vtenc_list_encode_u64_2);
  RUN_TEST(test_vtenc_list_encode_u64_3);

  RUN_TEST(test_vtenc_set_encode_u8_1);
  RUN_TEST(test_vtenc_set_encode_u8_2);
  RUN_TEST(test_vtenc_set_encode_u8_3);
  RUN_TEST(test_vtenc_set_encode_u8_4);
  RUN_TEST(test_vtenc_set_encode_u8_5);
  RUN_TEST(test_vtenc_set_encode_u8_6);
  RUN_TEST(test_vtenc_set_encode_u16_1);
  RUN_TEST(test_vtenc_set_encode_u16_2);
  RUN_TEST(test_vtenc_set_encode_u16_3);
  RUN_TEST(test_vtenc_set_encode_u16_4);
  RUN_TEST(test_vtenc_set_encode_u32_1);
  RUN_TEST(test_vtenc_set_encode_u32_2);
  RUN_TEST(test_vtenc_set_encode_u32_3);
  RUN_TEST(test_vtenc_set_encode_u32_4);
  RUN_TEST(test_vtenc_set_encode_u64_1);
  RUN_TEST(test_vtenc_set_encode_u64_2);
  RUN_TEST(test_vtenc_set_encode_u64_3);

  RUN_TEST(test_vtenc_list_max_encoded_size_u8);
  RUN_TEST(test_vtenc_list_max_encoded_size_u16);
  RUN_TEST(test_vtenc_list_max_encoded_size_u32);
  RUN_TEST(test_vtenc_list_max_encoded_size_u64);

  RUN_TEST(test_vtenc_set_max_encoded_size_u8);
  RUN_TEST(test_vtenc_set_max_encoded_size_u16);
  RUN_TEST(test_vtenc_set_max_encoded_size_u32);
  RUN_TEST(test_vtenc_set_max_encoded_size_u64);

  RUN_TEST(test_vtenc_list_decode_u8_1);
  RUN_TEST(test_vtenc_list_decode_u8_2);
  RUN_TEST(test_vtenc_list_decode_u8_3);
  RUN_TEST(test_vtenc_list_decode_u8_4);
  RUN_TEST(test_vtenc_list_decode_u8_5);
  RUN_TEST(test_vtenc_list_decode_u16_1);
  RUN_TEST(test_vtenc_list_decode_u16_2);
  RUN_TEST(test_vtenc_list_decode_u16_3);
  RUN_TEST(test_vtenc_list_decode_u16_4);
  RUN_TEST(test_vtenc_list_decode_u16_5);
  RUN_TEST(test_vtenc_list_decode_u32_1);
  RUN_TEST(test_vtenc_list_decode_u32_2);
  RUN_TEST(test_vtenc_list_decode_u32_3);
  RUN_TEST(test_vtenc_list_decode_u32_4);
  RUN_TEST(test_vtenc_list_decode_u32_5);
  RUN_TEST(test_vtenc_list_decode_u64_1);
  RUN_TEST(test_vtenc_list_decode_u64_2);
  RUN_TEST(test_vtenc_list_decode_u64_3);
  RUN_TEST(test_vtenc_list_decode_u64_4);
  RUN_TEST(test_vtenc_list_decode_u64_5);

  RUN_TEST(test_vtenc_set_decode_u8_1);
  RUN_TEST(test_vtenc_set_decode_u8_2);
  RUN_TEST(test_vtenc_set_decode_u8_3);
  RUN_TEST(test_vtenc_set_decode_u8_4);
  RUN_TEST(test_vtenc_set_decode_u8_5);
  RUN_TEST(test_vtenc_set_decode_u16_1);
  RUN_TEST(test_vtenc_set_decode_u16_2);
  RUN_TEST(test_vtenc_set_decode_u16_3);
  RUN_TEST(test_vtenc_set_decode_u16_4);
  RUN_TEST(test_vtenc_set_decode_u32_1);
  RUN_TEST(test_vtenc_set_decode_u32_2);
  RUN_TEST(test_vtenc_set_decode_u32_3);
  RUN_TEST(test_vtenc_set_decode_u32_4);
  RUN_TEST(test_vtenc_set_decode_u64_1);
  RUN_TEST(test_vtenc_set_decode_u64_2);
  RUN_TEST(test_vtenc_set_decode_u64_3);
  RUN_TEST(test_vtenc_set_decode_u64_4);
  RUN_TEST(test_vtenc_set_decode_u64_5);

  RUN_TEST(test_vtenc_list_decoded_size_1);
  RUN_TEST(test_vtenc_list_decoded_size_2);
  RUN_TEST(test_vtenc_list_decoded_size_3);
  RUN_TEST(test_vtenc_list_decoded_size_4);

  RUN_TEST(test_vtenc_set_decoded_size_with_wrong_input);
  RUN_TEST(test_vtenc_set_decoded_size_u8);
  RUN_TEST(test_vtenc_set_decoded_size_u16);
  RUN_TEST(test_vtenc_set_decoded_size_u32);
  RUN_TEST(test_vtenc_set_decoded_size_u64);

  return 0;
}
