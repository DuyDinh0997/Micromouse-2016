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
#ifndef __SETUP_SERIAL_OPERATIONS
#define __SETUP_SERIAL_OPERATIONS

#include "stm32f4xx_conf.h"
#include "setupSerial.h"

// SerialSaveString(...)
// Adds a string to the buffer to be sent
void Config_SerialSaveString(const char* message);

// SerialSaveNumber(...)
// Sends only the minimum number of bytes necessary for data to be sent
void Config_SerialSaveNumber(long data);

// SerialSaveRawInteger(...)
// Adds 4 bytes to the buffer to be sent
void Config_SerialSaveRawInteger(int data);

// SerialSaveIntegerAsString(...)
// Adds the integer as a string to the buffer
void Config_SerialSaveIntegerAsString(int num);


void Config_SerialSaveDoubleAsString(double num);

#endif
