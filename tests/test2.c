#include <stdio.h>
#include "pid.h"

int main()
{
    PID pid;
    PIDSetup(&pid, 1, 0, 0);
    printf("Yo2\n");
    return 0;
}
