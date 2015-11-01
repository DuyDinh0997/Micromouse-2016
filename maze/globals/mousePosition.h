#ifndef __MOUSE_POSITION_H__
#define __MOUSE_POSITION_H__

#include "maze.h"

typedef struct MousePosition
{
    int x;
    int y;
    Compass forwardDirection;
} MousePosition;

void MousePositionInit(MousePosition* this);

void MousePositionGoForward(MousePosition* this);
void MousePositionGoBackward(MousePosition* this);
void MousePositionRotateLeft(MousePosition* this);
void MousePositionRotateRight(MousePosition* this);
#endif
