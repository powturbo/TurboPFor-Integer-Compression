TurboPFor: Fastest Integer Compression [![Build Status](https://travis-ci.org/powturbo/TurboPFor.svg?branch=master)](https://travis-ci.org/powturbo/TurboPFor)
======================================

- 100% C (C++ compatible headers), without inline assembly
<p>
- Fastest **"Variable Byte"** implementation
<p>
- Novel **"Variable Simple"** faster than simple16 and more compact than simple8-b
<p>
- Scalar **"Bit Packing"** with bulk decoding as fast as SIMD FastPFor in realistic and practical (No "pure cache") scenarios
- Bit Packing with **Direct/Random Access** without decompressing entire blocks
- Access any single bit packed entry with **zero decompression**
- **New:** **Direct Update** of individual bit packed entries
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
i7-2600k at 3.4GHz, gcc 4.9, ubuntu 14.10, Single thread.
- Realistic and practical benchmark with large integer arrays.
- No PURE cache benchmark

#### Synthetic data: 
 - Generate and test skewed distribution.

    *./icbench -a1.5 -m0 -M8 -n100000000*

<table>
<tr><th>Size</th><th>Ratio in %</th><th>Bits/Integer</th><th>C Time MB/s</th><th>D Time MB/s</th><th>Function</th></tr>
<tr><th> 63392801</th><th>15.85</th><th> 5.07</th><th>  316.96</th><th>  893.67</th><th>TurboPFor</th></tr>
<tr><th> 63392801</th><th>15.85</th><th> 5.07</th><th>  315.59</th><th>  227.15</th><th>TurboPForDA</th></tr>
<tr><th> 65359916</th><th>16.34</th><th> 5.23</th><th>    7.09</th><th>  638.96</th><th>OptPFD</th></tr>
<tr><th> 72364024</th><th>18.09</th><th> 5.79</th><th>   85.31</th><th>  762.00</th><th>Simple16</th></tr>
<tr><th> 78514276</th><th>19.63</th><th> 6.28</th><th>  229.21</th><th>  748.32</th><th>SimpleV</th></tr>
<tr><th> 95915096</th><th>23.98</th><th> 7.67</th><th>  221.46</th><th> 1049.70</th><th>Simple-8b</th></tr>
<tr><th> 99910930</th><th>24.98</th><th> 7.99</th><th> 1553.92</th><th> 1904.21</th><th>SIMDPackFPF</th></tr>
<tr><th> 99910930</th><th>24.98</th><th> 7.99</th><th>  953.29</th><th> 1872.02</th><th>TurboPack</th></tr>
<tr><th> 99910930</th><th>24.98</th><th> 7.99</th><th>  953.13</th><th>  869.84</th><th>TurboPackDA</th></tr>
<tr><th>102074663</th><th>25.52</th><th> 8.17</th><th> 1131.47</th><th> 1184.68</th><th>TurboVbyte</th></tr>
<tr><th>102074663</th><th>25.52</th><th> 8.17</th><th> 1110.75</th><th>  897.86</th><th>VbyteFPF</th></tr>
<tr><th>112500000</th><th>28.12</th><th> 9.00</th><th>  305.85</th><th> 1899.15</th><th>VarintG8IU</th></tr>
<tr><th>400000000</th><th>100.00</th><th>32.00</th><th> 1470.87</th><th> 1477.93</th><th>Copy</th></tr>
</table>

#### data files
 - gov2.sorted (from http://lemire.me/data/integercompression2014.html) Blocksize=128<br />
    (+ SimpleV 64k). Benchmark repeated several times.

   *./icbench -c1 gov2.sorted*
   
<table>
<tr><th>Size</th><th>Ratio in %</th><th>Bits/Integer</th><th>C Time MB/s</th><th>D Time MB/s</th><th>Function</th></tr>
<tr><th> 3214763689</th><th>13.44</th><th>4.30</th><th>279.93</th><th> 665.41</th><th>SimpleV 64k</th></tr>
<tr><th> 3337758854</th><th>13.95</th><th>4.47</th><th>5.06</th><th> 513.00</th><th>OptPFD</th></tr>
<tr><th> 3357673495</th><th>14.04</th><th>4.49</th><th>270.57</th><th> 813.83</th><th>TurboPFor</th></tr>
<tr><th> 3501671314</th><th>14.64</th><th>4.68</th><th>258.56</th><th> 720.76</th><th>SimpleV</th></tr>
<tr><th> 3820190182</th><th>15.97</th><th>5.11</th><th>118.81</th><th> 650.21</th><th>Simple16</th></tr>
<tr><th> 4521326518</th><th>18.90</th><th>6.05</th><th>209.17</th><th> 824.26</th><th>Simple-8b</th></tr>
<tr><th> 4953768342</th><th>20.71</th><th>6.63</th><th>647.75</th><th>1501.24</th><th>TurboPack</th></tr>
<tr><th> 5203353057</th><th>21.75</th><th>6.96</th><th>1560.34</th><th>1806.60</th><th>SIMDPackFPFD1</th></tr>
<tr><th> 6699519000</th><th>28.01</th><th>8.96</th><th>502.86</th><th> 624.12</th><th>TurboVbyte</th></tr>
<tr><th> 6699519000</th><th>28.01</th><th>8.96</th><th>472.01</th><th> 495.12</th><th>VbyteFPF</th></tr>
<tr><th> 7622896878</th><th>31.87</th><th>10.20</th><th>208.73</th><th>1197.74</th><th>VarintG8IU</th></tr>
<tr><th>23918861764</th><th>100.00</th><th>32.00</th><th>1391.82</th><th>1420.03</th><th>Copy</th></tr>
</table>

## Compile:
  *make*

## Testing
###### Synthetic data: 
  1. test all functions<br />

    *./icbench -a1.0 -m0 -M8 -n100000000*

    - zipfian distribution alpha = 1.0 (Ex. -a1.0=uniform -a1.5=skewed distribution)
    - number of integers = 100000000
    - integer range from 0 to 255 (integer size = 0 to 8 bits)
  
  2. individual function test (ex. copy TurboPack TurboPack Direct access)<br />

    *./icbench -a1.5 -m0 -M8 -ecopy/turbopack/turbopackda -n100000000*

###### Data files: 
  - Data file Benchmark (file format as in FastPFOR)

    *./icbench -c1 gov2.sorted*

###### Benchmarking intersections
  - Download gov2 (or clueweb09) + query file "aol.txt" 
    from "http://lemire.me/data/integercompression2014.html"

  - Create index file

    *./idxcr gov2.sorted .*

    create inverted index file "gov2.sorted.i" in the current directory

  - Benchmarking intersections

    *./idxqry gov2.sorted.i aol.txt*

    run queries in file "aol.txt" over the index of gov2 file

   8GB RAM required (16GB recommended for benchmarking "clueweb09" files).


## Function usage:
In general compression/decompression functions are of the form:

   **char *endptr = compress( unsigned *in, int n, [int b,], char *out)**<br />
   endptr : set by compress to the next character in "out" after the compressed buffer<br />
   in     : input integer array<br />
   n      : number of elements<br />
   out    : pointer to output buffer<br />
   b      : number of bits. Only for bit packing functions

   **char *endptr = decompress( char *in, int n, [int b,] unsigned *out)**<br />
   endptr : set by decompress to the next character in "in" after the decompressed buffer<br />
   in     : pointer to input buffer<br />
   n      : number of elements<br />
   out    : output integer array<br />
   b      : number of bits. Only for bit unpacking functions

header files to use with documentation :<br />
<table>
<tr><th>vint.h</th><th>Variable byte</th></tr>
<tr><th>vsimple.h</th><th>Variable simple</th></tr>
<tr><th>vp4dc.h, vp4dd.h</th><th>TurboPFor</th></tr>
<tr><th>bitpack.h bitunpack.h</th><th>Bit Packing</th></tr>
</table>
  

## Reference:
 - "SIMD-BitPack FPF" from FastPFor https://github.com/lemire/simdcomp
 - Sorted integer datasets from http://lemire.me/data/integercompression2014.html
 - OptPFD (OptP4) and Simple-16 from http://jinruhe.com/ 

#------------------------------------------------
