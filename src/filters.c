#include <filters.h>
#include <stdlib.h>
#include <string.h>

double blur_filter_matrix[5][5] = {
    0, 0, 1, 0, 0,  //
    0, 1, 1, 1, 0,  //
    1, 1, 1, 1, 1,  //
    0, 1, 1, 1, 0,  //
    0, 0, 1, 0, 0,  //
};

double motion_blur_filter_matrix[9][9] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0,  //
    0, 1, 0, 0, 0, 0, 0, 0, 0,  //
    0, 0, 1, 0, 0, 0, 0, 0, 0,  //
    0, 0, 0, 1, 0, 0, 0, 0, 0,  //
    0, 0, 0, 0, 1, 0, 0, 0, 0,  //
    0, 0, 0, 0, 0, 1, 0, 0, 0,  //
    0, 0, 0, 0, 0, 0, 1, 0, 0,  //
    0, 0, 0, 0, 0, 0, 0, 1, 0,  //
    0, 0, 0, 0, 0, 0, 0, 0, 1,  //
};

double** copy_filter_matrix(int size, double matrix[size][size]) {
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
