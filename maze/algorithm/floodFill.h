#ifndef __FLOODFILL_H__
#define __FLOODFILL_H__

#include "maze.h"
#include "robotState.h"

typedef struct XYTCoords
{
    unsigned char x, y;
    unsigned int level;
    Compass direction;
    int isInQueue;

} XYTCoords;

typedef struct FloodFill
{
    Maze* maze;
    RobotState* state;
} FloodFill;

void SimpleFloodFillSolve(FloodFill* this);

#endif
