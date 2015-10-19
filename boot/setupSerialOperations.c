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
#include "setupSerialOperations.h"

void Config_SerialSaveDoubleAsString(double num)
{
	if(num < 0)
	{
		Config_SerialSaveRawChar('-');
		num = -num;
	}

	// Split the number into integer and decimal number
	int intNum = (int)num;
	double decNum = num - intNum;

	Config_SerialSaveIntegerAsString(intNum);
	Config_SerialSaveRawChar('.');

	int i;
	for(i = 0; i < 7; i++)
	{
		decNum *= 10;
		int tempNum = (int)decNum;
		decNum -= tempNum;
		Config_SerialSaveRawChar('0' + tempNum);
	}
}

// Take an integer and convert it to a string
void Config_SerialSaveIntegerAsString(int num)
{
	int tempNum = num;
	int lc = 0;

	if(num < 0)
	{
		Config_SerialSaveRawChar('-');
		num = -num;
		tempNum = num;
	}

	while(1==1)
	{
		lc++;
		if(tempNum / 10 <= 0)
			break;
		tempNum /= 10;
	}

	int i;
	for(i = 0; i < lc; i++)
	{
		int pow = 1;
		int j;
		for(j = i; j < lc-1; j++)
			pow*=10;

		int result = (num/pow % 10);
		Config_SerialSaveRawChar('0' + result);
	}
}

// Save an arbitrary number to editingBuffer
// This will only show important bytes!
// ie, 5 (0101), will be sent as 00000101
// instead of 00000000 00000000 00000000 00000101
void Config_SerialSaveNumber(long data)
{
	int sentFlag = 0;

	int i;
	for(i = sizeof(data)-1; i >= 0; i--)
	{
		// Get the byte i of the data
		char num = (data >> (i*8) ) & 0xFF;

		// If number isn't 0,
		//  or we've saved some data before,
		//  or we're on the last byte
		if( (num != 0 || sentFlag)
			|| (i == 0))
		{
			sentFlag = 1;
			Config_SerialSaveRawChar(num);
		}
	}
}

// Saves 4 bytes to editingBuffer
void Config_SerialSaveRawInteger(int data)
{
	int i;
	for(i = 3; i >= 0; i--)
	{
		Config_SerialSaveRawChar((data >> (i*8) ) & 0xFF);
	}
}

// non-DMA way of doing USART
void USART_puts(volatile char s)
{
	// wait until data register is empty
	while( !(USART1->SR & 0x00000040) );
	USART_SendData(USART1, s);
}

// Save a string to editingBuffer
void Config_SerialSaveString(const char* message)
{
	while(*message)
	{
		Config_SerialSaveRawChar(*message);
		message++;
	}
}

