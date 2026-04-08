#include "MouseState.h"
#include <Arduino.h>

StateMachine::StateMachine() : _currentState(STARTING) {}

void StateMachine::setState(State newState)
{
    _currentState = newState;
    Serial.print("State changed to: ");
    Serial.println(newState);
}

void StateMachine::update()
{
    switch (_currentState)
    {
    case STARTING:
        // Wait for a hand wave in front of the IR sensors
        break;
    case SEARCHING:
        handleSearching();
        break;
    case SPEED_RUN:
        handleSpeedRun();
        break;
    case EMERGENCY_STOP:
        // Halt all motors
        break;
    }
}

void StateMachine::handleSearching()
{
    // Logic: Move one cell, check walls, update Maze Map
    // If center reached -> setState(DECIDING)
}

void StateMachine::handleSpeedRun()
{
    // Logic: Use pre-calculated path, high acceleration
}