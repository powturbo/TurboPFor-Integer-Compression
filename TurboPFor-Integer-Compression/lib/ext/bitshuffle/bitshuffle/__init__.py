# flake8: noqa
"""
Filter for improving compression of typed binary data.

Functions
=========

    using_NEON
    using_SSE2
    using_AVX2
    using_AVX512
    bitshuffle
    bitunshuffle
    compress_lz4
    decompress_lz4
    compress_zstd
    decompress_zstd

"""

from __future__ import absolute_import


from bitshuffle.ext import (
    __version__,
    __zstd__,
    bitshuffle,
    bitunshuffle,
    using_NEON,
    using_SSE2,
    using_AVX2,
    using_AVX512,
    compress_lz4,
    decompress_lz4,
)

# Import ZSTD API if enabled
zstd_api = []
if __zstd__:
    from bitshuffle.ext import (
        compress_zstd,
        decompress_zstd,
    )

    zstd_api += ["compress_zstd", "decompress_zstd"]

__all__ = [
    "__version__",
    "bitshuffle",
    "bitunshuffle",
    "using_NEON",
    "using_SSE2",
    "using_AVX2",
    "using_AVX512",
    "compress_lz4",
    "decompress_lz4",
] + zstd_api
