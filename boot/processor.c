#include <stdlib.h>

#include "processor.h"
#include "setupGPIO.h"
#include "setupScreen.h"
#include "setupSensors.h"
#include "setupMotors.h"
#include "setupFlash.h"
#include "setupSerial.h"
#include "setupSerialOperations.h"
#include "SensorDefinitions.h"

void ProcessorSetLed(int, int);
void ProcessorSetMotor(int, int);
void ProcessorSetScreenWithInt(int);
void ProcessorSetScreenWithString(const char*);
void ProcessorSetScreenWithChar(char, int);

void ProcessorSetupGPIO();
void ProcessorSetupSerial(int);
void ProcessorSetupMotors();
void ProcessorSetupSensors();
void ProcessorSetRXCallback(callbackFunction func);

void ProcessorResetSensor(int);
int ProcessorGetSensor(int);

void ProcessorWriteMemoryWithString(char*);
void ProcessorWriteMemoryWithChar(char);
void ProcessorWriteMemoryWithInt(int);
void ProcessorEraseMemory();

void ProcessorSendSerialString(const char*);
void ProcessorSendSerialRawInt(int);
void ProcessorSendSerialInt(int);
void ProcessorSendSerialDouble(double);
void ProcessorSetBuzzerFrequency(int);
void ProcessorSendSerialChar(char value);

Processor* SingletonProcessor()
{
    static Processor* processor;

    if (processor == 0)
    {
        processor = malloc(sizeof(Processor));
        processor->setupGPIO = ProcessorSetupGPIO;
        processor->setupSerial = ProcessorSetupSerial;
        processor->setupMotors = ProcessorSetupMotors;
        processor->setupSensors = ProcessorSetupSensors;

        processor->setMotor = ProcessorSetMotor;
        processor->setLED = ProcessorSetLed;

        processor->setScreenWithInt = ProcessorSetScreenWithInt;
        processor->setScreenWithChar = ProcessorSetScreenWithChar;
        processor->setScreenWithString = ProcessorSetScreenWithString;

        processor->getSensor = ProcessorGetSensor;
        processor->resetSensor = ProcessorResetSensor;

        processor->writeMemoryWithString = ProcessorWriteMemoryWithString;
        processor->writeMemoryWithChar = ProcessorWriteMemoryWithChar;
        processor->writeMemoryWithInt = ProcessorWriteMemoryWithInt;
        processor->eraseMemory = ProcessorEraseMemory;
        processor->serialSendString = ProcessorSendSerialString;
        processor->serialSendRawInt = ProcessorSendSerialRawInt;
        processor->serialSendInt = ProcessorSendSerialInt;
        processor->serialSendDouble = ProcessorSendSerialDouble;
        processor->serialSendChar = ProcessorSendSerialChar;
        processor->serialSetRXCallback = ProcessorSetRXCallback;

        processor->setBuzzerFrequency = ProcessorSetBuzzerFrequency;
    }

    return processor;
}

void ProcessorSetRXCallback(callbackFunction func)
{
	SetCallBackSerialFunction(func);
}

void ProcessorSetupSensors()
{
    Config_SensorInit();
}

void ProcessorWriteMemoryWithString(char* s)
{
    Flash_SaveString(s);
}

void ProcessorWriteMemoryWithChar(char c)
{
    Flash_SaveByte(c);
}

void ProcessorEraseMemory()
{
	Flash_EraseDataBuffer();
}

void ProcessorWriteMemoryWithInt(int num)
{
    int tempNum = num;
    int lc = 0;

    if (num < 0)
    {
        Flash_SaveByte('-');
        num = -num;
        tempNum = num;
    }
        
    while (1==1)
    {
        lc++;
        if (tempNum / 10 <= 0) break;
        tempNum /= 10;
    }
    
    int i;
    for (i = 0; i < lc; i++)
    {
        int pow = 1;
        int j;
        for (j = i; j < lc-1; j++)
            pow *= 10;
        
        int result = (num/pow % 10);
        Flash_SaveByte('0' + result);
    }
}

void ProcessorSetMotor(int motor, int value)
{   
    value = -value; // Flip so positive values back the mouse go forwards.
    if (value > 1000) value = 1000;
    if (value < -1000) value = -1000;

    if ((motor&LEFT_MOTOR) == LEFT_MOTOR)
    {
        Config_SetLeftPWM(value);
    }
    if ((motor&RIGHT_MOTOR) == RIGHT_MOTOR)
    {
        Config_SetRightPWM(value);   
    }
}

void ProcessorSetupGPIO()
{
    SetupGPIO(); 

    ProcessorSetLed(LED_ALL, LED_OFF);
   
    SPIConfig();
    HCMS_LoadControl(0b10000001); 
    HCMS_LoadControl(0b01111111);
    HCMS_LoadDot();

    // Turn on the update loop and make it the highest priority.
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    NVIC_SetPriority(SysTick_IRQn, 1);
	Flash_ResetFlashAddress();
}

void ProcessorSetupSerial(int baud)
{
    Config_SerialInit(baud);
    Config_SerialSaveString("Micromouse Callisto\n");
}

void ProcessorSetupMotors()
{
    Config_MotorsInit();
    ProcessorSetMotor(LEFT_MOTOR, 0);
    ProcessorSetMotor(RIGHT_MOTOR, 0);
}

void __setLedValue(GPIO_TypeDef* reg, int pin, int value)
{
    if (value == LED_ON)
        GPIO_SetBits(reg, pin);
    else if (value == LED_OFF)
        GPIO_ResetBits(reg, pin);
    else if (value == LED_TOGGLE)
        GPIO_ToggleBits(reg, pin);
}

void ProcessorSetLed(int led, int value)
{
    if ( (led&LED_LEFT_1) == LED_LEFT_1)
        __setLedValue(GPIOC, GPIO_Pin_4, value);

    if( (led&LED_LEFT_2) == LED_LEFT_2)
        __setLedValue(GPIOA, GPIO_Pin_7, value);

    if( (led&LED_LEFT_3) == LED_LEFT_3)
        __setLedValue(GPIOA, GPIO_Pin_6, value); 

    if( (led&LED_RIGHT_1) == LED_RIGHT_1)
        __setLedValue(GPIOC, GPIO_Pin_12, value);
    
    if( (led&LED_RIGHT_2) == LED_RIGHT_2)
        __setLedValue(GPIOH, GPIO_Pin_1, value);
        
    if( (led&LED_RIGHT_3) == LED_RIGHT_3)
        __setLedValue(GPIOH, GPIO_Pin_0, value);
    
    if( (led&LED_CENTER_1) == LED_CENTER_1)
        __setLedValue(GPIOC, GPIO_Pin_0, value);

    if( (led&LED_CENTER_2) == LED_CENTER_2)
        __setLedValue(GPIOC, GPIO_Pin_13, value);
    
    if( (led&LED_CENTER_3) == LED_CENTER_3)
        __setLedValue(GPIOC, GPIO_Pin_14, value);
}

void ProcessorSetScreenWithInt(int num)
{
    HCMS_SetNumber(num);
    HCMS_LoadDot(); 
}

void ProcessorSetScreenWithString(const char* msg)
{
    HCMS_SetMessage(msg);
    HCMS_LoadDot();
}

void ProcessorSetScreenWithChar(char character, int index)
{   
    HCMS_SetCharacter(character, index);
    HCMS_LoadDot();
}

int ProcessorGetSensor(int sensor)
{
    switch(sensor)
    {
        case SENSOR_LEFT_1:
            return Config_SensorGetValue(IR_FRONT_LEFT);
            break;

        case SENSOR_LEFT_2:
            return Config_SensorGetValue(IR_SIDE_LEFT_2);
            break;

        case SENSOR_LEFT_3:
            return Config_SensorGetValue(IR_SIDE_LEFT);
            break;

        case SENSOR_RIGHT_1:
            return Config_SensorGetValue(IR_FRONT_RIGHT);
            break;

        case SENSOR_RIGHT_2:
            return Config_SensorGetValue(IR_SIDE_RIGHT_2);
            break;

        case SENSOR_RIGHT_3:
            return Config_SensorGetValue(IR_SIDE_RIGHT);
            break;

        case SENSOR_GYRO:
            return Config_SensorGetValue(GYRO);
            break;

        case SENSOR_ENCODER_LEFT:
            return Config_GetLeftEncoder();
            break;

        case SENSOR_ENCODER_RIGHT:
            return Config_GetRightEncoder();
            break;

        case SENSOR_BUTTON_1:
            return Config_isButtonPushed();
            break;
        
        default:
            return 0;
            break;
    }
}

void ProcessorResetSensor(int sensor)
{
    switch(sensor)
    {
        case SENSOR_ENCODER_LEFT:
            Config_ResetLeftEncoder();
            break;

        case SENSOR_ENCODER_RIGHT:
            Config_ResetRightEncoder();
            break;
    }
}

void ProcessorSendSerialString(const char* msg)
{
    Config_SerialSaveString(msg);
}

void ProcessorSendSerialRawInt(int number)
{
    Config_SerialSaveRawInteger(number);
}

void ProcessorSendSerialInt(int number)
{
    Config_SerialSaveIntegerAsString(number);
}

void ProcessorSendSerialChar(char value)
{
	Config_SerialSaveRawChar(value);
}

void ProcessorSendSerialDouble(double number)
{
    Config_SerialSaveDoubleAsString(number);
}

void ProcessorSetBuzzerFrequency(int freq)
{
    SetBuzzerFrequency(freq);
}

void timing_handler()
{
    Processor* proc = SingletonProcessor();

    if (proc->callback != 0)
        proc->callback();
}

void _init()
{
    
}
