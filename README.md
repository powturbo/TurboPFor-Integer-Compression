## TurboPFor: Fastest Integer Compression</br> 
[![Build ubuntu](https://github.com/powturbo/TurboPFor-Integer-Compression/actions/workflows/build.yaml/badge.svg)](https://github.com/powturbo/TurboPFor-Integer-Compression/actions/workflows/build.yaml)

* **TurboPFor: The synonym for "integer compression"**
  * **ALL** functions available for **AMD/Intel**, **64 bits ARMv8 NEON** Linux+MacOS/M1 & **Power9 Altivec**
  * 100% C (C++ headers), as simple as memcpy. OS:Linux amd64, arm64, Power9, MacOs (Amd/intel + Apple M1),
  * :new:(2023.04) **Rust** Bindings. Access TurboPFor **incl. SSE/AVX2/Neon!** from Rust
  * :+1: **Java** Critical Natives/JNI. Access TurboPFor **incl. SSE/AVX2/Neon!** from Java as fast as calling from C
  * :sparkles: **FULL** range 8/16/32/64 bits scalar + 16/32/64 bits SIMD functions
  * No other "Integer Compression" compress/decompress faster
  * :sparkles: Direct Access, **integrated** (SIMD/AVX2) FOR/delta/Delta of Delta/Zigzag for sorted/unsorted arrays
* **For/PFor/PForDelta**
  * **Novel TurboPFor** (PFor/PForDelta) scheme w./ **direct access** + **SIMD/AVX2**. **+RLE**
  * Outstanding compression/speed. More efficient than **ANY** other fast "integer compression" scheme.
* **Bit Packing**
  * Fastest and most efficient **"SIMD Bit Packing"** **>20 Billions integers/sec (80Gb/s!)**
  * Extremely fast scalar **"Bit Packing"**
  * **Direct/Random Access** : Access any single bit packed entry with **zero decompression**
* **Variable byte**
  * Scalar **"Variable Byte"** faster and more efficient than **ANY** other implementation
  * SIMD **TurboByte** fastest group varint (16+32 bits) incl. integrated delta,zigzag,xor,...
  * :new:(2023.03)**TurboBitByte** novel hybrid scheme combining the fastest SIMD codecs TurboByte+TurboPack.
    Compress considerably better and can be 3 times faster than streamvbyte
* **Simple family**
  * **Novel** **"Variable Simple"** (incl. **RLE**) faster and more efficient than simple16, simple-8b
* **Elias fano**
  * Fastest **"Elias Fano"** implementation w/ or w/o SIMD/AVX2
* :new:(2023.03)**TurboVLC** novel variable length encoding for large integers 
  with exponent + variable bit mantissa 
* :new:(2023.03)**Binary interpolative coding** : fastest implementation
+ **Transform**
  * Scalar & SIMD Transform: Delta, Zigzag, Zigzag of delta, XOR, 
  * :new:(2023.03) Transpose/Shuffle with integrated Xor and zigzag delta
  * :new:(2023.03) 2D/3D/4D transpose
  * **lossy** floating point compression with *TurboPFor* or [TurboTranspose](https://github.com/powturbo/TurboTranspose)+lz77/bwt
+ :new:(2023.03)**IC Codecs** transpose/rle + general purpose compression with lz4,zstd,turborc (range coder),bwt...
* **Floating Point Compression**
  * Delta/Zigzag + improved gorilla style + (Differential) Finite Context Method FCM/DFCM floating point compression
  * Using **TurboPFor**, unsurpassed compression and more than 8 GB/s throughput
  * Point wise relative error bound **lossy** floating point compression
  * **TurboFloat** novel efficient floating point compression using TurboPFor
  * :new:(2023.03)**TurboFloat LzXor** novel floating point lempel-ziv compression
  * :new:(2023.06) _Float16 16 bits floating point support
  * :new:(2023.06) float 16/32/64 bits quantization with variable quantization bit size. 
* **Time Series Compression**
  * **Fastest Gorilla** 16/32/64 bits style compression (**zigzag of delta** + **RLE**).
  * can compress timestamps to only 0.01%. Speed > 10 GB/s compression and > 13 GB/s decompress.
* **Inverted Index ...do less, go fast!**
  * Direct Access to compressed *frequency* and *position* data w/ zero decompression
  * **Novel** **"Intersection w/ skip intervals"**, decompress the minimum necessary blocks (**~10-15%)!**. 
  * **Novel** Implicit skips with zero extra overhead
  * **Novel** Efficient **Bidirectional** Inverted Index Architecture (forward/backwards traversal) incl. "integer compression".
  * more than **2000! queries per second** on GOV2 dataset (25 millions documents) on a **SINGLE** core
  * :sparkles: Revolutionary Parallel Query Processing on Multicores **> 7000!!! queries/sec** on a simple quad core PC.<br>
   **...forget** ~~Map Reduce, Hadoop, multi-node clusters,~~ ...

![Promo video](turbopfor.jpg?raw=true)

### Integer Compression Benchmark (single thread):
- Download [IcApp](https://github.com/powturbo/TurboPFor-Integer-Compression/releases/tag/2023.03) a new benchmark for TurboPFor<br>
  for testing allmost all integer and floating point file types.
  ( type: icapp ZIPF )
- [Benchmark: TurboTranspose+iccodecs vs Quantile Compression](https://github.com/powturbo/TurboPFor-Integer-Compression/issues/100)
- [Benchmark: TurboByte+TurboBitByte vs streamvbtyte](https://github.com/powturbo/TurboPFor-Integer-Compression/issues/96)
- [Benchmark: Time Series - TurboPFor, TurboFloat, TurboFloat LzX, TurboGorilla,...](https://github.com/powturbo/TurboPFor-Integer-Compression/issues/95)
- [Benchmark: Lossy Floating Point Preprocessing Turbo Razor vs Granular bitround vs libroundfast](https://github.com/powturbo/TurboPFor-Integer-Compression/issues/94)
- [Benchmark: Lossless/Lossy Floating Point Compression. TurboPFor vs zfp & blosc](https://github.com/powturbo/TurboPFor-Integer-Compression/issues/92)
- [Benchmark: TurboPFor: IcApp 16 bits Integer Compression](https://github.com/powturbo/TurboPFor-Integer-Compression/issues/50)
- [Benchmark Intel CPU: Skylake i7-6700 3.4GHz gcc 9.2](https://github.com/powturbo/TurboPFor/issues)
- [Benchmark ARM: ARMv8 A73-ODROID-N2 1.8GHz](https://github.com/powturbo/TurboPFor/issues/49)

##### - Synthetic data:
 - Generate and test (zipfian) skewed distribution (100.000.000 integers, Block size=128/256)<br>
   Note: Unlike general purpose compression, a small fixed size (ex. 128 integers) is in general used in "integer compression".
   Large blocks involved, while processing queries (inverted index, search engines, databases, graphs, in memory computing,...) need to be entirely decoded.

        ./icapp -a1.5 -m0 -M255 -n100M ZIPF

|C Size|ratio%|Bits/Integer|C MB/s|D MB/s|Name  2019.11|
|--------:|-----:|--------:|----------:|----------:|--------------|
|62,939,886| 15.7| 5.04|**2369**|**10950**|**TurboPFor256**|
|63,392,759| 15.8| 5.07|1359|7803|**TurboPFor128**|
|63,392,801| 15.8| 5.07|1328|924|**TurboPForDA**|
|65,060,504| 16.3| 5.20|60|2748|[FP_SIMDOptPFor](#FastPFor)|
|65,359,916|16.3| 5.23| 32|2436|PC_OptPFD|
|73,477,088|18.4| 5.88|408|2484|PC_Simple16|
|73,481,096| 18.4| 5.88|624|8748|[FP_SimdFastPFor](#FastPFor) 64Ki *|
|76,345,136| 19.1| 6.11|1072|2878|**VSimple**|
|91,947,533| 23.0| 7.36|284|11737|[QMX](#QMX) 64k *|
|93,285,864| 23.3| 7.46|1568|10232|[FP_GroupSimple](#FastPFor) 64Ki *|
|95,915,096|24.0| 7.67|  848|3832|Simple-8b|
|99,910,930| 25.0| 7.99|**17298**|**12408**|**TurboByte+TurboPack**|
|99,910,930| 25.0| 7.99|**17357**|**12363**|**TurboPackV** sse|
|99,910,930| 25.0| 7.99|11694|10138|**TurboPack** scalar|
|99,910,930| 25.0| 7.99|8420|8876|**TurboFor**|
|100,332,929| 25.1| 8.03|17077|11170|**TurboPack256V** avx2|
|101,015,650| 25.3| 8.08|11191|10333|**TurboVByte**|
|102,074,663| 25.5| 8.17|6689|9524|[MaskedVByte](#MaskedVByte)|
|102,074,663| 25.5| 8.17|2260|4208|[PC_Vbyte](#PolyCom)|
|102,083,036| 25.5| 8.17|5200|4268|[FP_VByte](#FastPFor)|
|112,500,000| 28.1| 9.00|1528|12140|[VarintG8IU](#VarintG8IU)|
|125,000,000| 31.2|10.00|13039|12366|**TurboByte**|
|125,000,000| 31.2|10.00|11197|11984|[StreamVbyte 2019](#StreamVByte)|
|400,000,000|	100.00|	32.00| 8960|8948|Copy|
|         |      |     |   N/A  | N/A   |EliasFano|

(*) codecs inefficient for small block sizes are tested with 64Ki integers/block.

- MB/s: 1.000.000 bytes/second. **1000 MB/s = 1 GB/s**<br> 
- **#BOLD** = pareto frontier.<br>
- FP=FastPFor SC:simdcomp PC:Polycom<br>
- TurboPForDA,TurboForDA: Direct Access is normally used when accessing few individual values.<br>
- Eliasfano can be directly used only for increasing sequences
------------------------------------------------------------------------
##### - Data files:
 - gov2.sorted from [DocId data set](#DocId) Block size=128/Delta coding

 ![Speed/Ratio](lib/ext/gov2.png "Speed/Ratio: Decompression")

|Size |Ratio %|Bits/Integer|C Time MB/s|D Time MB/s|Function 2019.11|
|-----------:|------:|-----:|-------:|-------:|---------------------|
| 3,321,663,893| 13.9| 4.44|**1320**|**6088**|**TurboPFor**| 
| 3,339,730,557| 14.0| 4.47|  32| 2144|PC.OptPFD|
| 3,350,717,959| 14.0| 4.48|**1536**|**7128**|**TurboPFor256**| 
| 3,501,671,314| 14.6| 4.68|  56| 2840|**VSimple**|
| 3,768,146,467| 15.8| 5.04|**3228**| 3652|**EliasFanoV**|
| 3,822,161,885| 16.0| 5.11| 572| 2444|PC_Simple16|
| 4,411,714,936| 18.4| 5.90|**9304**|**10444**|**TurboByte+TurboPack**|
| 4,521,326,518| 18.9| 6.05| 836| 3296|Simple-8b|
| 4,649,671,427| 19.4| 6.22|3084| 3848|**TurboVbyte**|
| 4,955,740,045| 20.7| 6.63|7064|10268|**TurboPackV**|
| 4,955,740,045| 20.7| 6.63|5724| 8020|**TurboPack**|
| 5,205,324,760| 21.8| 6.96|6952| 9488|SC_SIMDPack128|
| 5,393,769,503| 22.5| 7.21|**14466**|**11902**|**TurboPackV256**|
| 6,221,886,390| 26.0| 8.32|6668| 6952|**TurboFor**|
| 6,221,886,390| 26.0| 8.32|6644| 2260|**TurboForDA**|
| 6,699,519,000| 28.0| 8.96|1888| 1980|FP_Vbyte|
| 6,700,989,563| 28.0| 8.96|2740| 3384|MaskedVByte|
| 7,622,896,878| 31.9|10.20| 836| 4792|VarintG8IU|
| 8,060,125,035| 33.7|11.50|8456| 9476|Streamvbyte 2019|
| 8,594,342,216| 35.9|11.50|5228| 6376|libfor|
|23,918,861,764|100.0|32.00|5824| 5924|Copy|

Block size: 64Ki = 256k bytes. Ki=1024 Integers

|Size |Ratio %|Bits/Integer|C Time MB/s|D Time MB/s|Function |
|----------:|-----:|----:|------:|------:|---------------------|
| 3,164,940,562| 13.2|**4.23**|**1344**|**6004**|**TurboPFor 64Ki**|
| 3,273,213,464| 13.7| 4.38|**1496**|**7008**|**TurboPFor256 64Ki**|
| 3,965,982,954| 16.6| 5.30|**1520**| 2452|[lz4](#lz4)+DT 64Ki|
| 4,234,154,427| 17.7| 5.66| 436| 5672|qmx 64Ki| 
| 6,074,995,117| 25.4| 8.13| 1976| 2916|[blosc_lz4](#blosc) 64Ki| 
| 8,773,150,644| 36.7|11.74| 2548|5204|blosc_lz 64Ki|

"lz4+DT 64Ki" = Delta+Transpose from TurboPFor + lz4<br>
"blosc_lz4" internal lz4 compressor+vectorized shuffle

##### - Time Series:
- Test file  [Timestamps: ts.txt(sorted)](https://github.com/zhenjl/encoding/tree/master/benchmark/data)

        ./icapp -Ft ts.txt -I15 -J15

|Function        |C MB/s|   size  |ratio%| D MB/s|Text
|----------------|-----:|--------:|------:|------:|--------------------|
|bvzenc32        |**10632**|45,909|0.008|**12823**|ZigZag|
|bvzzenc32       |**8914**|56,713|0.010|**13499**|ZigZag Delta of delta|
|vsenc32         |**12294**|140,400| 0.024 |12877 |Variable Simple|
|p4nzenc256v32   | 1932| 596,018|  0.10 |13326 |TurboPFor256 ZigZag|
|p4ndenc256v32   | 1961| 596,018|  0.10 |13339 |TurboPFor256 Delta| 
|bitndpack256v32 |**12564**|909,189|  0.16 |13505 |TurboPackV256 Delta|
|p4nzenc32       | 1810|  1,159,633|  0.20 | 8502 |TurboPFor ZigZag|
|p4nzenc128v32   | 1795|  1,159,633|  0.20 |13338 |TurboPFor ZigZag| 
|bitnzpack256v32 | 9651|  1,254,757|  0.22 |**13503**|TurboPackV256 ZigZag| 
|bitnzpack128v32 |10155|  1,472,804|  0.26 |13380 |TurboPackV ZigZag| 
|vbddenc32       | 6198| 18,057,296|  3.13 |10982 |TurboVByte Delta of delta|
|memcpy          |13397|577,141,992|100.00||

##### - Transpose/Shuffle (no compression)
        ./icapp -e117,118,119 ZIPF

|Size |C Time MB/s|D Time MB/s|Function|
|----------:|------:|------:|-----------------------------------|
|100,000,000|**9400**|**9132**|**TPbyte 4** TurboPFor Byte Transpose/shuffle AVX2|
|100,000,000|8784|8860|**TPbyte 4** TurboPFor Byte Transpose/shuffle SSE|
|100,000,000|7688|7656|Blosc_Shuffle AVX2|
|100,000,000|**5204**|**7460**|**TPnibble 4** TurboPFor Nibble Transpose/shuffle SSE|
|100,000,000|6620|6284|Blosc shuffle SSE|
|100,000,000|3156|3372|Bitshuffle AVX2|
|100,000,000|2100|2176|Bitshuffle SSE|

##### - (Lossy) Floating point compression: 
        ./icapp -Fd file          " 64 bits floating point raw file 
        ./icapp -Ff file          " 32 bits floating point raw file 
        ./icapp -Fcf file         " text file with miltiple entries (ex.  8.657,56.8,4.5 ...)
        ./icapp -Ftf file         " text file (1 entry per line)
        ./icapp -Ftf file -v5     " + display the first entries read
        ./icapp -Ftf file.csv -K3 " but 3th column in a csv file (ex. number,Text,456.5 -> 456.5
        ./icapp -Ftf file -g.001  " lossy compression with allowed pointwise relative error 0.001

- see also [TurboTranspose](https://github.com/powturbo/TurboTranspose)

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
- "integer compression" GOV2 experiments [On Inverted Index Compression for Search Engine Efficiency](http://www.dcs.gla.ac.uk/~craigm/publications/catena14compression.pdf) using 8-core Xeon PC are reporting 1.2 seconds per query (for 1.000 Top-k docids).

### Compile:
        Download or clone TurboPFor
		git clone https://github.com/powturbo/TurboPFor-Integer-Compression.git
		cd TurboPFor-Integer-Compression
		make
        
        To benchmark TurboPFor + general purpose compression codecs (zstd,lz4, Turbo-Range-Coder, bwt, bitshuffle):
        git clone --recursive https://github.com/powturbo/TurboPFor-Integer-Compression.git
		cd TurboPFor-Integer-Compression
        make ICCODEC=1

        To benchmark external libraries: 
		Download the external libraries from github into the current directory
		Activate/deactivate the ext. libs in the makefile 
        make CODEC1=1 CODEC2=1 ICCODEC=1


###### Windows visual c++
		nmake /f makefile.vs

###### Windows visual studio c++
        project files under vs/vs2022
  
### Testing:
##### - Synthetic data (use ZIPF parameter):
  + benchmark groups of "integer compression" functions <br />

        ./icapp -a1.2 -m0 -M255 -n100M ZIPF
        ./icapp -a1.2 -m0 -M255 -n100M ZIPF -e20-50
   
   >*-zipfian distribution alpha = 1.2 (Ex. -a1.0=uniform -a1.5=skewed distribution)<br />
     -number of integers = 100.000.000<br />
     -integer range from 0 to 255<br />*
  
  + Unsorted lists: individual function test<br />

        ./icapp -a1.5 -m0 -M255 -e1,2,3 ZIPF

  + Unsorted lists: Zigzag encoding<br />

         ./icapp -e10,11,12 ZIPF

  + Sorted lists: differential coding (increasing/strictly increasing)<br />

        ./icapp -e4,5,6 ZIPF
        ./icapp -e7,8,9 ZIPF
        
  + Transpose/RLE/TurboVByte + General purpose compressor (lz,zstd,turborc,bwt...)<br />

        ./icapp file -e80-95
        ./icapp file -e80-95 -Ezstd,15 
        ./icapp file -e80-95 -Eturborc,1
        ./icapp file -e80-95 -Eturborc,20
        
  + 2D/3D/4D Transpose + General purpose compressor (lz,zstd,turborc,...)<br />
  
        ./icapp file512x128.f32 R512x128 -Ff  
        ./icapp file512x128.f32 -R512x128 -Ff -e100,101,102 
        ./icapp file1024x512x128.f32 -R1024x512x128 -Ff -e100,101,102
        
    Automatic dimension determination from file name ( option -R0 )
    
        ./icapp file1024x512x128.f32 -R0 -Ff -e103,104,105
        ./icapp file1024x512x128.f64 -R0 -Fl -e103,104,105
        
  + Lossy floating point compression
  
        ./icapp file512x128.f32 -R512x128 -Ff -g.0001
        ./icapp file.f32 -Ff -g.001
        ./icapp file.f64 -Fd -g.001


##### - Data files:
  - Raw 32 bits binary data file [Test data](https://github.com/ot/partitioned_elias_fano/tree/master/test/test_data)

        ./icapp file           
        ./icapp -Fs file         "16 bits raw binary file
        ./icapp -Fu file         "32 bits raw binary file
        ./icapp -Fl file         "64 bits raw binary file
        ./icapp -Ff file         "32 bits raw floating point binary file
        ./icapp -Fd file         "64 bits raw floating point binary file

  - Text file: 1 entry per line. [Test data: ts.txt(sorted) and lat.txt(unsorted)](https://github.com/zhenjl/encoding/tree/master/benchmark/data))

        ./icapp -Fts data.txt            "text file, one 16 bits integer per line
        ./icapp -Ftu ts.txt              "text file, one 32 bits integer per line
        ./icapp -Ftl ts.txt              "text file, one 64 bits integer per line
        ./icapp -Ftf file                "text file, one 32 bits floating point (ex. 8.32456) per line
        ./icapp -Ftd file                "text file, one 64 bits floating point (ex. 8.324567789) per line
        ./icapp -Ftd file -v5            "like prev., display the first 100 values read
        ./icapp -Ftd file -v5 -g.00001   "like prev., error bound lossy floating point compression
        ./icapp -Ftt file                "text file, timestamp in seconds iso-8601 -> 32 bits integer (ex. 2018-03-12T04:31:06)
        ./icapp -FtT file                "text file, timestamp in milliseconds iso-8601 -> 64 bits integer (ex. 2018-03-12T04:31:06.345)
        ./icapp -Ftl -D2 -H file         "skip 1th line, convert numbers with 2 decimal digits to 64 bits integers (ex. 456.23 -> 45623)
        ./icapp -Ftl -D2 -H -K3 file.csv  "like prev., use the 3th number in the line (ex. label=3245, text=99 usage=456.23 -> 456.23 )
        ./icapp -Ftl -D2 -H -K3 -k| file.csv "like prev., use '|' as separator

  - Text file: multiple numbers separated by non-digits (0..9,-,.) characters (ex. 134534,-45678,98788,4345, )

        ./icapp -Fc data.txt         "text file, 32 bits integers (ex. 56789,3245,23,678 ) 
        ./icapp -Fcd data.txt        "text file, 64 bits floting-point numbers (ex. 34.7689,5.20,45.789 )

##### - Intersections:
  1 - Download Gov2 (or ClueWeb09) + query files (Ex. "1mq.txt") from [DocId data set](#DocId)<br />
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
See benchmark "icapp" program for "integer compression" usage examples.
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

  **Simple high level functions:** 
  >**size_t compressed_size = encode( unsigned *in, size_t n, char *out)**<br />
  compressed_size : number of bytes written into compressed output buffer out<br />
   
  >**size_t compressed_size = decode( char *in, size_t n, unsigned *out)**<br />
  compressed_size : number of bytes read from compressed input buffer in<br />

### Function syntax:
 - {vb | p4 | bit | vs | v8 | bic }[n][d | d1 | f | fm | z ]{enc/dec | pack/unpack}[| 128v | 256v][8 | 16 | 32 | 64]:<br />
   vb:  variable byte<br />
   p4:  turbopfor<br />
   vs:  variable simple<br />
   v8:  TurboByte SIMD + Hybrid TurboByte + TurboPack<br />
   bit: bit packing<br />
   fp:  Floating Point + Turbo Razor: pointwise relative error rounding algorithm<br />

   n :  high level array functions for large arrays.
	
   ''  : encoding for unsorted integer lists<br />
   'd' : delta encoding for increasing integer lists (sorted w/ duplicate)<br />
   'd1': delta encoding for strictly increasing integer lists (sorted unique)<br />
   'f' : FOR encoding for sorted integer lists<br />
   'z' :  ZigZag encoding for unsorted integer lists<br />
   
   'enc' or 'pack'  : encode or bitpack<br />
   'dec' or 'unpack': decode or bitunpack<br />
   'NN'             : integer size (8/16/32/64)<br />
   
public header file to use with documentation:<br />
        include/ic.h

Note: Some low level functions (like p4enc32) are limited to 128/256 (SSE/AVX2) integers per call.

### Environment:
###### OS/Compiler (64 bits):
- Windows: MinGW-w64 makefile
- Windows: Visual c++ (>=VS2008) - makefile.vs (for nmake)
- Windows: Visual Studio project file - vs/vs2022
- Linux amd64: GNU GCC (>=4.6)
- Linux amd64: Clang (>=3.2) 
- Linux arm64: 64 bits aarch64 ARMv8:  gcc (>=6.3)
- Linux arm64: 64 bits aarch64 ARMv8:  clang
- MaxOS: XCode (>=9)
- MaxOS: Apple M1 (Clang)
- PowerPC ppc64le (incl. SIMD): gcc (>=8.0)

###### Multithreading:
- All TurboPFor integer compression functions are thread safe

#### Knowns issues
- Actually (2023.04) there are no known issues or bugs
- The TurboPFor functions can work with arbitrary inputs
- TurboPFor does normally not read outside the input (encode/decode) buffers
  and does not write outside the output buffer at decoding.
- TurboPFor does not write above a properly sized output buffers at encoding.
  Use the bound (ex. v8bound,p4bound) functions to allocate a max. memory output buffer.   

##  LICENSE
- GPL 2.0
- A commercial license is available. Contact us at powturbo [AT] gmail.com for more information.

### References:

*   [TurboPFor: an analysis](https://michael.stapelberg.ch/posts/2019-02-05-turbopfor-analysis/)

* **Applications:**
  * [Debian Code Search](https://github.com/Debian/dcs/)</br>
    [Debian Code Search: positional index, TurboPFor-compressed](https://michael.stapelberg.ch/posts/2019-09-29-dcs-positional-turbopfor-index/)
  * [Graph500](https://github.com/julianromera/graph500)
  * [Small Polygon Compression](https://arxiv.org/abs/1509.05505) + [Poster](http://abhinavjauhri.me/publications/dcc_poster_2016.pdf) + [code](https://github.com/ajauhri/bignum_compression)
  * [Parallel Graph Analysis (Lecture 18)](http://www.cs.rpi.edu/~slotag/classes/FA16/) + [code](http://www.cs.rpi.edu/~slotag/classes/FA16/handson/lec18-comp2.cpp)

* **Benchmark references:**
  * <a name="FastPFor"></a>[FastPFor](https://github.com/lemire/FastPFor) + [Simdcomp](https://github.com/lemire/simdcomp): SIMDPack FPF, Vbyte FPF, VarintG8IU, StreamVbyte, GroupSimple
  * <a name="OptPFD"></a><a name="Simple16"></a>[Optimized Pfor-delta compression code](http://jinruhe.com): OptPFD/OptP4, Simple16 (limited to 28 bits integers)
  * <a name="MaskedVByte"></a>[MaskedVByte](http://maskedvbyte.org/). See also: [Vectorized VByte Decoding](http://engineering.indeed.com/blog/2015/03/vectorized-vbyte-decoding-high-performance-vector-instructions/)
  * <a name="Streamvbyte"></a>[Streamvbyte](https://github.com/lemire/streamvbyte). 
  * <a name="Simple-8b"></a>[Index Compression Using 64-Bit Words](http://people.eng.unimelb.edu.au/ammoffat/abstracts/am10spe.html): Simple-8b (speed optimized version tested)
  * <a name="libfor"></a>[libfor](https://github.com/cruppstahl/for)
  * <a name="QMX"></a>[Compression, SIMD, and Postings Lists](http://www.cs.otago.ac.nz/homepages/andrew/papers/) QMX integer compression from the "simple family"
  * <a name="lz4"></a>[lz4](https://github.com/Cyan4973/lz4). included w. block size 64K as indication. Tested after preprocessing w. delta+transpose
  * <a name="blosc"></a>[blosc](https://github.com/Blosc/c-blosc). blosc is like transpose/shuffle+lz77. Tested blosc+lz4 and blosclz incl. vectorizeed shuffle.<br>
  * <a name="DocId"></a>[Document identifier data set](http://lemire.me/data/integercompression2014.html)

* **Integer compression publications:**
  * :green_book:[Evaluating Lightweight Integer Compression Algorithms in Column-Oriented In-Memory DBMS](http://www.adms-conf.org/2021-camera-ready/heinzl_adms21.pdf)
  * :green_book:[In Vacuo and In Situ Evaluation of SIMD Codecs (TurboPackV,TurboPFor/QMX)](http://dl.acm.org/citation.cfm?id=3015023) + [paper](http://www.cs.otago.ac.nz/homepages/andrew/papers/)
  * :green_book:[SIMD Compression and the Intersection of Sorted Integers](http://arxiv.org/abs/1401.6399)
  * :green_book:[Partitioned Elias-Fano Indexes](http://www.di.unipi.it/~ottavian/files/elias_fano_sigir14.pdf)
  * :green_book:[On Inverted Index Compression for Search Engine Efficiency](http://www.dcs.gla.ac.uk/~craigm/publications/catena14compression.pdf)
  * :green_book:[Google's Group Varint Encoding](http://static.googleusercontent.com/media/research.google.com/de//people/jeff/WSDM09-keynote.pdf)
  * :green_book:[Integer Compression tweets](https://twitter.com/search?q=%23integercompression&src=typd)
  * :green_book:[Efficient Compression of Scientific Floating-Point Data and An Application in Structural Analysis](https://www.jstage.jst.go.jp/article/jsces/2017/0/2017_20170002/_article)
  * :green_book:[SPDP is a compression/decompression algorithm for binary IEEE 754 32/64 bits floating-point data](http://cs.txstate.edu/~burtscher/research/SPDPcompressor/)<br />
    :green_book:[ SPDP - An Automatically Synthesized Lossless Compression Algorithm for Floating-Point Data](http://cs.txstate.edu/~mb92/papers/dcc18.pdf) + [DCC 2018](http://www.cs.brandeis.edu//~dcc/Programs/Program2018.pdf)

Last update:  10 JUN 2023

