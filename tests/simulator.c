#include "processor.h"
#include "mouse.h"

int main(int args, char* argv[])
{
    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    mouse->initiate();
    BuzzerBufferAddTone(&mouse->buzzer, 800, 200);

     
    

    while (1==1);
    return 0;
}
