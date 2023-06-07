/*
 * Dynamically loaded filter plugin for HDF5 Bitshuffle filter.
 *
 * This file is part of Bitshuffle
 * Author: Kiyoshi Masui <kiyo@physics.ubc.ca>
 * Website: http://www.github.com/kiyo-masui/bitshuffle
 * Created: 2014
 *
 * See LICENSE file for details about copyright and rights to use.
 *
 */


#include "bshuf_h5filter.h"
#include "H5PLextern.h"

H5PL_type_t H5PLget_plugin_type(void) {return H5PL_TYPE_FILTER;}
const void* H5PLget_plugin_info(void) {return bshuf_H5Filter;}

