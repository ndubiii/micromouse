#include <Arduino.h>
#include "Motor.h"
#include "Encoder.h"
#include "IRScanner.h"
#include "Sonar.h"
#include "PID.h"
#include "Planner.h"
#include "Motion.h"
#include "Mapper.h"
#include "MouseState.h"

// --- Hardware Drivers ---
ESP32MotorDriver lDr(18, 5), rDr(19, 17);
ESP32Encoder lEncDr(34, 35), rEncDr(32, 33);

// --- Sensors ---
IRScanner frontIR(36);
IRScanner leftIR(39);
IRScanner rightIR(38);
Sonar frontSonar(25, 26);

// --- Logic Setup ---
Motor lMotor(lDr), rMotor(rDr);
Encoder lEnc(lEncDr, 34.0, 360), rEnc(rEncDr, 34.0, 360);

PIDController pidL(1.0, 0.1, 0.05, -100, 100);
PIDController pidR(1.0, 0.1, 0.05, -100, 100);

Mapper mapper(leftIR, rightIR, frontSonar);
Planner planner;
Motion motion(lMotor, rMotor, lEnc, rEnc, pidL, pidR, leftIR, rightIR, frontSonar);

// ✅ ADD STATE MACHINE
StateMachine mouse(mapper, planner, motion);

// --- ISR Bridges ---
void IRAM_ATTR lISR() { lEncDr.handleInterrupt(); }
void IRAM_ATTR rISR() { rEncDr.handleInterrupt(); }

void setup()
{
    Serial.begin(115200);

    lDr.begin();
    rDr.begin();

    lEncDr.begin(lISR);
    rEncDr.begin(rISR);

    frontIR.begin();
    leftIR.begin();
    rightIR.begin();
    frontSonar.begin();

    Serial.println("Mouse ready.");
}

void loop()
{
    mouse.update();
}