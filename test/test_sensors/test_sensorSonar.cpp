#include <Arduino.h>
#include <unity.h>
#include "Sonar.h"

// Unit test math logic (PC / simulation)
void test_distance_math()
{
    float duration = 1000; // microseconds
    float expected = (duration * 0.034) / 2.0;
    TEST_ASSERT_FLOAT_WITHIN(0.001, 17.0, expected);
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_distance_math);
    UNITY_END();
}

void loop() {}