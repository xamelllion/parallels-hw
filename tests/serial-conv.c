#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unity.h>

#include <pct/filters.h>
#include <pct/utils.h>

#define MIN_IMG_SIZE 128
#define MAX_IMG_SIZE 1024

// The function for test
void serial_convolution(struct image_info* info, struct filter filter);

void setUp(void) {}
void tearDown(void) {}

void identity_serial_test(int w, int h) {
    struct filter* filters = init_filters();

    struct image_info info = {
        .width = w,
        .height = h,
        .image = generate_image(w, h),
    };

    serial_convolution(&info, filters[id_filter_type]);

    TEST_ASSERT_EQUAL_MEMORY(info.image, info.result, w * h * sizeof(struct rgb_image));

    free_filters(filters);
    free_image_info(&info);
}

void filter_composition_test(int w, int h, int s1, int s2) {
    struct image_info info1 = {
        .width = w,
        .height = h,
        .image = generate_image(w, h),
    };

    struct image_info info2 = copy_image_info(&info1);

    struct filter filter1 = generate_random_filter(s1);
    struct filter filter2 = generate_random_filter(s2);

    struct filter composed = filter_composition(&filter1, &filter2);

    serial_convolution(&info1, filter1);
    prepare_image_for_relaunch(&info1);
    serial_convolution(&info1, filter2);

    serial_convolution(&info2, composed);

    TEST_ASSERT_TRUE(compare_images(info1.result, info2.result, w, h));

    free_filter(&filter1);
    free_filter(&filter2);
    free_filter(&composed);

    free_image_info(&info1);
    free_image_info(&info2);
}

void compose_filters_into_identity_test(int w, int h) {
    struct filter* filters = init_filters();

    struct image_info info1 = {
        .width = w,
        .height = h,
        .image = generate_image(w, h),
    };

    struct image_info info2 = copy_image_info(&info1);

    const double shift_right[3][3] = {{0, 0, 0}, {0, 0, 1}, {0, 0, 0}};
    const double shift_left[3][3] = {{0, 0, 0}, {1, 0, 0}, {0, 0, 0}};

    struct filter right = create_filter(3, 1.0, 0.0, shift_right);
    struct filter left = create_filter(3, 1.0, 0.0, shift_left);

    struct filter composed = filter_composition(&right, &left);

    serial_convolution(&info1, right);
    prepare_image_for_relaunch(&info1);
    serial_convolution(&info1, left);

    serial_convolution(&info2, filters[id_filter_type]);

    TEST_ASSERT_TRUE(compare_images(info1.result, info2.result, w, h));

    free_filter(&right);
    free_filter(&left);
    free_filter(&composed);

    free_image_info(&info1);
    free_image_info(&info2);
}

void zero_padding_filter_test(int w, int h) {
    struct image_info info1 = {
        .width = w,
        .height = h,
        .image = generate_image(w, h),
    };

    struct image_info info2 = copy_image_info(&info1);

    struct filter filter1 = generate_random_filter(5);
    struct filter filter2 = expand_filter(&filter1, 1);

    serial_convolution(&info1, filter1);
    serial_convolution(&info2, filter2);

    TEST_ASSERT_TRUE(compare_images(info1.result, info2.result, w, h));

    free_filter(&filter1);
    free_filter(&filter2);

    free_image_info(&info1);
    free_image_info(&info2);
}

void run_identity_serial_test() {
    for (int image_size = MIN_IMG_SIZE; image_size <= MAX_IMG_SIZE; image_size += 128) {
        printf("Run on %dx%d image\n", image_size, image_size);
        identity_serial_test(image_size, image_size);
    }
}

void run_filter_composition_test() {
    for (int image_size = MIN_IMG_SIZE; image_size <= MAX_IMG_SIZE; image_size += 128) {
        int s1 = 3 + 2 * (rand() % 7);
        int s2 = 3 + 2 * (rand() % 7);
        printf("Run on %dx%d image with %d and %d filters\n", image_size, image_size, s1, s2);
        filter_composition_test(image_size, image_size, s1, s2);
    }
}

void run_compose_filters_into_identity_test() {
    for (int image_size = MIN_IMG_SIZE; image_size <= MAX_IMG_SIZE; image_size += 128) {
        printf("Run on %dx%d image\n", image_size, image_size);
        compose_filters_into_identity_test(image_size, image_size);
    }
}

void run_zero_padding_filter_test() {
    for (int image_size = MIN_IMG_SIZE; image_size <= MAX_IMG_SIZE; image_size += 128) {
        printf("Run on %dx%d image\n", image_size, image_size);
        zero_padding_filter_test(image_size, image_size);
    }
}

int main() {
    UNITY_BEGIN();
    RUN_TEST(run_identity_serial_test);
    RUN_TEST(run_filter_composition_test);
    RUN_TEST(run_compose_filters_into_identity_test);
    RUN_TEST(run_zero_padding_filter_test);
    return UNITY_END();
}
