//-- "Integer Compression: max.bits, delta, zigzag, xor" -----------------------------------------------------------------------
//---------- max. bit length + transform for sorted/unsorted arrays, delta,delta 1, delta > 1, zigzag, zigzag of delta, xor, FOR,----------------
#ifdef __cplusplus
extern "C" {
#endif
//------ ORed array, used to determine the maximum bit length of the elements in an unsorted integer array ---------------------
uint8_t  bit8(     uint8_t  *in, unsigned n, uint8_t  *px);
uint16_t bit16(    uint16_t *in, unsigned n, uint16_t *px);
uint32_t bit32(    uint32_t *in, unsigned n, uint32_t *px);
uint64_t bit64(    uint64_t *in, unsigned n, uint64_t *px);
uint32_t bit256v32(uint32_t *in, unsigned n, uint32_t *px);

//-------------- delta = 0: Sorted integer array w/ mindelta = 0 ----------------------------------------------
//-- ORed array, maximum bit length of the non decreasing integer array. out[i] = in[i] - in[i-1]
uint8_t  bitd8(     uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitd16(    uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitd32(    uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitd64(    uint64_t *in, unsigned n, uint64_t *px, uint64_t start);
uint32_t bitd256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);

//-- in-place reverse delta 0
void bitddec8(  uint8_t  *p,  unsigned n, uint8_t  start); // non decreasing (out[i] = in[i] - in[i-1])
void bitddec16( uint16_t *p,  unsigned n, uint16_t start);
void bitddec32( uint32_t *p,  unsigned n, uint32_t start);
void bitddec64( uint64_t *p,  unsigned n, uint64_t start);

//-- vectorized fast delta4 one: out[0] = in[4]-in[0], out[1]=in[5]-in[1], out[2]=in[6]-in[2], out[3]=in[7]-in[3],...
uint16_t bits128v16(   uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bits128v32(   uint32_t *in, unsigned n, uint32_t *px, uint32_t start);

//------------- delta = 1: Sorted integer array w/ mindelta = 1 ---------------------------------------------
//-- get delta maximum bit length of the non strictly decreasing integer array. out[i] = in[i] - in[i-1] - 1
uint8_t  bitd18( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitd116(uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitd132(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitd164(uint64_t *in, unsigned n, uint64_t *px, uint64_t start);
uint32_t bitd1256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);

//-- in-place reverse delta one
void bitd1dec8(     uint8_t  *p,  unsigned n, uint8_t  start); // non strictly decreasing (out[i] = in[i] - in[i-1] - 1)
void bitd1dec16(    uint16_t *p,  unsigned n, uint16_t start);
void bitd1dec32(    uint32_t *p,  unsigned n, uint32_t start);
void bitd1dec64(    uint64_t *p,  unsigned n, uint64_t start);

//------------- delta > 1: Sorted integer array w/ mindelta > 1 ---------------------------------------------
//-- ORed array, for max. bit length get min. delta ()
uint8_t  bitdi8(    uint8_t  *in, unsigned n, uint8_t  *px,  uint8_t  start);
uint16_t bitdi16(   uint16_t *in, unsigned n, uint16_t *px,  uint16_t start);
uint32_t bitdi32(   uint32_t *in, unsigned n, uint32_t *px,  uint32_t start);
uint64_t bitdi64(   uint64_t *in, unsigned n, uint64_t *px,  uint64_t start);
//-- transform sorted integer array to delta array: out[i] = in[i] - in[i-1] - mindelta
uint8_t  bitdienc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta);
uint16_t bitdienc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta);
uint32_t bitdienc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta);
uint64_t bitdienc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta);
//-- in-place reverse delta
void     bitdidec8( uint8_t  *in, unsigned n,                uint8_t  start, uint8_t  mindelta);
void     bitdidec16(uint16_t *in, unsigned n,                uint16_t start, uint16_t mindelta);
void     bitdidec32(uint32_t *in, unsigned n,                uint32_t start, uint32_t mindelta);
void     bitdidec64(uint64_t *in, unsigned n,                uint64_t start, uint64_t mindelta);

//------------- FOR : array bit length: ---------------------------------------------------------------------
//------ ORed array, for max. bit length of the non decreasing integer array.  out[i] = in[i] - start
uint8_t  bitf8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitf16(uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitf32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitf64(uint64_t *in, unsigned n, uint64_t *px, uint64_t start);

//------ ORed array, for max. bit length of the non strictly decreasing integer array out[i] = in[i] - 1 - start
uint8_t  bitf18( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitf116(uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitf132(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitf164(uint64_t *in, unsigned n, uint64_t *px, uint64_t start);

//------ ORed array, for max. bit length for usorted array
uint8_t  bitfm8( uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  *pmin);  // unsorted
uint16_t bitfm16(uint16_t *in, unsigned n, uint16_t *px, uint16_t *pmin);
uint32_t bitfm32(uint32_t *in, unsigned n, uint32_t *px, uint32_t *pmin);
uint64_t bitfm64(uint64_t *in, unsigned n, uint64_t *px, uint64_t *pmin);

//------------- Zigzag encoding for unsorted integer lists: out[i] = in[i] - in[i-1] ------------------------
//-- ORed array, to get maximum zigzag bit length integer array
uint8_t  bitz8(    uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitz16(   uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitz32(   uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitz64(   uint64_t *in, unsigned n, uint64_t *px, uint64_t start);
uint32_t bitz256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
//-- Zigzag transform
uint8_t  bitzenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta);
uint16_t bitzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta);
uint32_t bitzenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta);
uint64_t bitzenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta);
//-- in-place zigzag reverse transform
void bitzdec8(     uint8_t  *in, unsigned n,                uint8_t  start);
void bitzdec16(    uint16_t *in, unsigned n,                uint16_t start);
void bitzdec32(    uint32_t *in, unsigned n,                uint32_t start);
void bitzdec64(    uint64_t *in, unsigned n,                uint64_t start);

//------------- Zigzag of zigzag/delta : unsorted/sorted integer array ----------------------------------------------------
//-- get delta maximum bit length of the non strictly decreasing integer array. out[i] = in[i] - in[i-1] - 1
uint8_t  bitzz8(    uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitzz16(   uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitzz32(   uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitzz64(   uint64_t *in, unsigned n, uint64_t *px, uint64_t start);

uint8_t  bitzzenc8( uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start, uint8_t  mindelta);
uint16_t bitzzenc16(uint16_t *in, unsigned n, uint16_t *out, uint16_t start, uint16_t mindelta);
uint32_t bitzzenc32(uint32_t *in, unsigned n, uint32_t *out, uint32_t start, uint32_t mindelta);
uint64_t bitzzenc64(uint64_t *in, unsigned n, uint64_t *out, uint64_t start, uint64_t mindelta);

//-- in-place reverse zigzag of delta (encoded w/ bitdiencNN and parameter mindelta = 1)
void bitzzdec8(     uint8_t  *in,  unsigned n, uint8_t  start); // non strictly decreasing (out[i] = in[i] - in[i-1] - 1)
void bitzzdec16(    uint16_t *in,  unsigned n, uint16_t start);
void bitzzdec32(    uint32_t *in,  unsigned n, uint32_t start);
void bitzzdec64(    uint64_t *in,  unsigned n, uint64_t start);

//------------- XOR encoding for unsorted integer lists: out[i] = in[i] - in[i-1] -------------
//-- ORed array, to get maximum zigzag bit length integer array
uint8_t  bitx8(     uint8_t  *in, unsigned n, uint8_t  *px, uint8_t  start);
uint16_t bitx16(    uint16_t *in, unsigned n, uint16_t *px, uint16_t start);
uint32_t bitx32(    uint32_t *in, unsigned n, uint32_t *px, uint32_t start);
uint64_t bitx64(    uint64_t *in, unsigned n, uint64_t *px, uint64_t start);
uint32_t bitx256v32(uint32_t *in, unsigned n, uint32_t *px, uint32_t start);

//-- XOR transform
uint8_t  bitxenc8(  uint8_t  *in, unsigned n, uint8_t  *out, uint8_t  start);
uint16_t bitxenc16( uint16_t *in, unsigned n, uint16_t *out, uint16_t start);
uint32_t bitxenc32( uint32_t *in, unsigned n, uint32_t *out, uint32_t start);
uint64_t bitxenc64( uint64_t *in, unsigned n, uint64_t *out, uint64_t start);

//-- XOR in-place reverse transform
void bitxdec8(      uint8_t  *p,  unsigned n, uint8_t  start);
void bitxdec16(     uint16_t *p,  unsigned n, uint16_t start);
void bitxdec32(     uint32_t *p,  unsigned n, uint32_t start);
void bitxdec64(     uint64_t *p,  unsigned n, uint64_t start);
//--------------------------------------------------------------------
void bitxenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize);
void bitxdec(unsigned char *in, unsigned n,                     unsigned esize);
void bitzenc(unsigned char *in, unsigned n, unsigned char *out, unsigned esize);
void bitzdec(unsigned char *in, unsigned n,                     unsigned esize);

//------- Floating point ----------------------------------------
#define FLOAT2INT(fval, scalse, bias) round(fval * scale + bias)
#define INT2FLOAT(ival, scalse, bias) ((ival - bias) / scale)

#if defined(__clang__) && defined(__is_identifier)
  #if !__is_identifier(_Float16)
    #undef FLT16_BUILTIN
  #endif
#elif defined(FLT16_MAX)
#define FLT16_BUILTIN
#endif
//------- Quantization : b number of bits quantized in out ----------------
  #if defined(FLT16_BUILTIN) 
void fpquant8e16( _Float16 *in, size_t n, uint8_t  *out, unsigned b, _Float16 *pfmin, _Float16 *pfmax);
void fpquant16e16(_Float16 *in, size_t n, uint16_t *out, unsigned b, _Float16 *pfmin, _Float16 *pfmax);
  #endif                                                                                              
																									  
void fpquant8e32(    float *in, size_t n, uint8_t  *out, unsigned b, float    *pfmin,    float *pfmax);
void fpquant16e32(   float *in, size_t n, uint16_t *out, unsigned b, float    *pfmin,    float *pfmax);
void fpquant32e32(   float *in, size_t n, uint32_t *out, unsigned b, float    *pfmin,    float *pfmax);
																									  
void fpquant8e64(   double *in, size_t n, uint8_t  *out, unsigned b, double   *pfmin,   double *pfmax);
void fpquant16e64(  double *in, size_t n, uint16_t *out, unsigned b, double   *pfmin,   double *pfmax);
void fpquant32e64(  double *in, size_t n, uint32_t *out, unsigned b, double   *pfmin,   double *pfmax);
void fpquant64e64(  double *in, size_t n, uint64_t *out, unsigned b, double   *pfmin,   double *pfmax);
																									  
  #if defined(FLT16_BUILTIN)                                                                          
void fpquant8d16( uint8_t  *in, size_t n, _Float16 *out, unsigned b, _Float16   fmin, _Float16   fmax);
void fpquant16d16(uint16_t *in, size_t n, _Float16 *out, unsigned b, _Float16   fmin, _Float16   fmax);
  #endif                                                                                              
																									  
void fpquant8d32( uint8_t  *in, size_t n, float    *out, unsigned b, float      fmin,    float   fmax);
void fpquant16d32(uint16_t *in, size_t n, float    *out, unsigned b, float      fmin,    float   fmax);
void fpquant32d32(uint32_t *in, size_t n, float    *out, unsigned b, float      fmin,    float   fmax);
																									  
void fpquant8d64( uint8_t  *in, size_t n, double   *out, unsigned b, double     fmin,   double   fmax);
void fpquant16d64(uint16_t *in, size_t n, double   *out, unsigned b, double     fmin,   double   fmax);
void fpquant32d64(uint32_t *in, size_t n, double   *out, unsigned b, double     fmin,   double   fmax);
void fpquant64d64(uint64_t *in, size_t n, double   *out, unsigned b, double     fmin,   double   fmax);


//------- Lossy floating point transform: pad the trailing mantissa bits with zeros according to the error e (ex. e=0.00001)
// must include float.h to use _Float16 (see icapp.c)
 #ifdef FLT16_BUILTIN
_Float16 _fprazor16(_Float16 d, float e, int lg2e);  
void fprazor16(_Float16 *in, unsigned n, _Float16  *out, float  e);
  #endif

float  _fprazor32(float  d, float  e, int lg2e);
double _fprazor64(double d, double e, int lg2e);
void   fprazor32( float  *in, unsigned n, float  *out, float  e);
void   fprazor64(double  *in, unsigned n, double *out, double e);

#ifdef __cplusplus
}
#endif

