/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#include "vtenc.h"

#include "internals.h"

#define LIST_MAX_VALUES VTENC_LIST_MAX_VALUES
#define LIST_CARDINALITY_SIZE VTENC_LIST_CARDINALITY_SIZE

#define TYPE uint8_t
#define WIDTH 8
#define SET_MAX_VALUES VTENC_SET_MAX_VALUES_U8
#define SET_CARDINALITY_SIZE VTENC_SET_CARDINALITY_SIZE_U8
#include "decode_generic.h"
#undef TYPE
#undef WIDTH
#undef SET_MAX_VALUES
#undef SET_CARDINALITY_SIZE

#define TYPE uint16_t
#define WIDTH 16
#define SET_MAX_VALUES VTENC_SET_MAX_VALUES_U16
#define SET_CARDINALITY_SIZE VTENC_SET_CARDINALITY_SIZE_U16
#include "decode_generic.h"
#undef TYPE
#undef WIDTH
#undef SET_MAX_VALUES
#undef SET_CARDINALITY_SIZE

#define TYPE uint32_t
#define WIDTH 32
#define SET_MAX_VALUES VTENC_SET_MAX_VALUES_U32
#define SET_CARDINALITY_SIZE VTENC_SET_CARDINALITY_SIZE_U32
#include "decode_generic.h"
#undef TYPE
#undef WIDTH
#undef SET_MAX_VALUES
#undef SET_CARDINALITY_SIZE

#define TYPE uint64_t
#define WIDTH 64
#define SET_MAX_VALUES VTENC_SET_MAX_VALUES_U64
#define SET_CARDINALITY_SIZE VTENC_SET_CARDINALITY_SIZE_U64
#include "decode_generic.h"
#undef TYPE
#undef WIDTH
#undef SET_MAX_VALUES
#undef SET_CARDINALITY_SIZE

#undef LIST_MAX_VALUES
#undef LIST_CARDINALITY_SIZE
