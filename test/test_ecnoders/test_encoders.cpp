#include <Arduino.h>
#include <unity.h>
#include "../test_common/unity_test_runner.h"
#include "Encoder.h"

// -------------------- Fake Encoder --------------------
class FakeEncoderDriver : public IEncoderDriver
{
public:
    long ticks = 0;
    long getTicks() override { return ticks; }
    void reset() override { ticks = 0; }
};

// -------------------- Tests --------------------
void test_distance_calculation()
{
    FakeEncoderDriver fake;
    Encoder encoder(fake, 34.0, 360); // 34mm wheel, 360 ticks per rev

    fake.ticks = 360; // 1 full rotation
    TEST_ASSERT_FLOAT_WITHIN(0.1, 106.81, encoder.getDistance());
}

void test_velocity_calculation()
{
    FakeEncoderDriver fake;
    Encoder encoder(fake, 20.0, 200); // 20mm wheel, 200 ticks/rev

    fake.ticks = 0;
    encoder.reset();

    fake.ticks = 50;
    float vel = encoder.getVelocity(500); // 50 ticks in 0.5s

    float expected = (50 * (M_PI * 20.0 / 200)) / 0.5; // mm/s
    TEST_ASSERT_FLOAT_WITHIN(0.1, expected, vel);
}

void test_reset()
{
    FakeEncoderDriver fake;
    Encoder encoder(fake, 10.0, 100);
    fake.ticks = 50;
    encoder.reset();
    TEST_ASSERT_EQUAL(0, fake.getTicks());
}

void run_all_tests()
{
    RUN_TEST(test_distance_calculation);
    RUN_TEST(test_velocity_calculation);
    RUN_TEST(test_reset);
}

void setup()
{

    TestFunc tests[] = {run_all_tests};
    runUnityTests(tests, 1);
}

void loop() {}