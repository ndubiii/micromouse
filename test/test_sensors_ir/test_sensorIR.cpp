#include <Arduino.h>
#include <unity.h>
#include "IRScanner.h"
#include "../test_common/unity_test_runner.h"

// Fake calculation testing (no hardware)
void test_calculateDistanceMM()
{
    IRScanner ir(0);
    TEST_ASSERT_EQUAL_FLOAT(100, ir.calculateDistanceMM(200));
    TEST_ASSERT_EQUAL_FLOAT(800, ir.calculateDistanceMM(800));
    TEST_ASSERT_EQUAL_FLOAT(450, ir.calculateDistanceMM(500)); // Mid-point
}
void run_all_tests()
{
    RUN_TEST(test_calculateDistanceMM);
}
void setup()
{
    TestFunc tests[] = {run_all_tests};
    runUnityTests(tests, 1);
}

void loop() {}