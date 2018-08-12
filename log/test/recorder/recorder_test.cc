#include "../../gtinclude/gtest/gtest.h"
#include "../../recorder/recorder.h"

char section0[1024*1024];
TEST(recorder,para_test)
{
    unsigned short para0=1;
    unsigned int para1=2;
    EXPECT_EQ(0,record_section(0,(void*)section0,sizeof(section0)));
   EXPECT_EQ(0,record(0,0,0,0,2,&para0,sizeof(para0),&para1,sizeof(para1)));
    EXPECT_EQ(0,record_section_destory(0));
}

