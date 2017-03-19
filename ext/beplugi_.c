      #if C_C_BLOSC
    case P_BS_LZ4:  blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_LZ4_COMPNAME);break;
    case P_BS_ZLIB: blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_ZLIB_COMPNAME); break;
    case P_BS_LZ:   blosc_init(); blosc_set_nthreads(1); blosc_set_compressor(BLOSC_BLOSCLZ_COMPNAME); break;
      #endif     
      #if C_VARINTG8IU
    case P_VARINTG8IU: VarIntG8IU();
      #endif
       #ifdef LZTURBO  
 //   #include "../beplug0.h"
      #endif

