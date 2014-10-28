/**
 * This code is released under a BSD License.
 */
#ifndef SIMDBITPACKING_H_
#define SIMDBITPACKING_H_

#include <emmintrin.h>// SSE2 is required
#include <stdint.h> // use a C99-compliant compiler, please
#include <string.h> // for memset

//reads 128 values from "in", writes  "bit" 128-bit vectors to "out"
void simdpack(const uint32_t *  in,__m128i *  out, uint32_t bit);

//reads 128 values from "in", writes  "bit" 128-bit vectors to "out"
void simdpackwithoutmask(const uint32_t *  in,__m128i *  out, uint32_t bit);

//reads  "bit" 128-bit vectors from "in", writes  128 values to "out"
void simdunpack(const __m128i *  in,uint32_t *  out, uint32_t bit);


#endif /* SIMDBITPACKING_H_ */
