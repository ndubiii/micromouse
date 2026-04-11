#include "SPIFFS.h"
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

void Mapper::saveMazeToFile()
{
    File file = SPIFFS.open("/maze.txt", FILE_WRITE);
    if (!file)
        return;

    for (int y = 15; y >= 0; y--)
    {
        // TOP WALLS
        for (int x = 0; x < 16; x++)
        {
            file.print("+");
            file.print(walls[x][y][0] ? "---" : "   ");
        }
        file.println("+");

        // CELLS
        for (int x = 0; x < 16; x++)
        {
            file.print(walls[x][y][3] ? "|" : " ");

            if (x == this->x && y == this->y)
                file.print(" M ");
            else
                file.print(" . ");
        }

        file.println("|");
    }

    // BOTTOM BORDER
    for (int x = 0; x < 16; x++)
        file.print("+---");
    file.println("+");

    file.close();
}
