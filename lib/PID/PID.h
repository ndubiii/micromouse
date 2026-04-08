#pragma once

class PIDController
{
public:
    PIDController(float kP, float kI, float kD, float minOutput, float maxOutput);

    // Calculates the PID output given a setpoint and process variable
    float calculate(float setpoint, float pv);

    // Reset integral and previous error
    void reset();

private:
    float _kP, _kI, _kD;
    float _minOut, _maxOut;
    float _integral;
    float _lastError;
};