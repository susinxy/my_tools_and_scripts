#include "../../gtinclude/gtest/gtest.h"
#include "../../recorder/recorder.h"
#include <stdarg.h>
#include <string.h>

char section0[1024*1024];
char section1[1024*1024];
void init_meta_data_section(void *addr,int len,int block_size)
{
    int node_cnt = len/(sizeof(record_node)+block_size);
    int i;
    record_node *node =(record_node*)addr;
    for(i=0;i<node_cnt;i++)
    {
        node[i].in_use=0;
        node[i].block_offset=i*block_size+node_cnt*sizeof(record_node);
        node[i].how_many_blocks=0;
    }
}

TEST(recorder,para_test)
{
    unsigned short para0=1;
    unsigned int para1=2;
    EXPECT_EQ(0,record_section(0,1024,(void*)section0,sizeof(section0)));
    init_meta_data_section((void*)section1,sizeof(section1),1024);
    EXPECT_EQ(0,memcmp((void*)section0,(void*)section1,sizeof(section1)));
    EXPECT_EQ(0,record_section_destory(0));
}

