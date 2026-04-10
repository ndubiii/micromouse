#include "Mapper.h"

Mapper::Mapper(IRScanner &left, IRScanner &right, Sonar &front)
    : _leftIR(left), _rightIR(right), _frontSonar(front) {}

void Mapper::updateWalls()
{
    float front = _frontSonar.readDistanceCM() * 10.0f;
    float left = _leftIR.getDistanceMM();
    float right = _rightIR.getDistanceMM();

    const float th = 140.0f;

    if (front < th)
        setWall(x, y, heading);
    if (left < th)
        setWall(x, y, (heading + 3) % 4);
    if (right < th)
        setWall(x, y, (heading + 1) % 4);
}

void Mapper::setWall(int x, int y, int d)
{
    walls[x][y][d] = true;

    int nx = x + dx[d];
    int ny = y + dy[d];

    if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16)
        walls[nx][ny][(d + 2) % 4] = true;
}

bool Mapper::hasWall(int x, int y, int d) const
{
    return walls[x][y][d];
}

int Mapper::getX() const { return x; }
int Mapper::getY() const { return y; }
int Mapper::getHeading() const { return heading; }

void Mapper::moveForward()
{
    x += dx[heading];
    y += dy[heading];
}

void Mapper::turnLeft()
{
    heading = (heading + 3) % 4;
}

void Mapper::turnRight()
{
    heading = (heading + 1) % 4;
}

void Mapper::turnAround()
{
    heading = (heading + 2) % 4;
}