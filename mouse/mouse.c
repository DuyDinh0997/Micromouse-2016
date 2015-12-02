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

#include "mouse.h"
#include <stdlib.h>

void MouseInitiate();
void MouseCalibrateGyro();

Mouse* SingletonMouse()
{
    static Mouse* mouse;
    
    if (mouse == 0)
    {
        mouse = malloc(sizeof(Mouse));
        mouse->initiate = MouseInitiate;
        mouse->calibrateGyro = MouseCalibrateGyro;
        mouse->buzzer.msCounter = 0;
        mouse->buzzer.end = 0;
        mouse->buzzer.start = 0;
        mouse->motionType = 0;
		mouse->motorValueLeft = 0;
		mouse->motorValueRight = 0;
    }

    return mouse;
}

void MouseUpdate();

void MouseInitiate()
{
    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    proc->setupGPIO();
    proc->setupMotors();
    proc->setupSensors();
    proc->setupSerial(57600);
    proc->setScreenWithString("Menu");

    PIDSetup(&mouse->motorPIDLeft, 0.2, 0.0, 0.0);
    PIDSetup(&mouse->motorPIDRight, 0.2, 0.0, 0.0);
    PIDSetup(&mouse->gyroVelocityPID, 1, 0.0, 0.0);
    PIDSetup(&mouse->gyroPositionPID, 1, 0.0, 0.0);

    mouse->gyroCalibration = 0;

    // Everytime the processor updates every 1ms, this function is called.
    proc->callback = MouseUpdate;
}

void MouseCalibrateGyro()
{
    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    long startTime = mouse->time;

    long thisTime = startTime;
    int lc = 0;

    proc->setLED(LED_LEFT_2, LED_ON);

    mouse->gyroCalibration = 0;

    while (mouse->time - startTime < 1000)
    {
        mouse->gyroCalibration += proc->getSensor(SENSOR_GYRO);

        /*proc->writeMemoryWithString("L,");
    	proc->writeMemoryWithInt(proc->getSensor(SENSOR_GYRO));
        proc->writeMemoryWithString("Y,");
    	proc->writeMemoryWithInt(mouse->time);
     	proc->writeMemoryWithString("\n");*/

    	lc++;

        while (mouse->time <= thisTime)
        {

        }

        thisTime++;
    }

    mouse->gyroCalibration /= 1000.0;
    mouse->gyroPosition = 0;
    proc->setLED(LED_LEFT_2, LED_OFF);

	/*proc->writeMemoryWithString("C,");
	proc->writeMemoryWithInt((int)(mouse->gyroCalibration*1000));
	proc->writeMemoryWithString("\n");*/
}

#define ONE_DEG_CLOCKWISE 53900.0
#define GYRO_CUTOFF 0.0025

void MouseUpdate()
{
	static float previousDegreesValue = 0;

    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    mouse->time++;

    // Get and calculate encoder based data.
    int leftEncoderPos = proc->getSensor(SENSOR_ENCODER_LEFT);
    int rightEncoderPos = proc->getSensor(SENSOR_ENCODER_RIGHT);

    mouse->encoderVelocityLeft =
    		leftEncoderPos - mouse->previousEncoderValueLeft;

    mouse->encoderVelocityRight =
    		rightEncoderPos - mouse->previousEncoderValueRight;

    mouse->previousEncoderValueLeft = leftEncoderPos;
    mouse->previousEncoderValueRight = rightEncoderPos;

    // Get and calculate gyroscope based data.
    float changeindegrees = 
        (proc->getSensor(SENSOR_GYRO) - mouse->gyroCalibration) / ONE_DEG_CLOCKWISE;

    if (mouse->gyroCalibration != 0)
    {
		/*proc->writeMemoryWithString("T,");
		proc->writeMemoryWithInt(proc->getSensor(SENSOR_GYRO));
		proc->writeMemoryWithString("\n");*/

    	/*proc->writeMemoryWithString("E,");
    	proc->writeMemoryWithInt(changeindegrees*1000);
    	proc->writeMemoryWithString("G,");
    	proc->writeMemoryWithInt(mouse->gyroPosition*1000);
     	proc->writeMemoryWithString("\n");*/

		mouse->gyroPosition +=
			(changeindegrees > GYRO_CUTOFF || changeindegrees < -GYRO_CUTOFF)
				? changeindegrees : 0;
    }
    else
    {
    	mouse->gyroPosition = 0;
    }

    /*proc->serialSendString("G,");
    proc->serialSendDouble(mouse->gyroPosition);
    proc->serialSendString("\n");*/

    mouse->gyroVelocity = mouse->gyroPosition - previousDegreesValue;
    previousDegreesValue = mouse->gyroPosition;

    // Execute some sort of motion.
    if (mouse->motionType != 0)
    {
		proc->setLED(LED_RIGHT_1, LED_ON);
    	mouse->motionType();
    }
    // If there is currently no motionType being executed, set motors to off.
    else
    {
		proc->setLED(LED_RIGHT_1, LED_OFF);
        proc->setMotor(LEFT_MOTOR, 0);
        proc->setMotor(RIGHT_MOTOR, 0);
    }

    // Handle Buzzer Tones
    BuzzerTone* tone = BuzzerBufferGetCurrent(&mouse->buzzer);
   
    if (tone != 0)
    {
        if (mouse->buzzer.msCounter == 0)
        {
            proc->setBuzzerFrequency(tone->frequency);       
        }  
    
        mouse->buzzer.msCounter++;
       
        if (tone->timeMs <= mouse->buzzer.msCounter)
        {
            mouse->buzzer.msCounter = 0;
            BuzzerBufferDeleteCurrent(&mouse->buzzer);
        }
    }
    else
    {
        proc->setBuzzerFrequency(0);
    }
}
