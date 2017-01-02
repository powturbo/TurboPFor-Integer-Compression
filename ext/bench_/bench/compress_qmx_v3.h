/*
	COMPRESS_QMX_V3.H
	------------------
	QMX with:
		no overflow (Matt's changes)
		no VB lengths (backwards scanning selectors)
		loop unwinding
*/
#ifndef COMPRESS_QMX3_H_
#define COMPRESS_QMX3_H_

#include <stdint.h>
#include "compress.h"

/*
	class ANT_COMPRESS_QMX_V3
	-------------------------
*/
class ANT_compress_qmx_v3 : public ANT_compress
	{
	private:
		uint8_t *length_buffer;
		uint64_t length_buffer_length;

	public:
		ANT_compress_qmx_v3();
		virtual ~ANT_compress_qmx_v3();

		void encodeArray(const uint32_t *in, uint64_t len, uint32_t *out, uint64_t *nvalue);
		static void decodeArray(const uint32_t *in, uint64_t len, uint32_t *out, uint64_t nvalue);

		virtual uint64_t compress(uint8_t *destination, uint64_t destination_length, uint32_t *source, uint64_t source_integers)
			{
			uint64_t answer;
			encodeArray(source, source_integers, (uint32_t *)destination, &answer);
			return answer;
			}

		virtual void decompress(uint32_t *destination, uint64_t destinaton_integers, uint8_t *source, uint64_t source_length)
			{
			decodeArray((uint32_t *)source, source_length, destination, destinaton_integers);
			}
	} ;

#endif


