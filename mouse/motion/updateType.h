#ifndef __UPDATE_TYPE_H__
#define __UPDATE_TYPE_H__

#include "motion.h"
#include "mouse.h"
#include "processor.h"

void updateTypeBasic(MouseInfo* mouseInfo, MotionInfo* this);
void updateTypeTurn(MouseInfo* mouseInfo, MotionInfo* this);
void updateTypeDecel(MouseInfo* mouseInfo, MotionInfo* this);

typedef void (*updateType)(MotionInfo* info);

#endif
