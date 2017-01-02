/*
	COMPRESS_TURBOPACKV.H
	----------------------
*/
#ifndef COMPRESS_TURBOPACKV_H_
#define COMPRESS_TURBOPACKV_H_

#include <stdint.h>
#include "compress_turbopackv_internals.h"

/*
	class ANT_COMPRESS_TURBOPACKV
	------------------------------
*/
class ANT_compress_turbopackv : public ANT_compress
{
public:
	ANT_compress_turbopackv() {}
	virtual ~ANT_compress_turbopackv() {}

	virtual uint64_t compress(uint8_t *destination, uint64_t destination_length, uint32_t *source, uint64_t source_integers)
		{
		return turbopackv_compress(destination, destination_length, source, source_integers);
		}

	virtual void decompress(uint32_t *destination, uint64_t destinaton_integers, uint8_t *source, uint64_t source_length)
		{
		turbopackv_decompress(destination, destinaton_integers, source, source_length);
		}
} ;

#endif

