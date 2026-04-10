#include <Arduino.h>
#include <unity.h>
#include "Sonar.h"
#include "../test_common/unity_test_runner.h"

// Unit test math logic (PC / simulation)
void test_distance_math()
{
    float duration = 1000; // microseconds
    float expected = (duration * 0.034) / 2.0;
    TEST_ASSERT_FLOAT_WITHIN(0.001, 17.0, expected);
}

void run_all_tests()
{
    RUN_TEST(test_distance_math);
}
void setup()
{

    TestFunc tests[] = {run_all_tests};
    runUnityTests(tests, 1);
}

void loop() {}