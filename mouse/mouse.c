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

Mouse* SingletonMouse()
{
    static Mouse* mouse;
    
    if (mouse == 0)
    {
        mouse = malloc(sizeof(Mouse));
        mouse->initiate = MouseInitiate;
        mouse->buzzer.msCounter = 0;
        mouse->buzzer.end = 0;
        mouse->buzzer.start = 0;
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
    proc->setScreenWithString("Menu"); 
    proc->setupSensors();

    PIDSetup(&mouse->tmppid, 0.007, 0.0, 0.002); 
    PIDSetup(&mouse->angpid, 0.0002, 0.0, 0.0);

    // Everytime the processor updates every 1ms, this function is called.
    proc->callback = MouseUpdate;
}

void MouseCalibrateGyro()
{
    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    proc->serialSendString("Calibrating Gyroscope...");

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
#define GYRO_CUTOFF 0.0020

void MouseUpdate()
{
    Processor* proc = SingletonProcessor();
    Mouse* mouse = SingletonMouse();

    mouse->time++;

    float changeindegrees = 
        (proc->getSensor(SENSOR_GYRO) - mouse->gyroCalibration) / ONE_DEG_CLOCKWISE;

    mouse->degrees += 
        (changeindegrees > GYRO_CUTOFF || changeindegrees < -GYRO_CUTOFF) 
            ? changeindegrees : 0;

    int leftSensor = proc->getSensor(SENSOR_LEFT_1);
    int rightSensor = proc->getSensor(SENSOR_RIGHT_1);

    float feedback = PIDUpdate(&mouse->tmppid, 1, 50000 - (leftSensor + rightSensor)/2); 
    float feedback2 = PIDUpdate(&mouse->tmppid, 1, leftSensor - rightSensor + 4000);

    if (feedback > 0)
    {
        proc->setLED(LED_LEFT_3, LED_ON);
        proc->setLED(LED_RIGHT_3, LED_OFF);
    }
    else
    { 
        proc->setLED(LED_RIGHT_3, LED_ON);
        proc->setLED(LED_LEFT_3, LED_OFF);
    }

    if (50000 - leftSensor > 40000)
    {
        proc->setMotor(LEFT_MOTOR, 0);
        proc->setMotor(RIGHT_MOTOR, 0);
    }
    else
    {
        proc->setMotor(LEFT_MOTOR, feedback - feedback2);
        proc->setMotor(RIGHT_MOTOR, feedback + feedback2); 
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
