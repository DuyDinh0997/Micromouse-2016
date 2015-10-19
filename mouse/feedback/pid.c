/*
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

#include "pid.h"

/**
 * Sets all PID values to a default value.
 */
void PIDSetup(PID* con, double kP, double kI, double kD)
{
    con->kP = kP;
    con->kI = kI;
    con->kD = kD;
    con->error = 0;
    con->integral = 0;
    con->derivative = 0;
    con->previousError = 0;
}

/**
 * Calculates the feedback result for this PID and updates it's values.
 */
double PIDUpdate(PID* pid, double dT, double error)
{
    pid->error = error;
    pid->integral += error*dT; 
    pid->derivative = (pid->error - pid->previousError) / dT;
    pid->previousError = error;
    
    return pid->kP*pid->error + 
        pid->kI*pid->integral + 
        pid->kD*pid->derivative;
}
