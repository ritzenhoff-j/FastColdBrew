/*
 * FCB_GPIO_Pins.c
 *
 *  Created on: Nov 19, 2018
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



/**
 * Returns the peripheral value of a given Timer.
 */
uint32_t getTimerPeripheral(uint8_t timerIndex) {
	// dunno why I need APB2 instead of APB1 for Timer 1
	if(timerIndex == 1) return RCC_APB2Periph_TIM1;
	if(timerIndex == 2) return RCC_APB1Periph_TIM2;
	if(timerIndex == 3) return RCC_APB1Periph_TIM3;
	if(timerIndex == 4) return RCC_APB1Periph_TIM4;
	if(timerIndex == 5) return RCC_APB1Periph_TIM5;

	return 0;
}

/**
 * Returns the Alternate Function indication of a given timer.
 */
uint8_t getTimer_AF(uint8_t timerIndex) {
	if(timerIndex == 1) return GPIO_AF_TIM1;
	if(timerIndex == 2) return GPIO_AF_TIM2;
	if(timerIndex == 3) return GPIO_AF_TIM3;
	if(timerIndex == 4) return GPIO_AF_TIM4;
	if(timerIndex == 5) return GPIO_AF_TIM5;

	return 0;
}



/**
 * Returns the Default Defined Timer constant from a given Timer Type Definition.
 */
TIM_TypeDef* getTimer_Constant(uint8_t timerIndex) {
	if(timerIndex == 1) return TIM1;
	if(timerIndex == 2) return TIM2;
	if(timerIndex == 3) return TIM3;
	if(timerIndex == 4) return TIM4;
	if(timerIndex == 5) return TIM5;

	return 0;
}


/**
 * Checks whether the control registers of two timers are the same.
 */
uint8_t sameTIM_TypeDef(TIM_TypeDef* def1, TIM_TypeDef* def2) {
	return (uint8_t)
			/*(def1->ARR == def2->ARR)
			& (def1->BDTR == def2->BDTR)
			& (def1->CCER == def2->BDTR)
			& (def1->CCMR1 == def2->BDTR)
			& (def1->CCMR2 == def2->BDTR) & */
			(def1->CCR1 == def2->BDTR)
			& (def1->CCR2 == def2->BDTR)
			& (def1->CCR3 == def2->BDTR)
			& (def1->CCR4 == def2->BDTR)
			& (def1->CNT == def2->BDTR)
			& (def1->CR1 == def2->BDTR)
			& (def1->CR2 == def2->BDTR);
			/*
			& (def1->DCR == def2->BDTR)
			& (def1->DIER == def2->BDTR)
			& (def1->DMAR == def2->BDTR)
			& (def1->EGR == def2->BDTR)
			& (def1->OR == def2->BDTR)
			& (def1->PSC == def2->BDTR)
			& (def1->RCR == def2->BDTR)
			& (def1->RESERVED0 == def2->BDTR)
			& (def1->RESERVED1 == def2->BDTR)
			& (def1->RESERVED2 == def2->BDTR)
			& (def1->RESERVED3 == def2->BDTR)
			& (def1->RESERVED4 == def2->BDTR)
			& (def1->RESERVED5 == def2->BDTR)
			& (def1->RESERVED6 == def2->BDTR)
			& (def1->RESERVED7 == def2->BDTR)
			& (def1->RESERVED8 == def2->BDTR)
			& (def1->RESERVED9 == def2->BDTR)
			& (def1->RESERVED10 == def2->BDTR)
			& (def1->RESERVED11 == def2->BDTR)
			& (def1->SMCR == def2->BDTR)
			& (def1->SR == def2->BDTR); */

}


/**
 * Returns the pin source from a given GPIO_Pin_x.
 */
uint16_t getPinSource(uint16_t GPIO_Pin_x) {
	switch(GPIO_Pin_x) {
	case GPIO_Pin_0: return GPIO_PinSource0;
	case GPIO_Pin_1: return GPIO_PinSource1;
	case GPIO_Pin_2: return GPIO_PinSource2;
	case GPIO_Pin_3: return GPIO_PinSource3;
	case GPIO_Pin_4: return GPIO_PinSource4;
	case GPIO_Pin_5: return GPIO_PinSource5;
	case GPIO_Pin_6: return GPIO_PinSource6;
	case GPIO_Pin_7: return GPIO_PinSource7;
	case GPIO_Pin_8: return GPIO_PinSource8;
	case GPIO_Pin_9: return GPIO_PinSource9;
	case GPIO_Pin_10: return GPIO_PinSource10;
	case GPIO_Pin_11: return GPIO_PinSource11;
	case GPIO_Pin_12: return GPIO_PinSource12;
	case GPIO_Pin_13: return GPIO_PinSource13;
	case GPIO_Pin_14: return GPIO_PinSource14;
	case GPIO_Pin_15: return GPIO_PinSource15;
	}

	return 0;
}
