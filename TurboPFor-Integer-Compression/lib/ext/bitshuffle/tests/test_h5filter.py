from __future__ import absolute_import, division, print_function, unicode_literals

import unittest
import os
import glob

import numpy as np
import h5py
import pytest
from h5py import h5z

from bitshuffle import h5, __zstd__


os.environ["HDF5_PLUGIN_PATH"] = ""


class TestFilter(unittest.TestCase):
    def test_filter(self):
        shape = (32 * 1024 + 783,)
        chunks = (4 * 1024 + 23,)
        dtype = np.int64
        data = np.arange(shape[0])
        fname = "tmp_test_filters.h5"
        f = h5py.File(fname, "w")
        h5.create_dataset(
            f,
            b"range",
            shape,
            dtype,
            chunks,
            filter_pipeline=(32008, 32000),
            filter_flags=(h5z.FLAG_MANDATORY, h5z.FLAG_MANDATORY),
            filter_opts=None,
        )
        f["range"][:] = data

        f.close()

        f = h5py.File(fname, "r")
        d = f["range"][:]
        self.assertTrue(np.all(d == data))
        f.close()

    def test_with_block_size(self):
        shape = (128 * 1024 + 783,)
        chunks = (4 * 1024 + 23,)
        dtype = np.int64
        data = np.arange(shape[0])
        fname = "tmp_test_filters.h5"
        f = h5py.File(fname, "w")
        h5.create_dataset(
            f,
            b"range",
            shape,
            dtype,
            chunks,
            filter_pipeline=(32008, 32000),
            filter_flags=(h5z.FLAG_MANDATORY, h5z.FLAG_MANDATORY),
            filter_opts=((680,), ()),
        )
        f["range"][:] = data

        f.close()
        # os.system('h5dump -H -p tmp_test_filters.h5')

        f = h5py.File(fname, "r")
        d = f["range"][:]
        self.assertTrue(np.all(d == data))
        f.close()

    def test_with_lz4_compression(self):
        shape = (128 * 1024 + 783,)
        chunks = (4 * 1024 + 23,)
        dtype = np.int64
        data = np.arange(shape[0])
        fname = "tmp_test_filters.h5"
        f = h5py.File(fname, "w")
        h5.create_dataset(
            f,
            b"range",
            shape,
            dtype,
            chunks,
            filter_pipeline=(32008,),
            filter_flags=(h5z.FLAG_MANDATORY,),
            filter_opts=((0, h5.H5_COMPRESS_LZ4),),
        )
        f["range"][:] = data

        f.close()
        # os.system('h5dump -H -p tmp_test_filters.h5')

        f = h5py.File(fname, "r")
        d = f["range"][:]
        self.assertTrue(np.all(d == data))
        f.close()

    @pytest.mark.skipif(
        __zstd__ is False,
        reason="Bitshuffle has not been built with ZSTD support.",
    )
    def test_with_zstd_compression(self):
        shape = (128 * 1024 + 783,)
        chunks = (4 * 1024 + 23,)
        compression_lvl = 10
        dtype = np.int64
        data = np.arange(shape[0])
        fname = "tmp_test_filters.h5"
        f = h5py.File(fname, "w")
        h5.create_dataset(
            f,
            b"range",
            shape,
            dtype,
            chunks,
            filter_pipeline=(32008,),
            filter_flags=(h5z.FLAG_MANDATORY,),
            filter_opts=((0, h5.H5_COMPRESS_ZSTD, compression_lvl),),
        )
        f["range"][:] = data

        f.close()
        # os.system('h5dump -H -p tmp_test_filters.h5')

        f = h5py.File(fname, "r")
        d = f["range"][:]
        self.assertTrue(np.all(d == data))
        f.close()

    def tearDown(self):
        files = glob.glob("tmp_test_*")
        for f in files:
            os.remove(f)


if __name__ == "__main__":
    unittest.main()
