#include "../../gtinclude/gtest/gtest.h"
#include "../../compressor/compressor.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

/*void rand_inital(char *str,int num)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < num;i++,str++)
    {
        *str = rand() % 100 + 28;  
    }
}*/

char str[1024];
char compress_out[sizeof(str)];
char decompress_out[sizeof(str)];
TEST(compressor,para_test)
{   
    int compress_len_out,decompress_len_out;
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str);i++)
    {
        str[i] = rand() % 100 + 28;  
    }
    //printf("%s",str); 
    printf("%s",str);
    compress(str,sizeof(str),(void*)compress_out,&compress_len_out);
    decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out);   

    EXPECT_EQ(0,memcmp(str,decompress_out,decompress_len_out));
    EXPECT_LT(sizeof(str),decompress_len_out);
}
