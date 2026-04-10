#include "Motion.h"
#include <math.h>

Motion::Motion(Motor &lM, Motor &rM,
               Encoder &lE, Encoder &rE,
               PIDController &pidL,
               PIDController &pidR,
               IRScanner &left,
               IRScanner &right,
               Sonar &front)
    : _lM(lM), _rM(rM),
      _lE(lE), _rE(rE),
      _pidL(pidL), _pidR(pidR),
      _leftIR(left), _rightIR(right), _front(front)
{
}

void Motion::startMove(float distMM)
{
    _targetDist = distMM;
    _lE.reset();
    _rE.reset();
    _pidL.reset();
    _pidR.reset();

    _state = MOVING;
    _busy = true;
}
void Motion::startTurn(float degrees, bool left)
{
    _targetAngle = degrees;
    _turnLeft = left;

    _lE.reset();
    _rE.reset();
    _pidL.reset();
    _pidR.reset();

    _state = TURNING;
    _busy = true;
}
void Motion::update()
{
    switch (_state)
    {
    case MOVING:
        updateMove();
        break;

    case TURNING:
        updateTurn();
        break;

    case IDLE:
        _busy = false;
        break;
    }
}
void Motion::updateMove()
{
    if (_lE.getDistance() >= _targetDist &&
        _rE.getDistance() >= _targetDist)
    {
        _lM.stop();
        _rM.stop();

        _state = IDLE;
        return;
    }

    float leftDist = _leftIR.getDistanceMM();
    float rightDist = _rightIR.getDistanceMM();

    float error = (leftDist - rightDist);
    float correction = 0.5f * error;

    float baseL = _pidL.calculate(_targetDist, _lE.getDistance());
    float baseR = _pidR.calculate(_targetDist, _rE.getDistance());

    _lM.setSpeed(baseL - correction);
    _rM.setSpeed(baseR + correction);
}

void Motion::updateTurn()
{
    float angleDone = _lE.getDistance(); // assume encoder-to-angle mapping

    if (fabs(angleDone) >= fabs(_targetAngle))
    {
        _lM.stop();
        _rM.stop();

        _state = IDLE;
        return;
    }

    float l = _pidL.calculate(_targetAngle, _lE.getDistance());
    float r = _pidR.calculate(_targetAngle, _rE.getDistance());

    _lM.setSpeed(l);
    _rM.setSpeed(r);
}

void Motion::stop()
{
    _lM.stop();
    _rM.stop();
    _state = IDLE;
    _busy = false;
}
float Motion::getFrontDistance()
{
    return _front.readDistanceCM();
}
float Motion::getLeftDistance()
{
    return _leftIR.getDistanceMM();
}
float Motion::getRightDistance()
{
    return _rightIR.getDistanceMM();
}