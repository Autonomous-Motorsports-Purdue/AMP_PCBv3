#include "main.h"
#include "driver_ebrake.h"

// function to initialize the e-brake (retract for a set period of time)
void Driver_EBrake_Init()
{
	// retract e-brake
	Driver_EBrake_Retract();
	// let e-brake retract for some time
	HAL_Delay(2000);
	// extend e-brake
	Driver_EBrake_Extend();
	// let e-brake extend for some time
	HAL_Delay(1000);
	// turn e-brake off
	Driver_EBrake_Off();
}

// function to extend the e-brake servo
void Driver_EBrake_Extend()
{
	HAL_GPIO_WritePin(BRAKE_INA_GPIO_Port, BRAKE_INA_Pin, 1);
	HAL_GPIO_WritePin(BRAKE_INB_GPIO_Port, BRAKE_INB_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_SEL0_GPIO_Port, BRAKE_SEL0_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_PWM_GPIO_Port, BRAKE_PWM_Pin, 1);
}

// function to retract the e-brake servo
void Driver_EBrake_Retract()
{
	HAL_GPIO_WritePin(BRAKE_INA_GPIO_Port, BRAKE_INA_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_INB_GPIO_Port, BRAKE_INB_Pin, 1);
	HAL_GPIO_WritePin(BRAKE_SEL0_GPIO_Port, BRAKE_SEL0_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_PWM_GPIO_Port, BRAKE_PWM_Pin, 1);
}

// function to turn the e-brake servo off (no drive command)
void Driver_EBrake_Off()
{
	HAL_GPIO_WritePin(BRAKE_INA_GPIO_Port, BRAKE_INA_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_INB_GPIO_Port, BRAKE_INB_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_SEL0_GPIO_Port, BRAKE_SEL0_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_PWM_GPIO_Port, BRAKE_PWM_Pin, 0);
}
