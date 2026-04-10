#pragma once
#include "IRScanner.h"
#include "Sonar.h"

class Mapper
{
public:
    Mapper(IRScanner &left, IRScanner &right, Sonar &front);

    void updateWalls();
    void setWall(int x, int y, int d);

    bool hasWall(int x, int y, int d) const;

    int getX() const;
    int getY() const;
    int getHeading() const;

    void moveForward();
    void turnLeft();
    void turnRight();
    void turnAround();

    void setPosition(int px, int py)
    {
        x = px;
        y = py;
    }

private:
    IRScanner &_leftIR;
    IRScanner &_rightIR;
    Sonar &_frontSonar;

    int x = 0, y = 0, heading = 0;

    bool walls[16][16][4] = {false};

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};
};