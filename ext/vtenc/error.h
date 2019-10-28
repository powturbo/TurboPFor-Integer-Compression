/**
  Copyright (c) 2019 Vicente Romero Calero. All rights reserved.
  Licensed under the MIT License.
  See LICENSE file in the project root for full license information.
 */
#ifndef VTENC_ERROR_H_
#define VTENC_ERROR_H_

#include "vtenc.h"

#define RETURN_IF_ERROR(exp)          \
do {                                  \
  const VtencErrorCode code = (exp);  \
  if (code != VtencErrorNoError)      \
    return code;                      \
} while(0)

#define RETURN_ZERO_IF_ERROR(exp)     \
do {                                  \
  const VtencErrorCode code = (exp);  \
  if (code != VtencErrorNoError)      \
    return 0;                         \
} while(0)

#endif /* VTENC_ERROR_H_ */
