/*
	COMPRESS_OPT.H
	--------------
*/
#include "compress.h"
#include "optpfor.h"
#include "compress_variable_byte.h"

/*
	CLASS ANT_COMPRESS_OPT
	----------------------
*/
class ANT_compress_opt : public ANT_compress
	{
	FastPForLib::OPTPFor<128/32,FastPForLib::Simple16<false>> shrinkerator;
	ANT_compress_variable_byte compressor;


	void encodeArray(const uint32_t *in, uint64_t len, uint32_t *out, uint64_t *nvalue)
		{
		size_t answer = *nvalue;
		size_t PFORcompressed;
		size_t PFORencoded = (len) - (len % 128);
		uint64_t VBYTEencoded = len % 128;


		if (PFORencoded != 0)
			shrinkerator.encodeArray(in, PFORencoded, out, answer);
		else
			answer = 0;

		// For PFOR stuff, we want the number of uint32_t's, for VByte we want the number of bytes
		PFORcompressed = answer;
		uint64_t compressed_size = sizeof(uint32_t) * answer;
		compressed_size += compressor.compress(((uint8_t *)out) + compressed_size, (*nvalue - PFORcompressed) * sizeof(uint32_t), (uint32_t *)in + PFORencoded, VBYTEencoded);
		*nvalue = compressed_size;					// return the compressed size in bytes
		}


	void decodeArray(const uint32_t *in, uint64_t len, uint32_t *out, uint64_t nvalue)
		{
		size_t pfor_integers = nvalue - (nvalue % 128);
		uint8_t *source;

		if (pfor_integers != 0)
			source = (uint8_t *)shrinkerator.decodeArray(in, pfor_integers, out, pfor_integers);
		else
			source = (uint8_t *)in;
		compressor.decompress(out + pfor_integers, nvalue - pfor_integers, source, len - pfor_integers * sizeof(*out));
		}


	virtual uint64_t compress(uint8_t *destination, uint64_t destination_length, uint32_t *source, uint64_t source_integers)
      {
      uint64_t answer = source_integers;
      encodeArray(source, source_integers, (uint32_t *)destination, &answer);
      return answer;
      }

   virtual void decompress(uint32_t *destination, uint64_t destinaton_integers, uint8_t *source, uint64_t source_length)
      {
		decodeArray((uint32_t *)source, source_length, destination, destinaton_integers);
		}

	} ;
