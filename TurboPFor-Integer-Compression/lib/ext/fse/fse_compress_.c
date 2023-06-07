#define FSE_STATIC_LINKING_ONLY
#include "../zstd/lib/common/fse.h"  
#include "fse.h"  

#define FSE_TABLELOG_ABSOLUTE_MAX 15
#define HIST_WKSP_SIZE_U32 1024
 
FSE_CTable* FSE_createCTable (unsigned maxSymbolValue, unsigned tableLog)
{
    size_t size;
    if (tableLog > FSE_TABLELOG_ABSOLUTE_MAX) tableLog = FSE_TABLELOG_ABSOLUTE_MAX;
    size = FSE_CTABLE_SIZE_U32 (tableLog, maxSymbolValue) * sizeof(U32);
    return (FSE_CTable*)ZSTD_malloc(size);
}

void FSE_freeCTable (FSE_CTable* ct) { ZSTD_free(ct); }


/* fake FSE_CTable, for raw (uncompressed) input */
size_t FSE_buildCTable_raw (FSE_CTable* ct, unsigned nbBits)
{
    const unsigned tableSize = 1 << nbBits;
    const unsigned tableMask = tableSize - 1;
    const unsigned maxSymbolValue = tableMask;
    void* const ptr = ct;
    U16* const tableU16 = ( (U16*) ptr) + 2;
    void* const FSCT = ((U32*)ptr) + 1 /* header */ + (tableSize>>1);   /* assumption : tableLog >= 1 */
    FSE_symbolCompressionTransform* const symbolTT = (FSE_symbolCompressionTransform*) (FSCT);
    unsigned s;

    /* Sanity checks */
    if (nbBits < 1) return ERROR(GENERIC);             /* min size */

    /* header */
    tableU16[-2] = (U16) nbBits;
    tableU16[-1] = (U16) maxSymbolValue;

    /* Build table */
    for (s=0; s<tableSize; s++)
        tableU16[s] = (U16)(tableSize + s);

    /* Build Symbol Transformation Table */
    {   const U32 deltaNbBits = (nbBits << 16) - (1 << nbBits);
        for (s=0; s<=maxSymbolValue; s++) {
            symbolTT[s].deltaNbBits = deltaNbBits;
            symbolTT[s].deltaFindState = s-1;
    }   }

    return 0;
}

#ifndef ZSTD_NO_UNUSED_FUNCTIONS
/* FSE_compress_wksp() :
 * Same as FSE_compress2(), but using an externally allocated scratch buffer (`workSpace`).
 * `wkspSize` size must be `(1<<tableLog)`.
 */
size_t FSE_compress_wksp (void* dst, size_t dstSize, const void* src, size_t srcSize, unsigned maxSymbolValue, unsigned tableLog, void* workSpace, size_t wkspSize)
{
    BYTE* const ostart = (BYTE*) dst;
    BYTE* op = ostart;
    BYTE* const oend = ostart + dstSize;

    unsigned count[FSE_MAX_SYMBOL_VALUE+1];
    S16   norm[FSE_MAX_SYMBOL_VALUE+1];
    FSE_CTable* CTable = (FSE_CTable*)workSpace;
    size_t const CTableSize = FSE_CTABLE_SIZE_U32(tableLog, maxSymbolValue);
    void* scratchBuffer = (void*)(CTable + CTableSize);
    size_t const scratchBufferSize = wkspSize - (CTableSize * sizeof(FSE_CTable));

    /* init conditions */
    if (wkspSize < FSE_COMPRESS_WKSP_SIZE_U32(tableLog, maxSymbolValue)) return ERROR(tableLog_tooLarge);
    if (srcSize <= 1) return 0;  /* Not compressible */
    if (!maxSymbolValue) maxSymbolValue = FSE_MAX_SYMBOL_VALUE;
    if (!tableLog) tableLog = FSE_DEFAULT_TABLELOG;

    /* Scan input and build symbol stats */
    {   CHECK_V_F(maxCount, HIST_count_wksp(count, &maxSymbolValue, src, srcSize, scratchBuffer, scratchBufferSize) );
        if (maxCount == srcSize) return 1;   /* only a single symbol in src : rle */
        if (maxCount == 1) return 0;         /* each symbol present maximum once => not compressible */
        if (maxCount < (srcSize >> 7)) return 0;   /* Heuristic : not compressible enough */
    }

    tableLog = FSE_optimalTableLog(tableLog, srcSize, maxSymbolValue);
    CHECK_F( FSE_normalizeCount(norm, tableLog, count, srcSize, maxSymbolValue, /* useLowProbCount */ srcSize >= 2048) );

    /* Write table description header */
    {   CHECK_V_F(nc_err, FSE_writeNCount(op, oend-op, norm, maxSymbolValue, tableLog) );
        op += nc_err;
    }

    /* Compress */
    CHECK_F( FSE_buildCTable_wksp(CTable, norm, maxSymbolValue, tableLog, scratchBuffer, scratchBufferSize) );
    {   CHECK_V_F(cSize, FSE_compress_usingCTable(op, oend - op, src, srcSize, CTable) );
        if (cSize == 0) return 0;   /* not enough space for compressed data */
        op += cSize;
    }

    /* check compressibility */
    if ( (size_t)(op-ostart) >= srcSize-1 ) return 0;

    return op-ostart;
}

typedef struct {
    FSE_CTable CTable_max[FSE_CTABLE_SIZE_U32(FSE_MAX_TABLELOG, FSE_MAX_SYMBOL_VALUE)];
    union {
      U32 hist_wksp[HIST_WKSP_SIZE_U32];
      BYTE scratchBuffer[1 << FSE_MAX_TABLELOG];
    } workspace;
} fseWkspMax_t;

size_t FSE_compress2 (void* dst, size_t dstCapacity, const void* src, size_t srcSize, unsigned maxSymbolValue, unsigned tableLog)
{
    fseWkspMax_t scratchBuffer;
    DEBUG_STATIC_ASSERT(sizeof(scratchBuffer) >= FSE_COMPRESS_WKSP_SIZE_U32(FSE_MAX_TABLELOG, FSE_MAX_SYMBOL_VALUE));   /* compilation failures here means scratchBuffer is not large enough */
    if (tableLog > FSE_MAX_TABLELOG) return ERROR(tableLog_tooLarge);
    return FSE_compress_wksp(dst, dstCapacity, src, srcSize, maxSymbolValue, tableLog, &scratchBuffer, sizeof(scratchBuffer));
}

size_t FSE_compress (void* dst, size_t dstCapacity, const void* src, size_t srcSize)
{
    return FSE_compress2(dst, dstCapacity, src, srcSize, FSE_MAX_SYMBOL_VALUE, FSE_DEFAULT_TABLELOG);
}
#endif

//#endif   /* FSE_COMMONDEFS_ONLY */
