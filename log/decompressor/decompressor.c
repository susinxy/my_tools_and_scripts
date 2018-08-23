#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompressor.h"
// A Huffman tree node
static unsigned char codes[MAX_CHARACTER+5][MAX_TREE_HT+5];

 struct MinHeapNode
{
    // One of the input characters
    unsigned char data;
    // Frequency of the character
    unsigned freq;
    // Left and right child of this node
    struct MinHeapNode *left, *right;
};

// A Min Heap:  Collection of
// min heap (or Hufmman tree) nodes
 struct MinHeap
{
    // Current size of min heap
    unsigned size;
    // capacity of min heap
    unsigned capacity;
    // Attay of minheap node pointers
    struct MinHeapNode** array;
};

// A utility function allocate a new
// min heap node with given character
// and frequency of the character
static struct MinHeapNode* newNode(unsigned char data, unsigned freq)
{
    struct MinHeapNode* temp= (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// A utility function to create
// a min heap of given capacity
static struct MinHeap* createMinHeap(unsigned capacity)
{

    struct MinHeap* minHeap= (struct MinHeap*)malloc(sizeof(struct MinHeap));
    // current size is 0
    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array= (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

// A utility function to
// swap two min heap nodes
static void swapMinHeapNode(struct MinHeapNode** a,struct MinHeapNode** b)
{

    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
// The standard minHeapify function.
static void minHeapify(struct MinHeap* minHeap, int idx)
{

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check
// if size of heap is 1 or not
static int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}
// A standard function to extract
// minimum value node from heap
static struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]= minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// A utility function to insert
// a new node to Min Heap
static void insertMinHeap(struct MinHeap* minHeap,struct MinHeapNode* minHeapNode)
{

    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// A standard funvtion to build min heap
static void buildMinHeap(struct MinHeap* minHeap)
{

    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}


// Utility function to check if this node is leaf
static int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
static struct MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size)

{

    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// The main function that builds Huffman tree
static struct MinHeapNode* buildHuffmanTree(unsigned char data[], int freq[], int size)

{
    struct MinHeapNode *left, *right, *top;

    // Step 1: Create a min heap of capacity
    // equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap))
    {

        // Step 2: Extract the two minimum
        // freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Step 3:  Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // Step 4: The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
// It uses arr[] to store codes
static void acquireCodes(struct MinHeapNode* root, int arr[], int top)

{

    // Assign 0 to left edge and recur
    if (root->left)
    {
        arr[top] = 0;
        acquireCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right)
    {
        arr[top] = 1;
        acquireCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]

    /*//////////////////////////////////////////////////////what if top>MAX_TREE_HT/////////////////////////////*/
    if (isLeaf(root))
    {

        for(int i=0;i<top;++i)
        {
            codes[root->data][i]=arr[i]+'0';
        }
        codes[root->data][top]='\0';
    }
}

// The main function that builds a
// Huffman Tree and print codes by traversing
// the built Huffman Tree
static void HuffmanCodes(unsigned char data[], int freq[], int size)

{
    // Construct Huffman Tree
    struct MinHeapNode* root= buildHuffmanTree(data, freq, size);

    // Print Huffman codes using
    // the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;

    acquireCodes(root, arr, top);
}

static int getLen(const unsigned char s[])
{
    int nLen = 0;
    const unsigned char* p = s;
    while(*p!=0){
        nLen++;
        p++;
    }
    return nLen;
}
static void integer_to_binary(unsigned char s[], unsigned char x)
{
       for(int i=READ_LEN-1;i>=0;i--)
       {
           s[i]=(x%2==0?'0':'1');
           x/=2;
       }
       s[READ_LEN]='\0';
}
static int find_char(unsigned char* p,int *len)
{
    int i;
    for(i=0;i<MAX_CHARACTER;++i)
    {
        if(getLen(codes[i])==0) continue;
        if(memcmp(codes[i],p,getLen(codes[i]))==0){
             *len=getLen(codes[i]);
             break;
        }
    }
    return i;
}
int decompress(void *addr_in,int len_in, void*addr_out, int *len_out)
{
    unsigned char arr[MAX_CHARACTER+5];
    int cnt[MAX_CHARACTER+5];
    int size=0;

    unsigned char* read_addr=addr_in;
    unsigned char s[READ_LEN+5];
    unsigned char* decompress_contents=malloc(len_in*MAX_TREE_HT);
    size=*(int*)read_addr;
    read_addr+=sizeof(int);
    for(int i=0;i<size;++i)
    {
        arr[i]=*read_addr;
        //printf("%d ",arr[i]);
        read_addr++;
        cnt[i]=*((int*)read_addr);
       // printf("%d\n",cnt[i]);
        read_addr+=sizeof(int);
    }
    HuffmanCodes(arr,cnt,size);

    len_in-=sizeof(int)+size*(sizeof(int)+sizeof(unsigned char));

    unsigned remainder=*read_addr;
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
    //printf("%s\n",decompress_contents);
    unsigned char* p=decompress_contents;
    int len;
    unsigned char* write_addr=addr_out;
    int write_len=0;
    while(*p!='\0')
    {
        *(write_addr+write_len)=find_char(p,&len);
        p+=len;
        write_len++;
    }
    *len_out=write_len;
    free(decompress_contents);
    return 0;
}

