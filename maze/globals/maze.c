#include "maze.h"

void MazeInit(Maze* this)
{
    MazeReset(this);
}

void MazeReset(Maze* this)
{
    int i, j;
    
    MazeResetDistances(this);

    // Set all walls to not-set
    for (i = 0; i < 17; i++)
    {
        for (j = 0; j < 17; j++)
        {
            this->maze[i][j].northWall = 0;
            this->maze[i][j].westWall = 0;
            this->maze[i][j].explored = 0;
            this->maze[i][j].inPath = 0;
        }
    }

    for (i = 0; i < 16; i++)
    {
        MazeSetWall(this, i, 0, 1, NORTH);
        MazeSetWall(this, i, 15, 1, SOUTH);
        MazeSetWall(this, 0, i, 1, WEST);
        MazeSetWall(this, 15, i, 1, EAST);
    }
}

void MazeResetDistances(Maze* this)
{
    int x, y;
    
    for (x = 0; x < 17; x++)
    {
        for (y = 0; y < 17; y++)
        {
            this->maze[x][y].distance = 255; 
            this->maze[x][y].x = x;
            this->maze[x][y].y = y; 
        }
    }
}

void MazeSetWall(Maze* this, int x, int y, int value, Compass compass)
{
    if (compass == WEST)
        this->maze[x][y].westWall = value;

    else if (compass == NORTH)
        this->maze[x][y].northWall = value;

    else if (compass == EAST)
        this->maze[x+1][y].westWall = value;

    else if (compass == SOUTH)
        this->maze[x][y+1].northWall = value; 
}

void MazeSetDistance(Maze* this, int x, int y, int distance)
{
    this->maze[x][y].distance = distance; 
}

int MazeGetDistance(Maze* this, int x, int y)
{
    return this->maze[x][y].distance;
}

MazeCell* MazeGetCell(Maze* this, int x, int y)
{
    return &this->maze[x][y];
}

int MazeHasAWall(Maze* this, int x, int y, Compass compass)
{
    if (compass == WEST)
        return this->maze[x][y].westWall;

    else if (compass == NORTH)
        return this->maze[x][y].northWall;

    else if (compass == EAST)
        return this->maze[x+1][y].westWall;

    else if (compass == SOUTH)
        return this->maze[x][y+1].northWall;

    return 0;
}

#ifdef FAKE_MICROMOUSE
#include <stdio.h>

void MazePrint(Maze* this, int tarx, int tary)
{
    printf("Printing Maze\n");
    int x, y;

    for (y = 0; y < 17; y++)
    {
        // Print top walls
        for (x = 0; x < 18; x++)
        {
            if (x < 17)
                printf("+");

            if (x < 16)
            {
                if (MazeHasAWall(this, x, y, NORTH))
                    printf("---");
                else
                    printf("   ");
            }
        }

        printf("\n");

        if (y < 16)
        {
            // Print side walls and data
            for (x = 0; x < 18; x++)
            {
                if (x < 17)
                {
                    if (MazeHasAWall(this, x, y, WEST))
                        printf("|");
                    else
                        printf(" ");
                }

                if(x < 16)
                {
                    int distance = MazeGetDistance(this, x, y);
                    if (x == tarx && y == tary)
                        printf("\033[1;32m");
                    else if (this->maze[x][y].inPath)
                        printf("\033[1;33m");
                    else
                        printf("\033[1;31m");

                    printf("%3d\033[0m", distance);
                }


            }
            printf("\n");
        }
    }    
}

#endif
