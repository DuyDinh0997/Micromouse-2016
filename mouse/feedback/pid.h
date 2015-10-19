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

#ifndef __PID_H__
#define __PID_H__

/** 
 * Structure that holds information for each PID controller 
 */
typedef struct PID
{
    // Tuning Constants.
    double kP, kI, kD;
   
    double error, integral, derivative, previousError;
    
} PID;

// Updates the PID controller.
double PIDUpdate(PID* pid, double dT, double error);

// Sets the default values for all PID variabels.
void PIDSetup(PID* con, double kP, double kI, double kD);

#endif
