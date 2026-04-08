#include "motor_driver.h"
#include "pins.h"
#include <Arduino.h>

void MotorDriver::init() {
    pinMode(MOTOR_LEFT_PWM, OUTPUT);
    pinMode(MOTOR_LEFT_DIR, OUTPUT);
    pinMode(MOTOR_RIGHT_PWM, OUTPUT);
    pinMode(MOTOR_RIGHT_DIR, OUTPUT);
}

void MotorDriver::setSpeed(int left, int right) {
    // TODO: implement direction + PWM control
}