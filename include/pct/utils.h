#pragma once

#include <pct/filters.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define error(...)                    \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    } while (0);

struct rgb_image {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct image_info {
    int width;
    int height;
    struct rgb_image* image;
};

struct convolution_task {
    struct rgb_image* image;
    struct rgb_image* result;
    int w;
    int h;
    struct filter filter;
    int start_x;
    int end_x;
    int start_y;
    int end_y;
};

enum pct_mode {
    seq_mode,
    pixel_mode,
    row_mode,
    column_mode,
    grid_mode,
};

struct pct_options {
    char* read_path;
    char* write_path;
    enum pct_filter_type filter_type;
    enum pct_mode mode;
    int threads;
};

struct image_info load_image(const char* read_path);

void dump_image(const char* write_path, struct image_info info);
