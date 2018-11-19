/*
 * FCB_SonarSensor.h
 *
 *  Created on: Nov 14, 2018
 *      Author: ritze
 */

#ifndef FCB_GPIO_PINS
#define FCB_GPIO_PINS "FCB_GPIO_Pins.h"
#include FCB_GPIO_PINS
#endif


/**
 * The minimum water height in order for any size coffee to be made.
 *
 * Value is distance from Sonar Sensor. Measured in centimeters.
 */
static const int MinimumWaterHeight = 36;


uint8_t initializeSonarSensor();

float readDistance();

uint8_t isWaterHeightBelowMin();
