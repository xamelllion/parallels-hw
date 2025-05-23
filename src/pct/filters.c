#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <pct/filters.h>

const double id_filter_matrix[3][3] = {
    {0.0, 0.0, 0.0},  //
    {0.0, 1.0, 0.0},  //
    {0.0, 0.0, 0.0},  //
};

const double blur_filter_matrix[5][5] = {
    {0, 0, 1, 0, 0},  //
    {0, 1, 1, 1, 0},  //
    {1, 1, 1, 1, 1},  //
    {0, 1, 1, 1, 0},  //
    {0, 0, 1, 0, 0},  //
};

const double motion_blur_filter_matrix[9][9] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0},  //
    {0, 1, 0, 0, 0, 0, 0, 0, 0},  //
    {0, 0, 1, 0, 0, 0, 0, 0, 0},  //
    {0, 0, 0, 1, 0, 0, 0, 0, 0},  //
    {0, 0, 0, 0, 1, 0, 0, 0, 0},  //
    {0, 0, 0, 0, 0, 1, 0, 0, 0},  //
    {0, 0, 0, 0, 0, 0, 1, 0, 0},  //
    {0, 0, 0, 0, 0, 0, 0, 1, 0},  //
    {0, 0, 0, 0, 0, 0, 0, 0, 1},  //
};

const double edges_filter_matrix[5][5] = {
    {0, 0, -1, 0, 0},  //
    {0, 0, -1, 0, 0},  //
    {0, 0, 2, 0, 0},   //
    {0, 0, 0, 0, 0},   //
    {0, 0, 0, 0, 0},   //
};

const double sharpen_filter_matrix[3][3] = {
    {-1, -1, -1},  //
    {-1, 9, -1},   //
    {-1, -1, -1},  //
};

double** copy_filter_matrix(int size, const double matrix[size][size]) {
    double** result;

    result = malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        result[i] = malloc(size * sizeof(double));
        memcpy(result[i], matrix[i], size * sizeof(double));
    }

    return result;
}

void free_filter_matrix(struct filter* filter) {
    for (int i = 0; i < filter->size; i++) {
        free(filter->filter[i]);
    }

    free(filter->filter);
}

struct filter* init_filters() {
    struct filter* filters = malloc(FILTERS_NUM * sizeof(struct filter));

    filters[id_filter_type] = (struct filter){
        .factor = 1.0,
        .bias = 0.0,
        .size = 3,
        .filter = copy_filter_matrix(3, id_filter_matrix),
    };

    filters[blur_filter_type] = (struct filter){
        .factor = 1.0 / 13.0,
        .bias = 0.0,
        .size = 5,
        .filter = copy_filter_matrix(5, blur_filter_matrix),
    };

    filters[mb_filter_type] = (struct filter){
        .factor = 1.0 / 9.0,
        .bias = 0.0,
        .size = 9,
        .filter = copy_filter_matrix(9, motion_blur_filter_matrix),
    };

    filters[edges_filter_type] = (struct filter){
        .factor = 1.0,
        .bias = 0.0,
        .size = 5,
        .filter = copy_filter_matrix(5, edges_filter_matrix),
    };

    filters[sharpen_filter_type] = (struct filter){
        .factor = 1.0,
        .bias = 0.0,
        .size = 3,
        .filter = copy_filter_matrix(3, sharpen_filter_matrix),
    };

    return filters;
}

void free_filters(struct filter* filters) {
    for (size_t i = 0; i < FILTERS_NUM; i++) {
        free_filter_matrix(&filters[i]);
    }

    free(filters);
}
