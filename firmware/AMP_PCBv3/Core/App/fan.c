#include "fan.h"

Fan FAN1 = { .port = FAN1_GPIO_Port, .pin = FAN1_Pin };
Fan FAN2 = { .port = FAN2_GPIO_Port, .pin = FAN2_Pin };
Fan FAN3 = { .port = FAN3_GPIO_Port, .pin = FAN3_Pin };
Fan FAN4 = { .port = FAN4_GPIO_Port, .pin = FAN4_Pin };


void fan_on(Fan fan)
{
	HAL_GPIO_WritePin(fan.port, fan.pin, 1);
}
void fan_off(Fan fan)
{
	HAL_GPIO_WritePin(fan.port, fan.pin, 0);
}
void fan_toggle(Fan fan)
{
	HAL_GPIO_TogglePin(fan.port, fan.pin);
}
void fan_all_on()
{
	fan_on(FAN1);
	fan_on(FAN2);
	fan_on(FAN3);
	fan_on(FAN4);
}
void fan_all_off()
{
	fan_off(FAN1);
	fan_off(FAN2);
	fan_off(FAN3);
	fan_off(FAN4);
}
void fan_all_toggle()
{
	fan_toggle(FAN1);
	fan_toggle(FAN1);
	fan_toggle(FAN1);
	fan_toggle(FAN1);
}
