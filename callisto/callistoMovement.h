#ifndef __CALLISTO_MOVEMENT_H__
#define __CALLISTO_MOVEMENT_H__

#include "mouse.h"
#include "motion.h"
#include "maze.h"
#include "robotState.h"
#include "floodFill.h"

void CallistoExecuteMovement(char move, MousePosition* pos, MouseInfo* mouseInfo, MotionInfo* motionInfo);
void CallistoSearch(Maze* maze, RobotState* state, MouseInfo* mouseInfo, MotionInfo* motionInfo);
void CallistoPrintMaze(Maze* this);

#endif
