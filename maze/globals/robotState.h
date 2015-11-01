#ifndef __ROBOT_STATE_H__
#define __ROBOT_STATE_H__

#include "mousePosition.h"

typedef struct RobotState
{
    MousePosition pos;
    
    char targetXmin, targetXmax, targetYmin, targetYmax;
} RobotState;

#endif
