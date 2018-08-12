int compress(void *addr_in,int len_in, void**addr_out, int *len_out)
{
    *addr_out=addr_in;
    *len_out=len_in/2;
    return 0;
}

int decompress(void *addr_in,int len_in, void**addr_out, int *len_out)
{
    return 0;
}
