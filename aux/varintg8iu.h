#include <stdint.h>
void VarIntG8IU();
unsigned char *vintg8enc(const uint32_t *__restrict__ in, const size_t length, unsigned char      *__restrict__ out);
unsigned char *vintg8dec(const unsigned char *__restrict__ in, const size_t length, uint32_t *__restrict__ out);

