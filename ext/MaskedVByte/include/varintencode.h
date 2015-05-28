
#ifndef VARINTENCODE_H_
#define VARINTENCODE_H_

#include <stdint.h> // please use a C99-compatible compiler
#include <stddef.h>

// Encode an array of a given length read from in to bout in varint format.
// Returns the number of bytes written.
size_t vbyte_encode(uint32_t *in, size_t length, uint8_t *bout);

// Encode an array of a given length read from in to bout in varint format with differential
// coding starting at value prev. (Setting prev to 0 is a good default.)
// Returns the number of bytes written.
size_t vbyte_encode_delta(uint32_t *in, size_t length, uint8_t *bout, uint32_t prev);


#endif /* VARINTENCODE_H_ */
