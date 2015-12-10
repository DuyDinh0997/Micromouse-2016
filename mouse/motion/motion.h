#ifndef __MOTION_H__
#define __MOTION_H__

#include "trapProfile.h"

typedef struct MotionInfo
{
	int length;
	int startVelocity;
	int maxVelocity;
	int exitVelocity;
	double acceleration;

	TrapProfile* linearProfile;
	TrapProfile* angularProfile;

	int useWalls;

} MotionInfo;

void MotionStraightWithWall(MotionInfo* type);
void MotionStraight(MotionInfo*);
void MotionTurn(int degrees, TrapProfile* linearProfile, TrapProfile* angularProfile);
void MotionDecel(MotionInfo*);
#endif
