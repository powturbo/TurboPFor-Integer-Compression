// "variablebyte.h" C Version port by powturbo from https://github.com/lemire/FastPFor
/**
 * This code is released under the
 * Apache License Version 2.0 http://www.apache.org/licenses/.
 *
 * (c) Daniel Lemire, http://lemire.me/en/
 */

#define extract7bits(i, val) (val >> (7 * i)) & ((1U << 7) - 1)
#define extract7bitsmaskless(i, val) (val >> (7 * i))

unsigned char *vbyteenc(unsigned *in, const size_t length, unsigned *out/*,
                     size_t &nvalue*/) {
        unsigned char *bout = (unsigned char *)(out);
        //const unsigned char *const initbout = (unsigned char *)(out);
        //unsigned prev = 0;
        size_t k;
        for (k = 0; k < length; ++k) {
            const unsigned val = /*delta ? in[k] - prev :*/ in[k];
            //if (delta) prev = in[k];
            /**
             * Code below could be shorter. Whether it could be faster
             * depends on your compiler and machine.
             */
            if (val < (1U << 7)) {
                *bout = (unsigned char)(val | (1U << 7));
                ++bout;
            } else if (val < (1U << 14)) {
                *bout = extract7bits(0,val);
                ++bout;
                *bout = extract7bitsmaskless(1,val) | (1U << 7);
                ++bout;
            } else if (val < (1U << 21)) {
                *bout = extract7bits(0,val);
                ++bout;
                *bout = extract7bits(1,val);
                ++bout;
                *bout = extract7bitsmaskless(2,val) | (1U << 7);
                ++bout;
            } else if (val < (1U << 28)) {
                *bout = extract7bits(0, val);
                ++bout;
                *bout = extract7bits(1, val);
                ++bout;
                *bout = extract7bits(2, val);
                ++bout;
                *bout = extract7bitsmaskless(3, val) | (1U << 7);
                ++bout;
            } else {
                *bout = extract7bits(0,val);
                ++bout;
                *bout = extract7bits(1,val);
                ++bout;
                *bout = extract7bits(2,val);
                ++bout;
                *bout = extract7bits(3,val);
                ++bout;
                *bout = extract7bitsmaskless(4,val) | (1U << 7);
                ++bout;
            }
        }
        /*while (needPaddingTo32Bits(bout)) {
            *bout++ = 0;
        }
        const size_t storageinbytes = bout - initbout;
        assert((storageinbytes % 4) == 0);
        nvalue = storageinbytes / 4;*/
  return bout;
}


unsigned char *vbytedec(const unsigned char *in, const size_t length,
                                unsigned *out/*, size_t &nvalue*/) {
        unsigned prev = 0;
        if (length == 0) {
            //nvalue = 0;
            return (unsigned char *)in;//abort
        }
        const unsigned char *inbyte = (const unsigned char *)(in);
        const unsigned char *const endbyte = (const unsigned char *)(out
                                       + length);
        //const unsigned *const initout(out);

        while ((unsigned *)endbyte > out) {
            unsigned int shift = 0; unsigned v;
            for (v = 0; (unsigned *)endbyte > out; shift += 7) {
                unsigned char c = *inbyte++;
                v += ((c & 127) << shift);
                if ((c & 128)) {
                    *out++ = /*delta ? (prev = v + prev) :*/ v;
                    break;
                }
            }
        }
        //nvalue = out - initout;
        //inbyte = padTo32bits(inbyte);
        return (unsigned char *)inbyte;
    }

