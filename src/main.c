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

void initializeAll_IOPins();
void initializeGPIO(GPIO_TypeDef* GPIOx, uint16_t numberOfPins, PinLocation locations[],
		GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, GPIOOType_TypeDef type, GPIOPuPd_TypeDef pushPull);

void initializeAll_Peripherals();
void initialize_TemperatureSensor();
void initialize_SonarSensor();


uint32_t getPortRCC_Constant(GPIO_TypeDef * GPIOx);
GPIO_TypeDef* getPort(GPIO_TypeDef * GPIOx);
uint8_t sameGPIO_TypeDef(GPIO_TypeDef* def1, GPIO_TypeDef* def2);


void testFlashingLight();


void coolWaterTank();
void setSolenoidsForRecirc();
void setSolenoidsForPumping();

enum ColdBrewState {
	COOLING,
	FILLING_VACUUM_CHAMBER,
	RELEASING_VACUUM,
	BREWING,
	EMPTYING_VACUUM_CHAMBER
} static machineState;


int main(void) {
	// SystemInit() call from mcu sets the clock speed

	SystemInit();

	// Initialize all GPIO pins
	initializeAll_IOPins();

	// testFlashingLight();


	// Initialize all Peripheral Sensors (Temp, Sonar)
	initializeAll_Peripherals();


	// The machine always starts by cooling
	machineState = COOLING;

	while(1) {
		GPIO_ResetBits(CoolingOnLED.port, CoolingOnLED.pin);
		GPIO_ResetBits(BrewTimeLED.port, BrewTimeLED.pin);

		switch(machineState) {
		case COOLING:

			GPIO_SetBits(CoolingOnLED.port, CoolingOnLED.pin);

			coolWaterTank();
			break;

		case FILLING_VACUUM_CHAMBER:

			break;

		case RELEASING_VACUUM:

			break;

		case BREWING:

			break;

		case EMPTYING_VACUUM_CHAMBER:
			break;
		}
	}
}


void coolWaterTank() {

	while(1) {

		//		/* Wait until all are done on one onewire port */
		//		// while (!TM_DS18B20_AllDone(&OneWire));
		//
		//		/* Read temperature from each device separatelly */
		//		for (i = 0; i < count; i++)
		//		{
		//			/* Read temperature from ROM address and store it to temps variable */
		//			if (TM_DS18B20_Read(&OneWire, device[i], &temps[i]))
		//			{


		// check the value of the WATER temperature...
		// if below a certain range...

		// Make sure that the solenoids are prepped for recirculation
		setSolenoidsForRecirc();

		// Turn on the Peltiers AND the fans
		GPIO_SetBits(PeltierSwitch.port, PeltierSwitch.pin);

		// Turn on the Recirculation Motor
		GPIO_SetBits(RecircMotor.port, RecircMotor.pin);


		// declare a variable used if a button has been pressed...
		uint8_t buttonPressed = 0;

		while(1) {


			// Read temperature 2.0...
			// Should make that into a helper function...

			// If the temperature raises above a certain height...
			//		break;

			// If a button is pressed...
			//      buttonPressed = 1;
		}


		// Turn off the recirc motor
		GPIO_ResetBits(RecircMotor.port, RecircMotor.pin);

		// Turn off the cooling
		GPIO_ResetBits(PeltierSwitch.port, PeltierSwitch.pin);

		// If a button was pressed... Exit the loop as the water has been cooled to the appropriate temperature...
		// Else just keep looping through the while loop checking if the temperature has risen

		//			}
		//	        else {
		//				// there was an error... Troubleshoot
		//			}
		//
		//		}
	}

	machineState = FILLING_VACUUM_CHAMBER;
}








void testFlashingLight() {
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
}


void initializeAll_Peripherals() {
	// this will initialize connection with the DS11adnadsfkn Temperature Sensor
	initialize_TemperatureSensor();

	initialize_SonarSensor();
}

void initialize_TemperatureSensor() {
	// this will establish communication with the OneWire library and save values
	// to static references
}

void initialize_SonarSensor() {
	// this will establish communication with the HC-SR04 library and save values
	// to static references
}





void setSolenoidsForRecirc() {
	GPIO_ResetBits(ToVacChamberSol.port, ToVacChamberSol.pin);
	GPIO_SetBits(RecircSol.port, RecircSol.pin);
}

void setSolenoidsForPumping() {
	GPIO_SetBits(ToVacChamberSol.port, ToVacChamberSol.pin);
	GPIO_ResetBits(RecircSol.port, RecircSol.pin);
}



/**
 * An initialization of all Pins based on PinLocation input and output arrays.
 */
void initializeAll_IOPins() {
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
