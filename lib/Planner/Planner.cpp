#include "Planner.h"

// Optimized BFS: No dynamic allocation
void Planner::runBFS(Mapper &map, int tx, int ty)
{
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 16; j++)
            distField[i][j] = 255; // Use 255 as 'unvisited'

    struct Coord
    {
        uint8_t x, y;
    };
    Coord queue[256]; // Large enough for all cells
    int head = 0, tail = 0;

    distField[tx][ty] = 0;
    queue[tail++] = {(uint8_t)tx, (uint8_t)ty};

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    while (head < tail)
    {
        Coord c = queue[head++];

        for (int d = 0; d < 4; d++)
        {
            // Check wallConfidence instead of raw mapper
            if (wallConfidence[c.x][c.y][d] > THRESHOLD)
                continue;

            int nx = c.x + dx[d];
            int ny = c.y + dy[d];

            if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16 && distField[nx][ny] == 255)
            {
                distField[nx][ny] = distField[c.x][c.y] + 1;
                queue[tail++] = {(uint8_t)nx, (uint8_t)ny};
            }
        }
    }
}

void Planner::updateFilteredWalls(Mapper &map, float front, float left, float right)
{
    int x = map.getX();
    int y = map.getY();
    int h = map.getHeading();
    const float sensorThreshold = 140.0f;

    // Helper: Increases confidence if wall seen, decreases if not
    auto filter = [&](int d, bool detected)
    {
        uint8_t &val = wallConfidence[x][y][d];
        if (detected)
        {
            if (val < 240)
                val += 40; // Quick ramp up
        }
        else
        {
            if (val > 20)
                val -= 20; // Slower ramp down (safety)
        }

        // Ensure neighbor cell is synchronized
        int nx = x + (d == 1 ? 1 : d == 3 ? -1
                                          : 0);
        int ny = y + (d == 0 ? 1 : d == 2 ? -1
                                          : 0);
        if (nx >= 0 && nx < 16 && ny >= 0 && ny < 16)
        {
            wallConfidence[nx][ny][(d + 2) % 4] = val;
        }
    };

    filter(h, front < sensorThreshold);
    filter((h + 3) % 4, left < sensorThreshold);
    filter((h + 1) % 4, right < sensorThreshold);
}

int Planner::chooseSpeedDirection(Mapper &map, int heading)
{
    int x = map.getX();
    int y = map.getY();
    int bestDir = -1;
    int bestVal = 1000;

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    for (int d = 0; d < 4; d++)
    {
        if (wallConfidence[x][y][d] > THRESHOLD)
            continue;

        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx < 0 || nx >= 16 || ny < 0 || ny >= 16)
            continue;

        int val = distField[nx][ny];

        // HEURISTIC: In Speed Run, turns are expensive
        if (d == heading)
        {
            val -= 1; // Slight bias to stay straight
        }
        else if (d == (heading + 2) % 4)
        {
            val += 10; // Penalty for U-turns
        }

        if (val < bestVal)
        {
            bestVal = val;
            bestDir = d;
        }
    }
    return bestDir;
}

int Planner::getCompressedDirection(Mapper &map, int heading)
{
    int x = map.getX();
    int y = map.getY();

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {1, 0, -1, 0};

    int bestDir = 1;
    int bestVal = 1000;

    for (int d = 0; d < 4; d++)
    {
        if (wallConfidence[x][y][d] > THRESHOLD)
            continue;

        int nx = x + dx[d];
        int ny = y + dy[d];

        if (nx < 0 || nx >= 16 || ny < 0 || ny >= 16)
            continue;

        int val = distField[nx][ny];

        if (d == heading)
            val -= 1;
        else if (d == (heading + 2) % 4)
            val += 10;

        if (val < bestVal)
        {
            bestVal = val;
            bestDir = d;
        }
    }
    return bestDir;
}