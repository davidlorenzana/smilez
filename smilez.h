#ifndef SMILEZ_H
#define SMILEZ_H

#ifdef __cplusplus
extern "C" {
#endif

#define SMILEZ_VERSION "1.0"
#define SMILEZ_MAJOR_VERSION 1
#define SMILEZ_MINOR_VERSION 0

#define SMILEZ_COMPRESSION_VERSION 1

#define SMILEZ_BYTE_DICTIONARY 0
#define SMILEZ_WHITESPACE_DICTIONARY 1
#define SMILEZ_NUM_DICTIONARIES 2

const char *smilez_get_version(void);
int smilez_get_compression_version(void);

int smilez_compress(const char *in, int inlen, char *out, int outlen, int dictionary);
int smilez_decompress(const char *in, int inlen, char *out, int outlen);

#ifdef __cplusplus
}
#endif


#endif SMILEZ_H
