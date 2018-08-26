#include "../../gtinclude/gtest/gtest.h"
#include "../../recorder/recorder.h"
#include "../../compressor/compressor.h"
#include "../../decompressor/decompressor.h"
#include <stdio.h>
#include <string.h>


static char section[1024*1024];
static char compressed[sizeof(section)];

TEST(intergration,test00)
{
    int para0=0xe,para1=0xf;
    int compressed_len=0;
    FILE *log_file=0;
    EXPECT_EQ(0,record_section(0,52,(void*)section,sizeof(section)));
    EXPECT_EQ(0,record(0,0x282,0xAA,0xBB,2,&prar0,sizeof(para0),&para1,sizeof(para1)));
    EXPECT_EQ(0,compress(section,sizeof(section),compressed,&compressed_len));
    memset(section,0,sizeof(section));
    EXPECT_EQ(0,decompress(compressed,compressed_len,section,&compressed_len));
    EXOECT_EQ(sizeof(section),compressed_len);
    EXPECT_EQ(0,visualization(section,compressed_len,52));
}
  




