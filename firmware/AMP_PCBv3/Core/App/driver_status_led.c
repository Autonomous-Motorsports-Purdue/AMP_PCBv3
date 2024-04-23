/*
 * driver_status_led.c
 *
 *  Created on: Apr 23, 2024
 *      Author: benjaminowen
 */

#include "string.h"

#include "tim.h"

#include "driver_status_led.h"

#define NUM_STATUS_LED	2

// data array
// each CCR entry is 16 bits
// each LED is represented by 24 uint16_t entries
// first 2 LEDs will act as a reset signal, so we add two to total array length
uint16_t Status_LED_Data[(NUM_STATUS_LED + 2) * 24];

// pre-computed color values
// order is G7-G0, R7-R0, B7-B0
uint16_t STATUS_LED_RED[] =		{ 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1 };
uint16_t STATUS_LED_YELLOW[] =	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1 };
uint16_t STATUS_LED_GREEN[] =	{ 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
uint16_t STATUS_LED_CYAN[] =	{ 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2 };
uint16_t STATUS_LED_BLUE[] =	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2 };
uint16_t STATUS_LED_PURPLE[] =	{ 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2 };
uint16_t STATUS_LED_WHITE[] =	{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
uint16_t STATUS_LED_BLACK[] =	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

// local state tracker
LED_Color_T status_led_color;
LED_State_T status_led_state;
char mid_blink;

// function to initialize default states
void Driver_Status_LED_Init()
{
	// default variable values
	status_led_color = LED_COLOR_BLACK;
	status_led_state = LED_STATE_SOLID;
	mid_blink = 0;
	// initialize data array
	memset(Status_LED_Data, 0, sizeof(Status_LED_Data));
	// start PWM on LED signal timer channel pin
	HAL_TIMEx_PWMN_Start_DMA(&htim1, TIM_CHANNEL_3, (uint32_t *) Status_LED_Data, (NUM_STATUS_LED + 2) * 24);
	// enable timer interrupt for status LED blinking
	HAL_TIM_Base_Start_IT(&htim7);
}

// function to set the color of the status light
// can also set the status light to be a solid color or blinking
// save parameter should always be set to '1' when called from state machine
void Driver_Status_LED_ChangeColor(LED_Color_T color, LED_State_T state, char save)
{
	// set state
	if (save)
	{
		status_led_state = state;
		status_led_color = color;
	}

	switch (color)
	{
		case (LED_COLOR_RED):
		{
			// assign correct color to data array
			for (uint8_t curr_led = 0; curr_led < NUM_STATUS_LED; curr_led++)
			{
				memcpy((Status_LED_Data + ((curr_led + 2) * 24)), STATUS_LED_RED, 24 * sizeof(uint16_t));
			}
			break;
		}

		case (LED_COLOR_YELLOW):
		{
			// assign correct color to data array
			for (uint8_t curr_led = 0; curr_led < NUM_STATUS_LED; curr_led++)
			{
				memcpy((Status_LED_Data + ((curr_led + 2) * 24)), STATUS_LED_YELLOW, 24 * sizeof(uint16_t));
			}
			break;
		}

		case (LED_COLOR_GREEN):
		{
			// assign correct color to data array
			for (uint8_t curr_led = 0; curr_led < NUM_STATUS_LED; curr_led++)
			{
				memcpy((Status_LED_Data + ((curr_led + 2) * 24)), STATUS_LED_GREEN, 24 * sizeof(uint16_t));
			}
			break;
		}

		case (LED_COLOR_CYAN):
		{
			// assign correct color to data array
			for (uint8_t curr_led = 0; curr_led < NUM_STATUS_LED; curr_led++)
			{
				memcpy((Status_LED_Data + ((curr_led + 2) * 24)), STATUS_LED_CYAN, 24 * sizeof(uint16_t));
			}
			break;
		}

		case (LED_COLOR_BLUE):
		{
			// assign correct color to data array
			for (uint8_t curr_led = 0; curr_led < NUM_STATUS_LED; curr_led++)
			{
				memcpy((Status_LED_Data + ((curr_led + 2) * 24)), STATUS_LED_BLUE, 24 * sizeof(uint16_t));
			}
			break;
		}

		case (LED_COLOR_PURPLE):
		{
			// assign correct color to data array
			for (uint8_t curr_led = 0; curr_led < NUM_STATUS_LED; curr_led++)
			{
				memcpy((Status_LED_Data + ((curr_led + 2) * 24)), STATUS_LED_PURPLE, 24 * sizeof(uint16_t));
			}
			break;
		}

		case (LED_COLOR_WHITE):
		{
			// assign correct color to data array
			for (uint8_t curr_led = 0; curr_led < NUM_STATUS_LED; curr_led++)
			{
				memcpy((Status_LED_Data + ((curr_led + 2) * 24)), STATUS_LED_WHITE, 24 * sizeof(uint16_t));
			}
			break;
		}

		case (LED_COLOR_BLACK):
		{
			// assign correct color to data array
			for (uint8_t curr_led = 0; curr_led < NUM_STATUS_LED; curr_led++)
			{
				memcpy((Status_LED_Data + ((curr_led + 2) * 24)), STATUS_LED_BLACK, 24 * sizeof(uint16_t));
			}
			break;
		}
	}
}

// function to blink LEDs (if set)
// should be called twice per second from timer interrupt
void Driver_Status_LED_Blink()
{
	if (status_led_state == LED_STATE_BLINKING)
	{
		if (mid_blink)
		{
			// need to revert to color
			Driver_Status_LED_ChangeColor(status_led_color, status_led_state, 0);
		}
		else
		{
			// need to blink
			Driver_Status_LED_ChangeColor(LED_COLOR_BLACK, status_led_state, 0);
		}
		// invert blink state
		mid_blink = !mid_blink;
	}
}
