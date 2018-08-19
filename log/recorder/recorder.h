#ifndef _LOG_RECORDER_H
#define _LOG_RECORDER_H
#ifdef __cplusplus
extern "C" {
#endif
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

int record(int section_id,int type, int key, int oper,int para_cnt,...);
int record_section(int section_id,int block_size,void *addr,int len);
int record_section_destory(int section_id);

void visualization(void* addr,int length,int block_size,char* filename);
#ifdef __cplusplus
}
#endif
#endif

