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

// -------- HELPER --------
State getState(StateMachine &sm)
{
    return sm.getTelemetry().currentState;
}

// -------- TEST 1: START → SEARCH --------
void test_start_to_search_transition()
{
    FakeMotorDriver lmD, rmD;
    FakeEncoderDriver leD, reD;
    FakeIR leftIR(200), rightIR(200);
    FakeSonar frontSonar(5);

    Motor lm(lmD), rm(rmD);
    Encoder le(leD, 34.0, 360), re(reD, 34.0, 360);

    PIDController pidL(1, 0, 0, -100, 100);
    PIDController pidR(1, 0, 0, -100, 100);

    Mapper mapper(leftIR, rightIR, frontSonar);
    Planner planner;
    Motion motion(lm, rm, le, re, pidL, pidR,
                  leftIR, rightIR, frontSonar);

    StateMachine sm(mapper, planner, motion);

    TEST_ASSERT_EQUAL(STARTING, getState(sm));

    sm.update();

    TEST_ASSERT_EQUAL(SEARCHING, getState(sm));
}

// -------- TEST 2: SEARCHING runs --------
void test_searching_runs_without_crash()
{
    FakeMotorDriver lmD, rmD;
    FakeEncoderDriver leD, reD;
    FakeIR leftIR(200), rightIR(200);
    FakeSonar frontSonar(5);

    Motor lm(lmD), rm(rmD);
    Encoder le(leD, 34.0, 360), re(reD, 34.0, 360);

    PIDController pidL(1, 0, 0, -100, 100);
    PIDController pidR(1, 0, 0, -100, 100);

    Mapper mapper(leftIR, rightIR, frontSonar);
    Planner planner;
    Motion motion(lm, rm, le, re, pidL, pidR,
                  leftIR, rightIR, frontSonar);

    StateMachine sm(mapper, planner, motion);

    sm.update(); // START → SEARCH

    for (int i = 0; i < 5; i++)
    {
        leD.ticks = 200;
        reD.ticks = 200;
        sm.update();
    }

    State s = getState(sm);

    TEST_ASSERT_TRUE(s == SEARCHING || s == DECIDING);
}

// -------- TEST 3: TELEMETRY VALID --------
void test_telemetry_values()
{
    FakeMotorDriver lmD, rmD;
    FakeEncoderDriver leD, reD;
    FakeIR leftIR(120), rightIR(140);
    FakeSonar frontSonar(25);

    Motor lm(lmD), rm(rmD);
    Encoder le(leD, 34.0, 360), re(reD, 34.0, 360);

    PIDController pidL(1, 0, 0, -100, 100);
    PIDController pidR(1, 0, 0, -100, 100);

    Mapper mapper(leftIR, rightIR, frontSonar);
    Planner planner;
    Motion motion(lm, rm, le, re, pidL, pidR,
                  leftIR, rightIR, frontSonar);

    StateMachine sm(mapper, planner, motion);

    sm.update();

    auto t = sm.getTelemetry();

    TEST_ASSERT_TRUE(t.distLeft > 0);
    TEST_ASSERT_TRUE(t.distRight > 0);
    TEST_ASSERT_TRUE(t.distFront > 0);
}

// -------- TEST 4: SPEED RUN FLAG --------
void test_speed_run_flag_behavior()
{
    FakeMotorDriver lmD, rmD;
    FakeEncoderDriver leD, reD;
    FakeIR leftIR(200), rightIR(200);
    FakeSonar frontSonar(5);

    Motor lm(lmD), rm(rmD);
    Encoder le(leD, 34.0, 360), re(reD, 34.0, 360);

    PIDController pidL(1, 0, 0, -100, 100);
    PIDController pidR(1, 0, 0, -100, 100);

    Mapper mapper(leftIR, rightIR, frontSonar);
    Planner planner;
    Motion motion(lm, rm, le, re, pidL, pidR,
                  leftIR, rightIR, frontSonar);

    StateMachine sm(mapper, planner, motion);

    sm.update(); // → SEARCHING

    auto t = sm.getTelemetry();
    TEST_ASSERT_FALSE(t.isSpeedRun);

    for (int i = 0; i < 20; i++)
    {
        leD.ticks = 200;
        reD.ticks = 200;
        sm.update();
    }

    t = sm.getTelemetry();

    if (t.currentState == SPEED_RUN)
        TEST_ASSERT_TRUE(t.isSpeedRun);
}

// -------- TEST 5: VALID STATE PROGRESSION --------
void test_valid_state_progression()
{
    FakeMotorDriver lmD, rmD;
    FakeEncoderDriver leD, reD;
    FakeIR leftIR(200), rightIR(200);
    FakeSonar frontSonar(5);

    Motor lm(lmD), rm(rmD);
    Encoder le(leD, 34.0, 360), re(reD, 34.0, 360);

    PIDController pidL(1, 0, 0, -100, 100);
    PIDController pidR(1, 0, 0, -100, 100);

    Mapper mapper(leftIR, rightIR, frontSonar);
    Planner planner;
    Motion motion(lm, rm, le, re, pidL, pidR,
                  leftIR, rightIR, frontSonar);

    StateMachine sm(mapper, planner, motion);

    for (int i = 0; i < 30; i++)
    {
        leD.ticks = 200;
        reD.ticks = 200;
        sm.update();
    }

    State s = getState(sm);

    TEST_ASSERT_TRUE(
        s == SEARCHING ||
        s == DECIDING ||
        s == SPEED_RUN ||
        s == FINISHED);
}

// -------- UNITY --------
void run_all_tests()
{
    RUN_TEST(test_start_to_search_transition);
    RUN_TEST(test_searching_runs_without_crash);
    RUN_TEST(test_telemetry_values);
    RUN_TEST(test_speed_run_flag_behavior);
    RUN_TEST(test_valid_state_progression);
}
void setup()
{

    TestFunc tests[] = {run_all_tests};
    runUnityTests(tests, 1);
}

void loop() {}