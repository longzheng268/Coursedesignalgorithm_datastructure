#include "sorting.h"
#include "../utils/error_handler.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#define _GNU_SOURCE

static GtkWidget *text_view_input;
static GtkWidget *text_view_output;

// 函数声明
static int compare_ints(const void* a, const void* b);
static char* array_to_string(const int arr[], int size);

// 比较函数用于qsort
static int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// 从A构造D
void construct_D_from_A(const int* A, int N, int* D, int* D_size) {
    if (!A || !D || !D_size || N <= 0) {
        handle_error(NULL, ERROR_INVALID_INPUT, "无效的输入参数");
        return;
    }

    // 检查A[0]是否为0
    if (A[0] != 0) {
        handle_error(NULL, ERROR_INVALID_INPUT, "A序列的第一个数必须为0");
        return;
    }

    if (N > MAX_ARRAY_SIZE) {
        handle_error(NULL, ERROR_BUFFER_OVERFLOW, "输入数组过大");
        return;
    }

    // 计算D序列的大小
    int expected_size = (N * (N - 1)) / 2;
    if (expected_size > MAX_ARRAY_SIZE) {
        handle_error(NULL, ERROR_BUFFER_OVERFLOW, "结果数组将超出最大限制");
        return;
    }

    int k = 0;
    for (int i = 1; i < N && k < MAX_ARRAY_SIZE; i++) {
        for (int j = 0; j < i && k < MAX_ARRAY_SIZE; j++) {
            D[k++] = A[i] - A[j];
        }
    }
    *D_size = k;

    // 使用快速排序
    sort_array(D, k);
}

// 从D构造A
void construct_A_from_D(const int* D, int D_size, int* A, int* A_size) {
    if (!D || !A || !A_size || D_size <= 0) {
        handle_error(NULL, ERROR_INVALID_INPUT, "无效的输入参数");
        return;
    }

    if (D_size > MAX_ARRAY_SIZE) {
        handle_error(NULL, ERROR_BUFFER_OVERFLOW, "输入数组过大");
        return;
    }

    // 创建查找表
    int min_val = D[0], max_val = D[0];
    for (int i = 1; i < D_size; i++) {
        if (D[i] < min_val) min_val = D[i];
        if (D[i] > max_val) max_val = D[i];
    }

    int range = max_val - min_val + 1;
    char* exists = calloc(range, sizeof(char));
    if (!exists) {
        handle_error(NULL, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        return;
    }

    // 标记所有存在的差值
    for (int i = 0; i < D_size; i++) {
        exists[D[i] - min_val] = 1;
    }

    // A[0] = 0
    A[0] = 0;
    *A_size = 1;
    
    // 从D中选择合适的数构造A
    for (int i = 0; i < D_size && *A_size < MAX_ARRAY_SIZE; i++) {
        int candidate = D[i];
        int valid = 1;
        
        // 检查是否可以将candidate添加到A中
        for (int j = 0; j < *A_size && valid; j++) {
            int diff = abs(candidate - A[j]);
            if (diff < range && !exists[diff - min_val]) {
                valid = 0;
                break;
            }
        }
        
        if (valid) {
            A[*A_size] = candidate;
            (*A_size)++;
            
            if (*A_size >= MAX_ARRAY_SIZE) {
                break;
            }
        }
    }

    free(exists);

    // 检查是否成功构造了A序列
    if (*A_size == 1) {
        handle_error(NULL, ERROR_INVALID_INPUT, "无法构造有效的A序列：输入的D序列不是有效的差分序列");
    }
}

// 重命名函数并确保它是静态的
static int parse_array_numbers(const char* input, int* numbers, int* count) {
    char* input_copy = g_strdup(input);
    char* token = strtok(input_copy, " ,\n");
    *count = 0;
    
    while (token != NULL && *count < 100) {
        // 检查是否为有效数字
        for (int i = 0; token[i]; i++) {
            if (!isdigit(token[i]) && token[i] != '-') {
                free(input_copy);
                return 0;  // 解析失败
            }
        }
        numbers[*count] = atoi(token);
        (*count)++;
        token = strtok(NULL, " ,\n");
    }
    
    free(input_copy);
    return 1;  // 解析成功
}

// 构造D的回调函数
static void on_construct_D_clicked(GtkWidget *widget, gpointer data) {
    (void)data;
    
    ErrorContext error_ctx;
    init_error_context(&error_ctx);
    
    char *input_text = NULL;
    char *result_str = NULL;
    int *numbers = NULL;
    int *result = NULL;
    
    TRY(&error_ctx) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input));
        if (!buffer) {
            THROW(&error_ctx, ERROR_INVALID_OPERATION, "无法获取输入缓冲区");
        }
        
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        input_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        
        if (!input_text || strlen(input_text) == 0) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "请输入要处理的数列");
        }
        
        numbers = malloc(MAX_NUMBERS * sizeof(int));
        result = malloc(MAX_NUMBERS * sizeof(int));
        
        if (!numbers || !result) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        }
        
        int count;
        if (!parse_array_numbers(input_text, numbers, &count)) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "无效的输入格式 - 请输入用空格或逗号分隔的数字");
        }
        
        if (count < 2) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "请至少输入两个数字");
        }
        
        // 检查第一个数是否为0
        if (numbers[0] != 0) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "A序列的第一个数必须为0");
        }
        
        // 计算D序列
        int D_size;
        construct_D_from_A(numbers, count, result, &D_size);
        
        // 显示结果
        result_str = array_to_string(result, D_size);
        if (!result_str) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "无法生成结果字符串");
        }
        
        GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
        gtk_text_buffer_set_text(output_buffer, result_str, -1);
    }
    CATCH(&error_ctx) {
        handle_error(gtk_widget_get_toplevel(widget), error_ctx.code, error_ctx.message);
    }
    FINALLY({
        g_free(input_text);
        free(numbers);
        free(result);
        free(result_str);
    });
}

// 构造A的回调函数
static void on_construct_A_clicked(GtkWidget *widget, gpointer data) {
    (void)data;
    
    ErrorContext error_ctx;
    init_error_context(&error_ctx);
    
    char *input_text = NULL;
    char *result_str = NULL;
    int *numbers = NULL;
    int *result = NULL;
    
    TRY(&error_ctx) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input));
        if (!buffer) {
            THROW(&error_ctx, ERROR_INVALID_OPERATION, "无法获取输入缓冲区");
        }
        
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        input_text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        
        if (!input_text || strlen(input_text) == 0) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "请输入要处理的数列");
        }
        
        numbers = malloc(MAX_NUMBERS * sizeof(int));
        result = malloc(MAX_NUMBERS * sizeof(int));
        
        if (!numbers || !result) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        }
        
        int count;
        if (!parse_array_numbers(input_text, numbers, &count)) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "无效的输入格式 - 请输入用空格或逗号分隔的数字");
        }
        
        if (count < 1) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "请至少输入一个数字");
        }
        
        // 计算A序列
        int A_size;
        construct_A_from_D(numbers, count, result, &A_size);
        
        // 检查结果的第一个数是否为0
        if (A_size > 0 && result[0] != 0) {
            // 如果第一个数不是0，说明这个D序列不是有效的差分序列
            THROW(&error_ctx, ERROR_INVALID_INPUT, "无法构造有效的A序列：输入的D序列不是有效的差分序列（A序列必须从0开始）");
        }
        
        // 显示结果
        result_str = array_to_string(result, A_size);
        if (!result_str) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "无法生成结果字符串");
        }
        
        GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
        gtk_text_buffer_set_text(output_buffer, result_str, -1);
    }
    CATCH(&error_ctx) {
        handle_error(gtk_widget_get_toplevel(widget), error_ctx.code, error_ctx.message);
    }
    FINALLY({
        g_free(input_text);
        free(numbers);
        free(result);
        free(result_str);
    });
}

GtkWidget* create_sorting_page(void) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 15);

    // 创建标题
    GtkWidget *title = gtk_label_new("排序重构");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "title");
    gtk_box_pack_start(GTK_BOX(page), title, FALSE, FALSE, 10);

    // 创建说明文本
    GtkWidget *description = gtk_label_new(
        "1. 构造D：输入数列A（A[0]=0），计算D[i,j] = A[i]-A[j]\n"
        "2. 构造A：输入数列D，重构可能的数列A"
    );
    gtk_box_pack_start(GTK_BOX(page), description, FALSE, FALSE, 5);

    // 创建输入区域
    GtkWidget *input_frame = gtk_frame_new("输入数列（用空格或逗号分隔）");
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

    GtkWidget *construct_d_button = gtk_button_new_with_label("构造D");
    GtkWidget *construct_a_button = gtk_button_new_with_label("构造A");
    gtk_box_pack_start(GTK_BOX(button_box), construct_d_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(button_box), construct_a_button, TRUE, TRUE, 5);

    g_signal_connect(construct_d_button, "clicked", G_CALLBACK(on_construct_D_clicked), NULL);
    g_signal_connect(construct_a_button, "clicked", G_CALLBACK(on_construct_A_clicked), NULL);

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

// 排序数组 - 使用快速排序
void sort_array(int* arr, int size) {
    if (!arr || size <= 0) return;
    qsort(arr, size, sizeof(int), compare_ints);
}

// 将数组转换为字符串
static char* array_to_string(const int arr[], int size) {
    if (!arr || size <= 0) return NULL;
    
    char* result = malloc(size * 12 + 1);  // 每个数字最多10位，加上逗号和空格
    if (!result) return NULL;
    
    result[0] = '\0';
    for (int i = 0; i < size; i++) {
        char temp[12];
        snprintf(temp, sizeof(temp), "%d", arr[i]);
        strcat(result, temp);
        if (i < size - 1) {
            strcat(result, ", ");
        }
    }
    return result;
}
