/*
 * FCB_PWM.h
 *
 *  Created on: Nov 15, 2018
 *      Author: ritze
 */

#ifndef FCB_PWM_H_
#define FCB_PWM_H_
// I feel like i should put everything inside of here...
#endif /* FCB_PWM_H_ */



#ifndef FCB_GPIO_PINS
#define FCB_GPIO_PINS "FCB_GPIO_Pins.h"
#include FCB_GPIO_PINS
#endif

void initializePWM();

uint32_t get_timer_clock_frequency ();

void initializeTimerClock(uint8_t timerIndex);

void initializeTimerPWM(uint8_t timerIndex, uint16_t numberOfPossiblePins, PWM_PinLocation locations[]);

void setPWM(uint8_t timerIndex, uint8_t channel, uint32_t value);

void timer_start(uint8_t timerIndex);

void timer_end(uint8_t timerIndex);


