#include <stdarg.h>
#include <stdio.h>
typedef struct _record_element{
    unsigned short year;
    unsigned char month;
    unsigned char day;
    unsigned char hour;
    unsigned char min;
    unsigned char sec;
    unsigned int class;
    unsigned int key;
    unsigned int oper;
    unsigned int len;
    char data[0];
}record_element;

typedef struct _record_node{
    unsigned short node_len;
    unsigned char in_use;
    unsigned int next_offset;
}record_node;
    
int record(unsigned int section,unsigned int type, unsigned int key, unsigned int oper,int para_cnt,...)
{
    va_list v_args;
    unsigned int para_len;
    int i,j;
    char * para;

    va_start(v_args,para_cnt);
    for(j=0;j<para_cnt;j++)
    {
        para= va_arg(v_args,char*);
        para_len=va_arg(v_args,unsigned int);
        printf("para %d: ",j);
        for(i=0;i<para_len;i++)
        {
            printf("%2x ",para[i]);
        }
        printf("\n");
    }
    va_end(v_args);
        
    return 0;
}

int record_section(int section_id,void *addr,int len)
{
    return 0;
}
int record_section_destory(int section_id)
{
    return 0;
}
