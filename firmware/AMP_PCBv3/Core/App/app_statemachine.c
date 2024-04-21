/*
 * app_statemachine.c
 *
 *  Created on: Apr 19, 2024
 *      Author: benjaminowen
 */

#include "stdio.h"
#include "string.h"

#include "usart.h"
#include "spi.h"

#include "LoRa.h"

#include "app_statemachine.h"
#include "driver_fan.h"
#include "driver_ebrake.h"
#include "driver_status_led.h"
#include "driver_uart.h"

#define TICKS_PER_SEC	100

#define RC_THROTTLE_MIN	0
#define RC_THROTTLE_MAX	255
#define RC_STEERING_MIN	0
#define RC_STEERING_MAX	255

#define JETSON_THROTTLE_MIN	128
#define JETSON_THROTTLE_MAX	255
#define JETSON_STEERING_MIN	128
#define JETSON_STEERING_MAX	255

#define JETSON_UART_START_BYTE	254	// DECIMAL VALUE
#define JETSON_UART_END_BYTE	255	// DECIMAL VALUE
#define JETSON_UART_SEQ_LENGTH	2	// NOT INCLUDING START/END BYTES

State_T current_state;		// variable holding current kart state
uint32_t ticks_in_state;	// variable holding number of ticks in current state

uint8_t throttle;			// throttle value
uint8_t steering;			// steering value

// function to be called before tick() function
void App_StateMachine_Init()
{
	// initialize UART driver
	Driver_UART_Init();
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
		Driver_UART_Transmit(NUCLEO, "LoRa OK\r\n");
	}
	else
	{
		Driver_UART_Transmit(NUCLEO, "LoRa FAILED\r\n");
	}
	// turn all fans off (should be default state anyway)
	Driver_Fan_All_Off();
	// initialize e-brake
	Driver_EBrake_Init();
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
				App_StateMachine_ChangeState(STATE_AUTO);
			}
			break;
		}

		case (STATE_AUTO):
		{
			// set status LEDs
			Driver_Status_LED_SetHex(0x2);
			// get current UART sequence
			unsigned char * uart_seq = Driver_UART_GetBuffer(JETSON);
			uint8_t uart_seq_tail = Driver_UART_GetTail(JETSON);
			// make sure enough bytes have arrived and last byte is end byte
			if ((uart_seq_tail >= (JETSON_UART_SEQ_LENGTH + 2)) && (uart_seq[uart_seq_tail - 1] == JETSON_UART_END_BYTE))
			{
				// check if first byte was the start byte
				if (uart_seq[uart_seq_tail - JETSON_UART_SEQ_LENGTH - 2] == JETSON_UART_START_BYTE)
				{
					// first byte after start byte is throttle
					throttle = uart_seq[(uart_seq_tail - JETSON_UART_SEQ_LENGTH - 2) + 1];
					throttle <<= 1; // multiply by 2 (map 128-255 to 0-255)
					// next byte is steering
					steering = uart_seq[(uart_seq_tail - JETSON_UART_SEQ_LENGTH - 2) + 2];
					steering <<= 1; // multiply by 2 (map 128-255 to 0-255)
					// print received values to debug UART interface
					char buf[100];
					sprintf(buf, "Throttle: %d, Steering: %d\r\n", throttle, steering);
					Driver_UART_Transmit(NUCLEO, buf);
					// clear buffer for next data
					Driver_UART_ClearBuffer(JETSON);
				}
				else
				{
					// invalid UART sequence
					Driver_UART_Transmit(NUCLEO, "Incorrect start bit!\r\n");
					// clear buffer for next data
					Driver_UART_ClearBuffer(JETSON);
				}
			}
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
