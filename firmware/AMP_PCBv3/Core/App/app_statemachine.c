/*
 * app_statemachine.c
 *
 *  Created on: Apr 19, 2024
 *      Author: benjaminowen
 */

#include "string.h"

#include "usart.h"
#include "spi.h"

#include "LoRa.h"

#include "app_statemachine.h"
#include "driver_fan.h"
#include "driver_ebrake.h"
#include "driver_status_led.h"
#include "driver_steering.h"

#define TICKS_PER_SEC		100
#define UART2_MSG_LENGTH	80

State_T current_state;				// variable holding current kart state
uint32_t ticks_in_state;			// variable holding number of ticks in current state
char uart2_msg[UART2_MSG_LENGTH];	// buffer for messages to send over UART2

uint8_t steering;

// function to be called before tick() function
void App_StateMachine_Init()
{
	// variable default values
	steering = 128;
	// UART "hello" message
	strcpy(uart2_msg, "\e[2J\e[HAMP Kart UART Interface\r\n=======================\r\n");
	HAL_UART_Transmit_DMA(&huart2, (unsigned char *) uart2_msg, strlen(uart2_msg));
	// initialize LoRa
	LoRa lora;
	lora = newLoRa();

	lora.CS_port = LORA_NSS_GPIO_Port;
	lora.CS_pin = LORA_NSS_Pin;
	lora.reset_port = LORA_RST_GPIO_Port;
	lora.reset_pin = LORA_RST_Pin;
	lora.DIO0_port = LORA_DIO0_GPIO_Port;
	lora.DIO0_pin = LORA_DIO0_Pin;
	lora.hSPIx = &hspi2;

	lora.frequency = 915;

	LoRa_reset(&lora);
	uint16_t lora_status = LoRa_init(&lora);
	if (lora_status == LORA_OK)
	{
		strcpy(uart2_msg, "LoRA OK\r\n");
		HAL_UART_Transmit_DMA(&huart2, (unsigned char *) uart2_msg, strlen(uart2_msg));
	}
	else
	{
		strcpy(uart2_msg, "LoRA FAILED\r\n");
		HAL_UART_Transmit_DMA(&huart2, (unsigned char *) uart2_msg, strlen(uart2_msg));
	}
	// turn all fans off (should be default state anyway)
	Driver_Fan_All_Off();
	// initialize e-brake
	Driver_EBrake_Init();
	// initialize steering
	Driver_Steering_Init(steering);
	// set current state to idle
	current_state = STATE_IDLE;
}

// function to be called on timer interrupt
void App_StateMachine_Tick()
{
	// statements to be called regardless of state
	ticks_in_state += 1;
	// run state-specific code
	switch (current_state)
	{
		case (STATE_IDLE):
		{
			Driver_Status_LED_SetHex(0x1);
			if (ticks_in_state > (5 * TICKS_PER_SEC))
			{
				App_StateMachine_ChangeState(STATE_ERROR);
			}
			break;
		}

		case (STATE_AUTO):
		{
			Driver_Status_LED_SetHex(0x2);
			break;
		}

		case (STATE_RC):
		{
			Driver_Status_LED_SetHex(0x3);
			break;
		}

		case (STATE_EBRAKE):
		{
			Driver_Status_LED_SetHex(0x4);
			App_StateMachine_ChangeState(STATE_ERROR);
			break;
		}

		case (STATE_ERROR):
		{
			Driver_Status_LED_SetHex(0x8);
			break;
		}
	}
}

void App_StateMachine_ChangeState(State_T new_state)
{
	// statements to be called regardless of state transition
	ticks_in_state = 0;
	// change state
	current_state = new_state;
}
