"""
HDF5 support for Bitshuffle.

To read a dataset that uses the Bitshuffle filter using h5py, simply import
this module (unless you have installed the Bitshuffle dynamically loaded
filter, in which case importing this module is unnecessary).

To create a new dataset that includes the Bitshuffle filter, use one of the
convenience functions provided.


Constants
=========

    H5FILTER : The Bitshuffle HDF5 filter integer identifier.
    H5_COMPRESS_LZ4 : Filter option flag for LZ4 compression.
    H5_COMPRESS_ZSTD : Filter option flag for ZSTD compression.

Functions
=========

    create_dataset
    create_bitshuffle_lzf_dataset
    create_bitshuffle_compressed_dataset

Examples
========

    >>> import numpy as np
    >>> import h5py
    >>> import bitshuffle.h5

    >>> shape = (123, 456)
    >>> chunks = (10, 456)
    >>> dtype = np.float64

    >>> f = h5py.File("tmp_test.h5")
    >>> bitshuffle.h5.create_bitshuffle_compressed_dataset(
            f, "some_data", shape, dtype, chunks)
    >>> f["some_data"][:] = 42

"""

from __future__ import absolute_import, division, print_function, unicode_literals

import sys
import numpy
import h5py
from h5py import h5d, h5fd, h5s, h5t, h5p, h5z, defs, filters

cimport cython


cdef extern from b"bshuf_h5filter.h":
    int bshuf_register_h5filter()
    int BSHUF_H5FILTER
    int BSHUF_H5_COMPRESS_LZ4
    int BSHUF_H5_COMPRESS_ZSTD

cdef extern int init_filter(const char* libname)

cdef int LZF_FILTER = 32000

H5FILTER = BSHUF_H5FILTER
H5_COMPRESS_LZ4 = BSHUF_H5_COMPRESS_LZ4
H5_COMPRESS_ZSTD = BSHUF_H5_COMPRESS_ZSTD

# Init HDF5 dynamic loading with HDF5 library used by h5py
if not sys.platform.startswith('win'):
    if sys.version_info[0] >= 3:
        libs = [bytes(h5d.__file__, encoding='utf-8'),
                bytes(h5fd.__file__, encoding='utf-8'),
                bytes(h5s.__file__, encoding='utf-8'),
                bytes(h5t.__file__, encoding='utf-8'),
                bytes(h5p.__file__, encoding='utf-8'),
                bytes(h5z.__file__, encoding='utf-8'),
                bytes(defs.__file__, encoding='utf-8')]
    else:
        libs = [h5d.__file__, h5fd.__file__, h5s.__file__, h5t.__file__,
                h5p.__file__, h5z.__file__, defs.__file__]

    # Ensure all symbols are loaded
    success = -1
    for lib in libs:
        success = init_filter(lib)
        if success == 0:
            break

    if success == -1:
        raise RuntimeError("Failed to load all HDF5 symbols using these libs: {}".format(libs))


def register_h5_filter():
    ret = bshuf_register_h5filter()
    if ret < 0:
        raise RuntimeError("Failed to register bitshuffle HDF5 filter.", ret)


register_h5_filter()


def create_dataset(parent, name, shape, dtype, chunks=None, maxshape=None,
                   fillvalue=None, track_times=None,
                   filter_pipeline=(), filter_flags=None, filter_opts=None):
    """Create a dataset with an arbitrary filter pipeline.

    Return a new low-level dataset identifier.

    Much of this code is copied from h5py, but couldn't reuse much code due to
    unstable API.

    """

    if hasattr(filter_pipeline, "__getitem__"):
        filter_pipeline = list(filter_pipeline)
    else:
        filter_pipeline = [filter_pipeline]
        filter_flags = [filter_flags]
        filter_opts = [filter_opts]
    nfilters = len(filter_pipeline)
    if filter_flags is None:
        filter_flags = [None] * nfilters
    if filter_opts is None:
        filter_opts = [None] * nfilters
    if not len(filter_flags) == nfilters or not len(filter_opts) == nfilters:
        msg = "Supplied incompatible number of filters, flags, and options."
        raise ValueError(msg)

    shape = tuple(shape)

    tmp_shape = maxshape if maxshape is not None else shape
    # Validate chunk shape
    chunks_larger = (numpy.array([ not i>=j
                     for i, j in zip(tmp_shape, chunks) if i is not None])).any()
    if isinstance(chunks, tuple) and chunks_larger:
        errmsg = ("Chunk shape must not be greater than data shape in any "
                  "dimension. {} is not compatible with {}".format(chunks, shape))
        raise ValueError(errmsg)

    if isinstance(dtype, h5py.Datatype):
        # Named types are used as-is
        tid = dtype.id
        dtype = tid.dtype  # Following code needs this
    else:
        # Validate dtype
        dtype = numpy.dtype(dtype)
        tid = h5t.py_create(dtype, logical=1)

    if shape == ():
        if any((chunks, filter_pipeline)):
            raise TypeError("Scalar datasets don't support chunk/filter options")
        if maxshape and maxshape != ():
            raise TypeError("Scalar datasets cannot be extended")
        return h5p.create(h5p.DATASET_CREATE)

    def rq_tuple(tpl, name):
        """Check if chunks/maxshape match dataset rank"""
        if tpl in (None, True):
            return
        try:
            tpl = tuple(tpl)
        except TypeError:
            raise TypeError('"%s" argument must be None or a sequence object' % name)
        if len(tpl) != len(shape):
            raise ValueError('"%s" must have same rank as dataset shape' % name)

    rq_tuple(chunks, 'chunks')
    rq_tuple(maxshape, 'maxshape')

    if (chunks is True) or (chunks is None and filter_pipeline):
        chunks = filters.guess_chunk(shape, maxshape, dtype.itemsize)

    if maxshape is True:
        maxshape = (None,)*len(shape)

    dcpl = h5p.create(h5p.DATASET_CREATE)
    if chunks is not None:
        dcpl.set_chunk(chunks)
        dcpl.set_fill_time(h5d.FILL_TIME_ALLOC)  # prevent resize glitch

    if fillvalue is not None:
        fillvalue = numpy.array(fillvalue)
        dcpl.set_fill_value(fillvalue)

    if track_times in (True, False):
        dcpl.set_obj_track_times(track_times)
    elif track_times is not None:
        raise TypeError("track_times must be either True or False")

    for ii in range(nfilters):
        this_filter = filter_pipeline[ii]
        this_flags = filter_flags[ii]
        this_opts = filter_opts[ii]
        if this_flags is None:
            this_flags = 0
        if this_opts is None:
            this_opts = ()
        dcpl.set_filter(this_filter, this_flags, this_opts)

    if maxshape is not None:
        maxshape = tuple(m if m is not None else h5s.UNLIMITED
                         for m in maxshape)
    sid = h5s.create_simple(shape, maxshape)

    dset_id = h5d.create(parent.id, name, tid, sid, dcpl=dcpl)

    return dset_id


def create_bitshuffle_lzf_dataset(parent, name, shape, dtype, chunks=None,
                                  maxshape=None, fillvalue=None,
                                  track_times=None):
    """Create dataset with a filter pipeline including bitshuffle and LZF"""

    filter_pipeline = [H5FILTER, LZF_FILTER]
    dset_id = create_dataset(parent, name, shape, dtype, chunks=chunks,
                             filter_pipeline=filter_pipeline, maxshape=maxshape,
                             fillvalue=fillvalue, track_times=track_times)
    return dset_id


def create_bitshuffle_compressed_dataset(parent, name, shape, dtype,
                                         chunks=None, maxshape=None,
                                         fillvalue=None, track_times=None):
    """Create dataset with bitshuffle+internal LZ4 compression."""

    filter_pipeline = [H5FILTER, ]
    filter_opts = [(0, H5_COMPRESS_LZ4)]
    dset_id = create_dataset(parent, name, shape, dtype, chunks=chunks,
                             filter_pipeline=filter_pipeline,
                             filter_opts=filter_opts, maxshape=maxshape,
                             fillvalue=fillvalue, track_times=track_times)
    return dset_id


