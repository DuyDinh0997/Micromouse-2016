#include "floodFillStack.h"

void StackInit(Stack* this)
{
    this->pos = -1;
}

void* StackPop(Stack* this)
{
    if (this->pos < 0)
    {
        this->pos = -1;
        return 0;
    }
    else
    {
        this->pos--;
        return this->nodes[this->pos+1];
    }
}

void StackPush(Stack* this, void* cell)
{
    this->pos++;
    this->nodes[this->pos] = cell; 
}

void* StackPeek(Stack* this)
{
    if (this->pos >= 0) return this->nodes[this->pos];

    return 0;
}

int StackIsEmpty(Stack* this)
{
    return (this->pos == -1);
}

void StackReset(Stack* this)
{
    this->pos = -1;
}
