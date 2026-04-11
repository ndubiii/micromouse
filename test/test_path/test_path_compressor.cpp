#include <Arduino.h>
#include <unity.h>

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

// -------- FAKE HARDWARE --------
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

class FakeIR : public IRScanner
{
public:
    float fakeVal;
    FakeIR(float v = 200) : IRScanner(0), fakeVal(v) {}
    float getDistanceMM() override { return fakeVal; }
};

class FakeSonar : public Sonar
{
public:
    float fakeVal;
    FakeSonar(float v = 30) : Sonar(0, 1), fakeVal(v) {}
    float readDistanceCM() override { return fakeVal; }
};

// -------- TEST 6: COMPRESSION IMPACT (OKR TEST) --------
void test_path_compression_vs_old_planner()
{
    FakeMotorDriver lmD, rmD;
    FakeEncoderDriver leD, reD;
    FakeIR leftIR(200), rightIR(200);
    FakeSonar frontSonar(50);

    Motor lm(lmD), rm(rmD);
    Encoder le(leD, 34.0, 360), re(reD, 34.0, 360);

    PIDController pidL(1, 0, 0, -100, 100);
    PIDController pidR(1, 0, 0, -100, 100);

    Mapper mapper(leftIR, rightIR, frontSonar);

    Planner planner;
    Motion motion(lm, rm, le, re, pidL, pidR,
                  leftIR, rightIR, frontSonar);

    StateMachine sm(mapper, planner, motion);

    // ----------------------------
    // Simulated run metrics
    // ----------------------------

    int oldTurns = 0;
    int newTurns = 0;

    int oldLastDir = -1;
    int newLastDir = -1;

    int oldSteps = 0;
    int newSteps = 0;

    // ----------------------------
    // OLD PLANNER SIMULATION
    // ----------------------------
    for (int i = 0; i < 20; i++)
    {
        int dir = planner.chooseSpeedDirection(mapper, 0);

        if (dir != oldLastDir && oldLastDir != -1)
            oldTurns++;

        oldLastDir = dir;
        oldSteps++;
    }

    // ----------------------------
    // NEW PLANNER SIMULATION
    // ----------------------------
    for (int i = 0; i < 20; i++)
    {
        int dir = planner.getCompressedDirection(mapper, 0);

        if (dir != newLastDir && newLastDir != -1)
            newTurns++;

        newLastDir = dir;
        newSteps++;
    }

    TEST_ASSERT_TRUE(newTurns <= oldTurns);

    TEST_ASSERT_TRUE(newSteps <= oldSteps);
    TEST_ASSERT_TRUE(newLastDir != -1);
}

// -------- UNITY --------
void run_all_tests()
{
    RUN_TEST(test_path_compression_vs_old_planner);
}

void setup()
{

    TestFunc tests[] = {run_all_tests};
    runUnityTests(tests, 1);
}

void loop() {}