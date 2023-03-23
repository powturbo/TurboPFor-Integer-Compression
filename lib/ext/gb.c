// copy from https://github.com/ccr/ccr/tree/master/hdf5_plugins for benchmarking purpose 
# define NC_FLOAT 5
# define NC_DOUBLE 6
# define NC_FILL_FLOAT   (9.9692099683868690e+36f) /* near 15 * 2^119 */
# define NC_FILL_DOUBLE  (9.9692099683868690e+36)

/* Minimum number of explicit significand bits to preserve when zeroing/bit-masking floating point values
   Codes will preserve at least two explicit bits, IEEE significand representation contains one implicit bit
   Thus preserve a least three bits which is approximately one sigificant decimal digit
   Used in nco_ppc_bitmask() and nco_ppc_bitmask_scl() */
#define NCO_PPC_BIT_XPL_NBR_MIN 2

/* Pointer union for floating point and bitmask types */
typedef union{ /* ptr_unn */
  float *fp;
  double *dp;
  unsigned int *ui32p;
  unsigned long long *ui64p;
  void *vp;
} ptr_unn;

void ccr_gbr 			/* [fnc] Granular BitRound buffer of float values */
(const int nsd, 		/* I [nbr] Number of decimal significant digits to quantize to */
 const int type, 		/* I [enm] netCDF type of operand */
 const size_t sz, 		/* I [nbr] Size (in elements) of buffer to quantize */
 const int has_mss_val, /* I [flg] Flag for missing values */
 ptr_unn mss_val, 		/* I [val] Value of missing value */
 void *op1) 			/* I/O [frc] Values to quantize */
{
  const char fnc_nm[] = "ccr_gbr()"; /* [sng] Function name */

  /* Prefer constants defined in math.h, however, ...
     20201002 GCC environments can have hard time defining M_LN10/M_LN2 despite finding math.h */
#ifndef M_LN10
# define M_LN10         2.30258509299404568402  /* log_e 10 */
#endif /* M_LN10 */
#ifndef M_LN2
# define M_LN2          0.69314718055994530942  /* log_e 2 */
#endif /* M_LN2 */
  const double bit_per_dgt=M_LN10/M_LN2; /* 3.32 [frc] Bits per decimal digit of precision = log2(10) */
  const double dgt_per_bit=M_LN2/M_LN10; /* 0.301 [frc] Decimal digits per bit of precision = log10(2) */
  
  const int bit_xpl_nbr_sgn_flt=23; /* [nbr] Bits 0-22 of SP significands are explicit. Bit 23 is implicitly 1. */
  const int bit_xpl_nbr_sgn_dbl=52; /* [nbr] Bits 0-51 of DP significands are explicit. Bit 52 is implicitly 1. */
  
  double mnt; /* [frc] Mantissa, 0.5 <= mnt < 1.0 */
  double mnt_fabs; /* [frc] fabs(mantissa) */
  double mnt_log10_fabs; /* [frc] log10(fabs(mantissa))) */
  double val; /* [frc] Copy of input value to avoid indirection */
  
  double prc_bnr_xct=0.0; /* [nbr] Binary digits of precision, exact */
  double mss_val_cmp_dbl; /* Missing value for comparison to double precision values */

  float mss_val_cmp_flt; /* Missing value for comparison to single precision values */
  
  int bit_xpl_nbr_sgn=-1; /* [nbr] Number of explicit bits in significand */
  int bit_xpl_nbr_zro; /* [nbr] Number of explicit bits to zero */

  int dgt_nbr; /* [nbr] Number of digits before decimal point */
  int qnt_pwr; /* [nbr] Power of two in quantization mask: qnt_msk = 2^qnt_pwr */
  int xpn_bs2; /* [nbr] Binary exponent xpn_bs2 in val = sign(val) * 2^xpn_bs2 * mnt, 0.5 < mnt <= 1.0 */

  size_t idx;

  unsigned int *u32_ptr;
  unsigned int msk_f32_u32_zro;
  unsigned int msk_f32_u32_one;
  unsigned int msk_f32_u32_hshv;
  unsigned long long int *u64_ptr;
  unsigned long long int msk_f64_u64_zro;
  unsigned long long int msk_f64_u64_one;
  unsigned long long int msk_f64_u64_hshv;
  unsigned short prc_bnr_ceil=0; /* [nbr] Exact binary digits of precision rounded-up */
  unsigned short prc_bnr_xpl_rqr=0; /* [nbr] Explicitly represented binary digits required to retain */

  /* Disallow unreasonable quantization */
  //assert(nsd > 0);
  //assert(nsd <= 16);

  if(type == NC_FLOAT  && prc_bnr_xpl_rqr >= bit_xpl_nbr_sgn_flt) return;
  if(type == NC_DOUBLE && prc_bnr_xpl_rqr >= bit_xpl_nbr_sgn_dbl) return;

  switch(type){
  case NC_FLOAT:
    /* Missing value for comparison is _FillValue (if any) otherwise default NC_FILL_FLOAT/DOUBLE */
    if(has_mss_val) mss_val_cmp_flt=*mss_val.fp; else mss_val_cmp_flt=NC_FILL_FLOAT;
    bit_xpl_nbr_sgn=bit_xpl_nbr_sgn_flt;
    u32_ptr=op1; //.ui32p;
	float *fp = op1;

    for(idx=0L;idx<sz;idx++){
      if((val=fp[idx]) != mss_val_cmp_flt && u32_ptr[idx] != 0U){
	mnt=frexp(val,&xpn_bs2); /* DGG19 p. 4102 (8) */
	mnt_fabs=fabs(mnt);
	mnt_log10_fabs=log10(mnt_fabs);
	/* 20211003 Continuous determination of dgt_nbr improves CR by ~10% */
	dgt_nbr=(int)floor(xpn_bs2*dgt_per_bit+mnt_log10_fabs)+1; /* DGG19 p. 4102 (8.67) */
	qnt_pwr=(int)floor(bit_per_dgt*(dgt_nbr-nsd)); /* DGG19 p. 4101 (7) */
	prc_bnr_xpl_rqr= mnt_fabs == 0.0 ? 0 : abs((int)floor(xpn_bs2-bit_per_dgt*mnt_log10_fabs)-qnt_pwr); /* Protect against mnt = -0.0 */
	prc_bnr_xpl_rqr--; /* 20211003 Reduce formula result by 1 bit: Passes all tests, improves CR by ~10% */

	bit_xpl_nbr_zro=bit_xpl_nbr_sgn-prc_bnr_xpl_rqr;
	msk_f32_u32_zro=0u; /* Zero all bits */
	msk_f32_u32_zro=~msk_f32_u32_zro; /* Turn all bits to ones */
	/* Bit Shave mask for AND: Left shift zeros into bits to be rounded, leave ones in untouched bits */
	msk_f32_u32_zro <<= bit_xpl_nbr_zro;
	/* Bit Set   mask for OR:  Put ones into bits to be set, zeros in untouched bits */
	msk_f32_u32_one=~msk_f32_u32_zro;
	msk_f32_u32_hshv=msk_f32_u32_one & (msk_f32_u32_zro >> 1); /* Set one bit: the MSB of LSBs */
	u32_ptr[idx]+=msk_f32_u32_hshv; /* Add 1 to the MSB of LSBs, carry 1 to mantissa or even exponent */
	u32_ptr[idx]&=msk_f32_u32_zro; /* Shave it */
      } /* !mss_val_cmp_flt */
    } /* !idx */
    break; /* !NC_FLOAT */
  case NC_DOUBLE:
    /* Missing value for comparison is _FillValue (if any) otherwise default NC_FILL_FLOAT/DOUBLE */
    if(has_mss_val) mss_val_cmp_dbl=*mss_val.dp; else mss_val_cmp_dbl=NC_FILL_FLOAT;
    bit_xpl_nbr_sgn=bit_xpl_nbr_sgn_dbl;
    u64_ptr=op1; 
	double *dp = op1;

    for(idx=0L;idx<sz;idx++){
      if((val=dp[idx]) != mss_val_cmp_dbl && u64_ptr[idx] != 0U){
	mnt=frexp(val,&xpn_bs2); /* DGG19 p. 4102 (8) */
	mnt_fabs=fabs(mnt);
	mnt_log10_fabs=log10(mnt_fabs);
	/* 20211003 Continuous determination of dgt_nbr improves CR by ~10% */
	dgt_nbr=(int)floor(xpn_bs2*dgt_per_bit+mnt_log10_fabs)+1; /* DGG19 p. 4102 (8.67) */
	qnt_pwr=(int)floor(bit_per_dgt*(dgt_nbr-nsd)); /* DGG19 p. 4101 (7) */
	prc_bnr_xpl_rqr= mnt_fabs == 0.0 ? 0 : abs((int)floor(xpn_bs2-bit_per_dgt*mnt_log10_fabs)-qnt_pwr); /* Protect against mnt = -0.0 */
	prc_bnr_xpl_rqr--; /* 20211003 Reduce formula result by 1 bit: Passes all tests, improves CR by ~10% */

	bit_xpl_nbr_zro=bit_xpl_nbr_sgn-prc_bnr_xpl_rqr;
	msk_f64_u64_zro=0u; /* Zero all bits */
	msk_f64_u64_zro=~msk_f64_u64_zro; /* Turn all bits to ones */
	/* Bit Shave mask for AND: Left shift zeros into bits to be rounded, leave ones in untouched bits */
	msk_f64_u64_zro <<= bit_xpl_nbr_zro;
	/* Bit Set   mask for OR:  Put ones into bits to be set, zeros in untouched bits */
	msk_f64_u64_one=~msk_f64_u64_zro;
	msk_f64_u64_hshv=msk_f64_u64_one & (msk_f64_u64_zro >> 1); /* Set one bit: the MSB of LSBs */
	u64_ptr[idx]+=msk_f64_u64_hshv; /* Add 1 to the MSB of LSBs, carry 1 to mantissa or even exponent */
	u64_ptr[idx]&=msk_f64_u64_zro; /* Shave it */
      } /* !mss_val_cmp_dbl */
    } /* !idx */
    break; /* !NC_DOUBLE */
  default: 
    (void)fprintf(stderr,"ERROR: %s reports datum size = %d B is invalid for %s filter\n",fnc_nm,type,""/*CCR_FLT_NAME*/);
    break;
  } /* !type */
  
} /* ccr_gbr() */
