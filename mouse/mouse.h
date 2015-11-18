#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "processor.h"
#include "buzzer/buzzer.h"
#include "feedback/pid.h"
#include "menu/menuStrip.h"
#include "motion/updateType.h"
#include "motionprofile/trapProfile.h"

typedef struct Mouse
{
    BuzzerBuffer buzzer;
    Menu* menu;
    void (*motionType)(void);

    TrapProfile* profile;

    // TODO: These should be abstracted somewhere else probably
    int encoderVelocityLeft;
    int encoderVelocityRight;

    PID motorPIDLeft;
    PID motorPIDRight;
   
    float degrees; 
    float gyroCalibration;
    long time;

    void (*initiate)(); 
    void (*calibrateGyro)();
} Mouse;

Mouse* SingletonMouse();

#endif
