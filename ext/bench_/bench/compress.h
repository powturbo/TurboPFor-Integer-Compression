/*
	COMPRESS.H
	----------
*/
#ifndef COMPRESS_H_
#define COMPRESS_H_

#include <stdint.h>
#define ANT_compressable_integer uint32_t

/*
	class ANT_COMPRESS
	------------------
*/
class ANT_compress
	{
	public:
		ANT_compress() {}
		virtual ~ANT_compress() {}

		/*
			destination_length is in bytes.  source_integers is in units of integers, returns the length in bytes
		*/
		virtual uint64_t compress(uint8_t *destination, uint64_t destination_length, uint32_t *source, uint64_t source_integers) = 0;
		virtual void decompress(uint32_t *destination, uint64_t destination_integers, uint8_t *source, uint64_t source_length) = 0;
	} ;

#endif  /* COMPRESS_H_ */

