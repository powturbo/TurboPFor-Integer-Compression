//--  transpose.h - Byte/Nibble transpose for further compressing with lz77 or other compressors -------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
// Syntax
// in    : Input buffer
// n     : Total number of bytes in input buffer
// out   : output buffer
// esize : element size in bytes (ex. 2, 4, 8,... )

//---------- High level functions with dynamic cpu detection and JIT scalar/sse/avx2 switching
void tpenc(        unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // tranpose
void tpdec(        unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // reverse transpose

void tpzenc(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // zigzag integrated
void tpzdec(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

void tpxenc(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // xor integrated
void tpxdec(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

//2D transpose
void tp2denc(      unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out, unsigned esize); 
void tp2ddec(      unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out, unsigned esize);

void tp2denc2(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out); 
void tp2ddec2(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out);

void tp2denc4(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out); 
void tp2ddec4(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out);

void tp2denc8(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out); 
void tp2ddec8(     unsigned char *in,             unsigned nx, unsigned ny,             unsigned char *out);

//3D transpose
void tp3denc(      unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize); 
void tp3ddec(      unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize);

void tp3denc2(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp3ddec2(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

void tp3denc4(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp3ddec4(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

void tp3denc8(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp3ddec8(     unsigned char *in,             unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

//4D transpose
void tp4denc(      unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize); 
void tp4ddec(      unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out, unsigned esize);

void tp4denc2(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp4ddec2(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

void tp4denc4(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp4ddec4(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

void tp4denc8(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);
void tp4ddec8(     unsigned char *in, unsigned nw, unsigned nx, unsigned ny, unsigned nz, unsigned char *out);

// Nibble transpose SIMD (SSE2,AVX2, ARM Neon)
void tp4enc(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize);
void tp4dec(       unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

void tp4zenc(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // zigzag delta integrated
void tp4zdec(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

void tp4xenc(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize); // xor integrated
void tp4xdec(      unsigned char *in, unsigned n, unsigned char *out, unsigned esize);

//---------- Low level functions --------------------------------------------------------------------------------------------
void tpenc2(       unsigned char *in, unsigned n, unsigned char *out);  // scalar
void tpdec2(       unsigned char *in, unsigned n, unsigned char *out);

void tpenc3(       unsigned char *in, unsigned n, unsigned char *out);
void tpdec3(       unsigned char *in, unsigned n, unsigned char *out);

void tpenc4(       unsigned char *in, unsigned n, unsigned char *out);
void tpdec4(       unsigned char *in, unsigned n, unsigned char *out);

void tpenc8(       unsigned char *in, unsigned n, unsigned char *out);
void tpdec8(       unsigned char *in, unsigned n, unsigned char *out);

void tpenc16(      unsigned char *in, unsigned n, unsigned char *out);
void tpdec16(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc2(      unsigned char *in, unsigned n, unsigned char *out);  // scalar zigzag
void tpzdec2(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc3(      unsigned char *in, unsigned n, unsigned char *out);
void tpzdec3(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc4(      unsigned char *in, unsigned n, unsigned char *out);
void tpzdec4(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc8(      unsigned char *in, unsigned n, unsigned char *out);
void tpzdec8(      unsigned char *in, unsigned n, unsigned char *out);

void tpzenc16(     unsigned char *in, unsigned n, unsigned char *out);
void tpzdec16(     unsigned char *in, unsigned n, unsigned char *out);

void tpxenc2(      unsigned char *in, unsigned n, unsigned char *out);  // scalar xor
void tpxdec2(      unsigned char *in, unsigned n, unsigned char *out);

void tpxenc3(      unsigned char *in, unsigned n, unsigned char *out);
void tpxdec3(      unsigned char *in, unsigned n, unsigned char *out);

void tpxenc4(      unsigned char *in, unsigned n, unsigned char *out);
void tpxdec4(      unsigned char *in, unsigned n, unsigned char *out);

void tpxenc8(      unsigned char *in, unsigned n, unsigned char *out);
void tpxdec8(      unsigned char *in, unsigned n, unsigned char *out);

void tpxenc16(     unsigned char *in, unsigned n, unsigned char *out);
void tpxdec16(     unsigned char *in, unsigned n, unsigned char *out);

//-- byte transpose - sse ---------
void tpenc128v2(   unsigned char *in, unsigned n, unsigned char *out); // 16 bits
void tpdec128v2(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc128v2(  unsigned char *in, unsigned n, unsigned char *out);
void tpzdec128v2(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc128v2(  unsigned char *in, unsigned n, unsigned char *out);
void tpxdec128v2(  unsigned char *in, unsigned n, unsigned char *out);
//--
void tpenc128v4(   unsigned char *in, unsigned n, unsigned char *out); // 32 bits
void tpdec128v4(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc128v4(  unsigned char *in, unsigned n, unsigned char *out);
void tpzdec128v4(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc128v4(  unsigned char *in, unsigned n, unsigned char *out);
void tpxdec128v4(  unsigned char *in, unsigned n, unsigned char *out);

void tpenc128v8(   unsigned char *in, unsigned n, unsigned char *out); // 64 bits
void tpdec128v8(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc128v8(  unsigned char *in, unsigned n, unsigned char *out);
void tpzdec128v8(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc128v8(  unsigned char *in, unsigned n, unsigned char *out);
void tpxdec128v8(  unsigned char *in, unsigned n, unsigned char *out);

//-- nibble transpose
void tp4enc128v2(  unsigned char *in, unsigned n, unsigned char *out); // 16 bits
void tp4dec128v2(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc128v2( unsigned char *in, unsigned n, unsigned char *out);
void tp4zdec128v2( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc128v2( unsigned char *in, unsigned n, unsigned char *out);
void tp4xdec128v2( unsigned char *in, unsigned n, unsigned char *out);

void tp4enc128v4(  unsigned char *in, unsigned n, unsigned char *out); // 32 bits
void tp4dec128v4(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc128v4( unsigned char *in, unsigned n, unsigned char *out);
void tp4zdec128v4( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc128v4( unsigned char *in, unsigned n, unsigned char *out);
void tp4xdec128v4( unsigned char *in, unsigned n, unsigned char *out);

void tp4enc128v8(  unsigned char *in, unsigned n, unsigned char *out); // 64 bits
void tp4dec128v8(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc128v8( unsigned char *in, unsigned n, unsigned char *out);
void tp4zdec128v8( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc128v8( unsigned char *in, unsigned n, unsigned char *out);
void tp4xdec128v8( unsigned char *in, unsigned n, unsigned char *out);

//-- avx2 
//-- byte transpose
void tpenc256v2(   unsigned char *in, unsigned n, unsigned char *out); //-- 16 bits
void tpdec256v2(   unsigned char *in, unsigned n, unsigned char *out);

void tpenc256v4(   unsigned char *in, unsigned n, unsigned char *out); //-- 32 bits
void tpdec256v4(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc256v4(  unsigned char *in, unsigned n, unsigned char *out); // zigzag
void tpzdec256v4(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc256v4(  unsigned char *in, unsigned n, unsigned char *out); // xor
void tpxdec256v4(  unsigned char *in, unsigned n, unsigned char *out);

void tpenc256v8(   unsigned char *in, unsigned n, unsigned char *out); //-- 64 bits
void tpdec256v8(   unsigned char *in, unsigned n, unsigned char *out);

void tpzenc256v8(  unsigned char *in, unsigned n, unsigned char *out); // zigzag
void tpzdec256v8(  unsigned char *in, unsigned n, unsigned char *out);

void tpxenc256v8(  unsigned char *in, unsigned n, unsigned char *out); // xor
void tpxdec256v8(  unsigned char *in, unsigned n, unsigned char *out);

//-- Nibble transpose
void tp4enc256v2(  unsigned char *in, unsigned n, unsigned char *out); //-- 16 bits
void tp4dec256v2(  unsigned char *in, unsigned n, unsigned char *out);

void tp4enc256v4(  unsigned char *in, unsigned n, unsigned char *out); //-- 32 bits
void tp4dec256v4(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc256v4( unsigned char *in, unsigned n, unsigned char *out); // zigzag
void tp4zdec256v4( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc256v4( unsigned char *in, unsigned n, unsigned char *out); // xor
void tp4xdec256v4( unsigned char *in, unsigned n, unsigned char *out);

void tp4enc256v8(  unsigned char *in, unsigned n, unsigned char *out); // 64 bits
void tp4dec256v8(  unsigned char *in, unsigned n, unsigned char *out);

void tp4zenc256v8( unsigned char *in, unsigned n, unsigned char *out); // zigzag
void tp4zdec256v8( unsigned char *in, unsigned n, unsigned char *out);

void tp4xenc256v8( unsigned char *in, unsigned n, unsigned char *out); // xor
void tp4xdec256v8( unsigned char *in, unsigned n, unsigned char *out);

//------- CPU instruction set
// cpuiset  = 0: return current simd set,
// cpuiset != 0: set simd set 0:scalar, 20:sse2, 52:avx2
unsigned cpuini(unsigned cpuiset);

// convert simd set to string "sse3", "sse3", "sse4.1" or "avx2"
// Ex.: printf("current cpu set=%s\n", cpustr(cpuini(0)) );
char *cpustr(unsigned cpuisa);

unsigned cpuisa(void);
#ifdef __cplusplus
}
#endif

