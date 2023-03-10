#include "bg.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int versionNumber[4] = {BG_VER_MAJOR,BG_VER_MINOR,BG_VER_BUILD,BG_VER_REVISION};
//int BG_SIZE_TYPE = 8;

int dataEndianType = LITTLE_ENDIAN_DATA; //*endian type of the data read from disk
int sysEndianType; //*sysEndianType is actually set automatically.

//the confparams should be separate between compression and decopmression, in case of mutual-affection when calling compression/decompression alternatively
bg_params *confparams_cpr = NULL; //used for compression
bg_params *confparams_dec = NULL; //used for decompression 

bg_exedata *exe_params = NULL;

int bgMode_libpressio = BITGROOM;
int errorControlMode_libpressio = BG_NSD;
int nsd_libpressio = 5;
int dsd_libpressio = 5;

#if 0
unsigned char *BG_compress(int dataType, void *data, size_t *outSize, size_t nbEle)
{
	return BG_compress_args(dataType, data, outSize, confparams_cpr->bgMode, confparams_cpr->errorControlMode, confparams_cpr->NSD, confparams_cpr->DSD, nbEle);
}
#endif

unsigned char* BG_compress_args(int dataType, void *data, size_t *outSize, int bgMode, int errorControlMode, int nsd, int dsd, size_t nbEle, unsigned char *data_)
{

	int dataTypeLen = dataType==BG_FLOAT?sizeof(float):sizeof(double);	

	size_t bufferSize = dataTypeLen*nbEle;

	const double bit_per_dcm_dgt_prc=M_LN10/M_LN2; /* 3.32 [frc] Bits per decimal digit of precision */
	//const double dcm_per_bit_dgt_prc=M_LN2/M_LN10; /* 0.301 [frc] Bits per decimal digit of precision */
	const int bit_xpl_nbr_sgn_flt=23; /* [nbr] Bits 0-22 of SP significands are explicit. Bit 23 is implicitly 1. */
	const int bit_xpl_nbr_sgn_dbl=53; /* [nbr] Bits 0-52 of DP significands are explicit. Bit 53 is implicitly 1. */
	//const int ieee_xpn_fst_flt=127; /* [nbr] IEEE "exponent bias" = actual exponent minus stored exponent */  	

	double prc_bnr_xct; /* [nbr] Binary digits of precision, exact */

	int bit_xpl_nbr_sgn=int_CEWI; /* [nbr] Number of explicit bits in significand */
	int bit_xpl_nbr_zro; /* [nbr] Number of explicit bits to zero */

	long idx;

	unsigned int *u32_ptr;
	unsigned int msk_f32_u32_zro;
	unsigned int msk_f32_u32_one;
	//unsigned int msk_f32_u32_hshv;
	unsigned long long *u64_ptr;
	unsigned long long msk_f64_u64_zro;
	unsigned long long msk_f64_u64_one;
	//unsigned long int msk_f64_u64_hshv;
	unsigned short prc_bnr_ceil; /* [nbr] Exact binary digits of precision rounded-up */
	unsigned short prc_bnr_xpl_rqr; /* [nbr] Explicitly represented binary digits required to retain */

	if(errorControlMode == BG_NSD && (nsd < 0 || nsd >16))
	{
		printf("Error: wrong nsd input\n");
		return NULL;
	}

	/* How many bits to preserve? */
	prc_bnr_xct=nsd*bit_per_dcm_dgt_prc;
	/* Be conservative, round upwards */
	prc_bnr_ceil=(unsigned short)ceil(prc_bnr_xct);
	/* First bit is implicit not explicit but corner cases prevent our taking advantage of this */
	//prc_bnr_xpl_rqr=prc_bnr_ceil-1;
	//prc_bnr_xpl_rqr=prc_bnr_ceil;
	prc_bnr_xpl_rqr=prc_bnr_ceil+1;

	//unsigned char* data_ = (unsigned char*)malloc(bufferSize);		
	memcpy(data_, data, bufferSize);

	if(dataType == BG_DOUBLE) prc_bnr_xpl_rqr++; /* Seems necessary for double-precision ppc=array(1.234567,1.0e-6,$dmn) */  
	
	if(!(dataType == BG_FLOAT  && prc_bnr_xpl_rqr >= bit_xpl_nbr_sgn_flt) || (dataType == BG_DOUBLE && prc_bnr_xpl_rqr >= bit_xpl_nbr_sgn_dbl)) //required # bits is greater than the full length of bits	
	{	
		if(dataType==BG_FLOAT)
		{
			bit_xpl_nbr_sgn=bit_xpl_nbr_sgn_flt;
			bit_xpl_nbr_zro=bit_xpl_nbr_sgn-prc_bnr_xpl_rqr;		
			if(bit_xpl_nbr_zro > bit_xpl_nbr_sgn-NCO_PPC_BIT_XPL_NBR_MIN)
			{
				printf("Error: bit_xpl_nbr_zro > bit_xpl_nbr_sgn-NCO_PPC_BIT_XPL_NBR_MIN\n");
				return NULL;
			}	
			
			u32_ptr = (unsigned int*)data_;
			/* Create mask */
			msk_f32_u32_zro=0u; /* Zero all bits */
			msk_f32_u32_zro=~msk_f32_u32_zro; /* Turn all bits to ones */
			/* Bit Shave mask for AND: Left shift zeros into bits to be rounded, leave ones in untouched bits */
			msk_f32_u32_zro <<= bit_xpl_nbr_zro;
			/* Bit Set   mask for OR:  Put ones into bits to be set, zeros in untouched bits */
			msk_f32_u32_one=~msk_f32_u32_zro;
			//msk_f32_u32_hshv=msk_f32_u32_one & (msk_f32_u32_zro >> 1); /* Set one bit: the MSB of LSBs */	
			switch(bgMode)
			{
			case BITGROOM:
				for(idx=0L;idx<nbEle;idx+=2L) u32_ptr[idx]&=msk_f32_u32_zro;
				
				for(idx=1L;idx<nbEle;idx+=2L)
					if(u32_ptr[idx] != 0U) /* Never quantize upwards floating point values of zero */
						u32_ptr[idx]|=msk_f32_u32_one;		
				break;
			case BITSHAVE:
				for(idx=0L;idx<nbEle;idx++) u32_ptr[idx]&=msk_f32_u32_zro;
				break;
			case BITSET:
				for(idx=0L;idx<nbEle;idx++)
					if(u32_ptr[idx] != 0U) /* Never quantize upwards floating point values of zero */
						u32_ptr[idx]|=msk_f32_u32_one;		
				break;
			}	
		}
		else //BG_DOUBLE
		{
			bit_xpl_nbr_sgn=bit_xpl_nbr_sgn_dbl;
			bit_xpl_nbr_zro=bit_xpl_nbr_sgn-prc_bnr_xpl_rqr;
			if(bit_xpl_nbr_zro > bit_xpl_nbr_sgn-NCO_PPC_BIT_XPL_NBR_MIN)
			{
				printf("Error: bit_xpl_nbr_zro > bit_xpl_nbr_sgn-NCO_PPC_BIT_XPL_NBR_MIN\n");
				return NULL;
			}	
			
			u64_ptr=(unsigned long int*)data_;
			/* Create mask */
			msk_f64_u64_zro=0ul; /* Zero all bits */
			msk_f64_u64_zro=~msk_f64_u64_zro; /* Turn all bits to ones */
			/* Bit Shave mask for AND: Left shift zeros into bits to be rounded, leave ones in untouched bits */
			msk_f64_u64_zro <<= bit_xpl_nbr_zro;
			/* Bit Set   mask for OR:  Put ones into bits to be set, zeros in untouched bits */
			msk_f64_u64_one=~msk_f64_u64_zro;
			//msk_f64_u64_hshv=msk_f64_u64_one & (msk_f64_u64_zro >> 1); /* Set one bit: the MSB of LSBs */		
			switch(bgMode)
			{
			case BITGROOM:
			for(idx=0L;idx<nbEle;idx+=2L) u64_ptr[idx]&=msk_f64_u64_zro;
				for(idx=1L;idx<nbEle;idx+=2L)
					if(u64_ptr[idx] != 0UL) /* Never quantize upwards floating point values of zero */
						u64_ptr[idx]|=msk_f64_u64_one;			
				break;
			case BITSHAVE:
				for(idx=0L;idx<nbEle;idx++) u64_ptr[idx]&=msk_f64_u64_zro;
				break;
			case BITSET:
				for(idx=0L;idx<nbEle;idx++)
					if(u64_ptr[idx] != 0UL) /* Never quantize upwards floating point values of zero */
						u64_ptr[idx]|=msk_f64_u64_one;				
				break;
			}	
		}	
	}
	
	//perform DEFLATE algorithm by Zlib
	//unsigned char* outBytes = NULL;
	//*outSize = zlib_compress5(data_, bufferSize, &outBytes, 1);
	
	//free(data_);
	return data_; //outBytes;

}
#if 0
void *BG_decompress(int dataType, unsigned char *bytes, size_t byteLength, size_t nbEle)
{	
	if(dataType==BG_FLOAT)
	{
		unsigned char* decompressedData;
		zlib_uncompress5(bytes, byteLength, &decompressedData, nbEle*sizeof(float));
		return decompressedData;
	}
	else //BG_DOUBLE
	{
		unsigned char* decompressedData;
		zlib_uncompress5(bytes, byteLength, &decompressedData, nbEle*sizeof(double));
		return decompressedData;
	}
}
#endif
