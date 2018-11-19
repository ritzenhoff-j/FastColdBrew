/*
 * FCB_PWM.c
 *
 *  Created on: Nov 15, 2018
 *      Author: ritze
 */

#ifndef STM_STD_LIB
#define STM_STD_LIB "stm32f4xx.h"
#include STM_STD_LIB
#endif

#ifndef FCB_GPIO_PINS
#define FCB_GPIO_PINS "FCB_GPIO_Pins.h"
#include FCB_GPIO_PINS
#endif

#ifndef FCB_PWM
#define FCB_PWM "FCB_PWM.h"
#include FCB_PWM
#endif


/*
 * The first channel of timers 1-4 are used
 * TIM_1 = 16 bit
 * TIM_2 = 32 bit
 * TIM_3 = 16 bit
 * TIM_4 = 16 bit
 */

/*
 * The 4 PWM controlled devices are:
 * VacuumPump: TIM_2
 * RecircPump: TIM_3
 * MixingMotor: TIM_1
 * PeltierCoolingFan: TIM_4
 */

void initializePWM() {
	for(uint8_t timerArrayIndex = 0; timerArrayIndex < NUMBER_OF_TIMERS; timerArrayIndex++) {
		uint8_t timerIndex = knownUsedTimers[timerArrayIndex];

		initializeTimerClock(timerIndex);

		initializeTimerPWM(timerIndex, NUMBER_OF_PWM_PINS, *pwmPins);

		timer_start(timerIndex);
	}
}


void timer_start(uint8_t timerIndex)
{
  TIM_Cmd (getTimer_Constant(timerIndex), ENABLE);
}

void timer_stop(uint8_t timerIndex)
{
  TIM_Cmd (getTimer_Constant(timerIndex), DISABLE);
}


void initializeTimerClock(uint8_t timerIndex) {
	uint16_t PWM_Steps = 100;
	uint16_t PWM_Frequency = 100;

	// optionally hardcoded at 8MHz
	uint32_t TIMER_Frequency = get_timer_clock_frequency(); // 8000000;

	uint32_t COUNTER_Frequency = PWM_Steps * PWM_Frequency;
	uint32_t PSC_Value = (TIMER_Frequency / COUNTER_Frequency) - 1;
	uint16_t ARR_Value = PWM_Steps - 1;

	/* make sure the peripheral is clocked */
	RCC_APB1PeriphClockCmd (getTimerPeripheral(timerIndex), ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	/* set everything back to default values */
	TIM_TimeBaseStructInit (&TIM_TimeBaseStructure);
	/* only changes from the defaults are needed */
	TIM_TimeBaseStructure.TIM_Period = ARR_Value;
	TIM_TimeBaseStructure.TIM_Prescaler = PSC_Value;

	TIM_TimeBaseInit (getTimer_Constant(timerIndex), &TIM_TimeBaseStructure);
}

/**
 * Initialize all relevant channels of a given timer using the given pin mappings.
 */
void initializeTimerPWM(uint8_t timerIndex, uint16_t numberOfPossiblePins, PWM_PinLocation locations[]) {
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* Common settings for all channels */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_TypeDef* correctTimer = getTimer_Constant(timerIndex);

	for(uint16_t i = 0; i < numberOfPossiblePins; i++) {
		PWM_PinLocation currentPWMIO = locations[i];

		if(currentPWMIO.timerIndex == timerIndex) {
			switch(currentPWMIO.channel) {
			case 1:
				TIM_OC1Init(correctTimer, &TIM_OCInitStructure);
				break;
			case 2:
				TIM_OC2Init(correctTimer, &TIM_OCInitStructure);
				break;
			case 3:
				TIM_OC3Init(correctTimer, &TIM_OCInitStructure);
				break;
			case 4:
				TIM_OC1Init(correctTimer, &TIM_OCInitStructure);
				break;
			}
		}
	}
}


/**
 * Sets the PWM value of a given timer and channel to the provided value.
 */
void setPWM(uint8_t timerIndex, uint8_t channel, uint32_t value) {
	TIM_TypeDef* correctTimer = getTimer_Constant(timerIndex);

	switch(channel) {
	case 1:
		TIM_SetCompare1(correctTimer, value);
		break;
	case 2:
		TIM_SetCompare2(correctTimer, value);
		break;
	case 3:
		TIM_SetCompare3(correctTimer, value);
		break;
	case 4:
		TIM_SetCompare4(correctTimer, value);
		break;
	}
}


uint32_t get_timer_clock_frequency (void)
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq (&RCC_Clocks);
  uint32_t multiplier;
  if (RCC_Clocks.PCLK1_Frequency == RCC_Clocks.SYSCLK_Frequency) {
    multiplier = 1;
  } else {
    multiplier = 2;
  }
  return multiplier * RCC_Clocks.PCLK1_Frequency;
}
