#ifndef RECURSION_H
#define RECURSION_H

#include <gtk/gtk.h>

// 函数声明
GtkWidget* create_recursion_page(void);
void replace_includes(const char* filename, GtkTextBuffer *output_buffer, int depth);

#endif
