#pragma once
#include <Arduino.h>

class IRScanner
{
public:
    IRScanner(int pin);
    virtual void begin();
    virtual float getDistanceMM();              // Smoothed distance in mm
    virtual float calculateDistanceMM(int raw); // For testing / PC

private:
    int _pin;
    const int _samples = 5; // Moving average samples
};