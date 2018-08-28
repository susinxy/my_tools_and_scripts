#ifndef DECOMPRESSOR_H_INCLUDED
#define DECOMPRESSOR_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TREE_HT  256
#define MAX_CHAR 256
#define READ_LEN 8
int decompress(void *addr_in,int len_in, void*addr_out, int *len_out);

#ifdef __cplusplus
}
#endif
#endif
