#include "cellToCell.h"

void SearchingFirstCell(MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	Mouse* mouse = SingletonMouse();

    // Start at a higher speed so that acceleration happens sooner.
    mouse->motorValueLeft = 100;
    mouse->motorValueRight = 100;

	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;

	MotionStraight(mouseInfo, motionInfo,
    	0, mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, mouseInfo->firstCellDistance);
}

void SearchingStraight(MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;

	MotionStraight(mouseInfo, motionInfo,
    	mouseInfo->straightVelocity, mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, mouseInfo->normalCellDistance);
}

void SearchingTurn(int degrees, MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	motionInfo->useWalls = 0;
	motionInfo->turnInPlace = 0;

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
