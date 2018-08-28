#ifndef _LOG_RECORDER_H
#define _LOG_RECORDER_H
#ifdef __cplusplus
extern "C" {
#endif
#define MAX_SECTION 127    //128 sections maximumn,section id range from 0 to 127
#define MAX_BLOCK_SIZE 65535 //max integer unsighed short can represent,block size range from 1 to 65535
#define MAXLEN  10240
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

typedef struct _section
{
    unsigned int block_size;
    void *addr;
    unsigned int len;
    unsigned int next_write_address;
}section;

//if section_id is negative or greater than MAX_SECTION defined above, return -1
//if blockzz_size is not greater than 0 or greater than MAX_BLCOK_SIZE defined above, return -2
//if the section is created successfully ,return 0
int record_section(int section_id,int block_size,void *addr,int len);


//if the record is too long to be contained by the whole section,return -1
//if write the record successfully, return 0
int record(int section_id,int type,int key, int oper,int para_cnt,...);

int record_section_destory(int section_id);
int visualization(void* addr,int len,int block_size);
#ifdef __cplusplus
}
#endif
#endif
