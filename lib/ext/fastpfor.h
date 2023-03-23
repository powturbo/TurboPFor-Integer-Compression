#if defined(_MSC_VER) && _MSC_VER < 1600
#include "vs/stdint.h"
#else 
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
unsigned FastPFore32(    const uint32_t      *in, unsigned n, unsigned char *out, unsigned outsize);
unsigned FastPFord32(    const unsigned char *in, unsigned n, uint32_t      *out);

unsigned FastPFore128v32(const uint32_t      *in, unsigned n, unsigned char *out, unsigned outsize);
unsigned FastPFord128v32(const unsigned char *in, unsigned n, uint32_t *out);

unsigned OptPFore128v32( const uint32_t      *in, unsigned n, unsigned char *out, unsigned outsize);
unsigned OptPFord128v32( const unsigned char *in, unsigned n, uint32_t *out);
#ifdef __cplusplus
}
#endif
