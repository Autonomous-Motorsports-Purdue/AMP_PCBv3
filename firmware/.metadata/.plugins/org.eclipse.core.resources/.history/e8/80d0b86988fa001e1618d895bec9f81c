#ifndef __FAN_H
#define __FAN_H
#include "main.h"

typedef struct Fan
{
	GPIO_TypeDef* port;
	uint16_t pin;
} Fan;

extern Fan FAN1;
extern Fan FAN2;
extern Fan FAN3;
extern Fan FAN4;

void fan_on(Fan);
void fan_off(Fan);
void fan_toggle(Fan);
void fan_all_on();
void fan_all_off();
void fan_all_toggle();

#endif


