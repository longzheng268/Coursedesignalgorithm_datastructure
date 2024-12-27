#include "union.h"
#include "../utils/error_handler.h"
#include "../sorting/sorting.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// 定义全局变量
GtkWidget *text_view_input1;
GtkWidget *text_view_input2;
GtkWidget *text_view_output;

// 定义静态变量
static int result[MAX_SET_SIZE];

// 函数声明（移到使用之前）
static bool contains(const int arr[], int size, int value);
static char* array_to_string(const int arr[], int size);

static int parse_union_set(const char* input, int* set) {
    int count = 0;
    const unsigned char* p = (const unsigned char*)input;
    
    while (*p) {
        while (*p && (isspace(*p) || *p == ',')) {
            p++;
        }
        
        if (!*p) break;
        
        char* end;
        int num = strtol((const char*)p, &end, 10);
        
        if ((const char*)p == end) {
            return -1;
        }
        
        set[count++] = num;
        p = (const unsigned char*)end;
    }
    
    return count;
}

// 检查数组中是否包含某个元素
static bool contains(const int arr[], int size, int value) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == value) {
            return true;
        }
    }
    return false;
}

// 将数组转换为字符串
static char* array_to_string(const int arr[], int size) {
    char* result = malloc(size * 12 + 1);
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

void perform_set_union(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    
    ErrorContext error_ctx;
    init_error_context(&error_ctx);
    
    GtkTextBuffer *buffer1 = NULL, *buffer2 = NULL;
    char *set1_text = NULL, *set2_text = NULL;
    int *set1 = NULL, *set2 = NULL;
    char *result_str = NULL;
    gchar *display = NULL;
    
    TRY(&error_ctx) {
        buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input1));
        buffer2 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input2));
        
        if (!buffer1 || !buffer2) {
            THROW(&error_ctx, ERROR_INVALID_OPERATION, "无法获取输入缓冲区");
        }
        
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(buffer1, &start, &end);
        set1_text = gtk_text_buffer_get_text(buffer1, &start, &end, FALSE);
        
        gtk_text_buffer_get_bounds(buffer2, &start, &end);
        set2_text = gtk_text_buffer_get_text(buffer2, &start, &end, FALSE);
        
        if (!set1_text || !set2_text) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        }
        
        set1 = malloc(MAX_SET_SIZE * sizeof(int));
        set2 = malloc(MAX_SET_SIZE * sizeof(int));
        
        if (!set1 || !set2) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        }
        
        int size1 = parse_union_set(set1_text, set1);
        int size2 = parse_union_set(set2_text, set2);
        
        if (size1 < 0 || size2 < 0) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "无效输入 - 请输入有效的集合");
        }
        
        // 合并两个集合
        int result_size = 0;
        for (int i = 0; i < size1; i++) {
            result[result_size++] = set1[i];
        }
        for (int i = 0; i < size2; i++) {
            if (!contains(result, result_size, set2[i])) {
                result[result_size++] = set2[i];
            }
        }
        
        // 对结果排序
        sort_array(result, result_size);
        
        // 显示结果
        result_str = array_to_string(result, result_size);
        if (!result_str) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "无法生成结果字符串");
        }
        
        display = g_strdup_printf("并集结果：{%s}", result_str);
        if (!display) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "无法生成显示字符串");
        }
        
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
        gtk_text_buffer_set_text(buffer, display, -1);
    }
    CATCH(&error_ctx) {
        handle_error(gtk_widget_get_toplevel(widget), error_ctx.code, error_ctx.message);
    }
    FINALLY({
        g_free(set1_text);
        g_free(set2_text);
        free(set1);
        free(set2);
        free(result_str);
        g_free(display);
    });
}

void perform_set_intersection(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    
    ErrorContext error_ctx;
    init_error_context(&error_ctx);
    
    GtkTextBuffer *buffer1 = NULL, *buffer2 = NULL;
    char *set1_text = NULL, *set2_text = NULL;
    int *set1 = NULL, *set2 = NULL;
    char *result_str = NULL;
    gchar *display = NULL;
    
    TRY(&error_ctx) {
        buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input1));
        buffer2 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input2));
        
        if (!buffer1 || !buffer2) {
            THROW(&error_ctx, ERROR_INVALID_OPERATION, "无法获取输入缓冲区");
        }
        
        GtkTextIter start, end;
        gtk_text_buffer_get_bounds(buffer1, &start, &end);
        set1_text = gtk_text_buffer_get_text(buffer1, &start, &end, FALSE);
        
        gtk_text_buffer_get_bounds(buffer2, &start, &end);
        set2_text = gtk_text_buffer_get_text(buffer2, &start, &end, FALSE);
        
        if (!set1_text || !set2_text) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        }
        
        set1 = malloc(MAX_SET_SIZE * sizeof(int));
        set2 = malloc(MAX_SET_SIZE * sizeof(int));
        
        if (!set1 || !set2) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "内存分配失败");
        }
        
        int size1 = parse_union_set(set1_text, set1);
        int size2 = parse_union_set(set2_text, set2);
        
        if (size1 < 0 || size2 < 0) {
            THROW(&error_ctx, ERROR_INVALID_INPUT, "无效输入 - 请输入有效的集合");
        }
        
        // 计算交集
        int result_size = 0;
        for (int i = 0; i < size1; i++) {
            if (contains(set2, size2, set1[i])) {
                result[result_size++] = set1[i];
            }
        }
        
        // 对结果排序
        sort_array(result, result_size);
        
        // 显示结果
        result_str = array_to_string(result, result_size);
        if (!result_str) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "无法生成结果字符串");
        }
        
        display = g_strdup_printf("交集结果：{%s}", result_str);
        if (!display) {
            THROW(&error_ctx, ERROR_MEMORY_ALLOCATION, "无法生成显示字符串");
        }
        
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
        gtk_text_buffer_set_text(buffer, display, -1);
    }
    CATCH(&error_ctx) {
        handle_error(gtk_widget_get_toplevel(widget), error_ctx.code, error_ctx.message);
    }
    FINALLY({
        g_free(set1_text);
        g_free(set2_text);
        free(set1);
        free(set2);
        free(result_str);
        g_free(display);
    });
}

void perform_set_difference(GtkWidget *widget, gpointer data) {
    (void)widget;
    (void)data;
    
    GtkTextBuffer *buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input1));
    GtkTextBuffer *buffer2 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_input2));
    
    GtkTextIter start, end;
    gtk_text_buffer_get_bounds(buffer1, &start, &end);
    char *set1_text = gtk_text_buffer_get_text(buffer1, &start, &end, FALSE);
    
    gtk_text_buffer_get_bounds(buffer2, &start, &end);
    char *set2_text = gtk_text_buffer_get_text(buffer2, &start, &end, FALSE);
    
    int set1[MAX_SET_SIZE], set2[MAX_SET_SIZE];
    int size1 = parse_union_set(set1_text, set1);
    int size2 = parse_union_set(set2_text, set2);
    
    g_free(set1_text);
    g_free(set2_text);
    
    if (size1 < 0 || size2 < 0) {
        log_error(2, "无效输入 - 请输入两个集合");
        return;
    }
    
    // 计算差集 (set1 - set2)
    int result_size = 0;
    for (int i = 0; i < size1; i++) {
        if (!contains(set2, size2, set1[i])) {
            result[result_size++] = set1[i];
        }
    }
    
    // 对结果排序
    sort_array(result, result_size);
    
    // 显示结果
    char *result_str = array_to_string(result, result_size);
    gchar *display = g_strdup_printf("差集结果：{%s}", result_str);
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
    gtk_text_buffer_set_text(buffer, display, -1);
    
    free(result_str);
    g_free(display);
}

GtkWidget* create_union_page(void) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 15);

    // 创建标题
    GtkWidget *title = gtk_label_new("集合运算");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "title");
    gtk_box_pack_start(GTK_BOX(page), title, FALSE, FALSE, 10);

    // 创建说明文本
    GtkWidget *description = gtk_label_new("输入两个集合进行并集、交集、差集运算");
    gtk_style_context_add_class(gtk_widget_get_style_context(description), "description");
    gtk_box_pack_start(GTK_BOX(page), description, FALSE, FALSE, 5);

    // 创建输入区域1
    GtkWidget *input_frame1 = gtk_frame_new("集合1");
    gtk_box_pack_start(GTK_BOX(page), input_frame1, TRUE, TRUE, 5);

    GtkWidget *input_scroll1 = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(input_frame1), input_scroll1);
    text_view_input1 = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(input_scroll1), text_view_input1);

    // 创建输入区域2
    GtkWidget *input_frame2 = gtk_frame_new("集合2");
    gtk_box_pack_start(GTK_BOX(page), input_frame2, TRUE, TRUE, 5);

    GtkWidget *input_scroll2 = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(input_frame2), input_scroll2);
    text_view_input2 = gtk_text_view_new();
    gtk_container_add(GTK_CONTAINER(input_scroll2), text_view_input2);

    // 创建按钮区域
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(page), button_box, FALSE, FALSE, 5);

    GtkWidget *union_button = gtk_button_new_with_label("并集");
    GtkWidget *intersection_button = gtk_button_new_with_label("交集");
    GtkWidget *difference_button = gtk_button_new_with_label("差集");

    gtk_box_pack_start(GTK_BOX(button_box), union_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(button_box), intersection_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(button_box), difference_button, TRUE, TRUE, 5);

    g_signal_connect(union_button, "clicked", G_CALLBACK(perform_set_union), NULL);
    g_signal_connect(intersection_button, "clicked", G_CALLBACK(perform_set_intersection), NULL);
    g_signal_connect(difference_button, "clicked", G_CALLBACK(perform_set_difference), NULL);

    // 创建输出区域
    GtkWidget *output_frame = gtk_frame_new("运算结果");
    gtk_box_pack_start(GTK_BOX(page), output_frame, TRUE, TRUE, 5);

    GtkWidget *output_scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(output_frame), output_scroll);
    text_view_output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_output), FALSE);
    gtk_container_add(GTK_CONTAINER(output_scroll), text_view_output);

    return page;
}
