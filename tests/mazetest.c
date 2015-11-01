#include <stdio.h>
#include "maze.h"
#include "floodFill.h"
#include "robotState.h"
#include "moveGen.h"
#include "pathGen.h"


int main()
{
    printf("Maze Test\n");

    // Maze Init
    Maze maze;
    MazeReset(&maze);
    MazePrint(&maze, 0, 0);

    // Robot State Init 
    RobotState state;
    MousePositionInit(&state.pos);
    state.targetXmin = 7;
    state.targetYmin = 7;
    state.targetXmax = 8;
    state.targetYmax = 8;    

    FloodFill floodFill;
    floodFill.state = &state;
    floodFill.maze = &maze;
    SimpleFloodFillSolve(&floodFill);

    char ortho[255]; 

    PathGenOrtho(&maze, &state, ortho);

    /*while (1==1)
    {
        char nextMove = MoveGenGetNextMove(
            &maze, state.pos.x, state.pos.y, state.pos.forwardDirection); 

        executeMove(nextMove, &state.pos);     
        printf("Move = %c\n", nextMove);

        maze.maze[state.pos.x][state.pos.y].inPath = 1;

        if (state.pos.x >= 7 && state.pos.x <= 8 && 
            state.pos.y >= 7 && state.pos.y <= 8) break;
    }*/

    MazePrint(&maze, state.pos.x, state.pos.y);
   
    printf("Done!\n");
    return 0;
}
