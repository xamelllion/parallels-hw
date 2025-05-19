#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <pct/utils.h>

struct rgb_image* generate_image(int width, int height) {
    srand(time(NULL));

    struct rgb_image* image = malloc(width * height * sizeof(struct rgb_image));
    for (size_t i = 0; i < (size_t)(width * height); i++) {
        image[i].r = rand() % 256;
        image[i].g = rand() % 256;
        image[i].b = rand() % 256;
    }

    return image;
}

int compare_images(struct rgb_image* expected, struct rgb_image* actual, int width, int height) {
    int eps = 1;
    for (size_t i = 0; i < (size_t)(width * height); i++) {
        if (abs(expected[i].r - actual[i].r) > eps || abs(expected[i].g - actual[i].g) > eps ||
            abs(expected[i].b - actual[i].b) > eps) {
            return 0;
        }
    }

    return 1;
}

struct filter create_empty_filter(int size) {
    struct filter result = {
        .size = size,
        .factor = 1.0,
        .bias = 0.0,
        .filter = (double**)malloc(size * sizeof(double*)),
    };

    for (int i = 0; i < size; i++) {
        result.filter[i] = (double*)calloc(size, sizeof(double));
    }

    return result;
}

struct filter create_filter(int size, double factor, double bias, const double filter[size][size]) {
    struct filter result;
    result.size = size;
    result.factor = factor;
    result.bias = bias;

    result.filter = (double**)malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        result.filter[i] = (double*)calloc(size, sizeof(double));
        memcpy(result.filter[i], filter[i], size * sizeof(double));
    }

    return result;
}

struct filter filter_composition(const struct filter* f1, const struct filter* f2) {
    int new_size = f1->size + f2->size - 1;

    struct filter composed = create_empty_filter(new_size);

    for (int i = 0; i < new_size; i++) {
        for (int j = 0; j < new_size; j++) {
            for (int fi = 0; fi < f1->size; fi++) {
                for (int fj = 0; fj < f1->size; fj++) {
                    int i2 = i - fi;
                    int j2 = j - fj;

                    if (i2 >= 0 && i2 < f2->size && j2 >= 0 && j2 < f2->size) {
                        composed.filter[i][j] += f1->filter[fi][fj] * f2->filter[i2][j2];
                    }
                }
            }
        }
    }

    composed.factor = f1->factor * f2->factor;
    composed.bias = f1->bias * f2->factor + f2->bias;

    return composed;
}

struct filter generate_random_filter(int size) {
    struct filter random_filter = create_empty_filter(size);

    srand(time(NULL));

    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            random_filter.filter[i][j] = 0.0005 + ((double)rand() / RAND_MAX) * (1.0 - 0.0005);
            sum += random_filter.filter[i][j];
        }
    }

    if (sum != 0.0) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                random_filter.filter[i][j] /= sum;
            }
        }
    }

    return random_filter;
}

struct filter expand_filter(const struct filter* original, int padding) {
    int new_size = original->size + 2 * padding;

    struct filter expanded = create_empty_filter(new_size);
    expanded.factor = original->factor;
    expanded.bias = original->bias;

    int offset = padding;
    for (int i = 0; i < original->size; i++) {
        for (int j = 0; j < original->size; j++) {
            expanded.filter[i + offset][j + offset] = original->filter[i][j];
        }
    }

    return expanded;
}

void prepare_image_for_relaunch(struct image_info* info) {
    free(info->image);
    info->image = info->result;
    info->result = NULL;
}

void free_filter(struct filter* filter) {
    for (int i = 0; i < filter->size; i++) {
        free(filter->filter[i]);
    }

    free(filter->filter);
}

struct image_info copy_image_info(struct image_info* info) {
    struct image_info copy = {
        .width = info->width,
        .height = info->height,
        .image = malloc(info->width * info->height * sizeof(struct rgb_image)),
    };

    memcpy(copy.image, info->image, info->width * info->height * sizeof(struct rgb_image));

    return copy;
}

void free_image_info(struct image_info* info) {
    free(info->image);
    free(info->result);
}
