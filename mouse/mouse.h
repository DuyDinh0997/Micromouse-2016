#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "processor.h"
#include "buzzer/buzzer.h"
#include "feedback/pid.h"
#include "menu/menuStrip.h"
#include "motion/updateType.h"
#include "motion/motion.h"
#include "motionprofile/trapProfile.h"

#define ENCODER_TICKS_PER_MM 107.0

#define LEFT_WALL 36000
#define RIGHT_WALL 32000
#define WALL_EXISTS 12000

typedef struct Mouse
{
    BuzzerBuffer buzzer;
    Menu* menu;
    void (*motionType)(MotionInfo* info);

    TrapProfile* linearProfile;
    TrapProfile* angularProfile;

    // TODO: These should be abstracted somewhere else probably
    int encoderVelocityLeft;
    int encoderVelocityRight;
    int previousEncoderValueLeft;
    int previousEncoderValueRight;
    int targetLinearVelocity;

    int canUpdate;

    PID motorPIDLeft;
    PID motorPIDRight;
    PID gyroVelocityPID;
    PID gyroPositionPID;

    double motorValueLeft;
    double motorValueRight;
    double motorValueAngular;

    // Current mouse rotation in degrees (Note: Left 90 deg turn = -90)
    float gyroPosition;
    float gyroVelocity;
    float gyroCalibration;
    long time;

    void (*initiate)(); 
    void (*calibrateGyro)();
} Mouse;

// Settings for the mouse
typedef struct MouseInfo
{
	double straightVelocity;
	double turningVelocity;
	double turnInLength;
	double turnOutLength;
	double turningRadius;
	double acceleration;
	double turningAcceleration;

	// The value when searching and there is a
	// wall in front where the mouse should
	// start turning.
	double searchingWallFrontValue;

	// The value when searching and going down a
	// a straight the wall PID's should correct to.
	double searchingLeftWallValue;
	double searchingRightWallValue;

	// PID's for trap profile.
	PID lT1, lT2, lT3, rT1, rT2, rT3;

	//
	PID gT1, gT2, gT3;

} MouseInfo;

Mouse* SingletonMouse();

#endif
