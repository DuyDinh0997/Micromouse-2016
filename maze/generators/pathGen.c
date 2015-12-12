#include "pathGen.h"

void executeMove(char move, MousePosition* pos)
{
    if (move == 'f')
    {
        MousePositionGoForward(pos); 
    } 
    else if (move == 'b')
    {
        MousePositionGoBackward(pos);
    }
    else if (move == 'l')
    {
        MousePositionRotateLeft(pos);
        MousePositionGoForward(pos); 
    }
    else if (move == 'r')
    {
        MousePositionRotateRight(pos);
        MousePositionGoForward(pos); 
    }
}
#include "processor.h"
void PathGenOrtho(Maze* maze, RobotState* state, char* ortho)
{
    // This should create a shallow memory copy of state->pos
    MousePosition tmpPos = state->pos;
    Processor* proc = SingletonProcessor();

    while (1==1)
    {
        maze->maze[tmpPos.x][tmpPos.y].inPath = 1;

        char nextMove = MoveGenGetNextMove(maze,
            tmpPos.x, tmpPos.y, tmpPos.forwardDirection);


        *ortho++ = nextMove;

        executeMove(nextMove, &tmpPos); 

        // Break the loop if the mouse reached the desired location.
        if (tmpPos.x >= state->targetXmin && tmpPos.x <= state->targetXmax &&
            tmpPos.y >= state->targetYmin && tmpPos.y <= state->targetYmax) 
            break;
    } 

    *ortho = 0;
}
