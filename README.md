TurboPFor: Fastest Integer Compression [![Build Status](https://travis-ci.org/powturbo/TurboPFor.svg?branch=master)](https://travis-ci.org/powturbo/TurboPFor)
======================================
+ **TurboPFor**
 - 100% C (C++ compatible headers), w/o inline assembly
 - No other "Integer Compression" compress or decompress faster with better compression
 - Direct Access is several times faster than other libraries
 - Usage in C/C++ as easy as memcpy
 - :new: Integrated differential encoding/decoding for sorted integer lists
<p>
+ **Features**
 - Fastest **"Variable Byte"** implementation
<p>
 - **Novel** **"Variable Simple"** faster than simple16 and more compact than simple8-b
<p>
 - Scalar **"Bit Packing"** decoding as fast as SIMD-Packing in realistic (No "pure cache") scenarios
 - Bit Packing with **Direct/Random Access** without decompressing entire blocks
 - Access any single bit packed entry with **zero decompression**
 - :new: **Direct Update** of individual bit packed entries
 - Reducing **Cache Pollution**
<p>
 - :new: Fastest and most efficient **"SIMD Bit Packing"**
<p>
 - :new: Fastest **"Elias Fano"** implementation w/ or w/o SIMD.
<p>
 - **Novel** **"TurboPFor"** (Patched Frame-of-Reference) scheme with **direct access** or bulk decoding.<br>
  Outstanding compression and speed. More efficient than **ANY** other "integer compression" scheme.
<p>
+ **Inverted Index ...do less, go fast!**
 - Direct Access to compressed *frequency* and *position* data in inverted index with zero decompression
 - **Novel** **"Intersection w/ skip intervals"**, decompress the minimum necessary blocks (~10-15%). 
 - **Novel** Implicit skips with zero extra overhead
 - **Novel** Efficient Bidirectional Inverted Index Architecture (forward/backwards traversal).
 - more than **2000 queries per second** on GOV2 dataset (25 millions documents) on a **SINGLE** core
 - :new: Parallel Query Processing on Multicores w/ more than **7000 queries/sec** on a quad core PC.<br>
   ...forget ~~Map Reduce, Hadoop, multi-node clusters,~~ ...
   
### Benchmark:
CPU: Sandy bridge i7-2600k at 4.2GHz, gcc 5.1, ubuntu 15.04, single thread.
- Realistic and practical benchmark with large integer arrays.
- No PURE cache benchmark

##### - Synthetic data: 
 - Generate and test skewed distribution (100.000.000 integers, Block size=128).


        ./icbench -a1.5 -m0 -M255 -n100m
	
|Size|  Ratio % |Bits/Integer |C Time MI/s |D Time MI/s |Function |
|--------:|-----:|----:|-------:|-------:|---------|
| 63.392.801| 15.85| 5.07|**388.36**|**1368.25**|**TurboPFor**|
| 63.392.801| 15.85| 5.07|  365.26| 243.15|**TurboPForDA**|
| 65.359.916| 16.34| 5.23|    7.09| 638.96|OptPFD|
| 72.364.024| 18.09| 5.79|   85.31| 762.00|Simple16|
| 78.514.276| 19.63| 6.28|  249.17| 813.41|**SimpleV**|
| 95.915.096| 23.98| 7.67|  221.46|1049.70|Simple-8b|
| 99.910.930| 24.98| 7.99|**2603.47**|**1948.65**|**TurboPackV**|
| 99.910.930| 24.98| 7.99| 2524.50|1943.41|SIMDPackFPF|
| 99.910.930| 24.98| 7.99| 1883.21|1898.11|**TurboPack**|
| 99.910.930| 24.98| 7.99| 1877.25| 935.83|**TurboPackDA**|
|102.074.663| 25.52| 8.17| 1621.64|1694.64|**TurboVbyte**|
|102.074.663| 25.52|8.17|1214.12|1688.95|MaskedVByte|
|102.074.663| 25.52| 8.17| 1178.72| 949.59|VbyteFPF|
|112.500.000| 28.12| 9.00|  305.85|1899.15|VarintG8IU|
|400.000.000|100.00|32.00| 1451.11|1493.46|Copy|
|         |      |     |   N/A  | N/A   |**EliasFano**|
MI/s: 1.000.000 integers/second ( = 4.000.000 bytes/sec )<br>
**#BOLD** = pareto frontier

##### - Data files:
 - gov2.sorted from [DocId data set](http://lemire.me/data/integercompression2014.html) Block size=128 (lz4+SimpleV 64k)


        ./icbench -c1 gov2.sorted
   
|Size |Ratio %|Bits/Integer|C Time MI/s|D Time MI/s|Function |
|----------:|-----:|----:|------:|------:|---------------------|
| 3.214.763.689| 13.44| 4.30| 339.90| 837.69|**SimpleV 64k**|
| 3.337.758.854| 13.95| 4.47|   5.06| 513.00|OptPFD|
| 3.357.673.495| 14.04| 4.49|**357.77**|**1192.14**|**TurboPFor**|
| 3.501.671.314| 14.64| 4.68| 321.45| 813.90|**SimpleV**|
| 3.766.174.764| 15.75| 5.04|**617.88**| 712.31|**EliasFano**|
| 3.820.190.182| 15.97| 5.11| 118.81| 650.21|Simple16|
| 3.958.888.197| 16.55| 5.30| 279.19| 618.60|Lz4 64K|
| 4.521.326.518| 18.90| 6.05| 209.17| 824.26|Simple-8b|
| 4.683.323.301| 19.58| 6.27| 828.25| 991.34|**TurboVbyte**|
| 4.953.768.342| 20.71| 6.63|**1766.05**|**1943.87**|**TurboPackV**|
| 4.953.768.342| 20.71| 6.63|1419.35|1512.86|**TurboPack**|
| 5.203.353.057| 21.75| 6.96|1560.34|1806.60|SIMDPackFPFD1|
| 6.699.519.000| 28.01| 8.96| 472.01| 495.12|VbyteFPF|
| 6.700.989.563| 28.02| 8.96| 728.72| 991.57|MaskedVByte|
| 7.622.896.878| 31.87|10.20| 208.73|1197.74|VarintG8IU|
|23.918.861.764|100.00|32.00|1456.17|1480.78|Copy|

lz4 w/ delta+transpose similar to delta+[blosc](https://github.com/Blosc/c-blosc)

##### - Compressed Inverted Index Intersections with GOV2<br />
   GOV2: 426GB, 25 Millions documents, average doc. size=18k.

   + Aol query log: 18.000 queries<br />
     **~1300** queries per second (single core)<br />
     **~5000** queries per second (quad core)<br />
     Ratio = 14.37% Decoded/Total Integers.

   + TREC Million Query Track (1MQT):<br />
     **~1100** queries per second (Single core)<br /> 
     **~4500** queries per second (Quad core CPU)<br />
     Ratio = 11.59% Decoded/Total Integers.

- Benchmarking intersections (Single core, AOL query log)

| max.docid/q|Time s| q/s | ms/q | % docid found|
|-----------------:|---:|----:|-----:|-------:|
|1.000|7.88|2283.1|0.438|81|
|10.000|10.54|1708.5|0.585|84|
| ALL |13.96|1289.0|0.776|100|
q/s: queries/second, ms/q:milliseconds/query

- Benchmarking Parallel Query Processing (Quad core, AOL query log)

| max.docid/q|Time s| q/s | ms/q | % docids found|
|-----------------:|----:|----:|-----:|-------:|
|1.000|2.66|6772.6|0.148|81|
|10.000|3.39|5307.5|0.188|84|
|ALL|3.57|5036.5|0.199|100|
	 
### Compile:
  *make*

### Testing:
##### - Synthetic data:
  + test all functions<br />


      ./icbench -a1.0 -m0 -M255 -n100m


   >*-zipfian distribution alpha = 1.0 (Ex. -a1.0=uniform -a1.5=skewed distribution)<br />
     -number of integers = 100.000.000<br />
     -integer range from 0 to 255<br />*
  
  + individual function test (ex. Copy TurboPack TurboPFor)<br />


      ./icbench -a1.5 -m0 -M255 -ecopy/turbopack/turbopfor -n100m


##### - Data files:
  - Data file Benchmark (file from [DocId data set](http://lemire.me/data/integercompression2014.html))


    ./icbench -c1 gov2.sorted


##### - Intersections:
  1 - Download Gov2 (or ClueWeb09) + query files (Ex. "1mq.txt") from [DocId data set](http://lemire.me/data/integercompression2014.html)<br />
   8GB RAM required (16GB recommended for benchmarking "clueweb09" files).

  2 - Create index file


      ./idxcr gov2.sorted .


   >*create inverted index file "gov2.sorted.i" in the current directory*

  3 - Test intersections


      ./idxqry gov2.sorted.i 1mq.txt


  >*run queries in file "1mq.txt" over the index of gov2 file*

##### - Parallel Query Processing:
  1 - Create partitions

  
      ./idxseg gov2.sorted . -26m -s8

  
 >*create 8 (CPU hardware threads) partitions for a total of ~26 millions document ids*
  
  2 - Create index file for each partition


      ./idxcr gov2.sorted.s


  >*create inverted index file for all partitions "gov2.sorted.s00 - gov2.sorted.s07" in the current directory*

  3 - Intersections:
  
  delete "idxqry.o" file and then type "make para" to compile "idxqry" w. multithreading


      ./idxqry gov2.sorted.s\*.i 1mq.txt

  >*run queries in file "1mq.txt" over the index of all gov2 partitions "gov2.sorted.s00.i - gov2.sorted.s07.i".*

### Function usage:
In general encoding/decoding functions are of the form:


  >***char *endptr = encode( unsigned *in, unsigned n, char *out, [unsigned start], [int b])**<br />
  endptr : set by encode to the next character in "out" after the encoded buffer<br />
  in     : input integer array<br />
  n      : number of elements<br />
  out    : pointer to output buffer<br />
  b      : number of bits. Only for bit packing functions<br />
  start  : previous value. Only for integrated delta encoding functions*


   
  >**char *endptr = decode( char *in, unsigned n, unsigned *out, [unsigned start], [int b])**<br />
  endptr : set by decode to the next character in "in" after the decoded buffer<br />
  in     : pointer to input buffer<br />
  n      : number of elements<br />
  out    : output integer array<br />
  b      : number of bits. Only for bit unpacking functions<br />
  start  : previous value. Only for integrated delta decoding functions*

header files to use with documentation :<br />

| header file|Functions|
|------|--------------|
|vint.h|variable byte|
|vsimple.h|variable simple|
|vp4dc.h, vp4dd.h|TurboPFor|
|bitpack.h bitunpack.h|Bit Packing|
|eliasfano.h|Elias Fano|

### Environment:
###### OS/Compiler (64 bits):
- Linux: GNU GCC (>=4.6) 
- clang (>=3.2)
- Windows: MinGW-w64 (no parallel query processing)

###### Multithreading:
- All TurboPFor functions are thread safe

### References:

 + [FastPFor](https://github.com/lemire/FastPFor) + [Simdcomp](https://github.com/lemire/simdcomp): SIMDPackFPF, VbyteFPF
 + [Optimized Pfor-delta compression code](http://jinruhe.com): PForDelta: OptPFD or OptP4, Simple16
 + [MaskedVByte](http://maskedvbyte.org/). See also: [Vectorized VByte Decoding](http://engineering.indeed.com/blog/2015/03/vectorized-vbyte-decoding-high-performance-vector-instructions/)
 + [Document identifier data set](http://lemire.me/data/integercompression2014.html)
 + **Publications:**
   - [SIMD Compression and the Intersection of Sorted Integers](http://arxiv.org/abs/1401.6399)
   - [Partitioned Elias-Fano Indexes](http://www.di.unipi.it/~ottavian/files/elias_fano_sigir14.pdf)
