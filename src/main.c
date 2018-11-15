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

#ifndef FCB_TEMP_SENSOR
#define FCB_TEMP_SENSOR "FCB_TempSensor.h"
#include FCB_TEMP_SENSOR
#endif

#ifndef FCB_SONAR_SENSER
#define FCB_SONAR_SENSER "FCB_SonarSensor.h"
#include FCB_SONAR_SENSER
#endif

#ifndef TM_DELAY
#define TM_DELAY "tm_stm32f4_delay.h"
#include TM_DELAY
#endif




void testFlashingLight();
void testButtonFlashingLight();


void coolWaterTank();
void setSolenoidsForRecirc();
void setSolenoidsForPumping();

void initializeAll_Peripherals();

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

	// Initialize all Peripheral Sensors (Temp, Sonar)
	initializeAll_Peripherals();

	// testFlashingLight();
	testButtonFlashingLight();

	// The machine always starts by cooling
	machineState = COOLING;

	while(1) {
		GPIO_ResetBits(CoolingOnLED.port, CoolingOnLED.pin);
		GPIO_ResetBits(BrewMin1LED.port, BrewMin1LED.pin);

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
		GPIO_SetBits(RecircPump.port, RecircPump.pin);


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
		GPIO_ResetBits(RecircPump.port, RecircPump.pin);

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


void testButtonFlashingLight() {
	uint8_t bool = 0;
	uint8_t prev = 0;

	int maxCount = 5000000;

	int j = 0;

	while(1) {

		if(GPIO_ReadInputDataBit(SmallButton.port, SmallButton.pin)) {
			bool = 1;
		}
		else {
			bool = 0;
		}

		if(prev != bool) {
			if(bool) {
				GPIO_SetBits(LowWaterLED.port, LowWaterLED.pin);
			}
			else {
				GPIO_ResetBits(LowWaterLED.port, LowWaterLED.pin);
			}

			prev = bool;
		}
	}
}

/**
 * Initialize all peripherals of the Fast Cold Brew Machine
 */
void initializeAll_Peripherals() {
	TM_DELAY_Init();

	initializeAllTempSensors();

	initializeSonarSensor();
}






void setSolenoidsForRecirc() {
	GPIO_ResetBits(ToVacChamberSol.port, ToVacChamberSol.pin);
	GPIO_SetBits(RecircSol.port, RecircSol.pin);
}

void setSolenoidsForPumping() {
	GPIO_SetBits(ToVacChamberSol.port, ToVacChamberSol.pin);
	GPIO_ResetBits(RecircSol.port, RecircSol.pin);
}



