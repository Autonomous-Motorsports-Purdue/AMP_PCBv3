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
#include "driver_debug_led.h"
#include "driver_ebrake.h"
#include "driver_fan.h"
#include "driver_status_led.h"
#include "driver_steering.h"
#include "driver_throttle.h"
#include "driver_uart.h"

#define TICKS_PER_SEC	100

#define AUTO_BYPASS		1	// set to '1' to go from IDLE to AUTO without remote control connected

#define RC_THROTTLE_MIN	0
#define RC_THROTTLE_MAX	255
#define RC_STEERING_MIN	0
#define RC_STEERING_MAX	255

#define JETSON_THROTTLE_MIN	128
#define JETSON_THROTTLE_MAX	255
#define JETSON_STEERING_MIN	128
#define JETSON_STEERING_MAX	255

#define JETSON_UART_START_BYTE	0xFE	// HEX VALUE
#define JETSON_UART_END_BYTE	0xFF	// HEX VALUE
#define JETSON_UART_SEQ_LENGTH	2		// NOT INCLUDING START/END BYTES

State_T current_state;		// variable holding current kart state
uint32_t ticks_in_state;	// variable holding number of ticks in current state
uint32_t temp_data;			// to hold miscellaneous data within a state

char autonomous;					// autonomous button boolean
char ebrake;						// e-brake button boolean
uint8_t throttle;					// throttle value
Throttle_Direction_T throttle_dir;	// throttle direction
uint8_t steering;					// steering value

// function to be called before tick() function
void App_StateMachine_Init()
{
	// default variable values
	ticks_in_state = 0;
	temp_data = 0;
	autonomous = 0;
	ebrake = 0;
	throttle = 0;
	throttle_dir = THROTTLE_DIRECTION_FORWARD;
	steering = 128;
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
	uint16_t lora_debug = LoRa_init(&lora);
	if (lora_debug == LORA_OK)
	{
		Driver_UART_Transmit(NUCLEO, "LoRa OK\r\n\r\n");
	}
	else
	{
		Driver_UART_Transmit(NUCLEO, "LoRa FAILED\r\n\r\n");
	}
	// turn all fans off (should be default state anyway)
	Driver_Fan_All_Off();
	// initialize e-brake
	Driver_EBrake_Init();
	// initialize steering
	Driver_Steering_Init(steering);
	// initialize status LED
	Driver_Status_LED_Init();
	// initialize throttle
	Driver_Throttle_Init(throttle, throttle_dir);
	// set current state to idle
	App_StateMachine_ChangeState(STATE_IDLE);
}

// function to be called on timer interrupt
void App_StateMachine_Tick()
{
	// statements to be called regardless of state
	ticks_in_state += 1;
	Driver_Steering_SetDutyCycle(steering);
	Driver_Throttle_SetFrequency(throttle, throttle_dir);
	// run state-specific code
	switch (current_state)
	{
		case (STATE_IDLE):
		{
			// set debug LEDs
			Driver_Debug_LED_SetHex(0x1);
			// print prompt once
			if (ticks_in_state == 10)
			{
				Driver_UART_Transmit(NUCLEO, "Press ENTER key with 5 seconds to enter console control mode.\r\n");
			}

			// get current UART sequence
			unsigned char * uart_seq = Driver_UART_GetBuffer(NUCLEO);
			uint8_t uart_seq_tail = Driver_UART_GetTail(NUCLEO);
			// check if ENTER pressed
			if ((uart_seq_tail > 0) && (uart_seq[uart_seq_tail - 1] == 0x0D))
			{
				App_StateMachine_ChangeState(STATE_CONSOLE);
			}

			// if no user input after 5 seconds, continue to correct state
			if (ticks_in_state > (5 * TICKS_PER_SEC))
			{
				if (AUTO_BYPASS || autonomous)
				{
					// autonomous control
					App_StateMachine_ChangeState(STATE_AUTO);
				}
				else
				{
					// RC control
					App_StateMachine_ChangeState(STATE_RC);
				}
			}
			break;
		}

		case (STATE_CONSOLE):
		{
			// set debug LEDs
			Driver_Debug_LED_SetHex(0x2);
			// print prompt once
			if (ticks_in_state == 10)
			{
				Driver_UART_Transmit(NUCLEO, "Enter pairs of [(-)THROTTLE,STEERING] values. ESC to return to IDLE.\r\n");
			}
			// get current UART sequence
			unsigned char * uart_seq = Driver_UART_GetBuffer(NUCLEO);
			uint8_t uart_seq_tail = Driver_UART_GetTail(NUCLEO);

			// check if ESC pressed
			if ((uart_seq_tail > 0) && (uart_seq[uart_seq_tail - 1] == 0x1B))
			{
				// return to IDLE state
				App_StateMachine_ChangeState(STATE_IDLE);
			}

			// check if ENTER pressed
			if (uart_seq[uart_seq_tail - 1] == 0x0D)
			{
				// check for correct length of sequence
				if ((uart_seq_tail >= 10) || (uart_seq_tail < 3))
				{
					// invalid length
					Driver_UART_Transmit(NUCLEO, "Invalid values entered!\r\n");
					// mark failure to avoid printing outside loop
					temp_data = 2;
				}

				uint16_t throttle_temp = 0;
				Throttle_Direction_T throttle_dir_temp = THROTTLE_DIRECTION_FORWARD;
				uint16_t steering_temp = 0;
				// loop through buffer backwards and try to recreate values in decimal
				for (uint8_t char_num = 0; char_num < uart_seq_tail - 1; char_num++)
				{
					char curr_char = uart_seq[char_num];
					if (((curr_char < '0') || (curr_char > '9')) && (curr_char != ',') && (curr_char != '-'))
					{
						// not a number or comma
						Driver_UART_Transmit(NUCLEO, "Invalid values entered!\r\n");
						// mark failure to avoid printing outside loop
						temp_data = 2;
						break;
					}
					// valid character
					if (curr_char == '-')
					{
						// throttle direction will be negative
						throttle_dir_temp = THROTTLE_DIRECTION_REVERSE;
					}
					else if (curr_char == ',')
					{
						// prepare for next number
						throttle_temp /= 10;
						temp_data = 1;
					}
					else if (temp_data == 0)
					{
						// need to populate throttle
						throttle_temp += curr_char - '0';
						throttle_temp *= 10;
					}
					else if (temp_data == 1)
					{
						// need to populate steering
						steering_temp += curr_char - '0';
						steering_temp *= 10;
					}
				}
				steering_temp /= 10;
				// check for overflow
				if ((throttle_temp > 255) || (steering_temp > 255))
				{
					Driver_UART_Transmit(NUCLEO, "Invalid values entered!\r\n");
					// mark failure to avoid printing outside loop
					temp_data = 2;
				}

				// only run the following if input was valid
				if (temp_data == 1)
				{
					throttle = throttle_temp;
					throttle_dir = throttle_dir_temp;
					steering = steering_temp;
					// print updated values to console
					char buf[80];
					if (throttle_dir == THROTTLE_DIRECTION_FORWARD)
					{
						sprintf(buf, "Throttle: %d, Steering: %d\r\n", throttle, steering);
					}
					else
					{
						sprintf(buf, "Throttle: -%d, Steering: %d\r\n", throttle, steering);
					}
					Driver_UART_Transmit(NUCLEO, buf);
				}

				// reset for next iteration
				Driver_UART_ClearBuffer(NUCLEO);
				temp_data = 0;
			}
			break;
		}

		case (STATE_AUTO):
		{
			// set debug LEDs
			Driver_Debug_LED_SetHex(0x3);
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
					throttle_dir = THROTTLE_DIRECTION_FORWARD; // always forward in autonomous
					// next byte is steering
					steering = uart_seq[(uart_seq_tail - JETSON_UART_SEQ_LENGTH - 2) + 2];
					steering <<= 1; // multiply by 2 (map 128-255 to 0-255)
					// print received values to debug UART interface
					char buf[80];
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
			Driver_Debug_LED_SetHex(0x4);
			break;
		}

		case (STATE_EBRAKE):
		{
			Driver_Debug_LED_SetHex(0x5);
			App_StateMachine_ChangeState(STATE_ERROR);
			break;
		}

		case (STATE_ERROR):
		{
			Driver_Debug_LED_SetHex(0x8);
			break;
		}
	}
}

void App_StateMachine_ChangeState(State_T new_state)
{
	// statements to be called regardless of state transition
	ticks_in_state = 0;
	temp_data = 0;
	Driver_UART_ClearBuffer(NUCLEO);
	Driver_UART_ClearBuffer(JETSON);
	// indicate changing state
	switch (new_state)
	{
		case (STATE_IDLE):
		{
			Driver_UART_Transmit(NUCLEO, "Entering IDLE state\r\n\r\n");
			break;
		}

		case (STATE_CONSOLE):
		{
			Driver_UART_Transmit(NUCLEO, "Entering CONSOLE state\r\n\r\n");
			break;
		}

		case (STATE_AUTO):
		{
			Driver_UART_Transmit(NUCLEO, "Entering AUTO state\r\n\r\n");
			break;
		}

		case (STATE_RC):
		{
			Driver_UART_Transmit(NUCLEO, "Entering RC state\r\n\r\n");
			break;
		}

		case (STATE_EBRAKE):
		{
			Driver_UART_Transmit(NUCLEO, "Entering EBRAKE state\r\n\r\n");
			break;
		}

		case (STATE_ERROR):
		{
			Driver_UART_Transmit(NUCLEO, "Entering ERROR state\r\n\r\n");
			break;
		}

		default:
		{
			Driver_UART_Transmit(NUCLEO, "Invalid state!\r\n\r\n");
			break;
		}
	}
	// change state
	current_state = new_state;
}
