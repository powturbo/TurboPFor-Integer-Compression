TurboPFor: Fastest Integer Compression [![Build Status](https://travis-ci.org/powturbo/TurboPFor.svg?branch=master)](https://travis-ci.org/powturbo/TurboPFor)
======================================

- 100% C, without inline assembly
<p>
- Fastest **"Variable Byte"** implementation
<p>
- Novel **"Variable Simple"** faster than simple16 and more compact than simple64
<p>
- Scalar **"Binary Packing"** with bulk decoding as fast as SIMD FastPFor in realistic (No "pure cache") scenarios
- Binary Packing with **Direct/Random Access** without decompressing entire blocks
- Access any single binary packed entry with **zero decompression**
<p>
- Novel **"TurboPFor"** (Patched Frame-of-Reference) scheme with direct access or bulk decoding
<p>
- Several times faster than other libraries
- Usage as easy as memcpy
- Instant access to compressed *frequency* and *position* data in inverted index with zero decoding
 
# Benchmark:
i7-2600k at 3.4GHz, gcc 4.9, ubuntu 14.10.
- Single thread
- Realistic and practical benchmark with large integer arrays.
- No PURE cache benchmark

#### Synthetic data: 
coming soon!

#### data files
 - clueweb09.sorted from FastPFor (http://lemire.me/data/integercompression2014.html)<br />
   ./icbench -n10000000000 clueweb09.sorted

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

## Usage
###### Synthetic data: 
  1. test all functions<br \>
    ./icbench -a1.0 -m0 -x8 -n100000000

    - zipfian distribution alpha = 1.0 (Ex. -a1.0=uniform -a1.5=skewed distribution)
    - number of integers = 100000000
    - integer range from 0 to 255 (integer size = 0 to 8 bits)
  
  2. individual function test (ex. copy TurboPack TurboPack Direct access)<br \>
    ./icbench -a1.0 -m0 -x8 -ecopy/turbopack/turbopack,da -n100000000

###### Data files: 
  - Data file Benchmark (file format as in FastPFOR)<br \>
    ./icbench -n10000000000 clueweb09.sorted

## Reference:
 - "SIMD-BitPack FPF" from FastPFor https://github.com/lemire/simdcomp 
 - OptP4 and Simple-16 from http://jinruhe.com/ 

