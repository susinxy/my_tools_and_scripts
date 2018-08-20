#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "record.h"
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
void visualization(void* addr,int length,int block_size,char* filename)
{
	FILE *fp; //定义一个文件指针
	int  num, n; //n表示正在读的block编号
	unsigned char *p, *q;
	//将传入地址赋给指针q，q用来指向record_node，p用来指向record_element
	q = (unsigned char*)addr;
	//num用来存储计算出的该分区的block总数
	num = length / (record_node_len + block_size);
	//定义两个结构体指针
	record_node *rn = (record_node*)malloc(sizeof(record_node));
	record_element *re = (record_element*)malloc(sizeof(record_element));
	//将文件指针指向record.xls文件的开头
	fp = fopen(filename, "w");

	for (n = 0; n<num;) {
		memcpy(&rn->in_use, q, sizeof(char));
		memcpy(&rn->block_offset, q, sizeof(int));
		memcpy(&rn->how_many_blocks, q, sizeof(int));
		n = n + rn->how_many_blocks;//计算下条日志的n
		p = (unsigned char*)(addr + rn->block_offset);//计算本条日志的record_element地址
		//判断是否为日志开始
		if (rn->in_use == 1 && rn->how_many_blocks != 0) {
			if (n < num)
				q = q + rn->how_many_blocks * record_node_len;
			//针对日志一部分在分区前部，一部分在分区后部的情况
			//先将该日志分区后部的内容存到r指针中，再将分区前部的内容紧接着
			//存到r指针中，再将指针r的地址赋给指针p;
			else {
				//定义一个指针r并分配内存
				char* r = (char*)malloc(sizeof(rn->how_many_blocks*block_size));
				int n1,n2,n3;
				n1 = num + rn->how_many_blocks - n;
				n2 = n - num;
				n3 = n - rn->how_many_blocks;
				q = q - sizeof((n3 - n2)*record_node_len);
				memcpy(r, p, sizeof(n1*block_size));
				p = p - n3 * block_size;
				memcpy(r+sizeof(n1*block_size), p, sizeof(n2*block_size));
				p = (unsigned char*)r;
				free(r);
			}

			//将指针p的内容存入结构体指针re
			memcpy(&re->year, p, sizeof(short));
			p = p + sizeof(short);
			memcpy(&re->month, p, sizeof(char));
			p = p + sizeof(char);
			memcpy(&re->day, p, sizeof(char));
			p = p + sizeof(char);
			memcpy(&re->hour, p, sizeof(char));
			p = p + sizeof(char);
			memcpy(&re->min, p, sizeof(char));
			p = p + sizeof(char);
			memcpy(&re->sec, p, sizeof(char));
			p = p + sizeof(char);
			memcpy(&re->type, p, sizeof(int));
			p = p + sizeof(int);
			memcpy(&re->key, p, sizeof(int));
			p = p + sizeof(int);
			memcpy(&re->oper, p, sizeof(int));
			p = p + sizeof(int);
			memcpy(&re->len, p, sizeof(int));
			p = p + sizeof(int);
			memcpy(&re->data, p, re->len - record_element_len);

			//将结构体的内容读入文件
			fprintf(fp, "%d\t%c\t%c\t%c\t%c\t%c\t%d\t%d\t%d\t%d\t%s\n", re->year,\
				re->month, re->day, re->hour, re->min, re->sec, re->type, re->key, \
				re->oper, re->len, re->data);
		}
	}
	free(rn);
	free(re);
	fclose(fp);
}
