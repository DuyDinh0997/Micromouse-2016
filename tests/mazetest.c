#include <stdio.h>
#include "maze.h"
#include "floodFill.h"
#include "robotState.h"
#include "moveGen.h"

int main()
{
    printf("Maze Test\n");

    Maze maze;
    MazeReset(&maze);
    MazePrint(&maze, 0, 0);

    RobotState state;
    state.pos.forwardDirection = SOUTH;
    state.targetXmin = 7;
    state.targetYmin = 7;
    state.targetXmax = 8;
    state.targetYmax = 8;    

    FloodFill floodFill;
    floodFill.state = &state;
    floodFill.maze = &maze;
    
    SimpleFloodFillSolve(&floodFill);

    char nextMove = MoveGenGetNextMove(&maze, state.pos.x, state.pos.y, state.pos.forwardDirection); 

    printf("Next Move = %c\n", nextMove);
    MazePrint(&maze, 0, 0);
   
    printf("Done!\n");
    return 0;
}
