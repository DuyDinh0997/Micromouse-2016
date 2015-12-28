#ifndef __CELL_TO_CELL_H__
#define __CELL_TO_CELL_H__

#include "mouse.h"
#include "motion.h"

void SearchingFirstCell(MouseInfo* mouseInfo, MotionInfo* motionInfo);
void SearchingStraight(MouseInfo* mouseInfo, MotionInfo* motionInfo);
void SearchingTurn(int degrees, MouseInfo* mouseInfo, MotionInfo* motionInfo);
void SearchingTurnAround(MouseInfo* mouseInfo, MotionInfo* motionInfo);
void FastStraight(
	MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int cells, char prevMove, char nextMove, int);
void FastTurn(MouseInfo* mouseInfo, MotionInfo* motionInfo, char currentTurn);
void FastDiagonal(
	MouseInfo* mouseInfo, MotionInfo* motionInfo,
	int cells, char prevMove, char nextMove, int);

#endif
