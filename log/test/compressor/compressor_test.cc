#include "../../gtinclude/gtest/gtest.h"
#include "../../compressor/compressor.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


char str[1024];
char compress_out[sizeof(str)];
char decompress_out[sizeof(str)];
int compress_len_out,decompress_len_out;

TEST(compressor,para_test)
{   
    int compress_len_out,decompress_len_out;
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str);i++)
    {
        str[i] = rand() % 10 + 48;  
    }
    printf("%s",str);
    compress(str,sizeof(str),(void*)compress_out,&compress_len_out);
    decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out);   
    EXPECT_EQ(0,memcmp(str,decompress_out,decompress_len_out));
    EXPECT_LT(sizeof(str),decompress_len_out);
}

TEST(compressor,para_test01)
{   
    int compress_len_out,decompress_len_out;
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str);i++)
    {
        switch (rand()%2)
	{
  	  case 0:
    	    str[i]=rand()%26+65;
      	    break;
          case 1:
            str[i]=rand()%26+97;
            break;
        }     
    }
    printf("%s",str);
    compress(str,sizeof(str),(void*)compress_out,&compress_len_out);
    decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out);   
    EXPECT_EQ(0,memcmp(str,decompress_out,decompress_len_out));
    EXPECT_LT(sizeof(str),decompress_len_out);
}
TEST(recorder,para_test02)
{
   int compress_len_out,decompress_len_out;
   srand((unsigned)time(NULL));
   for (int i = 0; i < sizeof(str);i++)
   {
       str[i] = rand() % 100 + 28;  
   }
   printf("%s",str);
   compress(str,sizeof(str),(void*)compress_out,&compress_len_out);
   decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out);   
   EXPECT_EQ(0,memcmp(str,decompress_out,decompress_len_out));
   EXPECT_LT(sizeof(str),decompress_len_out);

}
