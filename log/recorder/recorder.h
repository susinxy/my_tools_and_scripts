#ifndef _LOG_RECORDER_H
#define _LOG_RECORDER_H

#ifdef __cplusplus
extern "C" {
#endif
int record(int section_id,int type, int key, int oper,int para_cnt,...);
int record_section(int section_id,int block_size,void *addr,int len);
int record_section_destory(int section_id);
#ifdef __cplusplus
}
#endif
#endif
