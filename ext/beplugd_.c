    //---- external libs -------------
    case P_COPY:       					 		return u32dec(   (unsigned *)in, n, out);
    case P_MCPY:       					 		memcpy(_out, in, _n); return in+_n;	
	
      #if C_SIMPLE8B  
    case AM_SIMPLE8B:					 		return vs8bdec(  in, n, out); 
      #endif
	  
      #if C_FASTPFOR128
	case FP_FASTPFOR: { 	
	  size_t nvalue = n;
      FastPForLib::FastPFor<4> ic; const uint32_t *ip = ic.decodeArray((const uint32_t *)(in+4), ctou32(in), out, nvalue);
      if(n & 127) { 
        nvalue = n - nvalue;
	    FastPForLib::VariableByte vc;
		return (unsigned char *)vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
      }
      return (unsigned char *)ip; 
    }

	case FP_SIMDFASTPFOR:  {
	  size_t nvalue = n;
      FastPForLib::SIMDFastPFor<4> ic; const uint32_t *ip = ic.decodeArray((const uint32_t *)(in+4), ctou32(in), out, nvalue);
      if(n & 127) { 
        nvalue = n - nvalue;
	    FastPForLib::VariableByte vc;
		return (unsigned char *)vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
      }
      return (unsigned char *)ip; 
    }
	case FP_SIMDOPTPFOR:  { 	
	  size_t nvalue = n;
      FastPForLib::SIMDOPTPFor<4> ic; const uint32_t *ip = ic.decodeArray((const uint32_t *)(in+4), ctou32(in), out, nvalue);
      if(n & 127) { 
        nvalue = n - nvalue;
	    FastPForLib::VariableByte vc;
		return (unsigned char *)vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
      }
      return (unsigned char *)ip; 
    } 
	case FP_GROUPSIMPLE:  { 	
	  size_t nvalue = n;
      FastPForLib::SIMDGroupSimple<false,false> ic; const uint32_t *ip = ic.decodeArray((const uint32_t *)(in+4), ctou32(in), out, nvalue);
      if(n & 127) { 
        nvalue = n - nvalue;
	    FastPForLib::VariableByte vc;
		return (unsigned char *)vc.decodeArray(ip, (const uint32_t *)in+1+ctou32(in) - ip, out + (n&(~127)), nvalue);	  //return vbdec32((unsigned char *)ip, n & 127, out + mynvalue1);
      }
      return (unsigned char *)ip; 
    }

    case FP_VBYTE:  //return vbytedec( in, n, out); 
	  { size_t nvalue=n; FastPForLib::VariableByte ic;       return (unsigned char *)ic.decodeArray((const uint32_t *)(in+4), ctou32(in), (uint32_t *)out, nvalue); }
    case FP_SIMPLE8BRLE:  
	  { size_t nvalue=n; FastPForLib::Simple8b_RLE<true> ic; ic.decodeArray((const uint32_t *)(in+4), ctou32(in), (uint32_t *)out, nvalue); return in+4+ctou32(in)*4; }
	  #endif
	
      #if C_LIBFOR
    case LF_FORX: 		 return for_selectx(in, n, out); //if(b < 0) b = *in++; return for_selectx(in, n, out, 0, b);//
    case LF_FOR: 		 return in + for_uncompress(in, out, n); //if(b < 0) b = *in++; return in + for_uncompress_bits(in, out, n, 0, b); //return in + for_uncompress(in, out, n);
      #endif

	  #if C_LITTLEPACK
	case LI_PACK:    	 if(b < 0) b = *in++; unpack32(          in, n, b, out); return in + byte_count(n,b);
	case LI_TURBOPACK:   if(b < 0) b = *in++; turbounpack32(     in, n, b, out); return in + byte_count(n,b);
	case LI_SCPACK:   	 if(b < 0) b = *in++; scunpack32(        in, n, b, out); return in + byte_count(n,b);
	case LI_HORPACK:   	 if(b < 0) b = *in++; horizontalunpack32(in, n, b, out); return in + byte_count(n,b);
        #ifdef __AVX2__
	case LI_BMIPACK:   	 if(b < 0) b = *in++; bmiunpack32(       in, n, b, out); return in + byte_count(n,b);
        #endif
      #endif

      #if C_MASKEDVBYTE
    case P_MASKEDVBYTE:  return in + masked_vbyte_decode(in, out, n);
      #endif
	  
      #if C_POLYCOM
    case PC_VBYTE:    	 return vbpolydec(in, n, out);
    case PC_SIMPLE16: 	 return vs16dec(  (unsigned *)in, n, out);  
    case PC_RICE:     	 return rcdec32(  (unsigned *)in, n, out);
    case PC_OPTPFD : 	 return optpfddec32(   in, n, out); //if(n < 128) return vbytedec(in, n, out); else { unsigned all_array[2048]; return (unsigned char *)detailed_p4_decode(out, (unsigned *)in, all_array); }
     #endif

      #if C_SIMDCOMP128 
	//case SC_PACK:          if(b < 0) b = *in++; return fastunpack32((uint32_t *)in, n, out, b); 
    case SC_FOR:
    case SC_SIMDPACK128: if(b < 0) b = *in++; return (unsigned char *)simdunpack_length( (__m128i *)in, n, out, b); 
    case SC_FORDA:  	 if(b < 0) b = *in++; return simdfor_selectx(in, n, out, 0, b);            
      #endif
      #if C_SIMDCOMP256 
    case SC_SIMDPACK256: if(b < 0) b = *in++; return (unsigned char *)avxunpackn( (unsigned *)in, n, out, b); 
      #endif

	  #if C_STREAMVBYTE 
    case P_STREAMVBYTE:  return in +  streamvbyte_decode(in, out, n); 
      #endif 
      #if C_QMX    										
	case P_QMX:  { JASS::compress_integer_qmx_improved qmx; qmx.decode(out, n,  in+4, ctou32(in)); return in+4+ctou32(in); }
      #endif

      #if C_VARINTG8IU
    case P_VARINTG8IU:    return vintg8dec(in, n, out);
      #endif

      #if C_VTENC
    case P_VTENC:  vtenc_list_decode_u32(in+4, ctou32(in), out,n); return in + outlen;
      #endif 

      #if C_BITSHUFFLE      
    case P_BITSHUFFLE:BITUNSHUFFLE(in, outlen, (unsigned char *)out,lev); 			   return in + outlen;
      #endif
      #if C_C_BLOSC
    case P_BS_SHUFFLE:unshuffle( lev, outlen, (unsigned char *)in, (unsigned char *)out);return in + outlen;
      #endif		
 
      //---------- transpose + lz77 ----------------------
      #if C_LZTURBO
    case P_LZT10:  	{ struct lzobj lz; lz.dstlen = outlen; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lz8d(&lz); tpdec(sbuf, outlen, (unsigned char *)out,4); } break;
    case P_LZT20: 
    case P_LZT22:  	{ struct lzobj lz; lz.dstlen = outlen; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzbd(&lz); tpdec(sbuf, outlen, (unsigned char *)out,4); } break;
    case P_LZT32:  	{ struct lzobj lz; lz.dstlen = outlen; lz.src = in; lz.dst = sbuf; lz.level = 0; in += lzhd(&lz); tpdec(sbuf, outlen, (unsigned char *)out,4); } break;
    //case P_VSHUF: { unsigned inlen = ctou16(in); printf("l=%d ", inlen); in+=2; in = mhdec(in, inlen, MH_BLK, sbuf, outlen); vsdec32(sbuf, n, out); } break;//      in = ansdecompress( in, outlen, sbuf); 
    //case P_VSHUF: { unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, out, outlen); } break;//      in = ansdecompress( in, outlen, sbuf); 
    case P_VSHUF:  	{ unsigned inlen = ctou32(in); in+=4; in = mhdec(in, inlen, MH_BLK, sbuf, outlen); tpdec(sbuf, outlen, (unsigned char *)out,4); /*vsdec32(sbuf, n, out);*/ } break;//      in = ansdecompress( in, outlen, sbuf); 
      #endif
	  
    //  #if C_BITSHUFFLE      
    //case TB_VSIMPLE_S1:     	in = vsdec32(in, n, (unsigned *)sbuf);  BITUNSHUFFLE(sbuf, outlen, (unsigned char *)out,4); break;
    //  #endif
    //case TB_VSIMPLE_S4:  in = vsdec32(in, n, (unsigned *)sbuf); tp4dec(sbuf, _n, (unsigned char *)out,4); break;
    //case TB_VSIMPLE_S8:  in = vsdec32(in, n, (unsigned *)sbuf); tpdec( sbuf, _n, (unsigned char *)out,4); break;
      
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_BIT:    in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); BITUNSHUFFLE(sbuf, outlen, (unsigned char *)out,lev); break;
    case LZ4_BITX:   in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); BITUNSHUFFLE(sbuf, outlen, (unsigned char *)out,lev); bitxdec32(out, n, 0); break;       
        #endif     
    case LZ4_  : 	 in += LZ4_decompress_fast((char *)in, (char *)out,  outlen); break;       
    //case LZ4_NIBBLE: in += LZ4_decompress_fast((char *)in, (char *)out, outlen); tp4dec((unsigned char *)out, outlen, (unsigned char *)sbuf,lev); lodec64((uint64_t *)sbuf, (outlen+7)/8, 3, (uint64_t *)out); break;
    case LZ4_NIBBLE: in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); tp4dec(sbuf, outlen, (unsigned char *)out,lev); break;
    case LZ4_NIBBLEX:in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); tp4dec(sbuf, outlen, (unsigned char *)out,lev); bitxdec32(out, n, 0); break;       
    case LZ4_BYTE:   in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); tpdec( sbuf, outlen, (unsigned char *)out,lev); break; 
    //case LZ4_FP8:    in += LZ4_decompress_fast((char *)in, (char *)sbuf, outlen); fpzdec64(sbuf, sbuf+outlen*2, outlen, (unsigned char *)out); break; 
      #endif
	  
      #if C_C_BLOSC
    case P_BS_LZ:    
    case P_BS_LZ4:    
    case P_BS_ZLIB: { blosc_decompress(in, out, outlen); size_t nbytes, cbytes,blocksize; blosc_cbuffer_sizes(in, &nbytes, &cbytes, &blocksize); return in+cbytes; }
      #endif
	  
      #if C_ZLIB
    case P_ZLIB:
      { uLongf outsize = outlen; int l = *(unsigned *)in, rc = uncompress(sbuf, &outsize, in+4, l); in += 4 + l; tpdec(sbuf, outlen, (unsigned char *)out,4); } break;
      #endif

