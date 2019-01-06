#define FCA_U32 unsigned int
const int m_chunkBits=2;
const int m_chunkBitsEncode=4;
int getV(int startOrEnd,int bitsOffset)
{
    unsigned int mask=((1UL<<m_chunkBits)-1);
    return (startOrEnd>>bitsOffset)&mask;
}
int getH(int startOrEnd,int chunkIndex,int chunkNum)
{
    return startOrEnd&(~((1UL<<(m_chunkBits*(chunkNum-chunkIndex)))-1));
}

void encodeChunks(int start,int splitChunkIndex,int vStart,int vEnd,int chunkNum,FCA_U32 &outEncode,FCA_U32 &outEncodeMask)
{
    const char  encodeArray[]={0x0,0x0,0x0,0x0,
                              0x0,0xC,0x8,0x8,
                              0x0,0x0,0xA,0x8,
                              0x0,0x0,0x0,0x9};
    const char encodeMaskArray[]={0xF,0x3,0x1,0x0,
                                  0xF,0xF,0x9,0x8,
                                  0xF,0xF,0xF,0xC,
                                  0xF,0xF,0xF,0xF};
    printf("encodding start:0x%x vstart:%d vend:%d split:%d\n",start,vStart,vEnd,splitChunkIndex);
    outEncode=0;outEncodeMask=0;
    for(int i=0,v=0;i<splitChunkIndex;i++){
        v=getV(start,m_chunkBits*(chunkNum-i-1));
        outEncode|=encodeArray[v*(1<<m_chunkBits)+v]<<(m_chunkBitsEncode*(chunkNum-i-1));
        outEncodeMask|=encodeMaskArray[v*(1<<m_chunkBits)+v]<<(m_chunkBitsEncode*(chunkNum-i-1));
    }
    outEncode|=encodeArray[vStart*(1<<m_chunkBits)+vEnd]<<(m_chunkBitsEncode*(chunkNum-splitChunkIndex-1));
    outEncodeMask|=encodeMaskArray[vStart*(1<<m_chunkBits)+vEnd]<<(m_chunkBitsEncode*(chunkNum-splitChunkIndex-1));
    printf("code %x mask %x\n",outEncode,outEncodeMask);
}
void encode(int start,int end,int chunkNum,FCA_U32 *outEncode,FCA_U32 *outEncodeMask,int &outCnt)
{
#define ADJUST_V(v,judge_value,startOrEnd,is_adjust) \
    { \
        int i; \
        is_adjust=0; \
        for(i=splitChunkIndex+1;i<chunkNum;i++){ \
            if(getV(startOrEnd,m_chunkBits*(chunkNum-i-1)) != judge_value){ \
                break; \
            } \
        } \
        if(i == chunkNum){ \
            is_adjust=1;v=getV(startOrEnd,m_chunkBits*(chunkNum-splitChunkIndex-1)); \
        } \
    }


    int splitChunkIndex,vStart,vEnd;
    
    for(splitChunkIndex=0;splitChunkIndex<chunkNum;splitChunkIndex++){
        vStart=getV(start,m_chunkBits*(chunkNum-splitChunkIndex-1));
        vEnd=getV(end,m_chunkBits*(chunkNum-splitChunkIndex-1));
        if(vStart != vEnd){
            break;
        }
    }
    splitChunkIndex=splitChunkIndex==chunkNum?chunkNum-1:splitChunkIndex;
#define SPLIT_CHUNK_BITS_OFFSET (m_chunkBits*(chunkNum-splitChunkIndex-1))
    
    if(vEnd-vStart>=2){
        int isStartAdjust,isEndAdjust;
        vStart+=1;vEnd-=1;
        ADJUST_V(vStart,0,start,isStartAdjust);
        ADJUST_V(vEnd,getV(0xFFFFFFFF,SPLIT_CHUNK_BITS_OFFSET),end,isEndAdjust);
        encodeChunks(start,splitChunkIndex,vStart,vEnd,chunkNum,outEncode[outCnt],outEncodeMask[outCnt]);
        outCnt+=1;
        if(isStartAdjust==0){
            int newEnd=vStart<<SPLIT_CHUNK_BITS_OFFSET;
            newEnd+=getH(start,splitChunkIndex,chunkNum);
            printf("newEnd: %x\n",newEnd);
            encode(start,newEnd-1,chunkNum,outEncodeMask,outEncodeMask,outCnt);
        }
        if(isEndAdjust==0){
            int newStart=(vEnd+1)<<SPLIT_CHUNK_BITS_OFFSET;
            newStart+=getH(end,splitChunkIndex,chunkNum);
            printf("newStart: %x\n",newStart);
            encode(newStart,end,chunkNum,outEncodeMask,outEncodeMask,outCnt);
        }
    }else if(vEnd-vStart==1){
        int newEdge=(vStart+1)<<SPLIT_CHUNK_BITS_OFFSET;
        newEdge+=getH(start,splitChunkIndex,chunkNum);
        printf("newEdge: %x\n",newEdge);
        encode(start,newEdge-1,chunkNum,outEncodeMask,outEncodeMask,outCnt);
        encode(newEdge,end,chunkNum,outEncodeMask,outEncodeMask,outCnt);
    }else{
        encodeChunks(start,splitChunkIndex,vStart,vEnd,chunkNum,outEncode[outCnt],outEncodeMask[outCnt]);
        outCnt+=1;
    }
}

