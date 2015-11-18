#include "setupSensors.h"
#include "setupSerialOperations.h"

// Functions that shouldn't be used outside of this file
static void SensorTIM();
static void SensorRCC();
static void SensorGPIO();
static void SensorNVIC();
static void SensorADC();

// Stores information about each sensor
typedef struct 
{
	int32_t data;
	int32_t ambientData;
	int32_t adjData;// Data that has ambient light removed from it
	uint8_t channel;
	GPIO_TypeDef* port;
	uint16_t pin;
}Sensor;

// Variable for all the sensors
volatile Sensor Sensors[10];

void Config_SensorInit()
{
	int i;
	for(i = 0; i < 5; i++)
		Sensors[i].data = 0;

	// Set all Defaults for the sensor
	Sensors[IR_FRONT_LEFT].channel = ADC_Channel_5;
	Sensors[IR_FRONT_LEFT].port = GPIOB;
	Sensors[IR_FRONT_LEFT].pin = GPIO_Pin_4;

	Sensors[IR_FRONT_RIGHT].channel = ADC_Channel_4;
	Sensors[IR_FRONT_RIGHT].port = GPIOD;
	Sensors[IR_FRONT_RIGHT].pin = GPIO_Pin_2;

	Sensors[IR_SIDE_LEFT].channel = ADC_Channel_12;
	Sensors[IR_SIDE_LEFT].port = GPIOB;
	Sensors[IR_SIDE_LEFT].pin = GPIO_Pin_6;

	Sensors[IR_SIDE_RIGHT].channel = ADC_Channel_2;
	Sensors[IR_SIDE_RIGHT].port = GPIOB;
	Sensors[IR_SIDE_RIGHT].pin = GPIO_Pin_8;

	Sensors[IR_SIDE_LEFT_2].channel = ADC_Channel_11;
	Sensors[IR_SIDE_LEFT_2].port = GPIOB;
	Sensors[IR_SIDE_LEFT_2].pin = GPIO_Pin_5;

	Sensors[IR_SIDE_RIGHT_2].channel = ADC_Channel_9;
	Sensors[IR_SIDE_RIGHT_2].port = GPIOB;
	Sensors[IR_SIDE_RIGHT_2].pin = GPIO_Pin_3;

	Sensors[GYRO].channel = ADC_Channel_8;
	Sensors[GYRO].port = 0;
	Sensors[GYRO].pin = 0;

	SensorRCC();
	SensorGPIO();
	SensorADC();
	SensorTIM();
	SensorNVIC();
}

// Return the value of each sensor
int32_t Config_SensorGetValue(int32_t sensor)
{
	return Sensors[sensor].data;
}

int32_t Config_SensorGetAmbientValue(int32_t sensor)
{
	return Sensors[sensor].ambientData;
}

int32_t Config_SensorGetAdjValue(int32_t sensor)
{
	return Sensors[sensor].adjData;
}

static void SensorTIM()
{
	// Set the Sensor Timers
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_OCInitTypeDef TIM_OC;

	// Prescalar will determine the speed of the timer counter
	// For instance, 84Mhz / 84 = 1Mhz (or 1us per count)
	TIM_Struct.TIM_Prescaler = 84 - 1;
	
	// Count from zero upwards
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;

	// Auto reload value, this will be when the counter resets back to zero
	TIM_Struct.TIM_Period = 166 - 1;

	// Initate timer
	TIM_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Struct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_Struct);

	// Have an interrupt thrown when the counter = 0us (the start)
	TIM_OC.TIM_OCMode = TIM_OCMode_Inactive;
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC.TIM_Pulse = 0;
	TIM_OC1Init(TIM4, &TIM_OC);	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);

	// Have an interrupt thrown when the counter = 80us
	TIM_OC.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC.TIM_Pulse = 70;
	TIM_OC2Init(TIM4, &TIM_OC);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);

	// Turn on Interrupts
	TIM_ITConfig(TIM4, TIM_IT_CC1 | TIM_IT_CC2, ENABLE);

	// Start Timer
	TIM_Cmd(TIM4, ENABLE);
}

static void SensorRCC()
{
	// Turn on perhiperhal clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
}

static void SensorGPIO()
{
	GPIO_InitTypeDef GPIO_Struct;

	// Sensor Emitter Pins
	GPIO_Struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Struct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_Struct);

	// Sensor Receiver pins
	GPIO_Struct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_Struct);

	GPIO_Struct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Struct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_Struct);
}

static void SensorADC()
{
	// Start the ADC
	ADC_InitTypeDef ADC_Struct;
	ADC_DeInit();

	// Set the ADC to do a single conversion
	ADC_Struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Struct.ADC_Resolution = ADC_Resolution_12b;
	ADC_Struct.ADC_ContinuousConvMode = DISABLE;
	ADC_Struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_Struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_Struct.ADC_NbrOfConversion = 1;
	ADC_Struct.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_Struct);

	ADC_Cmd(ADC1, ENABLE);	
}


volatile int _previousData[32];
volatile int _previousDataCounter;

static void SensorNVIC()
{
	// Turn on the TIM4 and ADC Interrupts
	// This allows us to use TIM4_IRQHandler and ADC_IRQHandler
	NVIC_InitTypeDef NVIC_Struct;
	NVIC_Struct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_Struct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Struct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Struct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Struct);

	NVIC_Struct.NVIC_IRQChannel = ADC_IRQn;
	NVIC_Init(&NVIC_Struct);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	_previousDataCounter = 0;
}

volatile int counter = 0;
volatile int ambient = 0;
volatile int lc = 0;


// TIM4_IRQHandler()
// This is an interrupt that handles all timer events
// This timer is setup to turn on a Sensor pin on CC1, 
//   then turn on an ADC conversion on CC2.  
//   The End-Of-Conversion interrupt will turn off the 
//   sensor pin
void TIM4_IRQHandler()
{
	// On startup of the timer
	if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);

		if (Sensors[counter].port != 0)
		{
			GPIO_WriteBit(Sensors[counter].port, Sensors[counter].pin, Bit_SET);
		}
	}

	// CC2 : After 80us pass for the sensor to turn on correctly, do a ADC conversion
	else if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);

		// Determine which pin to take the ADC conversion from
		ADC_RegularChannelConfig(ADC1, Sensors[counter].channel, 1, ADC_SampleTime_15Cycles);

		// Start the ADC
		ADC_SoftwareStartConv(ADC1);
	}
}

// ADC_IRQHandler
// This is an interrupt that handles all ADC events
// This is setup to have an event called when 
//   there is an End-Of-Conversion (EOC) event occur.
void ADC_IRQHandler()
{
	// TODO: Test Static variable
	//static int counter = 0;

	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);

		if (_previousDataCounter > 30)
		{
			// Turn off the sensor pin
			_previousData[_previousDataCounter] = ADC_GetConversionValue(ADC1);

			if(Sensors[counter].port != 0)
			{
				GPIO_WriteBit(Sensors[counter].port, Sensors[counter].pin, Bit_RESET);
			}

			int i;
			int sum = 0;
			for (i = 0; i < 32; i++)
			{
				sum += _previousData[i];
			}

			Sensors[counter].data = sum;

			/*if (counter == 0)
			{
				lc++;
				if (lc % 2 == 0)
				{
					Config_SerialSaveIntegerAsString(Sensors[counter].data);
					Config_SerialSaveRawChar('\n');
					Config_SerialSaveRawChar('\r');
				}
			}*/

			counter++;
			if (counter > 6)
				counter = 0;

			_previousDataCounter = 0;
		}
		else
		{
			_previousData[_previousDataCounter] = ADC_GetConversionValue(ADC1);
			_previousDataCounter++;
			ADC_SoftwareStartConv(ADC1);
		}
	}
}

