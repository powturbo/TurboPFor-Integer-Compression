TurboPFor: Fastest Integer Compression [![Build Status](https://travis-ci.org/powturbo/TurboPFor.svg?branch=master)](https://travis-ci.org/powturbo/TurboPFor)
======================================
+ **TurboPFor: The new synonym for "integer compression"**
 - 100% C (C++ compatible headers), w/o inline assembly
 - Usage as simple as memcpy
 - :+1: **Java** Critical Native Interface. Access TurboPFor **incl. SIMD!** from Java as fast as calling from C
 - :sparkles: **FULL** range 16/32/64 bits integer lists and Floating point
 - No other "Integer Compression" compress or decompress faster with better compression
 - Direct Access is several times faster than other libraries
 - :sparkles: Integrated (SIMD) differential/Zigzag encoding/decoding for sorted/unsorted integer lists
 - Compress better and faster than special binary compressors like blosc
<p>
+ **For/PFor/PForDelta**
 - **Novel** **"TurboPFor"** (Patched Frame-of-Reference,PFor/PForDelta) scheme with **direct access** or bulk decoding.
  Outstanding compression and speed. More efficient than **ANY** other fast "integer compression" scheme.
 - Compress 70 times faster and decompress up to 4 times faster than OptPFD
 - :new: **(2017) TurboPFor AVX2, now 50%! more faster!!!!**
 - :new: **(2017) TurboPFor Hybrid, better compression and more faster**
<p>
+ **Bit Packing**
 - :sparkles: Fastest and most efficient **"SIMD Bit Packing"**
 - :new: **(2017) TurboPack AVX2, now more faster. Decoding 10Billions intergers/seconds**
 - Scalar **"Bit Packing"** decoding as fast as SIMD-Packing in realistic (No "pure cache") scenarios
 - Bit Packing with **Direct/Random Access** without decompressing entire blocks
 - Access any single bit packed entry with **zero decompression**
 - :sparkles: **Direct Update** of individual bit packed entries
 - Reducing **Cache Pollution**
<p>
+ **Variable byte**
 - :sparkles: Scalar **"Variable Byte"** faster and more efficient than **ANY** other (incl. SIMD MaskedVByte) implementation
 - :new: **(2017) new scheme w. better compression and 30% more faster**
<p>
+ **Simple family**
 - :sparkles: **Novel** **"Variable Simple"** (incl. **RLE**) faster and more efficient than simple16, simple-8b
   or other "simple family" implementation
<p>
+ **Elias fano**
 - :sparkles: Fastest **"Elias Fano"** implementation w/ or w/o SIMD
<p>
+ **Transform**
 - :sparkles: Scalar & SIMD Transform: Delta, Zigzag, Transpose/Shuffle, Floating point<->Integer
<p>
+ **Inverted Index ...do less, go fast!**
 - Direct Access to compressed *frequency* and *position* data in inverted index with zero decompression
 - :sparkles: **Novel** **"Intersection w/ skip intervals"**, decompress the minimum necessary blocks (~10-15%)!. 
 - **Novel** Implicit skips with zero extra overhead
 - **Novel** Efficient **Bidirectional** Inverted Index Architecture (forward/backwards traversal) incl. "integer compression".
 - more than **2000! queries per second** on GOV2 dataset (25 millions documents) on a **SINGLE** core
 - :sparkles: Revolutionary Parallel Query Processing on Multicores w/ more than **7000!!! queries/sec** on a quad core PC.<br>
   **...forget** ~~Map Reduce, Hadoop, multi-node clusters,~~ ...
   
### Integer Compression Benchmark:
- Realistic and practical "integer compression" benchmark with **large** integer arrays.
- No **PURE** cache benchmark

##### - Synthetic data (2017): 
 - Generate and test (zipfian) skewed distribution (100.000.000 integers, Block size=128/256)<br>
   Note: Unlike general purpose compression, a small fixed size (ex. 128 integers) is in general used in "integer compression". 
   Large blocks involved, while processing queries (inverted index, search engines, databases, graphs, in memory computing,...) need to be entirely decoded


        ./icbench -a1.5 -m0 -M255 -n100m ZIPF
	
CPU: Skylake i7-6700 w/ only 3.7GHz gcc 6.2 single thread


|C Size|ratio%|Bits/Integer|C MI/s|D MI/s|Name|File|
|--------:|-----:|--------:|--------:|----------------|
|62939886| 15.7| 5.04|**392.67**|**2311.32**|**TurboPFor256**|
|63392759| 15.8| 5.07|329.70|1608.42|**TurboPFor**|
|63392801| 15.8| 5.07|326.18|230.97|**TurboPForDA**|
|65060504| 16.3| 5.20|15.77|687.13|[FP.SIMDOptPFor](#FastPFor)|
|65359916|16.34| 5.23| 7.58| 609.12|OptPFD|
|73477088|18.37| 5.88|101.68| 621.37|Simple16|
|73481096| 18.4| 5.88|155.16|2187.15|[FP.SimdFastPFor](#FastPFor)|
|76345136| 19.1| 6.11|245.21|652.78|**VSimple**|
|95915096|23.98| 7.67|  211.79|957.62|Simple-8b|
|99910930| 25.0| 7.99|**3289.58**|**2968.35**|**TurboPackV**|
|99910930| 25.0| 7.99|2122.43|2345.68|**TurboPack**|
|99910930| 25.0| 7.99|2105.47|2218.79|**TurboFor**|
|100332929| 25.1| 8.03|**3580.42**|**2998.17**|**TurboPack256V**|
|101015650| 25.3| 8.08|2380.40|2371.07|**TurboVByte**|
|101879302| 25.5| 8.15|65.16|2140.87|[QMX](#QMX)|
|102074663| 25.5| 8.17|1427.73|1979.27|[MaskedVByte](#MaskedVByte)|
|102074663| 25.5| 8.17|564.60|1052.28|[PC.Vbyte](#PolyCom)|
|102083036| 25.5| 8.17|1300.35|1067.45|[FP.VByte](#FastPFor)|
|112500000| 28.1| 9.00|381.85|**3034.90**|[VarintG8IU](#VarintG8IU)|
|125000000| 31.2|10.00|1110.68|2948.33|[StreamVbyte](#StreamVByte)|
|400000000|	100.00|	32.00| 2240.24|2237.05|Copy|
|         |      |     |   N/A  | N/A   |EliasFano|


MI/s: 1.000.000 integers/second. **1000 MI/s = 4 GB/s**<br> 
**#BOLD** = pareto frontier.<br>
FP=FastPFor SC:simdcomp PC:Polycom<br>
TurboPForDA,TurboForDA: Direct Access is normally used when accessing few individual values.

------------------------------------------------------------------------
##### - Data files (2016):
 - CPU: Sandy bridge i7-2600k at 4.2GHz 
 - gov2.sorted from [DocId data set](#DocId data set) Block size=128 (lz4+blosc+VSimple w/ 64Ki)


        ./icbench -c1 gov2.sorted
   
|Size |Ratio %|Bits/Integer|C Time MI/s|D Time MI/s|Function |
|-----------:|------:|-----:|-------:|-------:|---------------------|
| 3.319.692.190| 13.88| 4.44|**336.68**|**1410.74**|**TurboPFor**| 
| 3.337.758.854| 13.95| 4.47|   5.06| 513.00|OptPFD|
| 3.357.673.495| 14.04| 4.49|**357.77**|**1192.14**|**TurboPFor**|
| 3.501.671.314| 14.64| 4.68| 321.45| 827.01|**VSimple**|
| 3.766.174.764| 15.75| 5.04|**617.88**| 712.31|**EliasFano**|
| 3.820.190.182| 15.97| 5.11| 118.81| 650.21|Simple16|
| 4.521.326.518| 18.90| 6.05| 209.17| 824.26|Simple-8b|
| 4.647.699.724| 19.43| 6.22|**889.02**|1130.50|**TurboVbyte**|
| 4.683.323.301| 19.58| 6.27|**828.97**|1007.44|**TurboVbyte**|
| 4.953.768.342| 20.71| 6.63|**1766.05**|**1943.87**|**TurboPackV**|
| 4.953.768.342| 20.71| 6.63|1419.35|1512.86|**TurboPack**|
| 5.203.353.057| 21.75| 6.96|1560.34|1806.60|SIMDPackD1 FPF|
| 6.221.886.390| 26.01| 8.32|1666.76|1737.72|**TurboFor**|
| 6.221.886.390| 26.01| 8.32|1660.52| 565.25|**TurboForDA**|
| 6.699.519.000| 28.01| 8.96| 472.01| 495.12|Vbyte FPF|
| 6.700.989.563| 28.02| 8.96| 728.72| 991.57|MaskedVByte|
| 7.622.896.878| 31.87|10.20| 208.73|1197.74|VarintG8IU|
| 8.594.342.216| 35.93|11.50|1307.22|1593.07|libfor|
|23.918.861.764|100.00|32.00|1456.17|1480.78|Copy|


|Size |Ratio %|Bits/Integer|C Time MI/s|D Time MI/s|Function |
|----------:|-----:|----:|------:|------:|---------------------|
| 3.214.763.689| 13.44| 4.30| 339.90| 837.69|**VSimple 64Ki**|
| 3.958.888.197| 16.55| 5.30| 279.19| 618.60|[lz4](#lz4)+DT 64Ki|
| 6.074.995.117| 25.40| 8.13| 494.70| 729.97|[blosc_lz4](#blosc) 64Ki| 
| 8.773.150.644| 36.68|11.74| 637.83|1301.05|blosc_lz 64Ki|


Ki=1024 Integers. 64Ki = 256k bytes<br>
"lz4+DT 64Ki" = Delta+Transpose from TurboPFor + lz4<br>
"blosc_lz4" tested w/ lz4 compressor+vectorized shuffle

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

###### Notes:
- Search engines are spending 90% of the time in intersections when processing queries. 
- Most search engines are using pruning strategies, caching popular queries,... to reduce the time for intersections and query processing.
- As indication, google is processing [40.000 Queries per seconds](http://www.internetlivestats.com/google-search-statistics/),
using [900.000 multicore servers](https://www.cloudyn.com/blog/10-facts-didnt-know-server-farms/) for searching [8 billions web pages](http://searchenginewatch.com/sew/study/2063479/coincidentally-googles-index-size-jumps) (320 X size of GOV2).
- Recent "integer compression" GOV2 experiments (best paper at ECIR 2014) [On Inverted Index Compression for Search Engine Efficiency](http://www.dcs.gla.ac.uk/~craigm/publications/catena14compression.pdf) using 8-core Xeon PC are reporting 1.2 seconds per query (for 1.000 Top-k docids).

### Compile:
  *make*
  or
  *make AVX2=1* 

### Testing:
##### - Synthetic data:
  + benchmark "integer compression" functions<br />


        ./icbench -eBENCH -a1.2 -m0 -M255 -n100m ZIPF
        ./icbench -eBENCH/BITPACK/VBYTE -a1.2 -m0 -M255 -n100m ZIPF


   >*-zipfian distribution alpha = 1.2 (Ex. -a1.0=uniform -a1.5=skewed distribution)<br />
     -number of integers = 100.000.000<br />
     -integer range from 0 to 255<br />*
  
  + individual function test (ex. Copy TurboPack TurboPFor)<br />


        ./icbench -a1.5 -m0 -M255 -ecopy/turbopack/turbopfor/turbopack256v ZIPF


##### - Data files:
  - Sorted data file Benchmark (file from [DocId data set](#DocId data set))


        ./icbench -fS gov2.sorted


##### - Intersections:
  1 - Download Gov2 (or ClueWeb09) + query files (Ex. "1mq.txt") from [DocId data set](#DocId data set)<br />
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


      ./idxcr gov2.sorted.s*


  >*create inverted index file for all partitions "gov2.sorted.s00 - gov2.sorted.s07" in the current directory*

  3 - Intersections:
  
  delete "idxqry.o" file and then type "make para" to compile "idxqry" w. multithreading


      ./idxqry gov2.sorted.s*.i 1mq.txt

  >*run queries in file "1mq.txt" over the index of all gov2 partitions "gov2.sorted.s00.i - gov2.sorted.s07.i".*

### Function usage:
See benchmark "icbench" program for "integer compression" usage examples.
In general encoding/decoding functions are of the form:


  >**char *endptr = encode( unsigned *in, unsigned n, char *out, [unsigned start], [int b])**<br />
  endptr : set by encode to the next character in "out" after the encoded buffer<br />
  in     : input integer array<br />
  n      : number of elements<br />
  out    : pointer to output buffer<br />
  b      : number of bits. Only for bit packing functions<br />
  start  : previous value. Only for integrated delta encoding functions


   
  >**char *endptr = decode( char *in, unsigned n, unsigned *out, [unsigned start], [int b])**<br />
  endptr : set by decode to the next character in "in" after the decoded buffer<br />
  in     : pointer to input buffer<br />
  n      : number of elements<br />
  out    : output integer array<br />
  b      : number of bits. Only for bit unpacking functions<br />
  start  : previous value. Only for integrated delta decoding functions

header files to use with documentation:<br />

| header file|Integer Compression functions|
|------------|-----------------------------|
|vint.h|variable byte|
|vsimple.h|variable simple|
|vp4dc.h, vp4dd.h|TurboPFor|
|bitpack.h bitunpack.h|Bit Packing, For, +Direct Access|
|eliasfano.h|Elias Fano|

### Environment:
###### OS/Compiler (64 bits):
- Linux: GNU GCC (>=4.6)
- clang (>=3.2)
- Windows: MinGW-w64 (no parallel query processing)

###### Multithreading:
- All TurboPFor integer compression functions are thread safe

### References:

 + <a name="FastPFor"></a>[FastPFor](https://github.com/lemire/FastPFor) + [Simdcomp](https://github.com/lemire/simdcomp): SIMDPack FPF, Vbyte FPF, VarintG8IU, StreamVbyte
 + <a name="OptPFD"></a><a name="Simple16"></a>[Optimized Pfor-delta compression code](http://jinruhe.com): OptPFD/OptP4, Simple16 (limited to 28 bits integers)
 + <a name="MaskedVByte"></a>[MaskedVByte](http://maskedvbyte.org/). See also: [Vectorized VByte Decoding](http://engineering.indeed.com/blog/2015/03/vectorized-vbyte-decoding-high-performance-vector-instructions/)
 + <a name="Streamvbyte"></a>[Streamvbyte](https://github.com/lemire/streamvbyte). 
 + <a name="Simple-8b"></a>[Index Compression Using 64-Bit Words](http://people.eng.unimelb.edu.au/ammoffat/abstracts/am10spe.html): Simple-8b (speed optimized version tested)
 + <a name="libfor"></a>[libfor](https://github.com/cruppstahl/for)
 + <a name="QMX"></a>[Compression, SIMD, and Postings Lists](http://www.cs.otago.ac.nz/homepages/andrew/papers/) QMX integer compression from the "simple family"
 + <a name="lz4"></a>[lz4](https://github.com/Cyan4973/lz4). included w. block size 64K as indication. Tested after preprocessing w. delta+transpose
 + <a name="blosc"></a>[blosc](https://github.com/Blosc/c-blosc). blosc is like transpose/shuffle+lz77. Tested blosc+lz4 and blosclz incl. vectorizeed shuffle.<br>
 + <a name="DocId data set"></a>[Document identifier data set](http://lemire.me/data/integercompression2014.html)

 + **Integer compression publications:**
   - [In Vacuo and In Situ Evaluation of SIMD Codecs (TurboPackV,TurboPFor/QMX)](http://dl.acm.org/citation.cfm?id=3015023) + [paper](http://www.cs.otago.ac.nz/homepages/andrew/papers/)
   - [SIMD Compression and the Intersection of Sorted Integers](http://arxiv.org/abs/1401.6399)
   - [Partitioned Elias-Fano Indexes](http://www.di.unipi.it/~ottavian/files/elias_fano_sigir14.pdf)
   - [On Inverted Index Compression for Search Engine Efficiency](http://www.dcs.gla.ac.uk/~craigm/publications/catena14compression.pdf)
   - [Google's Group Varint Encoding](http://static.googleusercontent.com/media/research.google.com/de//people/jeff/WSDM09-keynote.pdf)

 + **Applications:**
   - [Graph500](https://github.com/julianromera/graph500)
   - [Small Polygon Compression](http://abhinavjauhri.com/publications/dcc_poster_2016.pdf) + [code](https://github.com/ajauhri/bignum_compression)

Last update:  02 JAN 2017
