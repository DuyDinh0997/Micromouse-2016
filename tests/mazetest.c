#include <stdio.h>
#include "maze.h"
#include "floodFill.h"
#include "robotState.h"
#include "moveGen.h"
#include "pathGen.h"

// Imports a maze represented in ascii
void importMaze(const char* fileName, Maze* maze)
{
    FILE* pFile;
    pFile = fopen(fileName, "r");

    if (pFile == 0) return;

    char line[255];
    int lc = 0;
 
    while (1)
    {
        if (fgets(line, 255, pFile) == NULL) break;

        // Check for North facing walls        
        if (lc % 2 == 0)
        {
            int x;
            for (x = 0; x < 16; x++)
            {
                if (line[x*4+1] != ' ')
                {
                    maze->maze[x][(int)lc/2].northWall = 1;
                }       
            } 
        }
        // Check for west facing walls
        else
        {
            int x;
            for (x = 0; x < 17; x++)
            {
                if (line[x*4] != ' ')
                {
                    maze->maze[x][(int)lc/2].westWall = 1;
                }
            }
        }

        lc++;
    }
}

int main()
{
    printf("Maze Test\n");

    // Maze Init
    Maze maze;
    MazeReset(&maze);
    importMaze("mazes/test.txt", &maze);

    Maze learningMaze;
    MazeReset(&learningMaze);

    // Robot State Init 
    RobotState state;
    MousePositionInit(&state.pos, NORTH, 15, 15);
    state.targetXmin = 13;
    state.targetYmin = 13;
    state.targetXmax = 13;
    state.targetYmax = 13;    

    // Execute Flood Fill on current maze
    FloodFill floodFill;
    floodFill.state = &state;
    floodFill.maze = &maze;

    SimpleFloodFillSolve(&floodFill);
    MazePrint(&maze, state.pos.x, state.pos.y);

    floodFill.maze = &learningMaze;

    MousePositionGoForward(&state.pos);

    char ortho[255]; 
    int lc = 0;

    while (1==1)
    {
        int i; for (i = 0; i < 255; i++) ortho[i] = 0;

        printf("---Adding Walls to Maze---\n");
        int valueForward = MazeHasAWallRelative(&maze, 
            state.pos.x, state.pos.y, state.pos.forwardDirection, FORWARD); 

        int valueLeft = MazeHasAWallRelative(&maze, 
            state.pos.x, state.pos.y, state.pos.forwardDirection, LEFT); 

        int valueRight = MazeHasAWallRelative(&maze, 
            state.pos.x, state.pos.y, state.pos.forwardDirection, RIGHT); 

        printf("Walls = F:%d L:%d R:%d\n", valueForward, valueLeft, valueRight);

        MazeSetWallRelative(&learningMaze, state.pos.x, state.pos.y, valueForward,
            state.pos.forwardDirection, FORWARD); 

        MazeSetWallRelative(&learningMaze, state.pos.x, state.pos.y, valueLeft,
            state.pos.forwardDirection, LEFT); 

        MazeSetWallRelative(&learningMaze, state.pos.x, state.pos.y, valueRight,
            state.pos.forwardDirection, RIGHT); 

        printf("---Flood Filling---\n");
 
        SimpleFloodFillSolve(&floodFill);
        MazePrint(&learningMaze, state.pos.x, state.pos.y);

        printf("---Generating Path---\n");
        PathGenOrtho(&learningMaze, &state, ortho);

        // Move!
        if (*ortho == 'f' || *ortho == 'F')
        {
            MousePositionGoForward(&state.pos);
        } 
        else if (*ortho == 'r' || *ortho == 'R')
        {
            MousePositionRotateRight(&state.pos); 
            MousePositionGoForward(&state.pos);
        }
        else if (*ortho == 'l' || *ortho == 'L')
        {
            MousePositionRotateLeft(&state.pos); 
            MousePositionGoForward(&state.pos);
        }
        else if (*ortho == 'b' || *ortho == 'B')
        {
            MousePositionGoBackward(&state.pos);
        }

        printf("New Position = %d, %d\n", state.pos.x, state.pos.y);
  
        printf("Ortho = %s\n", ortho);

        if (state.pos.x >= state.targetXmin && state.pos.x <= state.targetXmax &&
            state.pos.y >= state.targetYmin && state.pos.y <= state.targetYmax)
            break;

        if (lc++ > 15) break;       
    } 


    printf("Done!\n");
    return 0;
}
