//--  eliasfano.h - "Integer Compression" Elias Fano c/c++ header ---------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
// compress/decompress integer array with n values to the buffer out. Return value = end of output/input buffer
unsigned char *efanoenc32(     unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efanoenc64(     uint64_t      *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t start);

unsigned char *efanodec32(     unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);
unsigned char *efanodec64(     unsigned char *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start);

unsigned char *efano1enc32(    unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efano1enc64(    uint64_t      *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t start);

unsigned char *efano1dec32(    unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);
unsigned char *efano1dec64(    unsigned char *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t start);

unsigned char *efanoenc128v32( unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efanodec128v32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

unsigned char *efano1enc128v32(unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efano1dec128v32(unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

unsigned char *efanoenc256v32( unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efanodec256v32( unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

unsigned char *efano1enc256v32(unsigned      *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned start);
unsigned char *efano1dec256v32(unsigned char *__restrict in, unsigned n, unsigned       *__restrict out, unsigned start);

#ifdef __cplusplus
}
#endif

