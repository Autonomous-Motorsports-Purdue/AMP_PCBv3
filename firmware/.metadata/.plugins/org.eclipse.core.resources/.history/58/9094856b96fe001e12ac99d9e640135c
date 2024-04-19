#include "ebrake.h"

void ebrake_forward()
{
	HAL_GPIO_WritePin(BRAKE_INA_GPIO_Port, BRAKE_INA_Pin, 1);
	HAL_GPIO_WritePin(BRAKE_INB_GPIO_Port, BRAKE_INB_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_SEL0_GPIO_Port, BRAKE_SEL0_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_PWM_GPIO_Port, BRAKE_PWM_Pin, 1);
}
void ebrake_reverse()
{
	HAL_GPIO_WritePin(BRAKE_INA_GPIO_Port, BRAKE_INA_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_INB_GPIO_Port, BRAKE_INB_Pin, 1);
	HAL_GPIO_WritePin(BRAKE_SEL0_GPIO_Port, BRAKE_SEL0_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_PWM_GPIO_Port, BRAKE_PWM_Pin, 1);
}
void ebrake_off()
{
	HAL_GPIO_WritePin(BRAKE_INA_GPIO_Port, BRAKE_INA_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_INB_GPIO_Port, BRAKE_INB_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_SEL0_GPIO_Port, BRAKE_SEL0_Pin, 0);
	HAL_GPIO_WritePin(BRAKE_PWM_GPIO_Port, BRAKE_PWM_Pin, 0);
}
