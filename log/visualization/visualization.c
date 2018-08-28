#include<stdio.h>
#include <string.h>
#include <malloc.h>
#include "visualization.h"
int output(FILE* fp, record_element* re, int para_num)
{
	fprintf(fp, "%d\t", re->year);
	fprintf(fp, "%d\t", re->month);
	fprintf(fp, "%d\t", re->day);
	fprintf(fp, "%d\t", re->hour);
	fprintf(fp, "%d\t", re->min);
	fprintf(fp, "%d\t", re->sec);
	fprintf(fp, "%d\t", re->type);
	fprintf(fp, "%d\t", re->key);
	fprintf(fp, "%d\t", re->oper);
	char *initial_addr = (char*)re->data;
	int totol_len = 0;
	int para_cnt = *((int*)initial_addr);
	if (para_cnt > para_num)
		para_num = para_cnt;
	totol_len += sizeof(int);
	int para_len;
	char s[MAXLEN];
	for (int i = 0; i<para_cnt; ++i)
	{
		para_len = *((int*)(initial_addr + totol_len));
		totol_len += sizeof(int);
		memcpy(s, initial_addr + totol_len, para_len);
		totol_len += para_len;
		for (int i = 0; i<para_len; ++i) fprintf(fp, "%c", s[i]);
		fprintf(fp, "\t");
	}
	fprintf(fp, "\n");
	return para_num;//返回新的para_num
}

int visualization(void* addr, int len)
{
	char* initial_addr = (char*)addr;
	record_node* rn = (record_node*)(initial_addr);//定义指向record_node的结构体指针
	int block_size = rn->block_size;
	int n;
	int num = len / (record_node_len + block_size);//num表示分区内block总数
	int para_num = 0;//para_num用来存储最大的para_cnt
	char* filename = "out.xls";
	FILE *fp = fopen(filename, "w+");
	if (fp == NULL) return -1;

	for (n = 0; n<num;)
	{
		if (rn->in_use == 1 && rn->how_many_blocks != 0)
		{
			n += rn->how_many_blocks;
			char *record_contents = malloc(rn->how_many_blocks*block_size);//定义一个暂时存储一条记录的指针并分配内存
			int k = rn->how_many_blocks;
			for (int i = 0; i<k; ++i)
			{
				memcpy(record_contents + i * block_size, initial_addr + rn->block_offset, block_size);
				rn = (record_node*)(initial_addr + rn->next_offset);
			}
			record_element* re = (record_element*)record_contents;//定义指向record_element的结构体指针
			memcpy(re->data, record_contents + record_element_len, re->len - record_element_len);
			para_num = output(fp, re, para_num);
			free(record_contents);
		}
		else
		{
			rn = (record_node*)(initial_addr + rn->next_offset);
			n += 1;
		}

	}
	FILE *fp1 = fopen("result.xls", "a");
	if (fp1 == NULL) return -1;
	//向fp1指向的文件写入属性行
	fprintf(fp1, "year\tmonth\tday\thour\tmin\tsec\ttype\tkey\toper\t");
	for (int t = 1; t <= para_num; t++)
		fprintf(fp1, "%d\t", t);
	fprintf(fp1, "\n");
	rewind(fp);     // fp的指针已指到文件的末尾，为了完成复制，使位置指针重返回文件头 
	while (!feof(fp))
		fputc(fgetc(fp), fp1);//将fp指向的文件复制给fp1指向的文件
	fclose(fp);
	fclose(fp1);
	remove(filename);//删除fp指向的文件
	return 0;
}