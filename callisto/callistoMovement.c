#include "callistoMovement.h"
#include "cellToCell.h"
#include "pathGen.h"
#include "mousePosition.h"
#include "maze.h"
#include "processor.h"

void CallistoSearch(Maze* maze, RobotState* state, MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	Processor* proc = SingletonProcessor();

	MazeReset(maze);

	MousePositionInit(&state->pos, NORTH, 15, 15);
	state->targetXmin = 13;
	state->targetYmin = 13;
	state->targetXmax = 13;
	state->targetYmax = 13;

	char ortho[255];

	FloodFill floodFill;
	floodFill.state = state;
	floodFill.maze = maze;

	SearchingFirstCell(mouseInfo, motionInfo);
	MousePositionGoForward(&state->pos);

	int lc = 0;
	while (1==1)
	{
		int leftIR = proc->getSensor(SENSOR_LEFT_3);
		int rightIR = proc->getSensor(SENSOR_RIGHT_3);
		int frontIR = proc->getSensor(SENSOR_LEFT_1);

		int valueLeft = leftIR > mouseInfo->searchingWallExists ? 1 : 0;
		int valueRight = rightIR > mouseInfo->searchingWallExists ? 1 : 0;
		int valueForward = frontIR > mouseInfo->searchingWallExists ? 1 : 0;

		proc->writeMemoryWithString("X,");
		proc->writeMemoryWithInt(state->pos.x);
		proc->writeMemoryWithString(",Y,");
		proc->writeMemoryWithInt(state->pos.y);
		proc->writeMemoryWithString("\n");

		// Update Walls
        MazeSetWallRelative(maze, state->pos.x, state->pos.y, valueForward,
        	state->pos.forwardDirection, FORWARD);

        MazeSetWallRelative(maze, state->pos.x, state->pos.y, valueLeft,
        	state->pos.forwardDirection, LEFT);

        MazeSetWallRelative(maze, state->pos.x, state->pos.y, valueRight,
        	state->pos.forwardDirection, RIGHT);

		SimpleFloodFillSolve(&floodFill);
		PathGenOrtho(maze, state, ortho);

		CallistoExecuteMovement(*ortho, &state->pos, mouseInfo, motionInfo);

		        // Break the loop if the mouse reached the desired location.
        if (state->pos.x >= state->targetXmin && state->pos.x <= state->targetXmax &&
            state->pos.y >= state->targetYmin && state->pos.y <= state->targetYmax)
            break;

		if (lc++ > 15) break;
	}

	CallistoPrintMaze(maze);
}

void CallistoExecuteMovement(char move, MousePosition* pos, MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	// Searching
	if (move == 'f')
	{
		SearchingStraight(mouseInfo, motionInfo);
		MousePositionGoForward(pos);
	}

	if (move == 'r')
	{
		SearchingTurn(90, mouseInfo, motionInfo);
		MousePositionRotateRight(pos);
		MousePositionGoForward(pos);
	}

	if (move == 'l')
	{
		SearchingTurn(-90, mouseInfo, motionInfo);
		MousePositionRotateLeft(pos);
		MousePositionGoForward(pos);
	}

	//if (move == 'b')
}

void CallistoPrintMaze(Maze* this)
{
	Processor* proc = SingletonProcessor();

	int x, y;
	for (y = 0; y < 17; y++)
	{
		for (x = 0; x < 18; x++)
		{
			if (x < 17)
				proc->writeMemoryWithString("+");

			if (x < 16)
			{
				if (MazeHasAWall(this, x, y, NORTH))
					proc->writeMemoryWithString("---");
				else
					proc->writeMemoryWithString("   ");
			}
		}

		proc->writeMemoryWithString("\n");

		if (y < 16)
		{
			for (x = 0; x < 18; x++)
			{
				if (x < 17)
				{
					if (MazeHasAWall(this, x, y, WEST))
						proc->writeMemoryWithString("|");
					else
						proc->writeMemoryWithString(" ");
				}

				if (x < 16)
				{
					int distance = MazeGetDistance(this, x, y);

					if (distance < 10)
						proc->writeMemoryWithString("00");
					else if (distance < 100)
						proc->writeMemoryWithString("0");

					proc->writeMemoryWithInt(distance);
				}
			}
		}

		proc->writeMemoryWithString("\n");
	}
}
