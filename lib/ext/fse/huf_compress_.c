#define FSE_STATIC_LINKING_ONLY
#include "../zstd/lib/common/fse.h"        /* to compress headers */
#include "../zstd/lib/common/huf.h"
#include "../zstd/lib/compress/huf_compress.c"


/*! HUF_writeCTable() :
    `CTable` : Huffman tree to save, using huf representation.
    @return : size of saved CTable */
size_t HUF_writeCTable (void* dst, size_t maxDstSize,
                        const HUF_CElt* CTable, unsigned maxSymbolValue, unsigned huffLog)
{
    HUF_WriteCTableWksp wksp;
    return HUF_writeCTable_wksp(dst, maxDstSize, CTable, maxSymbolValue, huffLog, &wksp, sizeof(wksp));
}

typedef nodeElt huffNodeTable[2 * (HUF_SYMBOLVALUE_MAX + 1)];
#if 0
size_t HUF_compress1X_usingCTable(void* dst, size_t dstSize, const void* src, size_t srcSize, const HUF_CElt* CTable)
{
    return HUF_compress1X_usingCTable_bmi2(dst, dstSize, src, srcSize, CTable, /* bmi2 */ 0);
}
size_t HUF_compress4X_usingCTable(void* dst, size_t dstSize, const void* src, size_t srcSize, const HUF_CElt* CTable)
{
    return HUF_compress4X_usingCTable_bmi2(dst, dstSize, src, srcSize, CTable, /* bmi2 */ 0);
}
#endif

#if 1
size_t HUF_compress1X_wksp (void* dst, size_t dstSize,
                      const void* src, size_t srcSize,
                      unsigned maxSymbolValue, unsigned huffLog,
                      void* workSpace, size_t wkspSize)
{
    return HUF_compress_internal(dst, dstSize, src, srcSize,
                                 maxSymbolValue, huffLog, HUF_singleStream,
                                 workSpace, wkspSize,
                                 NULL, NULL, 0);
}

/* HUF_compress4X_repeat():
 * compress input using 4 streams.
 * provide workspace to generate compression tables */
size_t HUF_compress4X_wksp (void* dst, size_t dstSize,
                      const void* src, size_t srcSize,
                      unsigned maxSymbolValue, unsigned huffLog,
                      void* workSpace, size_t wkspSize)
{
    DEBUGLOG(5, "HUF_compress4X_wksp (srcSize = %zu)", srcSize);
    return HUF_compress_internal(dst, dstSize, src, srcSize,
                                 maxSymbolValue, huffLog, HUF_fourStreams,
                                 workSpace, wkspSize,
                                 NULL, NULL, 0);
}
#endif

size_t HUF_buildCTable (HUF_CElt* tree, const unsigned* count, unsigned maxSymbolValue, unsigned maxNbBits)
{
    HUF_buildCTable_wksp_tables workspace;
    return HUF_buildCTable_wksp(tree, count, maxSymbolValue, maxNbBits, &workspace, sizeof(workspace));
}

size_t HUF_compress1X (void* dst, size_t dstSize,
                 const void* src, size_t srcSize,
                 unsigned maxSymbolValue, unsigned huffLog)
{
    U64 workSpace[HUF_WORKSPACE_SIZE_U64];
    return HUF_compress1X_wksp(dst, dstSize, src, srcSize, maxSymbolValue, huffLog, workSpace, sizeof(workSpace));
}

size_t HUF_compress2 (void* dst, size_t dstSize,
                const void* src, size_t srcSize,
                unsigned maxSymbolValue, unsigned huffLog)
{
    U64 workSpace[HUF_WORKSPACE_SIZE_U64];
    return HUF_compress4X_wksp(dst, dstSize, src, srcSize, maxSymbolValue, huffLog, workSpace, sizeof(workSpace));
}

size_t HUF_compress (void* dst, size_t maxDstSize, const void* src, size_t srcSize)
{
    return HUF_compress2(dst, maxDstSize, src, srcSize, 255, HUF_TABLELOG_DEFAULT);
}
