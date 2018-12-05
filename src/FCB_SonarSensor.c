/*
  * FCB_SonarSensor.c
 *
 *  Created on: Nov 14, 2018
 *      Author: ritze
 */

#ifndef FCB_SONAR_SENSER
#define FCB_SONAR_SENSER "FCB_SonarSensor.h"
#include FCB_SONAR_SENSER
#endif

#ifndef FCB_GPIO_PINS
#define FCB_GPIO_PINS "FCB_GPIO_Pins.h"
#include FCB_GPIO_PINS
#endif

#ifndef TM_HCRSR04
#define TM_HCRSR04 "tm_stm32f4_hcsr04.h"
#include TM_HCRSR04
#endif

TM_HCSR04_t sonarSensor;

/**
 * Initializes the Water Height Sonar Sensor.
 *
 * Returns 0 if there was an error while initializing.
 */
uint8_t initializeSonarSensor() {
	return TM_HCSR04_Init(&sonarSensor, SonarSensorEcho.port, SonarSensorEcho.pin,
			SonarSensorTrigger.port, SonarSensorTrigger.pin);
}


/**
 * Returns the distance from the water sensor.
 *
 * Returns 0 if there was an error while initializing.
 */
float readDistance() {
	return TM_HCSR04_Read(&sonarSensor);
}


/**
 * Returns whether the water height is below it's minimum value.
 *
 * Returns 1 if true.
 */
uint8_t isWaterHeightBelowMin() {
	return readDistance() > MinimumWaterHeight;
}

/**
 * Delays the code until the water height has changed by the appropriate amount.
 *
 * Returns 1 if no timeout occurred.
 * Returns 0 if there was a timeout.
 */
uint8_t delayUntilChangeInOunces(uint8_t liquidAmount) {
	// 8oz = 23.8mm
	// 12oz = 35.7mm
	// 16oz = 47.6mm

	// 1oz = 1.976mm (change)
	float dOunce = 1.976 / 9.5;
	float liquidFloat = (float) (liquidAmount * 1.0);

	float currentHeight = readDistance();

	float endHeight = currentHeight + (dOunce * liquidFloat);

	uint16_t counter = 0;

	while(currentHeight < endHeight) {
		currentHeight = readDistance();

		if(counter < 200) {
			counter++;

			Delayms(50);
		}
		else {
			setPWM(PWM_RecircPump.timerIndex, PWM_RecircPump.channel, 0);
 			counter = 0;
			setPWM(PWM_RecircPump.timerIndex, PWM_RecircPump.channel, 50);
		}
	}

	setPWM(PWM_RecircPump.timerIndex, PWM_RecircPump.channel, 0);

	return 1;
}
