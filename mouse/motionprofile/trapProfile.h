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

#ifndef __TRAP_PROFILE_H__
#define __TRAP_PROFILE_H__

#include <math.h>
#include "../feedback/pid.h"

#define MODE_UNSET      0x00
#define MODE_T1         0x01
#define MODE_T2         0x02
#define MODE_T3         0x03
#define MODE_FINISHED   0x04     

typedef struct TrapProfile
{
	PID* lT1;
	PID* lT2;
	PID* lT3;
	PID* rT1;
	PID* rT2;
	PID* rT3;

    float startVelocity;
    float maxVelocity;
    float exitVelocity; 

    float acceleration;
    float deceleration;
    float targetDistance;

    float elaspedTime;

    int currentMode; 

    int startingPosition;

    float decelerationLength;
    float decelMaxVelocity; 
    float currentVelocity;

    int timeDuringDecel;
} TrapProfile;

void TrapProfileReset(TrapProfile* this, float startVelocity, float maxVelocity, float endVelocity, float acceleration, float targetDistance);
float TrapProfileUpdate(TrapProfile*, float, float, float);
PID* TrapProfileGetLeftPID(TrapProfile*);
PID* TrapProfileGetRightPID(TrapProfile*);
#endif
