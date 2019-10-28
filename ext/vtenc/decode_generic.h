/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "bitstream.h"
#include "common.h"
#include "error.h"
#include "internals.h"

#define DecodeCtx(_width_) PASTE2(DecodeCtx, _width_)
#define decode_ctx_init(_width_) ADD_UINT_SUFFIX(decode_ctx_init, _width_)
#define decode_full_subtree(_width_) ADD_UINT_SUFFIX(decode_full_subtree, _width_)
#define set_ones_at_bit_pos(_width_) ADD_UINT_SUFFIX(set_ones_at_bit_pos, _width_)
#define decode_bits_tree(_width_) ADD_UINT_SUFFIX(decode_bits_tree, _width_)
#define list_read_cardinality(_width_) ADD_UINT_SUFFIX(list_read_cardinality, _width_)
#define set_read_cardinality(_width_) ADD_UINT_SUFFIX(set_read_cardinality, _width_)
#define vtenc_list_decode(_width_) ADD_UINT_SUFFIX(vtenc_list_decode, _width_)
#define vtenc_set_decode(_width_) ADD_UINT_SUFFIX(vtenc_set_decode, _width_)
#define vtenc_list_decoded_size(_width_) ADD_UINT_SUFFIX(vtenc_list_decoded_size, _width_)
#define vtenc_set_decoded_size(_width_) ADD_UINT_SUFFIX(vtenc_set_decoded_size, _width_)

struct DecodeCtx(WIDTH) {
  TYPE *values;
  size_t values_len;
  unsigned int reconstruct_full_subtrees;
  BSReader bits_reader;
};

static VtencErrorCode decode_ctx_init(WIDTH)(struct DecodeCtx(WIDTH) *ctx,
  const uint8_t *in, size_t in_len, TYPE *out, size_t out_len)
{
  ctx->values = out;
  ctx->values_len = out_len;
  ctx->reconstruct_full_subtrees = 0;
  return bsreader_init(&(ctx->bits_reader), in, in_len);
}

static void decode_full_subtree(WIDTH)(TYPE *values, size_t values_len)
{
  size_t i;

  for (i = 0; i < values_len; ++i) {
    values[i] |= (TYPE)i;
  }
}

static void set_ones_at_bit_pos(WIDTH)(TYPE *values,
  size_t values_len, unsigned int bit_pos)
{
  int i;
  const TYPE mask = (const TYPE)BITS_POS_MASK[bit_pos];

  for (i = 0; i < values_len; ++i) {
    values[i] |= mask;
  }
}

static VtencErrorCode decode_bits_tree(WIDTH)(struct DecodeCtx(WIDTH) *ctx,
  unsigned int bit_pos, size_t idx_from, size_t idx_to)
{
  TYPE *values_chunk = ctx->values + idx_from;
  size_t values_chunk_len = idx_to - idx_from;
  unsigned int enc_len;
  uint64_t n_zeros;

  assert(bit_pos <= MAX_BIT_POS(WIDTH));
  assert(idx_from <= idx_to);
  assert(idx_to <= ctx->values_len);

  if (values_chunk_len == 0) return VtencErrorNoError;

  if (ctx->reconstruct_full_subtrees && is_full_subtree(values_chunk_len, bit_pos)) {
    decode_full_subtree(WIDTH)(values_chunk, values_chunk_len);
    return VtencErrorNoError;
  }

  enc_len = bits_len_u64(values_chunk_len);

  RETURN_IF_ERROR(bsreader_read(&(ctx->bits_reader), enc_len, &n_zeros));

  if (n_zeros > (uint64_t)values_chunk_len) return VtencErrorWrongFormat;

  set_ones_at_bit_pos(WIDTH)(values_chunk + n_zeros, values_chunk_len - n_zeros, bit_pos);

  if (bit_pos == 0) return VtencErrorNoError;

  RETURN_IF_ERROR(decode_bits_tree(WIDTH)(ctx, bit_pos - 1, idx_from, idx_from + n_zeros));
  return decode_bits_tree(WIDTH)(ctx, bit_pos - 1, idx_from + n_zeros, idx_to);
}

static VtencErrorCode list_read_cardinality(WIDTH)(BSReader *reader, uint64_t *cardinality)
{
  return bsreader_read(reader, LIST_CARDINALITY_SIZE, cardinality);
}

static VtencErrorCode set_read_cardinality(WIDTH)(BSReader *reader, uint64_t *cardinality)
{
  RETURN_IF_ERROR(bsreader_read(reader, SET_CARDINALITY_SIZE, cardinality));
  ++(*cardinality);
  return VtencErrorNoError;
}

VtencErrorCode vtenc_list_decode(WIDTH)(const uint8_t *in, size_t in_len, TYPE *out, size_t out_len)
{
  struct DecodeCtx(WIDTH) ctx;
  uint64_t cardinality;

  memset(out, 0, out_len * sizeof(*out));

  RETURN_IF_ERROR(decode_ctx_init(WIDTH)(&ctx, in, in_len, out, out_len));

  RETURN_IF_ERROR(list_read_cardinality(WIDTH)(&(ctx.bits_reader), &cardinality));

  if (cardinality > LIST_MAX_VALUES) return VtencErrorWrongFormat;
  if (cardinality != (uint64_t)out_len) return VtencErrorWrongFormat;

  RETURN_IF_ERROR(decode_bits_tree(WIDTH)(&ctx, MAX_BIT_POS(WIDTH), 0, out_len));

  return VtencErrorNoError;
}

VtencErrorCode vtenc_set_decode(WIDTH)(const uint8_t *in, size_t in_len, TYPE *out, size_t out_len)
{
  struct DecodeCtx(WIDTH) ctx;
  uint64_t cardinality;

  memset(out, 0, out_len * sizeof(*out));

  RETURN_IF_ERROR(decode_ctx_init(WIDTH)(&ctx, in, in_len, out, out_len));

  ctx.reconstruct_full_subtrees = 1;

  RETURN_IF_ERROR(set_read_cardinality(WIDTH)(&(ctx.bits_reader), &cardinality));

  if (cardinality > SET_MAX_VALUES) return VtencErrorWrongFormat;
  if (cardinality != (uint64_t)out_len) return VtencErrorWrongFormat;

  RETURN_IF_ERROR(decode_bits_tree(WIDTH)(&ctx, MAX_BIT_POS(WIDTH), 0, out_len));

  return VtencErrorNoError;
}

size_t vtenc_list_decoded_size(WIDTH)(const uint8_t *in, size_t in_len)
{
  BSReader reader;
  size_t dec_len;

  RETURN_ZERO_IF_ERROR(bsreader_init(&reader, in, in_len));
  RETURN_ZERO_IF_ERROR(list_read_cardinality(WIDTH)(&reader, &dec_len));

  return dec_len;
}

size_t vtenc_set_decoded_size(WIDTH)(const uint8_t *in, size_t in_len)
{
  BSReader reader;
  size_t dec_len;

  RETURN_ZERO_IF_ERROR(bsreader_init(&reader, in, in_len));
  RETURN_ZERO_IF_ERROR(set_read_cardinality(WIDTH)(&reader, &dec_len));

  return dec_len;
}
