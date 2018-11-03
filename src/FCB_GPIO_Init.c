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
	for(uint8_t portIndex = 1; portIndex < NUMBER_OF_PORTS; portIndex++) {

		GPIO_TypeDef* currentPort = knownUsedPorts[portIndex];

		initializeGPIO(currentPort, NUMBER_OF_OUTPUT_PINS, *outputPins,
						GPIO_Speed_50MHz, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);

		initializeGPIO(currentPort, NUMBER_OF_INPUT_PINS, *inputPins,
				GPIO_Speed_50MHz, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_DOWN);
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

uint32_t getPortRCC_Constant(GPIO_TypeDef * GPIOx) {
	GPIO_TypeDef* port = getPort(GPIOx);

	if(port == GPIOA) return RCC_AHB1Periph_GPIOA;
	if(port == GPIOB) return RCC_AHB1Periph_GPIOB;
	if(port == GPIOC) return RCC_AHB1Periph_GPIOC;
	if(port == GPIOD) return RCC_AHB1Periph_GPIOD;
	if(port == GPIOE) return RCC_AHB1Periph_GPIOE;
//	if(sameGPIO_TypeDef(&GPIOx, GPIOF)) return RCC_AHB1Periph_GPIOF;
//	if(sameGPIO_TypeDef(&GPIOx, GPIOG)) return RCC_AHB1Periph_GPIOG;
//	if(sameGPIO_TypeDef(&GPIOx, GPIOH)) return RCC_AHB1Periph_GPIOH;
//	if(sameGPIO_TypeDef(&GPIOx, GPIOI)) return RCC_AHB1Periph_GPIOI;

	return 0;
}

/**
 * Returns a complete pointer GPIO object based on a few matching fields of the input.
 */
GPIO_TypeDef* getPort(GPIO_TypeDef* GPIOx) {
	if(sameGPIO_TypeDef(GPIOx, GPIOA)) return GPIOA;
	if(sameGPIO_TypeDef(GPIOx, GPIOB)) return GPIOB;
	if(sameGPIO_TypeDef(GPIOx, GPIOC)) return GPIOC;
	if(sameGPIO_TypeDef(GPIOx, GPIOD)) return GPIOD;
	if(sameGPIO_TypeDef(GPIOx, GPIOE)) return GPIOE;
//	if(sameGPIO_TypeDef(GPIOx, GPIOF)) return GPIOF;
//	if(sameGPIO_TypeDef(GPIOx, GPIOG)) return GPIOG;
//	if(sameGPIO_TypeDef(GPIOx, GPIOH)) return GPIOH;
//	if(sameGPIO_TypeDef(GPIOx, GPIOI)) return GPIOI;

	return 0;
}
uint8_t sameGPIO_TypeDef(GPIO_TypeDef* def1, GPIO_TypeDef* def2) {
	return (uint8_t) (def1->MODER == def2->MODER);

	/*(def1->BSRRH == def2->BSRRH)
						& (def1->BSRRL == def2->BSRRL)
						& (def1->IDR == def2->IDR)
						& (def1->LCKR == def2->LCKR)

						& (def1->ODR == def2->ODR)
						& (def1->OSPEEDR == def2->OSPEEDR)
						& (def1->OTYPER == def2->OTYPER)
						& (def1->PUPDR == def2->PUPDR); */
}
