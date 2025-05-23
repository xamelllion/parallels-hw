#pragma once

#include <pct/filters.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

/**
 * @def error(...)
 * @brief Prints error message and exits program
 * @param ... printf-style format string and arguments
 */
#define error(...)                    \
    do {                              \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    } while (0);

/**
 * @struct rgb_image
 * @brief Represents an RGB pixel
 */
struct rgb_image {
    unsigned char r;  ///< Red component (0-255)
    unsigned char g;  ///< Green component (0-255)
    unsigned char b;  ///< Blue component (0-255)
};

/**
 * @struct image_info
 * @brief Contains image data and dimensions
 */
struct image_info {
    int width;                 ///< Image width in pixels
    int height;                ///< Image height in pixels
    struct rgb_image* image;   ///< Pointer to original image
    struct rgb_image* result;  ///< Pointer to processed image
};

/**
 * @struct convolution_task
 * @brief Task description for convolution operations
 */
struct convolution_task {
    struct rgb_image* image;   ///< Source image data
    struct rgb_image* result;  ///< Output buffer
    int w;                     ///< Image width
    int h;                     ///< Image height
    struct filter filter;      ///< Filter to apply
    int start_x;               ///< Starting x coordinate
    int end_x;                 ///< Ending x coordinate
    int start_y;               ///< Starting y coordinate
    int end_y;                 ///< Ending y coordinate
};

/**
 * @enum pct_mode
 * @brief Parallel processing modes
 */
enum pct_mode {
    seq_mode,     ///< Sequential (single-threaded) processing
    pixel_mode,   ///< Pixel-level parallelization
    row_mode,     ///< Row-level parallelization
    column_mode,  ///< Column-level parallelization
    grid_mode,    ///< Grid-based parallelization
};

/**
 * @struct pct_options
 * @brief Image processing configuration options
 */
struct pct_options {
    char* read_path;                   ///< Input image file path
    char* write_path;                  ///< Output image file path
    enum pct_filter_type filter_type;  ///< Filter type to apply
    enum pct_mode mode;                ///< Processing mode
    int threads;                       ///< Number of threads to use
    int log;                           ///< Logging status
};

/**
 * @brief Loads image from file
 * @param read_path Path to input image file
 * @return image_info structure containing loaded image
 * @note This tool supports only PNG images
 */
struct image_info load_image(const char* read_path);

/**
 * @brief Saves image to file
 * @param write_path Destination file path
 * @param info Image data to save
 * @note This tool supports only PNG images
 */
void dump_image(const char* write_path, struct image_info info);

/// Get the optimal number of threads for the current system
int get_thread_count();

/// Gets the current monotonic time in seconds
double get_time_in_seconds();
