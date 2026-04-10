#include <Arduino.h>
#include <unity.h>
#include "PID.h"
#include "../test_common/unity_test_runner.h"

void test_pid_basic_response()
{
    PIDController pid(1.0, 0.1, 0.05, -100, 100);

    // Simple proportional test
    float output = pid.calculate(50, 0); // error = 50
    TEST_ASSERT(output > 0 && output <= 100);

    // Check anti-windup
    pid.calculate(1000, 0); // large error
    float o2 = pid.calculate(1000, 0);
    TEST_ASSERT(o2 <= 100); // Should be clamped
}

void test_pid_reset()
{
    PIDController pid(1.0, 0.1, 0.05, -50, 50);
    pid.calculate(30, 0);
    pid.reset();
    float output = pid.calculate(0, 0); // After reset, integral = 0
    TEST_ASSERT_EQUAL_FLOAT(0, output);
}
void run_all_tests()
{
    RUN_TEST(test_pid_basic_response);
    RUN_TEST(test_pid_reset);
}
void setup()
{
    TestFunc tests[] = {run_all_tests};
    runUnityTests(tests, 1);
}

void loop() {}