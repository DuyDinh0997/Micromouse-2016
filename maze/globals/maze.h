#ifndef __MAZE_H__
#define __MAZE_H__

typedef struct MazeCell
{
    char x, y;
    char westWall, northWall;
    int distance;
    int explored;
    int inPath; // Used for printing out in debugger.    

} MazeCell;

typedef enum Compass
{
    WEST,
    NORTH,
    EAST,
    SOUTH,
    NONE
} Compass;

typedef enum Direction
{
    FORWARD,
    LEFT,
    RIGHT,
    BACK
} Direction;

/**
 * This keeps track of the current status of the maze.
 */
typedef struct Maze
{
    /**
     * Maze Cells go from top left to bottom right. Note: the extra 17th cell 
     * is needed as there are 17 walls side to side and 17 walls up and down,
     * even though that only makes up 16 "cells".
     */
    MazeCell maze[17][17];
} Maze;

void MazeInit(Maze*);
void MazeReset(Maze*);
void MazeResetDistances(Maze*);
void MazeSetWall(Maze*, int x, int y, int value, Compass);
int MazeHasAWall(Maze*, int x, int y, Compass compass);

#ifdef FAKE_MICROMOUSE
void MazePrint(Maze* this, int tarx, int tary);
#endif

#endif
