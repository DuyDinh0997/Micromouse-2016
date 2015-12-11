#include <stdio.h>
#include "trapProfile.h"
#include <math.h>

int main()
{
    printf("Starting Motion Profile Test\n"); 

    float pos = 0;
    float velocity = 0;
    float accel = 0.5;
    float startVelocity = 10;
    float maxVelocity = 75;
    float exitVelocity = 75;
    float targetDistance = 14200;

    int time = 0;
    //TrapProfileInfo info;

    TrapProfile prof;

    TrapProfileReset(&prof, 
        startVelocity, maxVelocity, exitVelocity, accel, targetDistance); 

    while (1)
    {
        printf("Time = %d, Pos = %lf, velocity = %lf\n", time, pos, velocity);
        if (prof.currentMode == MODE_FINISHED) break;
    
        velocity = TrapProfileUpdate(&prof, pos, velocity, 1); 
        pos += velocity;
        time++;
    } 

    printf("Done!\n");

    return 0;
}
