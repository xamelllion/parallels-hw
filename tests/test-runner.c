#include <unity.h>

extern void identity_serial_test();
extern void compose_filters_serial_test();

int main() {
    UNITY_BEGIN();
    RUN_TEST(identity_serial_test);
    RUN_TEST(compose_filters_serial_test);
    return UNITY_END();
}
