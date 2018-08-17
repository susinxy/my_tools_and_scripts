//
//  main_new.c
//  DataCompressionHuffman.0810
//
//  Created by xxx041 on 2018/8/16.
//  Copyright © 2018 xxx041. All rights reserved.
//

#include "main_new.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ******* Struct: Nodes in the Huffman Tree *******
typedef struct HuffmanTreeNode{
    int weight;     // Attribute: How Frequent the Character Appears in the Document
    char origin;    // Attribute: the Single Character(in ASCII code table) in Question
    struct HuffmanTreeNode *LF, *RT;    // Reference: Associated with 2 Children Nodes on the Left and Right
}HNode;


// ******* Global Variables: Maps, Sets and TempVar *******
char codeMap[256][50];      // 256: number of ASCII codes; 50: anticipated maximum length of new codes(shall be shorter than 8 as expected)
HNode LFset[100];        // 100: associated with the maximum length - maximum number of layers in the Huffman Tree
HNode RTset[100];        // the same as above, and be extremely cautious about the size!
unsigned char BinaryCode = 0;
unsigned char tempChar;


// ******* Function SORT: Rearrange the Nodes Contigent on Weight - Descending *******
void SORT(HNode* node, int length){
    int i=0, j=0;
    HNode temp;
    for(i=0;i<length-1;i++){
        for(j=0;j<length-i-1;j++){
            if(node[j].weight<=node[j+1].weight){
                temp=node[j+1];
                node[j+1]=node[j];
                node[j]=temp;
            }
        }
    }
};


// ******* Function TREEBUILDING: Construct the Huffman Tree *******
void TREEBUILDING(HNode* node, int length){
    if(length==1) return;
    else{
        // rearrange the nodes and prioritize ones with greater weights or shorter lifespan
        SORT(node, length);
        HNode parent;
        LFset[length]=node[length-2];
        RTset[length]=node[length-1];
        parent.LF=&LFset[length];
        parent.RT=&RTset[length];
        parent.weight=parent.LF->weight+parent.RT->weight;
        node[length-2]=parent;
        // iteration of the function until it has been reduced to length 1(greatest sum)
        TREEBUILDING(node, length-1);
    }
};


// ******* Function ENCODING: Encode the Terminal Nodes with Huffman Codes *******
void ENCODING(HNode *node, char *curCypher, int length){
    if(node->LF==NULL || node->RT==NULL){
        curCypher[length]='\0';
        strcpy(codeMap[node->origin-0], curCypher);
        return;
    }
    curCypher[length]='0';
    ENCODING(node->LF, curCypher, length+1);
    curCypher[length]='1';
    ENCODING(node->RT, curCypher, length+1);
};


// ******* Function UNZIPPING: Decode the Binary Codes into ASCII Codes *******
HNode* UNZIPPING(HNode *node, int flag){
    if(flag==0) return node->LF;    // Acquire the Digit of 0 = Go Left
    else if(flag==1) return node->RT;   // Otherwise Go Right
    else return NULL;   // In Case of Bizarre Output: Return NULL
}



// ******* MAIN FUNCTION: File Reading, Encoding and Decoding *******
int main(){
    // ******* LOCAL VARIABLES in Main Function *******
    // #REFRESHABLEMEDIATES#
    int i=0;                    // Iteration Variable
    int num=0;                  // Counting Variable
    
    // #FILEREADING#
    int countASCII[256]={0};    // Counting the Amount of Characters Appearing in the File
    char rawChar;               // Input Character at Each Node
    int oriLength=0;            // Counting of the Original Text's Length
    
    // #ENCODING#
    HNode node[256];            // HNode Set for Building the Huffman Tree
    for(i=0;i<256;i++){
        node[i].LF=NULL;
        node[i].RT=NULL;
        node[i].weight=0;
    }                           // Initializing: Nullify All Children Pointers of the Primal HNode Set
    char tempCypher[50];        // Huffman Code for Each Round of Encoding Process | Refreshable
    
    // #DECOMPRESSION#
    int op=128;                 // '1000 0000': Binary Code for AND Operation
    HNode *z;                   // HNode Variable for Searching Associated Character from Binary Codes
    int zipLength=0;            // Counting of the Decompressed Text's Length
    
    
    // ******* File Operations | TO BE MENDED FOR READING FROM ADDR *******
    FILE *fpr=fopen("/Users/xxx041/Desktop/DataCompressionHuffman.0810/DataCompressionHuffman.0810/testing7.txt","r");
    FILE *fpw=fopen("/Users/xxx041/Desktop/DataCompressionHuffman.0810/DataCompressionHuffman.0810/compressed7.txt","wb");
    FILE *fpr1=fopen("/Users/xxx041/Desktop/DataCompressionHuffman.0810/DataCompressionHuffman.0810/compressed7.txt","rb");
    FILE *fpw1=fopen("/Users/xxx041/Desktop/DataCompressionHuffman.0810/DataCompressionHuffman.0810/decompressed7.txt","w");
    
    
    // ******* COUNTING THE NUM OF CHARACTERS *******
    while((rawChar=fgetc(fpr))!=EOF){
        oriLength++;            // Length of All Recorded Characters, Some of Which Cannot Embody
        countASCII[rawChar-0]++;
    }
    
    
    // ******* LOADING CHARACTERS INTO NODES AT THE BOTTOM *******
    num=0;
    for(i=0;i<256;i++){
        if(countASCII[i]!=0){
            node[num].origin=i;
            node[num].weight=countASCII[i];
            num++;
        }
    }
    
    
    // ******* HUFFMAN TREE BUILDING *******
    TREEBUILDING(node,num);
    // ******* ENCODING *******
    ENCODING(&node[0], tempCypher, 0);

    
    // ******* COMPRESSION *******
    num=0;
    fseek(fpr,0L,0);    //  Reset the Pointer to the Beginning of the File to Be Compressed
    while((rawChar=fgetc(fpr))!=EOF){
        for(i=0;i<strlen(codeMap[rawChar-0]);i++){
            BinaryCode|=codeMap[rawChar-0][i]-'0';
            num++;
            if(num==8){
                fwrite(&BinaryCode,sizeof(char),1,fpw);
                zipLength++;
                BinaryCode=0;
                num=0;
            }
            else{
                BinaryCode=BinaryCode<<1;
            }
        }
    }
    if(num!=8){
        BinaryCode=BinaryCode<<(8-num);
        fwrite(&BinaryCode,sizeof(char),1,fpw);
        zipLength++;
    }
    fclose(fpr);
    fclose(fpw);
    
    // ******* DECOMPRESSION *******
    num=0;
    z=&node[0];
    while(fread(&rawChar,sizeof(char),1,fpr1)){
        if(num==oriLength) break;
        op=128;
        for(i=0;i<8;i++){
            tempChar=rawChar&op;
            rawChar=rawChar<<1;
            tempChar=tempChar>>7;
            z=UNZIPPING(z,tempChar-0);
            if(z->LF==NULL || z->RT==NULL){
                fprintf(fpw1,"%c",z->origin);
                num++;
                z=&node[0];
            }
        }
    }
    fclose(fpr1);
    fclose(fpw1);
    
    
    printf("Original Size: %d B \n", oriLength);
    printf("Compression Success! Compressed Size: %d B\n", zipLength);
    printf("Decompression Success! Resumed Size: %d B\n", num);
    printf("Compression Efficiency :%.2f %% \n", (float)(oriLength-zipLength)/oriLength*100);
    
    return 0;
}
