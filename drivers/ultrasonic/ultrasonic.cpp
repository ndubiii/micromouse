#include "ultrasonic.h"
#include "pins.h"
#include <Arduino.h>

static float distance = 0;

void Ultrasonic::init() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void Ultrasonic::read() {
    // TODO: trigger + measure echo pulse

    // NOTE:
    // This works WITH IR sensors:
    // Use ultrasonic only for front wall confirmation
}

float Ultrasonic::getDistance() {
    return distance;
}