"""
Wrappers for public and private bitshuffle routines

"""

from __future__ import absolute_import, division, print_function, unicode_literals

import numpy as np

cimport numpy as np
cimport cython


np.import_array()


# Repeat each calculation this many times. For timing.
cdef int REPEATC = 1
# cdef int REPEATC = 32

REPEAT = REPEATC

cdef extern from b"bitshuffle.h":
    int bshuf_using_NEON()
    int bshuf_using_SSE2()
    int bshuf_using_AVX2()
    int bshuf_using_AVX512()
    int bshuf_bitshuffle(void *A, void *B, int size, int elem_size,
                         int block_size) nogil
    int bshuf_bitunshuffle(void *A, void *B, int size, int elem_size,
                           int block_size) nogil
    int bshuf_compress_lz4_bound(int size, int elem_size, int block_size)
    int bshuf_compress_lz4(void *A, void *B, int size, int elem_size,
                           int block_size) nogil
    int bshuf_decompress_lz4(void *A, void *B, int size, int elem_size,
                             int block_size) nogil
    IF ZSTD_SUPPORT:
        int bshuf_compress_zstd_bound(int size, int elem_size, int block_size)
        int bshuf_compress_zstd(void *A, void *B, int size, int elem_size,
                                int block_size, const int comp_lvl) nogil
        int bshuf_decompress_zstd(void *A, void *B, int size, int elem_size,
                                  int block_size) nogil
    int BSHUF_VERSION_MAJOR
    int BSHUF_VERSION_MINOR
    int BSHUF_VERSION_POINT

__version__ = "%d.%d.%d" % (BSHUF_VERSION_MAJOR, BSHUF_VERSION_MINOR,
                            BSHUF_VERSION_POINT)

IF ZSTD_SUPPORT:
    __zstd__ = True
ELSE:
    __zstd__ = False

# Prototypes from bitshuffle.c
cdef extern int bshuf_copy(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_byte_elem_scal(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_byte_elem_SSE(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_byte_elem_NEON(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_byte_scal(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_byte_SSE(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_byte_NEON(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_byte_AVX(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_byte_AVX512(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bitrow_eight(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_elem_AVX512(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_elem_AVX(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_elem_SSE(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_elem_NEON(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_elem_scal(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_byte_bitrow_SSE(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_byte_bitrow_NEON(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_byte_bitrow_AVX(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_byte_bitrow_scal(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_shuffle_bit_eightelem_scal(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_shuffle_bit_eightelem_SSE(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_shuffle_bit_eightelem_NEON(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_shuffle_bit_eightelem_AVX(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_shuffle_bit_eightelem_AVX512(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_untrans_bit_elem_SSE(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_untrans_bit_elem_NEON(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_untrans_bit_elem_AVX(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_untrans_bit_elem_AVX512(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_untrans_bit_elem_scal(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_trans_bit_elem(void *A, void *B, int size, int elem_size)
cdef extern int bshuf_untrans_bit_elem(void *A, void *B, int size, int elem_size)


ctypedef int (*Cfptr) (void *A, void *B, int size, int elem_size)


def using_NEON():
    """Whether compiled using Arm NEON instructions."""
    if bshuf_using_NEON():
        return True
    else:
        return False


def using_SSE2():
    """Whether compiled using SSE2 instructions."""
    if bshuf_using_SSE2():
        return True
    else:
        return False


def using_AVX2():
    """Whether compiled using AVX2 instructions."""
    if bshuf_using_AVX2():
        return True
    else:
        return False


def using_AVX512():
    """Whether compiled using AVX512 instructions."""
    if bshuf_using_AVX512():
        return True
    else:
        return False


def _setup_arr(arr):
    shape = tuple(arr.shape)
    if not arr.flags['C_CONTIGUOUS']:
        msg = "Input array must be C-contiguous."
        raise ValueError(msg)
    size = arr.size
    dtype = arr.dtype
    itemsize = dtype.itemsize
    out = np.empty(shape, dtype=dtype)
    return out, size, itemsize


@cython.boundscheck(False)
@cython.wraparound(False)
cdef _wrap_C_fun(Cfptr fun, np.ndarray arr):
    """Wrap a C function with standard call signature."""

    cdef int ii, size, itemsize, count=0
    cdef np.ndarray out
    out, size, itemsize = _setup_arr(arr)

    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] arr_flat
    arr_flat = arr.view(np.uint8).ravel()
    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] out_flat
    out_flat = out.view(np.uint8).ravel()
    cdef void* arr_ptr = <void*> &arr_flat[0]
    cdef void* out_ptr = <void*> &out_flat[0]

    for ii in range(REPEATC):
        count = fun(arr_ptr, out_ptr, size, itemsize)
    if count < 0:
        msg = "Failed. Error code %d."
        excp = RuntimeError(msg % count, count)
        raise excp
    return out


def copy(np.ndarray arr not None):
    """Copies the data.

    For testing and profiling purposes.

    """
    return _wrap_C_fun(&bshuf_copy, arr)


def trans_byte_elem_scal(np.ndarray arr not None):
    """Transpose bytes within words but not bits.

    """
    return _wrap_C_fun(&bshuf_trans_byte_elem_scal, arr)


def trans_byte_elem_SSE(np.ndarray arr not None):
    """Transpose bytes within array elements.

    """
    return _wrap_C_fun(&bshuf_trans_byte_elem_SSE, arr)


def trans_byte_elem_NEON(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_byte_elem_NEON, arr)


def trans_bit_byte_scal(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_byte_scal, arr)


def trans_bit_byte_SSE(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_byte_SSE, arr)


def trans_bit_byte_NEON(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_byte_NEON, arr)


def trans_bit_byte_AVX(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_byte_AVX, arr)


def trans_bit_byte_AVX512(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_byte_AVX512, arr)


def trans_bitrow_eight(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bitrow_eight, arr)


def trans_bit_elem_AVX512(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_elem_AVX512, arr)


def trans_bit_elem_AVX(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_elem_AVX, arr)


def trans_bit_elem_scal(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_elem_scal, arr)


def trans_bit_elem_SSE(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_elem_SSE, arr)


def trans_bit_elem_NEON(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_elem_NEON, arr)


def trans_byte_bitrow_SSE(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_byte_bitrow_SSE, arr)


def trans_byte_bitrow_NEON(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_byte_bitrow_NEON, arr)


def trans_byte_bitrow_AVX(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_byte_bitrow_AVX, arr)


def trans_byte_bitrow_scal(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_byte_bitrow_scal, arr)


def shuffle_bit_eightelem_scal(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_shuffle_bit_eightelem_scal, arr)


def shuffle_bit_eightelem_SSE(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_shuffle_bit_eightelem_SSE, arr)


def shuffle_bit_eightelem_NEON(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_shuffle_bit_eightelem_NEON, arr)


def shuffle_bit_eightelem_AVX(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_shuffle_bit_eightelem_AVX, arr)


def shuffle_bit_eightelem_AVX512(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_shuffle_bit_eightelem_AVX512, arr)


def untrans_bit_elem_SSE(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_untrans_bit_elem_SSE, arr)


def untrans_bit_elem_NEON(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_untrans_bit_elem_NEON, arr)


def untrans_bit_elem_AVX(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_untrans_bit_elem_AVX, arr)


def untrans_bit_elem_AVX512(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_untrans_bit_elem_AVX512, arr)


def untrans_bit_elem_scal(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_untrans_bit_elem_scal, arr)


def trans_bit_elem(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_trans_bit_elem, arr)


def untrans_bit_elem(np.ndarray arr not None):
    return _wrap_C_fun(&bshuf_untrans_bit_elem, arr)


@cython.boundscheck(False)
@cython.wraparound(False)
def bitshuffle(np.ndarray arr not None, int block_size=0):
    """Bitshuffle an array.

    Output array is the same shape and data type as input array but underlying
    buffer has been bitshuffled.

    Parameters
    ----------
    arr : numpy array
        Data to ne processed.
    block_size : positive integer
        Block size in number of elements. By default, block size is chosen
        automatically.

    Returns
    -------
    out : numpy array
        Array with the same shape as input but underlying data has been
        bitshuffled.

    """

    cdef int ii, size, itemsize, count=0
    cdef np.ndarray out
    out, size, itemsize = _setup_arr(arr)

    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] arr_flat
    arr_flat = arr.view(np.uint8).ravel()
    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] out_flat
    out_flat = out.view(np.uint8).ravel()
    cdef void* arr_ptr = <void*> &arr_flat[0]
    cdef void* out_ptr = <void*> &out_flat[0]

    with nogil:
        for ii in range(REPEATC):
            count = bshuf_bitshuffle(arr_ptr, out_ptr, size, itemsize, block_size)
    if count < 0:
        msg = "Failed. Error code %d."
        excp = RuntimeError(msg % count, count)
        raise excp
    return out


@cython.boundscheck(False)
@cython.wraparound(False)
def bitunshuffle(np.ndarray arr not None, int block_size=0):
    """Bitshuffle an array.

    Output array is the same shape and data type as input array but underlying
    buffer has been un-bitshuffled.

    Parameters
    ----------
    arr : numpy array
        Data to ne processed.
    block_size : positive integer
        Block size in number of elements. Must match value used for shuffling.

    Returns
    -------
    out : numpy array
        Array with the same shape as input but underlying data has been
        un-bitshuffled.

    """

    cdef int ii, size, itemsize, count=0
    cdef np.ndarray out
    out, size, itemsize = _setup_arr(arr)

    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] arr_flat
    arr_flat = arr.view(np.uint8).ravel()
    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] out_flat
    out_flat = out.view(np.uint8).ravel()
    cdef void* arr_ptr = <void*> &arr_flat[0]
    cdef void* out_ptr = <void*> &out_flat[0]

    with nogil:
        for ii in range(REPEATC):
            count = bshuf_bitunshuffle(arr_ptr, out_ptr, size, itemsize, block_size)
    if count < 0:
        msg = "Failed. Error code %d."
        excp = RuntimeError(msg % count, count)
        raise excp
    return out


@cython.boundscheck(False)
@cython.wraparound(False)
def compress_lz4(np.ndarray arr not None, int block_size=0):
    """Bitshuffle then compress an array using LZ4.

    Parameters
    ----------
    arr : numpy array
        Data to ne processed.
    block_size : positive integer
        Block size in number of elements. By default, block size is chosen
        automatically.

    Returns
    -------
    out : array with np.uint8 data type
        Buffer holding compressed data.

    """

    cdef int ii, size, itemsize, count=0
    shape = (arr.shape[i] for i in range(arr.ndim))
    if not arr.flags['C_CONTIGUOUS']:
        msg = "Input array must be C-contiguous."
        raise ValueError(msg)
    size = arr.size
    dtype = arr.dtype
    itemsize = dtype.itemsize

    max_out_size = bshuf_compress_lz4_bound(size, itemsize, block_size)

    cdef np.ndarray out
    out = np.empty(max_out_size, dtype=np.uint8)

    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] arr_flat
    arr_flat = arr.view(np.uint8).ravel()
    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] out_flat
    out_flat = out.view(np.uint8).ravel()
    cdef void* arr_ptr = <void*> &arr_flat[0]
    cdef void* out_ptr = <void*> &out_flat[0]
    with nogil:
        for ii in range(REPEATC):
            count = bshuf_compress_lz4(arr_ptr, out_ptr, size, itemsize, block_size)
    if count < 0:
        msg = "Failed. Error code %d."
        excp = RuntimeError(msg % count, count)
        raise excp
    return out[:count]


@cython.boundscheck(False)
@cython.wraparound(False)
def decompress_lz4(np.ndarray arr not None, shape, dtype, int block_size=0):
    """Decompress a buffer using LZ4 then bitunshuffle it yielding an array.

    Parameters
    ----------
    arr : numpy array
        Input data to be decompressed.
    shape : tuple of integers
        Shape of the output (decompressed array). Must match the shape of the
        original data array before compression.
    dtype : numpy dtype
        Datatype of the output array. Must match the data type of the original
        data array before compression.
    block_size : positive integer
        Block size in number of elements. Must match value used for
        compression.

    Returns
    -------
    out : numpy array with shape *shape* and data type *dtype*
        Decompressed data.

    """

    cdef int ii, size, itemsize, count=0
    if not arr.flags['C_CONTIGUOUS']:
        msg = "Input array must be C-contiguous."
        raise ValueError(msg)
    size = np.prod(shape)
    itemsize = dtype.itemsize

    cdef np.ndarray out
    out = np.empty(tuple(shape), dtype=dtype)

    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] arr_flat
    arr_flat = arr.view(np.uint8).ravel()
    cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] out_flat
    out_flat = out.view(np.uint8).ravel()
    cdef void* arr_ptr = <void*> &arr_flat[0]
    cdef void* out_ptr = <void*> &out_flat[0]
    with nogil:
        for ii in range(REPEATC):
            count = bshuf_decompress_lz4(arr_ptr, out_ptr, size, itemsize,
                                         block_size)
    if count < 0:
        msg = "Failed. Error code %d."
        excp = RuntimeError(msg % count, count)
        raise excp
    if count != arr.size:
        msg = "Decompressed different number of bytes than input buffer size."
        msg += "Input buffer %d, decompressed %d." % (arr.size, count)
        raise RuntimeError(msg, count)
    return out


IF ZSTD_SUPPORT:
    @cython.boundscheck(False)
    @cython.wraparound(False)
    def compress_zstd(np.ndarray arr not None, int block_size=0, int comp_lvl=1):
        """Bitshuffle then compress an array using ZSTD.
    
        Parameters
        ----------
        arr : numpy array
            Data to be processed.
        block_size : positive integer
            Block size in number of elements. By default, block size is chosen
            automatically.
        comp_lvl : positive integer
            Compression level applied by ZSTD
    
        Returns
        -------
        out : array with np.uint8 data type
            Buffer holding compressed data.
    
        """
    
        cdef int ii, size, itemsize, count=0
        shape = (arr.shape[i] for i in range(arr.ndim))
        if not arr.flags['C_CONTIGUOUS']:
            msg = "Input array must be C-contiguous."
            raise ValueError(msg)
        size = arr.size
        dtype = arr.dtype
        itemsize = dtype.itemsize
    
        max_out_size = bshuf_compress_zstd_bound(size, itemsize, block_size)
    
        cdef np.ndarray out
        out = np.empty(max_out_size, dtype=np.uint8)
    
        cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] arr_flat
        arr_flat = arr.view(np.uint8).ravel()
        cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] out_flat
        out_flat = out.view(np.uint8).ravel()
        cdef void* arr_ptr = <void*> &arr_flat[0]
        cdef void* out_ptr = <void*> &out_flat[0]
        with nogil:
            for ii in range(REPEATC):
                count = bshuf_compress_zstd(arr_ptr, out_ptr, size, itemsize, block_size, comp_lvl)
        if count < 0:
            msg = "Failed. Error code %d."
            excp = RuntimeError(msg % count, count)
            raise excp
        return out[:count]
    
    @cython.boundscheck(False)
    @cython.wraparound(False)
    def decompress_zstd(np.ndarray arr not None, shape, dtype, int block_size=0):
        """Decompress a buffer using ZSTD then bitunshuffle it yielding an array.
    
        Parameters
        ----------
        arr : numpy array
            Input data to be decompressed.
        shape : tuple of integers
            Shape of the output (decompressed array). Must match the shape of the
            original data array before compression.
        dtype : numpy dtype
            Datatype of the output array. Must match the data type of the original
            data array before compression.
        block_size : positive integer
            Block size in number of elements. Must match value used for
            compression.
    
        Returns
        -------
        out : numpy array with shape *shape* and data type *dtype*
            Decompressed data.
    
        """
    
        cdef int ii, size, itemsize, count=0
        if not arr.flags['C_CONTIGUOUS']:
            msg = "Input array must be C-contiguous."
            raise ValueError(msg)
        size = np.prod(shape)
        itemsize = dtype.itemsize
    
        cdef np.ndarray out
        out = np.empty(tuple(shape), dtype=dtype)
    
        cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] arr_flat
        arr_flat = arr.view(np.uint8).ravel()
        cdef np.ndarray[dtype=np.uint8_t, ndim=1, mode="c"] out_flat
        out_flat = out.view(np.uint8).ravel()
        cdef void* arr_ptr = <void*> &arr_flat[0]
        cdef void* out_ptr = <void*> &out_flat[0]
        with nogil:
            for ii in range(REPEATC):
                count = bshuf_decompress_zstd(arr_ptr, out_ptr, size, itemsize,
                                              block_size)
        if count < 0:
            msg = "Failed. Error code %d."
            excp = RuntimeError(msg % count, count)
            raise excp
        if count != arr.size:
            msg = "Decompressed different number of bytes than input buffer size."
            msg += "Input buffer %d, decompressed %d." % (arr.size, count)
            raise RuntimeError(msg, count)
        return out
