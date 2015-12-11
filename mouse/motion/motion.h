#ifndef __MOTION_H__
#define __MOTION_H__

#include "trapProfile.h"
#include "mouse.h"
#include "processor.h"

typedef struct MotionInfo
{
	int useWalls;

} MotionInfo;

void MotionStraight(MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int startVelocity, int maxVelocity, int exitVelocity, float accel, int length);

void MotionTurn(MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int degrees, int velocity, float accel, int radius);

void MotionDecel(MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int startVelocity, int exitVelocity, float accel, int length);

#endif
