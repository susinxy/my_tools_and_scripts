#include<stdio.h>
#include <string.h>
#include <malloc.h>
#include "visualization.h"
#include "decompressor.h"

int main() {
	char* filename2 = malloc(30);
	printf("请输入你要打开的文件名及路径，如c:\\temp.txt\n");
	scanf("%s", filename2);
	FILE* fp2 = fopen(filename2, "r+");
	int block_size = 52;
	fseek(fp2, 0L, SEEK_END); /* 定位到文件末尾 */
	long int flen = ftell(fp2); /* 得到文件大小 */
	char* p = (char *)malloc(flen + 1); /* 根据文件大小动态分配内存空间 */
	void* addr = (char*)malloc(5 * flen);
	decompress(p, flen, addr, &flen);
	visualization(addr, flen, block_size);
	free(p);
	free(addr);
	return 0;
}
