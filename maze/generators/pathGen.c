#include "pathGen.h"

void executeMove(char move, MousePosition* pos)
{
    if (move == 'F' || move == 'f')
    {
        MousePositionGoForward(pos); 
    } 
    else if (move == 'B')
    {
        MousePositionGoBackward(pos);
    }
    else if (move == 'L' || move == 'l')
    {
        MousePositionRotateLeft(pos);
        MousePositionGoForward(pos); 
    }
    else if (move == 'R' || move == 'r')
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

        // This increments after setting *ortho to nextMove
        *ortho++ = nextMove;

        executeMove(nextMove, &tmpPos); 

        // Break the loop if the mouse reached the desired location.
        if (tmpPos.x >= state->targetXmin && tmpPos.x <= state->targetXmax &&
            tmpPos.y >= state->targetYmin && tmpPos.y <= state->targetYmax) 
            break;
    } 

    *ortho = 0;
}

int charToTernary(char move)
{
	if (move == 'F') return 0;
	if (move == 'L') return 1;
	if (move == 'R') return 2;

	return -28;
}

char PathGenGetDiagMove(char m1, char m2, char m3, int* is45)
{
	// Alan Dibley Ternary Method
	const char moves[27][2] =
	{ {'F', ' '}, // FFF
	  {'F', ' '}, // FFL
	  {'F', ' '}, // FFR
	  {'L', ' '}, // FLF
	  {'L', ' '}, // FLL
	  {'l', ' '}, // FLR
	  {'R', ' '}, // FRF
	  {'r', ' '}, // FRL
	  {'R', ' '}, // FRR
	  {'F', 'l'}, // LFF
	  {'F', 'l'}, // LFL
	  {'F', 'l'}, // LFR
	  {'L', 'x'}, // LLF
	  {' ', ' '}, // LLL
	  {'l', 'L'}, // LLR
	  {' ', 'f'}, // LRF
	  {'F', 'f'}, // LRL
	  {' ', 'f'}, // LRR
	  {'F', 'r'}, // RFF
	  {'F', 'r'}, // RFL
	  {'F', 'r'}, // RFR
	  {' ', 'f'}, // RLF
	  {' ', 'f'}, // RLL
	  {'F', 'f'}, // RLR
	  {'R', 'z'}, // RRF
	  {'r', 'R'}, // RRL
	  {' ', ' '}  // RRR
	};

	int num1 = charToTernary(m1);
	int num2 = charToTernary(m2);
	int num3 = charToTernary(m3);

	// Base 3 (Ternary), so convert Ternary number to Base 10 Number
	int index = num1*9 + num2*3 + num3;

	// If we get an invalid move, just return whatever the current move is.
	if (index < 0) return m2;

	char diagMove = moves[index][*is45];

	// Toggle is45 flag if necessary
	if (diagMove == 'x' ||
		diagMove == 'l' ||
		diagMove == 'r' ||
		diagMove == 'z')
	{
		*is45 = !*is45;
	}

	return diagMove;
}

void PathGenDiag(char* ortho, char* diag)
{
	// Count how large the ortho path is.
	int loopCounter = 0;
	char* tmp = ortho;
	while (*tmp++ != 0) loopCounter++;

	int is45 = 0;
	int amountToIncrement = 1;

	diag[0] = 'F';
	diag++;

	int q;
	for (q = 1; q < loopCounter-1; q++)
	{
		char x = ortho[q-1];
		char y = ortho[q];
		char z = ortho[q+1];

		char result = PathGenGetDiagMove(x, y, z, &is45);

		*diag = result;
		diag++;
	}

	printf("%c, %c\n", ortho[loopCounter-2], ortho[loopCounter-1]);
	*diag = PathGenGetDiagMove(ortho[loopCounter-1], ortho[loopCounter], 'F', &is45);
	printf("Result = %c\n", *diag);
	diag++;
	*diag = 0;
}
