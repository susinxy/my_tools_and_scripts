
#include "../../gtinclude/gtest/gtest.h"
#include "../../recorder/recorder.h"
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <malloc.h>

char test[512];
char test1[512];
char s[200]="A0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz";
char n[100]="C0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz";
char l[200]="A0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz";
char filename[50]= "/home/zby/my_tools_and_scripts/log/out.txt";
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

void Print(record_node *p)
{
    for(int i=0;i<8;++i)
    {
        printf("%d ",p[i].next_offset);
        printf("%d ",p[i].block_offset);
        printf("%d ",p[i].in_use);
        printf("%d\n",p[i].how_many_blocks);
    }
    printf("\n");
}

void init_meta_data_section(void *addr,int len,int block_size)
{
    int node_cnt = len/(sizeof(record_node)+block_size);
    int i;
    record_node *node =(record_node*)addr;
    for(i=0;i<node_cnt;i++)
    {
        node[i].in_use=0;
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
    EXPECT_EQ(0,record_section(0,52,(void*)test,sizeof(test)));
    init_meta_data_section((void*)test1,sizeof(test1),52);
    EXPECT_EQ(0,memcmp((void*)test,(void*)test1,sizeof(test)));
    EXPECT_EQ(0,record_section_destory(0));
}

TEST(recorder,para_test01)//record_section:512%(54+12) != 0
{
    EXPECT_EQ(0,record_section(0,54,(void*)test,sizeof(test)));
    init_meta_data_section((void*)test1,sizeof(test1),54);
    EXPECT_EQ(0,memcmp((void*)test,(void*)test1,sizeof(test)));
    EXPECT_EQ(0,record_section_destory(0));
}

TEST(recorder,para_test02)
{ 
    record_section(1,52,test,512);
    time_t t;
    t = time(0);
    timess = localtime(&t);
    init_meta_data_section((void*)test1,sizeof(test1),52);
    record(1,1,1,1,1,s,4);

    test1[0]=1;
    test1[2]=1;
    record_elements *p=(record_elements*)malloc(52);

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

    memcpy(test1+96,p,32);  
    memcpy(test1+96+32,a,4);
    free(p);
    EXPECT_EQ(0,memcmp((void*)test,(void*)test1,132));
}

TEST(recorder,para_test03)
{ 
    record_section(1,52,test,512);
    time_t t;
    t = time(0);
    timess = localtime(&t);
    init_meta_data_section((void*)test1,sizeof(test1),52);
    record(1,2,2,2,1,s,30);
    test1[0]=1;
    test1[2]=2;
    test1[12]=1;
    record_elements *p=(record_elements*)malloc(116);
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

    memcpy(test1+96,p,32);   
    memcpy(test1+96+32,a,30);

    free(p);
    EXPECT_EQ(0,memcmp((void*)test,(void*)test1,158));
    
}

/*TEST(recorder,para_test04)
{ 
    record_section(1,52,test,512);
    time_t t;
    t = time(0);
    timess = localtime(&t);
    init_meta_data_section((void*)test1,sizeof(test1),52);
    record(1,2,2,2,1,s,30);
    record(1,2,3,3,3,s,100,n,50,l,50);
    test1[0]=1;
    test1[2]=2;
    test1[12]=1;
    test1[24]=1;
    test1[26]=5;
    test1[36]=1;
    test1[48]=1;
    test1[60]=1;
    test1[72]=1;


    record_elements *p=(record_elements*)malloc(512);
    record_elements *q=(record_elements*)malloc(512);

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
    memcpy(test1+96,p,32);    
    memcpy(test1+96+32,a,30);

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

    char *b = "A0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyzB0123456789abcdefghijklmnop";
    char *c="A0123456789abcdefghijklmnopqrstuvwxyz0123456789abc";
    int datalen=50;
    memcpy(test1+96,p,32);    
    memcpy(test1+96+32,b,100);
    memcpy(test1+96+32+100,&datalen,4);
    memcpy(test1+96+32+100+4,c,50);
    memcpy(test1+96+32+100+4+50,&datalen,4);
    memcpy(test1+96+32+100+4+50+4,c,50);

    free(p);
    EXPECT_EQ(0,memcmp((void*)test,(void*)test1,));
    
}
TEST(recorder,para_test02)
{   
    EXPECT_EQ(0,record_section(1,52,test,512));
    init_meta_data_section((void*)test1,sizeof(test1),52);
    record_node *p=(record_node*)test1;
  
    record(1,1,1,1,1,s,4);
    
    EXPECT_EQ(0,memcmp((void*)test,(void*)test1,132));
 
    visualization(test,512,52,filename);
    
    record(1,2,2,2,1,s,30);
    Print(p);
    visualization(test,512,52,filename);
 
    record(1,3,3,3,2,s,70,l,15);
    Print(p);
    visualization(test,512,52,filename);
    
    record(1,4,4,4,3,s,70,n,70,l,15);
    Print(p);
    visualization(test,512,52,filename);
    
    record(1,5,5,5,1,s,72);
    Print(p);
    visualization(test,512,52,filename);
    
    record(1,6,6,6,1,l,20);
    Print(p);
    visualization(test,512,52,filename);
    
    record(1,7,7,7,1,s,72);
    Print(p);
    visualization(test,512,52,filename);
}*/



