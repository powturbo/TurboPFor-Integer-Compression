// C port Version of "VarIntG8IU.h" from https://github.com/lemire/FastPFor
/**
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 */
/**
 *
 * Implementation of varint-G8IU taken from
 * Stepanov et al., SIMD-Based Decoding of Posting Lists, CIKM 2011
 *
 * Update: D. Lemire believes that this scheme was patented by Rose, Stepanov et al. (patent 20120221539).
 * We wrote this code before the patent was published (August 2012).
 *
 * By Maxime Caron
 * From
 * https://github.com/maximecaron/SIMD-Based-Posting-lists
 * with minor modifications by D. Lemire.
 */
#include <string.h>
#ifndef __SSSE3__
#pragma message "Disabling varintg8iu due to lack of SSSE3 support, try adding -mssse3"
#else
#ifndef VARINTG8IU_H__
#define VARINTG8IU_H__
#include <emmintrin.h>
//#include "codecs.h"
#ifdef __GNUC__
#define PREDICT_FALSE(x) (__builtin_expect(x, 0))
#define PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else
#define PREDICT_FALSE(x) x
#define PREDICT_TRUE(x) x
#endif
#include "varintg8iu.h"

typedef char v16qi __attribute__ ((vector_size (16)));

static int maskOutputSize[256];
static char mask[256][32];

    int getNumByteNeeded(const uint32_t value) {
        if (value > 0x000000FF) {
            if (value > 0x0000FFFF) {
                if (value > 0x00FFFFFF) {
                    return 4;
                } else {
                    return 3;
                }
            } else {
                return 2;
            }
        } else {
            return 1;
        }
    }


    // For all possible values of the
    // descriptor we build a table of any shuffle sequence
    // that might be needed at decode time.
void VarIntG8IU() {
        for (int desc = 0; desc <= 255; desc++) {
            int bitmask = 0x00000001;
            int bitindex = 0;
            // count number of 0 in the char
            int complete = 0;
            int ithSize[8];
            int lastpos = -1;
            while (bitindex < 8) {
                if ((desc & bitmask) == 0) {
                    ithSize[complete] = bitindex - lastpos;
                    lastpos = bitindex;
                    complete++;
                }
                bitindex++;
                bitmask = bitmask << 1;
            }
            maskOutputSize[desc] = complete;

            int j = 0;
            int k = 0;
            for (int i = 0; i < complete; i++) {
                for (int n = 0; n < 4; n++) {
                    if (n < ithSize[i]) {
                        mask[desc][k] = j;
                        j = j + 1;
                    } else {
                        mask[desc][k] = -1;
                    }
                    k = k + 1;
                }
            }

        }

    }

unsigned char *vintg8enc(const uint32_t *__restrict in, const size_t length, unsigned char *__restrict out) {
        const uint32_t *in_ = in + length; //size_t srclength = length * 4;unsigned char* dest = (unsigned char*)(out);size_t dstlength = length * 4;
        //size_t compressed_size = 0;
        while(in < in_ /*srclength > 0 && dstlength >= 9*/) {    //compressed_size += encodeBlock(in, srclength, dst, nvalue);
          unsigned char desc = 0xFF;
          unsigned char bitmask = 0x01;
          uint32_t buffer[8];
          int ithSize[8];
          int length = 0;
          int numInt = 0;

          while (in < in_ /*srclength > 0*/) {
            const uint32_t* temp = in;
            int byteNeeded = getNumByteNeeded(*temp);

            if (PREDICT_FALSE(length + byteNeeded > 8)) {
                break;
            }

            //flip the correct bit in desc
            bitmask = bitmask << (byteNeeded - 1);
            desc = desc ^ bitmask;
            bitmask = bitmask << 1;

            ithSize[numInt] = byteNeeded;
            length += byteNeeded;
            buffer[numInt] = *temp;
            ++in;// = in + 1;
            //srclength -= 4;
            numInt++;
          }
          out[0] = desc;
          int written = 1;
          for(int i = 0; i < numInt; i++) {
            int size = ithSize[i];
            uint32_t value = buffer[i];
            for (int j = 0; j < size; j++) {
              out[written++] = value >> (j * 8);
            }
          }
          out += 9;          //dstlength -= 9;        //compressed_size += 9;
        }
        //Ouput might not be a multiple of 4 so we make it so
        return out; //out + ((compressed_size + 3)/ 4);
    }

#define __builtin_ia32_storedqu(dst, result) _mm_storeu_si128(dst,(__m128i )result)

unsigned char *vintg8dec(unsigned char *__restrict in, const size_t length, uint32_t *__restrict out) {
        size_t srclength = length * 4;
        const unsigned *out_ = out + length; //uint32_t * dest = out;size_t nvalue = length * 4;    //uint32_t uncompressSize = 0;
        while (out < out_ /*srclength >= 9*/) {           //uncompressSize += decodeBlock(in, srclength, dst/*, nvalue*/);
          const unsigned char* pdesc = in++;
          unsigned char desc = *pdesc;
          srclength -= 1;

          const unsigned char* peek = in;
          v16qi data;
          if (PREDICT_TRUE(srclength >= 16)) {
            // read 16 byte of data only if we need to avoid cache miss
            data = __builtin_ia32_lddqu((const char*) (peek));
          } else {
            static char buff[16];
            memcpy(buff, peek, 8);
            data = __builtin_ia32_lddqu(buff);
          }
          // load de required mask
          v16qi shf    = __builtin_ia32_lddqu(mask[desc]);
          v16qi result = __builtin_ia32_pshufb128(data, shf);
          char* dst = (char*) (out);
          __builtin_ia32_storedqu(dst, result);
          int readSize = maskOutputSize[desc];

          if (PREDICT_TRUE( readSize >= 4)) {
            v16qi shf2    = __builtin_ia32_lddqu(mask[desc] + 16);
            v16qi result2 = __builtin_ia32_pshufb128(data, shf2);
            __builtin_ia32_storedqu(dst + (16), result2);
          }
          // pop 8 input char
          in += 8; srclength -= 8; out += readSize;  //dstlength -= readSize * 4;//      uncompressSize += readSize;
        }
        return in; //(uint32_t *) (((uintptr_t) (src) + 3) & ~3);

}

#endif //__SSE3__
#endif
