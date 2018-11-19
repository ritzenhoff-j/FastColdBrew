/*
 * FCB_TempSensor.c
 *
 *  Created on: Nov 14, 2018
 *      Author: ritze
 */

#ifndef FCB_TEMP_SENSOR
#define FCB_TEMP_SENSOR "FCB_TempSensor.h"
#include FCB_TEMP_SENSOR
#endif

#ifndef FCB_GPIO_PINS
#define FCB_GPIO_PINS "FCB_GPIO_Pins.h"
#include FCB_GPIO_PINS
#endif

#ifndef TM_ONEWIRE
#define TM_ONEWIRE "tm_stm32f4_onewire.h"
#include TM_ONEWIRE
#endif

#ifndef TM_DS18B20
#define TM_DS18B20 "tm_stm32f4_ds18b20.h"
#include TM_DS18B20
#endif

#ifndef TM_DELAY
#define TM_DELAY "tm_stm32f4_delay.h"
#include TM_DELAY
#endif



TM_OneWire_t waterTankTempSensor;
TM_OneWire_t peltierTempSensor;

uint8_t waterTankROM[8];
uint8_t peltierROM[8];

/**
 * Initializes all Temperature Sensors known in TempSensor Enumeration.
 *
 * Returns 0 if there was an error while initializing.
 */
uint8_t initializeAllTempSensors() {
	return initializeTempSensor(WaterTank); // & initializeTempSensor(PeltierHeatSink);
}

/**
 * Initializes a specific Temperature Sensor given TempSensor Enumeration input.
 *
 * Returns 0 if there was an error while initializing.
 */
uint8_t initializeTempSensor(TempSensor sensor) {
	TM_OneWire_t* tempSensor;
	PinLocation* tempLocation;
	uint8_t* sensorROM;

	uint8_t initialized = 0;

	if(sensor == WaterTank) {
		tempSensor = &waterTankTempSensor;
		tempLocation = &WaterTempSensor;
		sensorROM = &waterTankROM[0];
		initialized = 1;
	}

	if(sensor == PeltierHeatSink) {
		tempSensor = &peltierTempSensor;
		tempLocation = &PeltierTempSensor;
		sensorROM = &peltierROM[0];
		initialized = 1;
	}

	if(!initialized) return 0;

	// Initialize the temperature sensor on the appropriate port
	TM_OneWire_Init(tempSensor, tempLocation->port, tempLocation->pin);

	uint32_t breakoutTime = 50000;

	// Check if there is a Temperature Sensor signal on the port
	while(!TM_OneWire_First(tempSensor)) {
		breakoutTime = breakoutTime - 1;

		if(breakoutTime <= 0) return 0;
	}

	// Write the full Read-Only Memory ID of the sensor to the 8-bit number
	TM_OneWire_GetFullROM(tempSensor, sensorROM);

	// Set the resolution of the ID'ed ROM on the appropriate port
	if(!TM_DS18B20_SetResolution(tempSensor, sensorROM, TM_DS18B20_Resolution_12bits)) return 0;

	return 1;
}


/**
 * Returns the temperature reading of a current sensor in Celcius.
 * Returns 0 if there was an error.
 *
 * NOTE: This function not work with sub-0 degree liquids
 */
float readTemperature(TempSensor sensor) {
	TM_OneWire_t* tempSensor;
	uint8_t* sensorROM;
	float* temp;

	uint8_t initialized = 0;

	if(sensor == WaterTank) {
		tempSensor = &waterTankTempSensor;
		sensorROM = &waterTankROM[0];
		initialized = 1;
	}

	if(sensor == PeltierHeatSink) {
		tempSensor = &peltierTempSensor;
		sensorROM = &peltierROM[0];
		initialized = 1;
	}

	if(!initialized) return 0;

	/* Start temperature conversion on all devices on one bus */
	TM_DS18B20_StartAll(tempSensor);

	uint32_t breakoutTime = 50000;

	/* Wait until all are done on one onewire port */
	while (!TM_DS18B20_AllDone(tempSensor)) {
		breakoutTime = breakoutTime - 1;

		if(breakoutTime <= 0) return 0;
	}

	/* Read temperature from ROM address and store it to temps variable */
	if (!TM_DS18B20_Read(tempSensor, sensorROM, temp)) return 0;

	return (*temp);
}

/**
 * Returns whether a given Temperature sensor is below it's defined maximum temperature value.
 *
 * NOTE: Max temperatures are defined at top of header file.
 */
uint8_t isTemperatureBelowMax(TempSensor sensor) {

	int maxTemperature;
	uint8_t initialized = 0;

	if(sensor == WaterTank) {
		maxTemperature = WaterTargetMaxTemp;
		initialized = 1;
	}

	if(sensor == PeltierHeatSink) {
		maxTemperature = PeltierMaxTemperature;
		initialized = 1;
	}

	if(!initialized) return 0;

	float actualTemperature = readTemperature(sensor);
	return (0 < actualTemperature) & (actualTemperature < maxTemperature);
}
