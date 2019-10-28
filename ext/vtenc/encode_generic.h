/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "bitstream.h"
#include "common.h"
#include "error.h"
#include "internals.h"

#define EncodeCtx(_width_) PASTE2(EncodeCtx, _width_)
#define encode_ctx_init(_width_) ADD_UINT_SUFFIX(encode_ctx_init, _width_)
#define encode_ctx_close(_width_) ADD_UINT_SUFFIX(encode_ctx_close, _width_)
#define count_zeros_at_bit_pos(_width_) ADD_UINT_SUFFIX(count_zeros_at_bit_pos, _width_)
#define encode_bits_tree(_width_) ADD_UINT_SUFFIX(encode_bits_tree, _width_)
#define list_write_cardinality(_width_) ADD_UINT_SUFFIX(list_write_cardinality, _width_)
#define set_write_cardinality(_width_) ADD_UINT_SUFFIX(set_write_cardinality, _width_)
#define vtenc_list_encode(_width_) ADD_UINT_SUFFIX(vtenc_list_encode, _width_)
#define vtenc_set_encode(_width_) ADD_UINT_SUFFIX(vtenc_set_encode, _width_)
#define vtenc_list_max_encoded_size(_width_) ADD_UINT_SUFFIX(vtenc_list_max_encoded_size, _width_)
#define vtenc_set_max_encoded_size(_width_) ADD_UINT_SUFFIX(vtenc_set_max_encoded_size, _width_)

struct EncodeCtx(WIDTH) {
  const TYPE *values;
  size_t values_len;
  unsigned int skip_full_subtrees;
  BSWriter bits_writer;
};

static VtencErrorCode encode_ctx_init(WIDTH)(struct EncodeCtx(WIDTH) *ctx,
  const TYPE *in, size_t in_len, uint8_t *out, size_t out_cap)
{
  ctx->values = in;
  ctx->values_len = in_len;
  ctx->skip_full_subtrees = 0;
  return bswriter_init(&(ctx->bits_writer), out, out_cap);
}

static size_t encode_ctx_close(WIDTH)(struct EncodeCtx(WIDTH) *ctx)
{
  return bswriter_close(&(ctx->bits_writer));
}

static size_t count_zeros_at_bit_pos(WIDTH)(const TYPE *values,
  size_t values_len, unsigned int bit_pos)
{
  const TYPE mask = (const TYPE)BITS_POS_MASK[bit_pos];
  size_t l = 0;
  size_t r = values_len;
  size_t m, i;

  do {
    m = (r - l) / 2;
    i = l + m;
    if((values[i] & mask) == 0) {
      l = i;
    } else {
      r = i;
    }
  } while (m > 0);

  return r;
}

static VtencErrorCode encode_bits_tree(WIDTH)(struct EncodeCtx(WIDTH) *ctx,
  unsigned int bit_pos, size_t idx_from, size_t idx_to)
{
  const TYPE *values_chunk = ctx->values + idx_from;
  const size_t values_chunk_len = idx_to - idx_from;
  size_t n_zeros;
  unsigned int enc_len;

  assert(bit_pos <= MAX_BIT_POS(WIDTH));
  assert(idx_from <= idx_to);
  assert(idx_to <= ctx->values_len);

  if (values_chunk_len == 0) return VtencErrorNoError;

  if (ctx->skip_full_subtrees && is_full_subtree(values_chunk_len, bit_pos))
    return VtencErrorNoError;

  n_zeros = count_zeros_at_bit_pos(WIDTH)(values_chunk, values_chunk_len, bit_pos);
  enc_len = bits_len_u64(values_chunk_len);

  RETURN_IF_ERROR(bswriter_write(&(ctx->bits_writer), n_zeros, enc_len));

  if (bit_pos == 0) return VtencErrorNoError;

  RETURN_IF_ERROR(encode_bits_tree(WIDTH)(ctx, bit_pos - 1, idx_from, idx_from + n_zeros));
  return encode_bits_tree(WIDTH)(ctx, bit_pos - 1, idx_from + n_zeros, idx_to);
}

static VtencErrorCode list_write_cardinality(WIDTH)(struct EncodeCtx(WIDTH) *ctx)
{
  return bswriter_write(&(ctx->bits_writer), ctx->values_len, LIST_CARDINALITY_SIZE);
}

static VtencErrorCode set_write_cardinality(WIDTH)(struct EncodeCtx(WIDTH) *ctx)
{
  return bswriter_write(&(ctx->bits_writer), ctx->values_len - 1, SET_CARDINALITY_SIZE);
}

VtencErrorCode vtenc_list_encode(WIDTH)(const TYPE *in, size_t in_len,
  uint8_t *out, size_t out_cap, size_t *out_len)
{
  struct EncodeCtx(WIDTH) ctx;

  if ((uint64_t)in_len > LIST_MAX_VALUES) return VtencErrorInputTooBig;

  RETURN_IF_ERROR(encode_ctx_init(WIDTH)(&ctx, in, in_len, out, out_cap));

  RETURN_IF_ERROR(list_write_cardinality(WIDTH)(&ctx));

  RETURN_IF_ERROR(encode_bits_tree(WIDTH)(&ctx, MAX_BIT_POS(WIDTH), 0, in_len));

  *out_len = encode_ctx_close(WIDTH)(&ctx);

  return VtencErrorNoError;
}

VtencErrorCode vtenc_set_encode(WIDTH)(const TYPE *in, size_t in_len,
  uint8_t *out, size_t out_cap, size_t *out_len)
{
  struct EncodeCtx(WIDTH) ctx;

  if (in_len == 0) return VtencErrorInputTooSmall;
  if ((uint64_t)in_len > SET_MAX_VALUES) return VtencErrorInputTooBig;

  RETURN_IF_ERROR(encode_ctx_init(WIDTH)(&ctx, in, in_len, out, out_cap));

  ctx.skip_full_subtrees = 1;

  RETURN_IF_ERROR(set_write_cardinality(WIDTH)(&ctx));

  RETURN_IF_ERROR(encode_bits_tree(WIDTH)(&ctx, MAX_BIT_POS(WIDTH), 0, in_len));

  *out_len = encode_ctx_close(WIDTH)(&ctx);

  return VtencErrorNoError;
}

size_t vtenc_list_max_encoded_size(WIDTH)(size_t in_len)
{
  return bswriter_align_buffer_size((LIST_CARDINALITY_SIZE / 8) + (WIDTH / 8) * (in_len + 1));
}

size_t vtenc_set_max_encoded_size(WIDTH)(size_t in_len)
{
  return bswriter_align_buffer_size((SET_CARDINALITY_SIZE / 8) + (WIDTH / 8) * (in_len + 1));
}
