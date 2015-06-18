/*
 * Copyright (C) 2005-2015 Christoph Rupp (chris@crupp.de).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * A fast implementation for Frame of Reference encoding.
 *
 * See the README.md file for more information, example code and references.
 *
 * Feel free to send comments/questions to chris@crupp.de. I am available
 * for consulting.
 */

#ifndef FOR_H_5580af15_4570_41f9_ba2b_8afb1400e81e
#define FOR_H_5580af15_4570_41f9_ba2b_8afb1400e81e

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Returns the size required to compress a sequence of |length| ints,
 * each compressed with |bits| bits
 *
 * This function will NOT include any overhead required by
 * for_compress_sorted() and for_compress_unsorted().
 *
 * Invariant: bits <= 32
 */
extern uint32_t
for_compressed_size_bits(uint32_t length, uint32_t bits);

/**
 * Returns the size required to compress an unsorted sequence of |length| ints.
 * 
 * This routine scans |in| for the min/max values and then calls
 * for_compressed_size_bits().
 *
 * The returned size will include the overhead required for
 * for_compress_sorted() and for_compressed_unsorted().
 */
extern uint32_t
for_compressed_size_unsorted(const uint32_t *in, uint32_t length);

/**
 * Returns the size required to compress a sorted sequence of |length| ints.
 * 
 * This routine extracts min/max values at the beginning and end of
 * the sequence, then calls for_compressed_size_bits(). It is therefore
 * slightly faster than for_compressed_size_unsorted().
 *
 * The returned size will include the overhead required for
 * for_compress_sorted() and for_compressed_unsorted().
 */
extern uint32_t
for_compressed_size_sorted(const uint32_t *in, uint32_t length);

/**
 * Compresses a sequence of |length| ints at |in| and stores the result
 * in |out|.
 *
 * |base| is the "offset" (or common delta value) of all ints. It is usually
 * set to the minimum value of the uncompressed sequence.
 *
 * |bits| are the bits required to store a single integer.
 *
 * Returns the number of bytes used for compression.
 *
 * This is for advanced users who opt for storing |base| and |bits| on their
 * own. This function is called by for_compress_sorted() and
 * for_compress_unsorted().
 *
 * Invariant: bits <= 32
 */
extern uint32_t
for_compress_bits(const uint32_t *in, uint8_t *out, uint32_t length,
                uint32_t base, uint32_t bits);

/**
 * Compresses an unsorted sequence of |length| ints at |in| and stores the
 * result in |out|.
 *
 * This routine scans |in| for the min/max values and then calls
 * for_compress_bits().
 *
 * The minimun value and the bits are stored as metadata in |out|.
 */
extern uint32_t
for_compress_unsorted(const uint32_t *in, uint8_t *out, uint32_t length);

/**
 * Compresses a sorted sequence of |length| ints at |in| and stores the
 * result in |out|.
 *
 * This routine extracts min/max values at the beginning and end of
 * the sequence, then calls for_compress_bits().
 *
 * The minimun value and the bits are stored as metadata in |out|.
 */
extern uint32_t
for_compress_sorted(const uint32_t *in, uint8_t *out, uint32_t length);

/**
 * Uncompresses a sequence of |length| ints at |in| and stores the
 * result in |out|.
 *
 * |base| is the "offset" (or common delta value) of all ints. It is usually
 * set to the minimum value of the uncompressed sequence.
 *
 * |bits| are the bits required to store a single integer.
 *
 * Returns the number of compressed bytes processed.
 *
 * This function is for advanced users. It is the counterpart of
 * for_compress_bits().
 *
 * Invariant: bits <= 32
 */
extern uint32_t
for_uncompress_bits(const uint8_t *in, uint32_t *out, uint32_t length,
                uint32_t base, uint32_t bits);

/**
 * Uncompresses a sequence of |length| ints at |in| and stores the
 * result in |out|.
 *
 * This function is a convenience wrapper for for_uncompress_bits(). It
 * expects metadata at the beginning of |in|. Use in combination with
 * for_compress_sorted() and for_compress_unsorted().
 *
 * Returns the number of compressed bytes processed.
 */
extern uint32_t
for_uncompress(const uint8_t *in, uint32_t *out, uint32_t length);

/**
 * Returns the value at the given |index| from a compressed sequence.
 *
 * Make sure that |index| does not exceed the length of the sequence.
 *
 * |base| is the "offset" (or common delta value) of all ints. It is usually
 * set to the minimum value of the uncompressed sequence.
 *
 * Invariant: bits <= 32
 */
extern uint32_t
for_select_bits(const uint8_t *in, uint32_t base, uint32_t bits,
                uint32_t index);

/**
 * Returns the value at the given |index| from a compressed sequence.
 *
 * Make sure that |index| does not exceed the length of the sequence.
 *
 * This function is a convenience wrapper for for_select_bits(). It
 * expects metadata at the beginning of |in|. Use in combination with
 * for_compress_sorted() and for_compress_unsorted().
 */
extern uint32_t
for_select(const uint8_t *in, uint32_t index);

/**
 * Performs a linear search for |value|.
 *
 * Returns the index of the found element, or |length| if the key was not
 * found.
 *
 * This function is a convenience wrapper for for_linear_search_bits(). It
 * expects metadata at the beginning of |in|. Use in combination with
 * for_compress_sorted() and for_compress_unsorted().
 */
extern uint32_t
for_linear_search(const uint8_t *in, uint32_t length, uint32_t value);

/**
 * Performs a linear search for |value|.
 *
 * Returns the index of the found element, or |length| if the key was not
 * found.
 *
 * |base| is the "offset" (or common delta value) of all ints. It is usually
 * set to the minimum value of the uncompressed sequence.
 *
 * Invariant: bits <= 32
 */
extern uint32_t
for_linear_search_bits(const uint8_t *in, uint32_t length, uint32_t base,
                uint32_t bits, uint32_t value);

/**
 * Performs lower bound binary search search for |value|.
 * 
 * A lower bound search returns the first element in the sequence which does
 * not compare less than |value|.
 * The actual result is stored in |*actual|.
 *
 * This function is a convenience wrapper for for_lower_bound_search_bits(). It
 * expects metadata at the beginning of |in|. Use in combination with
 * for_compress_sorted() and for_compress_unsorted().
 */
extern uint32_t
for_lower_bound_search(const uint8_t *in, uint32_t length, uint32_t value,
                uint32_t *actual);

/**
 * Performs lower bound binary search search for |value|.
 * 
 * A lower bound search returns the first element in the sequence which does
 * not compare less than |value|.
 * The actual result is stored in |*actual|.
 *
 * |base| is the "offset" (or common delta value) of all ints. It is usually
 * set to the minimum value of the uncompressed sequence.
 *
 * Invariant: bits <= 32
 */
extern uint32_t
for_lower_bound_search_bits(const uint8_t *in, uint32_t length, uint32_t base,
                uint32_t bits, uint32_t value, uint32_t *actual);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FOR_H_5580af15_4570_41f9_ba2b_8afb1400e81e */
