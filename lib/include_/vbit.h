//-- "Integer Compression" variable length encoding with bitio ------------------

#ifdef __cplusplus
extern "C" {
#endif
//-- Gamma coding (length limited) : Full 32 bit range
size_t bitgenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);  
size_t bitgdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

//-- Rice Coding (adaptive length limited) : Full 32 bit range ---
size_t bitrenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);   
size_t bitrdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlcenc32(   unsigned char *_in, size_t _inlen,  unsigned char  *out);   // TurboVLC 32 bits
size_t vlcdec32(   unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlcenc16(   unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 16 bits
size_t vlcdec16(   unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlczenc16(  unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 16 bits zigzag
size_t vlczdec16(  unsigned char  *in, size_t _outlen, unsigned char *_out);

size_t vlczenc32(  unsigned char *_in, size_t _inlen,  unsigned char  *out);  // TurboVLC 32 bits zigzag
size_t vlczdec32(  unsigned char  *in, size_t _outlen, unsigned char *_out);

#ifdef __cplusplus
}
#endif

