#include "recursion.h"
#include "../utils/error_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <linux/limits.h>
#endif

#define MAX_LINE_LENGTH 2048
#define MAX_DEPTH 10

static GtkWidget *text_view_output;

// 处理单个文件的内容
void process_file(const char* content, GtkTextBuffer *output_buffer) {
    GtkTextIter end;
    gtk_text_buffer_get_end_iter(output_buffer, &end);
    
    // 格式化处理
    char* formatted = strdup(content);
    if (formatted) {
        // 替换中文标点为英文标点
        for (char* p = formatted; *p; p++) {
            if (*p == '：') *p = ':';
            if (*p == '（') *p = '(';
            if (*p == '）') *p = ')';
        }
        gtk_text_buffer_insert(output_buffer, &end, formatted, -1);
        free(formatted);
    } else {
        gtk_text_buffer_insert(output_buffer, &end, content, -1);
    }
}

// 递归处理include指令
void replace_includes(const char* filename, GtkTextBuffer *output_buffer, int depth) {
    GtkTextIter end;
    GtkWidget *window = gtk_widget_get_toplevel(gtk_widget_get_parent(gtk_widget_get_parent(GTK_WIDGET(text_view_output))));
    
    // 检查递归深度
    if (depth > MAX_DEPTH) {
        handle_error(window, ERROR_INVALID_OPERATION, "包含文件嵌套层数过多");
        return;
    }

    // 调试输出
    g_print("Trying to open file: %s\n", filename);
    
    // 打开文件
    FILE *file = fopen(filename, "r");
    if (!file) {
        // 尝试在不同的目录中查找文件
        const char* search_paths[] = {
            ".",
            // "./resources/test_files",
            // "../resources/test_files",
            // "../../resources/test_files",
            // "../../../resources/test_files",
            // NULL
        };
        
        for (size_t i = 0; i < sizeof(search_paths)/sizeof(search_paths[0]); i++) {
            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(full_path), "%s/%s", search_paths[i], filename);
            g_print("Trying alternate path: %s\n", full_path);
            file = fopen(full_path, "r");
            if (file) {
                g_print("Successfully opened: %s\n", full_path);
                break;
            }
        }
        
        if (!file) {
            char error_msg[256];
            snprintf(error_msg, sizeof(error_msg), "无法打开文件: %s", filename);
            handle_error(window, ERROR_FILE_NOT_FOUND, error_msg);
            return;
        }
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // 检查缓冲区溢出
        if (strlen(line) >= sizeof(line) - 1) {
            handle_error(window, ERROR_BUFFER_OVERFLOW, "行内容过长");
            fclose(file);
            return;
        }

        if (strstr(line, "#include") == line) {
            char included_file[MAX_LINE_LENGTH];
            char *start = strchr(line, '"') + 1;
            char *end_quote = strrchr(line, '"');
            if (start && end_quote && end_quote > start) {
                size_t len = end_quote - start;
                if (len >= MAX_LINE_LENGTH - 1) {
                    handle_error(window, ERROR_BUFFER_OVERFLOW, "包含的文件路径过长");
                    return;
                }
                strncpy(included_file, start, len);
                included_file[len] = '\0';
                
                // 构造包含文件的完整路径
                char *dirname = g_path_get_dirname(filename);
                char full_path[PATH_MAX];
                size_t path_len = strlen(dirname) + strlen(included_file) + 2;  // +2 for '/' and '\0'
                if (path_len >= PATH_MAX) {
                    handle_error(window, ERROR_BUFFER_OVERFLOW, "路径太长");
                    return;
                }
                snprintf(full_path, PATH_MAX, "%s/%s", dirname, included_file);
                g_free(dirname);
                
                gtk_text_buffer_get_end_iter(output_buffer, &end);
                gtk_text_buffer_insert(output_buffer, &end, 
                    "\n// ----- Begin included file: ", -1);
                gtk_text_buffer_insert(output_buffer, &end, included_file, -1);
                gtk_text_buffer_insert(output_buffer, &end, " -----\n\n", -1);
                
                replace_includes(full_path, output_buffer, depth + 1);
                
                gtk_text_buffer_get_end_iter(output_buffer, &end);
                gtk_text_buffer_insert(output_buffer, &end, 
                    "\n// ----- End included file: ", -1);
                gtk_text_buffer_insert(output_buffer, &end, included_file, -1);
                gtk_text_buffer_insert(output_buffer, &end, " -----\n\n", -1);
            }
        } else {
            // 处理普通行
            char formatted_line[MAX_LINE_LENGTH * 2];  // 双倍缓冲区以防万一
            const char* src = line;
            char* dst = formatted_line;
            
            // 格式化处理
            while (*src) {
                if (*src == '：') *dst++ = ':';
                else if (*src == '（') *dst++ = '(';
                else if (*src == '）') *dst++ = ')';
                else *dst++ = *src;
                src++;
            }
            *dst = '\0';
            
            gtk_text_buffer_get_end_iter(output_buffer, &end);
            gtk_text_buffer_insert(output_buffer, &end, formatted_line, -1);
        }
    }
    
    fclose(file);
}

static void create_file_chooser_dialog(GtkWidget *parent) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "选择C/C++源文件",
        GTK_WINDOW(gtk_widget_get_toplevel(parent)),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "_取消", GTK_RESPONSE_CANCEL,
        "_打开", GTK_RESPONSE_ACCEPT,
        NULL
    );

    // 设置文件过滤器
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_add_pattern(filter, "*.c");
    gtk_file_filter_add_pattern(filter, "*.cpp");
    gtk_file_filter_add_pattern(filter, "*.h");
    gtk_file_filter_set_name(filter, "C/C++ 源文件");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // 在 create_file_chooser_dialog 函数中
    const char* test_dirs[] = {
        "resources/test_files",
        "../resources/test_files",
        "../../resources/test_files",
        "../../../resources/test_files"
    };

    // 尝试设置初始目录
    for (size_t i = 0; i < sizeof(test_dirs) / sizeof(test_dirs[0]); i++) {
        if (g_file_test(test_dirs[i], G_FILE_TEST_IS_DIR)) {
            gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), test_dirs[i]);
            break;
        }
    }

    // 运行对话框
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (filename) {
            GtkTextBuffer *output_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view_output));
            gtk_text_buffer_set_text(output_buffer, "", -1);
            replace_includes(filename, output_buffer, 0);
            g_free(filename);
        }
    }

    gtk_widget_destroy(dialog);
}

GtkWidget* create_recursion_page(void) {
    GtkWidget *page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(page), 15);

    // 创建标题
    GtkWidget *title = gtk_label_new("递归替换");
    gtk_style_context_add_class(gtk_widget_get_style_context(title), "title");
    gtk_box_pack_start(GTK_BOX(page), title, FALSE, FALSE, 10);

    // 创建说明文本
    GtkWidget *description = gtk_label_new("选择一个C/C++源文件，程序将递归展开其中的 #include 指令");
    gtk_style_context_add_class(gtk_widget_get_style_context(description), "description");
    gtk_box_pack_start(GTK_BOX(page), description, FALSE, FALSE, 5);

    // 文件选择区域
    GtkWidget *file_frame = gtk_frame_new("文件选择");
    gtk_box_pack_start(GTK_BOX(page), file_frame, FALSE, FALSE, 10);

    GtkWidget *file_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(file_frame), file_box);
    gtk_container_set_border_width(GTK_CONTAINER(file_box), 10);

    GtkWidget *select_button = gtk_button_new_with_label("选择文件");
    gtk_box_pack_start(GTK_BOX(file_box), select_button, TRUE, TRUE, 5);
    g_signal_connect(select_button, "clicked", G_CALLBACK(create_file_chooser_dialog), NULL);

    // 输出显示区域
    GtkWidget *output_frame = gtk_frame_new("文件内容");
    gtk_box_pack_start(GTK_BOX(page), output_frame, TRUE, TRUE, 0);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(output_frame), scroll);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
                                 GTK_POLICY_AUTOMATIC,
                                 GTK_POLICY_AUTOMATIC);

    text_view_output = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view_output), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view_output), GTK_WRAP_WORD_CHAR);
    gtk_container_add(GTK_CONTAINER(scroll), text_view_output);

    return page;
}
