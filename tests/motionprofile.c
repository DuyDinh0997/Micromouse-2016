#include <stdio.h>
#include "trapProfile.h"

int main()
{
    TrapProfile profile; 
    TrapProfileReset(&profile, 0, 10, 0, 1, 50);

    float position = 0;
    float velocity = 0;
    
    while (profile.currentMode != MODE_FINISHED)
    {
        double result = TrapProfileUpdate(&profile, position, velocity, 1);
        velocity = result;
        position += velocity;
        printf("%lf Pos: %lf Vel: %lf Res: %lf\n", profile.elaspedTime, position, velocity, result);
    }    
    
    return 0; 
}
