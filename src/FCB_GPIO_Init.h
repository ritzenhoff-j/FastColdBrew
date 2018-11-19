#ifndef STM_STD_LIB
#define STM_STD_LIB "stm32f4xx.h"
#include STM_STD_LIB
#endif


/**
 * In the GPIO_Init header.
 */
void initializeAll_IOPins();
void initializeGPIO(GPIO_TypeDef* GPIOx, uint16_t numberOfPins, PinLocation locations[],
		GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, GPIOOType_TypeDef type, GPIOPuPd_TypeDef pushPull);

void initializeTimerPort(GPIO_TypeDef* GPIOx, uint16_t numberOfPossiblePins, PWM_PinLocation locations[]);
