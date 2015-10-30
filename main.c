#include <stdlib.h>
#include "setupGPIO.h"
#include "processor.h"
#include "mouse.h"

int main(void) 
{
    // Wait for everything to stableize?
    Mouse* mouse = SingletonMouse();
   
    mouse->initiate();
    BuzzerBufferAddTone(&mouse->buzzer, 800, 200); 
    BuzzerBufferAddTone(&mouse->buzzer, 1600, 200);
    BuzzerBufferAddTone(&mouse->buzzer, 800, 200);

    while(1==1);

    return 0;
}


