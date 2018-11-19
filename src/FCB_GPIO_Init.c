/*
 * FCB_GPIO_Init.c
 *
 *  Created on: Nov 3, 2018
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

#ifndef FCB_GPIO_INIT
#define FCB_GPIO_INIT "FCB_GPIO_Init.h"
#include FCB_GPIO_INIT
#endif


/**
 * An initialization of all Pins based on PinLocation input and output arrays.
 */
void initializeAll_IOPins() {
	for(uint8_t portIndex = 0; portIndex < NUMBER_OF_PORTS; portIndex++) {

		GPIO_TypeDef* currentPort = knownUsedPorts[portIndex];

		initializeGPIO(currentPort, NUMBER_OF_OUTPUT_PINS, *outputPins,
						GPIO_Speed_50MHz, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);

		initializeGPIO(currentPort, NUMBER_OF_INPUT_PINS, *inputPins,
				GPIO_Speed_50MHz, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_DOWN);

		initializeTimerPort(currentPort, NUMBER_OF_PWM_PINS, *pwmPins);
	}
}
void initializeGPIO(GPIO_TypeDef* GPIOx, uint16_t numberOfPins, PinLocation locations[],
		GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode,
		GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pushPull) {

	// Initialize the clock on the board
	uint32_t RCC_AHBPeriph = getPortRCC_Constant(GPIOx);
	RCC_AHB1PeriphClockCmd(RCC_AHBPeriph, ENABLE);


	GPIO_InitTypeDef currentPort_InitStruct;

	currentPort_InitStruct.GPIO_Speed = speed; // 50 MHZ clock frequency
	currentPort_InitStruct.GPIO_Mode = mode; // Set GPIO output
	currentPort_InitStruct.GPIO_OType = otype; // Push pull mod
	currentPort_InitStruct.GPIO_PuPd = pushPull; // Pull up

	for(uint16_t pinIndex = 0; pinIndex < numberOfPins; pinIndex++) {
		PinLocation currentIO = locations[pinIndex];

		if(sameGPIO_TypeDef(currentIO.port, GPIOx)) {
			// This takes in a binary number... To use multiple LED's one needs to use a Bitwise OR operator
			currentPort_InitStruct.GPIO_Pin |= currentIO.pin;
		}
	}

	GPIO_Init(getPort(GPIOx), &currentPort_InitStruct); // do the init
}


/**
 * This must ALWAYS be called AFTER initializing all normal pins.
 */
void initializeTimerPort(GPIO_TypeDef* GPIOx, uint16_t numberOfPossiblePins, PWM_PinLocation locations[]) {
	GPIO_InitTypeDef  currentPort_InitStruct;

	currentPort_InitStruct.GPIO_Mode = GPIO_Mode_AF; // set AF Mode

	for(uint16_t i = 0; i < numberOfPossiblePins; i++) {
		PWM_PinLocation currentPWMIO = locations[i];
		PinLocation currentIO = *currentPWMIO.location;

		if(sameGPIO_TypeDef(currentIO.port, GPIOx)) {
			// This takes in a binary number... To use multiple LED's one needs to use a Bitwise OR operator
			currentPort_InitStruct.GPIO_Pin |= currentIO.pin;

			GPIO_PinAFConfig (getPort(GPIOx), getPinSource(currentIO.pin), getTimer_AF(currentPWMIO.timerIndex));
		}
	}

	GPIO_Init(getPort(GPIOx), &currentPort_InitStruct); // do the init
}
