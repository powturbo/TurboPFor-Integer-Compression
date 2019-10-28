/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include "bitstream.h"

#include <assert.h>

#include "bits.h"
#include "mem.h"

size_t bswriter_align_buffer_size(size_t orig_size)
{
  if ((orig_size > 0) && (orig_size%8 == 0)) return orig_size;
  return orig_size + 8 - (orig_size % 8);
}

VtencErrorCode bswriter_init(BSWriter *writer, uint8_t *out_buf, size_t out_capacity)
{
  writer->bit_container = 0;
  writer->bit_pos = 0;
  writer->start_ptr = out_buf;
  writer->ptr = writer->start_ptr;
  writer->end_ptr = writer->start_ptr + out_capacity - sizeof(writer->bit_container);

  if (out_capacity < sizeof(writer->bit_container)) return VtencErrorBufferTooSmall;
  return VtencErrorNoError;
}

static void bswriter_append(BSWriter *writer, uint64_t value, unsigned int n_bits)
{
  assert(n_bits <= BIT_STREAM_MAX_WRITE);
  assert(n_bits < BITS_SIZE_MASK_LEN);
  assert(n_bits + writer->bit_pos < 64);

  writer->bit_container |= (value & BITS_SIZE_MASK[n_bits]) << writer->bit_pos;
  writer->bit_pos += n_bits;
}

static VtencErrorCode bswriter_flush(BSWriter *writer)
{
  size_t const n_bytes = writer->bit_pos >> 3;

  assert(writer->bit_pos < 64);
  mem_write_le_u64(writer->ptr, writer->bit_container);

  writer->ptr += n_bytes;
  writer->bit_pos &= 7;
  writer->bit_container >>= (n_bytes << 3);

  if (writer->ptr > writer->end_ptr) return VtencErrorEndOfStream;
  return VtencErrorNoError;
}

VtencErrorCode bswriter_write(BSWriter *writer, uint64_t value, unsigned int n_bits)
{
  assert(n_bits <= BIT_STREAM_MAX_WRITE);

  if (writer->ptr > writer->end_ptr) return VtencErrorEndOfStream;

  if (n_bits + writer->bit_pos >= 64)
    RETURN_IF_ERROR(bswriter_flush(writer));

  bswriter_append(writer, value, n_bits);

  return VtencErrorNoError;
}

size_t bswriter_close(BSWriter *writer)
{
  if (writer->ptr <= writer->end_ptr)
    bswriter_flush(writer);

  return (writer->ptr - writer->start_ptr) + (writer->bit_pos > 0);
}

VtencErrorCode bsreader_init(BSReader *reader, const uint8_t *buf, size_t buf_len)
{
  reader->bit_container = 0;
  reader->bits_loaded = 0;
  reader->bits_consumed = 0;
  reader->start_ptr = buf;
  reader->ptr = reader->start_ptr;
  reader->end_ptr = reader->start_ptr + buf_len;

  if (buf_len == 0) return VtencErrorBufferTooSmall;
  return VtencErrorNoError;
}

static VtencErrorCode bsreader_load(BSReader *reader)
{
  size_t n_bytes;

  reader->ptr += reader->bits_consumed >> 3;

  assert(reader->ptr <= reader->end_ptr);
  n_bytes = reader->end_ptr - reader->ptr;

  if (n_bytes == 0) return VtencErrorEndOfStream;

  if (n_bytes >= 8) {
    reader->bit_container = mem_read_le_u64(reader->ptr);
    reader->bits_loaded = 64;
    reader->bits_consumed &= 7;
  } else {
    reader->bit_container = (uint64_t)(reader->ptr[0]);

    switch (n_bytes) {
      case 7: reader->bit_container |= (uint64_t)(reader->ptr[6]) << 48;
      case 6: reader->bit_container |= (uint64_t)(reader->ptr[5]) << 40;
      case 5: reader->bit_container |= (uint64_t)(reader->ptr[4]) << 32;
      case 4: reader->bit_container |= (uint64_t)(reader->ptr[3]) << 24;
      case 3: reader->bit_container |= (uint64_t)(reader->ptr[2]) << 16;
      case 2: reader->bit_container |= (uint64_t)(reader->ptr[1]) << 8;
      default: break;
    }

    reader->bits_loaded = n_bytes << 3;
    reader->bits_consumed &= 7;
  }

  return VtencErrorNoError;
}

VtencErrorCode bsreader_read(BSReader *reader, unsigned int n_bits, uint64_t *read_value)
{
  assert(n_bits <= BIT_STREAM_MAX_READ);
  assert(n_bits < BITS_SIZE_MASK_LEN);
  assert(reader->bits_consumed <= reader->bits_loaded);

  if (n_bits + reader->bits_consumed > reader->bits_loaded) {
    RETURN_IF_ERROR(bsreader_load(reader));

    if (n_bits + reader->bits_consumed > reader->bits_loaded)
      return VtencErrorNotEnoughBits;
  }

  *read_value = (reader->bit_container >> reader->bits_consumed) & BITS_SIZE_MASK[n_bits];
  reader->bits_consumed += n_bits;

  return VtencErrorNoError;
}

size_t bsreader_size(BSReader *reader)
{
  return (reader->ptr - reader->start_ptr) + (reader->bits_consumed >> 3) + ((reader->bits_consumed & 7) > 0);
}
