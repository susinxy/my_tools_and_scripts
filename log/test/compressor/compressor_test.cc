#include "../../gtinclude/gtest/gtest.h"
#include "../../compressor/compressor.h"
#include "../../decompressor/decompressor.h"
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

struct timeval start;
struct timeval end;
unsigned long timer1;
unsigned long timer2;
unsigned char str[1024*1024];
unsigned char str1[1024*1024];
unsigned char str2[1024*1024];
unsigned char str3[1024*1024*10];
unsigned char str4[1024*1024*10];
unsigned char str5[1024*1024];
unsigned char str6[1024*1024*20];

char compress_out[sizeof(str)];
char decompress_out[sizeof(str)];
char compress_out1[sizeof(str1)];
char decompress_out1[sizeof(str1)];
char compress_out2[sizeof(str2)];
char decompress_out2[sizeof(str2)];
char compress_out3[sizeof(str3)];
char decompress_out3[sizeof(str3)];
char compress_out4[sizeof(str4)];
char decompress_out4[sizeof(str4)];
char decompress_out5[sizeof(str5)];
char compress_out5[sizeof(str5)];
char decompress_out6[sizeof(str6)];
char compress_out6[sizeof(str6)];

int compress_len_out,decompress_len_out;
int  a[30];
int Pow(int a,int b)
{
    if(b==0) return 1;
    if(b&1) return a*Pow(a,b-1);
    else
    {
        int t=Pow(a,b/2);
        return t*t;
    }
}

TEST(compressor,para_test)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str);i++)
    {
        str[i] = rand() % 10;
    }
    gettimeofday(&start, NULL);
    int q = compress(str,sizeof(str),(void*)compress_out,&compress_len_out);
    gettimeofday(&end, NULL);
    timer1 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld us\n", timer1);
    
    if(!q)
    {
        gettimeofday(&start, NULL);
        decompress(compress_out,compress_len_out,(void*)decompress_out,&decompress_len_out);
        gettimeofday(&end, NULL);
        timer2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("time for decompress： %ld us\n", timer2);
	int rate =compress_len_out*100/decompress_len_out;
        printf("The compressor rate is:  %d%\n",rate);

        EXPECT_EQ(0,memcmp(str,decompress_out,decompress_len_out));
	
        
    }
    else
    {
        printf("failed!\n");
    }
}

TEST(compressor,para_test01)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str1);i++)
    {
        str1[i] = rand() % 128;
    }
    gettimeofday(&start, NULL);
    int q1 = compress(str1,sizeof(str1),(void*)compress_out1,&compress_len_out);
    gettimeofday(&end, NULL);
    timer1 = 1000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld us\n", timer1);
    
    if(!q1)
    {
        gettimeofday(&start, NULL);
        decompress(compress_out1,compress_len_out,(void*)decompress_out1,&decompress_len_out);
        gettimeofday(&end, NULL);
        timer2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("time for decompress： %ld us\n", timer2);
        int rate = compress_len_out*100/decompress_len_out;
        printf("The compressor rate is:  %d%\n",rate);
	EXPECT_EQ(0,memcmp(str1,decompress_out1,decompress_len_out));
        
        
    }
    else
    {
        printf("failed!\n");
    }
    
}

TEST(compressor,para_test02)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str2);i++)
    {
        str2[i] = rand() % 255;
    }
    gettimeofday(&start, NULL);
    int q2 = compress(str2,sizeof(str2),(void*)compress_out2,&compress_len_out);
    gettimeofday(&end, NULL);
    timer1 = 1000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld us\n", timer1);
    
    if(!q2)
    {
        gettimeofday(&start, NULL);
        decompress(compress_out2,compress_len_out,(void*)decompress_out2,&decompress_len_out);
        gettimeofday(&end, NULL);
        timer2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("time for decompress： %ld us\n", timer2);
        int rate =compress_len_out*100/decompress_len_out;
        printf("The compressor rate is:  %d%\n",rate);
	EXPECT_EQ(0,memcmp(str2,decompress_out2,decompress_len_out)); 
    }
    else
    {
        printf("failed!\n");
    }
    
}

TEST(compressor,para_test03)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str3);i++)
    {
        str3[i] = rand() % 10;
    }
    gettimeofday(&start, NULL);
    int q3 = compress(str3,sizeof(str3),(void*)compress_out3,&compress_len_out);
    gettimeofday(&end, NULL);
    timer1 = 1000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld us\n", timer1);
    
    if(!q3)
    {
        gettimeofday(&start, NULL);
        decompress(compress_out3,compress_len_out,(void*)decompress_out3,&decompress_len_out);
        gettimeofday(&end, NULL);
        timer2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("time for decompress： %ld us\n", timer2);
        EXPECT_EQ(0,memcmp(str3,decompress_out3,decompress_len_out));
        int rate = compress_len_out*100/decompress_len_out;
        printf("The compressor rate is:  %d%\n",rate);
    }
    else
    {
        printf("failed!\n");
    }
     
}

TEST(compressor,para_test04)
{
    a[0]=1;
    for(int i=1;i<=23;++i) a[i]=Pow(2,i-1);
    int j=0;
    unsigned char *p=str4;
    for(int i=0;i<=23;++i)
    {
        j+=a[i];
        while(a[i]--)
        {
            *p=i;
            p++;
        }
    }
    for(int i=0;i<sizeof(str4)-j;++i)
    {
        *p=25;
        p++;
    }
    gettimeofday(&start, NULL);
    int q4 = compress(str4,sizeof(str4),(void*)compress_out4,&compress_len_out);
    gettimeofday(&end, NULL);
    timer1 = 1000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld us\n", timer1);
    
    if(!q4)
    {
        gettimeofday(&start, NULL);
        decompress(compress_out4,compress_len_out,(void*)decompress_out4,&decompress_len_out);
        gettimeofday(&end, NULL);
        timer2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("time for decompress： %ld us\n", timer2);
        int rate = compress_len_out*100/decompress_len_out;
        printf("The compressor rate is:  %d%\n",rate);
      
        
    }
    else
    {
        printf("failed!\n");
    }
     
}

TEST(compressor,para_test05)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str5);i++)
    {   
	int j = rand()%2;
	if (j == 0)
	  str5[i] = 0;
	else str5[i] = rand()%128;
	 
    }
    gettimeofday(&start, NULL);
    int q5 = compress(str5,sizeof(str5),(void*)compress_out5,&compress_len_out);
    gettimeofday(&end, NULL);
    timer1 = 1000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld us\n", timer1);
    
    if(!q5)
    {
        gettimeofday(&start, NULL);
        decompress(compress_out5,compress_len_out,(void*)decompress_out5,&decompress_len_out);
        gettimeofday(&end, NULL);
        timer2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("time for decompress： %ld us\n", timer2);
        EXPECT_EQ(0,memcmp(str5,decompress_out5,decompress_len_out));
        int rate = compress_len_out*100/decompress_len_out;
        printf("The compressor rate is:  %d%\n",rate);
    }
    else
    {
        printf("failed!\n");
    }
     
}

TEST(compressor,para_test06)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(str6);i++)
    {   
	int j = rand()%2;
	if (j == 0)
	  str6[i] = 0;
	else str6[i] = rand()%128;
	 
    }
    gettimeofday(&start, NULL);
    int q6 = compress(str6,sizeof(str6),(void*)compress_out6,&compress_len_out);
    gettimeofday(&end, NULL);
    timer1 = 1000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("time for compress： %ld us\n", timer1);
    
    if(!q6)
    {
        gettimeofday(&start, NULL);
        decompress(compress_out6,compress_len_out,(void*)decompress_out6,&decompress_len_out);
        gettimeofday(&end, NULL);
        timer2 = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
        printf("time for decompress： %ld us\n", timer2);
        EXPECT_EQ(0,memcmp(str6,decompress_out6,decompress_len_out));
        int rate = compress_len_out*100/decompress_len_out;
        printf("The compressor rate is:  %d%\n",rate);
    }
    else
    {
        printf("failed!\n");
    }
     
}






