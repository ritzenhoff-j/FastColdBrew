/*
 * FCB_Peripheral_Init.h
 *
 *  Created on: Nov 14, 2018
 *      Author: ritze
 */

#ifndef FCB_GPIO_PINS
#define FCB_GPIO_PINS "FCB_GPIO_Pins.h"
#include FCB_GPIO_PINS
#endif

#ifndef TM_ONEWIRE
#define TM_ONEWIRE "tm_stm32f4_onewire.h"
#include TM_ONEWIRE
#endif

// local OneWire

typedef enum {
	PeltierHeatSink,
	WaterTank
} TempSensor;


/**
 * The maximum temperature that the water is allowed to be before the coffee making can commence.
 */
static const int WaterTargetMaxTemp = 7;

/**
 * Maximum temperature that the Peltiers can reach before being shut-off.
 * Is equivalent to 45 C / ~120 F
 *
 */
static const int PeltierMaxTemperature = 45;


uint8_t initializeAllTempSensors();
uint8_t initializeTempSensor(TempSensor sensor);
float readTemperature(TempSensor sensor);
uint8_t isTemperatureBelowMax(TempSensor sensor);

TM_OneWire_t* getTempSensor(TempSensor sensor, uint8_t *allCorrect);
PinLocation getPinLocation(TempSensor sensor, uint8_t *allCorrect);

