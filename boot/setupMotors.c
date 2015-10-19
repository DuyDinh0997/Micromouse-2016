#include "setupMotors.h"

static void MotorGPIO();
static void MotorTimer();
static void MotorRCC();
static void MotorEncoderTimer();

void Config_MotorsInit()
{
	MotorRCC();
	MotorGPIO();
	MotorTimer();
	MotorEncoderTimer();
}

static void MotorRCC()
{
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

 	/* GPIOC and GPIOB clock enable */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |  RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
}

static void MotorGPIO()
{
	GPIO_InitTypeDef GPIO_Struct;

	GPIO_StructInit(&GPIO_Struct);

	// Sets GPIO pins for PWM or Encoder Input (both Alternate Function (AF) )
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_Struct);

	/*GPIO_Struct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_9;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_Struct);*/

	// Motor PWM
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM3);

	// Encoder
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
}

static void MotorTimer()
{
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_OCInitTypeDef TIMOC_Struct;

	uint16_t period = 1000;
	uint16_t prescalerValue = 0;
	prescalerValue = 3-1;

	// Timer Init
	TIM_Struct.TIM_Period = period;
	TIM_Struct.TIM_Prescaler = prescalerValue;
	TIM_Struct.TIM_ClockDivision = 0;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_Struct);

	// PWM Mode Configuration
	TIMOC_Struct.TIM_OCMode = TIM_OCMode_PWM1;
	TIMOC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	TIMOC_Struct.TIM_Pulse = 0;
	TIMOC_Struct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM3, &TIMOC_Struct);

	TIMOC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	TIMOC_Struct.TIM_Pulse = 0;
	TIM_OC2Init(TIM3, &TIMOC_Struct);

	TIMOC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	TIMOC_Struct.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIMOC_Struct);

	TIMOC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	TIMOC_Struct.TIM_Pulse = 0;
	TIM_OC4Init(TIM3, &TIMOC_Struct);

	TIM_Cmd(TIM3, ENABLE);
}

int32_t Config_GetRightEncoder()
{
	static long encPos = 0;
	encPos += (TIM2->CNT - ENC_DEFAULT_VALUE);
	TIM2->CNT = ENC_DEFAULT_VALUE;

	return encPos;
}
void Config_ResetRightEncoder()
{
	TIM2->CNT = ENC_DEFAULT_VALUE;
}
int32_t Config_GetLeftEncoder()
{
	static long encPos = 0;
	encPos += (TIM5->CNT - ENC_DEFAULT_VALUE);
	TIM5->CNT = ENC_DEFAULT_VALUE;

	return encPos;
}
void Config_ResetLeftEncoder()
{
	TIM5->CNT = ENC_DEFAULT_VALUE;
}

// Sets left motor from 0 to 1000
void Config_SetLeftPWM(int32_t value)
{
	if(value < 0)
	{
		TIM3->CCR1 = -value;
		TIM3->CCR2 = 0;
	}
	else
	{
		TIM3->CCR1 = 0;
		TIM3->CCR2 = value;
	}
}

// Sets left motor from 0 to 1000
void Config_SetRightPWM(int32_t value)
{
	if(value < 0)
	{
		TIM3->CCR3 = -value;
		TIM3->CCR4 = 0;
	}
	else
	{
		TIM3->CCR3 = 0;
		TIM3->CCR4 = value;
	}
}

static void MotorEncoderTimer()
{
	// Setup TIM2 (PA15 & PB3) (TIM5 is PA0 and PA1) as an encoder
	// By switching one of the TIM_ICPolarity_Rising, to TIM_ICPolarity_Falling, 
	//   the encoders will count backwards
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

	// Default Value to assist with negative values
	Config_ResetRightEncoder();
	Config_ResetLeftEncoder();

	// Start timer
	TIM_Cmd(TIM5, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

