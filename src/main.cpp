#include <Arduino.h>
#include "Motor.h"
#include "Encoder.h"
#include "IRScanner.h"
#include "Sonar.h"
#include "PID.h"
#include "MazeNavigator.h"

// --- Hardware Drivers ---
ESP32MotorDriver lDr(18, 5), rDr(19, 17);
ESP32Encoder lEncDr(34, 35), rEncDr(32, 33);

// --- Sensors ---
IRScanner frontIR(36);    // front IR pin
IRScanner leftIR(39);     // left IR pin
IRScanner rightIR(38);    // right IR pin
Sonar frontSonar(25, 26); // trig, echo pins

// --- Logic Setup ---
Motor lMotor(lDr), rMotor(rDr);
Encoder lEnc(lEncDr, 34.0, 360), rEnc(rEncDr, 34.0, 360);

// PID for wheels
PIDController pidL(1.0, 0.1, 0.05, -100, 100);
PIDController pidR(1.0, 0.1, 0.05, -100, 100);

// Navigator with sensors & PID
Navigator nav(lMotor, rMotor, lEnc, rEnc,
              leftIR, rightIR, frontSonar,
              pidL, pidR);

// --- ISR Bridges for Encoders ---
void IRAM_ATTR lISR() { lEncDr.handleInterrupt(); }
void IRAM_ATTR rISR() { rEncDr.handleInterrupt(); }

void setup()
{
    Serial.begin(115200);

    // Init hardware
    lDr.begin();
    rDr.begin();
    lEncDr.begin(lISR);
    rEncDr.begin(rISR);

    // Optionally initialize sensors
    frontIR.begin();
    leftIR.begin();
    rightIR.begin();
    frontSonar.begin();

    Serial.println("Navigator ready.");
}

void loop()
{
    // --- Update wall map from sensors ---
    nav.updateWalls();

    // --- Compute BFS distance map to goal (center 7,7) ---
    auto dist = nav.runBFS(7, 7);

    // --- Step one move according to floodfill/BFS ---
    nav.step(dist);

    delay(50); // small delay for loop stability
}