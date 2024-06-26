/*
 * driver_steering.c
 *
 *  Created on: Apr 21, 2024
 *      Author: Matthew Briggs
 */

#include "main.h"
#include "gpio.h"
#include "tim.h"

#include "driver_steering.h"

#define STEERING_MIN_DUTY_CYCLE	5
#define STEERING_MAX_DUTY_CYCLE	250

/*
 * Steering Servo Input Descriptions
 *
 * Enable:
 * Used as output
 *
 * Input A+:
 * If used as output, asserting will cause servo to ignore changes to B+
 * If used as input, it is used as home position sensor
 *
 * Input B+:
 * Used as output (PWM/frequency command for position)
 */

// function to initialize default timer and pin states
void Driver_Steering_Init(uint8_t duty_cycle)
{
	// set direction pins for level shifters
	HAL_GPIO_WritePin(STR_INPUTA__DIR_GPIO_Port, STR_INPUTA__DIR_Pin, 1); // input A+ as output
	HAL_GPIO_WritePin(STR_INPUTB__DIR_GPIO_Port, STR_INPUTB__DIR_Pin, 1); // input B+ as output
	// set hard-coded outputs
	HAL_GPIO_WritePin(STR_EN__GPIO_Port, STR_EN__Pin, 1); // enable pin high
	HAL_GPIO_WritePin(STR_INPUTA__GPIO_Port, STR_INPUTA__Pin, 0); // input A+ low
	// set default PWM values
	Driver_Steering_SetDutyCycle(duty_cycle);
	// enable PWM output channel
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
}

// function to change duty cycle of steering signal
void Driver_Steering_SetDutyCycle(uint8_t duty_cycle)
{
	// Input validation for duty cycle
	// Need to clamp from 5 to 250 to avoid no edges (steering servo doesn't like that)
	if (duty_cycle > STEERING_MAX_DUTY_CYCLE)
	{
		duty_cycle = STEERING_MAX_DUTY_CYCLE;
	}
	else if (duty_cycle < STEERING_MIN_DUTY_CYCLE)
	{
		duty_cycle = STEERING_MIN_DUTY_CYCLE;
	}

	// Set compare (8-Bit Resolution)
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, duty_cycle);
}
