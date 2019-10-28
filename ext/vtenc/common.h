/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#ifndef VTENC_COMMON_H_
#define VTENC_COMMON_H_

#include <stddef.h>
#include <stdint.h>

#include "bits.h"

static unsigned int is_full_subtree(size_t values_len, unsigned int bit_pos)
{
  return ((uint64_t)values_len == BITS_POS_MASK[bit_pos + 1]);
}

#endif /* VTENC_COMMON_H_ */
