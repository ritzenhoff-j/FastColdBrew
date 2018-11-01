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
	GPIO_TypeDef * port;
	uint16_t pin;
};

// Reference the entire PinLocation struct just as a PinLocation
typedef struct PinLocation PinLocation;

static const PinLocation VacuumMotor = { GPIOA, GPIO_Pin_0 };
static const PinLocation RecircMotor = { GPIOA, GPIO_Pin_5};
static const PinLocation MixingMotor = { GPIOA, GPIO_Pin_6};
static const PinLocation RecircSol = { GPIOA, GPIO_Pin_8 };
static const PinLocation ToVacChamberSol = { GPIOA, GPIO_Pin_9 };
static const PinLocation CoffeeReleaseSol = { GPIOA, GPIO_Pin_10 };
static const PinLocation SonarSensor = { GPIOA, GPIO_Pin_11 };
static const PinLocation TempSensor = { GPIOA, GPIO_Pin_12 };
static const PinLocation PeltierSwitch = { GPIOA, GPIO_Pin_13 };
static const PinLocation LowWaterLED = { GPIOB, GPIO_Pin_3 };
static const PinLocation BrewTimeLED = { GPIOB, GPIO_Pin_4 };
static const PinLocation CoolingOnLED = { GPIOB, GPIO_Pin_5 };
static const PinLocation PressureOnLED = { GPIOB, GPIO_Pin_6 };

static const PinLocation SmallButton = { GPIOB, GPIO_Pin_12 };
static const PinLocation MediumButton = { GPIOB, GPIO_Pin_13};
static const PinLocation LargeButton = { GPIOB, GPIO_Pin_14 };

static const PinLocation RxPin = { GPIOA, GPIO_Pin_2 };
static const PinLocation TxPin = { GPIOA, GPIO_Pin_3 };



// Array of all pins utilized for GPIO initialization
/* The variable needs to be defined or else C does not realize it as unchangeable
 * and will not allow the number to be a size for the PinLocation array */
#define NUMBER_OF_OUTPUT_PINS ((int)15)
static const PinLocation *outputPins[NUMBER_OF_OUTPUT_PINS] = {
		&VacuumMotor, &RecircMotor, &MixingMotor,
		&RecircSol, &ToVacChamberSol, &CoffeeReleaseSol,
		&SonarSensor, &TempSensor,
		&PeltierSwitch,
		&LowWaterLED, &BrewTimeLED, &CoolingOnLED, &PressureOnLED,
		&TxPin};

#define NUMBER_OF_INPUT_PINS ((int)4)
static const PinLocation *inputPins[NUMBER_OF_INPUT_PINS] = {
		&SmallButton, &MediumButton, &LargeButton,
		&RxPin };


// Array of all ports utilized for GPIO initialization
#define NUMBER_OF_PORTS 2
static const GPIO_TypeDef * knownUsedPorts[NUMBER_OF_PORTS] = { GPIOA, GPIOB };

void initializeAllPins();
void initializeGPIO(GPIO_TypeDef* GPIOx, uint16_t numberOfPins, PinLocation locations[],
		GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, GPIOOType_TypeDef type, GPIOPuPd_TypeDef pushPull);

uint32_t getPortRCC_Constant(GPIO_TypeDef * GPIOx);
GPIO_TypeDef* getPort(GPIO_TypeDef * GPIOx);
uint8_t sameGPIO_TypeDef(GPIO_TypeDef* def1, GPIO_TypeDef* def2);


int main(void) {
	// SystemInit() call from mcu sets the clock speed

	SystemInit();

	/*
	uint32_t RCC_AHBPeriph = getPortRCC_Constant(LowWaterLED.port);
	uint32_t RCC_AHB_Actual = RCC_AHB1Periph_GPIOB;

	RCC_AHB1PeriphClockCmd(RCC_AHB_Actual, ENABLE);


	GPIO_InitTypeDef currentPort_InitStruct;

	currentPort_InitStruct.GPIO_Pin = GPIO_Pin_3;
	currentPort_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 50 MHZ clock frequency
	currentPort_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // Set GPIO output
	currentPort_InitStruct.GPIO_OType = GPIO_OType_PP ; // Push pull mod
	currentPort_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // Pull up

	int actual = GPIOB;
	int found = LowWaterLED.port;


	GPIO_Init(found, &currentPort_InitStruct);
	*/


	// Initialize all GPIO pins
	initializeAllPins();


	uint8_t bool = 0;

	while(1) {

		if(bool) {
			GPIO_ResetBits(LowWaterLED.port, LowWaterLED.pin);

			bool = 0;
		}
		else {

			GPIO_SetBits(LowWaterLED.port, LowWaterLED.pin);

			bool = 1;
		}

		int j = 0;

		for(int i = 0; i < 5000000; i++) {
			j++;
		}

	}

	for(;;);
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

void initializeAllPins() {
	for(uint8_t portIndex = 1; portIndex < NUMBER_OF_PORTS; portIndex++) {

		GPIO_TypeDef* currentPort = knownUsedPorts[portIndex];

		initializeGPIO(currentPort, NUMBER_OF_OUTPUT_PINS, *outputPins,
				GPIO_Speed_50MHz, GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL);

		/*
		initializeGPIO(currentPort, NUMBER_OF_INPUT_PINS, *inputPins,
				GPIO_Speed_50MHz, GPIO_Mode_IN, GPIO_OType_PP, GPIO_PuPd_NOPULL);
		*/
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

uint8_t sameGPIO_TypeDef(GPIO_TypeDef* def1, GPIO_TypeDef* def2) {
	return (def1->MODER == def2->MODER);

	/*(def1->BSRRH == def2->BSRRH)
						& (def1->BSRRL == def2->BSRRL)
						& (def1->IDR == def2->IDR)
						& (def1->LCKR == def2->LCKR)

						& (def1->ODR == def2->ODR)
						& (def1->OSPEEDR == def2->OSPEEDR)
						& (def1->OTYPER == def2->OTYPER)
						& (def1->PUPDR == def2->PUPDR); */
}
