#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "processor.h"
#include "buzzer/buzzer.h"
#include "feedback/pid.h"
#include "menu/menuStrip.h"
#include "motion/updateType.h"
#include "motionprofile/trapProfile.h"

#define ENCODER_TICKS_PER_MM 107.0

typedef struct Mouse
{
    BuzzerBuffer buzzer;
    Menu* menu;
    void (*motionType)(void);

    TrapProfile* linearProfile;
    TrapProfile* angularProfile;

    // TODO: These should be abstracted somewhere else probably
    int encoderVelocityLeft;
    int encoderVelocityRight;
    int previousEncoderValueLeft;
    int previousEncoderValueRight;
    int targetLinearVelocity;

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

Mouse* SingletonMouse();

#endif
