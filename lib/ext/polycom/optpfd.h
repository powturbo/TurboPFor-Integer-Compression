#ifdef __cplusplus
extern "C" {
#endif
#define OPTPFDMAX 2048
unsigned char *optpfdenc32(unsigned      *in, int n, unsigned char *out);
unsigned char *optpfddec32(unsigned char *in, int n, unsigned      *out);

#ifdef __cplusplus
}
#endif

