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

    while(mouse->time - startTime < 200)
    {
        mouse->gyroCalibration += proc->getSensor(SENSOR_GYRO);
        lc++;

        while(mouse->time <= thisTime)
        {

        }

        thisTime++;
    }

    mouse->gyroCalibration /= 200.0;
    mouse->degrees = 0;
}

#define ONE_DEG_CLOCKWISE 53900.0
#define GYRO_CUTOFF 0.0025

void MouseUpdate()
{
	static int previousEncoderValueLeft = 0;
	static int previousEncoderValueRight = 0;

    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    mouse->time++;

    int leftEncoderPos = proc->getSensor(SENSOR_ENCODER_LEFT);
    int rightEncoderPos = proc->getSensor(SENSOR_ENCODER_RIGHT);

    mouse->encoderVelocityLeft =
    		leftEncoderPos - previousEncoderValueLeft;

    mouse->encoderVelocityRight =
    		rightEncoderPos - previousEncoderValueRight;

    previousEncoderValueLeft = leftEncoderPos;
    previousEncoderValueRight = rightEncoderPos;

    float changeindegrees = 
        (proc->getSensor(SENSOR_GYRO) - mouse->gyroCalibration) / ONE_DEG_CLOCKWISE;

    mouse->degrees += 
        (changeindegrees > GYRO_CUTOFF || changeindegrees < -GYRO_CUTOFF) 
            ? changeindegrees : 0;

    if (mouse->motionType != 0)
    {
		proc->setLED(LED_RIGHT_1, LED_ON);
    	mouse->motionType();
    }
    else
    {
		proc->setLED(LED_RIGHT_1, LED_OFF);
        proc->setMotor(LEFT_MOTOR, 0);
        proc->setMotor(RIGHT_MOTOR, 0);
    }

    static int counter = 0;

    if (++counter % 20 == 0)
    {
		proc->serialSendString("G,");
		proc->serialSendInt(proc->getSensor(SENSOR_GYRO));
		proc->serialSendString(",C,");
		proc->serialSendDouble((double)mouse->gyroCalibration);
		proc->serialSendString(",D,");
		proc->serialSendDouble(mouse->degrees);
		proc->serialSendString("\n");
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
