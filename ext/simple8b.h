#define SIMPLE8BMAX 1024
#ifdef __cplusplus
extern "C" {
#endif
unsigned char *vs8benc(unsigned      *__restrict in, int n, unsigned char *__restrict out);
unsigned char *vs8bdec(unsigned char *__restrict in, int n, unsigned *__restrict out);
#ifdef __cplusplus
}
#endif
