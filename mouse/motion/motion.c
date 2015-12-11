#include "motion.h"
#include "updateType.h"

void MotionDecel(MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int startVelocity, int exitVelocity, float accel, int length)
{
	Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

	// Go forward again.
    proc->resetSensor(SENSOR_ENCODER_LEFT);
    proc->resetSensor(SENSOR_ENCODER_RIGHT);
    mouse->previousEncoderValueLeft = 0;
    mouse->previousEncoderValueRight = 0;

    TrapProfileReset(&mouseInfo->linearProfile,
    	startVelocity, startVelocity, exitVelocity,
		accel, length);

	TrapProfileSetDecelOnly(&mouseInfo->linearProfile, startVelocity);

    TrapProfileReset(&mouseInfo->angularProfile,
    	0, 0, 0,
		0, 0);

    updateTypeDecel(mouseInfo, motionInfo);
}

void MotionStraight(MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int startVelocity, int maxVelocity, int exitVelocity, float accel, int length)
{
    Mouse* mouse = SingletonMouse();
    Processor* proc = SingletonProcessor();

	// Go forward again.
    proc->resetSensor(SENSOR_ENCODER_LEFT);
    proc->resetSensor(SENSOR_ENCODER_RIGHT);
    mouse->previousEncoderValueLeft = 0;
    mouse->previousEncoderValueRight = 0;

    TrapProfileReset(&mouseInfo->linearProfile,
    	startVelocity, maxVelocity, exitVelocity,
		accel, length);

    TrapProfileReset(&mouseInfo->angularProfile,
    	0, 0, 0,
		0, 0);

    updateTypeBasic(mouseInfo, motionInfo);
}

void MotionTurn(MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int degrees, int velocity, float accel, int turnRadius)
{
    Mouse* mouse = SingletonMouse();

    int dir = (degrees < 0) ? -1 : 1;

    // Turn 90 deg
    float angVelocityRadian = velocity / (turnRadius * ENCODER_TICKS_PER_MM);
    float angVelocityDegrees = angVelocityRadian * 57.2957795 * dir; // 57.295 is the scaling between radians and degrees.

    TrapProfileReset(&mouseInfo->angularProfile,
    	0, angVelocityDegrees, 0, accel*dir, degrees-3*dir);

    mouse->motorValueLeft += 100*dir;
    mouse->motorValueRight -= 100*dir;

    updateTypeTurn(mouseInfo, 0);

    mouse->gyroPosition -= degrees;
}
