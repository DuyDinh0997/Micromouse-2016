/**
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * The author can be contacted by email at jmarple@umass.edu
 */

#include "trapProfile.h" 

void TrapProfileReset(
    TrapProfile* this, 
    float startVelocity, 
    float maxVelocity, float endVelocity, 
    float acceleration, float targetDistance)
{
    this->currentMode = MODE_UNSET;
    this->elaspedTime = 0;

    this->currentVelocity = startVelocity;
    this->acceleration = acceleration;
    this->deceleration = acceleration;
    this->startVelocity = startVelocity;
    this->maxVelocity = maxVelocity;
    this->exitVelocity = endVelocity;
    this->targetDistance = targetDistance;
    this->timeDuringDecel = 1;
    this->decelOnly = 0;
}

// Tell the trap profile to skip acceleration/smaxVelocity
// and just skip to deceleration.
void TrapProfileSetDecelOnly(TrapProfile* this, float maxVelocity)
{
	this->decelOnly = 1;
	this->currentMode = MODE_T3;
	this->currentVelocity = maxVelocity;
}

int TrapProfileShouldDecelerate(TrapProfile* this,
    float tmpAccel, float distanceLeft)
{
    int dir = (this->maxVelocity < 0) ? -1 : 1; 

    float currentVelocity = this->currentVelocity;
    float acceleration = tmpAccel;
    float deceleration = this->deceleration;
    float exitVelocity = this->exitVelocity;
 
    // Max Velocity - exitVelocity;
    float effectiveVelocity = 
        currentVelocity+acceleration-exitVelocity;

    // Deceleration Time
    float n = effectiveVelocity/deceleration;
    
    float result = 
        n*exitVelocity + 
        n*(n+1)/2*deceleration - distanceLeft; 
     
    return (result*dir > 0) ? 1 : 0;
}

void TrapProfileGetTrapInfo(
    TrapProfile* this, 
    float distanceLeft)
{
    int dir = (this->maxVelocity < 0) ? -1 : 1; 

    float acceleration = this->acceleration;
    float exitVelocity = this->exitVelocity;
    //float currentVelocity = this->currentVelocity;

    // Quadratic Formula for determining how 
    //  many time steps to decelerate by
    float a = acceleration/2; 
    float b = a + exitVelocity; 
    float c = -distanceLeft;

    float quad1 = (-b + sqrtf(b*b - 4*a*c)) / (2*a);
    float quad2 = (-b - sqrtf(b*b - 4*a*c)) / (2*a);
    float quadResult = (quad1>quad2) ? quad1 : quad2; 

    // if (quadResult < 0) return -1; 
    
    int totalTime = 
        (quadResult - (int)quadResult == 0)
        ? (int)quadResult : (int)quadResult + 1;

    float vOffTest = 
        (totalTime*exitVelocity + 
        totalTime*(totalTime+1)*acceleration/2 - 
        distanceLeft) / totalTime ;

    int accelerationTime = 
        (vOffTest*dir > acceleration*dir) 
        ? totalTime-1: totalTime; 

    float vOffReal = 
        (totalTime*exitVelocity + 
        accelerationTime*(accelerationTime+1)*acceleration/2 - 
        distanceLeft) / accelerationTime;

    float startVelocity = 
        exitVelocity + accelerationTime * acceleration - vOffReal;

    this->decelerationLength = totalTime;
    this->decelMaxVelocity = startVelocity;
}

PID* TrapProfileGetLeftPID(TrapProfile* this)
{
	if (this->currentMode == MODE_T1)
		return this->lT1;

	if (this->currentMode == MODE_T2)
		return this->lT2;

	if (this->currentMode == MODE_T3)
		return this->lT3;

	if (this->currentMode == MODE_UNSET)
		return this->lT1;

	return this->lT3;
}

PID* TrapProfileGetRightPID(TrapProfile* this)
{
	if (this->currentMode == MODE_T1)
		return this->rT1;

	if (this->currentMode == MODE_T2)
		return this->rT2;

	if (this->currentMode == MODE_T3)
		return this->rT3;

	if (this->currentMode == MODE_UNSET)
		return this->rT1;

	return this->rT3;
}

float TrapProfileUpdate(
    TrapProfile* this, 
    float pos, float velocity, float dT)
{
    int dir = (this->maxVelocity < 0) ? -1 : 1; 

    this->elaspedTime += dT;

    switch(this->currentMode)
    {
        // Currently Accelerating
        case MODE_UNSET:
            this->currentMode = MODE_T1;
            // Intentionally don't break here, continue to T1

        case MODE_T1:
        case MODE_T2:

            // Current Velocity
            this->currentVelocity = 
                this->startVelocity + this->acceleration*this->elaspedTime;
           
            float tmpAccel = this->acceleration; 
            if (this->currentVelocity*dir >= this->maxVelocity*dir)
            {
            	this->currentMode = MODE_T2;
                tmpAccel = 0; 
                this->currentVelocity = this->maxVelocity;
            }

            if (TrapProfileShouldDecelerate(this, tmpAccel,
                this->targetDistance - pos))
            {
                if (this->exitVelocity*dir >= this->maxVelocity*dir)
                {
                    this->currentMode = MODE_FINISHED;
                    return this->exitVelocity;
                }

                this->currentMode = MODE_T3;   
                TrapProfileGetTrapInfo(this, this->targetDistance - pos); 
                this->currentVelocity = this->decelMaxVelocity;
            }

            if (this->deceleration != 0)
            {
                // Forget about T3 or something

            }

            return this->currentVelocity;

        // Currently Decelerating
        case MODE_T3:

            this->currentVelocity -= this->deceleration;

            if (this->currentVelocity*dir <= this->exitVelocity*dir)
                this->currentVelocity = this->exitVelocity;

            this->timeDuringDecel++; 

            //if (this->timeDuringDecel >= this->decelerationLength)
            //   this->currentMode = MODE_FINISHED;

            if (this->currentVelocity*dir <= this->exitVelocity*dir)
            	this->currentMode = MODE_FINISHED;

            return this->currentVelocity;

        // After finished
        case MODE_FINISHED:
            return this->exitVelocity;

        // This should never happen.
        default:
            return 0;
    }
}
