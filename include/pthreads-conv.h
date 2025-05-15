#pragma once

#include <filters.h>
#include <pthread.h>

// Структура для представления пикселя
struct pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

// Структура для задания свертки (передается в поток)
struct convolution_task {
    unsigned char* pixel_array;
    struct pixel* result;
    int w;
    int h;
    struct filter filter;
    int start_x;
    int end_x;
    int start_y;
    int end_y;
};

// Основная функция параллельной свертки
void* parallel_convolution(void* arg);

// Функции для разных способов разделения изображения:

// 1. Попиксельное разделение
void pixelwise_convolution(unsigned char* pixel_array, int w, int h, struct filter filter,
                           int num_threads);

// 2. Разделение по строкам
void row_convolution(unsigned char* pixel_array, int w, int h, struct filter filter,
                     int num_threads);

// 3. Разделение по столбцам
void column_convolution(unsigned char* pixel_array, int w, int h, struct filter filter,
                        int num_threads);

// 4. Произвольная прямоугольная решётка
void grid_convolution(unsigned char* pixel_array, int w, int h, struct filter filter,
                      int num_threads);
