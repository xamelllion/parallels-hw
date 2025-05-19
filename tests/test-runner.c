#include <unity.h>

extern void run_identity_serial_test();
extern void run_filter_composition_test();
extern void run_compose_filters_into_identity_test();
extern void run_zero_padding_filter_test();

int main() {
    UNITY_BEGIN();
    RUN_TEST(run_identity_serial_test);
    RUN_TEST(run_filter_composition_test);
    RUN_TEST(run_compose_filters_into_identity_test);
    RUN_TEST(run_zero_padding_filter_test);
    return UNITY_END();
}
