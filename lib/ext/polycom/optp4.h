#ifdef __cplusplus
extern "C" {
#endif
#define OPTPFDMAX 2048
unsigned char *optpfdenc32(unsigned *__restrict in, int n, unsigned *__restrict out);
unsigned char *optpfddec32(unsigned *__restrict in, int n, unsigned *__restrict out);

#ifdef __cplusplus
}
#endif

