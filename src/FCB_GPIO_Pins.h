#ifndef STM_STD_LIB
#define STM_STD_LIB "stm32f4xx.h"
#include STM_STD_LIB
#endif



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


// static const PinLocation SWDIO = { GPIOA, GPIO_Pin_13 };
// static const PinLocation SWCLK = { GPIOA, GPIO_Pin_14 };
// nRST

static const PinLocation VacuumPump = { GPIOA, GPIO_Pin_4 };
static const PinLocation RecircPump = { GPIOA, GPIO_Pin_5 };
static const PinLocation MixingMotor = { GPIOA, GPIO_Pin_6 };
static const PinLocation HBridgeEnable = { GPIOA, GPIO_Pin_7 };
static const PinLocation RecircSol = { GPIOA, GPIO_Pin_8 };
static const PinLocation ToVacChamberSol = { GPIOA, GPIO_Pin_9 };
static const PinLocation PressureReleaseSol = { GPIOA, GPIO_Pin_10 };
static const PinLocation CoffeeReleaseSol = { GPIOA, GPIO_Pin_11 };
static const PinLocation SonarSensorTrigger = { GPIOB, GPIO_Pin_12 };
static const PinLocation SonarSensorEcho = { GPIOB, GPIO_Pin_13 };
static const PinLocation WaterTempSensor = { GPIOB, GPIO_Pin_14 };
static const PinLocation PeltierTempSensor = { GPIOB, GPIO_Pin_15 };
static const PinLocation PeltierSwitch = { GPIOC, GPIO_Pin_6 };
static const PinLocation PeltierCoolingFan = { GPIOC, GPIO_Pin_7 };
static const PinLocation PressureSensor = { GPIOC, GPIO_Pin_8 };

static const PinLocation LowWaterLED = { GPIOB, GPIO_Pin_3 };
static const PinLocation BrewMin1LED = { GPIOB, GPIO_Pin_4 };
static const PinLocation BrewMin2LED = { GPIOB, GPIO_Pin_5 };
static const PinLocation BrewMin3LED = { GPIOB, GPIO_Pin_6 };
static const PinLocation CoolingOnLED = { GPIOB, GPIO_Pin_8 };
static const PinLocation PressureOnLED = { GPIOB, GPIO_Pin_9 };
static const PinLocation SmallButtonLED = { GPIOC, GPIO_Pin_0 };
static const PinLocation MediumButtonLED = { GPIOC, GPIO_Pin_1 };
static const PinLocation LargeButtonLED = { GPIOC, GPIO_Pin_2 };


static const PinLocation SmallButton = { GPIOC, GPIO_Pin_10 };
static const PinLocation MediumButton = { GPIOC, GPIO_Pin_11 };
static const PinLocation LargeButton = { GPIOC, GPIO_Pin_12 };
static const PinLocation RxPin = { GPIOA, GPIO_Pin_2 };
static const PinLocation TxPin = { GPIOA, GPIO_Pin_3 };



// Array of all pins utilized for GPIO initialization
/* The variable needs to be defined or else C does not realize it as unchangeable
 * and will not allow the number to be a size for the PinLocation array */
#define NUMBER_OF_OUTPUT_PINS ((int)23)
static const PinLocation *outputPins[NUMBER_OF_OUTPUT_PINS] = {
		&VacuumPump, &RecircPump, &MixingMotor, &HBridgeEnable,
		&RecircSol, &ToVacChamberSol, &PressureReleaseSol, &CoffeeReleaseSol,
		&WaterTempSensor, &PeltierTempSensor,
		&SonarSensorTrigger,
		&PeltierSwitch, &PeltierCoolingFan,
		&LowWaterLED, &BrewMin1LED, &BrewMin2LED, &BrewMin3LED, &CoolingOnLED, &PressureOnLED,
		&SmallButtonLED, &MediumButtonLED, &LargeButtonLED,
		&TxPin};

#define NUMBER_OF_INPUT_PINS ((int)6)
static const PinLocation *inputPins[NUMBER_OF_INPUT_PINS] = {
		&SonarSensorEcho,
		&PressureSensor,
		&SmallButton, &MediumButton, &LargeButton,
		&RxPin };


// Array of all ports utilized for GPIO initialization
#define NUMBER_OF_PORTS ((int) 3)
static const GPIO_TypeDef * knownUsedPorts[NUMBER_OF_PORTS] = { GPIOA, GPIOB, GPIOC };
