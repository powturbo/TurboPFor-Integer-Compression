/*
SPDP code: SPDP is a unified compression/decompression algorithm that works
well on both binary 32-bit single-precision (float) and binary 64-bit double-
precision (double) floating-point data.

Copyright (c) 2016, Texas State University. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted for academic, research, experimental, or personal use provided
that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice,
     this list of conditions, and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions, and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   * Neither the name of Texas State University nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

For all other uses, please contact the Office for Commercialization and Industry
Relations at Texas State University <http://www.txstate.edu/ocir/>.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Authors: Martin Burtscher and Steven Claggett
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_TABLE_SIZE (1 << 18)

typedef unsigned char byte_t;
typedef unsigned int word_t;


static size_t spdp_compress(const byte_t level, const size_t length, byte_t* const buf1, byte_t* const buf2)
{
  word_t* in = (word_t*)buf1;
  word_t* out = (word_t*)buf2;
  size_t len = length / sizeof(word_t);

  word_t prev2 = 0;
  word_t prev1 = 0;
  size_t pos;
  for (pos = 0; pos < len; pos++) {
    word_t curr = in[pos];
    out[pos] = curr - prev2;
    prev2 = prev1;
    prev1 = curr;
  }

  for (pos = len * sizeof(word_t); pos < length; pos++) {
    buf2[pos] = buf1[pos];
  }

  byte_t prev = 0;
  size_t wpos = 0;
  size_t d;
  for (d = 0; d < 8; d++) {
    size_t rpos;
    for (rpos = d; rpos < length; rpos += 8) {
      byte_t curr = buf2[rpos];
      buf1[wpos] = curr - prev;
      prev = curr;
      wpos++;
    }
  }

  size_t predtabsize = 1 << (level + 9);
  if (predtabsize > MAX_TABLE_SIZE) predtabsize = MAX_TABLE_SIZE;
  const size_t predtabsizem1 = predtabsize - 1;

  unsigned int lastpos[MAX_TABLE_SIZE];
  memset(lastpos, 0, predtabsize * sizeof(unsigned int));

  size_t rpos = 0;
  wpos = 0;
  unsigned int hist = 0;
  while (rpos < length) {
    byte_t val = buf1[rpos];
    unsigned int lpos = lastpos[hist];
    if (lpos >= 6) {
      if ((buf1[lpos - 6] == buf1[rpos - 6]) && (buf1[lpos - 5] == buf1[rpos - 5]) &&
          (buf1[lpos - 4] == buf1[rpos - 4]) && (buf1[lpos - 3] == buf1[rpos - 3]) &&
          (buf1[lpos - 2] == buf1[rpos - 2]) && (buf1[lpos - 1] == buf1[rpos - 1])) {
        byte_t cnt = 0;
        while ((val == buf1[lpos]) && (cnt < 255) && (rpos < (length - 1))) {
          lastpos[hist] = rpos;
          hist = ((hist << 2) ^ val) & predtabsizem1;
          rpos++;
          lpos++;
          cnt++;
          val = buf1[rpos];
        }
        buf2[wpos] = cnt;
        wpos++;
      }
    }
    buf2[wpos] = val;
    wpos++;
    lastpos[hist] = rpos;
    hist = ((hist << 2) ^ val) & predtabsizem1;
    rpos++;
  }

  return wpos;
}

static void spdp_decompress(const byte_t level, const size_t length, byte_t* const buf2, byte_t* const buf1)
{
  unsigned int predtabsize = 1 << (level + 9);
  if (predtabsize > MAX_TABLE_SIZE) predtabsize = MAX_TABLE_SIZE;
  const unsigned int predtabsizem1 = predtabsize - 1;

  unsigned int lastpos[MAX_TABLE_SIZE];
  memset(lastpos, 0, predtabsize * sizeof(unsigned int));

  size_t rpos = 0;
  size_t wpos = 0;
  unsigned int hist = 0;
  while (rpos < length) {
    unsigned int lpos = lastpos[hist];
    if (lpos >= 6) {
      if ((buf1[lpos - 6] == buf1[wpos - 6]) && (buf1[lpos - 5] == buf1[wpos - 5]) &&
          (buf1[lpos - 4] == buf1[wpos - 4]) && (buf1[lpos - 3] == buf1[wpos - 3]) &&
          (buf1[lpos - 2] == buf1[wpos - 2]) && (buf1[lpos - 1] == buf1[wpos - 1])) {
        byte_t cnt = buf2[rpos];
        rpos++;
        byte_t j;
        for (j = 0; j < cnt; j++) {
          byte_t val = buf1[wpos] = buf1[lpos];
          lastpos[hist] = wpos;
          hist = ((hist << 2) ^ val) & predtabsizem1;
          wpos++;
          lpos++;
        }
      }
    }
    byte_t val = buf1[wpos] = buf2[rpos];
    lastpos[hist] = wpos;
    hist = ((hist << 2) ^ val) & predtabsizem1;
    wpos++;
    rpos++;
  }
  const size_t usize = wpos;

  byte_t val = 0;
  rpos = 0;
  size_t d;
  for (d = 0; d < 8; d++) {
    size_t wpos;
    for (wpos = d; wpos < usize; wpos += 8) {
      val += buf1[rpos];
      buf2[wpos] = val;
      rpos++;
    }
  }

  word_t* in = (word_t*)buf2;
  word_t* out = (word_t*)buf1;
  const size_t len = usize / sizeof(word_t);

  word_t prev2 = 0;
  word_t prev1 = 0;
  size_t pos;
  for (pos = 0; pos < len; pos++) {
    word_t curr = in[pos] + prev2;
    out[pos] = curr;
    prev2 = prev1;
    prev1 = curr;
  }
  for (pos = len * sizeof(word_t); pos < usize; pos++) {
    buf1[pos] = buf2[pos];
  }
}
#ifndef NMAIN
#define BUFFER_SIZE (1 << 23)
static byte_t buffer1[BUFFER_SIZE];
static byte_t buffer2[BUFFER_SIZE * 2 + 9];
int main(int argc, char *argv[])
{
  fprintf(stderr, "SPDP Floating-Point Compressor v1.0\n");
  fprintf(stderr, "Copyright (c) 2016 Texas State University\n\n");

  if ((argc != 1) && (argc != 2)) {
    fprintf(stderr, "compression usage: %s level < uncompressed_file > compressed_file\n", argv[0]);
    fprintf(stderr, "decompression usage: %s < compressed_file > decompressed_file\n", argv[0]);
    return -1;
  }

  if (argc == 2) {  // compression
    byte_t level = atoi(argv[1]);
    if (level < 0) level = 0;
    if (level > 9) level = 9;
    fwrite(&level, sizeof(byte_t), 1, stdout);

    int length = fread(buffer1, sizeof(byte_t), BUFFER_SIZE, stdin);
    while (length > 0) {
      fwrite(&length, sizeof(int), 1, stdout);
      int csize = compress(level, length, buffer1, buffer2);
      fwrite(&csize, sizeof(int), 1, stdout);
      fwrite(buffer2, sizeof(byte_t), csize, stdout);
      length = fread(buffer1, sizeof(byte_t), BUFFER_SIZE, stdin);
    }
  } else {  // decompression
    byte_t level = 10;
    fread(&level, sizeof(byte_t), 1, stdin);
    if ((level < 0) || (level > 9)) {
      fprintf(stderr, "incorrect input file type\n");
      return -2;
    }

    int length;
    while (fread(&length, sizeof(int), 1, stdin) > 0) {
      int csize;
      fread(&csize, sizeof(int), 1, stdin);
      fread(buffer2, sizeof(byte_t), csize, stdin);
      decompress(level, csize, buffer2, buffer1);
      fwrite(buffer1, sizeof(byte_t), length, stdout);
    }
  }

  return 0;
}
#endif
