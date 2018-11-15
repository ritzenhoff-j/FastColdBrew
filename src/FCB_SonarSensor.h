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
 * Initializes all Sonar Sensor used for water height.
 *
 * Returns 0 if there was an error while initializing.
 */


// TM_HCSR04_t sonarSensor;

/**
 * The minimum water height in order for any size coffee to be made.
 *
 * Value is distance from Sonar Sensor. Measured in centimeters.
 */
static const int MinimumWaterHeight = 36;


/**
 * Initializes the Water Height Sonar Sensor.
 *
 * Returns 0 if there was an error while initializing.
 */
uint8_t initializeSonarSensor();


/**
 * Returns whether the water height is below it's minimum value.
 *
 * Returns 1 if true.
 */
uint8_t isWaterHeightBelowMin();
