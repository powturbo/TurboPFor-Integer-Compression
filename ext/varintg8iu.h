#include <stdint.h>
void VarIntG8IU();
unsigned char *vintg8enc(const uint32_t *__restrict in, const size_t length, unsigned char      *__restrict out);
unsigned char *vintg8dec(unsigned char *__restrict in, const size_t length, uint32_t *__restrict out);

