#ifndef __UPDATE_TYPE_H__
#define __UPDATE_TYPE_H__

#include "mouse.h"
#include "processor.h"

void updateTypeBasic();
void updateTypeTurn();

typedef void (*updateType)();

#endif
