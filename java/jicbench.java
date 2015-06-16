/**
    Copyright (C) powturbo 2013-2015
    GPL v2 License
  
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    - homepage : https://sites.google.com/site/powturbo/
    - github   : https://github.com/powturbo
    - twitter  : https://twitter.com/powturbo
    - email    : powturbo [_AT_] gmail [_DOT_] com
**/
//     icbench - "Integer Compression" Java Critical Native Interface

/* Usage: (actually no makefile available)
1 - generate header jic.h
$ javah -jni jic

2 - Compile jic and jicbench
$ javac jic.java
$ javac jicbench.java

3 - compile & link a shared library
$ gcc -O3 -march=native -fstrict-aliasing -m64 -shared -fPIC -I/usr/lib/jvm/java-7-openjdk-amd64/include bitpack.c bitunpack.c bitpackv.c bitunpackv.c vp4dc.c vp4dd.c vsimple.c vint.c bitutil.c jic.c -o libic.so

4 - copy "libic.so" to java library directory

5 - start icbench
$java icbench
*/

class jicbench {
  // Note: this is a simple interface test not a real benchmark 

  public static void main(String args[]) {
    jic ic = new jic();
    final  int[]  in = new  int[256];
    final byte[] out = new byte[256*5];
    final  int[] cpy = new  int[256];

    for (int i = 0; i < 128; ++i) { 
      in[i] = i;
      cpy[i] = 0;
    }
    long t0 = System.currentTimeMillis();		 
    int b=0, bnum = 125000000; // 16 billions integers. 64 GB
    for (int i = 0; i < bnum; ++i) { 
      //ic.vbenc32(in,  128, out);				 
      //ic.vbennc32(in, 128, out);

      b = ic.bit32(in, 128);
      ic.bitpackv32(  in, 128, out, b);
    }

    long t = System.currentTimeMillis() - t0;    
    System.out.println("encode time'" + t + "'");
    t0 = System.currentTimeMillis();	
    for (int i = 0; i < bnum; ++i) { 
      //ic.vbenc32(out,  128, cpy);				 
      //ic.vsenc32(out, 128, cpy);

      ic.bitunpackv32(out, 128, cpy, b);
    }

    for (int i = 0; i < 128; ++i) { 
      if(in[i] != cpy[i]) {
        System.err.println("Error at'" + i + "'");
        System.exit(1);
      }
    }
    t = System.currentTimeMillis() - t0;
    System.out.println("decode time'" + t + "'");
  }
}
