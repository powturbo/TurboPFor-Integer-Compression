from __future__ import absolute_import, division, print_function, unicode_literals
import unittest
import os
import glob

import numpy as np
import h5py
import pytest
from subprocess import Popen, PIPE, STDOUT

import bitshuffle


plugin_dir = os.path.join(os.path.dirname(bitshuffle.__file__), "plugin")
os.environ["HDF5_PLUGIN_PATH"] = plugin_dir


H5VERSION = h5py.h5.get_libversion()
if H5VERSION[0] < 1 or (
    H5VERSION[0] == 1
    and (H5VERSION[1] < 8 or (H5VERSION[1] == 8 and H5VERSION[2] < 11))
):
    H51811P = False
else:
    H51811P = True


class TestFilterPlugins(unittest.TestCase):
    @pytest.mark.skipif(
        "CIBUILDWHEEL" in os.environ,
        reason="Can't build dynamic HDF5 plugin into bitshuffle wheel.",
    )
    def test_plugins(self):
        if not H51811P:
            return
        shape = (32 * 1024,)
        chunks = (4 * 1024,)
        dtype = np.int64
        data = np.arange(shape[0])
        fname = "tmp_test_filters.h5"
        f = h5py.File(fname, "w")
        dset = f.create_dataset(
            "range", shape=shape, dtype=dtype, chunks=chunks, compression=32008
        )
        dset[:] = data
        f.close()

        # Make sure the filters are working outside of h5py by calling h5dump
        h5dump = Popen(["h5dump", fname], stdout=PIPE, stderr=STDOUT)
        stdout, nothing = h5dump.communicate()
        err = h5dump.returncode
        self.assertEqual(err, 0)

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
