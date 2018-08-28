#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompressor.h"
#include "visualization.h"

#define maxn 1024*1024
unsigned char de_addr_in[maxn + 5];
unsigned char de_addr_out[maxn + 5];
int main()
{
	char* filename = malloc(30);
	printf("请输入你要打开的文件名及路径，如c:\\temp.txt\n");
	scanf("%s", filename);
	FILE* fp = fopen(filename, "r");
	unsigned char *p = de_addr_in;
	int de_len_out;
	int x;
	int de_len_in = 0;
	int block_size = 0;
	while (fscanf(fp, "%d", &x) != EOF)
	{
	//printf("%d\n",x);
	*p = x;
	p++;
	de_len_in++;
	}
	printf("%d\n", de_len_in);
	decompress(de_addr_in, de_len_in, de_addr_out, &de_len_out);
	printf("length after decompress: %d\n", de_len_out);
	visualization(de_addr_out, de_len_out, block_size);
	fclose(fp);
	free(filename);
	return 0;
}
