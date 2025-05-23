#pragma once

/// Number of available filter types
#define FILTERS_NUM 5

/// Image processing filter structure
struct filter {
    double factor;    //< Scaling factor (e.g. 1.0 for no scaling)
    double bias;      //< Constant offset (e.g. 0.0 for no offset)
    int size;         //< Filter matrix size (size x size)
    double** filter;  //< 2D array of coefficients
};

/// Filter type identifiers
enum pct_filter_type {
    id_filter_type,      //< Identity filter (no effect)
    blur_filter_type,    //< Gaussian blur filter
    mb_filter_type,      //< Motion blur filter
    edges_filter_type,   //< Edge detection filter
    sharpen_filter_type  //< Image sharpening filter
};

/**
 * @brief Initialize all available filters
 * @return Array of FILTERS_NUM preconfigured filters
 */
struct filter* init_filters();

/**
 * @brief Free filters array memory
 * @param filters Array created by init_filters()
 */
void free_filters(struct filter* filters);
