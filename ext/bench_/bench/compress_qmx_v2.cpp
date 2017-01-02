/*
	COMPRESS_QMX_V2.CPP
	-------------------
	Copyright (c) 2014 by Andrew Trotman
	Licensed BSD

	A version of BinPacking where we pack into a 128-bit SSE register the following:
		256  0-bit words
		128  1-bit words 
		 64	 2-bit words
		 40  3-bit words
		 32  4-bit words
		 24  5-bit words
		 20  6-bit words
		 16  8-bit words
		 12 10-bit words
		  8 16-bit words
		  4 32-bit words
		or pack into two 128-bit words (i.e. 256 bits) the following:
		 36  7-bit words
		 28  9-bit words
		 20 12-bit words
		 12 21-bit words
		
	This gives us 15 possible combinations.  The combinaton is stored in the top 4 bits of a selector byte.  The
	bottom 4-bits of the selector store a run-length (the number of such sequences seen in a row.

	The 128-bit (or 256-bit) packed binary values are stored first.  Then we store the selectors,  Finally,
	stored variable byte encoded, is a pointer to the start of the selector (from the end of the sequence).

	This way, all reads and writes are 128-bit word aligned, except addressing the selector (and the pointer
	the selector).  These reads are byte aligned.

	Note:  There is currently 1 unused encoding (i.e. 16 unused selecvtor values).  These might in the future be
	used for encoding exceptions, much as PForDelta does.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include "compress_qmx_v2.h"

//#define MAKE_DECOMPRESS 1		/* uncomment this and it will create a program that writes the decompressor */
//#define TEST_ONE_STRING 1		/* Uncomment this and it will create a program that can be used to test the compressor and decompressor */
#define NO_ZEROS 1					/* stores runs of 256  1s in a row (not 1-bit number, but actual 1 values). */
#define SHORT_END_BLOCKS 1

#ifdef _MSC_VER
	#define ALIGN_16 __declspec(align(16))
#else
	#define ALIGN_16 __attribute__ ((aligned (16)))
#endif

//#define STATS						/* uncomment this and it will count the selector usage */
#ifdef STATS
	static uint32_t stats[65] = {0};
#endif

/*
	ANT_COMPRESS_QMX_V2::ANT_COMPRESS_QMX_V2()
	------------------------------------
*/
ANT_compress_qmx_v2::ANT_compress_qmx_v2()
{
length_buffer = NULL;
length_buffer_length = 0;
}

/*
	ANT_COMPRESS_QMX_V2::~ANT_COMPRESS_QMX_V2()
	-------------------------------------
*/
ANT_compress_qmx_v2::~ANT_compress_qmx_v2()
{
delete [] length_buffer;
#ifdef STATS
	uint32_t which;
	for (which = 0; which <= 32; which++)
		if (stats[which] != 0)
			printf("%d\t%d\ttimes\n", which, stats[which]);
#endif
}

/*
	BYTES_NEEDED_FOR()
	------------------
*/
static uint8_t bytes_needed_for(uint32_t value)
{
if (value <= 0xFF)
	return 1;
else if (value <= 0xFFFF)
	return 2;
else if (value <= 0xFFFFFF)
	return 3;
else
	return 4;
}

/*
	BITS_NEEDED_FOR()
	-----------------
*/
static uint8_t bits_needed_for(uint32_t value)
{
if (value == 0x01)
	return 0;
else if (value <= 0x01)
	return 1;
else if (value <= 0x03)
	return 2;
else if (value <= 0x07)
	return 3;
else if (value <= 0x0F)
	return 4;
else if (value <= 0x1F)
	return 5;
else if (value <= 0x3F)
	return 6;
else if (value <= 0x7F)
	return 7;
else if (value <= 0xFF)
	return 8;
else if (value <= 0x1FF)
	return 9;
else if (value <= 0x3FF)
	return 10;
else if (value <= 0xFFF)
	return 12;
else if (value <= 0xFFFF)
	return 16;
else if (value <= 0x1FFFFF)
	return 21;
else
	return 32;
}

/*
	WRITE_OUT()
	-----------
*/
static void write_out(uint8_t **buffer, uint32_t *source, uint32_t raw_count, uint32_t size_in_bits, uint8_t **length_buffer)
{
uint32_t current, batch;
uint8_t *destination = *buffer;
uint32_t *end = source + raw_count;
uint8_t *key_store = *length_buffer;
uint32_t ALIGN_16 sequence_buffer[4];
uint32_t instance, value;
uint8_t type;
uint32_t count;

uint32_t max_bytes = 1;		// this is the bytw-width for type128 encoded non-SSE integers

#ifdef STATS
	stats[size_in_bits] += raw_count;
#endif

if (size_in_bits == 0)
	{
	type = 0;
	count = (raw_count + 255) / 256;
	}
else if (size_in_bits == 1)
	{
	type = 1;		// 1 bit per integer
	count = (raw_count + 127) / 128;
	}
else if (size_in_bits == 2)
	{
	type = 2;		// 2 bits per integer
	count = (raw_count + 63) / 64;
	}
else if (size_in_bits == 3)
	{
	type = 3;		// 3 bits per integer
	count = (raw_count + 39) / 40;
	}
else if (size_in_bits == 4)
	{
	type = 4;		// 4 bits per integer
	count = (raw_count + 31) / 32;
 	}
else if (size_in_bits == 5)
	{
	type = 5;		// 5 bits per integer
	count = (raw_count + 23) / 24;
 	}
else if (size_in_bits == 6)
	{
	type = 6;		// 6 bits per integer
	count = (raw_count + 19) / 20;
 	}
else if (size_in_bits == 7)
	{
	type = 7;		// 7 bits per integer, 18 integers per read (but requires 2 reads)
	count = (raw_count + 35) / 36;
	}
else if (size_in_bits == 8)
	{
	type = 8;		// 8 bits per integer
	count = (raw_count + 15) / 16;
	}
else if (size_in_bits == 9)
	{
	type = 9;		// 9 bits per integer, 14 integers per read (but requires 2 reads)
	count = (raw_count + 27) / 28;
	}
else if (size_in_bits == 10)
	{
	type = 10;		// 10 bits per integer
	count = (raw_count + 11) / 12;
	}
else if (size_in_bits == 12)
	{
	type = 11;		// 12 bits per integer, 10 integers per read (but requires 2 reads)
	count = (raw_count + 19) / 20;
	}
else if (size_in_bits == 16)
	{
	type = 12;		// 16 bits per integer
	count = (raw_count + 7) / 8;
	}
else if (size_in_bits == 21)
	{
	type = 13;		// 21 bits per integer, 6 integers per read (but requires 2 reads)
	count = (raw_count + 11) / 12;
	}
else if (size_in_bits == 32)
	{
	type = 14;		// 32 bits per integer
	count = (raw_count + 3) / 4;
	}
else if (size_in_bits == 128)
	{
	type = 15;
	count = raw_count;
	/*
		As the count for type 128 can only be 1, 2, or 3, we can re-appropriate it and store the bit-length in there too.
	*/
	max_bytes = 1;
	for (uint32_t integer = 0; integer < count; integer++)
		{
		if (bytes_needed_for(source[integer]) > max_bytes)
			max_bytes = bytes_needed_for(source[integer]);
		}
	}
else
	exit(printf("Can't compress into integers of size %dbits\n", size_in_bits));

while (count > 0)
	{
	batch = count > 16 ? 16 : count;
	*key_store++ = (type << 4) | (~(batch - 1) & 0x0F);

	count -= batch;

	for (current = 0; current < batch; current++)
		{
		switch (size_in_bits)
			{
			case 0:		// 0 bits per integer (i.e. a long sequence of zeros)
				/*
					In this case we don't need to store a 4 byte integer because its implicit
				*/
				source += 256;
				break;
			case 1:		// 1 bit per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 128; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 1);

				memcpy(destination, sequence_buffer, 16);
				destination += 16;
				source += 128;
				break;
			case 2:		// 2 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 64; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 2);

				memcpy(destination, sequence_buffer, 16);
				destination += 16;
				source += 64;
				break;
			case 3:		// 3 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 40; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 3);

				memcpy(destination, sequence_buffer, 16);
				destination += 16;
				source += 40;
				break;
			case 4:		// 4 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 32; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 4);

				memcpy(destination, sequence_buffer, 16);
				destination += 16;
				source += 32;
				break;
			case 5:		// 5 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 24; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 5);

				memcpy(destination, sequence_buffer, 16);
				destination += 16;
				source += 24;
				break;
			case 6:		// 6 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 20; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 6);
				memcpy(destination, sequence_buffer, 16);
				destination += 16;
				source += 20;
				break;
			case 7:		// 7 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 20; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 7);
				memcpy(destination, sequence_buffer, 16);
				destination += 16;

				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 16; value < 20; value++)
					sequence_buffer[value & 0x03] |= source[value] >> 4;
				for (value = 20; value < 36; value++)
					sequence_buffer[value & 0x03] |= source[value] << (((value - 20) / 4) * 7 + 3);
				memcpy(destination, sequence_buffer, 16);

				destination += 16;
				source += 36;				// 36 in a double 128-bit word
				break;
			case 8:		// 8 bits per integer
#ifdef SHORT_END_BLOCKS
				for (instance = 0; instance < 16 && source < end; instance++)
#else
				for (instance = 0; instance < 16; instance++)
#endif
					*destination++ = (uint8_t)*source++;
				break;
			case 9:		// 9 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 16; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 9);
				memcpy(destination, sequence_buffer, 16);
				destination += 16;

				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 12; value < 16; value++)
					sequence_buffer[value & 0x03] |= source[value] >> 5;
				for (value = 16; value < 28; value++)
					sequence_buffer[value & 0x03] |= source[value] << (((value - 16) / 4) * 9 + 4);
				memcpy(destination, sequence_buffer, 16);

				destination += 16;
				source += 28;				// 28 in a double 128-bit word
				break;
			case 10:		// 10 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 12; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 10);

				memcpy(destination, sequence_buffer, 16);
				destination += 16;
				source += 12;
				break;
			case 12:		// 12 bit integers
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 12; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 12);
				memcpy(destination, sequence_buffer, 16);
				destination += 16;

				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 8; value < 12; value++)
					sequence_buffer[value & 0x03] |= source[value] >> 8;
				for (value = 12; value < 20; value++)
					sequence_buffer[value & 0x03] |= source[value] << (((value - 12) / 4) * 12 + 8);
				memcpy(destination, sequence_buffer, 16);

				destination += 16;
				source += 20;				// 20 in a double 128-bit word
				break;
			case 16:		// 16 bits per integer
#ifdef SHORT_END_BLOCKS
				for (instance = 0; instance < 8 && source < end; instance++)
#else
				for (instance = 0; instance < 8; instance++)
#endif
					{
					*(uint16_t *)destination = (uint16_t)*source++;
					destination += 2;
					}
				break;
			case 21:		// 21 bits per integer
				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 0; value < 8; value++)
					sequence_buffer[value & 0x03] |= source[value] << ((value / 4) * 21);
				memcpy(destination, sequence_buffer, 16);
				destination += 16;

				memset(sequence_buffer, 0, sizeof(sequence_buffer));
				for (value = 4; value < 8; value++)
					sequence_buffer[value & 0x03] |= source[value] >> 11;
				for (value = 8; value < 12; value++)
					sequence_buffer[value & 0x03] |= source[value] << (((value - 8) / 4) * 21 + 11);
				memcpy(destination, sequence_buffer, 16);

				destination += 16;
				source += 12;				// 12 in a double 128-bit word
				break;
			case 32:		// 32 bits per integer
#ifdef SHORT_END_BLOCKS
				for (instance = 0; instance < 4 && source < end; instance++)
#else
				for (instance = 0; instance < 4; instance++)
#endif
					{
					*(uint32_t *)destination = (uint32_t)*source++;
					destination += 4;
					}
				break;
			case 128:
				if (max_bytes == 1)
					{
					*(uint8_t *)destination = (uint8_t)*source;
					source++;
					destination += 1;
					*(key_store - 1) = (type << 4) | (~(batch - 1) & 0x03);
					}
				else if (max_bytes == 2)
					{
					*(uint16_t *)destination = (uint16_t)*source;
					source++;
					destination += 2;
					*(key_store - 1) = (type << 4) | 4 | (~(batch - 1) & 0x03);
					}
				else if (max_bytes == 3)
					{
					*destination++ = (uint8_t)((*source >> 16) & 0xFF);
					*destination++ = (uint8_t)((*source >>  8) & 0xFF);
					*destination++ = (uint8_t)((*source >>  0) & 0xFF);
					source++;
					
					*(key_store - 1) = (type << 4) | 8 | (~(batch - 1) & 0x03);
					}
				else if (max_bytes == 4)
					{
					*(uint32_t *)destination = (uint32_t)*source;
					source++;
					destination += 4;
					*(key_store - 1) = (type << 4) | 0x0C | (~(batch - 1) & 0x03);
					}
				else
					printf("max_bytes must be 1, 2, 3, or 4, but is:%d", (int)max_bytes);
				break;
			}
		}
	}
*buffer = destination;
*length_buffer = key_store;
}

/*
	MAX()
	-----
*/
template <class T>
T max(T a, T b)
{
return a > b ? a : b;
}

/*
	MAX()
	-----
*/
template <class T>
T max(T a, T b, T c, T d)
{
return max(max(a, b), max(c, d));
}

/*
	ANT_COMPRESS_QMX_V2::ENCODEARRAY()
	-------------------------------
*/
void ANT_compress_qmx_v2::encodeArray(const uint32_t *source, uint64_t source_integers, uint32_t *into, uint64_t *nvalue)
{
const uint32_t WASTAGE = 512;
uint8_t *current_length, *destination = (uint8_t *)into, *keys;
uint32_t *current, run_length, bits, new_needed, wastage;
uint32_t block, largest;

/*
	make sure we have enough room to store the lengths
*/
if (length_buffer_length < source_integers)
	{
	delete [] length_buffer;
	length_buffer = new uint8_t [(size_t)(length_buffer_length = source_integers) + WASTAGE];
	}

/*
	Get the lengths of the integers
*/
current_length = length_buffer;
for (current = (uint32_t *)source; current < source + source_integers; current++)
	*current_length++ = bits_needed_for(*current);

/*
	Shove a bunch of 0 length integers on the end to allow for overflow
*/
for (wastage = 0; wastage < WASTAGE; wastage++)
	*current_length++ = 0;

/*
	Process the lengths.  To maximise SSE throughput we need each write to be 128-bit (4*32-bit) alignned
	and therefore we need each compress "block" to be the same size where a compress "block" is a set of
	four encoded integers starting on a 4-integer boundary.
*/
for (current_length = length_buffer; current_length < length_buffer + source_integers + 4; current_length += 4)
	*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = max(*current_length, *(current_length + 1), *(current_length + 2), *(current_length + 3));

/*
	This code makes sure we can do aligned reads, promoting to larger integers if necessary
*/
current_length = length_buffer;
while (current_length < length_buffer + source_integers)
	{
#ifdef SHORT_END_BLOCKS
	/*
		If there are fewer than 16 values remaining and they all fit into 8-bits then its smaller than storing stripes
		If there are fewer than 8 values remaining and they all fit into 16-bits then its smaller than storing stripes
		If there are fewer than 4 values remaining and they all fit into 32-bits then its smaller than storing stripes
	*/
	if (source_integers - (current_length - length_buffer)  < 4)
		{
		largest = 0;
		for (block = 0; block < 8; block++)
			largest = max((uint8_t)largest, *(current_length + block));
		if (largest <= 8)
			for (block = 0; block < 8; block++)
				*(current_length + block) = 8;
		else if (largest <= 16)
			for (block = 0; block < 8; block++)
				*(current_length + block) = 16;
		else if (largest <= 32)
			for (block = 0; block < 8; block++)
				*(current_length + block) = 32;
		}
	else if (source_integers - (current_length - length_buffer)  < 8)
		{
		largest = 0;
		for (block = 0; block < 8; block++)
			largest = max((uint8_t)largest, *(current_length + block));
		if (largest <= 8)
			for (block = 0; block < 8; block++)
				*(current_length + block) = 8;
		else if (largest <= 8)
			for (block = 0; block < 8; block++)
				*(current_length + block) = 16;
		}
	else if (source_integers - (current_length - length_buffer)  < 16)
		{
		largest = 0;
		for (block = 0; block < 16; block++)
			largest = max((uint8_t)largest, *(current_length + block));
		if (largest <= 8)
			for (block = 0; block < 16; block++)
				*(current_length + block) = 8;
		}
	/*
		Otherwise we have the standard rules for a block
	*/
#endif
	/*
		Two things need to happen to be able to use a particular selector. The first is that all the 
		values that would end up in that block need to use at most the bit value of that block.
		The second is that there need to be at least as many numbers remaining as the block encodes.
		
		For example, if the current block only needs 0-bits per int, then check that the 256 values
		that would be encoded only take 0-bits. If any value needs more, or there aren't 256 numbers remaining,
		then promote the current block to try encode 128 1-bit values.
	*/
	switch (*current_length)
		{
		case 0:
			if (source_integers - (current_length - length_buffer) < 256)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 1;				// promote
				break;
				}
			for (block = 0; block < 256; block += 4)
				if (*(current_length + block) > 0)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 1;				// promote
			if (*current_length == 0)
				{
				for (block = 0; block < 256; block++)
					current_length[block] = 0;
				current_length += 256;
				}
			break;
		case 1:
			if (source_integers - (current_length - length_buffer) < 128)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 2;				// promote
				break;
				}
			for (block = 0; block < 128; block += 4)
				if (*(current_length + block) > 1)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 2;				// promote
			if (*current_length == 1)
				{
				for (block = 0; block < 128; block++)
					current_length[block] = 1;
				current_length += 128;
				}
			break;
		case 2:
			if (source_integers - (current_length - length_buffer) < 64)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 3;				// promote
				break;
				}
			for (block = 0; block < 64; block += 4)
				if (*(current_length + block) > 2)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 3;				// promote
			if (*current_length == 2)
				{
				for (block = 0; block < 64; block++)
					current_length[block] = 2;
				current_length += 64;
				}
			break;
		case 3:
			if (source_integers - (current_length - length_buffer) < 40)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 4;				// promote
				break;
				}
			for (block = 0; block < 40; block += 4)
				if (*(current_length + block) > 3)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 4;				// promote
			if (*current_length == 3)
				{
				for (block = 0; block < 40; block++)
					current_length[block] = 3;
				current_length += 40;
				}
			break;
		case 4:
			if (source_integers - (current_length - length_buffer) < 32)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 5;				// promote
				break;
				}
			for (block = 0; block < 32; block += 4)
				if (*(current_length + block) > 4)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 5;				// promote
			if (*current_length == 4)
				{
				for (block = 0; block < 32; block++)
					current_length[block] = 4;
				current_length += 32;
				}
			break;
		case 5:
			if (source_integers - (current_length - length_buffer) < 24)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 6;				// promote
				break;
				}
			for (block = 0; block < 24; block += 4)
				if (*(current_length + block) > 5)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 6;				// promote
			if (*current_length == 5)
				{
				for (block = 0; block < 24; block++)
					current_length[block] = 5;
				current_length += 24;
				}
			break;
		case 6:
			if (source_integers - (current_length - length_buffer) < 20)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 7;				// promote
				break;
				}
			for (block = 0; block < 20; block += 4)
				if (*(current_length + block) > 6)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 7;				// promote
			if (*current_length == 6)
				{
				for (block = 0; block < 20; block++)
					current_length[block] = 6;
				current_length += 20;
				}
			break;
		case 7:
			if (source_integers - (current_length - length_buffer) < 36)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 8;				// promote
				break;
				}
			for (block = 0; block < 36; block += 4)		// 36 in a double 128-bit word
				if (*(current_length + block) > 7)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 8;				// promote
			if (*current_length == 7)
				{
				for (block = 0; block < 36; block++)
					current_length[block] = 7;
				current_length += 36;
				}
			break;
		case 8:
			if (source_integers - (current_length - length_buffer) < 16)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 9;				// promote
				break;
				}
			for (block = 0; block < 16; block += 4)
				if (*(current_length + block) > 8)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 9;				// promote
			if (*current_length == 8)
				{
				for (block = 0; block < 16; block++)
					current_length[block] = 8;
				current_length += 16;
				}
			break;
		case 9:
			if (source_integers - (current_length - length_buffer) < 28)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 10;				// promote
				break;
				}
			for (block = 0; block < 28; block += 4)		// 28 in a double 128-bit word
				if (*(current_length + block) > 9)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 10;				// promote
			if (*current_length == 9)
				{
				for (block = 0; block < 28; block++)
					current_length[block] = 9;
				current_length += 28;
				}
			break;
		case 10:
			if (source_integers - (current_length - length_buffer) < 12)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 12;				// promote
				break;
				}
			for (block = 0; block < 12; block += 4)
				if (*(current_length + block) > 10)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 12;				// promote
			if (*current_length == 10)
				{
				for (block = 0; block < 12; block++)
					current_length[block] = 10;
				current_length += 12;
				}
			break;
		case 12:
			if (source_integers - (current_length - length_buffer) < 20)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 16;				// promote
				break;
				}
			for (block = 0; block < 20; block += 4)		// 20 in a double 128-bit word
				if (*(current_length + block) > 12)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 16;				// promote
			if (*current_length == 12)
				{
				for (block = 0; block < 20; block++)
					current_length[block] = 12;
				current_length += 20;
				}
			break;
		case 16:
			if (source_integers - (current_length - length_buffer) < 8)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 21;				// promote
				break;
				}
			for (block = 0; block < 8; block += 4)
				if (*(current_length + block) > 16)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 21;				// promote
			if (*current_length == 16)
				{
				for (block = 0; block < 8; block++)
					current_length[block] = 16;
				current_length += 8;
				}
			break;
		case 21:
			if (source_integers - (current_length - length_buffer) < 12)
				{
				*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 32;				// promote
				break;
				}
			for (block = 0; block < 12; block += 4)		// 12 in a double 128-bit word
				if (*(current_length + block) > 21)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 32;				// promote
			if (*current_length == 21)
				{
				for (block = 0; block < 12; block++)
					current_length[block] = 21;
				current_length += 12;
				}
			break;
		case 32:
			if (source_integers - (current_length - length_buffer) < 4)
				{
				for (block = 0; block < (source_integers - (current_length - length_buffer)); block++)
					*(current_length + block) = 128; // promote
				break;
				}
			for (block = 0; block < 4; block += 4)
				if (*(current_length + block) > 32)
					*current_length = *(current_length + 1) = *(current_length + 2) = *(current_length + 3) = 64;				// promote
			if (*current_length == 32)
				{
				for (block = 0; block < 4; block++)
					current_length[block] = 32;
				current_length += 4;
				}
			break;
		case 128:
			/*
				The 128-bit selector is used as a last resort when there are not enough numbers to use an
				earlier selector. So don't worry about checking the rest.
			*/
			current_length += source_integers - (current_length - length_buffer);
			break;
		default:
			exit(printf("Selecting on a non whole power of 2, must exit\n"));
			break;
		}
	}

/*
	We can now compress based on the lengths in length_buffer
*/
run_length = 1;
bits = length_buffer[0];
keys = length_buffer;				// we're going to re-use the length_buffer because it can't overlap and this saves a double malloc
for (current = (uint32_t *)source + 1; current < source + source_integers; current++)
	{
	new_needed = length_buffer[current - source];
	if (new_needed == bits)
		run_length++;
	else
		{
		write_out(&destination, (uint32_t *)current - run_length, run_length, bits, &keys);
		bits = new_needed;
		run_length = 1;
		}
	}
write_out(&destination, (uint32_t *)current - run_length, run_length, bits, &keys);

/*
	Copy the lengths to the end, backwards
*/
uint8_t *from = length_buffer + (keys - length_buffer) - 1;
uint8_t *to = destination;
for (uint32_t pos = 0; pos < keys - length_buffer; pos++)
	*to++ = *from--;
destination += keys - length_buffer;

/*
	Compute the length (in bytes)
*/
*nvalue = destination - (uint8_t *)into;	// return length in bytes
}

#ifdef MAKE_DECOMPRESS
	/*
		The following program generates the source code for ANT_compress_qmx_v2::decodeArray()
	*/
	/*
		MAIN()
		------
		This version assumes SSE4.1 and so it is *not* portable to non X86 architectures
	*/
	int main(void)
	{
	uint32_t instance;

	printf("static uint32_t ALIGN_16 static_mask_21[]  = {0x1fffff, 0x1fffff, 0x1fffff, 0x1fffff};\n");
	printf("static uint32_t ALIGN_16 static_mask_12[]  = {0xfff, 0xfff, 0xfff, 0xfff};\n");
	printf("static uint32_t ALIGN_16 static_mask_10[] = {0x3ff, 0x3ff, 0x3ff, 0x3ff};\n");
	printf("static uint32_t ALIGN_16 static_mask_9[]  = {0x1ff, 0x1ff, 0x1ff, 0x1ff};\n");
	printf("static uint32_t ALIGN_16 static_mask_7[]  = {0x7f, 0x7f, 0x7f, 0x7f};\n");
	printf("static uint32_t ALIGN_16 static_mask_6[]  = {0x3f, 0x3f, 0x3f, 0x3f};\n");
	printf("static uint32_t ALIGN_16 static_mask_5[]  = {0x1f, 0x1f, 0x1f, 0x1f};\n");
	printf("static uint32_t ALIGN_16 static_mask_4[]  = {0x0f, 0x0f, 0x0f, 0x0f};\n");
	printf("static uint32_t ALIGN_16 static_mask_3[]  = {0x07, 0x07, 0x07, 0x07};\n");
	printf("static uint32_t ALIGN_16 static_mask_2[]  = {0x03, 0x03, 0x03, 0x03};\n");
	printf("static uint32_t ALIGN_16 static_mask_1[]  = {0x01, 0x01, 0x01, 0x01};\n");
	printf("void ANT_compress_qmx_v2::decodeArray(const uint32_t *source, uint64_t len, uint32_t *to, uint64_t destination_integers)\n");
	printf("{\n");
	printf("__m128i byte_stream, byte_stream_2, tmp, tmp2, mask_21, mask_12, mask_10, mask_9, mask_7, mask_6, mask_5, mask_4, mask_3, mask_2, mask_1;\n");
	printf("uint8_t *in = (uint8_t *)source;\n");
	printf("uint8_t *keys = ((uint8_t *)source) + len - 1;\n");

 	printf("\n");
	printf("mask_21 = _mm_loadu_si128((__m128i *)static_mask_21);\n");
	printf("mask_12 = _mm_loadu_si128((__m128i *)static_mask_12);\n");
	printf("mask_10 = _mm_loadu_si128((__m128i *)static_mask_10);\n");
	printf("mask_9 = _mm_loadu_si128((__m128i *)static_mask_9);\n");
	printf("mask_7 = _mm_loadu_si128((__m128i *)static_mask_7);\n");
	printf("mask_6 = _mm_loadu_si128((__m128i *)static_mask_6);\n");
	printf("mask_5 = _mm_loadu_si128((__m128i *)static_mask_5);\n");
	printf("mask_4 = _mm_loadu_si128((__m128i *)static_mask_4);\n");
	printf("mask_3 = _mm_loadu_si128((__m128i *)static_mask_3);\n");
	printf("mask_2 = _mm_loadu_si128((__m128i *)static_mask_2);\n");
	printf("mask_1 = _mm_loadu_si128((__m128i *)static_mask_1);\n");
	printf("\n");

	printf("while (in <= keys)			// <= because there can be a boundary case where the final key is 255*0 bit integers\n");
	printf("\t{\n");
	printf("\tswitch (*keys--)\n");
	printf("\t\t{\n");

	for (instance = 0; instance <= 0xFF; instance++)
		{
		printf("\t\tcase 0x%02x:\n", instance);
		if ((instance >> 4) == 0)
			{
			/*
				256 0-bit integers
			*/
			printf("#ifdef NO_ZEROS\n");
			printf("\t\t\ttmp = _mm_loadu_si128((__m128i *)static_mask_1);\n");
			printf("#else\n");
			printf("\t\t\ttmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));\n");
			printf("#endif\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 5, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 6, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 7, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 8, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 9, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 10, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 11, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 12, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 13, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 14, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 15, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 16, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 17, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 18, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 19, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 20, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 21, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 22, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 23, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 24, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 25, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 26, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 27, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 28, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 29, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 30, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 31, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 32, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 33, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 34, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 35, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 36, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 37, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 38, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 39, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 40, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 41, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 42, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 43, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 44, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 45, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 46, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 47, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 48, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 49, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 50, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 51, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 52, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 53, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 54, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 55, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 56, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 57, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 58, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 59, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 60, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 61, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 62, tmp);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 63, tmp);\n");
			printf("\t\t\tto += 256;\n");		// becomes 256 integers
			}
		else if (instance >> 4 == 1)
			{
			/*
				128 * 1-bit integers
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 128;\n");		// becomes 128 integers
			}
		else if (instance >> 4 == 2)
			{
			/*
				64 * 2-bit integers
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 2);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 64;\n");		// becomes 64 integers
			}
		else if (instance >> 4 == 3)
			{
			/*
				40 * 3-bit integers
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 40;\n");		// becomes 40 integers
			}
		else if (instance >> 4 == 4)
			{
			/*
				32 * 4-bit integers
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 4);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 4);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 4);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 4);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 4);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 4);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 4);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 32;\n");		// becomes 32 integers
			}
		else if (instance >> 4 == 5)
			{
			/*
				24 * 5-bit integers
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 5);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 5);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 5);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 5);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 5);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 24;\n");		// becomes 24 integers
			}
		else if (instance >> 4 == 6)
			{
			/*
				20 * 6-bit integers
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 6);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 6);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 6);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 6);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 20;\n");		// becomes 20 integers
			}
		else if (instance >> 4 == 7)
			{
			/*
				36 * 7 bit integers (in two 128-bit words)
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 7);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 7);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 7);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));\n");

			printf("\t\t\tbyte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream_2, 3);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 7);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 7);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 7);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));\n");

			printf("\t\t\tin += 32;\n");		// 32 bytes
			printf("\t\t\tto += 36;\n");		// becomes 36 integers
			}
		else if (instance >> 4 == 8)
			{
			/*
				16 * 8-bit integers
			*/
			printf("\t\t\ttmp = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));\n");
			printf("\t\t\ttmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));\n");
			printf("\t\t\ttmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));\n");
			printf("\t\t\ttmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 16;\n");		// becomes 16 integers
			}
		else if (instance >> 4 == 9)
			{
			/*
				28 * 9-bit ingtegers (in two 128-bit words)
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 9);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 9);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));\n");
			printf("\t\t\tbyte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream_2, 4);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 9);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 9);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));\n");
			printf("\t\t\tin += 32;\n");		// 32 bytes
			printf("\t\t\tto += 28;\n");		// becomes 28 integers
			}
		else if (instance >> 4 == 10)
			{
			/*
				12 * 10-bit integers
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 10);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi64(byte_stream, 10);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 12;\n");		// becomes 12 integers
			}
		else if (instance >> 4 == 11)
			{
			/*
				20 * 12-bit ingtegers (in two 128-bit words)
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 12);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));\n");
			printf("\t\t\tbyte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream_2, 8);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));\n");
			printf("\t\t\tbyte_stream = _mm_srli_epi32(byte_stream, 12);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));\n");

			printf("\t\t\tin += 32;\n");		// 32 bytes
			printf("\t\t\tto += 20;\n");		// becomes 20 integers
			}
		else if (instance >> 4 == 12)
			{
			/*
				16-bit integers
			*/
			printf("\t\t\ttmp = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 8;\n");			// becomes 8 integers
			}
		else if (instance >> 4 == 13)
			{
			/*
				12 * 21-bit ingtegers (in two 128-bit words)
			*/
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));\n");
			printf("\t\t\tbyte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));\n");

			printf("\t\t\tin += 32;\n");			// 32 bytes
			printf("\t\t\tto += 12;\n");			// becomes 8 integers
			}
		else if (instance >> 4 == 14)
			{
			/*
				32-bit integers
			*/
			printf("\t\t\ttmp = _mm_loadu_si128((__m128i *)in);\n");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, tmp);\n");

			printf("\t\t\tin += 16;\n");		// 16 bytes
			printf("\t\t\tto += 4;\n");			// becomes 4 integers
			}
		else if (instance >> 4 == 15)
			{
			/*
				128-bit integers
				if there are fewer than 4 integes then we just bit-pack them in to 8, 16, 24, or 32-bit words
			*/
			if ((instance & 0x0C) == 0x00)
				{
				printf("\t\t\t*to = *(uint8_t *)in;\n");
				printf("\t\t\tin += 1;\n");			// 1 byte integer
				printf("\t\t\tto += 1;\n");			// becomes 1 integer
				}
			else if ((instance & 0x0C) == 0x04)
				{
				printf("\t\t\t*to = *(uint16_t *)in;\n");
				printf("\t\t\tin += 2;\n");			// 2 byte integers
				printf("\t\t\tto += 1;\n");			// becomes 1 integer
				}
			else if ((instance & 0x0C) == 0x08)
				{
				printf("\t\t\t*to = (*(uint8_t *)in << 16) | (*(uint8_t *)(in + 1) << 8) | (*(uint8_t *)(in + 2));\n");
				printf("\t\t\tin += 3;\n");			// 3 byte integer
				printf("\t\t\tto += 1;\n");			// becomes 1 integer
				}
			else if ((instance & 0x0C) == 0x0C)
				{
				printf("\t\t\t*to = *(uint32_t *)in;\n");
				printf("\t\t\tin += 4;\n");			// 4 byte integer
				printf("\t\t\tto += 1;\n");			// becomes 1 integer
				}
			if (instance == 0xFF || instance == 0xFB || instance == 0xF7 || instance == 0xF3)
				printf("\t\t\tbreak;\n");
			}
		else
			{
			printf("\t\t\tin++;\n");			// dummy, can't occur
			}
		if ((instance & 0xF) == 0xF)
			printf("\t\t\tbreak;\n");		// every 32 instances we break (its the end of the fall through)
		}
	printf("\t\t}\n");
	printf("\t}\n");
	printf("}\n");
	}
#endif

#ifdef TEST_ONE_STRING
	static uint32_t sequence[]={13,1,1,26,18,3,1,9,4,8,5,19,7,26,1,5,7,3,12,5,39,16,3,5,19,8,18,1,1,1,2,5,9,3,21,2,6,37,3,5,5,18,3,31,3,22,5,17,6,12,6,2,5,10,3,12,51,14,7,8,1,2,3,27,19,1,10,8,2,7,2,9,16,6,6,5,6,4,18,21,13,2,1,11,3,22,2,16,13,61,21,12,51,10,6,31,14,65,15,82,5,4,18,3,1,1,4,34,5,9,4,7,1,25,17,52,60,8,8,4,22,7,49,26,2,72,29,33,6,11,3,8,1,23,37,1,3,1,1,1,3,20,6,1,2,1,1,1,14,2,4,1,6,4,4,3,1,1,2,2,1,9,29,1,10,11,4,10,31};

	static uint32_t second_compress_buffer[100000];
	static uint32_t second_decompress_buffer[100000];

	uint32_t second_compress_buffer_size = sizeof(second_compress_buffer) / sizeof(*second_compress_buffer);
	uint32_t second_decompress_buffer_size = sizeof(second_decompress_buffer) / sizeof(*second_decompress_buffer);

	/*
		CHECK()
		-------
	*/
	void check(uint32_t *sequence, uint32_t sequence_length)
	{
  	ANT_compress_qmx_v2 compressor;
	uint64_t buffer_size;
	uint32_t pos;
	uint32_t fail;

	memset(second_compress_buffer, 0, second_compress_buffer_size);
	memset(second_decompress_buffer, 0, second_decompress_buffer_size);

	compressor.encodeArray(sequence, sequence_length, (uint32_t *)second_compress_buffer, &buffer_size);
	second_compress_buffer[buffer_size] = 0;
	second_compress_buffer[buffer_size + 1] = 0;
	second_compress_buffer[buffer_size + 2] = 0;
	second_compress_buffer[buffer_size + 3] = 0;

	for (pos = 0; pos < buffer_size; pos++)
		printf("%02X ", ((uint8_t *)second_compress_buffer)[pos]);
	puts("");

	compressor.decodeArray((uint32_t *)second_compress_buffer, buffer_size, (uint32_t *)second_decompress_buffer, sequence_length);

	fail = false;
	for (pos = 0; pos < sequence_length; pos++)
		if (sequence[pos] != second_decompress_buffer[pos])
			{
			printf("p[%d]:%X != %X\n", pos, sequence[pos], second_decompress_buffer[pos]);
			fail = true;
			}
		else
			printf("p[%d]:%X == %X\n", pos, sequence[pos], second_decompress_buffer[pos]);

	if (fail)
		puts("Test failed");
	else
		puts("Test succeeded");
	}

	/*
		MAIN()
		------
	*/
	int main(void)
	{
	check(sequence,  sizeof(sequence) / sizeof(*sequence));
	}
#endif
/*
	ANT_COMPRESS_QMX_V2::DECODEARRAY()
	--------------------------------
	this code was generated by the method above.
*/
#include "compress_qmx_v2_decompress.cpp"
