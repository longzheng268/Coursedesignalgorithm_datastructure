#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include "union/union.h"
#include "recursion/recursion.h"
#include "huffman/huffman.h"
#include "sorting/sorting.h"
#include "utils/error_handler.h"

#define BACKGROUND_CHANGE_INTERVAL 300000
#define DEFAULT_WINDOW_WIDTH 1366
#define DEFAULT_WINDOW_HEIGHT 768

typedef struct {
    GtkWidget *window;
    GtkWidget *background_image;
    GtkWidget *content_box;
    GtkWidget *notebook;
    GtkWidget *font_scale;
    char **image_paths;
    int image_count;
    double scale_factor;
    GdkPixbuf *current_pixbuf;
    int base_font_size;
    double font_scale_factor;
} AppContext;

static AppContext app_ctx;

// 更新背景图片
static gboolean update_background(gpointer data) {
    if (app_ctx.image_count <= 0) {
        g_print("No background images found!\n");
        return G_SOURCE_CONTINUE;
    }

    // 获取窗口当前大小
    int width, height;
    gtk_window_get_size(GTK_WINDOW(app_ctx.window), &width, &height);

    // 随机选择一张图片
    int image_index = rand() % app_ctx.image_count;
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(app_ctx.image_paths[image_index], &error);
    
    if (error) {
        g_print("Error loading background image: %s\n", error->message);
        g_error_free(error);
        return G_SOURCE_CONTINUE;
    }
    
    if (pixbuf) {
        // 缩放图片以适应窗口
        GdkPixbuf *scaled = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);
        if (scaled) {
            // 清理旧的背景
            if (app_ctx.current_pixbuf) {
                g_object_unref(app_ctx.current_pixbuf);
            }
            app_ctx.current_pixbuf = scaled;
            gtk_image_set_from_pixbuf(GTK_IMAGE(app_ctx.background_image), scaled);
        }
        g_object_unref(pixbuf);
    }

    return G_SOURCE_CONTINUE;
}

// 添加字体大小更新函数
static void update_font_size(void) {
    // 计算实际字体大小
    int actual_size = app_ctx.base_font_size * app_ctx.font_scale_factor;
    
    // 创建 CSS 提供器
    GtkCssProvider *provider = gtk_css_provider_new();
    char css[1024];  // 增加缓冲区大小以容纳更多样式
    snprintf(css, sizeof(css),
             "* { font-size: %dpx; }\n"
             ".title { font-size: %dpx; font-weight: bold; }\n"
             ".description { font-size: %dpx; font-style: italic; }\n"
             "button {\n"
             "    background-image: none;\n"
             "    background-color: rgba(255, 255, 255, 0.9);\n"
             "    border: 1px solid #999;\n"
             "    border-radius: 4px;\n"
             "    padding: 6px 12px;\n"
             "    transition: all 200ms ease;\n"
             "}\n"
             "button:hover {\n"
             "    background-color: rgba(220, 220, 220, 0.9);\n"
             "    border-color: #666;\n"
             "    box-shadow: 0 2px 4px rgba(0,0,0,0.2);\n"
             "}\n"
             "button:active {\n"
             "    background-color: rgba(200, 200, 200, 0.9);\n"
             "    box-shadow: inset 0 2px 4px rgba(0,0,0,0.1);\n"
             "}\n"
             "frame {\n"
             "    background-color: rgba(255, 255, 255, 0.95);\n"
             "    border-radius: 6px;\n"
             "    box-shadow: 0 2px 6px rgba(0,0,0,0.1);\n"
             "}\n"
             "frame > label {\n"
             "    padding: 5px;\n"
             "    font-weight: bold;\n"
             "}\n"
             "notebook {\n"
             "    background-color: transparent;\n"
             "}\n"
             "notebook tab {\n"
             "    padding: 6px 12px;\n"
             "    background-color: rgba(240, 240, 240, 0.9);\n"
             "    border: 1px solid #ccc;\n"
             "    transition: all 200ms ease;\n"
             "}\n"
             "notebook tab:hover {\n"
             "    background-color: rgba(255, 255, 255, 0.95);\n"
             "}\n"
             "notebook tab:checked {\n"
             "    background-color: rgba(255, 255, 255, 0.95);\n"
             "    border-bottom: 2px solid #4a90d9;\n"
             "}\n"
             "scale {\n"
             "    margin: 8px;\n"
             "}\n"
             "scale slider {\n"
             "    background-color: #fff;\n"
             "    border: 1px solid #999;\n"
             "    border-radius: 50%%;\n"
             "    box-shadow: 0 1px 3px rgba(0,0,0,0.1);\n"
             "    transition: all 200ms ease;\n"
             "}\n"
             "scale slider:hover {\n"
             "    background-color: #f0f0f0;\n"
             "    box-shadow: 0 2px 4px rgba(0,0,0,0.2);\n"
             "}\n"
             "textview {\n"
             "    background-color: rgba(255, 255, 255, 0.95);\n"
             "    padding: 4px;\n"
             "}\n",
             actual_size, 
             (int)(actual_size * 1.5),  // 标题字体更大
             actual_size);
    
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    
    // 应用到整个应用程序
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    g_object_unref(provider);
}

// 字体大小滑块变化回调
static void on_font_scale_changed(GtkRange *range, gpointer data) {
    (void)data;
    app_ctx.font_scale_factor = gtk_range_get_value(range);
    update_font_size();
}

// 窗口大小改变回调
static gboolean on_window_resize(GtkWidget *widget, 
                               GdkEventConfigure *event, 
                               gpointer data) {
    static guint resize_timer = 0;
    static int last_width = 0;
    
    int width, height;
    gtk_window_get_size(GTK_WINDOW(app_ctx.window), &width, &height);
    
    // 根据窗口宽度调整字体大小
    if (width != last_width) {
        app_ctx.font_scale_factor = (double)width / 1920.0;
        gtk_range_set_value(GTK_RANGE(app_ctx.font_scale), app_ctx.font_scale_factor);
        update_font_size();
        last_width = width;
    }
    
    if (resize_timer > 0) {
        g_source_remove(resize_timer);
    }
    resize_timer = g_timeout_add(500, (GSourceFunc)update_background, NULL);
    
    return FALSE;
}

// 加载背景图片
static void load_backgrounds(void) {
    const char *bg_dir = "resources/backgrounds";
    DIR *dir = opendir(bg_dir);
    if (!dir) {
        g_print("Cannot open backgrounds directory: %s\n", bg_dir);
        return;
    }

    // 计算图片数量
    struct dirent *entry;
    app_ctx.image_count = 0;
    while ((entry = readdir(dir)) != NULL) {
        const char *name = entry->d_name;
        if (strstr(name, ".jpg") || strstr(name, ".png") || strstr(name, ".jpeg")) {
            app_ctx.image_count++;
        }
    }
    
    g_print("Found %d background images\n", app_ctx.image_count);
    
    if (app_ctx.image_count > 0) {
        app_ctx.image_paths = malloc(sizeof(char*) * app_ctx.image_count);
        rewinddir(dir);
        
        int index = 0;
        while ((entry = readdir(dir)) != NULL) {
            const char *name = entry->d_name;
            if (strstr(name, ".jpg") || strstr(name, ".png") || strstr(name, ".jpeg")) {
                char *path = malloc(strlen(bg_dir) + strlen(name) + 2);
                sprintf(path, "%s/%s", bg_dir, name);
                app_ctx.image_paths[index] = path;
                g_print("Loaded background: %s\n", path);
                index++;
            }
        }
    }
    closedir(dir);
}

// 窗口大小变化的回调函数
static void on_window_size_changed(GtkWidget *widget, 
                                 GtkAllocation *allocation, 
                                 gpointer user_data) {
    // 获取新的窗口尺寸
    int width, height;
    gtk_window_get_size(GTK_WINDOW(widget), &width, &height);
    
    // 更新UI元素大小或进行其他必要的调整
    // ... 根据需要添加代码 ...
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    srand(time(NULL));

    // 初始化应用上下文
    memset(&app_ctx, 0, sizeof(AppContext));
    app_ctx.scale_factor = 1.0;

    // 创建主窗口
    app_ctx.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(app_ctx.window), "算法与数据结构课程设计展示系统 by 210052202019 龙正");
    
    // 设置默认窗口大小
    gtk_window_set_default_size(GTK_WINDOW(app_ctx.window), 
                               DEFAULT_WINDOW_WIDTH, 
                               DEFAULT_WINDOW_HEIGHT);
    
    // 允许调整窗口大小
    gtk_window_set_resizable(GTK_WINDOW(app_ctx.window), TRUE);
    
    // 设置窗口最小尺寸（可选）
    gtk_widget_set_size_request(app_ctx.window, 800, 600);
    
    // 添加窗口大小变化的回调（如果需要响应窗口大小变化）
    g_signal_connect(G_OBJECT(app_ctx.window), "size-allocate",
                    G_CALLBACK(on_window_size_changed), NULL);

    // 创建叠加容器
    GtkWidget *overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(app_ctx.window), overlay);

    // 创建背景图片
    app_ctx.background_image = gtk_image_new();
    gtk_widget_set_halign(app_ctx.background_image, GTK_ALIGN_FILL);
    gtk_widget_set_valign(app_ctx.background_image, GTK_ALIGN_FILL);
    gtk_container_add(GTK_CONTAINER(overlay), app_ctx.background_image);

    // 创建内容区域
    app_ctx.content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_opacity(app_ctx.content_box, 0.95);
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), app_ctx.content_box);

    // 创建字体控制工具栏
    GtkWidget *toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_margin_start(toolbar, 10);
    gtk_widget_set_margin_end(toolbar, 10);
    gtk_widget_set_margin_top(toolbar, 5);
    gtk_widget_set_margin_bottom(toolbar, 5);
    gtk_box_pack_start(GTK_BOX(app_ctx.content_box), toolbar, FALSE, FALSE, 5);
    
    // 添加字体大小控制
    GtkWidget *font_label = gtk_label_new("字体大小：");
    gtk_widget_set_margin_end(font_label, 10);
    gtk_box_pack_start(GTK_BOX(toolbar), font_label, FALSE, FALSE, 5);
    
    app_ctx.font_scale = gtk_scale_new_with_range(GTK_ORIENTATION_HORIZONTAL, 0.5, 2.0, 0.1);
    gtk_range_set_value(GTK_RANGE(app_ctx.font_scale), 1.0);
    gtk_widget_set_size_request(app_ctx.font_scale, 200, -1);  // 增加滑块宽度
    gtk_scale_set_draw_value(GTK_SCALE(app_ctx.font_scale), TRUE);  // 显示数值
    gtk_scale_set_value_pos(GTK_SCALE(app_ctx.font_scale), GTK_POS_RIGHT);  // 数值显示在右侧
    gtk_box_pack_start(GTK_BOX(toolbar), app_ctx.font_scale, FALSE, FALSE, 5);
    
    g_signal_connect(app_ctx.font_scale, "value-changed", G_CALLBACK(on_font_scale_changed), NULL);

    // 创建标签页
    app_ctx.notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(app_ctx.content_box), app_ctx.notebook, TRUE, TRUE, 0);

    // 添加各个页面
    GtkWidget *union_page = create_union_page();
    GtkWidget *union_label = gtk_label_new("集合运算");
    gtk_notebook_append_page(GTK_NOTEBOOK(app_ctx.notebook), union_page, union_label);

    GtkWidget *recursion_page = create_recursion_page();
    GtkWidget *recursion_label = gtk_label_new("递归替换");
    gtk_notebook_append_page(GTK_NOTEBOOK(app_ctx.notebook), recursion_page, recursion_label);

    GtkWidget *huffman_page = create_huffman_page();
    GtkWidget *huffman_label = gtk_label_new("哈夫曼编码");
    gtk_notebook_append_page(GTK_NOTEBOOK(app_ctx.notebook), huffman_page, huffman_label);

    GtkWidget *sorting_page = create_sorting_page();
    GtkWidget *sorting_label = gtk_label_new("排序重构");
    gtk_notebook_append_page(GTK_NOTEBOOK(app_ctx.notebook), sorting_page, sorting_label);

    // 加载背景图片
    load_backgrounds();

    // 连接信号
    g_signal_connect(app_ctx.window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(app_ctx.window, "configure-event", G_CALLBACK(on_window_resize), NULL);

    // 设置背景更新定时器
    g_timeout_add(BACKGROUND_CHANGE_INTERVAL, update_background, NULL);

    // 初始化字体相关变量
    app_ctx.base_font_size = 14;  // 基础字体大小
    app_ctx.font_scale_factor = 1.0;
    
    // 应用初始字体大小
    update_font_size();

    // 显示窗口
    gtk_widget_show_all(app_ctx.window);

    // 初始化背景
    update_background(NULL);

    gtk_main();

    // 清理资源
    if (app_ctx.current_pixbuf) {
        g_object_unref(app_ctx.current_pixbuf);
    }
    for (int i = 0; i < app_ctx.image_count; i++) {
        free(app_ctx.image_paths[i]);
    }
    free(app_ctx.image_paths);

    return 0;
}
