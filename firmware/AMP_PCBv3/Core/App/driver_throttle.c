/*
 * driver_throttle.c
 *
 *  Created on: Apr 23, 2024
 *      Author: benjaminowen
 */

#include "tim.h"

#include "driver_throttle.h"

// clock at 180 Mhz, PSC at 179 (division by 100), frequency range is 1 Hz to 1 Mhz
// below values are in microseconds
#define THROTTLE_MIN_PERIOD_US	1000
#define THROTTLE_MID_PERIOD_US	1500
#define THROTTLE_MAX_PERIOD_US	2000

#define THROTTLE_MIN_FREQUENCY_ARR	(1000000 / (1000000 / THROTTLE_MIN_PERIOD_US) - 1)
#define THROTTLE_MID_FREQUENCY_ARR	(1000000 / (1000000 / THROTTLE_MID_PERIOD_US) - 1)
#define THROTTLE_MAX_FREQUENCY_ARR	(1000000 / (1000000 / THROTTLE_MAX_PERIOD_US) - 1)

// function to initialize throttle default state
void Driver_Throttle_Init(uint8_t magnitude)
{
	// set initial value (always forward, should always be 0 anyway)
	Driver_Throttle_SetFrequency(magnitude, THROTTLE_DIRECTION_FORWARD);
	// start timer PWM output
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_4);
}

void Driver_Throttle_SetFrequency(uint8_t magnitude, Throttle_Direction_T direction)
{
	// map function:
	// (input - in_min) * (out_max - out_min) / (in_max - in_min) + out_min
	// maps 'input' with range ('in_min' to 'in_max') to a range ('out_min' to 'out_max')

	uint32_t mapped_throttle;
	// map throttle taking direction into account
	if (direction == THROTTLE_DIRECTION_FORWARD)
	{
		mapped_throttle = (magnitude - 0) * (THROTTLE_MAX_FREQUENCY_ARR - THROTTLE_MID_FREQUENCY_ARR) / (255 - 0) + THROTTLE_MID_FREQUENCY_ARR;
	}
	else if (direction == THROTTLE_DIRECTION_REVERSE)
	{
		mapped_throttle = (magnitude - 0) * (THROTTLE_MIN_FREQUENCY_ARR - THROTTLE_MID_FREQUENCY_ARR) / (255 - 0) + THROTTLE_MID_FREQUENCY_ARR;
	}

	// set ARR to calculated value
	__HAL_TIM_SET_AUTORELOAD(&htim8, mapped_throttle);
	__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_4, (mapped_throttle / 2));
	__HAL_TIM_SET_COUNTER(&htim8, 0);
}
