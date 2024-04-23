/*
 * driver_status_led.h
 *
 *  Created on: Apr 23, 2024
 *      Author: benjaminowen
 */

#ifndef APP_INC_DRIVER_STATUS_LED_H_
#define APP_INC_DRIVER_STATUS_LED_H_

typedef enum
{
	LED_COLOR_RED,
	LED_COLOR_YELLOW,
	LED_COLOR_GREEN,
	LED_COLOR_CYAN,
	LED_COLOR_BLUE,
	LED_COLOR_PURPLE,
	LED_COLOR_WHITE,
	LED_COLOR_BLACK
} LED_Color_T;

typedef enum
{
	LED_STATE_SOLID,
	LED_STATE_BLINKING
} LED_State_T;

void Driver_Status_LED_Init();
void Driver_Status_LED_ChangeColor(LED_Color_T, LED_State_T, char);
void Driver_Status_LED_Blink();

#endif /* APP_INC_DRIVER_STATUS_LED_H_ */
