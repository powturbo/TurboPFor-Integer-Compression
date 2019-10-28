# VTEnc

[VTEnc](https://vteromero.github.io/2019/07/28/vtenc.html) is a compression algorithm for sorted lists of unsigned integers. This C library provides an interface to use VTEnc with different data types (8, 16, 32 and 64 bits) and with both lists and sets.

## Results

*This is a work-in-progress section. Last updated on: 27/10/2019*

For reference, VTEnc has been tested on a laptop Ubuntu Desktop 19.04 with a Core i7-6700HQ CPU @ 2.60GHz x 8, using different data sets (see [Tests](https://github.com/vteromero/vtenc#tests) for further information), and comparing it with other integer compression algorithms.

* Timestamps (`ts.txt`):

| Algorithm |Encoded Size|Ratio %|Encoding Speed|Decoding Speed|
|:----------|-----------:|------:|:------------:|:------------:|
| VTEnc     |      21,686| 0.0038| - | - |
| memcpy    | 577,141,992|  100.0| - | - |

* `gov2.sorted`:

| Algorithm | Encoded Size |Ratio %|Encoding Speed|Decoding Speed|
|:----------|-------------:|------:|:------------:|:------------:|
| VTEnc     | 2,889,599,350|  12.08| - | - |
| memcpy    |23,918,861,764|  100.0| - | - |

## API

The documented interface is found in the header file `vtenc.h`.

## Examples

Encoding example:

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vtenc.h"

int main()
{
  const uint8_t in[] = {13, 14, 29, 39, 65, 80, 88, 106, 152, 154, 155, 177};
  const size_t in_len = sizeof(in) / sizeof(in[0]);
  const size_t out_cap = vtenc_list_max_encoded_size_u8(in_len); /* output capacity */
  uint8_t *out=NULL;
  size_t out_len=0;
  VtencErrorCode code;

  /* allocate `out_cap` bytes */
  out = (uint8_t *) malloc(out_cap * sizeof(uint8_t));
  if (out == NULL) {
    fprintf(stderr, "allocation error\n");
    return 1;
  }

  /* encode `in` list into `out` stream of bytes */
  code = vtenc_list_encode_u8(in, in_len, out, out_cap, &out_len);
  if (code != VtencErrorNoError) {
    fprintf(stderr, "failed encoding with error code: %u\n", code);
    free(out);
    return 1;
  }

  /* here `out` holds the encoded output, which is `out_len` bytes long */

  free(out);

  return 0;
}
```

Decoding example:

```c
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vtenc.h"

int main()
{
  const uint8_t in[] = {0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90, 0x26,
    0x24, 0x8d, 0x75, 0xfd, 0x95, 0x83, 0x9b, 0x03};
  const size_t in_len = sizeof(in) / sizeof(in[0]);
  size_t out_len = vtenc_list_decoded_size_u8(in, in_len); /* output length */
  uint8_t *out=NULL;
  VtencErrorCode code;

  /* allocate `out_len` items */
  out = (uint8_t *) malloc(out_len * sizeof(uint8_t));
  if (out == NULL) {
    fprintf(stderr, "allocation error\n");
    return 1;
  }

  /* decode `in` stream of bytes into `out` list */
  code = vtenc_list_decode_u8(in, in_len, out, out_len);
  if (code != VtencErrorNoError) {
    fprintf(stderr, "failed decoding with error code: %u\n", code);
    free(out);
    return 1;
  }

  /* here `out` holds the decoded list of integers, of size `out_len` */

  free(out);

  return 0;
}
```

## Limitations

For now, this library has only been tested on Linux and MacOS systems. It shouldn't be too difficult to support Windows as well and I'm planing to do so in future releases.

## Building

To compile the library, run `make` in root directory. That will generate the static and the shared libraries (`.a` and `.so` files), ready to be included in your own projects.

## Tests

This library is well covered with unit tests and with a few little programs to test different data sets. To build both the programs and the unit tests, run `make test` in the root directory. The following executable files will be created:

* `tests/unit/unit_tests`: invoke it to run all the unit tests.

* `tests/timestamps`: program to test [ts.txt](https://github.com/zentures/encoding/tree/master/benchmark/data), a text file with a big list of timestamps.

* `tests/gov2`: program to test [gov2.sorted](https://lemire.me/data/integercompression2014.html) file, which is part of the "Document identifier data set" created by [D. Lemire](https://lemire.me/en/).
