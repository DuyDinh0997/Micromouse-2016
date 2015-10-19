#ifndef __CONFIG_SENSORS_H
#define __CONFIG_SENSORS_H

// ConfigSensors.h
// Author: Justin Marple (jmarple@umass.edu)
// Date: 7/4/14
// 
// This file implements the IR sensor loop at 1Khz
// There are 5 sensors in total and the timer
//   sends an interrupt every 200us
// The Timer uses 2 output compares, one to turn
//   on the Sensor Emitter, then another 80us later
//   to turn on the ADC conversion.  There is an EOC
//   interrupt when the ADC is done that will save the
//   value of the sensor. 
// It takes a total of about ~95usec for the whole 
//   process to finish, so the loop can be improved
//   to run at 2Khz if needed

#include "stm32f4xx_conf.h"
#include "SensorDefinitions.h"

// SensorInit(...)
// Initializes and starts the sensors routine
void Config_SensorInit();

// SensorGetValue(...)
// Fetches the current value of a certain sensor
// Sensor Indexes are located in SensorDefinitions.h
int32_t Config_SensorGetValue(int32_t sensor);
int32_t Config_SensorGetAmbientValue(int32_t sensor);
int32_t Config_SensorGetAdjValue(int32_t sensor);

#endif

