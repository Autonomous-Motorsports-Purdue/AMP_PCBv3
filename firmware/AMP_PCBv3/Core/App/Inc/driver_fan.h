#ifndef __FAN_H
#define __FAN_H

#include "main.h"

typedef struct Fan_T
{
	GPIO_TypeDef* port;
	uint16_t pin;
} Fan_T;

extern Fan_T FAN1;
extern Fan_T FAN2;
extern Fan_T FAN3;
extern Fan_T FAN4;

void Driver_Fan_On(Fan_T);
void Driver_Fan_Off(Fan_T);
void Driver_Fan_All_On();
void Driver_Fan_All_Off();

#endif


