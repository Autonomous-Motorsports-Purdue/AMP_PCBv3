/*
 * driver_Debug_led.c
 *
 *  Created on: Apr 19, 2024
 *      Author: benjaminowen
 */

#include <driver_debug_led.h>
#include "main.h"

// constant LED struct values
Stat_LED_T STAT1 = { .port = STAT1_GPIO_Port, .pin = STAT1_Pin };
Stat_LED_T STAT2 = { .port = STAT2_GPIO_Port, .pin = STAT2_Pin };
Stat_LED_T STAT3 = { .port = STAT3_GPIO_Port, .pin = STAT3_Pin };
Stat_LED_T STAT4 = { .port = STAT4_GPIO_Port, .pin = STAT4_Pin };

// function to turn a Debug LED on
void Driver_Debug_LED_On(Stat_LED_T led)
{
	HAL_GPIO_WritePin(led.port, led.pin, 1);
}

// function to turn a Debug LED off
void Driver_Debug_LED_Off(Stat_LED_T led)
{
	HAL_GPIO_WritePin(led.port, led.pin, 0);
}

// function to convert a 4-bit hex value into LED Debug function calls
void Driver_Debug_LED_SetHex(uint8_t value)
{
	if (value & 0x1)
	{
		Driver_Debug_LED_On(STAT1);
	}
	else
	{
		Driver_Debug_LED_Off(STAT1);
	}

	if (value & 0x2)
	{
		Driver_Debug_LED_On(STAT2);
	}
	else
	{
		Driver_Debug_LED_Off(STAT2);
	}

	if (value & 0x4)
	{
		Driver_Debug_LED_On(STAT3);
	}
	else
	{
		Driver_Debug_LED_Off(STAT3);
	}

	if (value & 0x8)
	{
		Driver_Debug_LED_On(STAT4);
	}
	else
	{
		Driver_Debug_LED_Off(STAT4);
	}
}
