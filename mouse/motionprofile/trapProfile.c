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

double TrapProfileGetExchangeTime(TrapProfile* this);

void TrapProfileReset(TrapProfile* this)
{
    this->currentMode = MODE_UNSET;
    this->elaspedTime = 0;
    this->startPosition = 0;

    float dir = 1 - 2*(this->startVelocity > this->maxVelocity);

    // Get the time at which the acceleration will switch over to the deceleration
    this->exchangeTime = TrapProfileGetExchangeTime(this);
    this->velocityAtExchange = this->startVelocity + this->acceleration * this->exchangeTime;

    this->willReachMaxVelocity = (this->velocityAtExchange*dir < this->maxVelocity*dir) ? 0 : 1;
}

double TrapProfileGetExchangeTime(TrapProfile* this)
{
    float accel = this->acceleration;
    float decel = this->deceleration;

    float veli = this->startVelocity;
    float vele = this->exitVelocity;

    float distance = this->targetDistance;

    // Perform Quadratic formula
    // Explanation:
    // We need to find the time at which the acceleration and deceleration switch over
    // So we know we will travel s0(distance) when accelerating and s1(distance)
    //  when decelerating
    // We will denote the time of the change-over as t
    // From kinematics:
    //  s0 = v_inital * t + 1/2 * acceleration * t^2
    //  s1 = (v_final^2 - v_m^2) / (2*deceleration)
    //  v_m = v_initial + acceleration * t
    //  (where v_m is the velocity at the point of change-over)
    // Since we know the desired distance of travel, s, we can say
    //  s = s0 + s1
    // At this point, do the algebra and you will come out with the following terms for
    //  the quadratic equation when solving for t
    float a = (accel * decel - accel * accel);
    float b = (2 * veli * decel - 2 * veli * accel);
    float c = (vele * vele - veli * veli - distance * 2 * decel);

    float squareTerm = b*b - 4*a*c;
    if(squareTerm < 0)
    {
        // Error no possible motion profile possible
        return 0;
    }

    // Acceleration must equal zero, so assume max velocity will
    //  be immediately reached
    if(a == 0)
    {
        return 0;
    }

    // Choose the option that comes out positive
    float option1 = (-b + (float)sqrt(squareTerm))/(2.0*a);
    float option2 = (-b - (float)sqrt(squareTerm))/(2.0*a);

    if(option1 > 0 && option2 < 0)
        return option1;
    if(option2 > 0 && option1 < 0)
        return option2;
    if(option1 < 0 && option2 < 0)
    {
        // Both times are negative D:
        return 0;
    }

    return option1;
}

double TrapProfileUpdate(TrapProfile* this, float position, float velocity, float dT)
{
    this->elaspedTime += dT;

    position -= this->startPosition;

    float currentVelocity;
    float timeToStop = 0;
    float distanceToStop = 0;
    float decelTime = 0;

    float dir = 1 - 2 * (this->startVelocity > this->maxVelocity);

    // If "constant" speed.
    if (this->startVelocity == this->maxVelocity && 
        this->startVelocity == this->exitVelocity)
    {
        if (this->targetDistance*dir <= position*dir)
            this->currentMode = MODE_FINISHED;

        return this->exitVelocity;
    }

    switch(this->currentMode)
    {
        // Currently Accelerating
        case MODE_UNSET:
            this->currentMode = MODE_T1;
            this->startPosition = position;
            position = 0;
            // Intentionally don't break here, continue to T1

        case MODE_T1:

            // Current Velocity
            currentVelocity = this->startVelocity + this->acceleration*this->elaspedTime;

            if (currentVelocity*dir >= this->maxVelocity*dir)
            {
                this->currentMode = MODE_T2;
                return this->maxVelocity;
            }

            if (this->deceleration != 0)
            {
                timeToStop = (this->exitVelocity - currentVelocity) / this->deceleration;
                distanceToStop = 
                    currentVelocity * timeToStop + 
                    0.5 * this->deceleration * timeToStop * timeToStop;
            }
            
            if ((this->targetDistance - position)*dir < distanceToStop*dir)
            {
                this->currentMode = MODE_T3;
                this->maxVelocity = currentVelocity;
                this->timeWhenStartingToDecelerating = this->exchangeTime;
                return this->maxVelocity;
            }

            return currentVelocity;

        // Currently at Max Speed
        case MODE_T2:

            // Calculate deceleration value
            if (this->deceleration != 0)
            {
                timeToStop = 
                    (this->exitVelocity - this->maxVelocity) / this->deceleration;
                distanceToStop = 
                    this->maxVelocity * timeToStop + 
                    0.5 * this->deceleration * timeToStop * timeToStop;
            }

            // Check to see if this profile has traveled far enough to move onto 
            // decelerating.
            if ((this->targetDistance - position - velocity*dT)*dir <= 
                (distanceToStop)*dir)
            {
                float distError = 
                    this->targetDistance - position - velocity*dT - distanceToStop;
                float timeOffset = distError / velocity;
                
                this->currentMode = MODE_T3;
                this->timeWhenStartingToDecelerating = this->elaspedTime - timeOffset;
                return this->maxVelocity;
            }

            return this->maxVelocity;

        // Currently Decelerating
        case MODE_T3:

            timeToStop = (this->exitVelocity - this->maxVelocity) / this->deceleration;
            decelTime = this->elaspedTime - this->timeWhenStartingToDecelerating;
            currentVelocity = this->maxVelocity + this->deceleration * decelTime;

            if (this->elaspedTime - this->timeWhenStartingToDecelerating >= timeToStop)
            {
                this->currentMode = MODE_FINISHED;
                return this->exitVelocity;
            }

            return currentVelocity;

        // After finished
        case MODE_FINISHED:
            return this->exitVelocity;

        // This should never happen.
        default:
            return 0;
    }
}
