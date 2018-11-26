/*
 * FCB_PressureSensor.h
 *
 *  Created on: Nov 26, 2018
 *      Author: ritze
 */

/**
 * The minimum water height in order for any size coffee to be made.
 *
 * Value is distance from Sonar Sensor. Measured in centimeters.
 */
static const float MaxAllowedPressure = -16;

uint8_t initializePressureSensor();

float readPressure();

uint8_t isPressureBelowMax();

uint8_t delayUntilPressureBelowMax();

uint8_t delayUntilPressureAtRoom();
