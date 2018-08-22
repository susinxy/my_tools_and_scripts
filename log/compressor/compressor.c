// C program for Huffman Coding
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// This constant can be avoided by explicitly
// calculating height of Huffman Tree
#define MAX_TREE_HT  32
#define MAX_CHARACTER 256
#define READ_LEN 8
// A Huffman tree node
unsigned char codes[MAX_CHARACTER][MAX_TREE_HT];
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
struct MinHeapNode* newNode(unsigned char data, unsigned freq)
{
    struct MinHeapNode* temp= (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// A utility function to create
// a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)
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
void swapMinHeapNode(struct MinHeapNode** a,struct MinHeapNode** b)
{

    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
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
int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}
// A standard function to extract
// minimum value node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0]= minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// A utility function to insert
// a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap,struct MinHeapNode* minHeapNode)
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
void buildMinHeap(struct MinHeap* minHeap)
{

    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}


// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right);
}

// Creates a min heap of capacity
// equal to size and inserts all character of
// data[] in min heap. Initially size of
// min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size)

{

    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(unsigned char data[], int freq[], int size)

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
void acquireCodes(struct MinHeapNode* root, int arr[], int top)

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
        /*printf("%u: ", root->data);
        printArr(arr, top);*/
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
void HuffmanCodes(unsigned char data[], int freq[], int size)

{
    // Construct Huffman Tree
    struct MinHeapNode* root= buildHuffmanTree(data, freq, size);

    // Print Huffman codes using
    // the Huffman tree built above
    int arr[MAX_TREE_HT], top = 0;

    acquireCodes(root, arr, top);
}

// Driver program to test above functions
int Pow(int a,int b)
{
    if(b==0) return 1;
    if(b&1) return a*Pow(a,b-1);
    else
    {
        int t=Pow(a,b/2);
        return t*t;
    }
}
unsigned char binary_to_integer(unsigned char *s)
{
    unsigned char ans=0;
    for(int i=0;i<READ_LEN;++i)
    {
        ans+=Pow(2,READ_LEN-1-i)*(s[i]=='1'?1:0);
    }
    return ans;
}
void integer_to_binary(unsigned char s[], unsigned char x)
{
       for(int i=READ_LEN-1;i>=0;i--)
       {
           s[i]=(x%2==0?'0':'1');
           x/=2;
       }
       s[READ_LEN]='\0';
}
int COMPRESS(void *addr_in,int len_in,void* addr_out,int *len_out)
{
    unsigned char arr[MAX_CHARACTER+5];
    int fre[MAX_CHARACTER+5];
    int cnt[MAX_CHARACTER+5];
    int size=0;
    memset(fre,0,sizeof(fre));
    memset(cnt,0,sizeof(cnt));

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
            cnt[size]=fre[i];
            size++;
        }
    }
    /*printf("%d\n",size);
    for(int i=0;i<size;i++)
        printf("%d,",cnt[i]);
    printf("\n");*/

    HuffmanCodes(arr,cnt,size);

   /* for(int i=0;i<MAX_CHARACTER;++i)
        printf("%d %s\n",i,codes[i]);*/

    unsigned char* compress_contents=malloc(len_in*MAX_TREE_HT);
    p=(unsigned char*)addr_in;
    int totol_len=0;
    int str_len=0;
    for(int i=0;i<len_in;++i)
    {
        if(totol_len>=(len_in-1)*READ_LEN) return -1;
        str_len=strlen(codes[*p]);
        memcpy(compress_contents+totol_len,codes[*p],str_len);
        totol_len+=str_len;
        p++;
    }
    //printf("%s %d\n",compress_contents,totol_len);

    unsigned char remainder=totol_len%READ_LEN;
    int blocks=totol_len/READ_LEN+1;
     *len_out=blocks+1;

    for(int i=0;i<READ_LEN-remainder;++i)
    *(compress_contents+totol_len+i)='0';

    unsigned char* read_addr=compress_contents;
    unsigned char* write_addr=addr_out;
    int offset=0;
    unsigned char tmp[READ_LEN+5];
    *write_addr=remainder;
    write_addr++;
    for(int i=0;i<blocks;++i)
    {
        memcpy(tmp,compress_contents+offset,READ_LEN);
        tmp[READ_LEN]='\0';
        offset+=READ_LEN;
        //printf("%s\n",tmp);
        *write_addr=binary_to_integer(tmp);
        write_addr++;
    }
    free(compress_contents);
    return 0;
}
int find_char(unsigned char* p,int *len)
{
    for(int i=0;i<MAX_CHARACTER;++i)
    {
        if(strlen(codes[i])==0) continue;
        if(memcmp(codes[i],p,strlen(codes[i]))==0)
        {
             *len=strlen(codes[i]);
             return i;
        }
    }
}
int DECOMPRESS(void *addr_in,int len_in,void* addr_out, int *len_out)
{

    unsigned char* read_addr=addr_in;
    unsigned char s[READ_LEN+5]="";
    unsigned char* decompress_contents=malloc(len_in*READ_LEN);

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
       // printf("%d ",find_char(p,&len));
        *(write_addr+write_len)=find_char(p,&len);
        p+=len;
        write_len++;
    }
    *len_out=write_len;
   // printf("%d",*len_out);
   // printf("\n");
    free(decompress_contents);
    return 0;
}


/*int main()
{
   unsigned char* s="Although both aforementioned methods can combine an arbitrary number of symbols for more efficient coding and generally adapt to the actual input statistics, arithmetic coding does so without significantly increasing its computational or algorithmic complexities (though the simplest version is slower and more complex than Huffman coding). Such flexibility is especially useful when input probabilities are not precisely known or vary significantly within the stream. However, Huffman coding is usually faster and arithmetic coding was historically a subject of some concern over patent issues. Thus many technologies have historically avoided arithmetic coding in favor of Huffman and other prefix coding techniques. As of mid-2010, the most commonly used techniques for this alternative to Huffman coding have passed into the public domain as the early patents have expired.";
   unsigned char x[1024];
   unsigned char y[1024+5];
   unsigned char *tmp=s;

   int len_out;
   int a;
   COMPRESS(s,strlen(s),x,&len_out);
   DECOMPRESS(x,len_out,y,&a);
   if(memcmp(s,y,a)==0)
   {
       printf("YES\n");
       printf("%d %d\n",len_out,strlen(s));
    printf("%.2f\n",(double)len_out/strlen(s));
   }
   else printf("NO");
   return 0;
}*/
