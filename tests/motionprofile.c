#include <stdio.h>
#include "trapProfile.h"
#include <math.h>

/*typedef struct TrapProfileInfo
{
    int decelerationTime; 
    double decelMaxVelocity;
} TrapProfileInfo;

void calculateTrapInfo(
    double currentVelocity, 
    double distanceLeft, 
    double acceleration, 
    TrapProfileInfo* info,
    double exitVelocity)
{
    // Quadratic Formula to determine length of deceleration
    double a = acceleration/2; 
    double b = a + exitVelocity; 
    double c = -distanceLeft;

    double quad1 = (-b + sqrt(b*b - 4*a*c)) / (2*a);
    double quad2 = (-b - sqrt(b*b - 4*a*c)) / (2*a);
    double quadResult = (quad1 > quad2) ? quad1 : quad2; 
  
    // if (quadResult < 0) return -1;
 
    int totalTime = (quadResult - (int)quadResult == 0)
        ? (int)quadResult : (int)quadResult + 1;

    double vOffTest = 
        (totalTime*exitVelocity + 
        totalTime*(totalTime+1)*acceleration/2 - 
        distanceLeft) / totalTime;
    printf("vOffTest = %lf\n", vOffTest);    

    int accelerationTime = (vOffTest > acceleration) ? totalTime-1: totalTime; 
    printf("accelerationTime = %d\n", accelerationTime);

    double vOffReal = 
        (totalTime*exitVelocity + 
        accelerationTime*(accelerationTime+1)*acceleration/2 - 
        distanceLeft) / accelerationTime;
    printf("VoffReal = %lf\n", vOffReal);

    double startVelocity = 
        exitVelocity + accelerationTime * acceleration - vOffReal;

    printf("%lf, %lf, %lf, %lf\n", 
        currentVelocity, distanceLeft, acceleration, exitVelocity);
    printf("%d, %lf\n", totalTime, startVelocity);

    info->decelerationTime = totalTime;
    info->decelMaxVelocity = startVelocity;
}

int shouldDecelerate(
    double distanceLeft, 
    double currentVelocity,
    double exitVelocity,
    double acceleration,
    double deceleration) 
{
    // Max Velocity - exitVelocity;
    double effectiveVelocity = currentVelocity+acceleration-exitVelocity;

    // Deceleration Time
    double n = effectiveVelocity/deceleration;
    
    double result = n*exitVelocity + n*(n+1)/2*deceleration - distanceLeft; 
    
    return (result > 0) ? 1 : 0;
}
*/
int main()
{
    printf("Starting Motion Profile Test\n"); 

    float pos = 0;
    float velocity = 0;
    float accel = 1.0;
    float startVelocity = 8;
    float maxVelocity = 5.0;
    float exitVelocity = 0.0;
    float targetDistance = 90;

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

    /*while (1)
    {
        printf("Time = %d, Pos = %lf, velocity = %lf\n", time, pos, velocity);

        // Set Acceleration to zero if at max velocity already.
        double tmpAccel = velocity >= maxVelocity ? 0 : accel; 

        if (shouldDecelerate(
            targetDistance-pos, velocity, velocityExit, tmpAccel, accel))
        {
            printf("Breaking!\n");
            calculateTrapInfo(
                velocity, targetDistance-pos, accel, &info, velocityExit);
            break;      
        } 

        velocity += accel; 
        if (velocity > maxVelocity) velocity = maxVelocity;
        pos += velocity; 
        time++;
    } 

    printf("New Velocity = %lf\n", info.decelMaxVelocity);
    printf("Deceleration Time = %d\n", info.decelerationTime); 
  
    velocity = info.decelMaxVelocity;
    pos += velocity;
    time++;
 
    int i; 
    for (i = 0; i < info.decelerationTime; i++)
    {
        printf("Time = %d, Pos = %lf, velocity = %lf\n", time, pos, velocity);

        velocity -= accel;
        if (velocity < velocityExit) velocity = velocityExit;
        pos += velocity;
        time++;
    } 
    */ 
    printf("Done!\n");

    return 0;
}
