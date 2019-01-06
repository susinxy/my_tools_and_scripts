#include <stdio.h>
#include "dirpe.cpp"


int main(int a,char * agr[])
{
    FCA_U32 code[32];
    FCA_U32 mask[32];
    int start=0x1234,end=0x5678;
    int chunkNum=4,cnt=0;
    printf("\ngetV(0x%x):\n",start);
    for(int i=0;i<chunkNum;i++){
        printf("%x ",getV(start,i*m_chunkBits));
    }
    printf("\ngetH(0x%x):\n",start);
    for(int i=0;i<chunkNum;i++){
        printf("%x ",getH(start,i,chunkNum));
    }

    chunkNum=4;start=0x10;end=0x1F;cnt=0;
    printf("\nencode(0x%x 0x%x):\n",start,end);
    encode(start,end,chunkNum,code,mask,cnt);
    for(int i=0;i<cnt;i++){
        printf("%x %x\n",code[i],mask[i]);
    }

    chunkNum=4;start=0x10;end=0x10;cnt=0;
    printf("\nencode(0x%x 0x%x):\n",start,end);
    encode(start,end,chunkNum,code,mask,cnt);
    for(int i=0;i<cnt;i++){
        printf("%x %x\n",code[i],mask[i]);
    }
    return 0;
}

    




