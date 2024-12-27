#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define MAX_CHAR 128

// 哈夫曼树节点结构
typedef struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
} MinHeapNode;

// 最小堆结构
typedef struct MinHeap {
    unsigned size;
    unsigned capacity;
    MinHeapNode** array;
} MinHeap;

// 哈夫曼编码结构
typedef struct {
    char character;
    char* code;
} HuffmanCode;

// 基本函数声明
GtkWidget* create_huffman_page(void);
MinHeapNode* new_node(char data, unsigned freq);
MinHeap* create_min_heap(unsigned capacity);
void swap_min_heap_node(MinHeapNode** a, MinHeapNode** b);
void minHeapify(MinHeap* minHeap, int idx);
int is_size_one(MinHeap* minHeap);
MinHeapNode* extract_min(MinHeap* minHeap);
void insert_min_heap(MinHeap* minHeap, MinHeapNode* minHeapNode);
void build_min_heap(MinHeap* minHeap);
MinHeapNode* build_huffman_tree(char data[], int freq[], int size);
void print_codes(MinHeapNode* root, char* code_str, int top, GtkTextBuffer* buffer);
void count_frequency(const char* text, char* data, int* freq, int* size);
void encode_text(const char* text, GtkTextBuffer* output_buffer);
char* decode_text(const char* encoded_text, MinHeapNode* root);
void store_code(char data, char* code);
char* get_code(char c);
void clear_huffman_codes(void);

#endif
