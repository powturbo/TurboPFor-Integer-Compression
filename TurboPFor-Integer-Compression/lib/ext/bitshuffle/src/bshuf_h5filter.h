/*
 * Bitshuffle HDF5 filter
 *
 * This file is part of Bitshuffle
 * Author: Kiyoshi Masui <kiyo@physics.ubc.ca>
 * Website: http://www.github.com/kiyo-masui/bitshuffle
 * Created: 2014
 *
 * See LICENSE file for details about copyright and rights to use.
 *
 *
 * Header File
 *
 * Filter Options
 * --------------
 *  block_size (option slot 0) : integer (optional)
 *      What block size to use (in elements not bytes). Default is 0,
 *      for which bitshuffle will pick a block size with a target of 8kb.
 *  Compression (option slot 1) : 0 or BSHUF_H5_COMPRESS_LZ4
 *      Whether to apply LZ4 compression to the data after bitshuffling.
 *      This is much faster than applying compression as a second filter
 *      because it is done when the small block of data is already in the
 *      L1 cache.
 *
 *      For LZ4 compression, the compressed format of the data is the same as
 *      for the normal LZ4 filter described in
 *      http://www.hdfgroup.org/services/filters/HDF5_LZ4.pdf.
 *
 */


#ifndef BSHUF_H5FILTER_H
#define BSHUF_H5FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#define H5Z_class_t_vers 2
#include "hdf5.h"


#define BSHUF_H5FILTER 32008


#define BSHUF_H5_COMPRESS_LZ4 2
#define BSHUF_H5_COMPRESS_ZSTD 3


extern H5Z_class_t bshuf_H5Filter[1];


/* ---- bshuf_register_h5filter ----
 *
 * Register the bitshuffle HDF5 filter within the HDF5 library.
 *
 * Call this before using the bitshuffle HDF5 filter from C unless
 * using dynamically loaded filters.
 *
 */
int bshuf_register_h5filter(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // BSHUF_H5FILTER_H
