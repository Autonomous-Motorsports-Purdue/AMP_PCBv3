#include "main.h"
#include "driver_fan.h"

Fan_T FAN1 = { .port = FAN1_GPIO_Port, .pin = FAN1_Pin };
Fan_T FAN2 = { .port = FAN2_GPIO_Port, .pin = FAN2_Pin };
Fan_T FAN3 = { .port = FAN3_GPIO_Port, .pin = FAN3_Pin };
Fan_T FAN4 = { .port = FAN4_GPIO_Port, .pin = FAN4_Pin };

// function to turn a fan on
void Driver_Fan_On(Fan_T fan)
{
	HAL_GPIO_WritePin(fan.port, fan.pin, 1);
}

// function to turn a fan off
void Driver_Fan_Off(Fan_T fan)
{
	HAL_GPIO_WritePin(fan.port, fan.pin, 0);
}

// function to toggle the state of a fan
void Driver_Fan_Toggle(Fan_T fan)
{
	HAL_GPIO_TogglePin(fan.port, fan.pin);
}

// function to turn all fans on
void Driver_Fan_All_On()
{
	Driver_Fan_On(FAN1);
	Driver_Fan_On(FAN2);
	Driver_Fan_On(FAN3);
	Driver_Fan_On(FAN4);
}

// toggle to turn all fans off
void Driver_Fan_All_Off()
{
	Driver_Fan_Off(FAN1);
	Driver_Fan_Off(FAN2);
	Driver_Fan_Off(FAN3);
	Driver_Fan_Off(FAN4);
}

// function to toggle all fans
void Driver_Fan_All_Toggle()
{
	Driver_Fan_Toggle(FAN1);
	Driver_Fan_Toggle(FAN2);
	Driver_Fan_Toggle(FAN3);
	Driver_Fan_Toggle(FAN4);
}
