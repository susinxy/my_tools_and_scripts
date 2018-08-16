#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include "linkedlist.h"
#define MaxSize 128

#define record_node_len sizeof(record_node)
#define record_element_len sizeof(record_element)
typedef struct _record_element
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
    char data[0];
}record_element;

typedef struct _record_node
{
    unsigned char in_use;
    unsigned int block_offset;
    unsigned int how_many_blocks;
}record_node;

typedef struct _section
{
    int block_size;
    void *addr;
    unsigned int len;
    unsigned int next_write_address;
}section;

section table[MaxSize];

int record(int section_id,unsigned int type, unsigned int key, unsigned int oper,int para_cnt,...)
{
    va_list v_args;
    unsigned int para_len;
    int totol_len=0;//the totol length of data[0] in struct record_elemnt
    char *para;
    va_start(v_args,para_cnt);

    List *list = init_list();//create a list to record the para and para_len
    for(int i=0;i<para_cnt;i++)
    {
        para= va_arg(v_args,char*);
        para_len=va_arg(v_args,unsigned int);
        insert_to_list(para,para_len,list);
        totol_len+=para_len;
    }
    va_end(v_args);
   //join the paras together
   char *tmp=(char*)malloc(totol_len+1);
   Node *current = list->head;
   if(current!=NULL)
   {
       while(current->next != NULL)
      {
          memcpy(tmp,current->data,current->len);
          tmp+=current->len;
          current = current->next;
      }
       memcpy(tmp,current->data,current->len);
       tmp=tmp+current->len-totol_len;
       tmp[totol_len]='\0';
   }
   destroy(list);
    //here variable contents means a record which will be written into memory
    record_element *contents=(record_element*)malloc(record_element_len+totol_len+1);
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
    contents->len=record_element_len+totol_len+1;
    memcpy(contents->data,tmp,totol_len+1);
    free(tmp);
   //compute how many blocks will be covered by the record
   unsigned int block_covered;
   if(contents->len%table[section_id].block_size==0)
    block_covered=contents->len/table[section_id].block_size;
   else block_covered=contents->len/table[section_id].block_size+1;

   //mark the blocks which will be rewrite
   record_node* p=(record_node*)(table[section_id].addr);
   int i,j;
   for(i=table[section_id].next_write_address;i<block_covered;++i)
   {
       j=p[i].how_many_blocks;
       if(p[i].in_use==1&&j!=0)
       {
            while(j--)
            {
                p[i].in_use=0;
                ++i;
            }
            --i;
       }
   }
   //write memory
   char *q=(char*)(table[section_id].addr);
   memcpy(q+p[table[section_id].next_write_address].block_offset,contents,contents->len);
   //update next_write_address and record_node
   unsigned int cnt=table[section_id].len/(record_node_len+table[section_id].block_size);
   table[section_id].next_write_address=(table[section_id].next_write_address+block_covered)%cnt;
   unsigned int k=table[section_id].next_write_address;
   for(int it=0;it<block_covered;++it)
   {
       if(it==0) p[k].how_many_blocks=block_covered;
       p[(k+it)%cnt].in_use=1;
   }
   return 0;
}

int record_section(int section_id,int block_size,void *addr,int len)
{
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
        current->in_use=0;
        current->how_many_blocks=0;
        current->block_offset=record_node_len*cnt+block_size*i;
        current+=record_node_len;
    }
    return 0;
}
int record_section_destory(int section_id)
{
    //do nothing
    return 0;
}
