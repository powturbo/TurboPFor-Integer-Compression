/**
 * This code is released under a BSD License.
 */
#include <stdio.h>
#include <stdlib.h>
#include "simdcomp.h"


int main() {
    int N = 5000 * SIMDBlockSize;
    __m128i * buffer = malloc(SIMDBlockSize * sizeof(uint32_t));
    uint32_t * datain = malloc(N * sizeof(uint32_t));
    uint32_t * backbuffer = malloc(SIMDBlockSize * sizeof(uint32_t));
    for (int gap = 1; gap <= 387420489; gap *= 3) {
        printf(" gap = %u \n", gap);
        for (int k = 0; k < N; ++k)
            datain[k] = k * gap;
        uint32_t offset = 0;
        for (int k = 0; k * SIMDBlockSize < N; ++k) {
            /////////////////////////////
            // First part works for general arrays (sorted or unsorted)
            /////////////////////////////
       	    // we compute the bit width
            const uint32_t b = maxbits(datain + k * SIMDBlockSize);
            // we read 128 integers at "datain + k * SIMDBlockSize" and
            // write b 128-bit vectors at "buffer"
            simdpackwithoutmask(datain + k * SIMDBlockSize, buffer, b);
            // we read back b1 128-bit vectors at "buffer" and write 128 integers at backbuffer
            simdunpack(buffer, backbuffer, b);//uncompressed
            for (int j = 0; j < SIMDBlockSize; ++j) {
                if (backbuffer[j] != datain[k * SIMDBlockSize + j]) {
                    printf("bug in simdpack\n");
                    return -2;
                }
            }
            /////////////////////////////
            // next part assumes that the data is sorted (uses differential coding)
            /////////////////////////////
            // we compute the bit width
            const uint32_t b1 = simdmaxbitsd1(offset,
                    datain + k * SIMDBlockSize);
            // we read 128 integers at "datain + k * SIMDBlockSize" and
            // write b1 128-bit vectors at "buffer"
            simdpackwithoutmaskd1(offset, datain + k * SIMDBlockSize, buffer,
                    b1);
            // we read back b1 128-bit vectors at "buffer" and write 128 integers at backbuffer
            simdunpackd1(offset, buffer, backbuffer, b1);
            for (int j = 0; j < SIMDBlockSize; ++j) {
                if (backbuffer[j] != datain[k * SIMDBlockSize + j]) {
                    printf("bug in simdpack d1\n");
                    return -3;
                }
            }
            offset = datain[k * SIMDBlockSize + SIMDBlockSize - 1];

        }
    }
    free(buffer);
    free(datain);
    free(backbuffer);
    printf("Code looks good.\n");
    return 0;
}
