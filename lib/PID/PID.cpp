#include "PID.h"
#include <Arduino.h>

PIDController::PIDController(float p, float i, float d, float min, float max)
    : _kP(p), _kI(i), _kD(d), _minOut(min), _maxOut(max), _integral(0), _lastError(0) {}

// -------------------- PID Calculation --------------------
float PIDController::calculate(float setpoint, float pv)
{
    float error = setpoint - pv;

    // Integrate error
    _integral += error;

    // Anti-windup: clamp integral
    if (_integral > _maxOut)
        _integral = _maxOut;
    if (_integral < _minOut)
        _integral = _minOut;

    // Derivative (change in error)
    float derivative = error - _lastError;
    _lastError = error;

    // PID output
    float output = (_kP * error) + (_kI * _integral) + (_kD * derivative);

    // Clamp output
    if (output > _maxOut)
        return _maxOut;
    if (output < _minOut)
        return _minOut;
    return output;
}

// -------------------- Reset --------------------
void PIDController::reset()
{
    _integral = 0;
    _lastError = 0;
}