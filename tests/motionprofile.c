#include <stdio.h>
#include "trapProfile.h"
#include <math.h>

typedef struct TrapProfileInfo
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

int main()
{
    printf("Starting Motion Profile Test\n"); 

    double pos = 0;
    double velocity = 0;
    double accel = 1.435;
    double maxVelocity = 7.0;
    double velocityExit = 1.0;
    double targetDistance = 100;
    int time = 0;
    TrapProfileInfo info;

    while (1)
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
    
    printf("Done!\n");
    //calculateTrapInfo(5, 13, 1, 2);

    return 0;

    /*while (1)
    {
        if (shouldStartToDecel(
            time, targetDistance - pos, velocityExit, accel) == 1) 
            break;

        velocity += accel;  
        pos += velocity;
        time++; 

        printf("Pos = %lf, Vel = %lf, Time = %d\n\n", pos, velocity, time);
    }
        
    printf("Pos = %lf, Vel = %lf, Time = %d\n\n", pos, velocity, time);

    printf("SWITCHING\n");

    int steptime = getDiscreteTimeNeeded(velocity + accel, velocityExit, accel);   
    double Vco = calculateVcutoff(steptime, targetDistance - pos, velocityExit, accel);

    printf("StepTime = %d, Vco = %lf\n", steptime, Vco);

    if (Vco <= 1)
    {
        printf("A\n");
        velocity += (1-Vco); 

        while (1)
        {
            // New Values
            printf("Pos = %lf, Vel = %lf, Time = %d\n\n", pos, velocity, time);
            if (velocity < velocityExit) break;

            // Wait here for one time period

            // Update values here
            pos+=velocity;
            velocity-=accel;
            time++;


        }
    }
    else
    {
        double Vco = calculateVcutoff(steptime-1, targetDistance - pos, velocityExit, accel);
        printf("Vco = %lf, st = %d\n", Vco, steptime);
        velocity += Vco;

        while (1)
        {
            pos+=velocity;
            velocity-=accel;
            time++;
            printf("Pos = %lf, Vel = %lf, Time = %d\n\n", pos, velocity, time);
            
            if (velocity < 0) break;
        }    
    }

    printf("Pos = %lf, Vel = %lf, Time = %d\n\n", pos, velocity, time);

    return 0; */
}
