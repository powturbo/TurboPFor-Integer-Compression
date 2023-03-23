//-- "Integer Compression" variable byte (scalar TurboVByte+ SIMD TurboByte) ---------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//----------------------------- TurboVByte 'vb':Variable byte + SIMD TurboByte 'v8': array functions -----
size_t vbbound8( size_t n);
size_t vbbound16(size_t n);
size_t vbbound32(size_t n);
size_t vbbound64(size_t n);

// Encoding/Decoding: Return value = end of compressed/decompressed output/input buffer out/in

//----------------------- Encoding/Decoding unsorted array with n integer values --------------------------
unsigned char *vbenc16(  unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out); //TurboVByte
unsigned char *vbdec16(  unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out);

unsigned char *vbenc32(  unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbdec32(  unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out);

unsigned char *vbenc64(  uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *vbdec64(  unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out);

//-- Get value stored at index idx (idx:0...n-1)
unsigned short vbgetx16( unsigned char *__restrict in, unsigned idx);
unsigned       vbgetx32( unsigned char *__restrict in, unsigned idx);
uint64_t       vbgetx64( unsigned char *__restrict in, unsigned idx);

//-- Search and return index of next value equal to key or n when no key value found
// ex. unsigned idx;unsigned char *ip; for(idx=0,ip=in;;) { if((idx = vgeteq32(&ip, idx, 4321))>=n) break; printf("found at %u ", idx); }
unsigned vbgeteq16( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short key);
unsigned vbgeteq32( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned       key);
unsigned vbgeteq64( unsigned char **__restrict in, unsigned n, unsigned idx, uint64_t       key);

//---------------------- Delta encoding/decoding sorted array ---------------------------------------------
//-- Increasing integer array. out[i] = out[i-1] + in[i]
unsigned char *vbdenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbddec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *vbdenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbddec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

unsigned char *vbdenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbddec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);

//-- Get value stored at index idx (idx:0...n-1)
unsigned short vbdgetx16(  unsigned char *__restrict in, unsigned idx, unsigned short start);
unsigned       vbdgetx32(  unsigned char *__restrict in, unsigned idx, unsigned start);
uint64_t       vbdgetx64(  unsigned char *__restrict in, unsigned idx, uint64_t start);

//-- Search and return index of next value equal to key or n when no key value found
// ex. unsigned idx;unsigned char *ip; for(idx=0,ip=in;;) { if((idx = vgeteq32(&ip, idx, 4321))>=n) break; printf("found at %u ", idx); }
unsigned vbdgetgeq16( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short *key, unsigned short start);
unsigned vbdgetgeq32( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned       *key, unsigned       start);
unsigned vbdgetgeq64( unsigned char **__restrict in, unsigned n, unsigned idx, uint64_t       *key, uint64_t       start);

//-- Strictly increasing (never remaining constant or decreasing) integer array. out[i] = out[i-1] + in[i] + 1
unsigned char *vbd1enc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbd1enc32(unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbd1enc64(uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);

unsigned char *vbd1dec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *vbd1dec32(unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);
unsigned char *vbd1dec64(unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);


//-- Get value stored at index idx (idx:0...n-1)
unsigned short vbd1getx16(  unsigned char *__restrict in, unsigned idx, unsigned short start);
unsigned       vbd1getx32(  unsigned char *__restrict in, unsigned idx, unsigned       start);
uint64_t       vbd1getx64(  unsigned char *__restrict in, unsigned idx, uint64_t       start);

//-- Search and return index of next value equal to key or n when no key value found
// ex. unsigned idx;unsigned char *ip; for(idx=0,ip=in;;) { if((idx = vgeteq32(&ip, idx, 4321))>=n) break; printf("found at %u ", idx); }
unsigned vbd1getgeq16( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short *key, unsigned short start);
unsigned vbd1getgeq32( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned       *key, unsigned       start);
unsigned vbd1getgeq64( unsigned char **__restrict in, unsigned n, unsigned idx, uint64_t       *key, uint64_t       start);

//---------------------- Zigzag encoding/decoding for unsorted integer lists.
unsigned char *vbzenc8(  unsigned char  *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned char start);
unsigned char *vbzdec8(  unsigned char  *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned char  start);

unsigned char *vbzenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbzdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *vbzenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbzdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

unsigned char *vbzenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbzdec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);

//---------------------- XOR encoding/decoding for unsorted integer lists.
unsigned char *vbxenc8(  unsigned char  *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned char start);
unsigned char *vbxdec8(  unsigned char  *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned char  start);

unsigned char *vbxenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbxdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *vbxenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbxdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

unsigned char *vbxenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbxdec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);

//---------------------- Delta of delta encoding/decoding for unsorted integer lists.
unsigned char *vbddenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *vbdddec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *vbddenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *vbdddec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

unsigned char *vbddenc64( uint64_t       *__restrict in, unsigned n, unsigned char  *__restrict out, uint64_t       start);
unsigned char *vbdddec64( unsigned char  *__restrict in, unsigned n, uint64_t       *__restrict out, uint64_t       start);

//-- Get value stored at index idx (idx:0...n-1)
unsigned short vbzgetx16(  unsigned char *__restrict in, unsigned idx, unsigned short start);
unsigned       vbzgetx32(  unsigned char *__restrict in, unsigned idx, unsigned       start);
uint64_t       vbzgetx64(  unsigned char *__restrict in, unsigned idx, uint64_t       start);

//-- Search and return index of next value equal to key or n when no key value found
// ex. unsigned idx;unsigned char *ip; for(idx=0,ip=in;;) { if((idx = vgeteq32(&ip, idx, 4321))>=n) break; printf("found at %u ", idx); }
/*unsigned vbzgeteq15( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short key, unsigned start);
unsigned vbzgeteq16( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned short key, unsigned start);
unsigned vbzgeteq32( unsigned char **__restrict in, unsigned n, unsigned idx, unsigned       key, unsigned start);
unsigned vbzgeteq64( unsigned char **__restrict in, unsigned n, unsigned idx, uint64_t       key, unsigned start);*/

//-------------------------- TurboByte (SIMD Group varint) --------------------------------------------------------------
size_t v8bound16(size_t n);
size_t v8bound32(size_t n);

size_t v8len32(const uint32_t *in, size_t n);
size_t v8len16(const uint16_t *in, size_t n);

unsigned char *v8enc16(  unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out); //TurboByte
unsigned char *v8dec16(  unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out);

unsigned char *v8enc32(  unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out);
unsigned char *v8dec32(  unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out);

//------ delta ---------
unsigned char *v8denc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *v8ddec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *v8denc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *v8ddec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

//------ delta 1 -------
unsigned char *v8d1enc16(unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *v8d1dec16(unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *v8d1enc32(unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *v8d1dec32(unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

//------- zigzag -------
unsigned char *v8zenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *v8zdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);

unsigned char *v8zenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);
unsigned char *v8zdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

//------- xor ----------
unsigned char *v8xenc16( unsigned short *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned short start);
unsigned char *v8xenc32( unsigned       *__restrict in, unsigned n, unsigned char  *__restrict out, unsigned       start);

unsigned char *v8xdec16( unsigned char  *__restrict in, unsigned n, unsigned short *__restrict out, unsigned short start);
unsigned char *v8xdec32( unsigned char  *__restrict in, unsigned n, unsigned       *__restrict out, unsigned       start);

//-------------------------- TurboByte Hybrid (SIMD Group varint) + Bitpacking -------------------------------------------
size_t v8nenc16(  uint16_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec16(  unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nenc32(  uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec32(  unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8ndenc16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // delta
size_t v8nddec16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8ndenc32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nddec32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nd1enc16(uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // delta 1
size_t v8nd1dec16(unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nd1enc32(uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nd1dec32(unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nzenc16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // zigzag delta
size_t v8nzdec16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nzenc32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nzdec32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nxenc16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // xor
size_t v8nxdec16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nxenc32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nxdec32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

//------------- sse / neon ----------------------------------------------------------------------------
size_t v8nenc128v16(  uint16_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec128v16(  unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nenc128v32(  uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec128v32(  unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8ndenc128v16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // delta
size_t v8nddec128v16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8ndenc128v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nddec128v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nd1enc128v16(uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // delta 1
size_t v8nd1dec128v16(unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nd1enc128v32(uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nd1dec128v32(unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nzenc128v16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // zigzag delta
size_t v8nzdec128v16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nzenc128v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nzdec128v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nxenc128v16( uint16_t      *__restrict in, size_t n, unsigned char *__restrict out); // xor
size_t v8nxdec128v16( unsigned char *__restrict in, size_t n, uint16_t      *__restrict out);

size_t v8nxenc128v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nxdec128v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

//------------- avx2 ------------------------------------------------------------------------------------
size_t v8nenc256v32(  uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8ndec256v32(  unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8ndenc256v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nddec256v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nd1enc256v32(uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nd1dec256v32(unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nzenc256v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nzdec256v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

size_t v8nxenc256v32( uint32_t      *__restrict in, size_t n, unsigned char *__restrict out);
size_t v8nxdec256v32( unsigned char *__restrict in, size_t n, uint32_t      *__restrict out);

#ifdef __cplusplus
}
#endif

