#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

// 错误类型枚举
typedef enum {
    ERROR_NONE = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_INVALID_INPUT,
    ERROR_MEMORY_ALLOCATION,
    ERROR_BUFFER_OVERFLOW,
    ERROR_INVALID_OPERATION,
    ERROR_SYSTEM,
    ERROR_UNKNOWN
} ErrorCode;

// 错误处理结构
typedef struct {
    ErrorCode code;
    char message[256];
    jmp_buf jump_buffer;
    gboolean has_error;
} ErrorContext;

// 定义错误处理宏
#define TRY(ctx) if (setjmp((ctx)->jump_buffer) == 0)
#define CATCH(ctx) else
#define THROW(ctx, error_code, msg) do { \
    (ctx)->code = (error_code); \
    strncpy((ctx)->message, (msg), sizeof((ctx)->message) - 1); \
    (ctx)->message[sizeof((ctx)->message) - 1] = '\0'; \
    (ctx)->has_error = TRUE; \
    longjmp((ctx)->jump_buffer, 1); \
} while(0)
#define FINALLY(code) code

// 错误处理函数声明
void init_error_context(ErrorContext *ctx);
void handle_error(GtkWidget *parent, ErrorCode code, const char *message);
const char* get_error_string(ErrorCode code);
void log_error(ErrorCode code, const char *message);

#endif 