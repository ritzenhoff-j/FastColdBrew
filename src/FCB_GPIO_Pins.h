#ifndef STM_STD_LIB
#define STM_STD_LIB "stm32f4xx.h"
#include STM_STD_LIB
#endif



// Discussion of static const int, #define, and enum
// https://stackoverflow.com/questions/1674032/static-const-vs-define-vs-enum

// decision was made to declare all global variables as static const uint8_t
// 		- pin mapping based off of 64 pin chip
//		- no port should have more than 16 pins

typedef struct {
	GPIO_TypeDef * port;
	uint16_t pin;
} PinLocation;


typedef struct {
	PinLocation* location;
	uint8_t timerIndex;
	uint8_t channel;
} PWM_PinLocation;


// static const PinLocation SWDIO = { GPIOA, GPIO_Pin_13 };
// static const PinLocation SWCLK = { GPIOA, GPIO_Pin_14 };
// nRST

static const PinLocation HBridgeEnablePin = { GPIOA, GPIO_Pin_4 };

static const PinLocation VacuumPumpPin = { GPIOA, GPIO_Pin_5 };  // TIM2_CH1
static const PinLocation RecircPumpPin = { GPIOA, GPIO_Pin_6 }; // TIM3_CH1
static const PinLocation MixingMotorPin = { GPIOA, GPIO_Pin_8 }; // TIM1_CH1

static const PinLocation PeltierCoolingFanPin = { GPIOB, GPIO_Pin_6 }; // TIM4_CH1

static const PWM_PinLocation PWM_VacuumPump = { &VacuumPumpPin, 2, 1 };
static const PWM_PinLocation PWM_RecircPump = { &RecircPumpPin, 3, 1 };
static const PWM_PinLocation PWM_MixingMotor = { &MixingMotorPin, 1, 1 };
static const PWM_PinLocation PWM_PeltierCoolingFan = { &PeltierCoolingFanPin, 4, 1 };



static const PinLocation RecircSol = { GPIOA, GPIO_Pin_7 };
static const PinLocation ToVacChamberSol = { GPIOA, GPIO_Pin_9 };
static const PinLocation PressureReleaseSol = { GPIOA, GPIO_Pin_10 };
static const PinLocation CoffeeReleaseSol = { GPIOA, GPIO_Pin_11 };

static const PinLocation SonarSensorTrigger = { GPIOB, GPIO_Pin_0 };
static const PinLocation SonarSensorEcho = { GPIOB, GPIO_Pin_1 };
static const PinLocation WaterTempSensor = { GPIOB, GPIO_Pin_2 };
static const PinLocation PeltierTempSensor = { GPIOB, GPIO_Pin_3 };
static const PinLocation PressureSensor = { GPIOB, GPIO_Pin_4 };
static const PinLocation PeltierSwitch = { GPIOB, GPIO_Pin_5 };

static const PinLocation SmallButton = { GPIOB, GPIO_Pin_12 };
static const PinLocation MediumButton = { GPIOB, GPIO_Pin_13 };
static const PinLocation LargeButton = { GPIOB, GPIO_Pin_14 };
static const PinLocation CancelButton = { GPIOB, GPIO_Pin_15 };

static const PinLocation LowWaterLED = { GPIOC, GPIO_Pin_0 };
static const PinLocation BrewMin1LED = { GPIOC, GPIO_Pin_1 };
static const PinLocation BrewMin2LED = { GPIOC, GPIO_Pin_2 };
static const PinLocation BrewMin3LED = { GPIOC, GPIO_Pin_3 };
static const PinLocation CoolingOnLED = { GPIOC, GPIO_Pin_4 };
static const PinLocation PressureOnLED = { GPIOC, GPIO_Pin_5 };
static const PinLocation SmallButtonLED = { GPIOC, GPIO_Pin_6 };
static const PinLocation MediumButtonLED = { GPIOC, GPIO_Pin_7 };
static const PinLocation LargeButtonLED = { GPIOC, GPIO_Pin_8 };
static const PinLocation TestLED = { GPIOC, GPIO_Pin_9 };

static const PinLocation RxPin = { GPIOA, GPIO_Pin_2 };
static const PinLocation TxPin = { GPIOA, GPIO_Pin_3 };


// Array of all pins utilized for GPIO initialization
/* The variable needs to be defined or else C does not realize it as unchangeable
 * and will not allow the number to be a size for the PinLocation array */
#define NUMBER_OF_OUTPUT_PINS ((int)20)
static const PinLocation *outputPins[NUMBER_OF_OUTPUT_PINS] = {
		&RecircSol, &ToVacChamberSol, &PressureReleaseSol, &CoffeeReleaseSol,
		&WaterTempSensor, &PeltierTempSensor,
		&SonarSensorTrigger,
		&PeltierSwitch,
		&LowWaterLED, &BrewMin1LED, &BrewMin2LED, &BrewMin3LED, &CoolingOnLED, &PressureOnLED,
		&SmallButtonLED, &MediumButtonLED, &LargeButtonLED, &TestLED,
		&TxPin};

#define NUMBER_OF_INPUT_PINS ((int)7)
static const PinLocation *inputPins[NUMBER_OF_INPUT_PINS] = {
		&SonarSensorEcho,
		&PressureSensor,
		&SmallButton, &MediumButton, &LargeButton, &CancelButton,
		&RxPin };

// Array of all ports utilized for GPIO initialization
#define NUMBER_OF_PORTS ((int) 3)
static const GPIO_TypeDef * knownUsedPorts[NUMBER_OF_PORTS] = { GPIOA, GPIOB, GPIOC };

#define NUMBER_OF_PWM_PINS ((int)4)
static const PWM_PinLocation *pwmPins[NUMBER_OF_PWM_PINS] = {
		&PWM_VacuumPump,
		&PWM_RecircPump,
		&PWM_MixingMotor,
		&PWM_PeltierCoolingFan
};


// Array of all ports utilized for GPIO initialization
#define NUMBER_OF_TIMERS ((int) 4)
static const uint8_t knownUsedTimers[NUMBER_OF_TIMERS] = { 1, 2, 3, 4 };


// Normal GPIO Initialization
uint32_t getPortRCC_Constant(GPIO_TypeDef * GPIOx);
GPIO_TypeDef* getPort(GPIO_TypeDef * GPIOx);
uint8_t sameGPIO_TypeDef(GPIO_TypeDef* def1, GPIO_TypeDef* def2);

// PWM GPIO Initialization
uint32_t getTimerPeripheral(uint8_t timerIndex);
uint8_t getTimer_AF(uint8_t timerIndex);
TIM_TypeDef* getTimer_Constant(uint8_t timerIndex);
uint8_t sameTIM_TypeDef(TIM_TypeDef* def1, TIM_TypeDef* def2);

uint16_t getPinSource(uint16_t GPIO_Pin_x);


