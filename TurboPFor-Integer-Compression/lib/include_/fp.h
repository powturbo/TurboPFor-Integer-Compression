//---------- "Floating Point + Integer Compression" -----------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

// ---------- TurboPFor Zigzag of delta (=delta of delta + zigzag encoding) (TurboPFor)
size_t p4nzzenc128v8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t p4nzzdec128v8(    unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t p4nzzenc128v16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t p4nzzdec128v16(   unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t p4nzzenc128v32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t p4nzzdec128v32(   unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t p4nzzenc128v64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t p4nzzdec128v64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- Zigzag (bit/io) -------------------------------------------------------
size_t bvzenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t bvzdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t bvzenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t bvzdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t bvzenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t bvzdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t bvzenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t bvzdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
//----------- Zigzag of delta (bit/io) ---------------------------------------------
size_t bvzzenc8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t bvzzdec8(    unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t bvzzenc16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t bvzzdec16(   unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t bvzzenc32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t bvzzdec32(   unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t bvzzenc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t bvzzdec64(   unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboGorilla : Improved gorilla style + RLE (bit/io) ------------------
size_t fpgenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpgdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpgenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpgdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpgenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpgdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpgenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpgdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- Chimp algo : (bit/io) ------------------------------------------------
size_t fphenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fphdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fphenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fphdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fphenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fphdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fphenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fphdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloatLz (bitio/io) ------------------
size_t fpc0enc8(    uint8_t       *in, size_t n, unsigned char *out, uint8_t  start); // fpcenc slower but stronger than fpc0enc
size_t fpc0enc16(   uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpc0enc32(   uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpc0enc64(   uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpcdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);
size_t fpcenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpcdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpcenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpcdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpcenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpcdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpcenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpcdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloat XOR : Last value predictor (TurboPFor) ---------------------
size_t fpxenc8(     uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpxdec8(     unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpxenc16(    uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpxdec16(    unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpxenc32(    uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpxdec32(    unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpxenc64(    uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpxdec64(    unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloat FCM: Finite Context Method Predictor (TurboPFor) -----------
size_t fpfcmenc8(   uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpfcmdec8(   unsigned char *in, size_t n, uint8_t      *out,  uint8_t  start);
size_t fpfcmenc16(  uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpfcmdec16(  unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpfcmenc32(  uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpfcmdec32(  unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpfcmenc64(  uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpfcmdec64(  unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloat DFCM: Differential Finite Context Method Predictor (TurboPFor)
size_t fpdfcmenc8(  uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fpdfcmdec8(  unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fpdfcmenc16( uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fpdfcmdec16( unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fpdfcmenc32( uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fpdfcmdec32( unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fpdfcmenc64( uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fpdfcmdec64( unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

//----------- TurboFloat 2D DFCM: Differential Finite Context Method Predictor -----
size_t fp2dfcmenc8( uint8_t       *in, size_t n, unsigned char *out, uint8_t  start);
size_t fp2dfcmdec8( unsigned char *in, size_t n, uint8_t       *out, uint8_t  start);
size_t fp2dfcmenc16(uint16_t      *in, size_t n, unsigned char *out, uint16_t start);
size_t fp2dfcmdec16(unsigned char *in, size_t n, uint16_t      *out, uint16_t start);
size_t fp2dfcmenc32(uint32_t      *in, size_t n, unsigned char *out, uint32_t start);
size_t fp2dfcmdec32(unsigned char *in, size_t n, uint32_t      *out, uint32_t start);
size_t fp2dfcmenc64(uint64_t      *in, size_t n, unsigned char *out, uint64_t start);
size_t fp2dfcmdec64(unsigned char *in, size_t n, uint64_t      *out, uint64_t start);

#ifdef __cplusplus
}
#endif

