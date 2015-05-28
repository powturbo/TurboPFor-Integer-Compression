
#ifndef VARINTDECODE_H_
#define VARINTDECODE_H_
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdint.h>// please use a C99-compatible compiler
#include <stddef.h>

// This function must be called once to initialized tables before using the other functions below
void simdvbyteinit(void);

// Read "length" 32-bit integers in varint format from in, storing the result in out.  Returns the number of bytes read.
size_t masked_vbyte_decode(const uint8_t* in, uint32_t* out, uint64_t length);

// Read "length" 32-bit integers in varint format from in, storing the result in out with differential coding starting at prev.  Setting prev to zero is a good default. Returns the number of bytes read.
size_t masked_vbyte_decode_delta(const uint8_t* in, uint32_t* out, uint64_t length, uint32_t  prev);

// Read 32-bit integers in varint format from in, reading inputsize bytes, storing the result in out. Returns the number of integers read.
size_t masked_vbyte_decode_fromcompressedsize(const uint8_t* in, uint32_t* out,
		size_t inputsize);

// Read 32-bit integers in varint format from in, reading inputsize bytes, storing the result in out with differential coding starting at prev. Setting prev to zero is a good default. Returns the number of integers read.
size_t masked_vbyte_decode_fromcompressedsize_delta(const uint8_t* in, uint32_t* out,
		size_t inputsize, uint32_t  prev);



#endif /* VARINTDECODE_H_ */
