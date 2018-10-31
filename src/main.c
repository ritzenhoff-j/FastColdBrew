/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"

// Discussion of static const int, #define, and enum
// https://stackoverflow.com/questions/1674032/static-const-vs-define-vs-enum

// decision was made to declare all global variables as static const uint8_t
// 		- pin mapping based off of 64 pin chip
//		- no port should have more than 16 pins

struct PinLocation {
	GPIO_TypeDef port;
	uint16_t pin;

};

// Reference the entire PinLocation struct just as a PinLocation
typedef struct PinLocation PinLocation;

static const PinLocation VacuumMotor = { GPIOA, 0 };
static const PinLocation RecircMotor = { GPIOA, 5};
static const PinLocation MixingMotor = { GPIOA, 6};
static const PinLocation RecircSol = { GPIOA, 8 };
static const PinLocation ToVacChamberSol = { GPIOA, 9 };
static const PinLocation CoffeeReleaseSol = { GPIOA, 10 };
static const PinLocation SonarSensor = { GPIOA, 11 };
static const PinLocation TempSensor = { GPIOA, 12 };
static const PinLocation PeltierSwitch = { GPIOA, 13 };
static const PinLocation LowWaterLED = { GPIOB, 3 };
static const PinLocation BrewTimeLED = { GPIOB, 4 };
static const PinLocation CoolingOnLED = { GPIOB, 5 };
static const PinLocation PressureOnLED = { GPIOB, 6 };

static const PinLocation SmallButton = { GPIOB, 12 };
static const PinLocation MediumButton = { GPIOB, 13};
static const PinLocation LargeButton = { GPIOB, 14 };

static const PinLocation RxPin = { GPIOA, 2 };
static const PinLocation TxPin = { GPIOA, 3 };

// Array of all pins utilized for GPIO initialization
static const uint8_t numberOfOutputPins = 15;
static const PinLocation outputPins[numberOfOutputPins] = {
		VacuumMotor, RecircMotor, MixingMotor,
		RecircSol, ToVacChamberSol, CoffeeReleaseSol,
		SonarSensor, TempSensor,
		PeltierSwitch,
		LowWaterLED, BrewTimeLED, CoolingOnLED, PressureOnLED,
		TxPin};

static const uint8_t numberOfInputPins = 4;
static const PinLocation inputPins[numberOfInputPins] = {
		SmallButton, MediumButton, LargeButton,
		RxPin };


// Array of all ports utilized for GPIO initialization
static const uint8_t numberOfPorts = 2;
static const GPIO_TypeDef knownUsedPorts[numberOfPorts] = { GPIOA, GPIOB };

uint32_t getPortRCC_Constant(GPIO_TypeDef GPIOx);
void initializeGPIO(GPIO_TypeDef GPIOx, uint16_t numberOfPins, PinLocation locations[],
		GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, GPIOOType_TypeDef type, GPIOPuPd_TypeDef pushPull);

int main(void) {
	// SystemInit() call from mcu sets the clock speed

	// Initialize all GPIO pins
	for(uint8_t portIndex = 0; portIndex < numberOfPorts; portIndex++) {
		GPIO_TypeDef currentPort = knownUsedPorts[portIndex];

		initializeGPIO(currentPort, numberOfOutputPins, outputPins,
				GPIO_Speed_50MHz, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP);

		initializeGPIO(currentPort, numberOfInputPins, inputPins,
				GPIO_Speed_50MHz, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_UP);
	}

	for(;;);
}


uint32_t getPortRCC_Constant(GPIO_TypeDef GPIOx) {
	switch(GPIOx) {
	case GPIOA: return RCC_AHB1Periph_GPIOA;
	case GPIOB: return RCC_AHB1Periph_GPIOB;
	case GPIOC: return RCC_AHB1Periph_GPIOC;
	case GPIOD: return RCC_AHB1Periph_GPIOD;
	case GPIOE: return RCC_AHB1Periph_GPIOE;
	case GPIOF: return RCC_AHB1Periph_GPIOF;
	case GPIOG: return RCC_AHB1Periph_GPIOG;
	case GPIOH: return RCC_AHB1Periph_GPIOH;
	case GPIOI:return RCC_AHB1Periph_GPIOI;
	default: return 0x0;
	}
}

void initializeGPIO(GPIO_TypeDef GPIOx, uint16_t numberOfPins, PinLocation locations[],
		GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode,
		GPIOOType_TypeDef otype, GPIOPuPd_TypeDef pushPull) {

	RCC_AHB1PeriphClockCmd(getPortRCC_Constant(GPIOx), ENABLE);

	GPIO_InitTypeDef currentPort_InitStruct;

	currentPort_InitStruct.GPIO_Speed = speed; // 50 MHZ clock frequency
	currentPort_InitStruct.GPIO_Mode = mode; // Set GPIO output
	currentPort_InitStruct.GPIO_OType = otype; // Push pull mod
	currentPort_InitStruct.GPIO_PuPd = pushPull; // Pull up

	for(uint16_t pinIndex = 0; pinIndex < numberOfOutputPins; pinIndex++) {
		if(locations[pinIndex].port == GPIOx) {
			// This takes in a binary number... To use multiple LED's one needs to use a Bitwise OR operator
			currentPort_InitStruct.GPIO_Pin |= locations[pinIndex].pin;
		}
	}

	GPIO_Init(GPIOx, &currentPort_InitStruct); // do the init

}
