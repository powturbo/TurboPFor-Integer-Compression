/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#ifndef VTENC_BITSTREAM_H_
#define VTENC_BITSTREAM_H_

#include <stddef.h>
#include <stdint.h>

#include "error.h"

#define BIT_STREAM_MAX_WRITE 57
#define BIT_STREAM_MAX_READ BIT_STREAM_MAX_WRITE

typedef struct {
  uint64_t bit_container;
  unsigned int bit_pos;
  uint8_t *start_ptr;
  uint8_t *ptr;
  uint8_t *end_ptr;
} BSWriter;

size_t bswriter_align_buffer_size(size_t orig_size);

VtencErrorCode bswriter_init(BSWriter *writer, uint8_t *out_buf, size_t out_capacity);
VtencErrorCode bswriter_write(BSWriter *writer, uint64_t value, unsigned int n_bits);
size_t bswriter_close(BSWriter *writer);

typedef struct {
  uint64_t bit_container;
  unsigned int bits_loaded;
  unsigned int bits_consumed;
  const uint8_t *start_ptr;
  const uint8_t *ptr;
  const uint8_t *end_ptr;
} BSReader;

VtencErrorCode bsreader_init(BSReader *reader, const uint8_t *buf, size_t buf_len);
VtencErrorCode bsreader_read(BSReader *reader, unsigned int n_bits, uint64_t *read_value);
size_t bsreader_size(BSReader *reader);

#endif /* VTENC_BITSTREAM_H_ */
