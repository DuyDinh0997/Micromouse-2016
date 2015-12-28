#include "cellToCell.h"

void SearchingFirstCell(MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	Mouse* mouse = SingletonMouse();

    // Start at a higher speed so that acceleration happens sooner.
    mouse->motorValueLeft = 100;
    mouse->motorValueRight = 100;

	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;
	motionInfo->endOnPost = 0;

	MotionStraight(mouseInfo, motionInfo,
    	0, mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, mouseInfo->firstCellDistance);
}

void SearchingStraight(MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;
	motionInfo->endOnPost = 0;

	MotionStraight(mouseInfo, motionInfo,
    	mouseInfo->straightVelocity, mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, mouseInfo->normalCellDistance);
}

void SearchingTurn(int degrees, MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;
	motionInfo->endOnPost = 0;

	MotionDecel(mouseInfo, motionInfo,
		mouseInfo->straightVelocity, mouseInfo->turningVelocity,
		mouseInfo->straightAccel, mouseInfo->turnInLength);

	MotionTurn(mouseInfo, motionInfo,
    	degrees,
		mouseInfo->turningVelocity,
		mouseInfo->turningAcceleration,
		mouseInfo->turningRadius);

	MotionStraight(mouseInfo, motionInfo,
		mouseInfo->turningVelocity,
		mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, mouseInfo->turnOutLength);
}

void SearchingTurnAround(MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 1;
	motionInfo->endOnPost = 0;

	MotionDecel(mouseInfo, motionInfo,
		mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, mouseInfo->turnInLength);

	MotionStraight(mouseInfo, motionInfo,
		mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		0, mouseInfo->straightAccel, 8000);

	MotionTurn(mouseInfo, motionInfo,
		180, 0.6, mouseInfo->turningAcceleration, 0);

	MotionStraight(mouseInfo, motionInfo,
		0, mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, 8000);
}

TurningSettings* getSettingsForTurn(MouseInfo* mouseInfo, char nextMove)
{
	// 90 Degree Turn
	if (nextMove == 'L' || nextMove == 'R')
		return &mouseInfo->fast90;

	// 45 Degree Turn
	if (nextMove == '<' || nextMove == '>')
		return &mouseInfo->fast45;

	// 135 Degree Turn
	if (nextMove == '/' || nextMove == '\\')
		return &mouseInfo->fast135;

	if (nextMove == '^' || nextMove == 'v')
		return &mouseInfo->fastTight90;

	return SingletonFirstCell();
}

int fixDir(char move)
{
	if (move == 'L' || move == '<' || move == '/' || move == '^')
		return -1;

	return 1;
}

void update45(MotionInfo* motionInfo, char move)
{
	if (move == '<' || move == '>' ||
		move == '\\' || move == '/')
		motionInfo->is45 = !motionInfo->is45;
}

void FastTurn(MouseInfo* mouseInfo, MotionInfo* motionInfo, char currentTurn)
{
	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;
	motionInfo->endOnPost = 0;

	TurningSettings* curTurn = getSettingsForTurn(mouseInfo, currentTurn);

	MotionStraight(mouseInfo, motionInfo,
		curTurn->speed, curTurn->speed, curTurn->speed,
		mouseInfo->straightAccel,
		motionInfo->is45 ? curTurn->inLength45: curTurn->inLength);

	MotionTurn(mouseInfo, motionInfo,
		curTurn->deg * fixDir(currentTurn),
		curTurn->speed, curTurn->accel, curTurn->radius);

	update45(motionInfo, currentTurn);
}

int getOrthoOffset(MouseInfo* mouseInfo, char prevTurn, char nextTurn)
{
	const int into90Deg = -2000;
	const int out90Deg = 0;

	const int into45Deg = -6700;
	const int into135Deg = -220;

	int returnValue = 0;

	if (prevTurn == 0)
		returnValue += -5800; // First cell value

	if (nextTurn == '>')
		returnValue += -3000;

	return returnValue;
}

int getDiagonalOffset(MouseInfo* mouseInfo, char prevTurn, char nextTurn)
{
	return 0;
}

void FastStraight(
	MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int cells, char prevMove, char nextMove, int postDetect)
{
	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;

	Processor* proc = SingletonProcessor();

	TurningSettings* prevTurn = getSettingsForTurn(mouseInfo, prevMove);
	TurningSettings* nextTurn = getSettingsForTurn(mouseInfo, nextMove);

	const int lookForWall = 8000; // How early to start looking for the wall passing?
	int offset = getOrthoOffset(mouseInfo, prevMove, nextMove) - lookForWall;

	int dir = fixDir(nextMove);

	if (postDetect == 0)
		motionInfo->endOnPost = 0;
	else if (dir < 0)
	{
		proc->setLED(LED_LEFT_1, LED_ON);
		proc->setLED(LED_RIGHT_1, LED_OFF);
		motionInfo->endOnPost = -1; // -1 = look for left post
	}
	else
	{
		proc->setLED(LED_LEFT_1, LED_OFF);
		proc->setLED(LED_RIGHT_1, LED_ON);
		motionInfo->endOnPost = 1; // 1 = look for right post
	}

	MotionStraight(mouseInfo, motionInfo,
		motionInfo->currentVelocity,
		mouseInfo->fastStraightSpeed,
		nextTurn->speed,
		mouseInfo->fastStraightAccel,
		mouseInfo->normalCellDistance*cells + offset);

	proc->setLED(LED_LEFT_1, LED_OFF);
	proc->setLED(LED_RIGHT_1, LED_OFF);
}

void FastDiagonal(
	MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int cells, char prevMove, char nextMove, int postDetect)
{
	Processor* proc = SingletonProcessor();

	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;
	motionInfo->endOnPost = 0;

	TurningSettings* prevTurn = getSettingsForTurn(mouseInfo, prevMove);
	TurningSettings* nextTurn = getSettingsForTurn(mouseInfo, nextMove);

	const int lookForWall = 8000; // How early to start looking for the wall passing?
	int offset = getDiagonalOffset(mouseInfo, prevMove, nextMove) - lookForWall;

	int dir = fixDir(nextMove);

	if (postDetect == 0)
		motionInfo->endOnPost = 0;
	else if (dir < 0)
	{
		proc->setLED(LED_LEFT_1, LED_ON);
		proc->setLED(LED_RIGHT_1, LED_OFF);
		motionInfo->endOnPost = -1; // -1 = look for left post
	}
	else
	{
		proc->setLED(LED_LEFT_1, LED_OFF);
		proc->setLED(LED_RIGHT_1, LED_ON);
		motionInfo->endOnPost = 1; // 1 = look for right post
	}

	MotionStraight(mouseInfo, motionInfo,
		motionInfo->currentVelocity,
		mouseInfo->fastDiagonalSpeed,
		nextTurn->speed,
		mouseInfo->fastDiagonalAccel,
		mouseInfo->diagonalCellDistance*cells + offset);

	proc->setLED(LED_LEFT_1, LED_OFF);
	proc->setLED(LED_RIGHT_1, LED_OFF);
}
