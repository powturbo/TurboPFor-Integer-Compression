//#include "defines.h"
//#include <rw.h>
#include <stdio.h>
#include "defines.h"
//#include "callZlib.h"
#include <stdint.h>
//#include <time.h>
//#include <sys/time.h>
//#include "conf.h"

#ifndef _BG_H
#define _BG_H

#ifdef __cplusplus
extern "C" {
#endif

# define M_LN10      2.30258509299404568401799145468436421   /* loge(10)       */
# define M_LN2       0.693147180559945309417232121458176568  /* loge(2)        */
#define int_CEWI 0
#define NCO_PPC_BIT_XPL_NBR_MIN 2

typedef union lint16
{
	unsigned short usvalue;
	short svalue;
	unsigned char byte[2];
} lint16;

typedef union lint32
{
	int ivalue;
	unsigned int uivalue;
	unsigned char byte[4];
} lint32;

typedef union lint64
{
	long lvalue;
	unsigned long ulvalue;
	unsigned char byte[8];
} lint64;

typedef union ldouble
{
    double value;
    unsigned long lvalue;
    unsigned char byte[8];
} ldouble;

typedef union lfloat
{
    float value;
    unsigned int ivalue;
    unsigned char byte[4];
} lfloat;

typedef struct bg_params
{
	int dataType;

	int sol_ID;// GB
	int zlibMode; //* four options: Z_NO_COMPRESSION, or Z_BEST_SPEED, Z_BEST_COMPRESSION, Z_DEFAULT_COMPRESSION
	int bgMode;  //BITGROOM, BITSHAVE or BITSET
	int errorControlMode;
	int NSD;
	int DSD;
	
	float fmin, fmax;
	double dmin, dmax;

} bg_params;


typedef struct bg_exedata
{
	unsigned int BG_SIZE_TYPE; //the length (# bytes) of the size_t in the system at runtime //4 or 8: sizeof(size_t) 
} bg_exedata;


extern int versionNumber[4];

//-------------------key global variables--------------
extern int dataEndianType; //*endian type of the data read from disk
extern int sysEndianType; //*sysEndianType is actually set automatically.

extern bg_params *confparams_cpr;
extern bg_params *confparams_dec;
extern bg_exedata *exe_params;


//for libpressio
extern int bgMode_libpressio;
extern int errorControlMode_libpressio;
extern int nsd_libpressio;
extern int dsd_libpressio;


unsigned char *BG_compress(int dataType, void *data, size_t *outSize, size_t nbEle);

unsigned char* BG_compress_args(int dataType, void *data, size_t *outSize, int bgMode, int errorControlMode, int nsd, int dsd, size_t nbEle, unsigned char *data_);

//void *BG_decompress(int dataType, unsigned char *bytes, size_t byteLength, size_t nbEle);

#ifdef __cplusplus
}
#endif

#endif /* ----- #ifndef _BG_H  ----- */
