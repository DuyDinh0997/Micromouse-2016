#include "mousePosition.h"

void MousePositionInit( 
    MousePosition* this, Compass forwardDir, int startingX, int startingY)
{
    this->forwardDirection = forwardDir;
    this->x = startingX; 
    this->y = startingY;
}

void MousePositionGoForward(MousePosition* this)
{
    if (this->forwardDirection == NORTH) this->y -= 1;
    if (this->forwardDirection == SOUTH) this->y += 1;
    if (this->forwardDirection == WEST)  this->x -= 1;
    if (this->forwardDirection == EAST)  this->x += 1;

    if (this->y < 0) this->y = 0;
    if (this->y > 15) this->y = 15;
    if (this->x < 0) this->x = 0;
    if (this->x > 15) this->x = 15;
}

void MousePositionGoBackward(MousePosition* this)
{
    if (this->forwardDirection == SOUTH) this->y -= 1;
    if (this->forwardDirection == NORTH) this->y += 1;
    if (this->forwardDirection == EAST)  this->x -= 1;
    if (this->forwardDirection == WEST)  this->x += 1;

    if (this->y < 0) this->y = 0;
    if (this->y > 15) this->y = 15;
    if (this->x < 0) this->x = 0;
    if (this->x > 15) this->x = 15;
}

void MousePositionRotateLeft(MousePosition* this)
{
    if (this->forwardDirection == NORTH)
        this->forwardDirection = WEST;
    else if (this->forwardDirection == SOUTH)
        this->forwardDirection = EAST;
    else if (this->forwardDirection == WEST)
        this->forwardDirection = SOUTH;
    else if (this->forwardDirection == EAST)
        this->forwardDirection = NORTH;
}

void MousePositionRotateRight(MousePosition* this)
{
    if (this->forwardDirection == NORTH)
        this->forwardDirection = EAST;
    else if (this->forwardDirection == SOUTH)
        this->forwardDirection = WEST;
    else if (this->forwardDirection == WEST)
        this->forwardDirection = NORTH;
    else if (this->forwardDirection == EAST)
        this->forwardDirection = SOUTH;
}

