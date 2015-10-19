// ConfigMotors.h
//
// Author: Justin Marple
// Contact: jmarple@umass.edu
// Date: 7/5/14
//
// This file implements the PWM motor timers and
// the quadrature encoders timers.
// TIM2 and TIM5 are used for the encoders, as they are
//   32 bit resolution.  They are setup using the
//   "TIM_EncoderInterfaceConfig(...)" function, so no
//   interrupts or software is needed for counting the
//   encoders.
// The Motors used PWM which is a form of output compare
//
// ------------------------------------------------------------------------
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// The author can be contacted by email at jmarple@umass.edu
//
// ------------------------------------------------------------------------

#ifndef __CONFIG_MOTORS_H
#define __CONFIG_MOTORS_H

#include "stm32f4xx_conf.h"
#include <stdint.h>

#define ENC_DEFAULT_VALUE 0x10000

// InitMotors(...)
// Start Motor PWM and Encoders
void Config_MotorsInit();

// MotorEncoder_(...)
// Returns the current motor count
int32_t Config_GetRightEncoder();
int32_t Config_GetLeftEncoder();

// SetMotor_(...)
// Sets the PWM of the motor, ranges from 0 to 1000
void Config_SetLeftPWM(int32_t value);
void Config_SetRightPWM(int32_t value);

// MotorEncoder_Reset(...)
// Reset the encoder timer to a default value
void Config_ResetRightEncoder();
void Config_ResetLeftEncoder();
#endif

