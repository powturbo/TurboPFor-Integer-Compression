from __future__ import absolute_import, division, print_function, unicode_literals

import unittest
import time

import numpy as np
from numpy import random

from bitshuffle import ext, __zstd__


# If we are doing timeings by what factor to increase workload.
# Remember to change `ext.REPEATC`.
TIME = 0
# TIME = 8    # 8kB blocks same as final blocking.
BLOCK = 1024


TEST_DTYPES = [
    np.uint8,
    np.uint16,
    np.int32,
    np.uint64,
    np.float32,
    np.float64,
    np.complex128,
]
TEST_DTYPES += [b"a3", b"a5", b"a6", b"a7", b"a9", b"a11", b"a12", b"a24", b"a48"]


class TestProfile(unittest.TestCase):
    def setUp(self):
        n = 1024  # bytes.
        if TIME:
            n *= TIME
        # Almost random bits, but now quite. All bits exercised (to fully test
        # transpose) but still slightly compressible.
        self.data = random.randint(0, 200, n).astype(np.uint8)
        self.fun = ext.copy
        self.check = None
        self.check_data = None
        self.case = "None"

    def tearDown(self):
        """Performs all tests and timings."""
        if TIME:
            reps = 10
        else:
            reps = 1
        delta_ts = []
        try:
            for ii in range(reps):
                t0 = time.time()
                out = self.fun(self.data)
                delta_ts.append(time.time() - t0)
        except RuntimeError as err:
            if len(err.args) > 1 and (err.args[1] == -11) and not ext.using_SSE2():
                return
            if len(err.args) > 1 and (err.args[1] == -12) and not ext.using_AVX2():
                return
            if len(err.args) > 1 and (err.args[1] == -14) and not ext.using_AVX512():
                return
            else:
                raise
        delta_t = min(delta_ts)
        size_i = self.data.size * self.data.dtype.itemsize
        size_o = out.size * out.dtype.itemsize
        size = max([size_i, size_o])
        speed = ext.REPEAT * size / delta_t / 1024**3  # GB/s
        if TIME:
            print("%-20s: %5.2f s/GB,   %5.2f GB/s" % (self.case, 1.0 / speed, speed))
        if self.check is not None:
            ans = self.check(self.data).view(np.uint8)
            self.assertTrue(np.all(ans == out.view(np.uint8)))
        if self.check_data is not None:
            ans = self.check_data.view(np.uint8)
            self.assertTrue(np.all(ans == out.view(np.uint8)))

    def test_00_copy(self):
        self.case = "copy"
        self.fun = ext.copy
        self.check = lambda x: x

    def test_01a_trans_byte_elem_scal_16(self):
        self.case = "byte T elem scal 16"
        self.data = self.data.view(np.int16)
        self.fun = ext.trans_byte_elem_scal
        self.check = trans_byte_elem

    def test_01b_trans_byte_elem_scal_32(self):
        self.case = "byte T elem scal 32"
        self.data = self.data.view(np.int32)
        self.fun = ext.trans_byte_elem_scal
        self.check = trans_byte_elem

    def test_01c_trans_byte_elem_scal_64(self):
        self.case = "byte T elem scal 64"
        self.data = self.data.view(np.int64)
        self.fun = ext.trans_byte_elem_scal
        self.check = trans_byte_elem

    def test_01d_trans_byte_elem_16(self):
        self.case = "byte T elem SSE 16"
        self.data = self.data.view(np.int16)
        self.fun = ext.trans_byte_elem_SSE
        self.check = trans_byte_elem

    def test_01e_trans_byte_elem_32(self):
        self.case = "byte T elem SSE 32"
        self.data = self.data.view(np.float32)
        self.fun = ext.trans_byte_elem_SSE
        self.check = trans_byte_elem

    def test_01f_trans_byte_elem_64(self):
        self.case = "byte T elem SSE 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_byte_elem_SSE
        self.check = trans_byte_elem

    def test_01g_trans_byte_elem_128(self):
        self.case = "byte T elem SSE 128"
        self.data = self.data.view(np.complex128)
        self.fun = ext.trans_byte_elem_SSE
        self.check = trans_byte_elem

    def test_01h_trans_byte_elem_96(self):
        self.case = "byte T elem SSE 96"
        n = self.data.size // 128 * 96
        dt = np.dtype(
            [(str("a"), np.int32), (str("b"), np.int32), (str("c"), np.int32)]
        )
        self.data = self.data[:n].view(dt)
        self.fun = ext.trans_byte_elem_SSE
        self.check = trans_byte_elem

    def test_01i_trans_byte_elem_80(self):
        self.case = "byte T elem SSE 80"
        n = self.data.size // 128 * 80
        dt = np.dtype(
            [
                (str("a"), np.int16),
                (str("b"), np.int16),
                (str("c"), np.int16),
                (str("d"), np.int16),
                (str("e"), np.int16),
            ]
        )
        self.data = self.data[:n].view(dt)
        self.fun = ext.trans_byte_elem_SSE
        self.check = trans_byte_elem

    def test_03a_trans_bit_byte(self):
        self.case = "bit T byte scal 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_byte_scal
        self.check = trans_bit_byte

    def test_03d_trans_bit_byte_SSE(self):
        self.case = "bit T byte SSE 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_byte_SSE
        self.check = trans_bit_byte

    def test_03f_trans_bit_byte_AVX(self):
        self.case = "bit T byte AVX 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_byte_AVX
        self.check = trans_bit_byte

    def test_03g_trans_bit_byte_AVX_32(self):
        self.case = "bit T byte AVX 32"
        self.data = self.data.view(np.float32)
        self.fun = ext.trans_bit_byte_AVX
        self.check = trans_bit_byte

    def test_03h_trans_bit_byte_AVX512(self):
        self.case = "bit T byte AVX512 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_byte_AVX512
        self.check = trans_bit_byte

    def test_03g_trans_bit_byte_AVX512_32(self):
        self.case = "bit T byte AVX512 32"
        self.data = self.data.view(np.float32)
        self.fun = ext.trans_bit_byte_AVX512
        self.check = trans_bit_byte

    def test_04a_trans_bit_elem_AVX(self):
        self.case = "bit T elem AVX 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_elem_AVX
        self.check = trans_bit_elem

    def test_04b_trans_bit_elem_AVX_128(self):
        self.case = "bit T elem AVX 128"
        self.data = self.data.view(np.complex128)
        self.fun = ext.trans_bit_elem_AVX
        self.check = trans_bit_elem

    def test_04c_trans_bit_elem_AVX_32(self):
        self.case = "bit T elem AVX 32"
        self.data = self.data.view(np.float32)
        self.fun = ext.trans_bit_elem_AVX
        self.check = trans_bit_elem

    def test_04d_trans_bit_elem_AVX_16(self):
        self.case = "bit T elem AVX 16"
        self.data = self.data.view(np.int16)
        self.fun = ext.trans_bit_elem_AVX
        self.check = trans_bit_elem

    def test_04e_trans_bit_elem_64(self):
        self.case = "bit T elem scal 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_elem_scal
        self.check = trans_bit_elem

    def test_04f_trans_bit_elem_SSE_32(self):
        self.case = "bit T elem SSE 32"
        self.data = self.data.view(np.float32)
        self.fun = ext.trans_bit_elem_SSE
        self.check = trans_bit_elem

    def test_04g_trans_bit_elem_SSE_64(self):
        self.case = "bit T elem SSE 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_elem_SSE
        self.check = trans_bit_elem

    def test_04h_trans_bit_elem_AVX512(self):
        self.case = "bit T elem AVX512 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_elem_AVX512
        self.check = trans_bit_elem

    def test_04i_trans_bit_elem_AVX512(self):
        self.case = "bit T elem AVX 128"
        self.data = self.data.view(np.complex128)
        self.fun = ext.trans_bit_elem_AVX512
        self.check = trans_bit_elem

    def test_04j_trans_bit_elem_AVX512_32(self):
        self.case = "bit T elem AVX512 32"
        self.data = self.data.view(np.float32)
        self.fun = ext.trans_bit_elem_AVX512
        self.check = trans_bit_elem

    def test_04k_trans_bit_elem_AVX512_16(self):
        self.case = "bit T elem AVX512 16"
        self.data = self.data.view(np.int16)
        self.fun = ext.trans_bit_elem_AVX512
        self.check = trans_bit_elem

    def test_06a_untrans_bit_elem_16(self):
        self.case = "bit U elem SSE 16"
        pre_trans = self.data.view(np.int16)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_SSE
        self.check_data = pre_trans

    def test_06b_untrans_bit_elem_128(self):
        self.case = "bit U elem SSE 128"
        pre_trans = self.data.view(np.complex128)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_SSE
        self.check_data = pre_trans

    def test_06c_untrans_bit_elem_32(self):
        self.case = "bit U elem SSE 32"
        pre_trans = self.data.view(np.float32)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_SSE
        self.check_data = pre_trans

    def test_06d_untrans_bit_elem_32(self):
        self.case = "bit U elem AVX 32"
        pre_trans = self.data.view(np.float32)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_AVX
        self.check_data = pre_trans

    def test_06e_untrans_bit_elem_64(self):
        self.case = "bit U elem SSE 64"
        pre_trans = self.data.view(np.float64)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_SSE
        self.check_data = pre_trans

    def test_06f_untrans_bit_elem_64(self):
        self.case = "bit U elem AVX 64"
        pre_trans = self.data.view(np.float64)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_AVX
        self.check_data = pre_trans

    def test_06g_untrans_bit_elem_64(self):
        self.case = "bit U elem scal 64"
        pre_trans = self.data.view(np.float64)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_scal
        self.check_data = pre_trans

    def test_06h_untrans_bit_elem_32(self):
        self.case = "bit U elem AVX512 32"
        pre_trans = self.data.view(np.float32)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_AVX512
        self.check_data = pre_trans

    def test_06i_untrans_bit_elem_64(self):
        self.case = "bit U elem AVX512 64"
        pre_trans = self.data.view(np.float64)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_AVX512
        self.check_data = pre_trans

    def test_07a_trans_byte_bitrow_64(self):
        self.case = "byte T row scal 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_byte_bitrow_scal

    def test_07b_trans_byte_bitrow_SSE_64(self):
        self.case = "byte T row SSE 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_byte_bitrow_SSE
        self.check = ext.trans_byte_bitrow_scal

    def test_07c_trans_byte_bitrow_AVX_64(self):
        self.case = "byte T row AVX 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_byte_bitrow_AVX
        self.check = ext.trans_byte_bitrow_scal

    def test_08a_shuffle_bit_eight_scal_64(self):
        self.case = "bit S eight scal 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.shuffle_bit_eightelem_scal

    def test_08b_shuffle_bit_eight_SSE_64(self):
        self.case = "bit S eight SSE 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.shuffle_bit_eightelem_SSE
        self.check = ext.shuffle_bit_eightelem_scal

    def test_08c_shuffle_bit_eight_AVX_32(self):
        self.case = "bit S eight AVX 32"
        self.data = self.data.view(np.float32)
        self.fun = ext.shuffle_bit_eightelem_AVX
        self.check = ext.shuffle_bit_eightelem_scal

    def test_08d_shuffle_bit_eight_AVX_64(self):
        self.case = "bit S eight AVX 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.shuffle_bit_eightelem_AVX
        self.check = ext.shuffle_bit_eightelem_scal

    def test_08e_shuffle_bit_eight_AVX_16(self):
        self.case = "bit S eight AVX 16"
        self.data = self.data.view(np.int16)
        self.fun = ext.shuffle_bit_eightelem_AVX
        self.check = ext.shuffle_bit_eightelem_scal

    def test_08f_shuffle_bit_eight_AVX_128(self):
        self.case = "bit S eight AVX 128"
        self.data = self.data.view(np.complex128)
        self.fun = ext.shuffle_bit_eightelem_AVX
        self.check = ext.shuffle_bit_eightelem_scal

    def test_08g_shuffle_bit_eight_AVX512_32(self):
        self.case = "bit S eight AVX 32"
        self.data = self.data.view(np.float32)
        self.fun = ext.shuffle_bit_eightelem_AVX512
        self.check = ext.shuffle_bit_eightelem_scal

    def test_08h_shuffle_bit_eight_AVX512_64(self):
        self.case = "bit S eight AVX512 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.shuffle_bit_eightelem_AVX512
        self.check = ext.shuffle_bit_eightelem_scal

    def test_08i_shuffle_bit_eight_AVX512_16(self):
        self.case = "bit S eight AVX512 16"
        self.data = self.data.view(np.int16)
        self.fun = ext.shuffle_bit_eightelem_AVX512
        self.check = ext.shuffle_bit_eightelem_scal

    def test_08i_shuffle_bit_eight_AVX512_128(self):
        self.case = "bit S eight AVX512 128"
        self.data = self.data.view(np.complex128)
        self.fun = ext.shuffle_bit_eightelem_AVX512
        self.check = ext.shuffle_bit_eightelem_scal

    def test_09a_trans_bit_elem_scal_64(self):
        self.case = "bit T elem scal 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_elem_scal
        self.check = trans_bit_elem

    def test_09b_trans_bit_elem_SSE_64(self):
        self.case = "bit T elem SSE 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_elem_SSE
        self.check = trans_bit_elem

    def test_09c_trans_bit_elem_AVX_64(self):
        self.case = "bit T elem AVX 64"
        self.data = self.data.view(np.float64)
        self.fun = ext.trans_bit_elem_AVX
        self.check = trans_bit_elem

    def test_09d_untrans_bit_elem_scal_64(self):
        self.case = "bit U elem scal 64"
        pre_trans = self.data.view(np.float64)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_scal
        self.check_data = pre_trans

    def test_09e_untrans_bit_elem_SSE_64(self):
        self.case = "bit U elem SSE 64"
        pre_trans = self.data.view(np.float64)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_SSE
        self.check_data = pre_trans

    def test_09f_untrans_bit_elem_AVX_64(self):
        self.case = "bit U elem AVX 64"
        pre_trans = self.data.view(np.float64)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_AVX
        self.check_data = pre_trans

    def test_09g_untrans_bit_elem_AVX_64(self):
        self.case = "bit U elem AVX512 64"
        pre_trans = self.data.view(np.float64)
        self.data = trans_bit_elem(pre_trans)
        self.fun = ext.untrans_bit_elem_AVX512
        self.check_data = pre_trans

    def test_10a_bitshuffle_64(self):
        self.case = "bitshuffle 64"
        self.data = self.data.view(np.float64)
        self.fun = lambda x: ext.bitshuffle(x, BLOCK)

    def test_10b_bitunshuffle_64(self):
        self.case = "bitunshuffle 64"
        pre_trans = self.data.view(np.float64)
        self.data = ext.bitshuffle(pre_trans, BLOCK)
        self.fun = lambda x: ext.bitunshuffle(x, BLOCK)
        self.check_data = pre_trans

    def test_10c_compress_64(self):
        self.case = "compress 64"
        self.data = self.data.view(np.float64)
        self.fun = lambda x: ext.compress_lz4(x, BLOCK)

    def test_10d_decompress_64(self):
        self.case = "decompress 64"
        pre_trans = self.data.view(np.float64)
        self.data = ext.compress_lz4(pre_trans, BLOCK)
        self.fun = lambda x: ext.decompress_lz4(
            x, pre_trans.shape, pre_trans.dtype, BLOCK
        )
        self.check_data = pre_trans

    @unittest.skipUnless(__zstd__, "ZSTD support not included")
    def test_10c_compress_z64(self):
        self.case = "compress zstd  64"
        self.data = self.data.view(np.float64)
        self.fun = lambda x: ext.compress_zstd(x, BLOCK)

    @unittest.skipUnless(__zstd__, "ZSTD support not included")
    def test_10d_decompress_z64(self):
        self.case = "decompress zstd 64"
        pre_trans = self.data.view(np.float64)
        self.data = ext.compress_zstd(pre_trans, BLOCK)
        self.fun = lambda x: ext.decompress_zstd(
            x, pre_trans.shape, pre_trans.dtype, BLOCK
        )
        self.check_data = pre_trans


"""
Commented out to prevent nose from finding them.
class TestDevCases(unittest.TestCase):

    def deactivated_test_trans_byte_bitrow_AVX(self):
        d = np.arange(256, dtype=np.uint32)
        #d = ext.trans_bit_elem(d)
        t = ext.trans_byte_bitrow_AVX(d).view(np.uint8)
        t1 = ext.trans_byte_bitrow_SSE(d).view(np.uint8)
        t.shape = (32, 32)
        t1.shape = (32, 32)
        #print t[:20,:18]
        self.assertTrue(np.all(t == t1))

    def deactivated_test_untrans_bit_elem(self):
        d = np.arange(32, dtype=np.uint16)
        #d = random.randint(0, 2**7, 256).astype(np.uint16)
        d1 = ext.trans_bit_elem(d)
        #print d
        t = ext.untrans_bit_elem_AVX(d1)
        #t1 = ext.untrans_bit_byte_scal(d1)
        #print np.reshape(d1.view(np.uint8), (16, 4))
        #print np.reshape(t1.view(np.uint8), (2, 32))
        #print np.reshape(t2.view(np.uint8), (32, 2))
        #print np.reshape(t.view(np.uint8), (32, 2))

    def deactivated_test_trans_bit_byte(self):
        d = np.arange(16, dtype=np.uint16)
        t = ext.trans_bit_byte_scal(d)
        #print t
        t1 = trans_bit_byte(d)
        #print t1
        self.assertTrue(np.all(t == t1))

    def deactivated_test_trans_byte_bitrow_SSE(self):
        d = np.arange(256, dtype = np.uint8)
        t = ext.trans_byte_bitrow_scal(d)
        #print np.reshape(t, (32, 8))
        t1 = ext.trans_byte_bitrow_SSE(d)
        #print np.reshape(t1, (32, 8))
        self.assertTrue(np.all(t == t1))

    def deactivated_test_trans_byte_elem_SSE(self):
        d = np.empty(16, dtype=([('a', 'u4'), ('b', 'u4'), ('c', 'u4')]))
        d['a'] = np.arange(16) * 1
        d['b'] = np.arange(16) * 2
        d['c'] = np.arange(16) * 3
        #print d.dtype.itemsize
        #print np.reshape(d.view(np.uint8), (16, 12))
        t1 = ext.trans_byte_elem_SSE(d)
        #print np.reshape(t1.view(np.uint8), (12, 16))
        t0 = trans_byte_elem(d)
        #print np.reshape(t0.view(np.uint8), (12, 16))
        self.assertTrue(np.all(t0.view(np.uint8) == t1.view(np.uint8)))

    def deactivated_test_bitshuffle(self):
        d = np.arange(128, dtype=np.uint16)
        t1 = ext.bitshuffle(d)
        #print t1
        t2 = ext.bitunshuffle(t1)
        #print t2
        self.assertTrue(np.all(t2.view(np.uint8) == d.view(np.uint8)))
"""


class TestOddLengths(unittest.TestCase):
    def setUp(self):
        self.reps = 10
        self.nmax = 128 * 8
        # self.nmax = 4 * 8    # XXX
        self.fun = ext.copy
        self.check = lambda x: x

    def test_trans_bit_elem_SSE(self):
        self.fun = ext.trans_bit_elem_SSE
        self.check = trans_bit_elem

    def test_untrans_bit_elem_SSE(self):
        self.fun = lambda x: ext.untrans_bit_elem_SSE(ext.trans_bit_elem(x))
        self.check = lambda x: x

    def test_trans_bit_elem_AVX(self):
        self.fun = ext.trans_bit_elem_AVX
        self.check = trans_bit_elem

    def test_trans_bit_elem_AVX512(self):
        self.fun = ext.trans_bit_elem_AVX512
        self.check = trans_bit_elem

    def test_untrans_bit_elem_AVX(self):
        self.fun = lambda x: ext.untrans_bit_elem_SSE(ext.trans_bit_elem(x))
        self.check = lambda x: x

    def test_untrans_bit_elem_AVX512(self):
        self.fun = lambda x: ext.untrans_bit_elem_SSE(ext.trans_bit_elem(x))
        self.check = lambda x: x

    def test_trans_bit_elem_scal(self):
        self.fun = ext.trans_bit_elem_scal
        self.check = trans_bit_elem

    def test_untrans_bit_elem_scal(self):
        self.fun = lambda x: ext.untrans_bit_elem_scal(ext.trans_bit_elem(x))
        self.check = lambda x: x

    def test_trans_byte_elem_SSE(self):
        self.fun = ext.trans_byte_elem_SSE
        self.check = trans_byte_elem

    def tearDown(self):
        try:
            for dtype in TEST_DTYPES:
                itemsize = np.dtype(dtype).itemsize
                nbyte_max = self.nmax * itemsize
                dbuf = random.randint(0, 255, nbyte_max).astype(np.uint8)
                dbuf = dbuf.view(dtype)
                for ii in range(self.reps):
                    n = random.randint(0, self.nmax // 8, 1)[0] * 8
                    data = dbuf[:n]
                    out = self.fun(data).view(np.uint8)
                    ans = self.check(data).view(np.uint8)
                    self.assertTrue(np.all(out == ans))
        except RuntimeError as err:
            if len(err.args) > 1 and (err.args[1] == -11) and not ext.using_SSE2():
                return
            if len(err.args) > 1 and (err.args[1] == -12) and not ext.using_AVX2():
                return
            if len(err.args) > 1 and (err.args[1] == -14) and not ext.using_AVX512():
                return
            else:
                raise


class TestBitShuffleCircle(unittest.TestCase):
    """Ensure that final filter is circularly consistent for any data type and
    any length buffer."""

    def test_circle(self):
        nmax = 100000
        reps = 20
        for dtype in TEST_DTYPES:
            itemsize = np.dtype(dtype).itemsize
            nbyte_max = nmax * itemsize
            dbuf = random.randint(0, 255, nbyte_max).astype(np.uint8)
            dbuf = dbuf.view(dtype)
            for ii in range(reps):
                n = random.randint(0, nmax, 1)[0]
                data = dbuf[:n]
                shuff = ext.bitshuffle(data)
                out = ext.bitunshuffle(shuff)
                self.assertTrue(out.dtype is data.dtype)
                self.assertTrue(np.all(data.view(np.uint8) == out.view(np.uint8)))

    def test_circle_with_compression(self):
        nmax = 100000
        reps = 20
        for dtype in TEST_DTYPES:
            itemsize = np.dtype(dtype).itemsize
            nbyte_max = nmax * itemsize
            dbuf = random.randint(0, 255, nbyte_max).astype(np.uint8)
            dbuf = dbuf.view(dtype)
            for ii in range(reps):
                n = random.randint(0, nmax, 1)[0]
                data = dbuf[:n]
                shuff = ext.compress_lz4(data)
                out = ext.decompress_lz4(shuff, data.shape, data.dtype)
                self.assertTrue(out.dtype is data.dtype)
                self.assertTrue(np.all(data.view(np.uint8) == out.view(np.uint8)))

    @unittest.skipUnless(__zstd__, "ZSTD support not included")
    def test_circle_with_zstd_compression(self):
        nmax = 100000
        reps = 20
        for dtype in TEST_DTYPES:
            itemsize = np.dtype(dtype).itemsize
            nbyte_max = nmax * itemsize
            dbuf = random.randint(0, 255, nbyte_max).astype(np.uint8)
            dbuf = dbuf.view(dtype)
            for ii in range(reps):
                n = random.randint(0, nmax, 1)[0]
                data = dbuf[:n]
                shuff = ext.compress_zstd(data)
                out = ext.decompress_zstd(shuff, data.shape, data.dtype)
                self.assertTrue(out.dtype is data.dtype)
                self.assertTrue(np.all(data.view(np.uint8) == out.view(np.uint8)))


# Python implementations for checking results.


def trans_byte_elem(arr):
    dtype = arr.dtype
    itemsize = dtype.itemsize
    in_buf = arr.flat[:].view(np.uint8)
    nelem = in_buf.size // itemsize
    in_buf.shape = (nelem, itemsize)

    out_buf = np.empty((itemsize, nelem), dtype=np.uint8)
    for ii in range(nelem):
        for jj in range(itemsize):
            out_buf[jj, ii] = in_buf[ii, jj]
    return out_buf.flat[:].view(dtype)


def trans_bit_byte(arr):
    n = arr.size
    dtype = arr.dtype
    itemsize = dtype.itemsize
    bits = np.unpackbits(arr.view(np.uint8))
    bits.shape = (n * itemsize, 8)
    # We have to reverse the order of the bits both for unpacking and packing,
    # since we want to call the least significant bit the first bit.
    bits = bits[:, ::-1]
    bits_shuff = (bits.T).copy()
    bits_shuff.shape = (n * itemsize, 8)
    bits_shuff = bits_shuff[:, ::-1]
    arr_bt = np.packbits(bits_shuff.flat[:])
    return arr_bt.view(dtype)


def trans_bit_elem(arr):
    n = arr.size
    dtype = arr.dtype
    itemsize = dtype.itemsize
    bits = np.unpackbits(arr.view(np.uint8))
    bits.shape = (n * itemsize, 8)
    # We have to reverse the order of the bits both for unpacking and packing,
    # since we want to call the least significant bit the first bit.
    bits = bits[:, ::-1].copy()
    bits.shape = (n, itemsize * 8)
    bits_shuff = (bits.T).copy()
    bits_shuff.shape = (n * itemsize, 8)
    bits_shuff = bits_shuff[:, ::-1]
    arr_bt = np.packbits(bits_shuff.flat[:])
    return arr_bt.view(dtype)


if __name__ == "__main__":
    unittest.main()
