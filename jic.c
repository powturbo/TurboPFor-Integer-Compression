/**
    Copyright (C) powturbo 2013-2017
    GPL v2 License
  
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    - homepage : https://sites.google.com/site/powturbo/
    - github   : https://github.com/powturbo
    - twitter  : https://twitter.com/powturbo
    - email    : powturbo [_AT_] gmail [_DOT_] com
**/
//     ic - "Integer Compression" Java Critical Native Interface
#include <stdio.h>
#include <stdlib.h>
#include "bitutil.h"
#include "vint.h"
#include "vsimple.h"
#include "bitpack.h"
#include "bitunpack.h"
#include "vp4dc.h"
#include "vp4dd.h"

#include "jic.h"

#define JNIB jboolean _inlock, _outlock;\
  jint i  = (*env)->GetArrayLength(env, _in);  jbyte *in  = (jbyte *)(*env)->GetPrimitiveArrayCritical(env, _in, &_inlock);\
  jint o = (*env)->GetArrayLength(env, _out); jbyte *out = (jbyte *)(*env)->GetPrimitiveArrayCritical(env, _out,&_outlock)

#define JNIE\
 (*env)->ReleasePrimitiveArrayCritical(env, _in,   in, JNI_ABORT);\
 (*env)->ReleasePrimitiveArrayCritical(env, _out, out, JNI_ABORT)

#define JNIENC(__func, _in, n, _out        ) JNIB; jint l = __func ((unsigned      *)in, n,(unsigned char *)out      ) - (unsigned char *)out; JNIE; return l
#define JNIDEC(__func, _in, n, _out        ) JNIB; jint l = __func ((unsigned char *)in, n,(unsigned      *)out      ) - (unsigned char *)in;  JNIE; return l

#define JNIENCS(__func, _in, n, _out,  x   ) JNIB; jint l = __func ((unsigned      *)in, n,(unsigned char *)out, x   ) - (unsigned char *)out; JNIE; return l
#define JNIDECS(__func, _in, n, _out,  x   ) JNIB; jint l = __func ((unsigned char *)in, n,(unsigned      *)out, x   ) - (unsigned char *)in;  JNIE; return l

#define JNIENCSB(__func, _in, n, _out, x, b) JNIB; jint l = __func ((unsigned      *)in, n,(unsigned char *)out, x, b) - (unsigned char *)out; JNIE; return l
#define JNIDECSB(__func, _in, n, _out, x, b) JNIB; jint l = __func ((unsigned char *)in, n,(unsigned      *)out, x, b) - (unsigned char *)in;  JNIE; return l

#define JNIBI jboolean _inlock; jint i = (*env)->GetArrayLength(env, _in); jbyte *in  = (jbyte *)(*env)->GetPrimitiveArrayCritical(env, _in, &_inlock);

#define JNIEI (*env)->ReleasePrimitiveArrayCritical(env, _in,   in, JNI_ABORT);

#define JNIBIT( __func, _in, n)    JNIBI; jint l = __func ((unsigned      *)in, n   ); JNIEI; return l
#define JNIBITS(__func, _in, n, x) JNIBI; jint l = __func ((unsigned      *)in, n, x); JNIEI; return l

//---------------------------------- variable byte ------------------------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_jic_vbenc32(       JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out                    ) { JNIENC(vbenc32,          _in, n, _out          ); } JNIEXPORT jint JNICALL JavaCritical_jic_vbenc32(         jint i,          jint *in, jint n, jint o, jbyte *out                    ) { return vbenc32(            (unsigned *)in, n, (unsigned char *)out          ) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_vbdec32(       JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out                    ) { JNIDEC(vbdec32,          _in, n, _out          ); } JNIEXPORT jint JNICALL JavaCritical_jic_vbdec32(         jint i,         jbyte *in, jint n, jint o,  jint *out                    ) { return vbdec32(       (unsigned char *)in, n,      (unsigned *)out          ) - (unsigned char *)in; }

JNIEXPORT jint JNICALL Java_jic_vbdenc32(      JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out, jint start        ) { JNIENCS(vbdenc32,        _in, n, _out, start   ); } JNIEXPORT jint JNICALL JavaCritical_jic_vbdenc32(        jint i,          jint *in, jint n, jint o, jbyte *out, jint start        ) { return vbdenc32(           (unsigned *)in, n, (unsigned char *)out, start   ) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_vbddec32(      JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out, jint start        ) { JNIDECS(vbddec32,        _in, n, _out, start   ); } JNIEXPORT jint JNICALL JavaCritical_jic_vbddec32(        jint i,         jbyte *in, jint n, jint o,  jint *out, jint start        ) { return vbddec32(      (unsigned char *)in, n,      (unsigned *)out, start   ) - (unsigned char *)in; }

JNIEXPORT jint JNICALL Java_jic_vbd1enc32(     JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out, jint start        ) { JNIENCS(vbd1enc32,       _in, n, _out, start   ); } JNIEXPORT jint JNICALL JavaCritical_jic_vbd1enc32(       jint i,          jint *in, jint n, jint o, jbyte *out, jint start        ) { return vbd1enc32(          (unsigned *)in, n, (unsigned char *)out, start   ) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_vbd1dec32(     JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out, jint start        ) { JNIDECS(vbd1dec32,       _in, n, _out, start   ); } JNIEXPORT jint JNICALL JavaCritical_jic_vbd1dec32(       jint i,         jbyte *in, jint n, jint o,  jint *out, jint start        ) { return vbd1dec32(     (unsigned char *)in, n,      (unsigned *)out, start   ) - (unsigned char *)in; }
//--------------------------------- variable simple ----------------------------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_jic_vsenc32(      JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out                    ) { JNIENC(vsenc32,          _in, n, _out           ); } JNIEXPORT jint JNICALL JavaCritical_jic_vsenc32(         jint i,          jint *in, jint n, jint o, jbyte *out                    ) { return vsenc32(            (unsigned *)in, n, (unsigned char *)out          ) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_vsdec32(      JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out                    ) { JNIDEC(vsdec32,          _in, n, _out           ); } JNIEXPORT jint JNICALL JavaCritical_jic_vsdec32(         jint i,         jbyte *in, jint n, jint o,  jint *out                    ) { return vsdec32(       (unsigned char *)in, n,      (unsigned *)out          ) - (unsigned char *)in; }
//--------------------------------- TurboPFor: PFor/PForDelta -----------------------------------------------------------------
JNIEXPORT jint JNICALL Java_jic_p4denc32(      JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out                    ) { JNIENC(p4denc32,         _in, n, _out          ); } JNIEXPORT jint JNICALL JavaCritical_jic_p4denc32(        jint i,          jint *in, jint n, jint o, jbyte *out                    ) { return p4denc32(           (unsigned *)in, n, (unsigned char *)out          ) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_p4ddec32(      JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out                    ) { JNIDEC(p4ddec32,         _in, n, _out          ); } JNIEXPORT jint JNICALL JavaCritical_jic_p4ddec32(        jint i,         jbyte *in, jint n, jint o,  jint *out                    ) { return p4ddec32(      (unsigned char *)in, n,      (unsigned *)out          ) - (unsigned char *)in; }

JNIEXPORT jint JNICALL Java_jic_p4dencv32(     JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out                    ) { JNIENC(p4dencv32,        _in, n, _out          ); } JNIEXPORT jint JNICALL JavaCritical_jic_p4dencv32(       jint i,          jint *in, jint n, jint o, jbyte *out                    ) { return p4dencv32(          (unsigned *)in, n, (unsigned char *)out          ) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_p4ddecv32(     JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out                    ) { JNIDEC(p4ddecv32,        _in, n, _out          ); } JNIEXPORT jint JNICALL JavaCritical_jic_p4ddecv32(       jint i,         jbyte *in, jint n, jint o,  jint *out                    ) { return p4ddecv32(     (unsigned char *)in, n,      (unsigned *)out          ) - (unsigned char *)in; }
//--------
JNIEXPORT jint JNICALL Java_jic_p4dddec32(     JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out, jint start        ) { JNIDECS(p4dddec32,        _in, n, _out, start  ); } JNIEXPORT jint JNICALL JavaCritical_jic_p4dddec32(       jint i,         jbyte *in, jint n, jint o,  jint *out, jint start        ) { return p4dddec32(     (unsigned char *)in, n,      (unsigned *)out, start   ) - (unsigned char *)in; }

JNIEXPORT jint JNICALL Java_jic_p4dd1dec32(    JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out, jint start        ) { JNIDECS(p4dd1dec32,       _in, n, _out, start  ); } JNIEXPORT jint JNICALL JavaCritical_jic_p4dd1dec32(      jint i,         jbyte *in, jint n, jint o,  jint *out, jint start        ) { return p4dd1dec32(    (unsigned char *)in, n,      (unsigned *)out, start   ) - (unsigned char *)in; }

//--------------------------------- BitPack/BitUnpack ----------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_jic_bitpack32(     JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out,             jint b) { JNIENCS(bitpack32,       _in, n, _out,        b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitpack32(     jint i,            jint *in, jint n, jint o, jbyte *out,             jint b) { return bitpack32(          (unsigned *)in, n, (unsigned char *)out,        b) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_bitunpack32(   JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out,             jint b) { JNIDECS(bitunpack32,     _in, n, _out,        b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitunpack32(   jint i,           jbyte *in, jint n, jint o,  jint *out,             jint b) { return bitunpack32(   (unsigned char *)in, n,      (unsigned *)out,        b) - (unsigned char *)in; }


JNIEXPORT jint JNICALL Java_jic_bitdpack32(    JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out, jint start, jint b) { JNIENCSB(bitdpack32,     _in, n, _out, start, b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitdpack32(    jint i,            jint *in, jint n, jint o, jbyte *out, jint start, jint b) { return bitdpack32((         unsigned *)in, n, (unsigned char *)out, start, b) - (unsigned char *)out;}

JNIEXPORT jint JNICALL Java_jic_bitdunpack32(  JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out, jint start, jint b) { JNIDECSB(bitdunpack32,   _in, n, _out, start, b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitdunpack32(  jint i,           jbyte *in, jint n, jint o,  jint *out, jint start, jint b) { return bitdunpack32((  unsigned char *)in, n,      (unsigned *)out, start, b) - (unsigned char *)in; }

JNIEXPORT jint JNICALL Java_jic_bitd1pack32(   JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out, jint start, jint b) { JNIENCSB(bitd1pack32,    _in, n, _out, start, b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitd1pack32(   jint i,            jint *in, jint n, jint o, jbyte *out, jint start, jint b) { return bitd1pack32((       unsigned  *)in, n, (unsigned char *)out, start, b) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_bitd1unpack32( JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out, jint start, jint b) { JNIDECSB(bitd1unpack32,  _in, n, _out, start, b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitd1unpack32( jint i,           jbyte *in, jint n, jint o,  jint *out, jint start, jint b) { return bitd1unpack32((unsigned char  *)in, n,      (unsigned *)out, start, b) - (unsigned char *)in; }

//--------------------------------- BitPack/BitUnpack SIMD -----------------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_jic_bitpackv32(    JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out,             jint b) { JNIENCS(bitpackv32,      _in, n, _out,        b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitpackv32(    jint i,            jint *in, jint n, jint o, jbyte *out,             jint b) { return bitpackv32(    (unsigned      *)in, n, (unsigned char *)out,        b) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_bitunpackv32(  JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out,             jint b) { JNIDECS(bitunpackv32,    _in, n, _out,        b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitunpackv32(  jint i,           jbyte *in, jint n, jint o,  jint *out,             jint b) { return bitunpackv32(  (unsigned char *)in, n,      (unsigned *)out,        b) - (unsigned char *)in; }

JNIEXPORT jint JNICALL Java_jic_bitdpackv32(   JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out, jint start, jint b) { JNIENCSB(bitdpackv32,    _in, n, _out, start, b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitdpackv32(   jint i,            jint *in, jint n, jint o, jbyte *out, jint start, jint b) { return bitdpackv32(   (unsigned      *)in, n, (unsigned char *)out, start, b) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_bitdunpackv32( JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out, jint start, jint b) { JNIDECSB(bitdunpackv32,  _in, n, _out, start, b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitdunpackv32( jint i,           jbyte *in, jint n, jint o,  jint *out, jint start, jint b) { return bitdunpackv32( (unsigned char *)in, n,      (unsigned *)out, start, b) - (unsigned char *)in; }

JNIEXPORT jint JNICALL Java_jic_bitd1packv32(  JNIEnv *env, jclass cls,  jintArray _in, jint n, jbyteArray _out, jint start, jint b) { JNIENCSB(bitd1packv32,   _in, n, _out, start, b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitd1packv32(  jint i,            jint *in, jint n, jint o, jbyte *out, jint start, jint b) { return bitd1packv32(  (unsigned      *)in, n, (unsigned char *)out, start, b) - (unsigned char *)out; }

JNIEXPORT jint JNICALL Java_jic_bitd1unpackv32(JNIEnv *env, jclass cls, jbyteArray _in, jint n,  jintArray _out, jint start, jint b) { JNIDECSB(bitd1unpackv32, _in, n, _out, start, b); } JNIEXPORT jint JNICALL JavaCritical_jic_bitd1unpackv32(jint i,           jbyte *in, jint n, jint o,  jint *out, jint start, jint b) { return bitd1unpackv32((unsigned char *)in, n,      (unsigned *)out, start, b) - (unsigned char *)in; }
//--------------------------------- bitutil --------------------------------------------------------------------------------------------------
JNIEXPORT jint JNICALL Java_jic_bit32(         JNIEnv *env, jclass cls,  jintArray _in, jint n                                     ) { JNIBIT( bit32,           _in, n                ); } JNIEXPORT jint JNICALL JavaCritical_jic_bit32(         jint i,            jint *in, jint n                                        ) { return bit32(         (unsigned      *)in, n                              ); }
JNIEXPORT jint JNICALL Java_jic_bitd32(        JNIEnv *env, jclass cls,  jintArray _in, jint n,                  jint start        ) { JNIBITS(bitd32,          _in, n,       start   ); } JNIEXPORT jint JNICALL JavaCritical_jic_bitd32(        jint i,            jint *in, jint n,                     jint start        ) { return bitd32(        (unsigned      *)in, n,                       start ); }

JNIEXPORT jint JNICALL Java_jic_bitd132(       JNIEnv *env, jclass cls,  jintArray _in, jint n,                  jint start        ) { JNIBITS(bitd132,         _in, n,       start   ); } JNIEXPORT jint JNICALL JavaCritical_jic_bitd132(       jint i,            jint *in, jint n,                     jint start        ) { return bitd132(       (unsigned      *)in, n,                       start ); }


