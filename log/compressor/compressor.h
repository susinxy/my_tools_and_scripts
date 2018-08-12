#ifndef _LOG_COMPRESSOR_H
#define _LOG_COMPRESSOR_H

#ifdef __cplusplus
extern "C" {
#endif
int compress(void *addr_in,int len_in, void**addr_out, int *len_out);
int decompress(void *addr_in,int len_in, void**addr_out, int *len_out);

#ifdef __cplusplus
}
#endif
#endif
