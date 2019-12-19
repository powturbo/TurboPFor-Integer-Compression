	//-------- Externa libs
    case P_COPY:                                          		return u32dec( (unsigned *)in, n, out);
    case P_MCPY: 	  memcpy(_out, in, _n); 		  	  		return _out+_n;

      #if C_SIMPLE8B  
    case AM_SIMPLE8B: vbxget32(in, x);*out++ = x; --n; in = vs8bdec(in, n, out); bitdidec32(out, n, x+mdelta, mdelta); break;
	  #endif

	  #if C_FASTPFOR
    case FP_VBYTE:    in = vbytedec(  in, n,   out);            bitdidec32(out, n, -mdelta, mdelta); break;
	  #endif
	  
      #if C_LIBFOR
    case LF_FORX:     return for_selectx(in, n, out); //{ out[0] = *(unsigned *)in, b = in[4]; return for_selectx(in+5, n-1, out+1, out[0], b); }
    case LF_FOR:      return in + for_uncompress(in, out, n);
      #endif

      #if C_MASKEDVBYTE
    case P_MASKEDVBYTE: in += masked_vbyte_decode_delta(in, out,   n, 0); break;
      #endif

	  #if C_POLYCOM
    case PC_VBYTE:                              in = vbpolydec(          in, n,   out);          bitdidec32(out, n, -mdelta, mdelta); break;
    case PC_SIMPLE16: vbxget32(in, x); *out = x; in = vs16dec((unsigned *)in, n-1, out+1);      bitdidec32(out, n, -mdelta, mdelta); break;
    case PC_OPTPFD:   vbxget32(in, x); *out = x; in = optpfddec32(in,n-1,out+1); bitdidec32(out+1, n-1, x, mdelta); break;
      /*if(n < 129)                          in = vbytedec(in, n, out);
      else {      _VBGET32(in, x, *out = x); unsigned all_array[2048]; in = (unsigned char *)detailed_p4_decode(out+1, (unsigned *)in, all_array); }
                                                                                            bitdidec32(out, n, -mdelta, mdelta); break;*/
	  #endif
      #if C_QMX    								
    case P_QMX:  { vbxget32(in, x); *out = x; if(n>1) { unsigned l = *(unsigned *)in; JASS::compress_integer_qmx_improved qmx; qmx.decode(out+1, n-1,  in+4, l); bitdidec32(out+1, n-1, x, mdelta); return in+=4+l; } return in; } 
      #endif
 	  
	  #if C_SIMDCOMP
    case SC_SIMDPACK128:
      if(n < 129) { in = vbytedec(in, n, out);               bitdidec32(out, n, -mdelta, mdelta); }
      else {        vbxget32(in, x);*out = x; b = *in++; in = simdunpackd1n((uint32_t *)in, n-1, out+1, out[0], b); } break;
	  
    case SC_FOR:    vbxget32(in, x);*out = x; b = *in++; if(n < 129) return (unsigned char *)simdunpackFOR_length(x, (const __m128i *)in, n-1, (unsigned *)(out+1), b); else { simdunpackFOR(x, (const __m128i *)in, (unsigned *)(out+1), b); return in + simdpackFOR_compressedbytes(n-1, b); }
    case SC_FORDA: { vbxget32(in, x);*out++ = x; --n; b = *in++; unsigned i; for(i=0; i < n;i++) out[i] = simdselectFOR(x, (const __m128i *)in, b, i); return in + simdpackFOR_compressedbytes(n, b); }
	  #endif

	  #if C_STREAMVBYTE
    case P_STREAMVBYTE:   return in + streamvbyte_delta_decode(in, out, n, mdelta);
	  #endif  
	  
      #if C_VARINTG8IU
    case P_VARINTG8IU:                              in = vintg8dec(          in, n,   out);          bitdidec32(out, n, -mdelta, mdelta); break;    
      #endif   

      #if C_VTENC
    case P_VTENC:  vtenc_list_decode_u32(in+4, ctou32(in), out, n); return in + outlen;
      #endif 

      #if C_ZLIB
    case P_ZLIB:
      { uLongf outsize = outlen; int l = *(unsigned *)in, rc = uncompress(sbuf, &outsize, in+4, l); in += 4 + l; tpdec(sbuf, outlen, (unsigned char *)out,4); bitdidec32(out, n, -mdelta, mdelta); } break;
      #endif

      #if C_C_BLOSC
    case P_BS_LZ:
    case P_BS_LZ4:
    case P_BS_ZLIB: { blosc_decompress(in, out, outlen); size_t nbytes, cbytes,blocksize; blosc_cbuffer_sizes(in, &nbytes, &cbytes, &blocksize); return in+cbytes; }
      #endif

      #if C_LZTURBO
    case P_LZT10: { struct lzobj lz; lz.dstlen = outlen; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lz8d(&lz);  tpdec(sbuf, outlen, (unsigned char *)out,4); bitdidec32(out, n, -mdelta, mdelta); } break;
    case P_LZT20:
    case P_LZT22: { struct lzobj lz; lz.dstlen = outlen; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzbd(&lz);  tpdec(sbuf, outlen, (unsigned char *)out,4); bitdidec32(out, n, -mdelta, mdelta); } break;
    //case P_VSHUF: vbxget32(in, x);*out = x; in += ansdecompress( in, (n-1)*4, sbuf); vsdec32( sbuf, n-1, out+1); bitdidec32(out, n, -mdelta, mdelta); break;

    case P_VSHUF:  { unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, outlen); tpdec(sbuf, outlen, (unsigned char *)out,4); bitdidec32(out, n, -mdelta, mdelta); } break;
    //case P_VSHUF:  { vbxget32(in, x);*out = x; unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, outlen); vsdec32(sbuf, n-1, out+1); bitdidec32(out+1, n-1, x, mdelta); } break;
      #endif
                     //---------- delta + transpose + lz77 ----------------------
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_BIT:   in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); BITUNSHUFFLE(sbuf, outlen, (unsigned char *)out,lev); bitdidec32(out, n, -mdelta, mdelta); break;       
    case LZ4_BITX:  in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); BITUNSHUFFLE(sbuf, outlen, (unsigned char *)out,lev); bitxdec32(out, n, mdelta); break;       
        #endif     
    case LZ4_  : 	in += LZ4_decompress_fast((char *)in, (char *)out,  outlen); break;       
    case LZ4_NIBBLE:in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); tp4dec(sbuf, outlen, (unsigned char *)out,lev); bitdidec32(out, n, -mdelta, mdelta); break;       
    case LZ4_NIBBLEX:in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); tp4dec(sbuf, outlen, (unsigned char *)out,lev); bitxdec32(out, n, mdelta); break;       
    case LZ4_BYTE:  in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); tpdec( sbuf, outlen, (unsigned char *)out,lev); bitdidec32(out, n, -mdelta, mdelta); break;       
      #endif     


