//-- codec: general purpose compression (lz, entropy coder, bwt,....)---------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
int lzidget(char *scmd);
char *codstr(unsigned cid);
size_t lzenc(unsigned char *in, size_t inlen, unsigned char *out, unsigned outsize, int codid, int codlev, char *codprm);
size_t lzdec(unsigned char *in, size_t inlen, unsigned char *out, unsigned outlen,  int codec, int codlev, char *codprm);
unsigned meshenc(const float *in,                             unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned outsize, unsigned char *tmp, int codid, int codlev, char *codprm);
void meshdec(  const uint8_t *in, unsigned inlen, float *out, unsigned nx, unsigned ny, unsigned nz,                                       unsigned char *tmp, int codid, int codlev, char *codprm);
//------------------- TurboByte + lz ----------------------------
unsigned lzv8enc(    unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lzv8dec(    unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lzv8zenc(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lzv8zdec(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lzv8xenc(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm); // TurboVByte
unsigned v8lzxdec(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
//----------------- Byte transpose  + Lz -------------------------------------------------------------------------------------
unsigned lztpenc(    unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztpdec(    unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztpxenc(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm); // XOR
unsigned lztpxdec(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztpzenc(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm); // Zigzag
unsigned lztpzdec(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
//------- rle ------------------------
unsigned tprleenc(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp);
unsigned tprledec(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp);
unsigned tprlexenc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp); // xor rle
unsigned tprlexdec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp);
unsigned tprlezenc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp); // Zigzag rle
unsigned tprlezdec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp);
//------- rle + lz -------------------
unsigned lztprleenc( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm); // Zigzag rle
unsigned lztprledec( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztprlexenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm); // Zigzag rle
unsigned lztprlexdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztprlezenc(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm); // Zigzag rle
unsigned lztprlezdec(unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp4enc(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztpd4ec(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp4xenc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm); //XOR
unsigned lztp4xdec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp4zenc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp4zdec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztpd2enc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm); 
unsigned lztpd2dec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm); //printf("#2D[%u,%u]", x, y);
unsigned lztpd2xenc( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm); 
unsigned lztpd2xdec( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm);
unsigned lztpd2zenc( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm); 
unsigned lztpd2zdec( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, int codid, int codlev, char *codprm);
unsigned lztpd3enc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd3dec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd3xenc( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd3xdec( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd3zenc( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd3zdec( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd4enc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd4dec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd4xenc( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd4xdec( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd4zenc( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztpd4zdec( unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, unsigned w, unsigned x, unsigned y, unsigned z, int codid, int codlev, char *codprm);
unsigned lztp1enc(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp1dec(   unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp1xenc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp1xdec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp1zenc(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outsize, unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
unsigned lztp1zdec(  unsigned char *in, unsigned inlen, unsigned char *out, unsigned outlen,  unsigned esize, unsigned char *tmp, int codid, int codlev, char *codprm);
void tpsizeset(unsigned _tpsize);

//-- TurboVLC : Novel Variable Length Coding for large integers with exponent + mantissa ------------------------
size_t vlccomp32(  unsigned char *_in, size_t _inlen, unsigned char *out,  size_t outsize, unsigned char *tmp, int codid, int codlev, char *codprm); 
size_t vlcdecomp32(unsigned char *in,  size_t inlen,  unsigned char *_out, size_t _outlen, unsigned char *tmp, int codid, int codlev, char *codprm);

#ifdef __cplusplus
}
#endif

