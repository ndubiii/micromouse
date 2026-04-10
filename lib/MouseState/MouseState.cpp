#include "MouseState.h"

StateMachine::StateMachine(Mapper &m, Planner &p, Motion &mo)
    : _mapper(m), _planner(p), _motion(mo), _currentState(STARTING)
{
    _pathDirty = true;
    _commandIssued = false;
    _data.currentState = STARTING;
    _data.isSpeedRun = false;
    _data.x = 0;
    _data.y = 0;
    _data.heading = 0;
    _data.distLeft = 0;
    _data.distRight = 0;
    _data.distFront = 0;
    _data.targetX = 7;
    _data.targetY = 7;
    _data.nextDir = -1;
}

void StateMachine::update()
{
    // 🔥 ALWAYS STEP MOTION FIRST
    _motion.update();

      if (_motion.isBusy())
        _commandIssued = false;
    int chosenDir = -1;

    switch (_currentState)
    {
    // -------------------------------------------------
    case STARTING:
    {
        _mapper.updateWalls();

        if (_motion.getFrontDistance() < 100)
        {
            _currentState = SEARCHING;
            _pathDirty = true;
        }
        break;
    }

    // -------------------------------------------------
    case SEARCHING:
    {
        _mapper.updateWalls();
        _pathDirty = true;

        // UPDATE PATH ONLY IF IDLE
        if (_pathDirty && !_motion.isBusy())
        {
            _planner.runBFS(_mapper, 7, 7);
            _pathDirty = false;
        }

        // MOVE ONLY WHEN FREE
        if (!_motion.isBusy() && !_commandIssued)
        {
            chosenDir = _planner.chooseSpeedDirection(
                _mapper,
                _mapper.getHeading());

            executeMove(chosenDir, false);

            _mapper.moveForward();
            _pathDirty = true;
            _commandIssued = true;
        }

        if (isAt(7, 7))
        {
            _currentState = DECIDING;
        }

        break;
    }

    // -------------------------------------------------
    case DECIDING:
    {
        // Start turn ONCE
        if (!_motion.isBusy())
        {
            _motion.startTurn(180.0f, true);
            _mapper.turnAround();

            _currentState = SPEED_RUN;
            _pathDirty = true;
        }

        break;
    }

    // -------------------------------------------------
    case SPEED_RUN:
    {
        if (_pathDirty && !_motion.isBusy())
        {
            _planner.runBFS(_mapper, 0, 0);
            _pathDirty = false;
        }

        if (!_motion.isBusy() && !_commandIssued)
        {
            chosenDir = _planner.chooseSpeedDirection(
                _mapper,
                _mapper.getHeading());

            executeMove(chosenDir, true);

            _mapper.moveForward();
            _pathDirty = true;
            _commandIssued = true;
        }

        if (isAt(0, 0))
        {
            _currentState = FINISHED;
        }

        break;
    }
    case RETURNING:
    {
        if (!_motion.isBusy() && !_commandIssued)
        {
            _motion.startTurn(180.0f, true);
            _commandIssued = true;
            break;
        }
        if (_motion.isBusy())
            _commandIssued = false;

        if (_pathDirty && !_motion.isBusy())
        {
            _planner.runBFS(_mapper, 0, 0);
            _pathDirty = false;
        }
        if (!_motion.isBusy() && !_commandIssued)
        {
            int dir = _planner.chooseSpeedDirection(
                _mapper,
                _mapper.getHeading());

            executeMove(dir, false);
            _mapper.moveForward();
            _pathDirty = true;
            _commandIssued = true;
        }
        if (isAt(0, 0))
        {
            _currentState = SPEED_RUN;
            _pathDirty = true;
        }
        break;
    }
    // -------------------------------------------------
    case FINISHED:
    {
        _motion.stop();
        break;
    }
    }
    captureInternalState(chosenDir);
}

// =====================================================
// NON-BLOCKING MOVE COMMAND BUILDER
// =====================================================
void StateMachine::executeMove(int nextDir, bool speed)
{
    int heading = _mapper.getHeading();
    int diff = (nextDir - heading + 4) % 4;

    // Turn FIRST (non-blocking start commands)
    if (diff == 1)
    {
        _motion.startTurn(90.0f, false);
        _mapper.turnRight();
    }
    else if (diff == 3)
    {
        _motion.startTurn(90.0f, true);
        _mapper.turnLeft();
    }
    else if (diff == 2)
    {
        _motion.startTurn(180.0f, true);
        _mapper.turnAround();
    }

    // Always start forward motion AFTER turn command
    _motion.startMove(180.0f);
}

void StateMachine::captureInternalState(int nextDir)
{
    _data.currentState = _currentState;
    _data.isSpeedRun = (_currentState == SPEED_RUN || _currentState == FINISHED);

    _data.x = _mapper.getX();
    _data.y = _mapper.getY();
    _data.heading = _mapper.getHeading();

    _data.distFront = _motion.getFrontDistance();
    _data.distLeft = _motion.getLeftDistance();
    _data.distRight = _motion.getRightDistance();

    _data.nextDir = nextDir;

    if (_currentState == SPEED_RUN)
    {
        _data.targetX = 0;
        _data.targetY = 0;
    }
    else
    {
        _data.targetX = 7;
        _data.targetY = 7;
    }
}