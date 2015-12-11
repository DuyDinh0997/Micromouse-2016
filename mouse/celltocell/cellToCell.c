#include "cellToCell.h"

void SearchingFirstCell(MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	Mouse* mouse = SingletonMouse();

    // Start at a higher speed so that acceleration happens sooner.
    mouse->motorValueLeft = 100;
    mouse->motorValueRight = 100;

	MotionStraight(mouseInfo, motionInfo,
    	0, mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, mouseInfo->firstCellDistance);
}

void SearchingStraight(MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	MotionStraight(mouseInfo, motionInfo,
    	mouseInfo->straightVelocity, mouseInfo->straightVelocity, mouseInfo->straightVelocity,
		mouseInfo->straightAccel, mouseInfo->normalCellDistance);
}

void SearchingTurn(int degrees, MouseInfo* mouseInfo, MotionInfo* motionInfo)
{
	motionInfo->useWalls = 0;

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
