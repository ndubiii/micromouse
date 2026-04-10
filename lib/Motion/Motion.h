#pragma once
#include "Motor.h"
#include "Encoder.h"
#include "PID.h"
#include "IRScanner.h"
#include "Sonar.h"

class Motion
{
public:
    Motion(Motor &lM, Motor &rM,
           Encoder &lE, Encoder &rE,
           PIDController &pidL,
           PIDController &pidR,
           IRScanner &left,
           IRScanner &right,
           Sonar &front);

    void startMove(float distMM);
    void startTurn(float degrees, bool left);

    void update();
    void stop();
    float getFrontDistance();
    float getLeftDistance();
    float getRightDistance();
    bool isBusy() const { return _busy; }

private:
    enum MotionState
    {
        IDLE,
        MOVING,
        TURNING
    };

    MotionState _state = IDLE;
    bool _busy = false;

    float _targetDist = 0;
    float _targetAngle = 0;
    bool _turnLeft = false;

    Motor &_lM, &_rM;
    Encoder &_lE, &_rE;
    PIDController &_pidL, &_pidR;
    IRScanner &_leftIR, &_rightIR;
    Sonar &_front;

    void updateMove();
    void updateTurn();
};