#ifndef __MOUSE_H__
#define __MOUSE_H__

#include "processor.h"
#include "buzzer/buzzer.h"
#include "feedback/pid.h"
#include "menu/menuStrip.h"

typedef struct Mouse
{
    BuzzerBuffer buzzer;
    Menu* menu;

    PID tmppid;
    PID angpid;
   
    float degrees; 
    float gyroCalibration;
    long time;

    void (*initiate)(); 
} Mouse;

Mouse* SingletonMouse();

#endif
