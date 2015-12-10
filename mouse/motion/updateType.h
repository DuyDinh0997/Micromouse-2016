#ifndef __UPDATE_TYPE_H__
#define __UPDATE_TYPE_H__

#include "mouse.h"
#include "processor.h"
#include "motion.h"

void updateTypeBasic(MotionInfo* this);
void updateTypeTurn(MotionInfo* this);
void updateTypeDecel(MotionInfo* this);

typedef void (*updateType)(MotionInfo* info);

#endif
