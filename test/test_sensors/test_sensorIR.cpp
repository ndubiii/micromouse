#include <Arduino.h>
#include <unity.h>
#include "IRScanner.h"

// Fake calculation testing (no hardware)
void test_calculateDistanceMM()
{
    IRScanner ir(0);
    TEST_ASSERT_EQUAL_FLOAT(100, ir.calculateDistanceMM(200));
    TEST_ASSERT_EQUAL_FLOAT(800, ir.calculateDistanceMM(800));
    TEST_ASSERT_EQUAL_FLOAT(450, ir.calculateDistanceMM(500)); // Mid-point
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_calculateDistanceMM);
    UNITY_END();
}

void loop() {}