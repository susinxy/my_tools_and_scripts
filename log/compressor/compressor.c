#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compressor.h"

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



// *Collage* FUNC power_base_head: obtain different exponential results of 2
static int power_base_head(int a,int b){
    if(b == 0) return 1;        // as long as a!=0, a^0 = 1
    if(b & 1) return a * power_base_head(a, b - 1);     // if b is an odd number, reduce it to the smaller even number adjacent to it
    else{
        int t = power_base_head(a, b / 2);      // calculate the half-headed result for even head powering, and return to its squared value
        return t * t;
    }
}
// *Collage* FUNC binary_to_integer: convert binary figures into decimal equivalents
static unsigned char binary_to_integer(char *s){
    unsigned char ans = 0;    //
    for(int i=0; i<READ_LEN; ++i)
        ans += power_base_head(2, READ_LEN - 1 - i) * (s[i] == '1' ? 1 : 0);
    return ans;
}


// ****** Main Function ******
// inputs: the address of buffer to compress; the size of buffer;
//         the address of compressed contents; the size of compressed text.
int compress(void *addr_in, int len_in, void* addr_out, int *len_out){
    unsigned char data[MAX_CHAR+5];     // used for storing characters *********
    int fre[MAX_CHAR+5];            // used for storing the times that each character appears in the text
    int size = 0;                   // used for measuring the expanding size of the MinHeap
    memset(fre, 0, sizeof(fre));    // setting the frequency array to empty status

    unsigned char *p=(unsigned char*)addr_in;   // obtain the buffer and its contents
    // calculating the times that each character appears in the contents
    for(int i=0; i<len_in; ++i){
        fre[*p]++;
        p++;
    }

    // filtering out the unfrequent characters and maintain the appeared ones
    for(int i=0; i<MAX_CHAR; ++i){
        if(fre[i] != 0){
            data[size] = i;
            fre[size] = fre[i];
            size++;
        }
    }
    // build the tree and encode characters
    HuffmanCodes(data, fre, size);

    // obtain the outflowing address for the pointer to start writing in compressed contents
    unsigned char* write_addr = addr_out;
    // convert the unsigned char ptr into int ptr and store the number of used characters in the first int(4 Bytes)
    *((int*)write_addr) = size;
    write_addr += sizeof(int);          // move the ptr ahead for 1 unit of the int type
    // write in the statistical records of nodes || size = the amount of recorded
    for(int i=0; i<size; ++i){
        // structure unit: [char]
        *write_addr=data[i];
        write_addr++;
        // structure unit: [times of appearing]
        *((int*)write_addr)=fre[i];
        write_addr+=sizeof(int);
    }
    // adding up the length: the pre-stored segment
    *len_out = sizeof(int) + size * (sizeof(int) + sizeof(unsigned char));
    // if the compression is not efficient enough
    if(*len_out >= len_in){
        // copy the authentic text into the compression
        memcpy(addr_out,addr_in,len_in);
        *len_out=len_in;
        return -1;
    }
    // locate the ptr at the input address
    p=(unsigned char*)addr_in;
    // firstly obtain the total length of binary code to determine how many loops are needed thereafter
    int total_len=0;
    for(int i=0; i<len_in; ++i){
        total_len += strlen(codes[*p]);
        p++;
    }
    // the last few digits short of 8-digit (char) type
    unsigned char remainder = total_len % READ_LEN;
    *write_addr = remainder;
    write_addr++;

    // the exported length is expanded
    // total_len / READ_LEN + 1: the compressed text
    // the last additional digit: 1
    *len_out = *len_out + total_len / READ_LEN + 1 + 1;
    // if compression efficiency is not as expected, return the original text
    if(*len_out>=len_in){
        memcpy(addr_out, addr_in, len_in);
        *len_out=len_in;
        return -1;
    }
    // obtain the original text for compression
    p = (unsigned char*)addr_in;
    char tmp[MAX_CHAR * 2 + 5];
    char buffer[WRITE_LEN + 5];
    char binary[WRITE_LEN + 5];
    int current_len = 0;
    int str_len = 0;
    int count = 0;
    // load in the text char by char
    for(int i=0; i<len_in; ++i){
        // obtain the length of associated huffman code
        str_len = (int)strlen(codes[*p]);
        // if the current digits haven't reached 8, directly copy it into the buffer
        if(current_len + str_len < WRITE_LEN){
            memcpy(tmp + current_len, codes[*p], str_len);
            current_len += str_len;
        }
        else{
            // move the buffer to the current position and copy the entire code to the tmp buffer
            memcpy(tmp + current_len, codes[*p], str_len);
            current_len += str_len;
            // get the times of iteration
            count = current_len / WRITE_LEN;
            for(int j=0; j<count; ++j){
                   memcpy(binary, tmp + j * WRITE_LEN, WRITE_LEN);
                   *write_addr = binary_to_integer(binary);
                   write_addr++;
            }
            // obtain the rest of digits that haven't reached 8
            current_len = current_len % WRITE_LEN;
            memcpy(buffer, tmp + count*WRITE_LEN, current_len);
            memcpy(tmp, buffer, current_len);
        }
        p++;
    }
    // to complete the last few digits to 8-digit, use '0'
    for(int i=0; i<WRITE_LEN - remainder; ++i) tmp[current_len+i] = '0';
    // store the remainder into the last character
    memcpy(binary, tmp, WRITE_LEN);
    *write_addr = binary_to_integer(binary);
    write_addr++;
    return 0;
}
