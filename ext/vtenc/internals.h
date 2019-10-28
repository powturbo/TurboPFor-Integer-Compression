/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#ifndef VTENC_INTERNALS_H_
#define VTENC_INTERNALS_H_

#include "bits.h"
#include "bitstream.h"

/* Shared macros */

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define PASTE2(x, y) x ## y
#define PASTE3(x, y, z) x ## y ## z
#define ADD_UINT_SUFFIX(_name_, _width_) PASTE3(_name_, _u, _width_)
#define MAX_BIT_POS(_width_) ((_width_) - 1)

/* VTEnc constants */

#define VTENC_LIST_CARDINALITY_SIZE   BIT_STREAM_MAX_WRITE
#define VTENC_LIST_MAX_VALUES         BITS_SIZE_MASK[BIT_STREAM_MAX_WRITE]

#define VTENC_SET_CARDINALITY_SIZE_U8   MIN(8, BIT_STREAM_MAX_WRITE)
#define VTENC_SET_MAX_VALUES_U8         BITS_POS_MASK[VTENC_SET_CARDINALITY_SIZE_U8]
#define VTENC_SET_CARDINALITY_SIZE_U16  MIN(16, BIT_STREAM_MAX_WRITE)
#define VTENC_SET_MAX_VALUES_U16        BITS_POS_MASK[VTENC_SET_CARDINALITY_SIZE_U16]
#define VTENC_SET_CARDINALITY_SIZE_U32  MIN(32, BIT_STREAM_MAX_WRITE)
#define VTENC_SET_MAX_VALUES_U32        BITS_POS_MASK[VTENC_SET_CARDINALITY_SIZE_U32]
#define VTENC_SET_CARDINALITY_SIZE_U64  MIN(64, BIT_STREAM_MAX_WRITE)
#define VTENC_SET_MAX_VALUES_U64        BITS_POS_MASK[VTENC_SET_CARDINALITY_SIZE_U64]

#endif /* VTENC_INTERNALS_H_ */
