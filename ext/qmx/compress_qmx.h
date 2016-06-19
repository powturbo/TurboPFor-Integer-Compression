/*
	COMPRESS_QMX.H
	--------------
*/
#ifndef COMPRESS_QMX_H_
#define COMPRESS_QMX_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

unsigned char *qmx_enc( const uint32_t *in, unsigned n, unsigned char *out);
unsigned char *qmx_dec(const unsigned char *in, unsigned len, uint32_t *out, unsigned n);

#ifdef __cplusplus
}
#endif
#endif

