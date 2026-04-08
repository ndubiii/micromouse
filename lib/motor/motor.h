#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

// -------------------- Motor Driver Interface --------------------
class IMotorDriver
{
public:
    virtual void setPWM(int pwm) = 0; // pwm: -255 to 255
    virtual void stop() = 0;
    virtual ~IMotorDriver() {}
};

// -------------------- ESP32 Motor Driver --------------------
class ESP32MotorDriver : public IMotorDriver
{
public:
    ESP32MotorDriver(int pwmPin, int dirPin, int channel = 0);
    void begin();
    void setPWM(int pwm) override;
    void stop() override;

private:
    int _pwmPin, _dirPin, _channel;
};

// -------------------- Motor Logic --------------------
class Motor
{
public:
    Motor(IMotorDriver &driver);
    void setSpeed(int percent); // percent: -100 to 100
    void stop();

private:
    IMotorDriver &_driver;
};

#endif