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
// 		- all pin mappings change depending on what chip is being used

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


int main(void)
{
	// SystemInit() call from mcu sets the clock speed

	for(;;);
}
