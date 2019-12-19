	//-------- External libs ----------   
      #if C_SIMPLE8B  
    case AM_SIMPLE8B: b = bitdienc32( in+1, --n, pa, in[0], mdelta); vbxput32(out, in[0]); if(b>28) die("simple-8b overflow.bits size>28\n");
																return vs8benc(  pa, n, out);
      #endif  
    case P_COPY:                                                return u32enc( in, n, (unsigned *)out);
    case P_MCPY: 	   	memcpy(out, _in, _n); 					return out+_n;
	  #if C_FASTPFOR
    case FP_VBYTE:      bitdienc32( in, n, pa, -mdelta, mdelta);      return vbyteenc( pa, n, (unsigned *)out); 
      #endif

	  #if C_MASKEDVBYTE
    case P_MASKEDVBYTE:                                         return out+vbyte_encode_delta(in, n, out, 0);     
      #endif  
	
      #if C_LIBFOR
    case LF_FORX: //b = bitf32( in+1, --n, in[0]); *(unsigned *)out = in[0]; out+= 4; *out++=b; return out + for_compress_bits( in+1, out, n, in[0], b); //if(b < 0) b = maxbits(in), *out++ = b; return out + for_compress_bits(in, out, n, 0, b);
    case LF_FOR:  return out + for_compress_sorted(in, out, n);
      #endif

	  #if C_POLYCOM
    case PC_VBYTE:          bitdienc32( in,     n, pa, -mdelta, mdelta);return vbpolyenc(pa, n, out);
    case PC_SIMPLE16:   b = bitdienc32( in+1, --n, pa, in[0], mdelta); vbxput32(out, in[0]);                    if(b>28) die("simple16 overflow.bits size>28\n");
															    return vs16enc(  pa, n, (unsigned *)out); 
    case PC_OPTPFD: b = bitdienc32( in+1, --n, pa, in[0], mdelta); vbxput32(out, in[0]);						if(b>28) die("optp4 overflow.bits size>28\n");
																return optpfdenc32(pa, n, out);
      /*b = bitdienc32( in+1, --n, pa, in[0], mdelta); vbxput32(out, in[0]);                                                     if(b>28) die("optp4 overflow.bits size>28\n");
      if(n < 128)                                                                           return vbyteenc(  pa, n, (unsigned *)out);
      else {                                                                                return out + OPT4(pa, n, (unsigned *)out); }*/
      #endif

	  #if C_QMX
    case P_QMX:  { bitdienc32( in+1, --n, pa, in[0], mdelta); vbxput32(out, in[0]); JASS::compress_integer_qmx_improved qmx; if(n) { unsigned r=qmx.encode(out+4, outsize, (uint32_t *)pa, n); ctou32(out) = r; return out+4+r; } return out; }
	  #endif  

      #if C_SIMDCOMP				  
    case SC_FOR:
    case SC_FORDA:  x = *in++; b = bitf32( in, --n, x); vbxput32(out, x); *out++=b; 	   return (unsigned char *)simdpackFOR_length(x, in, n, (__m128i *)out, b);
    case SC_SIMDPACK128:                                                                                   
      if(n < 129) { bitdienc32( in, n, pa, -mdelta, mdelta);                                     return vbyteenc((unsigned *)pa, n, (unsigned *)out); }
      else { b = simdmaxbitsd1(in[0], in+1);               vbxput32(out, in[0]); *out++=b; return simdpackwithoutmaskd1n(in+1, n-1, (unsigned *)out, in[0], b); }   
	  #endif																					  else {                                   simdpackFOR(in[0], (in+1),    (__m128i *)out, b); return out + simdpackFOR_compressedbytes(n, b); } 

	  #if C_STREAMVBYTE
    case P_STREAMVBYTE: 																		return out +  streamvbyte_delta_encode(in, n, out, mdelta);
      #endif  
	  
      #if C_VARINTG8IU
    case P_VARINTG8IU:     bitdienc32( in, n, pa, -mdelta, mdelta);                                  return vintg8enc(pa, n, out);
      #endif

      #if C_VTENC
    case P_VTENC:  { size_t l; vtenc_list_encode_u32(in, n, out+4, outsize, &l); ctou32(out) = l; return l+4; }
      #endif 

      // --------- delta + transpose + lz77 ----------------------------------------------------------------------------------------
      #if C_LZTURBO
    case P_LZT10:{ bitdienc32(in, n, (unsigned *)out, -mdelta, mdelta); tpenc((unsigned char *)out, _n, sbuf,4); struct lzobj lz; lz.srclen = _n; lz.src = sbuf; lz.dst = out; lz.dstlen = _n; lz.level = 0; lz.hbits = 16; return out + lz8c01(&lz); }
    case P_LZT20:{ bitdienc32(in, n, (unsigned *)out, -mdelta, mdelta); tpenc((unsigned char *)out, _n, sbuf,4); struct lzobj lz; lz.srclen = _n; lz.src = sbuf; lz.dst = out; lz.dstlen = _n; lz.level = 0; lz.hbits = 16; return out + lzbc01(&lz); }
    case P_LZT22:{ bitdienc32(in, n, (unsigned *)out, -mdelta, mdelta); tpenc((unsigned char *)out, _n, sbuf,4); struct lzobj lz; lz.srclen = _n; lz.src = sbuf; lz.dst = out; lz.dstlen = _n; lz.level = 2; lz.hbits = 26; return out + lzbc2(&lz);  }
    case P_VSHUF:{ bitdienc32(in, n, (unsigned *)out, -mdelta, mdelta); tpenc((unsigned char *)out, _n, sbuf,4); unsigned char *p = mheenc(sbuf, _n,    MH_BLK, out+4, out+_n+MH_BLK); ctou32(out) = p-(out+4); return p; }
    //case P_VSHUF:{ bitdienc32( in+1, --n, pa, in[0], mdelta); vbxput32(out, in[0]); unsigned char *p=vsenc32(pa, n, sbuf); p = mheenc(sbuf, p-sbuf, MH_BLK, out+4, out+_n+1024); ctou32(out) = p-(out+4); return p; }
      #endif
	  
      #if C_C_BLOSC
    case P_BS_LZ:    
    case P_BS_LZ4:    
    case P_BS_ZLIB: return out + blosc_compress(lev, BLOSC_SHUFFLE, 4/*typesize*/, _n, in, out, _n+BLOSC_MAX_OVERHEAD);
    //case P_BS_ZLIB_1:  return out + blosc_compress(lev, BLOSC_SHUFFLE/*BLOSC_DELTA*/, 4/*typesize*/, _n, in, out, _n+BLOSC_MAX_OVERHEAD);
      #endif

      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_BIT:     bitdienc32(in, n, (unsigned *)out, -mdelta, mdelta); BITSHUFFLE((unsigned char *)out, _n, sbuf,lev); return out + LZ4_compress((char *)sbuf, (char *)out, _n);
        #endif
    case LZ4_  : 	  return out + LZ4_compress((char *)in, (char *)out, _n);
    case LZ4_NIBBLE:{ bitdienc32(in, n, (uint32_t *)out, -mdelta, mdelta); tp4enc((unsigned char *)out, _n, sbuf,lev); return out + LZ4_compress((char *)sbuf, (char *)out, _n); } 
    case LZ4_BYTE:  { bitdienc32(in, n, (uint32_t *)out, -mdelta, mdelta); tpenc( (unsigned char *)out, _n, sbuf,lev); return out + LZ4_compress((char *)sbuf, (char *)out, _n); }
      #endif
	    
      #if C_ZLIB
    case P_ZLIB:
      { bitdienc32(in, n, (unsigned *)out, -mdelta, mdelta); tpenc((unsigned char *)out, _n, sbuf, 4); uLongf outlen = _n; int rc = compress2(out+4, &outlen, sbuf, _n, lev); if(rc != Z_OK) die("zlib compress2 rc=%d\n", rc); *(unsigned *)out = outlen; return out + 4 + outlen; }
      #endif


