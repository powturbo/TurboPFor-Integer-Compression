/**
 *  @file defines.h
 *  @author Sheng Di
 *  @date July, 2019
 *  @brief Header file for the dataCompression.c.
 *  (C) 2016 by Mathematics and Computer Science (MCS), Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef _BG_DEFINES_H
#define _BG_DEFINES_H

#define BG_VER_MAJOR 2
#define BG_VER_MINOR 1
#define BG_VER_BUILD 9
#define BG_VER_REVISION 0

#define BG 105
#define BITGROOM 0
#define BITSHAVE 1
#define BITSET 2

#define BG_FLOAT 0
#define BG_DOUBLE 1
#define BG_UINT8 2
#define BG_INT8 3
#define BG_UINT16 4
#define BG_INT16 5
#define BG_UINT32 6
#define BG_INT32 7
#define BG_UINT64 8
#define BG_INT64 9

#define BG_NSD 0
#define BG_DSD 1

#define LITTLE_ENDIAN_DATA 0 //refers to the endian type of the data read from the disk
#define BIG_ENDIAN_DATA 1 //big_endian (ppc, max, etc.) ; little_endian (x86, x64, etc.)

#define LITTLE_ENDIAN_SYSTEM 0 //refers to the endian type of the system
#define BIG_ENDIAN_SYSTEM 1

#define DynArrayInitLen 1024

//SUCCESS returning status
#define BG_SCES 0  //successful
#define BG_NSCS -1 //Not successful
#define BG_FERR -2 //Failed to open input file
#define BG_TERR -3 //wrong data type (should be only float or double)
#define BG_DERR -4 //dimension error
#define BG_MERR -5 //sz_mode error
#define BG_BERR -6 //bound-mode error (should be only ABS, REL, ABS_AND_REL, ABS_OR_REL, or PW_REL)

#endif /* _BG_DEFINES_H */
