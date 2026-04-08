#include "ir_sensors.h"
#include "pins.h"
#include <Arduino.h>

static int leftVal = 0;
static int rightVal = 0;

void IRSensors::init() {
    pinMode(IR_LEFT, INPUT);
    pinMode(IR_RIGHT, INPUT);
}

void IRSensors::read() {
    leftVal = analogRead(IR_LEFT);
    rightVal = analogRead(IR_RIGHT);

    // TODO: apply filtering
}

int IRSensors::getLeft() { return leftVal; }
int IRSensors::getRight() { return rightVal; }