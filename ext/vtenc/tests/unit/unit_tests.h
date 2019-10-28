/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#ifndef VTENC_UNIT_TESTS_H_
#define VTENC_UNIT_TESTS_H_

#include <stdio.h>

#define EXPECT_TRUE(test)                 \
do {                                      \
  if (!(test)) {                          \
    printf("\n\t\""#test"\" failed!\n");  \
    return 0;                             \
  }                                       \
} while(0)

int test_bits_swap_u16(void);
int test_bits_swap_u32(void);
int test_bits_swap_u64(void);

int test_bits_len_u8(void);
int test_bits_len_u16(void);
int test_bits_len_u32(void);
int test_bits_len_u64(void);

int test_little_endian_read_and_write_u16(void);
int test_little_endian_read_and_write_u32(void);
int test_little_endian_read_and_write_u64(void);

int test_bswriter_align_buffer_size(void);
int test_bswriter_init_1(void);
int test_bswriter_init_2(void);
int test_bswriter_write_1(void);
int test_bswriter_write_2(void);
int test_bswriter_write_3(void);
int test_bswriter_close_1(void);
int test_bswriter_close_2(void);

int test_bsreader_init_1(void);
int test_bsreader_init_2(void);
int test_bsreader_read_1(void);
int test_bsreader_read_2(void);
int test_bsreader_read_3(void);
int test_bsreader_read_4(void);
int test_bsreader_read_5(void);
int test_bsreader_read_6(void);
int test_bsreader_size(void);

int test_vtenc_list_encode_u8_1(void);
int test_vtenc_list_encode_u8_2(void);
int test_vtenc_list_encode_u8_3(void);
int test_vtenc_list_encode_u16_1(void);
int test_vtenc_list_encode_u16_2(void);
int test_vtenc_list_encode_u16_3(void);
int test_vtenc_list_encode_u32_1(void);
int test_vtenc_list_encode_u32_2(void);
int test_vtenc_list_encode_u32_3(void);
int test_vtenc_list_encode_u64_1(void);
int test_vtenc_list_encode_u64_2(void);
int test_vtenc_list_encode_u64_3(void);

int test_vtenc_set_encode_u8_1(void);
int test_vtenc_set_encode_u8_2(void);
int test_vtenc_set_encode_u8_3(void);
int test_vtenc_set_encode_u8_4(void);
int test_vtenc_set_encode_u8_5(void);
int test_vtenc_set_encode_u8_6(void);
int test_vtenc_set_encode_u16_1(void);
int test_vtenc_set_encode_u16_2(void);
int test_vtenc_set_encode_u16_3(void);
int test_vtenc_set_encode_u16_4(void);
int test_vtenc_set_encode_u32_1(void);
int test_vtenc_set_encode_u32_2(void);
int test_vtenc_set_encode_u32_3(void);
int test_vtenc_set_encode_u32_4(void);
int test_vtenc_set_encode_u64_1(void);
int test_vtenc_set_encode_u64_2(void);
int test_vtenc_set_encode_u64_3(void);

int test_vtenc_list_max_encoded_size_u8(void);
int test_vtenc_list_max_encoded_size_u16(void);
int test_vtenc_list_max_encoded_size_u32(void);
int test_vtenc_list_max_encoded_size_u64(void);

int test_vtenc_set_max_encoded_size_u8(void);
int test_vtenc_set_max_encoded_size_u16(void);
int test_vtenc_set_max_encoded_size_u32(void);
int test_vtenc_set_max_encoded_size_u64(void);

int test_vtenc_list_decode_u8_1(void);
int test_vtenc_list_decode_u8_2(void);
int test_vtenc_list_decode_u8_3(void);
int test_vtenc_list_decode_u8_4(void);
int test_vtenc_list_decode_u8_5(void);
int test_vtenc_list_decode_u16_1(void);
int test_vtenc_list_decode_u16_2(void);
int test_vtenc_list_decode_u16_3(void);
int test_vtenc_list_decode_u16_4(void);
int test_vtenc_list_decode_u16_5(void);
int test_vtenc_list_decode_u32_1(void);
int test_vtenc_list_decode_u32_2(void);
int test_vtenc_list_decode_u32_3(void);
int test_vtenc_list_decode_u32_4(void);
int test_vtenc_list_decode_u32_5(void);
int test_vtenc_list_decode_u64_1(void);
int test_vtenc_list_decode_u64_2(void);
int test_vtenc_list_decode_u64_3(void);
int test_vtenc_list_decode_u64_4(void);
int test_vtenc_list_decode_u64_5(void);

int test_vtenc_set_decode_u8_1(void);
int test_vtenc_set_decode_u8_2(void);
int test_vtenc_set_decode_u8_3(void);
int test_vtenc_set_decode_u8_4(void);
int test_vtenc_set_decode_u8_5(void);
int test_vtenc_set_decode_u16_1(void);
int test_vtenc_set_decode_u16_2(void);
int test_vtenc_set_decode_u16_3(void);
int test_vtenc_set_decode_u16_4(void);
int test_vtenc_set_decode_u32_1(void);
int test_vtenc_set_decode_u32_2(void);
int test_vtenc_set_decode_u32_3(void);
int test_vtenc_set_decode_u32_4(void);
int test_vtenc_set_decode_u64_1(void);
int test_vtenc_set_decode_u64_2(void);
int test_vtenc_set_decode_u64_3(void);
int test_vtenc_set_decode_u64_4(void);
int test_vtenc_set_decode_u64_5(void);

int test_vtenc_list_decoded_size_1(void);
int test_vtenc_list_decoded_size_2(void);
int test_vtenc_list_decoded_size_3(void);
int test_vtenc_list_decoded_size_4(void);

int test_vtenc_set_decoded_size_with_wrong_input(void);
int test_vtenc_set_decoded_size_u8(void);
int test_vtenc_set_decoded_size_u16(void);
int test_vtenc_set_decoded_size_u32(void);
int test_vtenc_set_decoded_size_u64(void);

#endif /* VTENC_UNIT_TESTS_H_ */
