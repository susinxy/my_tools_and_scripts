#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "recorder.h"
#include <malloc.h>

section table[MAX_SECTION];
char tmp[MAXLEN];
char record_contents[MAXLEN];
int record(int section_id,int type,int key,int oper,int para_cnt,...)
{
    unsigned int total_len=0;
    va_list v_args;
    unsigned int para_len;
    char *para;
    *((int*)tmp)=para_cnt;
    total_len+=sizeof(int);
    va_start(v_args,para_cnt);
    for(int i=0;i<para_cnt;i++)
    {
        para= va_arg(v_args,char*);
        para_len=va_arg(v_args,unsigned int);

        *((int*)(tmp+total_len))=para_len;

        total_len+=sizeof(unsigned int);
        memcpy(tmp+total_len,para,para_len);
        total_len+=para_len;
    }
    va_end(v_args);

    //here variable contents means a record which will be written into memory
    record_element *contents=(record_element*)record_contents;
    //Acquire system time
    struct tm *times;
    time_t t;
    t = time(0);
    times = localtime(&t);
    contents->year=times->tm_year+1900;
    contents->month=times->tm_mon+1;
    contents->day=times->tm_mday;
    contents->hour=times->tm_hour;
    contents->min=times->tm_min;
    contents->sec=times->tm_sec;
    contents->type=type;
    contents->key=key;
    contents->oper=oper;
    contents->len=record_element_len+total_len;
    memcpy(contents->data,tmp,total_len);
    char *yu=record_contents+contents->len;
    for(int i=0;i<MAXLEN-contents->len;++i) yu[i]='\0';
    //if the record is too long to be contained by the whole section,then return -1
   int cnt=table[section_id].len/(table[section_id].block_size+record_node_len);
   if(contents->len>cnt*table[section_id].block_size) return -1;
    //compute how many blocks will be covered by the record
   unsigned int block_covered;
   if(contents->len%table[section_id].block_size==0)
    block_covered=contents->len/table[section_id].block_size;
   else block_covered=contents->len/table[section_id].block_size+1;
    //if the record occupies 256 blocks or even more,return -2
   if(block_covered>MAX_BLOCK_COVERED) return -2;

   //mark the blocks which will be rewrite
   char *initial_addr=(char *)table[section_id].addr;
   record_node *p=(record_node*)(initial_addr+table[section_id].next_write_address);
   char *q=(char*)contents;
   for(int i=0;i<block_covered;++i)
   {
           //write memory
           memcpy(initial_addr+p->block_offset,q+table[section_id].block_size*i,table[section_id].block_size);
           //update record_node information
           if(i==0) p->how_many_blocks=block_covered;
           else p->how_many_blocks=0;
           p->in_use=1;
           table[section_id].next_write_address=p->next_offset;
           p=(record_node*)(initial_addr+table[section_id].next_write_address);
   }
   return 0;
}

int record_section(int section_id,int block_size,void *addr,int len)
{
    if(section_id<0||section_id>MAX_SECTION) return -1;
    if(block_size<=0||block_size>MAX_BLOCK_SIZE) return -2;
    //record the information about the section
    table[section_id].block_size=block_size;
    table[section_id].addr=addr;
    table[section_id].len=len;
    table[section_id].next_write_address=0;
    //compute how many blocks the section will be divided into
    unsigned int cnt=len/(record_node_len+block_size);
    record_node *current=(record_node*)addr;
    //initialize record_node information
    for(int i=0;i<cnt;++i)
    {
        current->block_size=block_size;
        current->in_use=0;
        current->how_many_blocks=0;
        current->block_offset=record_node_len*cnt+block_size*i;
        if(i==cnt-1)
        {
            current->next_offset=0;
        }
        else
        {
            current->next_offset=record_node_len*(i+1);
        }
        current+=1;
    }
    return 0;
}
int record_section_destory(int section_id)
{
    //do nothing
    return 0;
}
