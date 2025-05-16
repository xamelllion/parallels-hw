#include <unity.h>

extern void test_1();

int main() {
    UNITY_BEGIN();
    RUN_TEST(test_1);
    return UNITY_END();
}
