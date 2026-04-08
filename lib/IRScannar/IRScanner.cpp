#include "IRScanner.h"

// -------------------- Constructor --------------------
IRScanner::IRScanner(int pin) : _pin(pin) {}

// -------------------- Initialize pin --------------------
void IRScanner::begin()
{
    pinMode(_pin, INPUT);
}

// -------------------- Raw analog to mm (curve / simple map) --------------------
float IRScanner::calculateDistanceMM(int raw)
{
    // Example linear approximation: 200–800 raw -> 100–800 mm
    return map(constrain(raw, 200, 800), 200, 800, 100, 800);
}

// -------------------- Smoothed distance reading --------------------
float IRScanner::getDistanceMM()
{
    int sum = 0;
    for (int i = 0; i < _samples; i++)
    {
        sum += analogRead(_pin);
        delay(2); // small delay to stabilize reading
    }
    int avg = sum / _samples;
    return calculateDistanceMM(avg);
}