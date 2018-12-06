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

#ifndef TM_ADC
#define TM_ADC "tm_stm32f4_adc.h"
#include TM_ADC
#endif

#ifndef FCB_GPIO_PINS
#define FCB_GPIO_PINS "FCB_GPIO_Pins.h"
#include FCB_GPIO_PINS
#endif

#ifndef TM_DELAY
#define TM_DELAY "tm_stm32f4_delay.h"
#include TM_DELAY
#endif

// Defined in data sheet of pressure sensor
static const float conversionDivisorConst = 0.007652;
static const float conversionSubstractionConst = 0.92;
static const float kpaInAPsi = 6.894757;

static const float vSource = 5;

/**
 * Returns the pressure read by the sensor in PSI.
 */
float readPressure() {
	// VOUT = VS x (0.007652 x P + 0.92)
	// P = ((VOUT / VS) - 0.92) / 0.007652 ... convert from kPa to PSI

	// This number should be in the range of 0 to 5V
	uint16_t analogInput = TM_ADC_Read(ADC1, ADC_PressureSensor.channel);

	// ADC is 12-bit... Input is scaled from 0 -> { Max = 2^(12) = 4096 }
	float voltage = ((float) analogInput) * ( vSource / 4096.0);

	float finalPressureNumerator = (voltage / vSource) - conversionSubstractionConst;
	float finalPressureDenominator = (conversionDivisorConst * kpaInAPsi);

	float finalPressure = finalPressureNumerator / finalPressureDenominator;

	return finalPressure;
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

	float currentPressure = -0.1;

	while(1) {
		if(isPressureBelowMax()) {
			return 1;
		}

		Delayms(500);
	}

	return 1;
}


/**
 * Delays the code until the pressure rises above the default room.
 */
uint8_t delayUntilPressureAtRoom() {
	while(1) {
		if(readPressure() > -5) {
			return 1;
		}

		Delayms(500);
	}
}
