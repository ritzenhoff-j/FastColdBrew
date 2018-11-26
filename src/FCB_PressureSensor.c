/*
 * FCB_PressureSensor.c
 *
 *  Created on: Nov 26, 2018
 *      Author: ritze
 */

#ifndef STM_STD_LIB
#define STM_STD_LIB "stm32f4xx.h"
#include STM_STD_LIB
#endif

#ifndef FCB_PRESSURE_SENSOR
#define FCB_PRESSURE_SENSOR "FCB_PressureSensor.h"
#include FCB_PRESSURE_SENSOR
#endif


uint8_t initializePressureSensor() {
	return 1;
}

/**
 * Returns the pressure read by the sensor in PSI.
 */
float readPressure() {
	return 0.0;
}

/**
 * Returns 1 if pressure is below the desired value.
 *
 * NOTE: Minimum pressure is defined in Header File.
 */
uint8_t isPressureBelowMax() {
	return readPressure() < MaxAllowedPressure;
}


/**
 * Delays the code until the pressure drops below the accepted maximum.
 *
 * Returns 0 if the code timed out.
 */
uint8_t delayUntilPressureBelowMax() {
	return 1;
}


/**
 * Delays the code until the pressure rises above the default room.
 */
uint8_t delayUntilPressureAtRoom() {
	return 1;
}
