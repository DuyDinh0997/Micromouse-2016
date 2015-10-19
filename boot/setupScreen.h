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

#ifndef __SETUP_SCREEN_H__
#define __SETUP_SCREEN_H__

#include "stm32f4xx_spi.h"
#include "stm32f4xx_conf.h"

void SPIConfig();
void mySPI_Init(void);
void HCMS_LoadControl(uint8_t data);
void HCMS_LoadDot();
void HCMS_WriteLetter(int letter, int display, int horiz_offset, int vert_offset);
void HCMS_SetNumber(int number);
void HCMS_SetMessage(const char* message);
void HCMS_SetCharacter(char chacter, int index);

#endif

