// ----- TurboRLE - "Most efficient and fastest Run Length Encoding" -----------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
// RLE with specified escape char
unsigned _srlec8( const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint8_t e);
unsigned _srled8( const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen, uint8_t e);

unsigned _srlec16(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint16_t e);
unsigned _srled16(const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen, uint16_t e);

unsigned _srlec32(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint32_t e);
unsigned _srled32(const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen, uint32_t e);

unsigned _srlec64(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint64_t e);
unsigned _srled64(const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen, uint64_t e);

// functions w/ overflow handling
unsigned  srlec8( const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint8_t e);
unsigned  srled8( const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint8_t e);

unsigned  srlec16(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint16_t e);
unsigned  srled16(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint16_t e);

unsigned  srlec32(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint32_t e);
unsigned  srled32(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint32_t e);

unsigned  srlec64(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, uint64_t e);
unsigned  srled64(const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen, uint64_t e);

// RLE w. automatic escape char determination
unsigned  srlec(  const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out);
unsigned _srled(  const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen);
unsigned  srled(  const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen);

// Turbo RLE
unsigned  trlec(  const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out);
unsigned _trled(  const unsigned char *__restrict in,                 unsigned char *__restrict out, unsigned outlen);
unsigned  trled(  const unsigned char *__restrict in, unsigned inlen, unsigned char *__restrict out, unsigned outlen);
#ifdef __cplusplus
}
#endif

