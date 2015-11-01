#include <stdio.h>
#include "maze.h"
#include "floodFill.h"
#include "robotState.h"

int main()
{
    printf("Maze Test\n");

    Maze maze;
    MazeReset(&maze);
    MazePrint(&maze, 0, 0);

    RobotState state;
    state.targetXmin = 7;
    state.targetYmin = 7;
    state.targetXmax = 8;
    state.targetYmax = 8;    

    FloodFill floodFill;
    floodFill.state = &state;
    floodFill.maze = &maze;
    
    SimpleFloodFillSolve(&floodFill);

    MazePrint(&maze, 0, 0);
    
        
    printf("Done!\n");
    return 0;
}
