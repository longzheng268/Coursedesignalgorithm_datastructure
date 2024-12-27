#ifndef SORTING_H
#define SORTING_H

#include <gtk/gtk.h>
#define MAX_ARRAY_SIZE 100
#define MAX_NUMBERS 100  // 定义最大数字数量

// 函数声明
GtkWidget* create_sorting_page(void);
void construct_D_from_A(const int* A, int N, int* D, int* D_size);
void construct_A_from_D(const int* D, int D_size, int* A, int* A_size);
void sort_array(int* arr, int size);

// 内部函数声明
static char* array_to_string(const int arr[], int size);
static void construct_D_sequence(const int* numbers, int count, int* result);
static void construct_A_sequence(const int* numbers, int count, int* result);

#endif
