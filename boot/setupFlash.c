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
#include "stm32f4xx_flash.h"

#include "setupFlash.h"

uint32_t _flash_address;

void Flash_ResetFlashAddress()
{
	_flash_address = 0x08080000;
}

void Flash_EraseDataBuffer()
{
	FLASH_Unlock();
	FLASH_ClearFlag( FLASH_FLAG_EOP |  FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);

	// Erase Sectors
	// 8-10 is data logger
	FLASH_EraseSector(FLASH_Sector_8, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3);

	Flash_ResetFlashAddress();

	FLASH_Lock();
}

void Flash_SaveString(char* string)
{
	FLASH_Unlock();
	while(*string)
	{
		FLASH_ProgramByte(_flash_address, *string);
		++_flash_address;
		++string;
	}
	FLASH_Lock();
}

void Flash_SaveByte(uint8_t data)
{
	// This takes 4us to execute at 168Mhz
	FLASH_Unlock();
	FLASH_ProgramByte(_flash_address, data);
	++_flash_address;
	FLASH_Lock();
}

void Flash_SaveWord(uint32_t data)
{
	// This takes 4.5us to execute at 168Mhz
	FLASH_Unlock();
	FLASH_ProgramWord(_flash_address, data);
	_flash_address += 4;
	FLASH_Lock();
}

