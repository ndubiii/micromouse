#include <queue>
#include "MazeNavigator.h"

// -------------------- Constructor --------------------
Navigator::Navigator(Motor &lM, Motor &rM,
                     Encoder &lE, Encoder &rE,
                     IRScanner &left,
                     IRScanner &right,
                     Sonar &front,
                     PIDController &pidL,
                     PIDController &pidR)
    : _lM(lM), _rM(rM),
      _lE(lE), _rE(rE),
      _leftIR(left),
      _rightIR(right),
      _frontSonar(front),
      _pidL(pidL),
      _pidR(pidR) {}

// -------------------- Wall Setting --------------------
void Navigator::setWall(int x, int y, int d)
{
    walls[x][y][d] = true;

    int nx = x + dx[d];
    int ny = y + dy[d];

    if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16)
        walls[nx][ny][(d + 2) % 4] = true;
}

// -------------------- BFS --------------------
std::vector<std::vector<int>> Navigator::runBFS(int tx, int ty)
{
    std::vector<std::vector<int>> dist(16, std::vector<int>(16, -1));
    std::queue<std::pair<int, int>> q;

    dist[tx][ty] = 0;
    q.push({tx, ty});

    while (!q.empty())
    {
        auto current = q.front();
        q.pop();

        int cx = current.first;
        int cy = current.second;

        for (int d = 0; d < 4; d++)
        {
            if (walls[cx][cy][d])
                continue;

            int nx = cx + dx[d];
            int ny = cy + dy[d];

            if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16 && dist[nx][ny] == -1)
            {
                dist[nx][ny] = dist[cx][cy] + 1;
                q.push({nx, ny});
            }
        }
    }

    return dist;
}

// -------------------- Update walls using IR + Sonar --------------------
void Navigator::updateWalls()
{
    float frontDist = _frontSonar.readDistanceCM() * 10.0f; // cm → mm
    float leftDist = _leftIR.getDistanceMM();
    float rightDist = _rightIR.getDistanceMM();

    const float threshold = 140.0f;

    if (frontDist < threshold)
        setWall(x, y, heading);
    if (leftDist < threshold)
        setWall(x, y, (heading + 3) % 4);
    if (rightDist < threshold)
        setWall(x, y, (heading + 1) % 4);
}

// -------------------- Move one cell with PID --------------------
void Navigator::moveOneCell(float cellDistanceMM)
{
    _lE.reset();
    _rE.reset();
    _pidL.reset();
    _pidR.reset();

    while (_lE.getDistance() < cellDistanceMM ||
           _rE.getDistance() < cellDistanceMM)
    {
        float leftSpeed = _pidL.calculate(cellDistanceMM, _lE.getDistance());
        float rightSpeed = _pidR.calculate(cellDistanceMM, _rE.getDistance());

        _lM.setSpeed(leftSpeed);
        _rM.setSpeed(rightSpeed);

        updateWalls();
    }

    _lM.stop();
    _rM.stop();

    x += dx[heading];
    y += dy[heading];
}

// -------------------- Differential turn --------------------
void Navigator::differentialTurn(float degrees, bool left)
{
    _lE.reset();
    _rE.reset();
    _pidL.reset();
    _pidR.reset();

    const float WHEELBASE_MM = 120.0f;
    float arcLength = (WHEELBASE_MM * 3.14159f * degrees) / 360.0f;

    while (_lE.getDistance() < arcLength ||
           _rE.getDistance() < arcLength)
    {
        float leftTarget = left ? -arcLength : arcLength;
        float rightTarget = left ? arcLength : -arcLength;

        float leftSpeed = _pidL.calculate(leftTarget, _lE.getDistance());
        float rightSpeed = _pidR.calculate(rightTarget, _rE.getDistance());

        _lM.setSpeed(leftSpeed);
        _rM.setSpeed(rightSpeed);
    }

    _lM.stop();
    _rM.stop();

    heading = (heading + (left ? 3 : 1)) % 4;
}

// -------------------- Public turns --------------------
void Navigator::turnLeft() { differentialTurn(90.0f, true); }
void Navigator::turnRight() { differentialTurn(90.0f, false); }
void Navigator::turnAround() { differentialTurn(180.0f, true); }

// -------------------- Decision --------------------
int Navigator::chooseNextDirection(const std::vector<std::vector<int>> &dist)
{
    int bestDir = -1;
    int bestVal = 10000;

    for (int d = 0; d < 4; d++)
    {
        if (walls[x][y][d])
            continue;

        int nx = x + dx[d];
        int ny = y + dy[d];

        if (nx < 0 || nx >= 16 || ny < 0 || ny >= 16)
            continue;

        int value = dist[nx][ny];

        if (value >= 0 && value < bestVal)
        {
            bestVal = value;
            bestDir = d;
        }
    }

    return bestDir;
}

void Navigator::step(const std::vector<std::vector<int>> &dist)
{
    int nextDir = chooseNextDirection(dist);
    if (nextDir == -1)
        return;

    int diff = (nextDir - heading + 4) % 4;

    if (diff == 0)
        moveOneCell();
    else if (diff == 1)
    {
        turnRight();
        moveOneCell();
    }
    else if (diff == 3)
    {
        turnLeft();
        moveOneCell();
    }
    else if (diff == 2)
    {
        turnAround();
        moveOneCell();
    }
}