#pragma once
#include <stdint.h>
#include "Mapper.h"

class Planner
{
public:
        int distField[16][16];

    void runBFS(Mapper &map, int tx, int ty);

    int chooseDirection(Mapper &map);

    void updateFilteredWalls(Mapper &map, float front, float left, float right);

    int chooseSpeedDirection(Mapper &map, int currentHeading);

private:
    uint8_t wallConfidence[16][16][4] = {0};
    const uint8_t THRESHOLD = 150;
};