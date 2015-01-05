TurboPFor: Fastest Integer Compression [![Build Status](https://travis-ci.org/powturbo/TurboPFor.svg?branch=master)](https://travis-ci.org/powturbo/TurboPFor)
======================================

- 100% C (C++ compatible headers), without inline assembly
<p>
- Fastest **"Variable Byte"** implementation
<p>
- Novel **"Variable Simple"** faster than simple16 and more compact than simple64
<p>
- Scalar **"Bit Packing"** with bulk decoding as fast as SIMD FastPFor in realistic and practical (No "pure cache") scenarios
- Bit Packing with **Direct/Random Access** without decompressing entire blocks
- Access any single bit packed entry with **zero decompression**
- Reducing **Cache Pollution**
<p>
- Novel **"TurboPFor"** (Patched Frame-of-Reference) scheme with direct access or bulk decoding.
  Outstanding compression
<p>
- Several times faster than other libraries
- Usage in C/C++ as easy as memcpy
- Most functions optimized for speed and others for high compression ratio
- **New:** Include more functions
<p>
- Instant access to compressed *frequency* and *position* data in inverted index with zero decompression
- **New:** Inverted Index Demo + Benchmarks: Intersection of lists of sorted integers.
- more than **1000 queries per second** on gov2 (25 millions documents) on a **SINGLE** core.
- Decompress only the minimum necessary blocks.

# Benchmark:
i7-2600k at 3.4GHz, gcc 4.9, ubuntu 14.10.
- Single thread
- Realistic and practical benchmark with large integer arrays.
- No PURE cache benchmark

#### Synthetic data: 
coming soon!

#### data files
 - clueweb09.sorted from FastPFor (http://lemire.me/data/integercompression2014.html)<br />

   *./icbench -c1 -n10000000000 clueweb09.sorted*

<table>
<tr><th>Size</th><th>Ratio in %</th><th>Bits/Integer</th><th>C Time MB/s</th><th>D Time MB/s</th><th>Function</th></tr>
<tr><th> 514438405</th><th>8.16</th><th>2.61</th><th>357.22</th><th>1286.42</th><th>TurboPFor</th></tr>
<tr><th> 514438405</th><th>8.16</th><th>2.61</th><th>358.09</th><th>309.70</th><th>TurboPFor DA</th></tr>
<tr><th> 539841792</th><th>8.56</th><th>2.74</th><th>6.47</th><th>767.35</th><th>OptP4</th></tr>
<tr><th> 583184112</th><th>9.25</th><th>2.96</th><th>132.42</th><th>914.89</th><th>Simple16</th></tr>
<tr><th> 623548565</th><th>9.89</th><th>3.17</th><th>235.32</th><th>925.71</th><th>SimpleV</th></tr>
<tr><th> 733365952</th><th>11.64</th><th>3.72</th><th>162.21</th><th>1312.15</th><th>Simple64</th></tr>
<tr><th> 862464289</th><th>13.68</th><th>4.38</th><th>1274.01</th><th>1980.55</th><th>TurboPack</th></tr>
<tr><th> 862464289</th><th>13.68</th><th>4.38</th><th>1285.28</th><th>868.06</th><th>TurboPack DA</th></tr>
<tr><th> 862465391</th><th>13.68</th><th>4.38</th><th>1402.12</th><th>2075.15</th><th>SIMD-BitPack FPF</th></tr>
<tr><th>6303089028</th><th>100.00</th><th>32.00</th><th>1257.50</th><th>1308.22</th><th>copy</th></tr>
</table>

## Compile:
  make

## Benchmark
###### Synthetic data: 
  1. test all functions<br />

    *./icbench -a1.0 -m0 -x8 -n100000000*

    - zipfian distribution alpha = 1.0 (Ex. -a1.0=uniform -a1.5=skewed distribution)
    - number of integers = 100000000
    - integer range from 0 to 255 (integer size = 0 to 8 bits)
  
  2. individual function test (ex. copy TurboPack TurboPack Direct access)<br />

    *./icbench -a1.0 -m0 -x8 -ecopy/turbopack/turbopackda -n100000000*

###### Data files: 
  - Data file Benchmark (file format as in FastPFOR)

    *./icbench -c1 gov2.sorted*

###### Benchmarking intersections
  - Download "gov2.sorted" (or clueweb09) + query file "aol.txt" 
    from "http://lemire.me/data/integercompression2014.html"

  - Create index file gov2.sorted.i

    *./idxcr gov2.sorted .*

    create inverted index file "gov2.sorted.i" in the current directory

  - Benchmarking intersections

    *./idxqry gov2.sorted.i aol.txt*

    run queries in file "aol.txt" over the index of gov2 file

   8GB Minimum of RAM required (16GB recommended for benchmarking "clueweb09" files).


## Function usage:
In general compression/decompression functions are of the form:

   *char *endptr = compress( unsigned *in, int n, char *out)*<br />
   endptr : set by compress to the next character in "out" after the compressed buffer<br />
   in     : input integer array<br />
   n      : number of elements<br />
   out    : pointer to output buffer<br />
   
   *char *endptr = decompress( char *in, int n, unsigned *out)*<br />
   endptr : set by decompress to the next character in "in" after the decompressed buffer<br />
   in     : pointer to input buffer<br />
   n      : number of elements<br />
   out    : output integer array<br />

header files with documentation :<br />
  vint.h     - variable byte<br />
  vsimple.h  - variable simple<br />
  vp4dc.h,vp4dd.h - TurboPFor<br />
  bitpack.h,bitunpack.h - Bit Packing<br />
  

## Reference:
 - "SIMD-BitPack FPF" from FastPFor https://github.com/lemire/simdcomp
 - Sorted integer datasets from http://lemire.me/data/integercompression2014.html
 - OptP4 and Simple-16 from http://jinruhe.com/ 

#------------------------------------------------

