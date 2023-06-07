// -- "Integer Compression" variable simple "SimpleV" -----------------------------------------------------------
//  this belongs to the integer compression known as "simple family", like simple-9,simple-16
//  or simple-8b. SimpleV is compressing integers in groups into variable word size 32, 40 and 64 bits + RLE (run length encoding)
//  SimpleV is faster than simple-16 and compress better than simple-16 or simple-8b.

#ifdef __cplusplus
extern "C" {
#endif

size_t vsbound8( size_t n);
size_t vsbound16(size_t n);
size_t vsbound32(size_t n);
size_t vsbound64(size_t n);

// vsencNN: compress array with n unsigned (NN bits in[n]) values to the buffer out. Return value = end of compressed output buffer out
unsigned char *vsenc8( unsigned char  *__restrict in, size_t n, unsigned char  *__restrict out);
unsigned char *vsenc16(unsigned short *__restrict in, size_t n, unsigned char  *__restrict out);
unsigned char *vsenc32(unsigned       *__restrict in, size_t n, unsigned char  *__restrict out);
unsigned char *vsenc64(uint64_t       *__restrict in, size_t n, unsigned char  *__restrict out);

// vsdecNN: decompress buffer into an array of n unsigned values. Return value = end of compressed input buffer in
unsigned char *vsdec8( unsigned char  *__restrict in, size_t n, unsigned char  *__restrict out);
unsigned char *vsdec16(unsigned char  *__restrict in, size_t n, unsigned short *__restrict out);
unsigned char *vsdec32(unsigned char  *__restrict in, size_t n, unsigned       *__restrict out);
unsigned char *vsdec64(unsigned char  *__restrict in, size_t n, uint64_t       *__restrict out);

#ifdef __cplusplus
}
#endif

