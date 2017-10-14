/*
	COMPRESS_INTEGER_QMX_IMPROVED.H
	-------------------------------
	Copyright (c) 2014-2017 Andrew Trotman
	Released under the 2-clause BSD license (See:https://en.wikipedia.org/wiki/BSD_licenses)
*/
#pragma once

#include <stdint.h>
#include <assert.h>

namespace JASS
	{
	/*
		CLASS COMPRESS_INTEGER_QMX_IMPROVED
		-----------------------------------
	*/
	/*!
		@brief QMX compression improved (smaller and faster to decode)
		@details  Trotman & Lin  describe several improvements to the QMX codex in:

		A. Trotman, J. Lin (2016), In Vacuo and In Situ Evaluation of SIMD Codecs, Proceedings of The 21st Australasian Document Computing Symposium (ADCS 2016
		
		including removal of the vbyte encoded length from the end of the encoded sequence.  This version of QMX is the original QMX with that improvement added,
		but none of the other imprivements suggested by Trotman & Lin.  This makes the encoded sequence smaller, and faster to decode, than any of the other
		alrernatives suggested.  It does not include the code to prevent read and write overruns from the encoded string and into the decode buffer.  To account
		for overwrites make sure the decode-into buffer is at least 256 integers larger than required.  To prevent over-reads from the encoded string make sure
		that that string is at least 16 bytes longer than needed.
		
		At the request of Matthias Petri (University of Melbourne), the code no longer requires SIMD-word alignment to decode (the read and write 
		instructions have been changed from aligned to unaligned since Intel made them faster).
		
		For details on the original QMX encoding see:
		
		A. Trotman (2014), Compression, SIMD, and Postings Lists, Proceedings of the 19th Australasian Document Computing Symposium (ADCS 2014)
	*/
	class compress_integer_qmx_improved
		{
		typedef uint32_t integer;
		
		private:
			uint8_t *length_buffer;					///< Stores the number of bits needed to compress each integer
			uint64_t length_buffer_length;		///< The length of length_buffer
			uint32_t *full_length_buffer;			///< If the run_length is too short then 0-pad into this buffer

		private:
			/*
				COMPRESS_INTEGER_QMX_IMPROVED::WRITE_OUT()
				------------------------------------------
			*/
			/*!
				@brief Encode and write out the sequence into the buffer
				@param buffer [in] where to write the encoded sequence
				@param source [in] the integer sequence to encode
				@param raw_count [in] the numnber of integers to encode
				@param size_in_bits [in] the size, in bits, of the largest integer
				@param buffer_length [in] the length of buffer, in bytes
			*/
			void write_out(uint8_t **buffer, uint32_t *source, uint32_t raw_count, uint32_t size_in_bits, uint8_t **length_buffer);

		public:
			/*
				COMPRESS_INTEGER_QMX_IMPROVED::COMPRESS_INTEGER_QMX_IMPROVED()
				--------------------------------------------------------------
			*/
			/*!
				@brief Constructor
			*/
			compress_integer_qmx_improved() :
				length_buffer(nullptr),
				length_buffer_length(0),
				full_length_buffer(new uint32_t [256 * 16])
				{
				/* Nothing */
				}
				
			/*
				COMPRESS_INTEGER_QMX_IMPROVED::~COMPRESS_INTEGER_QMX_IMPROVED()
				---------------------------------------------------------------
			*/
			/*!
				@brief Destructor
			*/
			virtual ~compress_integer_qmx_improved()
				{
				delete [] length_buffer;
				delete [] full_length_buffer;
				}

			/*
				COMPRESS_INTEGER_QMX_IMPROVED::ENCODE()
				---------------------------------------
			*/
			/*!
				@brief Encode a sequence of integers returning the number of bytes used for the encoding, or 0 if the encoded sequence doesn't fit in the buffer.
				@param encoded [out] The sequence of bytes that is the encoded sequence.
				@param encoded_buffer_length [in] The length (in bytes) of the output buffer, encoded.
				@param source [in] The sequence of integers to encode.
				@param source_integers [in] The length (in integers) of the source buffer.
				@return The number of bytes used to encode the integer sequence, or 0 on error (i.e. overflow).
			*/
			virtual size_t encode(void *encoded, size_t encoded_buffer_length, const integer *source, size_t source_integers);

			/*
				COMPRESS_INTEGER_QMX_IMPROVED::DECODE()
				---------------------------------------
			*/
			/*!
				@brief Decode a sequence of integers encoded with this codex.
				@param decoded [out] The sequence of decoded integers.
				@param integers_to_decode [in] The minimum number of integers to decode (it may decode more).
				@param source [in] The encoded integers.
				@param source_length [in] The length (in bytes) of the source buffer.
			*/
			virtual void decode(integer *decoded, size_t integers_to_decode, const void *source, size_t source_length);


			/*
				COMPRESS_INTEGER_QMX_IMPROVED::ENCODEARRAY()
				--------------------------------------------
				provided for backwards compatibility
			*/
			virtual void encodeArray(const uint32_t *in, uint64_t len, uint32_t *out, uint64_t *nvalue)
				{
				*nvalue = encode(out, *nvalue, in, len);
				}
			
			/*
				COMPRESS_INTEGER_QMX_IMPROVED::DECODEARRAY()
				--------------------------------------------
				provided for backwards compatibility
			*/
			virtual void decodeArray(const uint32_t *in, uint64_t len, uint32_t *out, uint64_t nvalue)
				{
				decode(out, nvalue, in, len);
				}

			/*
				COMPRESS_INTEGER_QMX_IMPROVED::UNITTEST_ONE()
				---------------------------------------------
			*/
			/*!
				@brief Test one sequence to make sure it encodes and decodes to the same thing.  Assert if not.
				@para sequence [in] the sequernce to encode.
			*/
			static void unittest_one(const std::vector<uint32_t> &sequence);

			/*
				COMPRESS_INTEGER_QMX_IMPROVED::UNITTEST()
				-----------------------------------------
			*/
			/*!
				@brief Unit test this class
			*/
			static void unittest(void);
		};
	}

