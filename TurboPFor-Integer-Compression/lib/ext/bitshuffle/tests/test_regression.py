"""
Test that data encoded with earlier versions can still be decoded correctly.

"""

from __future__ import absolute_import, division, print_function

import pathlib
import unittest

import numpy as np
import h5py
from bitshuffle import __zstd__

from packaging import version

TEST_DATA_DIR = pathlib.Path(__file__).parent / "data"

OUT_FILE_TEMPLATE = "regression_%s.h5"

VERSIONS = ["0.1.3", "0.4.0"]


class TestAll(unittest.TestCase):
    def test_regression(self):
        for rev in VERSIONS:
            file_name = TEST_DATA_DIR / (OUT_FILE_TEMPLATE % rev)
            f = h5py.File(file_name, "r")
            g_orig = f["original"]
            g_comp = f["compressed"]

            for dset_name in g_comp.keys():
                self.assertTrue(np.all(g_comp[dset_name][:] == g_orig[dset_name][:]))

            # Only run ZSTD comparison on versions >= 0.4.0 and if ZSTD support
            # has been built into bitshuffle
            if version.parse(rev) >= version.parse("0.4.0") and __zstd__:
                g_comp_zstd = f["compressed_zstd"]
                for dset_name in g_comp_zstd.keys():
                    self.assertTrue(
                        np.all(g_comp_zstd[dset_name][:] == g_orig[dset_name][:])
                    )


if __name__ == "__main__":
    unittest.main()
