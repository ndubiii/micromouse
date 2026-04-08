#pragma once
#include <Arduino.h>
#include <math.h>

// -------------------- Encoder Interface --------------------
class IEncoderDriver
{
public:
    virtual long getTicks() = 0;
    virtual void reset() = 0;
    virtual ~IEncoderDriver() {}
};

// -------------------- Encoder Logic --------------------
class Encoder
{
public:
    Encoder(IEncoderDriver &driver, float diameterMm, int ticksPerRevolution);
    float getDistance();                          // Returns distance in mm
    float getVelocity(unsigned long deltaTimeMs); // Velocity in mm/s
    void reset();

private:
    IEncoderDriver &driver;
    float _mmPerTick;
    long _lastTicks = 0; // For velocity calculation
};

// -------------------- ESP32 Encoder Driver --------------------
class ESP32Encoder : public IEncoderDriver
{
public:
    ESP32Encoder(int pinA, int pinB);
    void begin(void (*isr)());
    void handleInterrupt(); // ISR wrapper calls this
    long getTicks() override;
    void reset() override;

private:
    int _pinA, _pinB;
    volatile long _ticks = 0;
};