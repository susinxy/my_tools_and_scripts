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
   printf("%d ",p->block_offset);
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
   unsigned int x=p[table[section_id].next_write_address].block_offset;
   if(x+(contents->len)>table[section_id].len)
   {
      memcpy(q+x,contents,table[section_id].len-x);
      memcpy(q,contents+table[section_id].len-x,contents->len+x-table[section_id].len);
   }
   else memcpy(q+x,contents,contents->len);

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
	fp = fopen(filename , "w");

	for (n = 0; n < num;) {
		memcpy(&rn->in_use, q, sizeof(char));
		memcpy(&rn->block_offset, q, sizeof(int));
		memcpy(&rn->how_many_blocks, q, sizeof(int));
		p = (unsigned char*)addr + rn->block_offset;//计算本条日志的record_element地址
		//判断是否为日志开始
		if (rn->in_use == 1 && rn->how_many_blocks != 0) {
			n = n + rn->how_many_blocks;//计算下条日志的开始record_node编号n
			if (n <= num)
				q = q + rn->how_many_blocks * record_node_len;
			//针对日志一部分在分区前部，一部分在分区后部的情况
			//先将该日志分区后部的内容存到r指针中，再将分区前部的内容紧接着
			//存到r指针中，再将指针r的地址赋给指针p;
			else {
				//定义一个指针r并分配内存
				char* r = (char*)malloc(sizeof(rn->how_many_blocks*block_size));
				int n1, n2, n3;
				n1 = num + rn->how_many_blocks - n;
				n2 = n - num;
				n3 = n - rn->how_many_blocks;
				memcpy(r, p, sizeof(n1*block_size));
				p = p - n3 * block_size;
				memcpy(r + sizeof(n1*block_size), p, sizeof(n2*block_size));
				p = r;
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
			fprintf(fp, "%d\t%c\t%c\t%c\t%c\t%c\t%d\t%d\t%d\t%d\t%s\n", re->year, \
				re->month, re->day, re->hour, re->min, re->sec, re->type, re->key, \
				re->oper, re->len, re->data);
		}
		else
			n = n + 1;
	}
	free(rn);
	free(re);
	fclose(fp);
}

