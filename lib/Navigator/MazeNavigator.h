#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <vector>
#include "Motor.h"
#include "Encoder.h"
#include "IRScanner.h"
#include "Sonar.h"
#include "PID.h"

enum Directions
{
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
};

class Navigator
{
public:
    Navigator(Motor &lM, Motor &rM,
              Encoder &lE, Encoder &rE,
              IRScanner &left,
              IRScanner &right,
              Sonar &front,
              PIDController &pidL,
              PIDController &pidR);

    // Maze functions
    void setWall(int x, int y, int d);
    void updateWalls();
    std::vector<std::vector<int>> runBFS(int tx, int ty);

    // Movement
    void moveOneCell(float cellDistanceMM = 180.0f);
    void differentialTurn(float degrees, bool left);
    void turnLeft();
    void turnRight();
    void turnAround();

    // Decision
    int chooseNextDirection(const std::vector<std::vector<int>> &dist);
    void step(const std::vector<std::vector<int>> &dist);

    // Getters for testing
    int getX() const { return x; }
    int getY() const { return y; }
    int getHeading() const { return heading; }

private:
    Motor &_lM, &_rM;
    Encoder &_lE, &_rE;

    IRScanner &_leftIR;
    IRScanner &_rightIR;
    Sonar &_frontSonar;

    PIDController &_pidL;
    PIDController &_pidR;

    int x = 0, y = 0, heading = NORTH;

    bool walls[16][16][4] = {false};

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};
};

#endif