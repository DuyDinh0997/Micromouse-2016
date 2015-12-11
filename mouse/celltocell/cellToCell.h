#ifndef __CELL_TO_CELL_H__
#define __CELL_TO_CELL_H__

#include "mouse.h"
#include "motion.h"

void SearchingFirstCell(MouseInfo* mouseInfo, MotionInfo* motionInfo);
void SearchingStraight(MouseInfo* mouseInfo, MotionInfo* motionInfo);
void SearchingTurn(int degrees, MouseInfo* mouseInfo, MotionInfo* motionInfo);

#endif
