#pragma once

#define FILTERS_NUM 5

struct filter {
    double factor;
    double bias;
    int size;
    double** filter;
};

enum pct_filter_type {
    id_filter_type,
    blur_filter_type,
    mb_filter_type,
    edges_filter_type,
    sharpen_filter_type,
};

struct filter* init_filters();

void free_filters(struct filter* filters);
