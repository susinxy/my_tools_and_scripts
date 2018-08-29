#ifndef _LOG_RECORDER_H
#define _LOG_RECORDER_H
#ifdef __cplusplus
extern "C" {
#endif
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
		unsigned short block_size;
		unsigned char in_use;
		unsigned char how_many_blocks;
		unsigned int next_offset;
		unsigned int block_offset;

	}record_node;

	int visualization(void* addr, int len);
#ifdef __cplusplus
}
#endif
#endif