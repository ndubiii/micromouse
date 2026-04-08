#include "Motor.h"

// -------------------- Motor Logic --------------------
Motor::Motor(IMotorDriver &driver) : _driver(driver) {}

void Motor::setSpeed(int percent)
{
    percent = constrain(percent, -100, 100);
    _driver.setPWM((percent * 255) / 100);
}

void Motor::stop()
{
    _driver.stop();
}

// -------------------- ESP32 Motor Driver --------------------
ESP32MotorDriver::ESP32MotorDriver(int pwmPin, int dirPin, int channel)
    : _pwmPin(pwmPin), _dirPin(dirPin), _channel(channel) {}

void ESP32MotorDriver::begin()
{
    pinMode(_dirPin, OUTPUT);
    ledcSetup(_channel, 20000, 8); // 20 kHz PWM, 8-bit resolution
    ledcAttachPin(_pwmPin, _channel);
}

void ESP32MotorDriver::setPWM(int pwm)
{
    if (pwm >= 0)
    {
        digitalWrite(_dirPin, HIGH);
        ledcWrite(_channel, min(pwm, 255));
    }
    else
    {
        digitalWrite(_dirPin, LOW);
        ledcWrite(_channel, min(-pwm, 255));
    }
}

void ESP32MotorDriver::stop()
{
    ledcWrite(_channel, 0);
}