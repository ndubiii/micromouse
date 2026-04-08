#include <Arduino.h>
#include <unity.h>
#include "Motor.h"

// -------------------- Fake Driver --------------------
class FakeDriver : public IMotorDriver
{
public:
    int lastPWM = 0;
    int lastDir = 0; // 1 = HIGH, 0 = LOW
    void setPWM(int pwm) override
    {
        lastPWM = abs(pwm);
        lastDir = pwm >= 0 ? 1 : 0;
    }
    void stop() override
    {
        lastPWM = 0;
        lastDir = 0;
    }
};

// -------------------- Tests --------------------
void test_positive_speed()
{
    FakeDriver fake;
    Motor motor(fake);
    motor.setSpeed(100);
    TEST_ASSERT_EQUAL(255, fake.lastPWM);
    TEST_ASSERT_EQUAL(1, fake.lastDir);
}

void test_negative_speed()
{
    FakeDriver fake;
    Motor motor(fake);
    motor.setSpeed(-50);
    TEST_ASSERT_EQUAL(127, fake.lastPWM);
    TEST_ASSERT_EQUAL(0, fake.lastDir);
}

void test_stop()
{
    FakeDriver fake;
    Motor motor(fake);
    motor.setSpeed(100);
    motor.stop();
    TEST_ASSERT_EQUAL(0, fake.lastPWM);
    TEST_ASSERT_EQUAL(0, fake.lastDir);
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_positive_speed);
    RUN_TEST(test_negative_speed);
    RUN_TEST(test_stop);
    UNITY_END();
}

void loop() {}