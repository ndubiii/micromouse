#include "Encoder.h"

// -------------------- Encoder Logic --------------------
Encoder::Encoder(IEncoderDriver &d, float diameterMm, int tpr) : driver(d)
{
    _mmPerTick = (M_PI * diameterMm) / (float)tpr;
}

float Encoder::getDistance()
{
    return driver.getTicks() * _mmPerTick;
}

float Encoder::getVelocity(unsigned long deltaTimeMs)
{
    if (deltaTimeMs == 0)
        return 0; // Avoid division by zero

    long currentTicks = driver.getTicks();
    long deltaTicks = currentTicks - _lastTicks;
    _lastTicks = currentTicks;

    float distanceMoved = deltaTicks * _mmPerTick;
    return distanceMoved / (deltaTimeMs / 1000.0f); // mm/s
}

void Encoder::reset()
{
    driver.reset();
    _lastTicks = 0;
}

// -------------------- ESP32 Encoder Driver --------------------
ESP32Encoder::ESP32Encoder(int a, int b) : _pinA(a), _pinB(b) {}

void ESP32Encoder::begin(void (*isr)())
{
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_pinA), isr, RISING);
}

void ESP32Encoder::handleInterrupt()
{
    // Simple quadrature: phase B determines direction
    if (digitalRead(_pinB) == HIGH)
        _ticks++;
    else
        _ticks--;
}

long ESP32Encoder::getTicks()
{
    return _ticks;
}

void ESP32Encoder::reset()
{
    _ticks = 0;
}