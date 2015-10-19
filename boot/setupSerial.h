#ifndef __SETUP_SERIAL_H
#define __SETUP_SERIAL_H

#include "stm32f4xx_conf.h"
#include "setupCircularBuffer.h"

// Serial___(...)
// Initiates the Serial peripherals
void Config_SerialInitPeripherals(int Baud);

// Holds the DMA information when wanting to send a message
DMA_InitTypeDef DMA_Struct;

// Holds the information that needs to be sent
#define BUFFER_STRING_LENGTH 	50
#define BUFFER_SIZE				100
#define BUFFER_EMPTY 			-1

// SerialInitiate
// This will setup a USART stream on our device
void Config_SerialInit(int Baud);

// SerialSaveRawChar(...)
// Adds a char to the current buffer for sending data
void Config_SerialSaveRawChar(char data);

// SerialForceBuffer(...)
// Forces the buffer to send it's content
void Config_SerialForceBuffer();

typedef void (*callbackFunction)(char* string);
void SetCallBackSerialFunction(callbackFunction func);

#endif

