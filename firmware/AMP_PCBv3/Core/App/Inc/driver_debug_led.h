/*
 * driver_Debug_led.h
 *
 *  Created on: Apr 19, 2024
 *      Author: benjaminowen
 */

#ifndef APP_INC_DRIVER_DEBUG_LED_H_
#define APP_INC_DRIVER_DEBUG_LED_H_

typedef struct Stat_LED_T
{
	GPIO_TypeDef* port;
	uint16_t pin;
} Stat_LED_T;

extern Stat_LED_T STAT1;
extern Stat_LED_T STAT2;
extern Stat_LED_T STAT3;
extern Stat_LED_T STAT4;

void Driver_Debug_LED_On(Stat_LED_T);
void Driver_Debug_LED_Off(Stat_LED_T);
void Driver_Debug_LED_SetHex(uint8_t);

#endif /* APP_INC_DRIVER_DEBUG_LED_H_ */
