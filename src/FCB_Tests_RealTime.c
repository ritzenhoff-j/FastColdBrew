/*
 * FCB_Tests_RealTime.c
 *
 *  Created on: Nov 26, 2018
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

void testPortValues() {
	GPIO_TypeDef* aDef = GPIOA;
	uint32_t aInt = GPIOA;

	uint8_t aEqual = 0;

	GPIO_TypeDef* aFrom = VacuumPumpPin.port;
	uint32_t aIntFrom = VacuumPumpPin.port;

	PinLocation locA = *outputPins[0];

	GPIO_TypeDef* aFromArray = locA.port;
	uint32_t aIntFromArray = locA.port;

}

void testFlashingLight() {
	uint8_t bool = 0;

	while(1) {

		GPIO_ToggleBits(LowWaterLED.port, LowWaterLED.pin);

		int j = 0;

		for(int i = 0; i < 5000000; i++) {
			j++;
		}
	}
}

void testPWM() {
	// run PWM test
	int l = 0;
	int brightness = 0;
	int increment = 5;

	setPWM(PWM_RecircPump.timerIndex, PWM_RecircPump.channel, 50);

	while (1) {
		for (int i = 0; i < 20; i++) {

			for(int j = 0; j < 50000; j++) { l = j; }

			setPWM(PWM_PeltierCoolingFan.timerIndex, PWM_PeltierCoolingFan.channel, brightness);

    			brightness += increment;
		}

		increment = -increment;
	}

}


void testButtonFlashingLight() {
	uint8_t small = 0;
	uint8_t prevSmall = 0;

	uint8_t medium = 0;
	uint8_t prevMed = 0;

	int maxCount = 5000000;

	int j = 0;

	while(1) {

		if(GPIO_ReadInputDataBit(SmallButton.port, SmallButton.pin)) {
			small = 1;
		}
		else {
			small = 0;
		}

		if(GPIO_ReadInputDataBit(MediumButton.port, MediumButton.pin)) {
			medium = 1;
		}
		else {
			medium = 0;
		}

		if(prevSmall != small) {
			if(small) {
				GPIO_SetBits(LowWaterLED.port, LowWaterLED.pin);
			}
			else {
				GPIO_ResetBits(LowWaterLED.port, LowWaterLED.pin);
			}

			prevSmall = small;
		}

		if(prevMed != medium) {
			if(medium) {
				GPIO_SetBits(BrewMin1LED.port, BrewMin1LED.pin);
			}
			else {
				GPIO_ResetBits(BrewMin1LED.port, BrewMin1LED.pin);
			}

			prevMed = medium;
		}
	}
}

void testTempAndSonar() {
	float temperature = 0;
	float distance = 0;

	while(1) {
		temperature = readTemperature(WaterTank);
		distance = readDistance();
	}
}

void testADC() {

	float pressure = 0;

	while(1) {
		pressure = readPressure();
	}
}


void testAllLEDs() {

	uint8_t counter = 0;

	// Turn off all size LEDs
//	GPIO_SetBits(LowWaterLED.port, LowWaterLED.pin)
//	GPIO_SetBits(SmallButtonLED.port, SmallButtonLED.pin | MediumButtonLED.pin | LargeButtonLED.pin);
//	GPIO_SetBits(BrewMin1LED.port, BrewMin1LED.pin | BrewMin2LED.pin | BrewMin3LED.pin);


	// blink Low Water (for 5 seconds)
	while(1) {
		GPIO_SetBits(LowWaterLED.port, LowWaterLED.pin);

		Delayms(1000);

		GPIO_ResetBits(LowWaterLED.port, LowWaterLED.pin);

		Delayms(1000);

		counter++;

		if(counter > 3) break;
	}

	counter = 0;


	// start full procedure
	while(1) {
		// Turn off all size LEDs
		GPIO_ResetBits(SmallButtonLED.port, SmallButtonLED.pin | MediumButtonLED.pin | LargeButtonLED.pin);

		// Turn off all counter LEDs
		GPIO_ResetBits(BrewMin1LED.port, BrewMin1LED.pin | BrewMin2LED.pin | BrewMin3LED.pin);

		// start cooling ON (5 seconds)
		GPIO_SetBits(CoolingOnLED.port, CoolingOnLED.pin);
		Delayms(4000);

		// start cooling OFF
		GPIO_ResetBits(CoolingOnLED.port, CoolingOnLED.pin);

		// wait for button input
		uint8_t size = getSizeInput();

		// turn on appropriate light
		switch(size) {
		case 8:
			GPIO_SetBits(SmallButtonLED.port, SmallButtonLED.pin);
			break;
		case 10:
			GPIO_SetBits(MediumButtonLED.port, MediumButtonLED.pin);
			break;
		case 12:
			GPIO_SetBits(LargeButtonLED.port, LargeButtonLED.pin);
			break;
		}

		// start pressure ON (5 seconds)
		GPIO_SetBits(PressureOnLED.port, PressureOnLED.pin);

		// delay 4 seconds
		Delayms(4000);
		GPIO_ResetBits(PressureOnLED.port, PressureOnLED.pin);

		// start timing ON (10 seconds per)
		Delayms(500);

		GPIO_SetBits(BrewMin1LED.port, BrewMin1LED.pin);
		Delayms(10000);

		GPIO_SetBits(BrewMin2LED.port, BrewMin2LED.pin);
		Delayms(10000);

		GPIO_SetBits(BrewMin3LED.port, BrewMin3LED.pin);
		Delayms(10000);
	}
}

