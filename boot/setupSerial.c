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
#include "setupSerial.h"

// Starts the DMA
static void startSerialDMA();

// Output Stuff
CircularBuffer serialOutput;

// Input Stuff
CircularBuffer serialInput;
char callBackString[256];
typedef void (*callbackFunction)(char* string);
callbackFunction returnStringFunction;

// The main entry point for the Serial initiation
void Config_SerialInit(int Baud)
{
	CircularBufferInit(&serialOutput);
	CircularBufferInit(&serialInput);
	returnStringFunction = 0;

	// Basic Initiation
	Config_SerialInitPeripherals(Baud);
}

// Save a char to editingBuffer
void Config_SerialSaveRawChar(char num)
{
	CircularBufferAdd(&serialOutput, num);
	//USART_TypeDef* USARTx = USART1;

	//while(!(USARTx->SR & 0x00000040) );
	//USART_SendData(USARTx, num);

	if(serialOutput.streaming == 0)
	{
		Config_SerialForceBuffer();
	}
}

// Forces the buffer to send anything in the buffer
//  even if the buffer isn't full!
void Config_SerialForceBuffer()
{
	if(serialOutput.streaming == 0)
	{
		serialOutput.streaming = 1;
		startSerialDMA();
	}
}


void Config_SerialInitPeripherals(int Baud)
{
	// Turns on Peripheral Clocks for GPIO and USART functions
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

	// Configure GPIO Pins
	GPIO_InitTypeDef GPIO_Struct;

	// Configures GPIOA pins 9 and 10 for Serial AF
	GPIO_Struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_Struct);

	// Setups Alternative Function
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	// Configure USART Peripheral
	USART_InitTypeDef USART_Struct;

	// Configure USART 1 settings
	USART_Struct.USART_BaudRate = Baud;
	USART_Struct.USART_WordLength = USART_WordLength_8b;
	USART_Struct.USART_StopBits = USART_StopBits_1;
	USART_Struct.USART_Parity = USART_Parity_No;
	USART_Struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Struct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_Struct);

	// Configure the Recieving Interrupt
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	// Enables USART1
	USART_Cmd(USART1, ENABLE);

	// Configure NVIC
	NVIC_InitTypeDef NVIC_Struct;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	NVIC_Struct.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_Struct);

	// Configure DMA for USART
	DMA_DeInit(DMA2_Stream7);

	DMA_Struct.DMA_Channel = DMA_Channel_4;
	DMA_Struct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//DMA_Struct.DMA_Memory0BaseAddr = (uint32_t)bufferAddr;
	DMA_Struct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	//DMA_Struct.DMA_BufferSize = BUFFER_STRING_LENGTH - 1;	
	DMA_Struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Struct.DMA_MemoryInc = DMA_MemoryInc_Enable;	
	DMA_Struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_Struct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_Struct.DMA_Mode = DMA_Mode_Normal;
	DMA_Struct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_Struct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_Struct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Struct.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_Struct.DMA_Priority = DMA_Priority_Low;

	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
}

// Starts the DMA process on our processor
static void startSerialDMA()
{
	int length = CircularBufferGetMemoryLength(&serialOutput);

	if(length > 0)
	{
		char* pointer = CircularBufferGetMemoryStartPointer(&serialOutput);
		CircularBufferIncrementBy(&serialOutput, length);

		DMA_DeInit(DMA2_Stream7);

		// Only transwer the number of bytes listed by dataLength
		DMA_Struct.DMA_BufferSize = length;//BufferList[0]->dataLength;

		// Set the pointer to the buffer to BufferList[0]
		DMA_Struct.DMA_Memory0BaseAddr = (uint32_t)pointer;//(uint32_t)BufferList[0];

		DMA_Init(DMA2_Stream7, &DMA_Struct);

		DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);

		DMA_Cmd(DMA2_Stream7, ENABLE);
	}
}


// DMA Completion Interrupt
// This is called when the DMA has completed transmitting it's current message
void DMA2_Stream7_IRQHandler(void)
{
	// DMA 2 Stream 7 Completion 
	if (DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7))
	{		
		if(CircularBufferSize(&serialOutput) > 0)
		{
			serialOutput.streaming = 1;
			startSerialDMA();
		}
		else
		{
			serialOutput.streaming = 0;
		}

		// Clear DMA Stream Transfer Complete interrupt pending bit 
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
	}
}


void SetCallBackSerialFunction(callbackFunction func)
{
	returnStringFunction = func;
}

// Receving function (not used atm)
void USART1_IRQHandler(void)
{
	// If you try to debug here, you may lose data that comes
	// in while your debugging FYI
	if( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET )
	{
		// Add character to Circular Buffer
		char t = USART_ReceiveData(USART1);
		CircularBufferAdd(&serialInput, t);

		// Add all data to string
		if(t == '\n')
		{
			int counter = 0;
			while(!CircularBufferIsEmpty(&serialInput))
			{
				char r = CircularBufferGet(&serialInput);
				callBackString[counter] = r;
				counter++;
			}
			callBackString[counter] = 0;

			if(returnStringFunction != 0)
			{
				returnStringFunction(callBackString);
			}
		}

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

