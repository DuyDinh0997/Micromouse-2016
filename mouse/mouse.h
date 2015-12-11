#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "processor.h"
#include "motionprofile/trapProfile.h"
#include "feedback/pid.h"
#include "buzzer/buzzer.h"
#include "menu/menuStrip.h"

#define ENCODER_TICKS_PER_MM 107.0

#define LEFT_WALL 36000
#define RIGHT_WALL 32000
#define WALL_EXISTS 12000

typedef struct Mouse
{
    BuzzerBuffer buzzer;
    Menu* menu;

    // TODO: These should be abstracted somewhere else probably
    int encoderVelocityLeft;
    int encoderVelocityRight;
    int previousEncoderValueLeft;
    int previousEncoderValueRight;

    int canUpdate;

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
	double currentVelocity;
	double straightVelocity;
	double turningVelocity;
	double turnInLength;
	double turnOutLength;
	double turningRadius;
	double straightAccel;
	double turningAcceleration;

	// The value when searching and there is a
	// wall in front where the mouse should
	// start turning.
	double searchingWallFrontValue;

	// The value when searching and going down a
	// a straight the wall PID's should correct to.
	double searchingLeftWallValue;
	double searchingRightWallValue;
	double searchingWallExists;

	// PID's for trap profile.
	PID lT1, lT2, lT3, rT1, rT2, rT3;

	// Angular velocity PID's
	// TODO: Do these ever get used now?
    PID glT1, glT2, glT3, grT1, grT2, grT3;

	TrapProfile linearProfile;
	TrapProfile angularProfile;

	// PID's for straight correction.
	PID gyroPositionPID, wallPositionPID;

} MouseInfo;

Mouse* SingletonMouse();

#endif
