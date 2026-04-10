#pragma once
#include "Mapper.h"
#include "Planner.h"
#include "Motion.h"
#include "IRScanner.h"
#include "Sonar.h"

enum State
{
    STARTING,
    SEARCHING,
    DECIDING,
    SPEED_RUN,
    RETURNING,
    FINISHED
};

// Struct to capture all live values for debugging/logging
struct MouseTelemetry
{
    State currentState;
    int x, y, heading;
    float distFront, distLeft, distRight;
    int targetX, targetY;
    int nextDir;
    bool isSpeedRun;
};

class StateMachine
{
public:
    StateMachine(Mapper &m, Planner &p, Motion &mo);

    // Main loop function
    void update();

    // Getter to retrieve the captured values
    MouseTelemetry getTelemetry() const { return _data; }

private:
    State _currentState = STARTING;
    Mapper &_mapper;
    Planner &_planner;
    Motion &_motion;

    // The storage for captured values
    MouseTelemetry _data;

    void executeMove(int nextDir, bool speed);
    bool isAt(int tx, int ty) { return _mapper.getX() == tx && _mapper.getY() == ty; }
    bool _pathDirty = true;
    bool _commandIssued = false;

    // Syncs internal _data with external modules
    void captureInternalState(int nextDir = -1);
};