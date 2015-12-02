// Only compile this file if we're running 
//  a fake micromouse
#ifdef FAKE_MICROMOUSE

#include "processor.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "mouse.h"

typedef void (*callbackFunction)(char* string);

int leftMotor, rightMotor;
int buzzerFreq;
pthread_t mainThread;
pthread_mutex_t threadId;
char screen[4];

void PrintMouseStatus()
{
    Mouse* mouse = SingletonMouse();
    printf("Time = %ld\n", mouse->time);
    printf("Buzzer Freq = %d\n", buzzerFreq);
    printf("Screen = %s\n", screen);
}

void* MainUpdateLoop(void* ID)
{
    Processor* proc = SingletonProcessor();

    while (1==1)
    {
        int num;
        scanf("%d", &num);        
        
        int i;
        for (i = 0; i < num; i++)
            proc->callback();

        PrintMouseStatus(); 
    }
    return NULL; 
}

void ProcessorSetupGPIO()
{
    pthread_mutex_init(&threadId, NULL);
    // Spawn a new thread that handles incrementing sensors and all that
    pthread_create(&mainThread, NULL, &MainUpdateLoop, &threadId);
} 

void DoNothing() {}

void ProcessorSetMotor(int left, int right) 
{
    leftMotor = left;
    rightMotor = right;
}

void ProcessorSetLed(int led, int status)
{
    // Do something with leds
}

void setBuzzerFrequency(int freq){buzzerFreq = freq;}

void serialSendString(const char* c) {printf("%s", c);}
// TODO: Make this print in binary not int
void serialSendRawInt(int c) {printf("%d", c);}
void serialSendInt(int c) {printf("%d", c);}
void serialSendDouble(double d) {printf("%lf", d);}
void serialSendChar(char c) {printf("%c", c);}

void serialSetRxCallback(callbackFunction func)
{   
    // TODO: DO something here
}

void setScreenWithInt(int c){}
void setScreenWithChar(char c, int l){}
void setScreenWithString(const char* c){} 
void writeMemoryWithString(char* c){}
void writeMemoryWithChar(char c){}
void writeMemoryWithInt(int c){}
int getSensor(int sensor){return 0;}
void resetSensor(int sensor){}

Processor* SingletonProcessor()
{
    static Processor* processor;
    
    if (processor == 0)
    {
        processor = malloc(sizeof(Processor));

        processor->setupGPIO = ProcessorSetupGPIO;
        processor->setupSerial = DoNothing;
        processor->setupMotors = DoNothing;
        processor->setupSensors = DoNothing;
        processor->setLED = ProcessorSetLed; 

        leftMotor = 0;
        rightMotor = 0;
        processor->setMotor = ProcessorSetMotor; 

        processor->serialSendString = serialSendString;
        processor->serialSendInt = serialSendInt;
        processor->serialSendRawInt = serialSendRawInt;
        processor->serialSendDouble = serialSendDouble;
        processor->serialSetRXCallback = 
            serialSetRxCallback;

        processor->getSensor = getSensor;
        processor->resetSensor = resetSensor;
        
        processor->setScreenWithInt = setScreenWithInt;
        processor->setScreenWithChar = setScreenWithChar;
        processor->setScreenWithString = setScreenWithString;
        processor->writeMemoryWithString = writeMemoryWithString;
        processor->writeMemoryWithChar = writeMemoryWithChar;
        processor->writeMemoryWithInt = writeMemoryWithInt;
        buzzerFreq = 0;
        processor->setBuzzerFrequency = setBuzzerFrequency;
    }

    return processor;
}

#endif
