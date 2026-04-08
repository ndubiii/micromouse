#include <Arduino.h>
#include <unity.h>
#include "MazeNavigator.h"
#include "Motor.h"
#include "Encoder.h"
#include "IRScanner.h"
#include "Sonar.h"
#include "PID.h"

// --- Fake hardware ---
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

// --- Tests ---
void test_full_maze_run()
{
    FakeMotorDriver flM, frM;
    FakeEncoderDriver flE, frE;
    FakeIR leftIR, rightIR;
    FakeSonar frontSonar;

    Motor leftMotor(flM), rightMotor(frM);
    Encoder leftEnc(flE, 34.0, 360), rightEnc(frE, 34.0, 360);

    PIDController pidL(1.0, 0.1, 0.05, -100, 100);
    PIDController pidR(1.0, 0.1, 0.05, -100, 100);

    Navigator nav(leftMotor, rightMotor, leftEnc, rightEnc,
                  leftIR, rightIR, frontSonar, pidL, pidR);

    for (int i = 0; i < 7; i++)
    {
        flE.ticks = 180;
        frE.ticks = 180;
        nav.moveOneCell();
    }

    TEST_ASSERT_EQUAL_INT(7, nav.getX() + nav.getY());
}

void setup()
{
    UNITY_BEGIN();
    RUN_TEST(test_full_maze_run);
    UNITY_END();
}

void loop() {}