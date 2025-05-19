#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unity.h>

#include <pct/filters.h>
#include <pct/utils.h>

#define MIN_IMG_SIZE 128
#define MAX_IMG_SIZE 1024
#define THREAD_NUM 8

void serial_convolution(struct image_info* info, struct filter filter);

// The functions for test
void pixelwise_convolution(struct image_info* info, struct filter filter, int num_threads);
void row_convolution(struct image_info* info, struct filter filter, int num_threads);
void column_convolution(struct image_info* info, struct filter filter, int num_threads);
void grid_convolution(struct image_info* info, struct filter filter, int num_threads);

void setUp(void) {}
void tearDown(void) {}

void parallel_convolution_test(int w, int h, int s,
                               void (*f)(struct image_info*, struct filter, int)) {
    struct image_info info1 = {
        .width = w,
        .height = h,
        .image = generate_image(w, h),
    };
    struct image_info info2 = copy_image_info(&info1);

    struct filter filter = generate_random_filter(s);

    serial_convolution(&info1, filter);
    f(&info2, filter, THREAD_NUM);

    TEST_ASSERT_EQUAL_MEMORY(info1.result, info2.result, w * h * sizeof(struct rgb_image));

    free_filter(&filter);

    free_image_info(&info1);
    free_image_info(&info2);
}

void run_pixelwise_convolution_test() {
    for (int image_size = MIN_IMG_SIZE; image_size <= MAX_IMG_SIZE; image_size += 128) {
        int s = 3 + 2 * (rand() % 7);
        printf("Run on %dx%d image with %d filter\n", image_size, image_size, s);
        parallel_convolution_test(image_size, image_size, s, pixelwise_convolution);
    }
}

void run_row_convolution_test() {
    for (int image_size = MIN_IMG_SIZE; image_size <= MAX_IMG_SIZE; image_size += 128) {
        int s = 3 + 2 * (rand() % 7);
        printf("Run on %dx%d image with %d filter\n", image_size, image_size, s);
        parallel_convolution_test(image_size, image_size, s, row_convolution);
    }
}

void run_column_convolution_test() {
    for (int image_size = MIN_IMG_SIZE; image_size <= MAX_IMG_SIZE; image_size += 128) {
        int s = 3 + 2 * (rand() % 7);
        printf("Run on %dx%d image with %d filter\n", image_size, image_size, s);
        parallel_convolution_test(image_size, image_size, s, column_convolution);
    }
}

void run_grid_convolution_test() {
    for (int image_size = MIN_IMG_SIZE; image_size <= MAX_IMG_SIZE; image_size += 128) {
        int s = 3 + 2 * (rand() % 7);
        printf("Run on %dx%d image with %d filter\n", image_size, image_size, s);
        parallel_convolution_test(image_size, image_size, s, grid_convolution);
    }
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(run_pixelwise_convolution_test);
    RUN_TEST(run_row_convolution_test);
    RUN_TEST(run_column_convolution_test);
    RUN_TEST(run_grid_convolution_test);
    return UNITY_END();
}
