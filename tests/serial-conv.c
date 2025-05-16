
#include "utils.h"

#include <stdlib.h>
#include <unity.h>

#include <pct/filters.h>
#include <pct/utils.h>

// Тестируемая функция
void serial_convolution(struct image_info* info, struct filter filter);

void setUp(void) {}
void tearDown(void) {}

void test_1() {
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

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_1);

    return UNITY_END();
}
