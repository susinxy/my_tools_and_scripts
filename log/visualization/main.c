#include<stdio.h>
#include <string.h>
#include <malloc.h>
#include "visualization.h"
#include "decompressor.h"

int main() {
	char* filename2 = malloc(30);
	printf("请输入你要打开的文件名及路径，如c:\\temp.txt\n");
	scanf("%s", filename2);
	FILE* fp2 = fopen(filename2, "r");
	int len = 0, block_size = 52;
	char* addr = malloc(40);
	char* buffer=malloc(256);
	int count = fread(buffer, 1, sizeof(buffer), fp2);
	decompress(buffer, count, addr, &len);
	visualization(addr, len, block_size);
	return 0;
}
