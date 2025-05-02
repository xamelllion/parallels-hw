#pragma once

struct filter {
    double factor;
    double bias;
    int size;
    double** filter;
};

extern double blur_filter_matrix[5][5];
extern double motion_blur_filter_matrix[9][9];

double** copy_filter_matrix(int size, double matrix[size][size]);
void free_filter_matrix(struct filter* filter);
