/*
	COMPRESS_QMX.C
	--------------
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
#include "compress_qmx.h"
/*
	class COMPRESS_QMX
	------------------
*/
class compress_qmx
{
private:
	uint8_t *length_buffer;
	uint64_t length_buffer_length;

public:
	compress_qmx();
	virtual ~compress_qmx();

	virtual void encodeArray(const uint32_t *in, uint64_t len, uint32_t *out, uint64_t *nvalue);
	virtual void decodeArray(const uint32_t *in, uint64_t len, uint32_t *out, uint64_t nvalue);
} ;

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
	COMPRESS_QMX::COMPRESS_QMX()
	----------------------------
*/
compress_qmx::compress_qmx()
{
length_buffer = NULL;
length_buffer_length = 0;
}

/*
	COMPRESS_QMX::!COMPRESS_QMX()
	-----------------------------
*/
compress_qmx::~compress_qmx()
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
	VBYTE_BYTES_NEEDED_FOR()
	------------------------
*/
static inline uint32_t vbyte_bytes_needed_for(uint32_t docno)
{
if (docno < (1 << 7))
	return 1;
else if (docno < (1 << 14))
	return 2;
else if (docno < (1 << 21))
	return 3;
else if (docno < (1 << 28))
	return 4;
else
	return 5;
}

/*
	VBYTE_COMPRESS_INTO()
	---------------------
	NOTE:	We compress "backwards" because we want to keep decompressing from the end of the string
			to get the number
*/
static inline void vbyte_compress_into(uint8_t *dest, uint32_t docno)
{
if (docno < (1 << 7))
	dest[0] = (docno & 0x7F) | 0x80;
else if (docno < (1 << 14))
	{
	dest[1] = (docno >> 7) & 0x7F;
	dest[0] = (docno & 0x7F) | 0x80;
	}
else if (docno < (1 << 21))
	{
	dest[2] = (docno >> 14) & 0x7F;
	dest[1] = (docno >> 7) & 0x7F;
	dest[0] = (docno & 0x7F) | 0x80;
	}
else if (docno < (1 << 28))
	{
	dest[3] = (docno >> 21) & 0x7F;
	dest[2] = (docno >> 14) & 0x7F;
	dest[1] = (docno >> 7) & 0x7F;
	dest[0] = (docno & 0x7F) | 0x80;
	}
else
	{
	dest[4] = (docno >> 28) & 0x7F;
	dest[3] = (docno >> 21) & 0x7F;
	dest[2] = (docno >> 14) & 0x7F;
	dest[1] = (docno >> 7) & 0x7F;
	dest[0] = (docno & 0x7F) | 0x80;
	}
}

/*
	VBYTE_DECOMPRESS()
	------------------
	NOTE:	this method is given a ponter to the end of the v-byte compressed
			integer.  The task is to work backwards until it gets the integer
*/
static inline uint32_t vbyte_decompress(uint8_t *source) 
{
uint32_t result;

if (*source & 0x80)
	return *source & 0x7F;
else
	{
	result = *source--;

	while (!(*source & 0x80))
	   result = (result << 7) | *source--;

	return (result << 7) | (*source & 0x7F);
	}
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
	COMPRESS_QMX::ENCODEARRAY()
	---------------------------
*/
void compress_qmx::encodeArray(const uint32_t *source, uint64_t source_integers, uint32_t *into, uint64_t *nvalue)
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
	length_buffer = new uint8_t [(size_t)((length_buffer_length = source_integers) + WASTAGE)];
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
	switch (*current_length)
		{
		case 0:
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
	Copy the lengths to the end
*/
memcpy(destination, length_buffer, keys - length_buffer);
destination += keys - length_buffer;

/*
	Add the pointer to the lengths
*/
uint32_t val = keys - length_buffer + vbyte_bytes_needed_for(keys - length_buffer); // offset (from the end) to the start of the keys
if (vbyte_bytes_needed_for(val) > vbyte_bytes_needed_for(keys - length_buffer))
	val = keys - length_buffer + vbyte_bytes_needed_for(val); 				// although rare, this happens when adding the length of the vbyte encoded length makes the vbyte encoding one byte longer (i.e. 127)
vbyte_compress_into(destination, val);

destination += vbyte_bytes_needed_for(val);


/*
	Compute the length (in bytes)
*/
*nvalue = destination - (uint8_t *)into;	// return length in bytes
}

#ifdef MAKE_DECOMPRESS
	/*
		The following program generates the source code for compress_runlength::decodeArray()
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
	printf("void compress_qmx::decodeArray(const uint32_t *source, uint64_t len, uint32_t *to, uint64_t destination_integers)\n");
	printf("{\n");
	printf("__m128i byte_stream, byte_stream_2, tmp, tmp2, mask_21, mask_12, mask_10, mask_9, mask_7, mask_6, mask_5, mask_4, mask_3, mask_2, mask_1;\n");
	printf("uint8_t *in = (uint8_t *)source;\n");
	printf("uint32_t *end = to + destination_integers;\n");
	printf("uint32_t key_start = vbyte_decompress((uint8_t *)source + len - 1);\n");
	printf("uint8_t *keys = (uint8_t *)source + len - key_start;\n");

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

	printf("while (to < end)\n");
	printf("\t{\n");
	printf("\tswitch (*keys++)\n");
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
			printf("\t\t\tbyte_stream = _mm_loadu_si128((__m128i *)in);");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));");
			printf("\t\t\tbyte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));");
			printf("\t\t\t_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));");

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
	static uint32_t sequence[]={0x333,0xC7,0x21C,0x78F,0x66A,0x787,0xD0C,0xEE,0x416,0x2F8,0x410,0xFF3,0x7A7,0x35C,0x5A8,0x4ED,0x3AD,0x121,0x3A7,0x5EC,0x53,0x50C,0xFD6,0x697,0xF4,0x894,0xB5F,0x381,0x10C,0xB1E,0x2E4,0x32,0x7EB,0x1C6,0x1DB,0xE3,0x27,0x920,0x262,0x718,0x95,0x7C0,0x155,0x8F,0x83A,0x1178,0xCEF,0x7DC,0x3CB,0x30E,0x2EA,0x16F,0x212,0x4A,0x9F0,0x233,0x7,0x9F7,0x1EE,0x91,0x12FD,0x7C,0x291,0x203,0x2F8,0x39B,0x411,0x61C,0x3E2,0x1DF,0xCD7,0x5DA,0xD35,0x21,0x1C8D,0x25,0x313,0x314,0xBBB,0xFB,0x1E2,0x60,0x3F5,0x513,0x3AC,0x769,0x45E,0x485,0x1BA,0x17B,0x2DC,0x173,0x151,0x163E,0x101,0xE9D,0xB67,0x28B,0x4CA,0x955,0x6B3,0x112,0x225,0x742,0x432,0x453,0x3CF,0x541,0xCCE,0xDB6,0x406,0x58,0x202,0x647,0x9F,0x29,0x153,0x51E,0x233,0x7A3,0x731,0x3A,0xA0,0xD23,0x3C7,0xD1,0x5C,0xB90,0x22C,0xE8,0x78B,0x5E3};

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
  	compress_qmx compressor;
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
	COMPRESS_QMX::DECODEARRAY()
	---------------------------
	this code was generated by the method above.
*/
static uint32_t ALIGN_16 static_mask_21[]  = {0x1fffff, 0x1fffff, 0x1fffff, 0x1fffff};
static uint32_t ALIGN_16 static_mask_12[]  = {0xfff, 0xfff, 0xfff, 0xfff};
static uint32_t ALIGN_16 static_mask_10[] = {0x3ff, 0x3ff, 0x3ff, 0x3ff};
static uint32_t ALIGN_16 static_mask_9[]  = {0x1ff, 0x1ff, 0x1ff, 0x1ff};
static uint32_t ALIGN_16 static_mask_7[]  = {0x7f, 0x7f, 0x7f, 0x7f};
static uint32_t ALIGN_16 static_mask_6[]  = {0x3f, 0x3f, 0x3f, 0x3f};
static uint32_t ALIGN_16 static_mask_5[]  = {0x1f, 0x1f, 0x1f, 0x1f};
static uint32_t ALIGN_16 static_mask_4[]  = {0x0f, 0x0f, 0x0f, 0x0f};
static uint32_t ALIGN_16 static_mask_3[]  = {0x07, 0x07, 0x07, 0x07};
static uint32_t ALIGN_16 static_mask_2[]  = {0x03, 0x03, 0x03, 0x03};
static uint32_t ALIGN_16 static_mask_1[]  = {0x01, 0x01, 0x01, 0x01};
void compress_qmx::decodeArray(const uint32_t *source, uint64_t len, uint32_t *to, uint64_t destination_integers)
{
__m128i byte_stream, byte_stream_2, tmp, tmp2, mask_21, mask_12, mask_10, mask_9, mask_7, mask_6, mask_5, mask_4, mask_3, mask_2, mask_1;
uint8_t *in = (uint8_t *)source;
uint32_t *end = to + destination_integers;
uint32_t key_start = vbyte_decompress((uint8_t *)source + len - 1);
uint8_t *keys = (uint8_t *)source + len - key_start;

mask_21 = _mm_loadu_si128((__m128i *)static_mask_21);
mask_12 = _mm_loadu_si128((__m128i *)static_mask_12);
mask_10 = _mm_loadu_si128((__m128i *)static_mask_10);
mask_9 = _mm_loadu_si128((__m128i *)static_mask_9);
mask_7 = _mm_loadu_si128((__m128i *)static_mask_7);
mask_6 = _mm_loadu_si128((__m128i *)static_mask_6);
mask_5 = _mm_loadu_si128((__m128i *)static_mask_5);
mask_4 = _mm_loadu_si128((__m128i *)static_mask_4);
mask_3 = _mm_loadu_si128((__m128i *)static_mask_3);
mask_2 = _mm_loadu_si128((__m128i *)static_mask_2);
mask_1 = _mm_loadu_si128((__m128i *)static_mask_1);

while (to < end)
	{
	switch (*keys++)
		{
		case 0x00:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x01:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x02:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x03:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x04:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x05:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x06:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x07:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x08:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x09:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x0a:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x0b:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x0c:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x0d:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x0e:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
		case 0x0f:
#ifdef NO_ZEROS
			tmp = _mm_loadu_si128((__m128i *)static_mask_1);
#else
			tmp = _mm_castps_si128(_mm_xor_ps(_mm_cvtepu8_epi32(tmp), _mm_cvtepu8_epi32(tmp)));
#endif
			_mm_storeu_si128((__m128i *)to, tmp);
			_mm_storeu_si128((__m128i *)to + 1, tmp);
			_mm_storeu_si128((__m128i *)to + 2, tmp);
			_mm_storeu_si128((__m128i *)to + 3, tmp);
			_mm_storeu_si128((__m128i *)to + 4, tmp);
			_mm_storeu_si128((__m128i *)to + 5, tmp);
			_mm_storeu_si128((__m128i *)to + 6, tmp);
			_mm_storeu_si128((__m128i *)to + 7, tmp);
			_mm_storeu_si128((__m128i *)to + 8, tmp);
			_mm_storeu_si128((__m128i *)to + 9, tmp);
			_mm_storeu_si128((__m128i *)to + 10, tmp);
			_mm_storeu_si128((__m128i *)to + 11, tmp);
			_mm_storeu_si128((__m128i *)to + 12, tmp);
			_mm_storeu_si128((__m128i *)to + 13, tmp);
			_mm_storeu_si128((__m128i *)to + 14, tmp);
			_mm_storeu_si128((__m128i *)to + 15, tmp);
			_mm_storeu_si128((__m128i *)to + 16, tmp);
			_mm_storeu_si128((__m128i *)to + 17, tmp);
			_mm_storeu_si128((__m128i *)to + 18, tmp);
			_mm_storeu_si128((__m128i *)to + 19, tmp);
			_mm_storeu_si128((__m128i *)to + 20, tmp);
			_mm_storeu_si128((__m128i *)to + 21, tmp);
			_mm_storeu_si128((__m128i *)to + 22, tmp);
			_mm_storeu_si128((__m128i *)to + 23, tmp);
			_mm_storeu_si128((__m128i *)to + 24, tmp);
			_mm_storeu_si128((__m128i *)to + 25, tmp);
			_mm_storeu_si128((__m128i *)to + 26, tmp);
			_mm_storeu_si128((__m128i *)to + 27, tmp);
			_mm_storeu_si128((__m128i *)to + 28, tmp);
			_mm_storeu_si128((__m128i *)to + 29, tmp);
			_mm_storeu_si128((__m128i *)to + 30, tmp);
			_mm_storeu_si128((__m128i *)to + 31, tmp);
			_mm_storeu_si128((__m128i *)to + 32, tmp);
			_mm_storeu_si128((__m128i *)to + 33, tmp);
			_mm_storeu_si128((__m128i *)to + 34, tmp);
			_mm_storeu_si128((__m128i *)to + 35, tmp);
			_mm_storeu_si128((__m128i *)to + 36, tmp);
			_mm_storeu_si128((__m128i *)to + 37, tmp);
			_mm_storeu_si128((__m128i *)to + 38, tmp);
			_mm_storeu_si128((__m128i *)to + 39, tmp);
			_mm_storeu_si128((__m128i *)to + 40, tmp);
			_mm_storeu_si128((__m128i *)to + 41, tmp);
			_mm_storeu_si128((__m128i *)to + 42, tmp);
			_mm_storeu_si128((__m128i *)to + 43, tmp);
			_mm_storeu_si128((__m128i *)to + 44, tmp);
			_mm_storeu_si128((__m128i *)to + 45, tmp);
			_mm_storeu_si128((__m128i *)to + 46, tmp);
			_mm_storeu_si128((__m128i *)to + 47, tmp);
			_mm_storeu_si128((__m128i *)to + 48, tmp);
			_mm_storeu_si128((__m128i *)to + 49, tmp);
			_mm_storeu_si128((__m128i *)to + 50, tmp);
			_mm_storeu_si128((__m128i *)to + 51, tmp);
			_mm_storeu_si128((__m128i *)to + 52, tmp);
			_mm_storeu_si128((__m128i *)to + 53, tmp);
			_mm_storeu_si128((__m128i *)to + 54, tmp);
			_mm_storeu_si128((__m128i *)to + 55, tmp);
			_mm_storeu_si128((__m128i *)to + 56, tmp);
			_mm_storeu_si128((__m128i *)to + 57, tmp);
			_mm_storeu_si128((__m128i *)to + 58, tmp);
			_mm_storeu_si128((__m128i *)to + 59, tmp);
			_mm_storeu_si128((__m128i *)to + 60, tmp);
			_mm_storeu_si128((__m128i *)to + 61, tmp);
			_mm_storeu_si128((__m128i *)to + 62, tmp);
			_mm_storeu_si128((__m128i *)to + 63, tmp);
			to += 256;
			break;
		case 0x10:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x11:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x12:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x13:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x14:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x15:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x16:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x17:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x18:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x19:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x1a:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x1b:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x1c:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x1d:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x1e:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
		case 0x1f:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 16, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 17, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 18, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 19, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 20, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 21, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 22, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 23, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 24, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 25, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 26, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 27, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 28, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 29, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 30, _mm_and_si128(byte_stream, mask_1));
			byte_stream = _mm_srli_epi64(byte_stream, 1);
			_mm_storeu_si128((__m128i *)to + 31, _mm_and_si128(byte_stream, mask_1));
			in += 16;
			to += 128;
			break;
		case 0x20:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x21:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x22:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x23:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x24:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x25:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x26:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x27:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x28:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x29:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x2a:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x2b:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x2c:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x2d:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x2e:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
		case 0x2f:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 10, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 11, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 12, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 13, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 14, _mm_and_si128(byte_stream, mask_2));
			byte_stream = _mm_srli_epi64(byte_stream, 2);
			_mm_storeu_si128((__m128i *)to + 15, _mm_and_si128(byte_stream, mask_2));
			in += 16;
			to += 64;
			break;
		case 0x30:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x31:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x32:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x33:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x34:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x35:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x36:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x37:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x38:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x39:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x3a:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x3b:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x3c:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x3d:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x3e:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
		case 0x3f:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_3));
			byte_stream = _mm_srli_epi64(byte_stream, 3);
			_mm_storeu_si128((__m128i *)to + 9, _mm_and_si128(byte_stream, mask_3));
			in += 16;
			to += 40;
			break;
		case 0x40:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x41:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x42:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x43:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x44:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x45:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x46:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x47:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x48:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x49:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x4a:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x4b:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x4c:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x4d:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x4e:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
		case 0x4f:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_4));
			byte_stream = _mm_srli_epi64(byte_stream, 4);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_4));
			in += 16;
			to += 32;
			break;
		case 0x50:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x51:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x52:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x53:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x54:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x55:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x56:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x57:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x58:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x59:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x5a:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x5b:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x5c:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x5d:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x5e:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
		case 0x5f:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_5));
			byte_stream = _mm_srli_epi64(byte_stream, 5);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_5));
			in += 16;
			to += 24;
			break;
		case 0x60:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x61:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x62:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x63:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x64:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x65:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x66:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x67:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x68:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x69:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x6a:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x6b:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x6c:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x6d:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x6e:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
		case 0x6f:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_6));
			byte_stream = _mm_srli_epi64(byte_stream, 6);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_6));
			in += 16;
			to += 20;
			break;
		case 0x70:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x71:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x72:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x73:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x74:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x75:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x76:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x77:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x78:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x79:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x7a:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x7b:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x7c:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x7d:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x7e:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
		case 0x7f:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_7));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 4), _mm_srli_epi32(byte_stream, 7)), mask_7));
			byte_stream = _mm_srli_epi32(byte_stream_2, 3);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 7, _mm_and_si128(byte_stream, mask_7));
			byte_stream = _mm_srli_epi32(byte_stream, 7);
			_mm_storeu_si128((__m128i *)to + 8, _mm_and_si128(byte_stream, mask_7));
			in += 32;
			to += 36;
			break;
		case 0x80:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x81:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x82:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x83:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x84:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x85:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x86:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x87:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x88:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x89:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x8a:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x8b:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x8c:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x8d:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x8e:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
		case 0x8f:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu8_epi32(tmp2));
			tmp = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)));
			_mm_storeu_si128((__m128i *)to + 2, _mm_cvtepu8_epi32(tmp));
			tmp2 = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp), 0x01));
			_mm_storeu_si128((__m128i *)to + 3, _mm_cvtepu8_epi32(tmp2));
			in += 16;
			to += 16;
			break;
		case 0x90:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x91:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x92:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x93:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x94:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x95:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x96:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x97:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x98:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x99:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x9a:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x9b:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x9c:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x9d:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x9e:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
		case 0x9f:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_9));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 5), _mm_srli_epi32(byte_stream, 9)), mask_9));
			byte_stream = _mm_srli_epi32(byte_stream_2, 4);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 5, _mm_and_si128(byte_stream, mask_9));
			byte_stream = _mm_srli_epi32(byte_stream, 9);
			_mm_storeu_si128((__m128i *)to + 6, _mm_and_si128(byte_stream, mask_9));
			in += 32;
			to += 28;
			break;
		case 0xa0:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa1:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa2:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa3:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa4:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa5:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa6:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa7:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa8:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xa9:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xaa:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xab:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xac:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xad:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xae:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
		case 0xaf:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_10));
			byte_stream = _mm_srli_epi64(byte_stream, 10);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(byte_stream, mask_10));
			in += 16;
			to += 12;
			break;
		case 0xb0:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb1:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb2:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb3:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb4:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb5:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb6:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb7:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb8:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xb9:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xba:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xbb:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xbc:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xbd:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xbe:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
		case 0xbf:
			byte_stream = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(byte_stream, mask_12));
			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);
			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 8), _mm_srli_epi32(byte_stream, 12)), mask_12));
			byte_stream = _mm_srli_epi32(byte_stream_2, 8);
			_mm_storeu_si128((__m128i *)to + 3, _mm_and_si128(byte_stream, mask_12));
			byte_stream = _mm_srli_epi32(byte_stream, 12);
			_mm_storeu_si128((__m128i *)to + 4, _mm_and_si128(byte_stream, mask_12));
			in += 32;
			to += 20;
			break;
		case 0xc0:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc1:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc2:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc3:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc4:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc5:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc6:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc7:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc8:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xc9:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xca:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xcb:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xcc:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xcd:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xce:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
		case 0xcf:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, _mm_cvtepu16_epi32(tmp));
			_mm_storeu_si128((__m128i *)to + 1, _mm_cvtepu16_epi32(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(tmp), _mm_castsi128_ps(tmp)))));
			in += 16;
			to += 8;
			break;
		case 0xd0:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd1:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd2:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd3:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd4:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd5:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd6:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd7:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd8:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xd9:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xda:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xdb:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xdc:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xdd:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xde:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
		case 0xdf:
			byte_stream = _mm_loadu_si128((__m128i *)in);			_mm_storeu_si128((__m128i *)to, _mm_and_si128(byte_stream, mask_21));			byte_stream_2 = _mm_loadu_si128((__m128i *)in + 1);			_mm_storeu_si128((__m128i *)to + 1, _mm_and_si128(_mm_or_si128(_mm_slli_epi32(byte_stream_2, 11), _mm_srli_epi32(byte_stream, 21)), mask_21));			_mm_storeu_si128((__m128i *)to + 2, _mm_and_si128(_mm_srli_epi32(byte_stream_2, 11), mask_21));			in += 32;
			to += 12;
			break;
		case 0xe0:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe1:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe2:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe3:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe4:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe5:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe6:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe7:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe8:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xe9:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xea:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xeb:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xec:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xed:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xee:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
		case 0xef:
			tmp = _mm_loadu_si128((__m128i *)in);
			_mm_storeu_si128((__m128i *)to, tmp);
			in += 16;
			to += 4;
			break;
		case 0xf0:
			in++;
		case 0xf1:
			in++;
		case 0xf2:
			in++;
		case 0xf3:
			in++;
		case 0xf4:
			in++;
		case 0xf5:
			in++;
		case 0xf6:
			in++;
		case 0xf7:
			in++;
		case 0xf8:
			in++;
		case 0xf9:
			in++;
		case 0xfa:
			in++;
		case 0xfb:
			in++;
		case 0xfc:
			in++;
		case 0xfd:
			in++;
		case 0xfe:
			in++;
		case 0xff:
			in++;
			break;
		}
	}
}

unsigned char *qmx_enc( const uint32_t *in, unsigned n, unsigned char *out)
{ compress_qmx compressor;
  uint64_t r;
  compressor.encodeArray(in, n, (uint32_t *)out, &r);
  return out + r;
}

unsigned char *qmx_dec(const unsigned char *in, unsigned len, uint32_t *out, unsigned n)
{ compress_qmx compressor;	
  compressor.decodeArray((uint32_t *)in, len, out, n);	
  return (unsigned char *)in + len;  
}
