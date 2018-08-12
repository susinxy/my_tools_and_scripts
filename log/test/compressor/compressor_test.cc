#include "../../gtinclude/gtest/gtest.h"
#include "../../compressor/compressor.h"

char addr_in[1024*1024];
char addr_out[sizeof(addr_in)];
TEST(compressor,para_test)
{
    int len_out;
    EXPECT_EQ(0,compress((void*)addr_in,sizeof(addr_in),(void**)addr_out,&len_out));
    EXPECT_LT(len_out,sizeof(addr_in));
}

