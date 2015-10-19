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
#include "setupGPIO.h"

void SetupGPIO()
{
	// GPIOC and GPIOB clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_Struct;

	// All Led's
	GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOH, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOC, &GPIO_Struct);

	// PC10 is button
	GPIO_Struct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOC, &GPIO_Struct);

	// Buzzer is on TIM1
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOA, &GPIO_Struct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_OCInitTypeDef TIMOC_Struct;

	uint16_t period = 3000-1;
	uint16_t prescalerValue = 16-1;

	// Timer Init
	TIM_Struct.TIM_Period = period;
	TIM_Struct.TIM_Prescaler = prescalerValue;
	TIM_Struct.TIM_ClockDivision = 0;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_Struct);

	// PWM Mode Configuration
	TIMOC_Struct.TIM_OCMode = TIM_OCMode_PWM1;
	TIMOC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	TIMOC_Struct.TIM_Pulse = 0;
	TIMOC_Struct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC4Init(TIM1, &TIMOC_Struct);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM1, ENABLE);

	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	TIM_Cmd(TIM1, ENABLE);

	SetBuzzerFrequency(0);
}

int Config_isButtonPushed()
{
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);
}

void SetBuzzerFrequency(int frequency)
{
	// Timer Init
	uint16_t period = 3000-1;
	uint16_t prescalerValue = 16-1;

	if (frequency == 0)
	{
		TIM_TimeBaseInitTypeDef TIM_Struct;
		TIM_Struct.TIM_Period = period;
		TIM_Struct.TIM_Prescaler = prescalerValue;
		TIM_Struct.TIM_ClockDivision = 0;
		TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM1, &TIM_Struct);

		TIM1->CCR4 = 3000;
	}
	else
	{
		double clockSpeed = 168000000;
		double a = clockSpeed / (double)(prescalerValue+1);
		period = (uint16_t)(a / (double)(frequency));

		TIM_TimeBaseInitTypeDef TIM_Struct;
		TIM_Struct.TIM_Period = period;
		TIM_Struct.TIM_Prescaler = prescalerValue;
		TIM_Struct.TIM_ClockDivision = 0;
		TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM1, &TIM_Struct);

		if (period < 600)
			TIM1->CCR4 = (uint16_t)period-1;
		else
			TIM1->CCR4 = (uint16_t)((double)period-600);
	}
}

