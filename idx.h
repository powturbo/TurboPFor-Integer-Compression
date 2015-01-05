#include <stdint.h>

#define BLK_DIDNUM (128+1) //  // Block size 128 + 1 (1 stored in skips)


// Compression method. Set only one METHOD!      			
							//							compressed size for 62 GB clueweb09.sorted
   					   		// Defaut is bitpack/bitunpack      18 GB
#define USE_SIMDPACK        // SIMD Bitpacking         			18 GB  
//#define USE_TURBOPFOR     // for compact version     			12 GB  
//#define USE_TURBOPACKD

//-------------------------- Mapping term id <-> posting offset in file ----------------------------------
typedef struct { uint8_t offseth; uint32_t offsetl; } __attribute__ ((packed)) tmap_t;   // 40 bits offsets -> 1 Terabyte

#define TIDMAPSET(__t, __ofs) { (__t)->offseth = (__ofs)>>32; (__t)->offsetl = (__ofs) & 0xffffffff; }
#define TIDMAPGET(__t) ((__off64_t)(__t)->offseth << 32 | (__t)->offsetl)
#define TIDMAP(__fdm, __tid) ({ char *_bp = __fdm; tmap_t *_t = (tmap_t *)&_bp[(__tid)*sizeof(tmap_t)]; TIDMAPGET(_t); })
//--------------------------------------------------------------------------------------------------------

