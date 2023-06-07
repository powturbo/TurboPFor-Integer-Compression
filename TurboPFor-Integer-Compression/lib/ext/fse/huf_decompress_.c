#define FSE_STATIC_LINKING_ONLY
#include "../zstd/lib/common/fse.h"        /* to compress headers */
#include "../zstd/lib/common/huf.h"
#include "../zstd/lib/decompress/huf_decompress.c"
//#define HUF_FORCE_DECOMPRESS_X2
//#define HUF_FORCE_DECOMPRESS_X1

size_t HUF_decompress4X1 (void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);   /**< single-symbol decoder */
#ifndef HUF_FORCE_DECOMPRESS_X1
size_t HUF_decompress4X2 (void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);   /**< double-symbols decoder */
#endif

size_t HUF_decompress4X_DCtx (HUF_DTable* dctx, void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);   /**< decodes RLE and uncompressed */
size_t HUF_decompress4X_hufOnly(HUF_DTable* dctx, void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize); /**< considers RLE and uncompressed as errors */
//size_t HUF_decompress4X_hufOnly_wksp(HUF_DTable* dctx, void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize, void* workSpace, size_t wkspSize); /**< considers RLE and uncompressed as errors */
size_t HUF_decompress4X1_DCtx(HUF_DTable* dctx, void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);   /**< single-symbol decoder */
//size_t HUF_decompress4X1_DCtx_wksp(HUF_DTable* dctx, void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize, void* workSpace, size_t wkspSize);   /**< single-symbol decoder */
#ifndef HUF_FORCE_DECOMPRESS_X1
size_t HUF_decompress4X2_DCtx(HUF_DTable* dctx, void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);   /**< double-symbols decoder */
//size_t HUF_decompress4X2_DCtx_wksp(HUF_DTable* dctx, void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize, void* workSpace, size_t wkspSize);   /**< double-symbols decoder */
#endif

#ifndef HUF_FORCE_DECOMPRESS_X2
size_t HUF_readDTableX1(HUF_DTable* DTable, const void* src, size_t srcSize)
{
    U32 workSpace[HUF_DECOMPRESS_WORKSPACE_SIZE_U32];
    return HUF_readDTableX1_wksp(DTable, src, srcSize,
                                 workSpace, sizeof(workSpace), HUF_flags_bmi2);
}

size_t HUF_decompress1X1_DCtx(HUF_DTable* DCtx, void* dst, size_t dstSize,
                              const void* cSrc, size_t cSrcSize)
{
   U32 workSpace[HUF_DECOMPRESS_WORKSPACE_SIZE_U32];
    return HUF_decompress1X1_DCtx_wksp(DCtx, dst, dstSize, cSrc, cSrcSize,
                                       workSpace, sizeof(workSpace), HUF_flags_bmi2);
}

size_t HUF_decompress1X1 (void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
    HUF_CREATE_STATIC_DTABLEX1(DTable, HUF_TABLELOG_MAX);
    return HUF_decompress1X1_DCtx (DTable, dst, dstSize, cSrc, cSrcSize);
}
#endif

#ifndef HUF_FORCE_DECOMPRESS_X1
size_t HUF_readDTableX2(HUF_DTable* DTable, const void* src, size_t srcSize)
{
  U32 workSpace[HUF_DECOMPRESS_WORKSPACE_SIZE_U32];
  return HUF_readDTableX2_wksp(DTable, src, srcSize,
                               workSpace, sizeof(workSpace), HUF_flags_bmi2);
}

size_t HUF_decompress1X2_DCtx(HUF_DTable* DCtx, void* dst, size_t dstSize,
                              const void* cSrc, size_t cSrcSize)
{
    U32 workSpace[HUF_DECOMPRESS_WORKSPACE_SIZE_U32];
    return HUF_decompress1X2_DCtx_wksp(DCtx, dst, dstSize, cSrc, cSrcSize,
                                       workSpace, sizeof(workSpace), HUF_flags_bmi2);
}

size_t HUF_decompress1X2 (void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
    HUF_CREATE_STATIC_DTABLEX2(DTable, HUF_TABLELOG_MAX);
    return HUF_decompress1X2_DCtx(DTable, dst, dstSize, cSrc, cSrcSize);
}
#endif

#ifndef HUF_FORCE_DECOMPRESS_X2
size_t HUF_decompress4X1_DCtx (HUF_DTable* dctx, void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
   U32 workSpace[HUF_DECOMPRESS_WORKSPACE_SIZE_U32];
    return HUF_decompress4X1_DCtx_wksp(dctx, dst, dstSize, cSrc, cSrcSize,
                                       workSpace, sizeof(workSpace), HUF_flags_bmi2);
}
size_t HUF_decompress4X1 (void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
    HUF_CREATE_STATIC_DTABLEX1(DTable, HUF_TABLELOG_MAX);
    return HUF_decompress4X1_DCtx(DTable, dst, dstSize, cSrc, cSrcSize);
}
#endif

#ifndef HUF_FORCE_DECOMPRESS_X1
size_t HUF_decompress4X2_DCtx(HUF_DTable* dctx, void* dst, size_t dstSize,
                              const void* cSrc, size_t cSrcSize)
{
    U32 workSpace[HUF_DECOMPRESS_WORKSPACE_SIZE_U32];
    return HUF_decompress4X2_DCtx_wksp(dctx, dst, dstSize, cSrc, cSrcSize,
                                       workSpace, sizeof(workSpace), HUF_flags_bmi2);
}

size_t HUF_decompress4X2 (void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
    HUF_CREATE_STATIC_DTABLEX2(DTable, HUF_TABLELOG_MAX);
    return HUF_decompress4X2_DCtx(DTable, dst, dstSize, cSrc, cSrcSize);
}
#endif

typedef size_t (*decompressionAlgo)(void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize);

size_t HUF_decompress (void* dst, size_t dstSize, const void* cSrc, size_t cSrcSize)
{
#if !defined(HUF_FORCE_DECOMPRESS_X1) && !defined(HUF_FORCE_DECOMPRESS_X2)
    static const decompressionAlgo decompress[2] = { HUF_decompress4X1, HUF_decompress4X2 };
#endif

    /* validation checks */
    if (dstSize == 0) return ERROR(dstSize_tooSmall);
    if (cSrcSize > dstSize) return ERROR(corruption_detected);   /* invalid */
    if (cSrcSize == dstSize) { ZSTD_memcpy(dst, cSrc, dstSize); return dstSize; }   /* not compressed */
    if (cSrcSize == 1) { ZSTD_memset(dst, *(const BYTE*)cSrc, dstSize); return dstSize; }   /* RLE */

    {   U32 const algoNb = HUF_selectDecoder(dstSize, cSrcSize);
#if defined(HUF_FORCE_DECOMPRESS_X1)
        (void)algoNb;
        assert(algoNb == 0);
        return HUF_decompress4X1(dst, dstSize, cSrc, cSrcSize);
#elif defined(HUF_FORCE_DECOMPRESS_X2)
        (void)algoNb;
        assert(algoNb == 1);
        return HUF_decompress4X2(dst, dstSize, cSrc, cSrcSize);
#else
        return decompress[algoNb](dst, dstSize, cSrc, cSrcSize);
#endif
    }
}