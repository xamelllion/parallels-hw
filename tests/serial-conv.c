
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

#include <pct/filters.h>
#include <pct/utils.h>

// Тестируемая функция
void serial_convolution(struct image_info* info, struct filter filter);

void setUp(void) {}
void tearDown(void) {}

void identity_serial_test() {
    struct filter* filters = init_filters();

    struct image_info info = {
        .width = 400,
        .height = 300,
        .image = generate_image(400, 300),
    };

    serial_convolution(&info, filters[id_filter_type]);

    TEST_ASSERT_EQUAL_MEMORY(info.image, info.result,
                             info.width * info.height * sizeof(struct rgb_image));

    free_filters(filters);
    free(info.image);
    free(info.result);
}

void compose_filters_with_parameters(int w, int h, int s1, int s2) {
    struct image_info info1 = {
        .width = w,
        .height = h,
        .image = generate_image(w, h),
    };

    struct image_info info2 = {
        .width = w,
        .height = h,
        .image = malloc(w * h * sizeof(struct rgb_image)),
    };
    memcpy(info2.image, info1.image, w * h * sizeof(struct rgb_image));

    struct filter filter1 = generate_random_filter(s1);
    struct filter filter2 = generate_random_filter(s2);

    struct filter composed = filter_composition(&filter1, &filter2);

    serial_convolution(&info1, filter1);
    free(info1.image);
    info1.image = info1.result;
    serial_convolution(&info1, filter2);

    serial_convolution(&info2, composed);

    TEST_ASSERT_TRUE(compare_images(info1.result, info2.result, w, h));

    free_filter(&filter1);
    free_filter(&filter2);
    free_filter(&composed);

    free(info1.image);
    free(info1.result);
    free(info2.image);
    free(info2.result);
}

void compose_filters_serial_test() {
    for (int image_size = 128; image_size <= 1024; image_size += 128) {
        int s1 = 3 + 2 * (rand() % 7);
        int s2 = 3 + 2 * (rand() % 7);
        printf("Run on %dx%d image with %d and %d filters\n", image_size, image_size, s1, s2);
        compose_filters_with_parameters(image_size, image_size, s1, s2);
    }
}
