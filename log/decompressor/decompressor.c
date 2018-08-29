#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompressor.h"
#include <time.h>

// ****** Global Variables ******
static char codes[MAX_CHAR + 5][MAX_TREE_HT + 5];  // within the range of ASCII codes(MAX_CHAR = 256), and the length of tree height maximized to 256;



// ****** Struct Denotation ******
// MinHeapSorting: MinHeapNode
struct MinHeapNode{
    unsigned char data;                 // storage of the character stored at the node
    int freq;                           // amount of the character that appears in the original text
    struct MinHeapNode *left, *right;       // linking to left and right child node of the node in question
};
// MinHeapSorting: MinHeap
struct MinHeap{
    int size;                       // the current size of the min heap
    int capacity;                   // capacity of the min heap
    struct MinHeapNode** array;        // array of minheap node pointers
};



// ****** Function Bundle ******
// *MinHeap* FUNC newNode: initialize the new node with character it stores and how frequently it has appeared in the given text
static struct MinHeapNode* newNode(unsigned char data, int freq){
    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->data = data;
    temp->left = temp->right = NULL;
    temp->freq = freq;
    return temp;
};
// *MinHeap* FUNC createMinHeap: construct the Min Heap
static struct MinHeap* createMinHeap(int capacity){
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array= (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
// *MinHeap* FUNC swapMinHeapNode: switch the position of two MinHeap nodes
static void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){
    struct MinHeapNode* mediate = *a;
    *a = *b;
    *b = mediate;
}
// *MinHeap* FUNC minHeapify
static void minHeapify(struct MinHeap* minHeap, int index){
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if(left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq) smallest = left;
    if(right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq) smallest = right;
    // reiterate the operation until the smallest node is placed at the root
    if (smallest != index){
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[index]);
        minHeapify(minHeap, smallest);
    }
}
// *MinHeap* FUNC isSizeOne: check if the size of the heap is 1 or not
static int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1) ;   // return 1 if the heap size surely dwindles to 1
}
// *MinHeap* FUNC extractMin: the obtain the node with the minimum value from the heap
static struct MinHeapNode* extractMin(struct MinHeap* minHeap){
    struct MinHeapNode* temp = minHeap->array[0];   // taking the least frequently appearing node from the heap
    minHeap->array[0] = minHeap->array[minHeap->size - 1];  // replacing the previoius root spot with the largest leaf
    --minHeap->size;    // shrink the heap size 1 unit smaller
    minHeapify(minHeap, 0);     // rearrage the heap and sink the leaf node to the leaf again
    return temp;    // new heap with size (n - 1)
}
// *MinHeap* FUNC insertMinHeap: insert new nodes into the heap
static void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode){
    ++minHeap->size;        // size incrementing to 1 unit larger
    int i = minHeap->size - 1;          // placing the node firstly to the largest leaf
    while(i && minHeapNode->freq < minHeap->array[(i-1)/2]->freq){
        minHeap->array[i] = minHeap->array[(i-1)/2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}
// *MinHeap* FUNC buildMinHeap: build up the min heap
static void buildMinHeap(struct MinHeap* minHeap){
    int n = minHeap->size - 1;
    for(int i = (n-1)/2; i >= 0; --i) minHeapify(minHeap, i);
}
// *MinHeap* FUNC isLeaf: check whether the node is a leaf node(considering the children nodes)
static int isLeaf(struct MinHeapNode* root){
    return !(root->left) && !(root->right);
}
// *MinHeap* FUNC createAndBuildMinHeap: build a min heap of capacity and insert all characters from data[] into the heap
static struct MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size){
    struct MinHeap* minHeap = createMinHeap(size);
    for(int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}



// *HuffmanTree* FUNC buildHuffmanTree: construct the huffman tree
static struct MinHeapNode* buildHuffmanTree(unsigned char data[], int freq[], int size){
    struct MinHeapNode *left, *right, *top;
    // #1 create a min heap with the capacity equal to size
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
    // iterate until the size becomes 1(flag)
    while(!isSizeOne(minHeap)){
        // #2 obtain the 2 least frequent nodes from the Min Heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        // #3 create internal nodes that are sums-up of 2 children nodes
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
    // #4 finally obtain the least frequent root node
    return extractMin(minHeap);
}
// *HuffmanTree* FUNC acquireCodes: encode the characters with the tree built
static void acquireCodes(struct MinHeapNode* root, int arr[], int top){
    if (root->left){
        arr[top] = 0;
        acquireCodes(root->left, arr, top + 1);
    }
    if (root->right){
        arr[top] = 1;
        acquireCodes(root->right, arr, top + 1);
    }
    if(isLeaf(root)){
        // when reached the leaf node, designate the value of Huffman codes to the global variable: codes[][]
        for(int i=0;i<top;++i)
            codes[root->data][i] = arr[i] + '0';
        codes[root->data][top] = '\0';  // the "floor" rank of the node infers the length of its huffman code
    }
}
// *HuffmanTree* FUNC HuffmanCodes: build the huffman and acquire its huffman codes
static void HuffmanCodes(unsigned char data[], int freq[], int size){
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);      // the array of characters and their frequencies as well as maximum size
    int arr[MAX_TREE_HT], top = 0;
    acquireCodes(root, arr, top);
}
static void integer_to_binary(unsigned char s[], unsigned char x)//binary string(8 bits) to an integer(0-255)
{
       for(int i=READ_LEN-1;i>=0;i--)
       {
           s[i]=(x%2==0?'0':'1');
           x/=2;
       }
       s[READ_LEN]='\0';
}

int decompress(void *addr_in,int len_in, void* addr_out, int *len_out)
{
    unsigned char arr[MAX_CHAR+5];//valid characters that have benn coded
    int cnt[MAX_CHAR+5];//the frequncy of valid characters
    int size=0;

    unsigned char* read_addr=addr_in;
    unsigned char s[READ_LEN+5];

    size=*(int*)read_addr;
    read_addr+=sizeof(int);
    for(int i=0;i<size;++i)
    {
        arr[i]=*read_addr;
        read_addr++;
        cnt[i]=*((int*)read_addr);
        read_addr+=sizeof(int);
    }
    HuffmanCodes(arr,cnt,size);//acquire the huffman codes
    char *decompress_contents=malloc(len_in*READ_LEN);
    //*************** acquire the decompress_contents in binary form**************//
    len_in-=sizeof(int)+size*(sizeof(int)+sizeof(unsigned char));
    unsigned char remainder=*read_addr;
    read_addr++;
    for(int i=0;i<len_in-2;++i)
    {
          integer_to_binary(s,*read_addr);
          memcpy(decompress_contents+i*READ_LEN,s,READ_LEN);
          read_addr++;
    }
    integer_to_binary(s,*read_addr);
    memcpy(decompress_contents+(len_in-2)*READ_LEN,s,remainder);
    decompress_contents[(len_in-2)*READ_LEN+remainder]='\0';

    //********** decode: change the binary form to integer form and write memory
    unsigned char* write_addr=addr_out;
    int write_len=0;
    int covered=0;
    int len=0;
    int total_len=strlen(decompress_contents);
    while(covered<total_len)
    {
        for(int i=0;i<MAX_CHAR;++i)
        {
            len=strlen(codes[i]);
            if(len==0) continue;
            else if(memcmp(decompress_contents+covered,codes[i],len)==0)
            {
                *write_addr=i;
                write_addr++;
                covered+=len;
                write_len++;
                break;
            }
        }
    }
    *len_out=write_len;
    free(decompress_contents);
    return 0;
}
