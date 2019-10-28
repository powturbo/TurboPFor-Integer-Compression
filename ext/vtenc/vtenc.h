/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#ifndef VTENC_H_
#define VTENC_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This library provides a series of functions to encode and decode sorted
 * sequences of unsigned integers using VTEnc algorithm. It supports different
 * types (uint8_t, uint16_t, uint32_t and uint64_t) and two different kind of
 * sequences: lists and sets.
 *
 * Lists and sets differ in that lists can have duplicate values, whereas sets
 * cannot. Both have to be sorted in ascending order.
 */

/* Error codes */
typedef enum {
  VtencErrorNoError = 0,
  VtencErrorBufferTooSmall = 1,
  VtencErrorEndOfStream = 2,
  VtencErrorNotEnoughBits = 3,
  VtencErrorInputTooSmall = 4,
  VtencErrorInputTooBig = 5,
  VtencErrorWrongFormat = 6
} VtencErrorCode;

/**
 * Encoding functions.
 *
 * Each function encodes the list or set `in` into the already-allocated stream
 * of bytes `out`.
 *
 * `in_len`: size of `in`.
 * `out_cap`: capacity in bytes of `out`.
 * `out_len`: actual size of the encoded output. This is an output argument, so
 *    its value will be available after the encode function is called.
 *
 * Returns 'VtencErrorNoError' if there was no error. Otherwise, a specific error
 * code is returned.
 *
 * Note that these functions assume that `in` is a sorted sequence and they don't
 * check its order. If you pass in an unsorted list or set, they'll still
 * return a 'VtencErrorNoError' code, but the output won't correspond to the
 * correct encoded stream for the input sequence.
 */
VtencErrorCode vtenc_list_encode_u8(const uint8_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len);
VtencErrorCode vtenc_list_encode_u16(const uint16_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len);
VtencErrorCode vtenc_list_encode_u32(const uint32_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len);
VtencErrorCode vtenc_list_encode_u64(const uint64_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len);

VtencErrorCode vtenc_set_encode_u8(const uint8_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len);
VtencErrorCode vtenc_set_encode_u16(const uint16_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len);
VtencErrorCode vtenc_set_encode_u32(const uint32_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len);
VtencErrorCode vtenc_set_encode_u64(const uint64_t *in, size_t in_len, uint8_t *out, size_t out_cap, size_t *out_len);

/**
 * Functions to calculate the maximum encoded size in bytes when encoding a list
 * or set of size `in_len`. The returned value is an approximation of the actual
 * encoded length and it's guaranteed to be at least as big as the actual size.
 */
size_t vtenc_list_max_encoded_size_u8(size_t in_len);
size_t vtenc_list_max_encoded_size_u16(size_t in_len);
size_t vtenc_list_max_encoded_size_u32(size_t in_len);
size_t vtenc_list_max_encoded_size_u64(size_t in_len);

size_t vtenc_set_max_encoded_size_u8(size_t in_len);
size_t vtenc_set_max_encoded_size_u16(size_t in_len);
size_t vtenc_set_max_encoded_size_u32(size_t in_len);
size_t vtenc_set_max_encoded_size_u64(size_t in_len);

/**
 * Decoding functions.
 *
 * Each function decodes the stream of bytes `in` into the already-allocated
 * list or set `out`.
 *
 * `in_len`: size of `in`.
 * `out_len`: size of `out`.
 *
 * Returns 'VtencErrorNoError' in case of success. Otherwise, a specific error
 * code is returned.
 */
VtencErrorCode vtenc_list_decode_u8(const uint8_t *in, size_t in_len, uint8_t *out, size_t out_len);
VtencErrorCode vtenc_list_decode_u16(const uint8_t *in, size_t in_len, uint16_t *out, size_t out_len);
VtencErrorCode vtenc_list_decode_u32(const uint8_t *in, size_t in_len, uint32_t *out, size_t out_len);
VtencErrorCode vtenc_list_decode_u64(const uint8_t *in, size_t in_len, uint64_t *out, size_t out_len);

VtencErrorCode vtenc_set_decode_u8(const uint8_t *in, size_t in_len, uint8_t *out, size_t out_len);
VtencErrorCode vtenc_set_decode_u16(const uint8_t *in, size_t in_len, uint16_t *out, size_t out_len);
VtencErrorCode vtenc_set_decode_u32(const uint8_t *in, size_t in_len, uint32_t *out, size_t out_len);
VtencErrorCode vtenc_set_decode_u64(const uint8_t *in, size_t in_len, uint64_t *out, size_t out_len);

/**
 * Functions to extract the size of the decoded sequence from the encoded stream
 * of bytes `in` of size `in_len`. These functions are used to allocate memory
 * for the list or set to be decoded before calling the corresponding decode
 * function.
 */
size_t vtenc_list_decoded_size_u8(const uint8_t *in, size_t in_len);
size_t vtenc_list_decoded_size_u16(const uint8_t *in, size_t in_len);
size_t vtenc_list_decoded_size_u32(const uint8_t *in, size_t in_len);
size_t vtenc_list_decoded_size_u64(const uint8_t *in, size_t in_len);

size_t vtenc_set_decoded_size_u8(const uint8_t *in, size_t in_len);
size_t vtenc_set_decoded_size_u16(const uint8_t *in, size_t in_len);
size_t vtenc_set_decoded_size_u32(const uint8_t *in, size_t in_len);
size_t vtenc_set_decoded_size_u64(const uint8_t *in, size_t in_len);

#ifdef __cplusplus
}
#endif

#endif /* VTENC_H_ */
