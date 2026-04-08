#pragma once

enum State
{
    STARTING,
    SEARCHING,
    DECIDING,
    SPEED_RUN,
    EMERGENCY_STOP
};

class StateMachine
{
public:
    StateMachine();
    void update(); // Called in every loop
    void setState(State newState);
    State getCurrentState();

private:
    State _currentState;
    void handleSearching();
    void handleSpeedRun();
};