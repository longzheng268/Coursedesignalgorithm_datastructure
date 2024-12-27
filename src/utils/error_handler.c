#include "error_handler.h"
#include <stdio.h>
#include <time.h>

// 初始化错误上下文
void init_error_context(ErrorContext *ctx) {
    ctx->code = ERROR_NONE;
    ctx->message[0] = '\0';
    ctx->details[0] = '\0';
    ctx->has_error = FALSE;
}

// 清理错误上下文
void clear_error_context(ErrorContext *ctx) {
    init_error_context(ctx);
}

// 添加全局错误处理器
static void global_error_handler(const gchar *log_domain,
                               GLogLevelFlags log_level,
                               const gchar *message,
                               gpointer user_data) __attribute__((unused));

// 获取错误代码对应的描述
const char* get_error_string(ErrorCode code) {
    switch (code) {
        case ERROR_NONE:
            return "无错误";
        case ERROR_FILE_NOT_FOUND:
            return "文件未找到";
        case ERROR_INVALID_INPUT:
            return "无效输入";
        case ERROR_MEMORY_ALLOCATION:
            return "内存分配失败";
        case ERROR_BUFFER_OVERFLOW:
            return "缓冲区溢出";
        case ERROR_INVALID_OPERATION:
            return "无效操作";
        case ERROR_SYSTEM:
            return "系统错误";
        default:
            return "未知错误";
    }
}

// 显示错误对话框
void show_error_dialog(GtkWidget *parent, const char *title, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                             GTK_DIALOG_MODAL,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_OK,
                                             "%s", title);
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// 在本地视图中显示错误信息
void show_error_message(GtkWidget *text_view, const char *message) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(buffer, message, -1);
}

// 记录错误到日志文件
void log_error(ErrorCode code, const char *message) {
    time_t now;
    time(&now);
    char timestamp[26];
    ctime_r(&now, timestamp);
    timestamp[24] = '\0';  // 移除换行符

    FILE *log_file = fopen("error.log", "a");
    if (log_file) {
        fprintf(log_file, "[%s] Error %d: %s - %s\n",
                timestamp, code, get_error_string(code), message);
        fclose(log_file);
    }
}

// 统一错误处理函数
void handle_error(GtkWidget *parent, ErrorCode code, const char *message) {
    // 记录错误
    log_error(code, message);

    // 构造错误信息
    char full_message[1280];
    snprintf(full_message, sizeof(full_message), 
             "错误类型: %s\n错误详情: %s", 
             get_error_string(code), message);

    // 显示错误对话框
    show_error_dialog(parent, "错误", full_message);
} 