    //----- External libs ----------
	  #if C_SIMPLE8B  
    case AM_SIMPLE8B:return vs8benc(  in, n, out);
      #endif
	  
	  #if C_FASTPFOR128
	case FP_FASTPFOR: {   
      size_t nvalue = outsize/4;
      FastPForLib::FastPFor<4> ic; ic.encodeArray((const uint32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
      if(n & 127) {
        size_t nvalue2 = outsize/4 - nvalue;
        FastPForLib::VariableByte vc; vc.encodeArray((const uint32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
        nvalue += nvalue2;
      }
	  ctou32(out) = nvalue;
	  return out+4+nvalue*4;
    }
	case FP_SIMDFASTPFOR: {   
      size_t nvalue = outsize/4;
      FastPForLib::SIMDFastPFor<4> ic; ic.encodeArray((const uint32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
      if(n & 127) {
        size_t nvalue2 = outsize/4 - nvalue;
        FastPForLib::VariableByte vc; vc.encodeArray((const uint32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
        nvalue += nvalue2;
      }
	  ctou32(out) = nvalue;
	  return out+4+nvalue*4;
    }
	case FP_SIMDOPTPFOR: {   
      size_t nvalue = outsize/4;
      FastPForLib::SIMDOPTPFor<4> ic; ic.encodeArray((const uint32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
      if(n & 127) {
        size_t nvalue2 = outsize/4 - nvalue;
        FastPForLib::VariableByte vc; vc.encodeArray((const uint32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
        nvalue += nvalue2;
      }
	  ctou32(out) = nvalue;
	  return out+4+nvalue*4;
    }
	case FP_GROUPSIMPLE: {   
      size_t nvalue = outsize/4;
      FastPForLib::SIMDGroupSimple<false,false> ic; ic.encodeArray((const uint32_t *)in, n & (~127), (uint32_t *)(out+4), nvalue);
      if(n & 127) {
        size_t nvalue2 = outsize/4 - nvalue;
        FastPForLib::VariableByte vc; vc.encodeArray((const uint32_t *)(in + (n & (~127))), n & 127, (uint32_t *)(out + 4 + nvalue*4), nvalue2);
        nvalue += nvalue2;
      }
	  ctou32(out) = nvalue;
	  return out+4+nvalue*4;
    }
    case FP_VBYTE:    	{ size_t nvalue=outsize/4; FastPForLib::VariableByte ic; ic.encodeArray((const uint32_t *)in, (const size_t)n, (uint32_t *)(out+4), nvalue); ctou32(out)=nvalue; return out+4+nvalue*4; }
    case FP_SIMPLE8BRLE:{ size_t nvalue=outsize/4; FastPForLib::Simple8b_RLE<true> ic; ic.encodeArray((const uint32_t *)in, (const size_t)n, (uint32_t *)(out+4), nvalue); ctou32(out)=nvalue; return out+4+nvalue*4; }
	  #endif 
	  
      #if C_LIBFOR
    case LF_FORX: 		//if(b < 0) b = maxbits(in), *out++ = b; return out + for_compress_bits(in, out, n, 0, b);
    case LF_FOR:  		return out + for_compress_unsorted(in, out, n);
      #endif
	  
	  #if C_LITTLEPACK
    case LI_PACK:     	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } pack32(     in, n, b, out); return out + byte_count(n,b);
    case LI_HORPACK:   	
    case LI_TURBOPACK:	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } turbopack32(in, n, b, out); return out + byte_count(n,b);                                                                                                                                
    case LI_SCPACK:   	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } scpack32(   in, n, b, out); return out + byte_count(n,b);                                                                                                                                
        #ifdef __AVX2__
    case LI_BMIPACK:  	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } bmipack32(  in, n, b, out); return out + byte_count(n,b);                                                                                                                                
        #endif
      #endif

      #if C_MASKEDVBYTE
    case P_MASKEDVBYTE: return out + vbyte_encode(in, n, out);
      #endif
	  
	  #if C_POLYCOM
    case PC_VBYTE:      return vbpolyenc(in, n, out);
    case PC_SIMPLE16:   return vs16enc(  in, n, (unsigned *)out); 
    case PC_RICE:       return rcenc32(  in, n, (unsigned *)out); 
    case PC_OPTPFD:     return optpfdenc32(in, n, out); //if(n < 128) return vbyteenc(in, n, (unsigned *)out); else { unsigned tmp[2048]; for(i = 0; i < n; i++) tmp[i] = in[i]; return out + OPT4(tmp, n, (unsigned *)out); }
      #endif 
	  //encode(void *encoded, size_t encoded_buffer_length, const integer *source, size_t source_integers)
	  #if C_QMX
    case P_QMX:  		{ JASS::compress_integer_qmx_improved    qmx; unsigned r=qmx.encode(out+4, outsize, (uint32_t *)in, n); ctou32(out)=r; return out+4+r; } 
	  #endif
	  
      #if C_SIMDCOMP128
    //case SC_PACK:    	if(b < 0) { b = maxbits_length(in,n); *out++ = b; } return fastpackwithoutmask32(in, n, (uint32_t *)out, b);                                                                                                                              
    case SC_FOR:
    case SC_FORDA:
    case SC_SIMDPACK128:if(b < 0) b =    maxbits(in), *out++ = b; return (unsigned char *)simdpack_length(in, n, (__m128i *)out, b);
      #endif
      #if C_SIMDCOMP256
    case SC_SIMDPACK256:if(b < 0) b = avxmaxbits(in), *out++ = b; return (unsigned char *)avxpackwithoutmaskn(in, n, (unsigned *)out, b);
      #endif
	  #if C_STREAMVBYTE
    case P_STREAMVBYTE: return out + streamvbyte_encode(in, n, out); 
      #endif   
      #if C_VARINTG8IU
    case P_VARINTG8IU:  return vintg8enc(in, n, out);
      #endif 

      #if C_VTENC
    case P_VTENC:  { size_t l; vtenc_list_encode_u32(in, n, out+4, outsize, &l); ctou32(out) = l; return l+4; }
      #endif 
	  
     #if C_BITSHUFFLE      
    case P_BITSHUFFLE:  BITSHUFFLE( (unsigned char *)in, _n, out,lev); 		return out + _n;    
      #endif
      #if C_C_BLOSC      
    case P_BS_SHUFFLE:  shuffle( lev, _n, (unsigned char *)in, out); 		return out + _n;
      #endif
      // --------- transpose + lz77 ----------------------------------------
      #if C_LZTURBO
    case P_LZT10: { n *= 4; tpenc( (unsigned char *)in, n, sbuf,4); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 0; lz.hbits = 16; return out + lz8c01(&lz); }
    case P_LZT20: { n *= 4; tpenc( (unsigned char *)in, n, sbuf,4); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 0; lz.hbits = 16; return out + lzbc01(&lz); }
    case P_LZT22: { n *= 4; tpenc( (unsigned char *)in, n, sbuf,4); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 2; lz.hbits = 26; return out + lzbc2(&lz); }     
    case P_LZT32: { n *= 4; tpenc( (unsigned char *)in, n, sbuf,4); struct lzobj lz; lz.srclen = n; lz.src = sbuf; lz.dst = out; lz.dstlen = n; lz.level = 2; lz.hbits = 26; return out + lzhc2(&lz); }
    //case P_VSHUF: { unsigned char *p = vsenc32(in, n, sbuf); p = mheenc(sbuf, p-sbuf, MH_BLK, out+2, out+_n+256); ctou16(out) = p-(out+2); printf("L=%d ", p-(out+2));  return p; } //out + anscompress( sbuf, xb, out, out+_n);
    //case P_VSHUF: { unsigned char                           *p = mheenc(in, _n, MH_BLK, out+4, out+_n+256); ctou32(out) = p-(out+4); return p; } //out + anscompress( sbuf, xb, out, out+_n);
    case P_VSHUF: { unsigned char *p;/* = vsenc32(in, n, sbuf)*/ n *= 4; tpenc( (unsigned char *)in, n, sbuf, 4); p = mheenc(sbuf, n/*p-sbuf*/, MH_BLK, out+4, out+n+256); ctou32(out) = p-(out+4); return p; } //out + anscompress( sbuf, xb, out, out+_n);
      #endif

	  #if C_BITSHUFFLE      
    //case TB_VSIMPLE_S1:  BITSHUFFLE( (unsigned char *)in, _n, sbuf,4); return vsenc32((unsigned *)sbuf, n, out);
	  #endif
      #if C_LZ4
        #if C_BITSHUFFLE      
    case LZ4_BIT: 	 BITSHUFFLE( (unsigned char *)in, _n, sbuf,lev);return out + LZ4_compress((char *)sbuf, (char *)out, _n);
    case LZ4_BITX:   bitxenc32(in, n, (unsigned *)out, 0); BITSHUFFLE((unsigned char *)out, _n, sbuf,lev); return out + LZ4_compress((char *)sbuf, (char *)out, _n);
	    #endif
    case LZ4_  : 	 return out + LZ4_compress((char *)in, (char *)out, _n);
    case LZ4_NIBBLE: tp4enc((unsigned char *)in, _n, sbuf, lev); return out + LZ4_compress((char *)sbuf, (char *)out, _n);
//    case LZ4_NIBBLE: loenc64((uint64_t *)in, (_n+7)/8, 3, (uint64_t *)out); tp4enc((unsigned char *)out, _n, sbuf, lev); return out + LZ4_compress((char *)sbuf, (char *)out, _n);
    case LZ4_NIBBLEX:{bitxenc32(in, n, (unsigned *)out, 0); tp4enc((unsigned char *)out, _n, sbuf,lev); return out + LZ4_compress((char *)sbuf, (char *)out, _n); } 
    case LZ4_BYTE:   tpenc( (unsigned char *)in, _n, sbuf, lev); return out + LZ4_compress((char *)sbuf, (char *)out, _n);
    //case LZ4_FP8:     fp8zenc( (unsigned char *)in, _n, sbuf, sbuf+_n*2); return out + LZ4_compress((char *)sbuf, (char *)out, _n);
      #endif

      #if C_C_BLOSC
    case P_BS_LZ:
    case P_BS_LZ4: 
    case P_BS_ZLIB:  return out + blosc_compress(lev, BLOSC_SHUFFLE, 4/*typesize*/, _n, in, out, _n+BLOSC_MAX_OVERHEAD);
    //return out + blosc_compress(1/*clevel*/, BLOSC_BITSHUFFLE, 4/*typesize*/, _n, in, out, _n+BLOSC_MAX_OVERHEAD);
      #endif
	  
      #if C_ZLIB
    case P_ZLIB:  
      { n *= 4; tpenc( (unsigned char *)in, n, sbuf, 4); uLongf outlen = n; int rc = compress2(out+4, &outlen, sbuf, n, lev); if(rc != Z_OK) die("zlib compress2 rc=%d\n", rc); *(unsigned *)out = outlen; return out + 4 + outlen; }
      #endif  


