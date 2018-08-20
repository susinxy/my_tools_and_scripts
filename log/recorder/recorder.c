#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "recorder.h"
#include <malloc.h>
#define record_node_len sizeof(record_node)
#define record_element_len sizeof(record_element)
section table[MAXSIZE];
int record(int section_id,int type,int key,int oper,int para_cnt,...)
{
    char tmp[MAXLEN];
    unsigned int totol_len=0;
    va_list v_args;

    unsigned int para_len;
    char *para;

    va_start(v_args,para_cnt);
    for(int i=0;i<para_cnt;i++)
    {
        para= va_arg(v_args,char*);
        para_len=va_arg(v_args,unsigned int);
        memcpy(tmp+totol_len,para,para_len);
        totol_len+=para_len;
    }
    va_end(v_args);

    char record_contents[MAXLEN];
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
    contents->len=record_element_len+totol_len;
    memcpy(contents->data,tmp,totol_len);
   //compute how many blocks will be covered by the record
   unsigned int block_covered;
   if(contents->len%table[section_id].block_size==0)
    block_covered=contents->len/table[section_id].block_size;
   else block_covered=contents->len/table[section_id].block_size+1;

   //mark the blocks which will be rewrite
   char *initial_addr=(char *)table[section_id].addr;
   record_node *p=(record_node*)(initial_addr+table[section_id].next_write_address);
   char *q=(char*)contents;
   for(int i=0;i<block_covered;++i)
   {
           //write memory
           memcpy(initial_addr+p->block_offset,q,table[section_id].block_size);
           q+=table[section_id].block_size;
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
int output(char* filename, record_element* re)
{
	FILE *fp=fopen(filename, "a");
	//将结构体的内容读入文件
	if(fp==NULL) return -1;
	fprintf(fp,"%d\t",re->year);
	fprintf(fp,"%d\t",re->month);
	fprintf(fp,"%d\t",re->day);
	fprintf(fp,"%d\t",re->hour);
	fprintf(fp,"%d\t",re->min);
	fprintf(fp,"%d\t",re->sec);
	fprintf(fp,"%d\t",re->type);
	fprintf(fp,"%d\t",re->key);
	fprintf(fp,"%d\t",re->oper);
	fprintf(fp,"%s\n",re->data);
	fclose(fp);
	return 0;
}

int visualization(void* addr,int len,int block_size,char* filename)
{
    unsigned int n,num = len / (record_node_len + block_size);
    char* initial_addr=(char*)addr;
    record_node* rn=(record_node*)(initial_addr);
    int flag=1;
    
    for(n=0;n<num;)
    {
            if(rn->in_use==1&&rn->how_many_blocks!=0)
            {
                n+=rn->how_many_blocks;
                
                char *record_contents=malloc(rn->how_many_blocks*block_size);
                int k=rn->how_many_blocks;
                for(int i=0;i<k;++i)
                {
                    
                    memcpy(record_contents+i*block_size,initial_addr+rn->block_offset,block_size);
                    rn=(record_node*)(initial_addr+rn->next_offset);
                }
                flag=0;
                record_element* re=(record_element*)record_contents;
                memcpy(re->data,record_contents+record_element_len,re->len-record_element_len);
                output(filename,re);
                free(record_contents);
            }
            else
            {
                    if(flag)
                    {
                        rn=(record_node*)(initial_addr+rn->next_offset);
                    }
                    n+=1;
            }
    }
    free(rn);
    return 0;
}

