#ifndef UNION_H
#define UNION_H

#include <gtk/gtk.h>

#define MAX_SET_SIZE 1000

// 声明全局变量
extern GtkWidget *text_view_input1;
extern GtkWidget *text_view_input2;
extern GtkWidget *text_view_output;

// 函数声明
void perform_set_union(GtkWidget *widget, gpointer data);
void perform_set_intersection(GtkWidget *widget, gpointer data);
void perform_set_difference(GtkWidget *widget, gpointer data);
GtkWidget* create_union_page(void);

#endif
