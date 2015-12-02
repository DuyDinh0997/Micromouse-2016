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

#ifndef __SETUP_FLASH_H__
#define __SETUP_FLASH_H__

void Flash_EraseDataBuffer();
void Flash_SaveByte(uint8_t data);
void Flash_SaveInt(int data);
void Flash_SaveWord(uint32_t data);
void Flash_SaveString(char* string);
void Flash_ResetFlashAddress();

#endif

