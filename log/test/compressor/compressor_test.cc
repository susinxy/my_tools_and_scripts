#include "../../gtinclude/gtest/gtest.h"
#include "../../compressor/compressor.h"
#include "../../decompressor/decompressor.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
struct timeval start;
struct timeval end;
unsigned long timer;
char str[1024*1024];
char str1[1024*1024];
char str2[1024*1024];
char str3[1024*1024];
char str4[1024*1024];

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
   
    gettimeofday(&start, NULL);
    compress(str,sizeof(str),(void*)compress_out,&compress_len_out);
    gettimeofday(&end, NULL);
    timer = 1000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld ms\n", timer);
    gettimeofday(&start, NULL);
    decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out);  
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for decompress： %ld ms\n", timer);
    double rate = double(compress_len_out)/double(decompress_len_out);
    printf("The compressor rate is:  %f%",rate);
   
    EXPECT_EQ(0,memcmp(str,decompress_out,decompress_len_out));
    EXPECT_GT(sizeof(str),compress_len_out);
}

TEST(compressor,para_test01)
{   
    int compress_len_out,decompress_len_out;
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str1);i++) 
    {
        switch (rand()%2)
	 { case 0:
    	    str1[i]=rand()%26+65;
      	    break;
          case 1:
            str1[i]=rand()%26+97;
            break;
         }  
     }  
  
    gettimeofday(&start, NULL);
    compress(str1,sizeof(str1),(void*)compress_out,&compress_len_out);
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld μs\n", timer);

    gettimeofday(&start, NULL);
    decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out); 
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for decompress： %ld μs\n", timer); 
    double rate =double(compress_len_out)/double(decompress_len_out);
    printf("The compressor rate is:  %f%",rate);
   
    
    EXPECT_EQ(0,memcmp(str1,decompress_out,decompress_len_out));
    EXPECT_GT(sizeof(str1),compress_len_out);

}
TEST(recorder,para_test02)
{
   int compress_len_out,decompress_len_out;
   srand((unsigned)time(NULL));
   for (int i = 0; i < sizeof(str);i++)
   {
       str2[i] = rand() % 100 + 28;  
   }

    gettimeofday(&start, NULL);
    compress(str2,sizeof(str2),(void*)compress_out,&compress_len_out);
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld μs\n", timer);

    gettimeofday(&start, NULL);
    decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out); 
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for decompress： %ld ms\n", timer);  
    double rate =double(compress_len_out)/double(decompress_len_out);  
   printf("The compressor rate is:  %f%",rate);
  
   EXPECT_EQ(0,memcmp(str2,decompress_out,decompress_len_out));
   EXPECT_GT(sizeof(str2),compress_len_out);

}
TEST(recorder,para_test03)
{
   int compress_len_out,decompress_len_out;
   srand((unsigned)time(NULL));
   for (int i = 0; i < sizeof(str3);i++)
   {
       str3[i] = rand() % 100 + 28;  
   }

   gettimeofday(&start, NULL);
    compress(str3,sizeof(str3),(void*)compress_out,&compress_len_out);
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld μs\n", timer);

    gettimeofday(&start, NULL);
    decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out);  
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for decompress： %ld ms\n", timer);
   double rate =double(compress_len_out)/double(decompress_len_out);   
   printf("The compressor rate is:  %f%",rate);
  
   EXPECT_EQ(0,memcmp(str3,decompress_out,decompress_len_out));
   EXPECT_GT(sizeof(str3),compress_len_out);

}


TEST(recorder,para_test04)
{
   int compress_len_out,decompress_len_out;
   srand((unsigned)time(NULL));
   for (int i = 0; i < sizeof(str4);i++)
    {
        str[i] = rand() % 10 + 48;  
    }

    gettimeofday(&start, NULL);
    compress(str4,sizeof(str4),(void*)compress_out,&compress_len_out);
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld μs\n", timer);

    gettimeofday(&start, NULL);
   decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out);  
    gettimeofday(&end, NULL);
    timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for decompress： %ld ms\n", timer); 
   double rate =double(compress_len_out)/double(decompress_len_out);
   printf("The compressor rate is:  %f%",rate);
   EXPECT_EQ(0,memcmp(str4,decompress_out,decompress_len_out));
   EXPECT_GT(sizeof(str4),compress_len_out);

}


