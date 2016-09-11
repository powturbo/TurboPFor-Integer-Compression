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
//     ic - "Integer Compression" Java Critical Native Interface Class
 
public class jic {
  // variable byte
  public native static int vbenc32(        int[] in, int n, byte[] out);
  public native static int vbdec32(       byte[] in, int n,  int[] out);

  public native static int vbdenc32(       int[] in, int n, byte[] out, int start);
  public native static int vbddec32(      byte[] in, int n,  int[] out, int start);
  
  public native static int vbd1enc32(      int[] in, int n, byte[] out, int start);
  public native static int vbd1dec32(     byte[] in, int n,  int[] out, int start);

  // variable simple
  public native static int vsenc32(        int[] in, int n, byte[] out);
  public native static int vsdec32(       byte[] in, int n,  int[] out);

  // TurboPFor: PFor/PForDelta
  public native static int p4denc32(       int[] in, int n, byte[] out);
  public native static int p4ddec32(      byte[] in, int n,  int[] out);

  public native static int p4dencv32(      int[] in, int n, byte[] out);
  public native static int p4ddecv32(     byte[] in, int n,  int[] out);

  public native static int p4dddec32(     byte[] in, int n,  int[] out, int start);
  public native static int p4dddecv32(    byte[] in, int n,  int[] out, int start);

  public native static int p4dd1dec32(    byte[] in, int n,  int[] out, int start);
  public native static int p4dd1decv32(   byte[] in, int n,  int[] out, int start);

  // bitpack
  public native static int bitpack32(      int[] in, int n, byte[] out, int b);
  public native static int bitunpack32(   byte[] in, int n,  int[] out, int b);

  public native static int bitdpack32(     int[] in, int n, byte[] out, int start, int b);
  public native static int bitdunpack32(  byte[] in, int n,  int[] out, int start, int b);
  
  public native static int bitd1pack32(    int[] in, int n, byte[] out, int start, int b);
  public native static int bitd1unpack32( byte[] in, int n,  int[] out, int start, int b);

  // bitpack SIMD 
  public native static int bitpackv32(     int[] in, int n, byte[] out, int b);
  public native static int bitunpackv32(  byte[] in, int n,  int[] out, int b);

  public native static int bitdpackv32(    int[] in, int n, byte[] out, int start, int b);
  public native static int bitdunpackv32( byte[] in, int n,  int[] out, int start, int b);
  
  public native static int bitd1packv32(   int[] in, int n, byte[] out, int start, int b);
  public native static int bitd1unpackv32(byte[] in, int n,  int[] out, int start, int b);

  // bitutil
  public native static int bit32(         int[] in, int n);
  public native static int bitd32(        int[] in, int n, int start);
  public native static int bitd132(       int[] in, int n, int start);

  static {
    try {
      //System.loadLibrary("libic.so");
      System.load("/home/x/TurboPFor/libic.so");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("can't load Native TurboPFor from '" + System.getProperty("java.library.path") + "'" + e);
      System.exit(1);
    }
  }
}

