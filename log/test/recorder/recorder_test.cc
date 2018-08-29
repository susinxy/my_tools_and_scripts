#include "../../gtinclude/gtest/gtest.h"
#include "../../recorder/recorder.h"
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <malloc.h>

char test0[512];
char test01[512];

char test1[512];
char test11[512];

char test2[512];
char test21[512];

char test3[512];
char test31[512];

char test4[512];
char test41[512];

char test5[512];
char test51[512];

char test6[512];
char test7[1024*1024];
char test8[1024*1024];
char test9[512];
char test10[1024*1024];
char test101[1024*1024];

char s1[200]="A0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz";
char s2[100]="B0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz";
char s3[200]="B0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz";
char s4[1024*20];
struct tm *timess;

typedef struct element
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned int type;
    unsigned int key;
    unsigned int oper;
    unsigned int len;
    int num;
    int first;
}record_elements;

void init_meta_data_section(void *addr,int len,int block_size)
{
    int node_cnt = len/(sizeof(record_node)+block_size);
    int i;
    record_node *node =(record_node*)addr;
    for(i=0;i<node_cnt;i++)
    {
        node[i].in_use=0;
	node[i].block_size=block_size;
        node[i].block_offset=i*block_size+node_cnt*sizeof(record_node);
	if(i==node_cnt-1)
        {
            node[i].next_offset=0;
        }
        else
        {
            node[i].next_offset=sizeof(record_node)*(i+1);
        }
        node[i].how_many_blocks=0;
    }
}


TEST(recorder,para_test)//record_section:512%(52+12) == 0
{
    EXPECT_EQ(0,record_section(0,52,(void*)test0,512));
    init_meta_data_section((void*)test01,512,52);
    EXPECT_EQ(0,memcmp((void*)test0,(void*)test01,512));
    EXPECT_EQ(0,record_section_destory(0));
}

TEST(recorder,para_test01)//record_section:512%(54+12) != 0
{
    EXPECT_EQ(0,record_section(0,54,(void*)test1,512));
    init_meta_data_section((void*)test11,512,54);
    EXPECT_EQ(0,memcmp((void*)test1,(void*)test11,512));
    EXPECT_EQ(0,record_section_destory(0));
}

TEST(recorder,para_test02)
{ 
    record_section(1,52,test2,512);
    init_meta_data_section((void*)test21,512,52);
    time_t t;
    t = time(0);
    timess = localtime(&t);
    record(1,1,1,1,1,s1,4);
   
    test21[2]=1;
    test21[3]=1;
    record_elements *p=(record_elements*)malloc(32);

    p->year=timess->tm_year+1900;
    p->month=timess->tm_mon+1;
    p->day=timess->tm_mday;
    p->hour=timess->tm_hour;
    p->min=timess->tm_min;
    p->sec=timess->tm_sec;
    p->type=1;
    p->key=1;
    p->oper=1;
    p->len=36;
    p->num=1;
    p->first=4;

    char *a = "A012";

    memcpy(test21+96,p,32);  
    memcpy(test21+128,a,4);
    for(int i=0;i<512;i++)
    {
        if(test2[i]!=test21[i])
	{
		printf("%d:%c|%c\n",i,test2[i],test21[i]);
	}
    }
    free(p);
    EXPECT_EQ(0,memcmp((void*)test2,(void*)test21,512));
  
}

TEST(recorder,para_test03)
{ 
    record_section(1,52,test3,512);
    time_t t;
    t = time(0);
    timess = localtime(&t);
    init_meta_data_section((void*)test31,sizeof(test31),52);
    record(1,2,2,2,1,s1,30);

    test31[2]=1;
    test31[3]=2;
    test31[14]=1;
    record_elements *p=(record_elements*)malloc(32);
    p->year=timess->tm_year+1900;
    p->month=timess->tm_mon+1;
    p->day=timess->tm_mday;
    p->hour=timess->tm_hour;
    p->min=timess->tm_min;
    p->sec=timess->tm_sec;
    p->type=2;
    p->key=2;
    p->oper=2;
    p->len=62;
    p->num=1;
    p->first=30;
  
    char *a = "A0123456789abcdefghijklmnopqrs";

    memcpy(test31+96,p,32);   
    memcpy(test31+128,a,30);
    for(int i=0;i<512;i++)
    {
        if(test3[i]!=test31[i])
	{
		printf("%d:%c|%c\n",i,test2[i],test21[i]);
	}
    }
    free(p);
    EXPECT_EQ(0,memcmp((void*)test3,(void*)test31,512));
    
}

TEST(recorder,para_test04)
{ 
    record_section(1,52,test4,512);
    time_t t;
    t = time(0);
    timess = localtime(&t);
    init_meta_data_section((void*)test41,sizeof(test41),52);
    record(1,2,2,2,1,s1,30);
    record(1,3,3,3,3,s1,100,s2,50,s3,50);
    record(1,4,4,4,1,s2,50);
    
    test41[2]=1;
    test41[14]=1;
    test41[26]=1;
    test41[27]=5;
    test41[38]=1;
    test41[50]=1;
    test41[62]=1;
    test41[74]=1;
    test41[86]=1;
    test41[87]=2;
    
    record_elements *p=(record_elements*)malloc(512);
    record_elements *q=(record_elements*)malloc(512);
    
    p->year=timess->tm_year+1900;
    p->month=timess->tm_mon+1;
    p->day=timess->tm_mday;
    p->hour=timess->tm_hour;
    p->min=timess->tm_min;
    p->sec=timess->tm_sec;
    p->type=4;
    p->key=4;
    p->oper=4;
    p->len=82;
    p->num=1;
    p->first=50;
    
    char *b = "A0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopq";
    char *c=  "B0123456789abcdefghijklmnopqrstuvwxyz0123456789abc";
    
    memcpy(test41+460,p,32);
    memcpy(test41+460+32,c,20);
    memcpy(test41+96,c+20,30);

    memcpy(test41+96+52,b+20,10);
    
   
    q->year=timess->tm_year+1900;
    q->month=timess->tm_mon+1;
    q->day=timess->tm_mday;
    q->hour=timess->tm_hour;
    q->min=timess->tm_min;
    q->sec=timess->tm_sec;
    q->type=3;
    q->key=3;
    q->oper=3;
    q->len=240;
    q->num=3;
    q->first=100;
    
    int datalen=50;
    memcpy(test41+200,q,32);
    memcpy(test41+232,b,100);
    memcpy(test41+332,&datalen,4);
    memcpy(test41+336,c,50);
    memcpy(test41+386,&datalen,4);
    memcpy(test41+390,c,50);
    for(int i=0;i<512;i++)
    {
        if(test4[i]!=test41[i])
	{
		printf("%d:%c|%c\n",i,test2[i],test21[i]);
	}
    }
	
    free(p);
    EXPECT_EQ(0,memcmp((void*)test4,(void*)test41,512));
   
}

TEST(recorder,para_test05)
{
    record_section(1,52,test5,512);
    time_t t;
    t = time(0);
    timess = localtime(&t);
    init_meta_data_section((void*)test51,sizeof(test51),52);
    record(1,2,2,2,1,s1,30);
    record(1,3,3,3,3,s1,100,s2,50,s3,50);
    record(1,4,4,4,1,s2,50);
    record(1,5,5,5,1,s1,100);
    
    test51[2]=1;
    test51[14]=1;
    test51[26]=1;
    test51[38]=1;
    test51[50]=1;
    test51[62]=1;
    test51[74]=1;
    test51[86]=1;
    test51[15]=3;
    test51[87]=2;
    
    int datalen=50;
    char *b = "A0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopq";
    char *c="B0123456789abcdefghijklmnopqrstuvwxyz0123456789abc";
    
    record_elements *p=(record_elements*)malloc(32);
    record_elements *q=(record_elements*)malloc(32);
    
    p->year=timess->tm_year+1900;
    p->month=timess->tm_mon+1;
    p->day=timess->tm_mday;
    p->hour=timess->tm_hour;
    p->min=timess->tm_min;
    p->sec=timess->tm_sec;
    p->type=5;
    p->key=5;
    p->oper=5;
    p->len=132;
    p->num=1;
    p->first=100;
    
    q->year=timess->tm_year+1900;
    q->month=timess->tm_mon+1;
    q->day=timess->tm_mday;
    q->hour=timess->tm_hour;
    q->min=timess->tm_min;
    q->sec=timess->tm_sec;
    q->type=4;
    q->key=4;
    q->oper=4;
    q->len=82;
    q->num=1;
    q->first=50;
    
    memcpy(test51+96,c+20,30);
    memcpy(test51+148,p,32);
    memcpy(test51+180,b,100);
    memcpy(test51+304,b+72,28);
    memcpy(test51+332,&datalen,4);
    memcpy(test51+336,c,50);
    memcpy(test51+386,&datalen,4);
    memcpy(test51+390,c,50);
    memcpy(test51+460,q,32);
    memcpy(test51+460+32,c,20);
    for(int i=0;i<512;i++)
    {
        if(test5[i]!=test51[i])
	{
		printf("%d:%c|%c\n",i,test2[i],test21[i]);
	}
    }

    free(p);
    free(q);
    EXPECT_EQ(0,memcmp((void*)test5,(void*)test51,512));
    
}



TEST(recorder,para_test06)
{
    if( record_section(-1,52,test6,256) == -1 ) 
	printf("Record_Section Create Failure:section_id should be in the range of 0~127\n");
}

TEST(recorder,para_test07)
{
    if( record_section(1,256*512,test7,1024*1024) == -2 ) 
	printf("Record_Section Create Failure:block_size should be in the range of 0~65535\n");
}

TEST(recorder,para_test08)
{
    record_section(1,1,test8,256*256);
    if( record(1,6,6,6,3,s1,100,s2,50,s3,100) == -2 ) 
	printf("Record Failure:how_many_block should be in the range of 0~255\n");
}
  

TEST(recorder,para_test09)
{
    record_section(1,52,test9,256);
    if( record(1,6,6,6,3,s1,100,s2,50,s3,50) == -1 ) 
	printf("Record Failure:the total size of block is not enough for the record\n");
}

TEST(recorder,para_test10)
{ 
    record_section(1,1012,test10,1024*1024);
    init_meta_data_section((void*)test101,1024*1024,1012);
    srand((unsigned)time(NULL));
    for (int i = 0; i < sizeof(s4);i++)
    {
        s4[i] = rand() % 128;
    }
    time_t t;
    t = time(0);
    timess = localtime(&t);
    record(1,7,7,7,1,s4,20*1024);
    test101[2]=1;
    test101[3]=21;
    test101[14]=1;
    test101[26]=1;
    test101[38]=1;
    test101[50]=1;
    test101[62]=1;
    test101[74]=1;
    test101[86]=1;
    test101[98]=1;
    test101[110]=1;
    test101[122]=1;
    test101[134]=1;
    test101[146]=1;
    test101[158]=1;
    test101[170]=1;
    test101[182]=1;
    test101[194]=1;
    test101[206]=1;
    test101[218]=1;
    test101[230]=1;
    test101[242]=1;
   

    record_elements *p=(record_elements*)malloc(32);

    p->year=timess->tm_year+1900;
    p->month=timess->tm_mon+1;
    p->day=timess->tm_mday;
    p->hour=timess->tm_hour;
    p->min=timess->tm_min;
    p->sec=timess->tm_sec;
    p->type=7;
    p->key=7;
    p->oper=7;
    p->len=20512;
    p->num=1;
    p->first=20480;

    memcpy(test101+12288,p,32);  
    memcpy(test101+12320,s4,20*1024);
    
    free(p);
    EXPECT_EQ(0,memcmp((void*)test10,(void*)test101,1024*1024));
}
 




