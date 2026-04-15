typedef enum PcoError {
  PcoSuccess,
  PcoInvalidType,
  PcoCompressionError,
  PcoDecompressionError,
} PcoError;

typedef struct PcoFfiVec {
  const void *ptr;
  unsigned int len;
  const void *raw_box;
} PcoFfiVec;

enum PcoError pco_simpler_compress(const void *nums,
                                   unsigned int len,
                                   unsigned char dtype,
                                   unsigned int level,
                                   struct PcoFfiVec *dst);

enum PcoError pco_simple_decompress(const void *compressed,
                                    unsigned int len,
                                    unsigned char dtype,
                                    struct PcoFfiVec *dst);

enum PcoError pco_free_pcovec(struct PcoFfiVec *ffi_vec);
