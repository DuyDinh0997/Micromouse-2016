#ifndef __SENSOR_DEFINITIONS_H
#define __SENSOR_DEFINITIONS_H

// ConfigDefinitions.h
// Author: Justin Marple (jmarple@umass.edu)
// Date: 7/4/14
// 
// This file simply includes some global definitions
//   for our sensors

#include "stm32f4xx_conf.h"

#define GYRO				0x00
#define IR_FRONT_LEFT		0x01
#define IR_FRONT_RIGHT 		0x02
#define IR_SIDE_LEFT		0x03
#define IR_SIDE_RIGHT		0x04
#define IR_SIDE_LEFT_2		0x05
#define IR_SIDE_RIGHT_2		0x06

#define FRONT_LEFT_LED		0x01
#define FRONT_CENTER_LED	0x02
#define FRONT_RIGHT_LED		0x04
#define FRONT_LED			FRONT_CENTER_LED | FRONT_RIGHT_LED | FRONT_LEFT_LED
#define BACK_LEFT_LED		0x08
#define BACK_CENTER_LED		0x10
#define BACK_RIGHT_LED		0x20
#define BACK_LED			BACK_CENTER_LED | BACK_LEFT_LED | BACK_RIGHT_LED
#define ALL_LED				FRONT_LED | BACK_LED

#endif

