#pragma once
#include <Arduino.h>

class Sonar
{
public:
    Sonar(int trigPin, int echoPin);
    virtual void begin();
    virtual float readDistanceCM();  // Smoothed distance
    virtual float singleReadingCM(); // One-shot reading, used internally & for tests

private:
    int _trig, _echo;
    const int _samples = 3; // Moving average
};