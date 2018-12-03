/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
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

#ifndef FCB_PWM_INIT
#define FCB_PWM_INIT "FCB_PWM.h"
#include FCB_PWM_INIT
#endif

#ifndef FCB_TEMP_SENSOR
#define FCB_TEMP_SENSOR "FCB_TempSensor.h"
#include FCB_TEMP_SENSOR
#endif

#ifndef FCB_SONAR_SENSER
#define FCB_SONAR_SENSER "FCB_SonarSensor.h"
#include FCB_SONAR_SENSER
#endif

#ifndef FCB_PRESSURE_SENSOR
#define FCB_PRESSURE_SENSOR "FCB_PressureSensor.h"
#include FCB_PRESSURE_SENSOR
#endif

#ifndef TM_DELAY
#define TM_DELAY "tm_stm32f4_delay.h"
#include TM_DELAY
#endif

#ifndef FCB_TESTS_REALTIME
#define FCB_TESTS_REALTIME "FCB_Tests_RealTime.h"
#include FCB_TESTS_REALTIME
#endif


void SysTickInit(uint16_t frequency);

void coolWaterTank();
uint8_t getSizeInput();
void fillVacuumChamber(uint8_t ozToFill);
void createVacuum();
void brewCoffee(uint8_t ozSize);
void releaseVacuum();
void emptyVacuumChamber(uint8_t ozSize);


void setSolenoidsForRecirc();
void setSolenoidsForFilling();

void initializeAll_Peripherals();

enum ColdBrewState {
	WAITING,
	COOLING,
	WAITING_FOR_SIZE,
	FILLING_VACUUM_CHAMBER,
	CREATING_VACUUM,
	RELEASING_VACUUM,
	BREWING,
	EMPTYING_VACUUM_CHAMBER
} static machineState;


int main(void) {
	// SystemInit() call from mcu sets the clock speed
	SystemInit();

	// initialize the tick
	SysTickInit(1000);

	// Initialize all GPIO pins ** AND TM_DELAY **
	initializeAll_IOPins();

	// Initialize all PWM pins
	initializePWM();

	// testFlashingLight();
	// testButtonFlashingLight();
	//testPWM();
	//testADC();

	testAllLEDs();

	// Initialize all Peripheral Sensors (Temp, Sonar)
	initializeAll_Peripherals();

	// testTempAndSonar();

	// The machine always starts by cooling
	machineState = COOLING;

	// turn off ALL outputs
	uint8_t brewSize = 0;

	while(1) {
		switch(machineState) {
		case COOLING:
			coolWaterTank();
			break;

		case WAITING_FOR_SIZE:
			brewSize = getSizeInput();
			break;

		case FILLING_VACUUM_CHAMBER:
			fillVacuumChamber(brewSize);
			break;

		case CREATING_VACUUM:
			createVacuum();
			break;

		case BREWING:
			brewCoffee(brewSize);
			break;

		case RELEASING_VACUUM:
			releaseVacuum();
			break;

		case EMPTYING_VACUUM_CHAMBER:
			emptyVacuumChamber(brewSize);

			break;
		}
	}
}

void SysTickInit (uint16_t frequency)
{
   RCC_ClocksTypeDef RCC_Clocks;
   RCC_GetClocksFreq (&RCC_Clocks);
   (void) SysTick_Config (RCC_Clocks.HCLK_Frequency / frequency);
}


/**
 * Cools the water chamber below its hardcoded threshold by turning on Peltiers.
 */
void coolWaterTank() {

	uint8_t lowWaterState = 0 ;

	while(!isTemperatureBelowMax(WaterTank) || isWaterHeightBelowMin()) {

		if(isWaterHeightBelowMin()) {
			if(lowWaterState) {
				// turn ON low water level indicator LED
				GPIO_SetBits(LowWaterLED.port, LowWaterLED.pin);
			}
			else {
				// turn OFF low water level indicator LED
				GPIO_ResetBits(LowWaterLED.port, LowWaterLED.pin);
			}

			// this creates a blinking of the light
			lowWaterState = ~lowWaterState;
		}
		else {
			// turn OFF low water level indicator LED
			GPIO_ResetBits(LowWaterLED.port, LowWaterLED.pin);


			// turn ON the cooling indicator LED
			GPIO_SetBits(CoolingOnLED.port, CoolingOnLED.pin);

			// turn ON the Peltiers
			GPIO_SetBits(PeltierSwitch.port, PeltierSwitch.pin);

			// turn ON the Peltiers cooling fan
			setPWM(PWM_PeltierCoolingFan.timerIndex, PWM_PeltierCoolingFan.channel, 100.0);



			// set the Solenoids for recirculation
			setSolenoidsForRecirc();

			// turn ON the recirculation pump
			setPWM(PWM_RecircPump.timerIndex, PWM_RecircPump.channel, 50);
		}

		// delay for a short period of time (from TM library)
		Delayms(500);

		// want to make sure that the peltiers are not constantly being turned on and off
		// - Idealy only switches state if there have been numerous readings in the opposite state
	}

	// turn OFF the cooling indicator LED
	GPIO_ResetBits(CoolingOnLED.port, CoolingOnLED.pin);

	// turn OFF the Peltiers
	GPIO_ResetBits(PeltierSwitch.port, PeltierSwitch.pin);

	// turn OFF the Peltiers cooling fan
	setPWM(PWM_PeltierCoolingFan.timerIndex, PWM_PeltierCoolingFan.channel, 0);

	// turn OFF cooling indicator LED
	GPIO_ResetBits(CoolingOnLED.port, CoolingOnLED.pin);

	// turn OFF the recirculation pump
	setPWM(PWM_RecircPump.timerIndex, PWM_RecircPump.channel, 0);

	machineState = WAITING_FOR_SIZE;
}

/**
 * Returns the size of the coffee to be brewed in ounces.
 *
 * NOTE: Only sizes offered are 8, 10, 12oz
 */
uint8_t getSizeInput() {

	uint8_t size = 0;

	while(size == 0) {

		if(GPIO_ReadInputDataBit(SmallButton.port, SmallButton.pin)) {
			size = 8;

			break;
		}

		if(GPIO_ReadInputDataBit(MediumButton.port, MediumButton.pin)) {
			size = 10;

			break;
		}

		if(GPIO_ReadInputDataBit(LargeButton.port, LargeButton.pin)) {
			size = 12;

			break;
		}

		// delay for 500 ms
		Delayms(500);
	}


	machineState = FILLING_VACUUM_CHAMBER;

	return size;
}

/**
 * Turns on the appropriate pins and delays the code until the vacuum chamber has been filled.
 */
void fillVacuumChamber(uint8_t ozToFill) {

	// set the solenoids for filling the water tank
	setSolenoidsForFilling();

	// turn ON the water pump
	setPWM(PWM_RecircPump.timerIndex, PWM_RecircPump.channel, 50);

	// delay until the water pump has been filled
	if(!delayUntilChangeInOunces(ozToFill)) {
		// there was a timeout
	}

	machineState = CREATING_VACUUM;
}

/**
 * Turns on the appropriate pins and delays the code until vacuum has been achieved.
 */
void createVacuum() {
	// turn on the vacuum pumps
	setPWM(PWM_VacuumPump.timerIndex, PWM_VacuumPump.channel, 100);

	if(!delayUntilPressureBelowMax()) {
		// timed out
	}

	// turn ON pressure on indicator LED
	GPIO_SetBits(PressureOnLED.port, PressureOnLED.pin);

	machineState = BREWING;
}

/**
 * Turns on the mixing motor, controls vacuum pressure, and delays code for set amount of time.
 */
void brewCoffee(uint8_t ozSize) {
	// turn on mixing motor
	setPWM(PWM_MixingMotor.timerIndex, PWM_MixingMotor.channel, 100);

	// ?? turn OFF vacuum pumps


	uint32_t oneMinute = 60 * 1000;

	// delay for one miute
	Delayms(oneMinute);
	// turn on appropriate indicator LED
	GPIO_SetBits(BrewMin1LED.port, BrewMin1LED.pin);

	// delay for another minute
	Delayms(oneMinute);
	// turn on appropriate indicator LED
	GPIO_SetBits(BrewMin2LED.port, BrewMin2LED.pin);

	// delay for another minute
	Delayms(oneMinute);
	// turn on appropriate indicator LED
	GPIO_SetBits(BrewMin3LED.port, BrewMin3LED.pin);



	// turn OFF vacuum pumps
	setPWM(PWM_VacuumPump.timerIndex, PWM_VacuumPump.channel, 0);

	// turn OFF mixing motor
	setPWM(PWM_MixingMotor.timerIndex, PWM_MixingMotor.channel, 0);

	machineState = RELEASING_VACUUM;
}

/**
 * Releases the vacuum and waits until the pressure rises above a given level.
 */
void releaseVacuum() {
	// release the pressure itself
	GPIO_SetBits(PressureReleaseSol.port, PressureReleaseSol.pin);

	// delay until the pressure has reached normal levels
	if(!delayUntilPressureAtRoom()) {
		// delay timed out
	}

	// turn OFF pressure on indicator LED
	GPIO_ResetBits(PressureOnLED.port, PressureOnLED.pin);

	machineState = EMPTYING_VACUUM_CHAMBER;
}

/**
 * Empties the vacuum chamber for a given amount of time.
 */
void emptyVacuumChamber(uint8_t ozSize) {
	// open the vacuum chamber release (pressure release should still be open)
	GPIO_SetBits(CoffeeReleaseSol.port, CoffeeReleaseSol.pin);

	int timeToEmpty = 30 * 1000;

	Delayms(timeToEmpty);

	// close the pressure release
	GPIO_ResetBits(CoffeeReleaseSol.port, CoffeeReleaseSol.pin);

	// close the vacuum chamber release
	GPIO_ResetBits(PressureReleaseSol.port, PressureReleaseSol.pin);

	machineState = COOLING;

}



/**
 * Initialize all peripherals of the Fast Cold Brew Machine.
 *
 * Initializes TM Delay.
 */
void initializeAll_Peripherals() {
	TM_DELAY_Init();

	initializeAllTempSensors();

	// initializeSonarSensor();
}


void setSolenoidsForRecirc() {
	GPIO_ResetBits(ToVacChamberSol.port, ToVacChamberSol.pin);
	GPIO_SetBits(RecircSol.port, RecircSol.pin);
}

void setSolenoidsForFilling() {
	GPIO_SetBits(ToVacChamberSol.port, ToVacChamberSol.pin);
	GPIO_ResetBits(RecircSol.port, RecircSol.pin);
}



