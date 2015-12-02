#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

typedef void (*callbackFunction)(char* string);

typedef struct Processor
{
    /**
     * Initialize parts of the mouse
     */
    void (*setupGPIO)();
    void (*setupSerial)(int);
    void (*setupMotors)(); 
    void (*setupSensors)(); 
    void (*setLED)(int, int);

    void (*setScreenWithInt)(int);
    void (*setScreenWithChar)(char, int);
    void (*setScreenWithString)(const char*);

    void (*serialSendString)(const char*);
    void (*serialSendRawInt)(int);
    void (*serialSendInt)(int);
    void (*serialSendDouble)(double);
    void (*serialSendChar)(char);
    void (*serialSetRXCallback)(callbackFunction func);
    void (*setBuzzerFrequency)(int);        
    int (*getSensor)(int);
    void (*resetSensor)(int);
    void (*setMotor)(int, int);

    void (*writeMemoryWithString)(char*);
    void (*writeMemoryWithChar)(char);
    void (*writeMemoryWithInt)(int);
    void (*eraseMemory)();

    void (*callback)();
} Processor;

Processor* SingletonProcessor();

#define LED_ON          0x001
#define LED_OFF         0x002
#define LED_TOGGLE      0x004

#define LEFT_MOTOR      0x001
#define RIGHT_MOTOR     0x002

#define LED_LEFT_1      0x001
#define LED_LEFT_2      0x002
#define LED_LEFT_3      0x004
#define LED_RIGHT_1     0x008
#define LED_RIGHT_2     0x010
#define LED_RIGHT_3     0x020
#define LED_CENTER_1    0x040
#define LED_CENTER_2    0x080
#define LED_CENTER_3    0x100
#define LED_ALL         0xFFFF

#define SENSOR_LEFT_1           0x01
#define SENSOR_LEFT_2           0x02
#define SENSOR_LEFT_3           0x03
#define SENSOR_RIGHT_1          0x04
#define SENSOR_RIGHT_2          0x05
#define SENSOR_RIGHT_3          0x06
#define SENSOR_GYRO             0x07
#define SENSOR_ENCODER_LEFT     0x08
#define SENSOR_ENCODER_RIGHT    0x09
#define SENSOR_BUTTON_1         0x10

#endif
