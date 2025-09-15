#include "huffman.h"
#include "../utils/error_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 前向声明所有静态函数
static void free_huffman_tree(MinHeapNode* node);
static void on_encode_clicked(GtkWidget *widget, gpointer user_data);
static void on_decode_clicked(GtkWidget *widget, gpointer data);

// 全局变量
static GtkWidget *text_view_input;
static GtkWidget *text_view_output;
static GtkWidget *text_view_codes;
static MinHeapNode* huffman_tree = NULL;

static HuffmanCode huffman_codes[MAX_CHAR];
static int code_count = 0;

// 释放哈夫曼树的内存
static void free_huffman_tree(MinHeapNode* node) {
    if (node == NULL) return;
    free_huffman_tree(node->left);
    free_huffman_tree(node->right);
    free(node);
}

// 存储编码
void store_code(char data, char* code) {
    huffman_codes[code_count].character = data;
    huffman_codes[code_count].code = g_strdup(code);
    code_count++;
}

// 获取字符的编码
char* get_code(char c) {
    for (int i = 0; i < code_count; i++) {
        if (huffman_codes[i].character == c) {
            return huffman_codes[i].code;
        }
    }
    return NULL;
}

// 清理编码表
void clear_huffman_codes() {
    for (int i = 0; i < code_count; i++) {
        g_free(huffman_codes[i].code);
    }
    code_count = 0;
}

// 创建新的最小堆节点
MinHeapNode* new_node(char data, unsigned freq) {
    MinHeapNode* temp = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

// 创建最小堆
MinHeap* create_min_heap(unsigned capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(minHeap->capacity * sizeof(MinHeapNode*));
    return minHeap;
}

// 交换两个最小堆节点
void swap_min_heap_node(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

// 最小堆化
void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    unsigned int size = minHeap->size;

    if (left < (int)size && 
        minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < (int)size && 
        minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swap_min_heap_node(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// 检查堆大小是否为1
int is_size_one(MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// 提取最小值节点
MinHeapNode* extract_min(MinHeap* minHeap) {
    MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// 插入新节点
void insert_min_heap(MinHeap* minHeap, MinHeapNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// 构建最小堆
void build_min_heap(MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// 构建哈夫曼树
MinHeapNode* build_huffman_tree(char data[], int freq[], int size) {
    MinHeapNode *left, *right, *top;
    MinHeap* minHeap = create_min_heap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = new_node(data[i], freq[i]);

    minHeap->size = size;
    build_min_heap(minHeap);

    while (!is_size_one(minHeap)) {
        left = extract_min(minHeap);
        right = extract_min(minHeap);
        top = new_node('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insert_min_heap(minHeap, top);
    }

    return extract_min(minHeap);
}

// 打印哈夫曼编码
void print_codes(MinHeapNode* root, char* code_str, int top, GtkTextBuffer* buffer) {
    if (root->left) {
        code_str[top] = '0';
        print_codes(root->left, code_str, top + 1, buffer);
    }

    if (root->right) {
        code_str[top] = '1';
        print_codes(root->right, code_str, top + 1, buffer);
    }

    if (!(root->left) && !(root->right)) {
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(buffer, &end);
        char temp[256];
        code_str[top] = '\0';
        
        // 存储编码
        store_code(root->data, code_str);
        
        if (root->data == ' ')
            sprintf(temp, "空格: %s\n", code_str);
        else if (root->data == '\n')
            sprintf(temp, "换行: %s\n", code_str);
        else
            sprintf(temp, "%c: %s\n", root->data, code_str);
        gtk_text_buffer_insert(buffer, &end, temp, -1);
    }
}

// 统计字符频率
void count_frequency(const char* text, char* data, int* freq, int* size) {
    int count[MAX_CHAR] = {0};
    int len = strlen(text);
    *size = 0;

    // 统计频率
    for (int i = 0; i < len; i++)
        count[(unsigned char)text[i]]++;

    // 收集非零频率的字符
    for (int i = 0; i < MAX_CHAR; i++) {
        if (count[i] > 0) {
            data[*size] = (char)i;
            freq[*size] = count[i];
            (*size)++;
        }
    }
}

// 编码回调函数
static void on_encode_clicked(GtkWidget *widget, gpointer user_data) {
    (void)user_data;
    
    // 清理之前的状态
    clear_huffman_codes();
    if (huffman_tree) {
        free_huffman_tree(huffman_tree);
        huffman_tree = NULL;
    }
    
    // 获取输入文本
    GtkTextBuffer *input_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(input_buffer, &start, &end);
    char *input_text = gtk_text_buffer_get_text(input_buffer, &start, &end, FALSE);
    
    if (!input_text || strlen(input_text) == 0) {
        handle_error(gtk_widget_get_toplevel(widget), ERROR_INVALID_INPUT, "请输入要编码的文本");
        g_free(input_text);
        return;
    }
    
    // 统计字符频率
    char char_array[MAX_CHAR];
    int freq[MAX_CHAR];
    int size = 0;
    count_frequency(input_text, char_array, freq, &size);
    
    // 构建哈夫曼树
    huffman_tree = build_huffman_tree(char_array, freq, size);
    if (!huffman_tree) {
        handle_error(gtk_widget_get_toplevel(widget), ERROR_MEMORY_ALLOCATION, "构建哈夫曼树失败");
        g_free(input_text);
        return;
    }
    
    // 清空编码表显示
    gtk_text_buffer_set_text(gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_codes)), "", -1);
    
    // 生成编码表
    char code_str[MAX_TREE_HT];
    print_codes(huffman_tree, code_str, 0, gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_codes)));
    
    // 编码文本
    GString *encoded = g_string_new("");
    for (char *p = input_text; *p; p++) {
        char *code = get_code(*p);
        if (code) {
            g_string_append(encoded, code);
            g_string_append(encoded, " ");  // 添加空格分隔
        }
    }
    
    // 显示结果
    GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
    gtk_text_buffer_set_text(output_buffer, encoded->str, -1);
    
    // 清理资源
    g_string_free(encoded, TRUE);
    g_free(input_text);
}

// 解码回调函数
static void on_decode_clicked(GtkWidget *widget, gpointer data) {
    (void)data;
    
    // 获取输入的编码
    GtkTextBuffer *input_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input));
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(input_buffer, &start, &end);
    char *encoded_text = gtk_text_buffer_get_text(input_buffer, &start, &end, FALSE);
    
    if (!encoded_text || strlen(encoded_text) == 0) {
        handle_error(gtk_widget_get_toplevel(widget), ERROR_INVALID_INPUT, "请输入要解码的二进制串");
        g_free(encoded_text);
        return;
    }
    
    // 检查输入是否为有效的二进制串
    for (char *p = encoded_text; *p; p++) {
        if (*p != '0' && *p != '1' && *p != ' ' && *p != '\n') {
            handle_error(gtk_widget_get_toplevel(widget), ERROR_INVALID_INPUT, 
                        "输入必须是二进制串（只包含0和1，可以包含空格和换行）");
            g_free(encoded_text);
            return;
        }
    }
    
    // 检查是否有哈夫曼树
    if (!huffman_tree) {
        handle_error(gtk_widget_get_toplevel(widget), ERROR_INVALID_OPERATION, 
                    "请先进行编码操作以生成哈夫曼树");
        g_free(encoded_text);
        return;
    }
    
    // 解码
    char *decoded = decode_text(encoded_text, huffman_tree);
    if (!decoded) {
        handle_error(gtk_widget_get_toplevel(widget), ERROR_INVALID_OPERATION, 
                    "解码失败：无效的编码或内存不足");
        g_free(encoded_text);
        return;
    }
    
    // 显示结果
    GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
    gtk_text_buffer_set_text(output_buffer, decoded, -1);
    
    // 清理资源
    g_free(encoded_text);
    free(decoded);
}

// 编码文本
void encode_text(const char* text, GtkTextBuffer* output_buffer) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(output_buffer, &end);
    
    for (int i = 0; text[i] != '\0'; i++) {
        char* code = get_code(text[i]);
        if (code) {
            gtk_text_buffer_insert(output_buffer, &end, code, -1);
            gtk_text_buffer_insert(output_buffer, &end, " ", -1); // 添加空格分隔
        }
    }
}

// 解码文本
char* decode_text(const char* encoded_text, MinHeapNode* root) {
    if (!root || !encoded_text) return NULL;
    
    // 预分配足够的空间
    size_t max_len = strlen(encoded_text) + 1;
    char* result = malloc(max_len);
    if (!result) return NULL;
    
    int result_index = 0;
    MinHeapNode* current = root;
    
    // 添加安全检查
    size_t max_iterations = strlen(encoded_text) * 2;
    size_t iteration_count = 0;
    
    for (const char* p = encoded_text; *p && result_index < max_len - 1; p++) {
        if (*p == ' ' || *p == '\n') continue;  // 跳过空格和换行
        
        if (*p != '0' && *p != '1') {
            free(result);
            return NULL;
        }
        
        if (!current) {  // 添加空指针检查
            free(result);
            return NULL;
        }
        
        current = (*p == '0') ? current->left : current->right;
        
        if (!current) {  // 添加空指针检查
            free(result);
            return NULL;
        }
        
        // 到达叶子节点
        if (!current->left && !current->right) {
            result[result_index++] = current->data;
            current = root;
        }
        
        // 安全检查
        if (++iteration_count > max_iterations) {
            free(result);
            return NULL;
        }
    }
    
    // 确保解码完成
    if (current != root) {
        free(result);
        return NULL;
    }
    
    result[result_index] = '\0';
    return result;
}

GtkWidget* create_huffman_page(void) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 15);

    // 创建标题
    GtkWidget *title = gtk_label_new("哈夫曼编码系统");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "title");
    gtk_box_pack_start(GTK_BOX(page), title, FALSE, FALSE, 10);

    // 创建说明文本
    GtkWidget *description = gtk_label_new("输入文本进行哈夫曼编码和解码");
    gtk_box_pack_start(GTK_BOX(page), description, FALSE, FALSE, 5);

    // 创建输入区域
    GtkWidget *input_frame = gtk_frame_new("输入文本");
    gtk_box_pack_start(GTK_BOX(page), input_frame, TRUE, TRUE, 5);

    GtkWidget *input_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(input_frame), input_scroll);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(input_scroll),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

    text_view_input = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(input_scroll), text_view_input);

    // 创建按钮区域
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(page), button_box, FALSE, FALSE, 5);

    GtkWidget *encode_button = gtk_button_new_with_label("编码");
    GtkWidget *decode_button = gtk_button_new_with_label("解码");
    gtk_box_pack_start(GTK_BOX(button_box), encode_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(button_box), decode_button, TRUE, TRUE, 5);

    g_signal_connect(encode_button, "clicked", G_CALLBACK(on_encode_clicked), NULL);
    g_signal_connect(decode_button, "clicked", G_CALLBACK(on_decode_clicked), NULL);

    // 创建编码表显示区域
    GtkWidget *codes_frame = gtk_frame_new("编码表");
    gtk_box_pack_start(GTK_BOX(page), codes_frame, TRUE, TRUE, 5);

    GtkWidget *codes_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(codes_frame), codes_scroll);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(codes_scroll),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

    text_view_codes = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_codes), FALSE);
    gtk_container_add(GTK_CONTAINER(codes_scroll), text_view_codes);

    // 创建输出区域
    GtkWidget *output_frame = gtk_frame_new("输出结果");
    gtk_box_pack_start(GTK_BOX(page), output_frame, TRUE, TRUE, 5);

    GtkWidget *output_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(output_frame), output_scroll);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(output_scroll),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

    text_view_output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_output), FALSE);
    gtk_container_add(GTK_CONTAINER(output_scroll), text_view_output);

    return page;
}
