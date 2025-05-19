#pragma once

#include <pct/utils.h>

/**
 * @brief Generates a new RGB image with specified dimensions
 * @param width  Width of the image in pixels
 * @param height Height of the image in pixels
 * @return Pointer to newly allocated rgb_image structure
 */
struct rgb_image* generate_image(int width, int height);

/**
 * @brief Compares two images pixel by pixel with epsilon
 * @param expected Reference image for comparison
 * @param actual   Image to compare against reference
 * @param width    Image width in pixels
 * @param height   Image height in pixels
 * @return 0 if images are identical, non-zero otherwise
 */
int compare_images(struct rgb_image* expected, struct rgb_image* actual, int width, int height);

/**
 * @brief Creates an empty filter with all coefficients set to zero
 * @param size Size of the square filter matrix
 * @return Initialized filter structure
 */
struct filter create_empty_filter(int size);

/**
 * @brief Creates a filter with specified parameters
 * @param size   Size of the square filter matrix
 * @param factor Scaling factor for filter application
 * @param bias   Constant bias to add after filtering
 * @param filter 2D array of filter coefficients (size x size)
 * @return Initialized filter structure
 */
struct filter create_filter(int size, double factor, double bias, const double filter[size][size]);

/**
 * @brief Composes two filters into a single equivalent filter
 * @param f1 First filter to compose
 * @param f2 Second filter to compose
 * @return New filter representing composition of f1 and f2
 */
struct filter filter_composition(const struct filter* f1, const struct filter* f2);

/**
 * @brief Generates a random filter with specified size
 * @param size Size of the square filter matrix to generate
 * @return Randomly initialized filter structure
 */
struct filter generate_random_filter(int size);

/**
 * @brief Expands a filter by adding zero padding around it
 * @param original Filter to expand
 * @param padding Number of zero layers to add around original filter
 * @return Expanded filter with zero padding
 */
struct filter expand_filter(const struct filter* original, int padding);

/**
 * @brief Prepares image information structure for relaunch
 * @param info Pointer to image_info structure to prepare
 */
void prepare_image_for_relaunch(struct image_info* info);

/**
 * @brief Frees memory allocated for a filter structure
 * @param filter Pointer to filter structure to deallocate
 */
void free_filter(struct filter* filter);

/**
 * @brief Creates a deep copy of image_info structure
 * @param info Source image_info structure to copy
 * @return New copy of the image_info structure
 */
struct image_info copy_image_info(struct image_info* info);

/**
 * @brief Frees memory allocated for image_info structure
 * @param info Pointer to image_info structure to deallocate
 */
void free_image_info(struct image_info* info);
