#include "sorting.h"
#include "../utils/error_handler.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static GtkWidget *text_view_input;
static GtkWidget *text_view_output;

// 从A构造D
void construct_D_from_A(const int* A, int N, int* D, int* D_size) {
    if (!A || !D || !D_size || N <= 0) {
        handle_error(NULL, ERROR_INVALID_INPUT, "无效的输入参数");
        return;
    }

    if (N > 100) {  // 假设最大允许100个元素
        handle_error(NULL, ERROR_BUFFER_OVERFLOW, "输入数组过大");
        return;
    }

    int k = 0;
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < i; j++) {
            D[k++] = A[i] - A[j];
        }
    }
    *D_size = k;
    sort_array(D, k);
}

// 排序数组
void sort_array(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// 从D构造A
void construct_A_from_D(const int* D, int D_size, int* A, int* A_size) {
    // A[0] = 0
    A[0] = 0;
    *A_size = 1;
    
    // 从D中选择合适的数构造A
    for (int i = 0; i < D_size && *A_size < 100; i++) {
        int candidate = D[i];
        int valid = 1;
        
        // 检查是否可以将candidate添加到A中
        for (int j = 0; j < *A_size; j++) {
            int diff = abs(candidate - A[j]);
            int found = 0;
            
            // 检查差值是否在D中
            for (int k = 0; k < D_size; k++) {
                if (D[k] == diff) {
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                valid = 0;
                break;
            }
        }
        
        if (valid) {
            A[*A_size] = candidate;
            (*A_size)++;
        }
    }
}

// 重命名函数并确保它是静态的
static int parse_array_numbers(const char* input, int* numbers, int* count) {
    char* input_copy = strdup(input);
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
        
        if (!input_text) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "无法获取输入文本");
        }
        
        numbers = malloc(MAX_NUMBERS * sizeof(int));
        result = malloc(MAX_NUMBERS * sizeof(int));
        
        if (!numbers || !result) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        }
        
        int count;
        if (!parse_array_numbers(input_text, numbers, &count)) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "无效的输入格式");
        }
        
        // 构造D序列
        construct_D_sequence(numbers, count, result);
        
        // 显示结果
        result_str = array_to_string(result, count);
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
        
        if (!input_text) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "无法获取输入文本");
        }
        
        numbers = malloc(MAX_NUMBERS * sizeof(int));
        result = malloc(MAX_NUMBERS * sizeof(int));
        
        if (!numbers || !result) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        }
        
        int count;
        if (!parse_array_numbers(input_text, numbers, &count)) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "无效的输入格式");
        }
        
        // 构造A序列
        construct_A_sequence(numbers, count, result);
        
        // 显示结果
        result_str = array_to_string(result, count);
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

// 将数组转换为字符串
static char* array_to_string(const int arr[], int size) {
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

// 构造D序列
static void construct_D_sequence(const int* numbers, int count, int* result) {
    int D_size;
    construct_D_from_A(numbers, count, result, &D_size);
}

// 构造A序列
static void construct_A_sequence(const int* numbers, int count, int* result) {
    int A_size;
    construct_A_from_D(numbers, count, result, &A_size);
}
