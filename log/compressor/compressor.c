#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compressor.h"
// A Huffman tree node
static char codes[MAX_CHARACTER+5][MAX_TREE_HT+5];
 struct MinHeapNode
{
    unsigned char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

// A Min Heap:  Collection of min heap (or Hufmman tree) nodes
 struct MinHeap
{
    // Current size of min heap
    unsigned size;
    // capacity of min heap
    unsigned capacity;
    // Attay of minheap node pointers
    struct MinHeapNode** array;
};

// A utility function allocate a new min heap node with given character and frequency of the character
static struct MinHeapNode* newNode(unsigned char data, unsigned freq)
{
    struct MinHeapNode* temp= (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}
// A utility function to create a min heap of given capacity
static struct MinHeap* createMinHeap(unsigned capacity)
{

    struct MinHeap* minHeap= (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array= (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
// A utility function to swap two min heap nodes
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
// A utility function to check if size of heap is 1 or not
static int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}
// A standard function to extract the minimum value node from heap
static struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]= minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// A utility function to insert a new node to Min Heap
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

// A standard function to build min heap
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

    // Step 1: Create a min heap of capacityequal to size.  Initially, there are modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap))
    {

        // Step 2: Extract the two minimum freq items from min heap
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

    // Step 4: The remaining node is the root node and the tree is complete.
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

    // Acquire Huffman codes using the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;

    acquireCodes(root, arr, top);

}

static int Pow(int a,int b)
{
    if(b==0) return 1;
    if(b&1) return a*Pow(a,b-1);
    else
    {
        int t=Pow(a,b/2);
        return t*t;
    }
}
static unsigned char binary_to_integer(char *s)
{
    unsigned char ans=0;
    for(int i=0;i<WRITE_LEN;++i)
    {
        ans+=Pow(2,WRITE_LEN-1-i)*(s[i]=='1'?1:0);
    }
    return ans;
}

int compress(void *addr_in,int len_in,void* addr_out,int *len_out)
{
    unsigned char arr[MAX_CHARACTER+5];
    int fre[MAX_CHARACTER+5];
    int size=0;
    memset(fre,0,sizeof(fre));

    unsigned char *p=(unsigned char*)addr_in;
    for(int i=0;i<len_in;++i)
    {
           fre[*p]++;
           p++;
    }

    for(int i=0;i<MAX_CHARACTER;++i)
    {
        if(fre[i]!=0)
        {
            arr[size]=i;
            fre[size]=fre[i];
            size++;
        }
    }
    HuffmanCodes(arr,fre,size);

    /*int sum=0;
    for(int i=0;i<MAX_CHARACTER;++i)
    {
        printf("%d\t%d\t%s\n",i,fre[i],codes[i]);
        sum+=fre[i];
    }
    printf("\n%d\n",sum);*/


    unsigned char* write_addr=addr_out;
    *((int*)write_addr)=size;
    write_addr+=sizeof(int);
    for(int i=0;i<size;++i)
    {
        *write_addr=arr[i];
        write_addr++;
        *((int*)write_addr)=fre[i];
        write_addr+=sizeof(int);
    }
    *len_out=sizeof(int)+size*(sizeof(int)+sizeof(unsigned char));
    if(*len_out>=len_in)
    {
        memcpy(addr_out,addr_in,len_in);
        *len_out=len_in;
        return -1;
    }

    p=(unsigned char*)addr_in;
    int totol_len=0;
    for(int i=0;i<len_in;++i)
    {
        totol_len+=strlen(codes[*p]);
        p++;
    }

    unsigned char remainder=totol_len%READ_LEN;
    *write_addr=remainder;
    write_addr++;


    *len_out=*len_out+totol_len/READ_LEN+1+1;
    if(*len_out>=len_in)
    {
        memcpy(addr_out,addr_in,len_in);
        *len_out=len_in;
        return -1;
    }


    p=(unsigned char*)addr_in;
    char binary[WRITE_LEN+5];
    unsigned int current_len=0;
    unsigned int str_len=0;
    for(int i=0;i<len_in;++i)
    {
        str_len=strlen(codes[*p]);
        if(current_len+str_len<READ_LEN)
        {
            memcpy(binary+current_len,codes[*p],str_len);
            current_len+=str_len;
        }
        else
        {
            memcpy(binary+current_len,codes[*p],WRITE_LEN-current_len);
            *write_addr=binary_to_integer(binary);
            write_addr++;
            memcpy(binary,codes[*p]+WRITE_LEN-current_len,str_len-WRITE_LEN+current_len);
            current_len=str_len-WRITE_LEN+current_len;
        }
        p++;
    }
    return 0;
}


