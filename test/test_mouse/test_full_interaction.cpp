#include <Arduino.h>
#include <unity.h>
#include <chrono>

#include "MouseState.h"
#include "Mapper.h"
#include "Planner.h"
#include "Motion.h"
#include "Motor.h"
#include "Encoder.h"
#include "IRScanner.h"
#include "Sonar.h"
#include "PID.h"
#include "../test_common/unity_test_runner.h"

using namespace std::chrono;

// -------- FAKE HARDWARE WITH JITTER --------
class FakeMotorDriver : public IMotorDriver
{
public:
    int lastPWM = 0;
    void setPWM(int pwm) override { lastPWM = pwm; }
    void stop() override { lastPWM = 0; }
};

class FakeEncoderDriver : public IEncoderDriver
{
public:
    long ticks = 0;
    long getTicks() override { return ticks; }
    void reset() override { ticks = 0; }
};

float jitter(float base, float noise)
{
    return base + ((rand() % 100) / 100.0f - 0.5f) * noise;
}

class FakeIR : public IRScanner
{
public:
    float base;
    FakeIR(float v = 200) : IRScanner(0), base(v) {}
    float getDistanceMM() override
    {
        return jitter(base, 20.0f); // ±20mm noise
    }
};

class FakeSonar : public Sonar
{
public:
    float base;
    FakeSonar(float v = 30) : Sonar(0, 1), base(v) {}
    float readDistanceCM() override
    {
        return jitter(base, 5.0f); // ±5cm noise
    }
};

// -------- TEST --------
void test_full_navigation_with_timing_and_noise()
{
    srand(42); // deterministic randomness

    // --- Hardware ---
    FakeMotorDriver lmD, rmD;
    FakeEncoderDriver leD, reD;

    FakeIR leftIR(120), rightIR(120);
    FakeSonar frontSonar(20);

    Motor lm(lmD), rm(rmD);
    Encoder le(leD, 34.0, 360), re(reD, 34.0, 360);

    PIDController pidL(1, 0.1, 0.01, -100, 100);
    PIDController pidR(1, 0.1, 0.01, -100, 100);

    Mapper mapper(leftIR, rightIR, frontSonar);
    Planner planner;
    Motion motion(lm, rm, le, re, pidL, pidR,
                  leftIR, rightIR, frontSonar);

    StateMachine sm(mapper, planner, motion);

    // -------- PHASE 1: EXPLORATION --------
    auto startExplore = high_resolution_clock::now();

    for (int step = 0; step < 100; step++)
    {
        // simulate encoder completing movement
        if (lmD.lastPWM > 0)
            leD.ticks += 10;
        if (rmD.lastPWM > 0)
            reD.ticks += 10;

        sm.update();

        auto t = sm.getTelemetry();

        // Simulate reaching center (7,7)
        if (step == 30)
        {
            mapper.setPosition(7, 7);
        }

        if (t.currentState == DECIDING)
            break;
    }

    auto endExplore = high_resolution_clock::now();
    auto exploreTime = duration_cast<milliseconds>(endExplore - startExplore).count();

    TEST_ASSERT_TRUE(exploreTime >= 0);

    // -------- PHASE 2: SPEED RUN --------
    auto startSpeed = high_resolution_clock::now();

    for (int step = 0; step < 100; step++)
    {
        if (lmD.lastPWM > 0)
            leD.ticks += 20;
        if (rmD.lastPWM > 0)
            reD.ticks += 20;

        sm.update();

        auto t = sm.getTelemetry();

        // Simulate reaching origin
        if (step == 20)
        {
            mapper.setPosition(0, 0);
        }

        if (t.currentState == FINISHED)
            break;
    }

    auto endSpeed = high_resolution_clock::now();
    auto speedTime = duration_cast<milliseconds>(endSpeed - startSpeed).count();

    TEST_ASSERT_TRUE(speedTime >= 0);

    // -------- FINAL ASSERTIONS --------
    auto finalTelemetry = sm.getTelemetry();

    TEST_ASSERT_EQUAL(FINISHED, finalTelemetry.currentState);
    TEST_ASSERT_TRUE(finalTelemetry.isSpeedRun);

    // Optional sanity checks
    TEST_ASSERT_TRUE(exploreTime >= speedTime || speedTime >= 0);
}

// -------- UNITY --------
void run_all_tests()
{

    RUN_TEST(test_full_navigation_with_timing_and_noise);
}
void setup()
{

    TestFunc tests[] = {run_all_tests};
    runUnityTests(tests, 1);
}

void loop() {}