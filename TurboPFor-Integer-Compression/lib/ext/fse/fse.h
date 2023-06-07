/*-****************************************
*  FSE simple functions
******************************************/
/*! FSE_compress() :
    Compress content of buffer 'src', of size 'srcSize', into destination buffer 'dst'.
    'dst' buffer must be already allocated. Compression runs faster is dstCapacity >= FSE_compressBound(srcSize).
    @return : size of compressed data (<= dstCapacity).
    Special values : if return == 0, srcData is not compressible => Nothing is stored within dst !!!
                     if return == 1, srcData is a single byte symbol * srcSize times. Use RLE compression instead.
                     if FSE_isError(return), compression failed (more details using FSE_getErrorName())
*/
FSE_PUBLIC_API size_t FSE_compress(void* dst, size_t dstCapacity,
                             const void* src, size_t srcSize);

/*! FSE_decompress():
    Decompress FSE data from buffer 'cSrc', of size 'cSrcSize',
    into already allocated destination buffer 'dst', of size 'dstCapacity'.
    @return : size of regenerated data (<= maxDstSize),
              or an error code, which can be tested using FSE_isError() .
    ** Important ** : FSE_decompress() does not decompress non-compressible nor RLE data !!!
    Why ? : making this distinction requires a header.
    Header management is intentionally delegated to the user layer, which can better manage special cases.
*/
FSE_PUBLIC_API size_t FSE_decompress(void* dst,  size_t dstCapacity,
                               const void* cSrc, size_t cSrcSize);

/*-*****************************************
*  FSE advanced functions
******************************************/
/*! FSE_compress2() :
    Same as FSE_compress(), but allows the selection of 'maxSymbolValue' and 'tableLog'
    Both parameters can be defined as '0' to mean : use default value
    @return : size of compressed data
    Special values : if return == 0, srcData is not compressible => Nothing is stored within cSrc !!!
                     if return == 1, srcData is a single byte symbol * srcSize times. Use RLE compression.
                     if FSE_isError(return), it's an error code.
*/
FSE_PUBLIC_API size_t FSE_compress2 (void* dst, size_t dstSize, const void* src, size_t srcSize, unsigned maxSymbolValue, unsigned tableLog);

FSE_PUBLIC_API FSE_CTable* FSE_createCTable (unsigned maxSymbolValue, unsigned tableLog);
FSE_PUBLIC_API void        FSE_freeCTable (FSE_CTable* ct);

FSE_PUBLIC_API FSE_DTable* FSE_createDTable(unsigned tableLog);
FSE_PUBLIC_API void        FSE_freeDTable(FSE_DTable* dt);

/*! FSE_buildDTable():
    Builds 'dt', which must be already allocated, using FSE_createDTable().
    return : 0, or an errorCode, which can be tested using FSE_isError() */
FSE_PUBLIC_API size_t FSE_buildDTable (FSE_DTable* dt, const short* normalizedCounter, unsigned maxSymbolValue, unsigned tableLog);

/*! FSE_decompress_usingDTable():
    Decompress compressed source `cSrc` of size `cSrcSize` using `dt`
    into `dst` which must be already allocated.
    @return : size of regenerated data (necessarily <= `dstCapacity`),
              or an errorCode, which can be tested using FSE_isError() */
FSE_PUBLIC_API size_t FSE_decompress_usingDTable(void* dst, size_t dstCapacity, const void* cSrc, size_t cSrcSize, const FSE_DTable* dt);

/* FSE_compress_wksp() :
 * Same as FSE_compress2(), but using an externally allocated scratch buffer (`workSpace`).
 * FSE_COMPRESS_WKSP_SIZE_U32() provides the minimum size required for `workSpace` as a table of FSE_CTable.
 */
#define FSE_COMPRESS_WKSP_SIZE_U32(maxTableLog, maxSymbolValue)   ( FSE_CTABLE_SIZE_U32(maxTableLog, maxSymbolValue) + ((maxTableLog > 12) ? (1 << (maxTableLog - 2)) : 1024) )
size_t FSE_compress_wksp (void* dst, size_t dstSize, const void* src, size_t srcSize, unsigned maxSymbolValue, unsigned tableLog, void* workSpace, size_t wkspSize);

size_t FSE_buildCTable_raw (FSE_CTable* ct, unsigned nbBits);
/**< build a fake FSE_CTable, designed for a flat distribution, where each symbol uses nbBits */

size_t FSE_buildDTable_raw (FSE_DTable* dt, unsigned nbBits);
/**< build a fake FSE_DTable, designed to read a flat distribution where each symbol uses nbBits */

size_t FSE_buildDTable_rle (FSE_DTable* dt, unsigned char symbolValue);
/**< build a fake FSE_DTable, designed to always generate the same symbolValue */

size_t FSE_decompress_wksp(void* dst, size_t dstCapacity, const void* cSrc, size_t cSrcSize, unsigned maxLog, void* workSpace, size_t wkspSize);
/**< same as FSE_decompress(), using an externally allocated `workSpace` produced with `FSE_DECOMPRESS_WKSP_SIZE_U32(maxLog, maxSymbolValue)` */

#define ZSTD_malloc(s) malloc(s)
#define ZSTD_calloc(n,s) calloc((n), (s))
#define ZSTD_free(p) free((p))
